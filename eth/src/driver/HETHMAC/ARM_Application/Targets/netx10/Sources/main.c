#include "defines.h"
#include "common_func_netx10.h"
#include "hal_ethmac.h"
#include "sys.h"
#include "phy.h"
#include "uprintf.h"
#include <string.h>

#define NO_OF_ETH_PORTS 1

static uint32_t s_ulMiimFreq;
static uint32_t s_ulMiimPreamble;

/* print one character to STDOUT */
static int sys_ser_putc(int iChar, void* pvStream)
{
  NX10_UART_PutCharacter(0, (unsigned char)iChar);
  return 0;
}

void sys_poll(void)
{
  /* left empty */
}

/* read PHY register */
static unsigned int sys_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;
  unsigned int uiMdcFreq  = 1 & (s_ulMiimFreq >> uiPhy);
  unsigned int uiPreamble = 1 & (s_ulMiimPreamble >> uiPhy);

  NX10_MIIMU_ReadPhyReg(uiPreamble,
                        uiMdcFreq,
                        NX10_MIIMU_RTA_0_BITS,
                        uiPhy,
                        uiReg,
                        &usMiimuData,
                        0);
  return usMiimuData;
}

/* write PHY register */
static void sys_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int usValue)
{
  unsigned int uiMdcFreq  = 1 & (s_ulMiimFreq >> uiPhy);
  unsigned int uiPreamble = 1 & (s_ulMiimPreamble >> uiPhy);

  NX10_MIIMU_WritePhyReg(uiPreamble,
                         uiMdcFreq,
                         uiPhy,
                         uiReg,
                         (uint16_t)usValue,
                         0);
}

/* MII Management Interface */
static PHY_OPS_T s_tPhyOps =
{
  sys_phy_write,
  sys_phy_read
};

/* Set Ready/Run LED to a defined color */
void sys_led_rdyrun_set(LED_COLOUR_E eColour)
{
  static const NX10_RDYRUN_LED_E s_aeRdyRunCol[] = { NX10_RDYRUN_LED_OFF, NX10_RDYRUN_LED_RED, NX10_RDYRUN_LED_GREEN };
  NX10_RDYRUN_SetRdyRunLed(s_aeRdyRunCol[eColour]);
}

void sys_time_get(uint32_t* pulTimeS, uint32_t* pulTimeNs)
{
  NX10_SYSTIME_GetSystime(pulTimeS, pulTimeNs, NULL);
}

/* wait */
void sys_sleep(unsigned int uiTimeMs)
{
  NX10_GPIO_Sleep(1, uiTimeMs * 1000);
}

int main(void)
{
  NX10_PHY_CONTROL_T tPhyControlReg;

  /* lock IRQs during the initialization */
  NX10_IRQFIQ_LOCK

  /* clear all pending IRQs, this is important for 'soft restarts' from within the debugger */
  NX10_VIC_ClearVic();

  s_ulMiimFreq = 0;
  s_ulMiimPreamble = 0xffffffff;

  /* initialize SYSTIME unit */
  NX10_SYSTIME_SetBorder(SYSTIME_BORDER - 1, 0);
  NX10_SYSTIME_SetSpeed(SYSTIME_SPEED, 0);

  /* Configure MMIO pin multiplexer */
  NX10_MMIO_SetCfg(  0, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  1, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  2, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  3, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  4, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  5, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  6, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  7, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  8, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg(  9, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg( 10, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg( 11, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg( 12, NX10_MMIO_CONFIG_UART0_CTS,    0, 0 );
  NX10_MMIO_SetCfg( 13, NX10_MMIO_CONFIG_UART0_RTS,    0, 0 );
  NX10_MMIO_SetCfg( 14, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg( 15, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg( 16, NX10_MMIO_CONFIG_XM0_IO0,      0, 0 );  /* Ethernet LED */
  NX10_MMIO_SetCfg( 17, NX10_MMIO_CONFIG_XM0_IO1,      0, 0 );  /* Ethernet LED */
  NX10_MMIO_SetCfg( 18, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg( 19, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg( 20, NX10_MMIO_CONFIG_UART0_RXD,    0, 0 );
  NX10_MMIO_SetCfg( 21, NX10_MMIO_CONFIG_UART0_TXD,    0, 0 );
  NX10_MMIO_SetCfg( 22, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );
  NX10_MMIO_SetCfg( 23, NX10_MMIO_CONFIG_PIO_MODE,     0, 0 );

  /* initialize PHY */
  tPhyControlReg.val = 0;
  tPhyControlReg.bf.phy_address       = NX10_PHY_CTRL_PHY_ADDR;
  tPhyControlReg.bf.phy0_mode         = NX10_PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
  tPhyControlReg.bf.phy0_fxmode       = 0;
  tPhyControlReg.bf.phy0_automdix     = NX10_PHY_CTRL_PHY_AUTOMDIX;
  tPhyControlReg.bf.phy0_enable       = NX10_PHY_CTRL_PHY_ENABLE;
  tPhyControlReg.bf.phy_sim_byp       = 0;
  tPhyControlReg.bf.phy_reset         = 0;
  NX10_INTPHY_Init( &tPhyControlReg );

  phy_init(0, NX10_PHY_CTRL_PHY_ADDR + 0, PHYID_NETX_INTPHY, &s_tPhyOps);

  /* reset pointer FIFO borders */
  NX10_PFIFO_Reset();

  /* initialize UART0 */
  NX10_UART_InitSerial(0);
  uprint_init(sys_ser_putc, NULL, NULL);

  /* unlock IRQs */
  NX10_IRQFIQ_UNLOCK

  if( main_task(NO_OF_ETH_PORTS) )
  {
    sys_led_rdyrun_set(LED_COLOUR_RED);
  }

  /* do not return */
  while(1);
}
