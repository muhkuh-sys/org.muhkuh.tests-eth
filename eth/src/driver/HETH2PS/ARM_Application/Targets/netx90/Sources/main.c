/****************************************************************************************
*  I/O Hardware Abstraction Layer
*
*  This module implements the low level I/O functions, e.g. LED control.
****************************************************************************************/

#include "sys.h"
#include "common_func_netx90.h"
#include "hw_defines.h"
#include "uprintf.h"
#include "phy.h"
#include "targetlib.h"
#include "target_defines.h"
#include <stdlib.h>

#if TARGET_HW == _NXEB90_SPE_
#include "hal_adinphy.h"
#include "sdram_test.h"
#include "eth_2ps.h"

#define EXTPHY_MDIO_ADDR_PORT0 0x0  /* MDIO hardware address of the external PHY on port 0 */
#define EXTPHY_MDIO_ADDR_PORT1 0x1  /* MDIO hardware address of the external PHY on port 1, if connected */

#define LED_ACT_LED_PERIOD 50       /* Period of the activity LED in milliseconds */

#endif /* TARGET_HW == _NXEB90_SPE_ */

/****************************************************************************************
*  Definitions
****************************************************************************************/

#ifndef INGRESS_LATENCY
#define INGRESS_LATENCY (200 + 20) /* default is internal OmniPhy EPHY (latency_red=0) + half clock period due timestamp reference point at rising edge of MII.RxClk */
#endif

#ifndef EGRESS_LATENCY
#define EGRESS_LATENCY  116 /* default is internal OmniPhy PHY */
#endif

/****************************************************************************************
*  Functions
****************************************************************************************/

#if TARGET_HW == _NXEB90_SPE_

extern ETH_T* g_ptEth;

/*
 * Link change update handler for external PHYs
 * \params
 *  void* pvContext       [in]  Pointer to the PHY data structure (SPE_PHY_T) on which the
 *                              link state change occurred.
 * \see adinphy.h
 */
void eth2ps_extphy_link_update_handler(void* pvContext)
{
  /* Pointer to the PHY's data structure that caused the link change */
  SPE_PHY_T* ext_phy = (SPE_PHY_T*)pvContext;
  ETH_T* eth_instance = ext_phy->pt_instance;
  unsigned int link, speed, duplex;

  /* Set PORT_ENABLE of the respective xC channel depending on link state. When using an external
   * PHY with SD-RAM on the board, this parameter suppresses the transmission of frames when the
   * link is down to prevent FIFO overflow on this port. The netX PHYx_LED_LINK_IN input pin
   * cannot be accessed to check the link state due to the SD-RAM occupying these pins. */
  if(phy_get_linkstate(ext_phy->port, &link, &speed, &duplex) == 0)
  {
    if(link == 0) {
      /* Disable xC transmit queue when PHY link is down */
      Eth2PS_SetParameter(ext_phy->port, ETH2PS_PARAM_PORT_ENABLE, 0);
      MLEDCTRL_SetOutput(0, ext_phy->port ? 7 : 5, false);
    }
    else {
      /* Enable xC transmit queue when PHY link is up */
      Eth2PS_SetParameter(ext_phy->port, ETH2PS_PARAM_PORT_ENABLE, 1);
      MLEDCTRL_SetOutput(0, ext_phy->port ? 7 : 5, true);
    }
  }

  /* Indicate a link state change at the corresponding ETH_2PS port */
  eth_2ps_mau_type_change_ind(eth_instance, ext_phy->port);
}

#endif /* TARGET_HW == _NXEB90_SPE_ */

/* Initialize I/O interfaces and peripheral hardware */
int main(void)
{
  /* Initialize netX 90 */
  asic_init();

  /* Initialize board IO */
  io_init();
  io_input_init();
  io_output_init();
  io_serial_init(0);

  /* Initialize COM leds */
  led_com_init(0);
  led_com_init(1);

#if TARGET_HW == _NXEB90_SPE_
  /* Reference external link change callback for the external PHY. The link status has to
   * be updated when an interrupt on pin HIF_DIRQ is asserted. */
  adinphy_register_link_state_callback(eth2ps_extphy_link_update_handler);

  /* Check if SD-RAM was initialized */
  sdram_test();

#if NUM_SPE_PORTS == 2
  /* Initialize external SPE PHYs on both xC channels, no internal PHY */
  adinphy_initialize(0, EXTPHY_MDIO_ADDR_PORT0, &(g_tEth2PS.tEth));
  adinphy_initialize(1, EXTPHY_MDIO_ADDR_PORT1, &(g_tEth2PS.tEth));

  /* Add PHYs to HAL driver */
  phy_add(0, EXTPHY_MDIO_ADDR_PORT0, adinphy_write_cl45, adinphy_read_cl45);
  phy_add(1, EXTPHY_MDIO_ADDR_PORT1, adinphy_write_cl45, adinphy_read_cl45);

  /* Register link state API for both PHYs */
  phy_register_ext_link_api(0, adinphy_get_linkstate);
  phy_register_ext_link_api(1, adinphy_get_linkstate);

  /* Set target-specific configuration for both ports */
  for(uint8_t i = 0; i < 2; i++) {
    g_tSysCfg.atPortCfg[i].fNoPhyLinkInput = true;   /* Disable link check during transmission this xC channel as well
                                                        as the link state change interrupt from xMAC[1:0] to ARM.

                                                        When using no internal PHY and 'sel_extphy' pinning option, this
                                                        interrupt is asserted on every edge of the HIF_DIRQ signal.
                                                        This is because PHY0_LED_LINK_IN is mux'd to HIF_DIRQ in
                                                        this use-case. As both external PHYs use the HIF_DIRQ pin to
                                                        signal an interrupt, a link state change on any SPE port would
                                                        trigger the COM0 interrupt this way.*/

    g_tSysCfg.atPortCfg[i].fDisablePort = true;      /* Disable transmit on this port initially. When a link comes up,
                                                        the link state change handler will enable the port again. */
  }

  /* Setup IRQ handling on HIF_DIRQ pin. Asserted interrupts will be cached by NVIC and
   * processed later after irq_init() is called when the 2ps driver is initialized */
  adinphy_setup_irq();

  /* Disable software power-down and enable auto-negotiation */
  adinphy_release(0);
  adinphy_release(1);

#else
  /* Initialize internal PHY on xC channel 0 only */
  intphy_init_port0_only();

  /* Initialize external SPE PHY on xC channel 1 */
  adinphy_initialize(1, EXTPHY_MDIO_ADDR_PORT1, &(g_tEth2PS.tEth));

  /* Add PHY to HAL driver */
  phy_add(1, EXTPHY_MDIO_ADDR_PORT1, adinphy_write_cl45, adinphy_read_cl45);

  /* Register link state API for external PHYs */
  phy_register_ext_link_api(1, adinphy_get_linkstate);

  /* Configure the port with the external PHY on it */
  g_tSysCfg.atPortCfg[1].fNoPhyLinkInput = true;        /* Disable link check during transmission for xC channel 1 as well
                                                           as the link state change interrupt from xMAC1 to ARM. */
  g_tSysCfg.atPortCfg[1].fDisablePort = true;           /* Disable transmit on this port initially. When a link comes up,
                                                           the link state change handler will enable the port again. */

  /* Setup IRQ handling on HIF_DIRQ pin. Asserted interrupts will be cached by NVIC and
   * processed later after irq_init() is called when the 2ps driver is initialized */
  adinphy_setup_irq();

  /* Disable software power-down and enable auto-negotiation */
  adinphy_release(1);

#endif /* NUM_SPE_PORTS == 2 */

#else
  /* Initialize both internal PHYs */
  intphy_init();
#endif /* TARGET_HW == _NXEB90_SPE_ */

  /* Fill out PHY latencies MDI<->MII */
  g_tSysCfg.uiSystime = 0;
  g_tSysCfg.atPortCfg[0].ulIngressLatency = INGRESS_LATENCY;
  g_tSysCfg.atPortCfg[0].ulEgressLatency = EGRESS_LATENCY;
  g_tSysCfg.atPortCfg[1].ulIngressLatency = INGRESS_LATENCY;
  g_tSysCfg.atPortCfg[1].ulEgressLatency = EGRESS_LATENCY;

  main_task();

  return 0;
}
