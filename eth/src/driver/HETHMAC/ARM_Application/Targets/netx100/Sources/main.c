#include "defines.h"
#include "common_func_netx500.h"
#include "hal_ethmac.h"
#include "sys.h"
#include "phy.h"
#include "usb_globals.h"
#include "usb_requests_cdc.h"
#include "uprintf.h"

static uint32_t s_ulMiimFreq;
static uint32_t s_ulMiimPreamble;

/* Select target hardware by defining TARGET_HW */
/* Possible values for TARGET_HW */
#define _NXHX500_ETM_     0
#define _NSS3_1NRT_       1

/* Default hardware is _NXHX500_ETM_ */
#ifndef TARGET_HW
#define TARGET_HW _NXHX500_ETM_
#endif

#if TARGET_HW == _NXHX500_ETM_
#define NO_OF_ETH_PORTS 2
#elif TARGET_HW == _NSS3_1NRT_
#define NO_OF_ETH_PORTS 3
#endif


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
  unsigned int uiMdcFreq  = 1 & (s_ulMiimFreq >> uiPhy);
  unsigned int uiPreamble = 1 & (s_ulMiimPreamble >> uiPhy);

  NX500_MIIMU_ReadPhyReg(uiPreamble,
                         uiMdcFreq,
                         NX500_MIIMU_RTA_0_BITS,
                         uiPhy,
                         uiReg,
                         &usMiimuData,
                         0);
  return (unsigned int)usMiimuData;
}

/* write PHY register */
static void sys_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  unsigned int uiMdcFreq  = 1 & (s_ulMiimFreq >> uiPhy);
  unsigned int uiPreamble = 1 & (s_ulMiimPreamble >> uiPhy);

  NX500_MIIMU_WritePhyReg(uiPreamble,
                          uiMdcFreq,
                          uiPhy,
                          uiReg,
                          (uint16_t)uiValue,
                          0);
}

/* MII Management Interface */
static PHY_OPS_T s_tPhyOps =
{
  sys_phy_write,
  sys_phy_read
};

#if TARGET_HW == _NSS3_1NRT_
/* reset and setup PHY */
static void sys_ext_phy_reset(unsigned int uiPhyAddr, uint32_t ulExpPhyId)
{
  unsigned short usPhyRegVal;
  uint32_t usPhyReg2Val;

  /* Reset external PHY */
  NX500_RSTCTRL_SetCfg(  MSK_NX500_reset_ctrl_RES_REQ_OUT
                        |MSK_NX500_reset_ctrl_EN_RES_REQ_OUT
                      );

  /* make a dummy MDIO read to cure MDIO first access bug (used to generate Reset Delay) */
  if( 0 != NX500_MIIMU_ReadPhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, NX500_MIIMU_RTA_0_BITS, uiPhyAddr, 0, &usPhyRegVal, 0 ) ) {
    NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_RED );
    while(1) {}
  }

  /* clear reset out pin */
  NX500_RSTCTRL_SetCfg( MSK_NX500_reset_ctrl_EN_RES_REQ_OUT );

  /* read PHY ID (Register 3) to check that PHY is accessible via MDIO */
  do {
    if( 0 != NX500_MIIMU_ReadPhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, NX500_MIIMU_RTA_0_BITS, uiPhyAddr, 2, &usPhyRegVal, 0 ) ) {
      NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_RED );
      while(1) {}
    }
    usPhyReg2Val = (uint32_t)usPhyRegVal << 16;
  } while( usPhyReg2Val != ulExpPhyId );

  /* set AutoNeg advertisement to all capable */
  if( 0 != NX500_MIIMU_ReadPhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, NX500_MIIMU_RTA_0_BITS, uiPhyAddr, 4, &usPhyRegVal, 0 ) ) {
    NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_RED );
    while(1) {}
  }
  usPhyRegVal |= (0xf << 5);
  if( 0 != NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uiPhyAddr, 4, usPhyRegVal, 0 ) ) {
    NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_RED );
    while(1) {}
  }

#ifdef PHY_MICREL_KSZ8041
  /* configure LED mode */
  if( 0 != NX500_MIIMU_ReadPhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, NX500_MIIMU_RTA_0_BITS, uiPhyAddr, 30, &usPhyRegVal, 0 ) ) {
    NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_RED );
    while(1) {}
  }
  usPhyRegVal = (usPhyRegVal & 0x3fff)
               |(0x1 << 14);
  if( 0 != NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uiPhyAddr, 30, usPhyRegVal, 0 ) ) {
    NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_RED );
    while(1) {}
  }
#endif

  /* enable and restart AutoNeg */
  usPhyRegVal = (1 << 12) | (1 << 9);
  if( 0 != NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uiPhyAddr, 0, usPhyRegVal, 0 ) ) {
    NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_RED );
    while(1) {}
  }
}
#endif

/* Set Ready/Run LED to a defined color */
void sys_led_rdyrun_set(LED_COLOUR_E eColour)
{
  static const NX500_RDYRUN_LED_E s_aeRdyRunCol[] = { NX500_RDYRUN_LED_OFF, NX500_RDYRUN_LED_RED, NX500_RDYRUN_LED_GREEN };
  NX500_RDYRUN_SetRdyRunLed(s_aeRdyRunCol[eColour]);
}

void sys_time_get(uint32_t* pulTimeS, uint32_t* pulTimeNs)
{
  NX500_SYSTIME_GetSystime(pulTimeS, pulTimeNs, NULL);
}

/* wait */
void sys_sleep(unsigned int uiTimeMs)
{
  NX500_GPIO_Sleep(1, uiTimeMs * 1000);
}

int main(void)
{
  bool fUsbReset = true;
  NX500_PHY_CONTROL_T tPhyControlReg;

  /* lock IRQs during the initialization */
  NX500_IRQFIQ_LOCK

  /* clear all pending IRQs, this is important for 'soft restarts' from within the HITOP debugger */
  NX500_VIC_ClearVic();

  s_ulMiimFreq = 0;
  s_ulMiimPreamble = 0xffffffff;

  /* initialize SYSTIME unit */
  NX500_SYSTIME_SetBorder(SYSTIME_BORDER - 1, NULL);
  NX500_SYSTIME_SetSpeed(SYSTIME_SPEED, NULL);

#ifdef __ENABLE_MMU__
  /* This call enables I/D Caches and places TTB at 0x1c000 */
  NX500_MMU_Init( (unsigned long*)0x1C000 );
#endif

#if TARGET_HW == _NXHX500_ETM_
  /* Configure PIOs 5..4 as output (Dual-LED) */
  NX500_PIO_SetMode( 4, NX500_PIO_MODE_OUT );
  NX500_PIO_SetMode( 5, NX500_PIO_MODE_OUT );

  /* Configure PIOs 15..8 as input (DIP switches) */
  NX500_PIO_SetMode( 8, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 9, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 10, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 11, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 12, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 13, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 14, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 15, NX500_PIO_MODE_IN );

  /* Configure PIOs 23..16 as output (LEDs) and clear low-active LEDs */
  NX500_PIO_SetMode( 16, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput( 16, 1 );
  NX500_PIO_SetMode( 17, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput( 17, 1 );
  NX500_PIO_SetMode( 18, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput( 18, 1 );
  NX500_PIO_SetMode( 19, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput( 19, 1 );
  NX500_PIO_SetMode( 20, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput( 20, 1 );
  NX500_PIO_SetMode( 21, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput( 21, 1 );
  NX500_PIO_SetMode( 22, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput( 22, 1 );
  NX500_PIO_SetMode( 23, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput( 23, 1 );

  /* initialize Dual-PHY */
  tPhyControlReg.bf.phy_address       = 0;
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
  NX500_INTPHY_Init( &tPhyControlReg );

  phy_init(0, 0, PHYID_NETX_INTPHY, &s_tPhyOps);
  phy_init(1, 1, PHYID_NETX_INTPHY, &s_tPhyOps);

  /* detect old revision of NXHX board: Revision 0..2 have pull down at output LEDs */
  NX500_GPIO_SetupMode(8, NX500_GPIO_MODE_INPUT_READ, NX500_GPIO_MODE_NOINVERT);
  if( 0 == (0x100 & NX500_GPIO_GetIn()) )
  {
    fUsbReset = false;
  }

#elif TARGET_HW == _NSS3_1NRT_
  /* Configure PIOs 5..4 as output (Dual-LED) and set them off */
  NX500_PIO_SetMode( 4, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput (4, 1);
  NX500_PIO_SetMode( 5, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput (5, 1);
  NX500_PIO_SetMode( 6, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput (6, 1);
  NX500_PIO_SetMode( 7, NX500_PIO_MODE_OUT ); NX500_PIO_SetOutput (7, 1);

  /* disable output enables of PIOs 0..3,8..14,16,17,26..30 to use PU/PD configuration of PHY */
  NX500_PIO_SetMode( 0, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 1, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 2, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 3, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 8, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 9, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 10, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 11, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 12, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 13, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 14, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 16, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 17, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 26, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 27, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 28, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 29, NX500_PIO_MODE_IN );
  NX500_PIO_SetMode( 30, NX500_PIO_MODE_IN );

  /* set io_config to enable MII2 */
  NX500_AsicCtrl_SetIoConfig( 1,  MSK_NX500_io_config_sel_mii2     /* for MII */
                                 |MSK_NX500_io_config_sel_mii23    /* for MDIO */
                                 |MSK_NX500_io_config_sel_led_mii2 /* for Link Signal */
                            );

  /* initialize Dual-PHY */
  tPhyControlReg.bf.phy_address       = (30 >> 1); /* Note: Internal PHY initialized to 30/31 to avoid MDIO address conflicts with external PHY */
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
  NX500_INTPHY_Init( &tPhyControlReg );

  /* initialize external PHY */
  sys_ext_phy_reset(1, PHYID_MICREL_KSZ8041);

  phy_init(0, 30, PHYID_NETX_INTPHY, &s_tPhyOps);
  phy_init(1, 31, PHYID_NETX_INTPHY, &s_tPhyOps);
  phy_init(2,  1, PHYID_MICREL_KSZ8041, &s_tPhyOps); /* MDIO address of external PHY */

  /* detect old revision of NS-S3 */
  if( 0 == NX500_GPIO_GetInput(13) )
  {
    /* HW REV <= 3 */
    fUsbReset = false;
  }

 #endif

  if(fUsbReset) {
    /* pull down USB_D+ (Bus Reset) */
    NX500_GPIO_SetupMode(12, NX500_GPIO_MODE_OUTPUT_SET_TO_0, NX500_GPIO_MODE_NOINVERT);
  }

  sys_sleep(10);

  /* initialize the USB port */
  usb_init();

  if(fUsbReset) {
    /* pull up USB_D+ (Release Bus Reset) */
    NX500_GPIO_SetupMode(12, NX500_GPIO_MODE_OUTPUT_SET_TO_1, NX500_GPIO_MODE_NOINVERT);
  }

  /* reset pointer FIFO borders */
  NX500_PFIFO_Reset();

  /* tell uprintf() how to print it's output */
  uprint_init(sys_ser_putc, sys_ser_flush, NULL);

  /* unlock IRQs */
  NX500_IRQFIQ_UNLOCK

  if( main_task(NO_OF_ETH_PORTS) )
  {
    sys_led_rdyrun_set(LED_COLOUR_RED);
  }

  /* do not return */
  while(1);
}
