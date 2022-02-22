#include "defines.h"
#include "common_func_netx50.h"
#include "hal_ethmac.h"
#include "sys.h"
#include "phy.h"
#include "usb_globals.h"
#include "usb_requests_cdc.h"
#include "uprintf.h"

#ifdef TARGET_NETX5
#include "init_netx5.h"
#include "common_func_netx5.h"
#define MIIMU_READ(phy,reg,data)  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, phy + NX5_INT_PHY1_ADDR, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, phy + NX5_INT_PHY1_ADDR, reg, data, NULL)
#else
#define MIIMU_READ(phy,reg,data)  NX50_MIIMU_ReadPhyReg(NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, NX50_MIIMU_RTA_0_BITS, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) NX50_MIIMU_WritePhyReg(NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, phy, reg, data, NULL)
#endif

#define NO_OF_ETH_PORTS 2


static int sys_ser_flush(void* pvStream)
{
  usb_cdc_buf_send_flush();
  return 0;
}

/* print one character to STDOUT */
static int sys_ser_putc(int iChar, void* pvStream)
{
  usb_cdc_buf_send_put((unsigned char)iChar);
  return 0;
}

void sys_poll(void)
{
  usb_pollConnection();
}

/* read PHY register */
static unsigned int sys_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;

  MIIMU_READ(uiPhy, uiReg, &usMiimuData);
  return (unsigned int)usMiimuData;
}

/* write PHY register */
static void sys_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  MIIMU_WRITE(uiPhy, uiReg, (uint16_t)uiValue);
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
  static const NX50_RDYRUN_LED_E s_aeRdyRunCol[] = { NX50_RDYRUN_LED_OFF, NX50_RDYRUN_LED_RED, NX50_RDYRUN_LED_GREEN };
  NX50_RDYRUN_SetRdyRunLed(s_aeRdyRunCol[eColour]);
}

void sys_time_get(uint32_t* pulTimeS, uint32_t* pulTimeNs)
{
  NX50_SYSTIME_GetSystime(pulTimeS, pulTimeNs, NULL);
}

/* wait */
void sys_sleep(unsigned int uiTimeMs)
{
  NX50_GPIO_Sleep(1, uiTimeMs * 1000);
}

int main(void)
{
  /* lock IRQs during the initialization */
  NX50_IRQFIQ_LOCK

  /* clear all pending IRQs, this is important for 'soft restarts' from within the debugger */
  NX50_VIC_ClearVic();

  /* initialize SYSTIME unit */
  NX50_SYSTIME_SetBorder(SYSTIME_BORDER - 1, NULL);
  NX50_SYSTIME_SetSpeed(SYSTIME_SPEED, NULL);

  /* NXHX50-ETM */
  NX50_MMIO_SetCfg(  0, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg(  1, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg(  2, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg(  3, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg(  4, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg(  5, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg(  6, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg(  7, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg(  8, MMIO_CONFIG_GPIO14,              0, 0 );  /* Pull USB_D+ */
  NX50_MMIO_SetCfg(  9, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 10, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 11, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 12, MMIO_CONFIG_XM0_IO0,             0, 0 );  /* ETH0 LINK */
  NX50_MMIO_SetCfg( 13, MMIO_CONFIG_XM0_IO1,             0, 0 );  /* ETH0 ACT  */
  NX50_MMIO_SetCfg( 14, MMIO_CONFIG_XM1_IO0,             0, 0 );  /* ETH1 LINK */
  NX50_MMIO_SetCfg( 15, MMIO_CONFIG_XM1_IO1,             0, 0 );  /* ETH1 ACT  */
  NX50_MMIO_SetCfg( 16, MMIO_CONFIG_GPIO15,              0, 0 );  /* MMCINS */
  NX50_MMIO_SetCfg( 17, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 18, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 19, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 20, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 21, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 22, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 23, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 24, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 25, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 26, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 27, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 28, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 29, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 30, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 31, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 32, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 33, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 34, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 35, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 36, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 37, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 38, MMIO_CONFIG_INPUT,               0, 0 );
  NX50_MMIO_SetCfg( 39, MMIO_CONFIG_INPUT,               0, 0 );

#ifdef TARGET_NETX5
  init_netx5(true);
  phy_init(0, PHY_CTRL_PHY_ADDR + 0, PHYID_BCM5241, &s_tPhyOps);
  phy_init(1, PHY_CTRL_PHY_ADDR + 1, PHYID_BCM5241, &s_tPhyOps);
#else
  {
    /* initialize Dual-PHY */
    NX50_PHY_CONTROL_T tPhyControlReg;
    tPhyControlReg.bf.phy_address       = PHY_CTRL_PHY_ADDR;
    tPhyControlReg.bf.phy0_mode         = PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
    tPhyControlReg.bf.phy0_fxmode       = 0;
    tPhyControlReg.bf.phy0_automdix     = PHY_CTRL_PHY1_AUTOMDIX;
    tPhyControlReg.bf.phy0_np_msg_code  = PHY_CTRL_PHY1_NP_MSG_CODE;
    tPhyControlReg.bf.phy0_enable       = PHY_CTRL_PHY1_ENABLE;
    tPhyControlReg.bf.phy1_mode         = PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
    tPhyControlReg.bf.phy1_fxmode       = 0;
    tPhyControlReg.bf.phy1_automdix     = PHY_CTRL_PHY2_AUTOMDIX;
    tPhyControlReg.bf.phy1_np_msg_code  = PHY_CTRL_PHY2_NP_MSG_CODE;
    tPhyControlReg.bf.phy1_enable       = PHY_CTRL_PHY2_ENABLE;
    tPhyControlReg.bf.reserved1         = 0;
    tPhyControlReg.bf.phy_sim_byp       = 0;
    tPhyControlReg.bf.phy_reset         = 0;
    NX50_INTPHY_Init( &tPhyControlReg );
  }

  phy_init(0, PHY_CTRL_PHY_ADDR + 0, PHYID_NETX_INTPHY, &s_tPhyOps);
  phy_init(1, PHY_CTRL_PHY_ADDR + 1, PHYID_NETX_INTPHY, &s_tPhyOps);
#endif

  /* pull down USB_D+ (Bus Reset) */
  NX50_GPIO_SetupMode(14, NX50_GPIO_MODE_OUTPUT_SET_TO_0, NX50_GPIO_MODE_NOINVERT);

  sys_sleep(10);

  /* initialize the USB port */
  usb_init();

  /* pull up USB_D+ (Release Bus Reset) */
  NX50_GPIO_SetupMode(14, NX50_GPIO_MODE_OUTPUT_SET_TO_1, NX50_GPIO_MODE_NOINVERT);

  /* reset pointer FIFO borders */
  NX50_PFIFO_Reset();

  /* tell uprintf() how to print it's output */
  uprint_init(sys_ser_putc, sys_ser_flush, NULL);

  /* unlock IRQs */
  NX50_IRQFIQ_UNLOCK

  if( main_task(NO_OF_ETH_PORTS) )
  {
    sys_led_rdyrun_set(LED_COLOUR_RED);
  }

  /* do not return */
  while(1);
}
