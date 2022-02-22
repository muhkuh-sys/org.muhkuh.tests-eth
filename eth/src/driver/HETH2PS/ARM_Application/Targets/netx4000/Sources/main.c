/****************************************************************************************
*  I/O Hardware Abstraction Layer
*
*  This module implements the low level I/O functions, e.g. LED control.
****************************************************************************************/

#include "sys.h"
#include "targetlib.h"
#include <string.h>

/****************************************************************************************
*  Definitions
****************************************************************************************/

/* Select target hardware by defining TARGET_HW */
/* Possible values for TARGET_HW */
#define _NXHX4000_JTAG_PLUS_  0 /* Revision 3 */
#define _NXHX4000_SWITCH_     1
#define _NXHX4100_JTAG_       2
#define _CIFX_PCIE4000_2RE_   3

/* Set default hardware */
#ifndef TARGET_HW
#define TARGET_HW _NXHX4000_JTAG_PLUS_
#endif

#ifndef EXT_MII
#define EXT_MII 0
#endif


#if (TARGET_HW == _CIFX_PCIE4000_2RE_) && (EXT_MII == 1)
#include "hal_phyled_xpic.h"

/* latencies for external PHY TLK105 */
#define INGRESS_LATENCY 1
#define EGRESS_LATENCY  1

#else

/* default is internal REE PHY */
#define INGRESS_LATENCY 213
#define EGRESS_LATENCY  34 /* default is internal REE PHY */

#endif

/****************************************************************************************
*  Functions
****************************************************************************************/

/* Initialize I/O interfaces and peripheral hardware */
int main(void)
{
  asic_init();
  io_init();
  io_input_init();
  io_output_init();
  io_serial_init(0);
  led_com_init(0);
  led_com_init(1);

#if EXT_MII == 0
  g_tSysCfg.uiSystime = 0;

  intphy_init();
#else
  g_tSysCfg.uiSystime = 1;

  extphy_init();

  /* add drivers external PHYs to port 0/1
   * Note: extphy_init() setups the PHY drivers for port 2/3
   */
  phy_add(0, 0, extphy_write, extphy_read);
  phy_add(1, 1, extphy_write, extphy_read);

  /* enable PHY LED blinking for external PHYs */
  PhyledXpic_Initialize(NULL);
  PhyledXpic_Start(NULL);
#endif

  /* fill out PHY latencies MDI<->MII */
  g_tSysCfg.atPortCfg[0].ulIngressLatency = INGRESS_LATENCY;
  g_tSysCfg.atPortCfg[0].ulEgressLatency  = EGRESS_LATENCY;
  g_tSysCfg.atPortCfg[1].ulIngressLatency = INGRESS_LATENCY;
  g_tSysCfg.atPortCfg[1].ulEgressLatency  = EGRESS_LATENCY;

  main_task();

  return 0;
}
