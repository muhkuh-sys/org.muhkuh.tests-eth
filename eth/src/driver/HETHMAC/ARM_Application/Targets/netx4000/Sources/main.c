#include "hal_ethmac.h"
#include "sys.h"
#include "common_func_netx4000.h"
#include "hw_defines.h"
#include "phy.h"
#include "hal_extphy.h"
#include "uprintf.h"

#define SYSTIME_INST 0

/* Select target hardware by defining TARGET_HW */
/* Possible values for TARGET_HW */
#define _NXHX4000_JTAG_PLUS_  0 /* Rev 3 */
#define _NXHX4000_SWITCH_     1
#define _CIFX_PCIE4000_2RE_   2

/* Default hardware is _NXHX4000_JTAG_PLUS_ */
#ifndef TARGET_HW
#define TARGET_HW _NXHX4000_JTAG_PLUS_
#endif

#if TARGET_HW == _NXHX4000_JTAG_PLUS_
#define NO_OF_ETH_PORTS 2

#define INIT_LEDCOM0    MMIO_SetMode( 53, MMIO_MODE_OUT ); MMIO_SetMode( 54, MMIO_MODE_OUT );
#define SET_LEDCOM0_OFF MMIO_SetOutput( 53, 0 ); MMIO_SetOutput( 54, 0 );
#define SET_LEDCOM0_RED MMIO_SetOutput( 53, 0 ); MMIO_SetOutput( 54, 1 );
#define SET_LEDCOM0_GRN MMIO_SetOutput( 53, 1 ); MMIO_SetOutput( 54, 0 );

#define INIT_LEDCOM1    MMIO_SetMode( 55, MMIO_MODE_OUT ); MMIO_SetMode( 56, MMIO_MODE_OUT );
#define SET_LEDCOM1_OFF MMIO_SetOutput( 55, 0 ); MMIO_SetOutput( 56, 0 );
#define SET_LEDCOM1_RED MMIO_SetOutput( 55, 0 ); MMIO_SetOutput( 56, 1 );
#define SET_LEDCOM1_GRN MMIO_SetOutput( 55, 1 ); MMIO_SetOutput( 56, 0 );

#elif TARGET_HW == _NXHX4000_SWITCH_
#define NO_OF_ETH_PORTS 4
#define EXT_MII         1

#define INIT_LEDCOM0    MMIO_SetMode( 55, MMIO_MODE_OUT ); MMIO_SetMode( 56, MMIO_MODE_OUT );
#define SET_LEDCOM0_OFF MMIO_SetOutput( 55, 0 ); MMIO_SetOutput( 56, 0 );
#define SET_LEDCOM0_RED MMIO_SetOutput( 55, 0 ); MMIO_SetOutput( 56, 1 );
#define SET_LEDCOM0_GRN MMIO_SetOutput( 55, 1 ); MMIO_SetOutput( 56, 0 );

#define INIT_LEDCOM1    MMIO_SetMode( 57, MMIO_MODE_OUT ); MMIO_SetMode( 58, MMIO_MODE_OUT );
#define SET_LEDCOM1_OFF MMIO_SetOutput( 57, 0 ); MMIO_SetOutput( 58, 0 );
#define SET_LEDCOM1_RED MMIO_SetOutput( 57, 0 ); MMIO_SetOutput( 58, 1 );
#define SET_LEDCOM1_GRN MMIO_SetOutput( 57, 1 ); MMIO_SetOutput( 58, 0 );

#elif TARGET_HW == _CIFX_PCIE4000_2RE_
#define NO_OF_ETH_PORTS 4
#define EXT_MII         1

#define INIT_LEDCOM0    MMIO_SetMode( 60, MMIO_MODE_OUT ); MMIO_SetMode( 61, MMIO_MODE_OUT );
#define SET_LEDCOM0_OFF MMIO_SetOutput( 60, 0 ); MMIO_SetOutput( 61, 0 );
#define SET_LEDCOM0_RED MMIO_SetOutput( 60, 0 ); MMIO_SetOutput( 61, 1 );
#define SET_LEDCOM0_GRN MMIO_SetOutput( 60, 1 ); MMIO_SetOutput( 61, 0 );

#define INIT_LEDCOM1    MMIO_SetMode( 62, MMIO_MODE_OUT ); MMIO_SetMode( 63, MMIO_MODE_OUT );
#define SET_LEDCOM1_OFF MMIO_SetOutput( 62, 0 ); MMIO_SetOutput( 63, 0 );
#define SET_LEDCOM1_RED MMIO_SetOutput( 62, 0 ); MMIO_SetOutput( 63, 1 );
#define SET_LEDCOM1_GRN MMIO_SetOutput( 62, 1 ); MMIO_SetOutput( 63, 0 );

#define INIT_LEDCOM2    MMIO_SetMode( 4, MMIO_MODE_OUT ); MMIO_SetMode( 5, MMIO_MODE_OUT );
#define SET_LEDCOM2_OFF MMIO_SetOutput( 4, 0 ); MMIO_SetOutput( 5, 0 );
#define SET_LEDCOM2_RED MMIO_SetOutput( 4, 0 ); MMIO_SetOutput( 5, 1 );
#define SET_LEDCOM2_GRN MMIO_SetOutput( 4, 1 ); MMIO_SetOutput( 5, 0 );

#define INIT_LEDCOM3    MMIO_SetMode( 6, MMIO_MODE_OUT ); MMIO_SetMode( 7, MMIO_MODE_OUT );
#define SET_LEDCOM3_OFF MMIO_SetOutput( 6, 0 ); MMIO_SetOutput( 7, 0 );
#define SET_LEDCOM3_RED MMIO_SetOutput( 6, 0 ); MMIO_SetOutput( 7, 1 );
#define SET_LEDCOM3_GRN MMIO_SetOutput( 6, 1 ); MMIO_SetOutput( 7, 0 );

#endif

#define CONCAT(a,b) a##b
#define BF(name, val) ((uint32_t)val << CONCAT(SRT_NX4000_, name))
#define BM(name) (CONCAT(MSK_NX4000_, name))

typedef struct MMIO_Ttag
{
  unsigned int uiMmioNo;
  MMIO_CONFIG_E eSel;
  unsigned int uiInvOut;
  unsigned int uiInvIn;
} MMIO_T;

#if TARGET_HW == _NXHX4000_JTAG_PLUS_

static MMIO_T s_atMmio[] =
{
  {  46, MMIO_CONFIG_PHY0_LED_PHY_CTRL_LNK, 1, 0 },
  {  45, MMIO_CONFIG_PHY0_LED_PHY_CTRL_ACT, 1, 0 },
  {  52, MMIO_CONFIG_PHY1_LED_PHY_CTRL_LNK, 1, 0 },
  {  51, MMIO_CONFIG_PHY1_LED_PHY_CTRL_ACT, 1, 0 },
  {  53, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM0 Green low-active */
  {  54, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM0 Red   low-active */
  {  55, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM1 Green low-active */
  {  56, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM1 Red   low-active */
};

#elif TARGET_HW == _NXHX4000_SWITCH_

static MMIO_T s_atMmio[] =
{
  {   5, MMIO_CONFIG_XM10_MII_MDIO,         0, 0 }, /* MDIO for extPHY0 (PHY Address = 0) */
  {   6, MMIO_CONFIG_XM10_MII_MDC,          0, 0 }, /* MDC for extPHY0 (PHY Address = 0) */
  {  40, MMIO_CONFIG_XM11_MII_MDIO,         0, 0 }, /* MDC  for extPHY1 (PHY Address = 1) */
  {  41, MMIO_CONFIG_XM11_MII_MDC,          0, 0 }, /* MDIO for extPHY1 (PHY Address = 1) */
  {  42, MMIO_CONFIG_PHY2_LED_PHY_CTRL_ACT, 1, 0 }, /* XC_INST1.COM1.red (used as activity indicator)  */
  {  43, MMIO_CONFIG_PHY2_LED_PHY_CTRL_LNK, 1, 0 }, /* XC_INST1.COM1.green (used as link indicator) */
  {  44, MMIO_CONFIG_PHY3_LED_PHY_CTRL_ACT, 1, 0 }, /* XC_INST1.COM0.red (used as activity indicator)  */
  {  45, MMIO_CONFIG_PHY3_LED_PHY_CTRL_LNK, 1, 0 }, /* XC_INST1.COM0.green (used as link indicator) */
  {  51, MMIO_CONFIG_GPIO8,                 1, 0 }, /* Unused */
  {  52, MMIO_CONFIG_GPIO9,                 1, 0 }, /* Unused */
  {  53, MMIO_CONFIG_GPIO10,                1, 0 }, /* APL.red */
  {  54, MMIO_CONFIG_GPIO11,                1, 0 }, /* APL.green */
  {  55, MMIO_CONFIG_GPIO0,                 1, 0 }, /* XC_INST0.COM0.red   */
  {  56, MMIO_CONFIG_GPIO1,                 1, 0 }, /* XC_INST0.COM0.green */
  {  57, MMIO_CONFIG_GPIO2,                 1, 0 }, /* XC_INST0.COM1.red   */
  {  58, MMIO_CONFIG_GPIO3,                 1, 0 }, /* XC_INST0.COM1.green */
  {  59, MMIO_CONFIG_XC0_TRIGGER0,          0, 0 },
  {  60, MMIO_CONFIG_XC0_TRIGGER1,          0, 0 },
  {  61, MMIO_CONFIG_XC1_TRIGGER0,          0, 0 },
  {  62, MMIO_CONFIG_XC1_TRIGGER1,          0, 0 },
  {  64, MMIO_CONFIG_PHY2_LED_ACT,          0, 1 }, /* invert low-active BCM5241.activity */
  {  65, MMIO_CONFIG_PHY2_LED_LNK,          0, 1 }, /* invert low-active BCM5241.link     */
  {  66, MMIO_CONFIG_PHY3_LED_ACT,          0, 1 }, /* invert low-active BCM5241.activity */
  {  67, MMIO_CONFIG_PHY3_LED_LNK,          0, 1 }, /* invert low-active BCM5241.link     */
  {  70, MMIO_CONFIG_PHY0_LED_PHY_CTRL_ACT, 1, 0 },
  {  71, MMIO_CONFIG_PHY0_LED_PHY_CTRL_LNK, 1, 0 },
  {  72, MMIO_CONFIG_PHY1_LED_PHY_CTRL_ACT, 1, 0 },
  {  73, MMIO_CONFIG_PHY1_LED_PHY_CTRL_LNK, 1, 0 },
  {  74, MMIO_CONFIG_GPIO12,                1, 0 }, /* Unused */
  {  75, MMIO_CONFIG_GPIO13,                1, 0 }  /* Unused */
};
#elif TARGET_HW == _CIFX_PCIE4000_2RE_

static MMIO_T s_atMmio[] =
{
  {   8, MMIO_CONFIG_XM10_MII_MDIO,         0, 0 }, /* MDIO for extPHY0 (PHY Address = 0) */
  {   9, MMIO_CONFIG_XM10_MII_MDC,          0, 0 }, /* MDC  for extPHY0 (PHY Address = 0) */
  {  12, MMIO_CONFIG_XM11_MII_MDIO,         0, 0 }, /* MDC  for extPHY1 (PHY Address = 1) */
  {  13, MMIO_CONFIG_XM11_MII_MDC,          0, 0 }, /* MDIO for extPHY1 (PHY Address = 1) */

  {  56, MMIO_CONFIG_PHY0_LED_PHY_CTRL_LNK, 1, 0 },
  {  57, MMIO_CONFIG_PHY0_LED_PHY_CTRL_ACT, 1, 0 },
  {  58, MMIO_CONFIG_PHY1_LED_PHY_CTRL_LNK, 1, 0 },
  {  59, MMIO_CONFIG_PHY1_LED_PHY_CTRL_ACT, 1, 0 },
  {  42, MMIO_CONFIG_PHY2_LED_PHY_CTRL_LNK, 1, 0 },
  {   1, MMIO_CONFIG_PHY2_LED_PHY_CTRL_ACT, 1, 0 },
  {   2, MMIO_CONFIG_PHY3_LED_PHY_CTRL_LNK, 1, 0 },
  {   3, MMIO_CONFIG_PHY3_LED_PHY_CTRL_ACT, 1, 0 },

  {  60, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM0.green */
  {  61, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM0.red   */
  {  62, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM1.green */
  {  63, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM1.red   */
  {   4, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM2.green */
  {   5, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM2.red   */
  {   6, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM3.green */
  {   7, MMIO_CONFIG_PIO_MODE,              1, 0 }, /* COM3.red   */

  {  45, MMIO_CONFIG_PHY2_LED_LNK,          0, 1 }, /* invert low-active BCM5241.activity */
  {  46, MMIO_CONFIG_PHY2_LED_ACT,          0, 1 }, /* invert low-active BCM5241.link     */
  {  50, MMIO_CONFIG_PHY3_LED_LNK,          0, 1 }, /* invert low-active BCM5241.activity */
  {  51, MMIO_CONFIG_PHY3_LED_ACT,          0, 1 }, /* invert low-active BCM5241.link     */
};
#else
#error "Unknown Target Hardware (TARGET_HW)"
#endif

#define MIIMU_READ(phy,reg,data)  MIIMU_ReadPhyReg(MIIMU_SEND_PREAMBLE, MIIMU_MDCFREQ_HIGH, MIIMU_RTA_1_BITS, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) MIIMU_WritePhyReg(MIIMU_SEND_PREAMBLE, MIIMU_MDCFREQ_HIGH, MIIMU_RTA_1_BITS, phy, reg, data, NULL)

/* print one character to STDOUT */
static int sys_ser_putc(int iChar, void* pvStream)
{
  USBCDC_BufSendPut((unsigned char)iChar);
  return 0;
}

void sys_poll(void)
{
  /* left empty */
}

int sys_ser_flush(void* pvStream)
{
  USBCDC_BufSendFlush();
  return 0;
}

/* read internal PHY register */
static unsigned int sys_int_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;
  MIIMU_READ(uiPhy, uiReg, &usMiimuData);
  return (unsigned int)usMiimuData;
}

/* write internal PHY register */
static void sys_int_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  MIIMU_WRITE(uiPhy, uiReg, (uint16_t)uiValue);
}

/* MII Management Interface */
static PHY_OPS_T s_tIntPhyOps =
{
  sys_int_phy_write,
  sys_int_phy_read
};

#if defined(EXT_MII)
/* read external PHY register */
static unsigned int sys_ext_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
  unsigned int uiPhyCtrlInst = uiPhy + 2; /* ExtPhy0/1 connected at PhyCtrl2/3 has address 0/1 */
  uint16_t usMiimuData = 0;
  if(uiPhy < 2) {
    usMiimuData = (uint16_t) EXTPHY_Read( uiPhyCtrlInst, uiPhy, uiReg );
  }

  return (unsigned int)usMiimuData;
}

/* write external PHY register */
static void sys_ext_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  unsigned int uiPhyCtrlInst = uiPhy + 2; /* ExtPhy0/1 connected at PhyCtrl2/3 has address 0/1 */
  if(uiPhy < 2) {
    EXTPHY_Write(uiPhyCtrlInst, uiPhy, uiReg, uiValue);
  }
}

static PHY_OPS_T s_tExtPhyOps =
{
  sys_ext_phy_write,
  sys_ext_phy_read
};
#endif

/* Set Ready/Run LED to a defined color */
void sys_led_rdyrun_set(LED_COLOUR_E eColour)
{
  static const RDYRUN_LED_E s_aeRdyRunCol[] = { RDYRUN_LED_OFF, RDYRUN_LED_RED, RDYRUN_LED_GREEN };
  RDYRUN_SetRdyRunLed(s_aeRdyRunCol[eColour]);
}

void sys_time_get(uint32_t* pulTimeS, uint32_t* pulTimeNs)
{
  SYSTIME_GetSystime(SYSTIME_INST, pulTimeS, pulTimeNs, NULL);
}

/* wait */
void sys_sleep(unsigned int uiTimeMs)
{
  ArmTimer_Sleep(0, uiTimeMs * 100000);
}

#if defined(EXT_MII)

static void ext_phy_reset( void )
{
  /* apply reset to external PHYs */
  AsicCtrl_SetResetOut(true, true); /* drive RESET_OUT low */

  /* Wait 1ms (reset pulse) */
  sys_sleep(1);

  /* release reset */
  AsicCtrl_SetResetOut(false, true); /* drive RESET_OUT high */

  /* Wait 10ms for PHY recovers from reset */
  sys_sleep(10);

  EXTPHY_Init(2, 0, /* PHY address */ PHY_MODE_AUTONEG_AUTOMDIXEN);
  EXTPHY_Init(3, 1, /* PHY address */ PHY_MODE_AUTONEG_AUTOMDIXEN);
}

#endif

int main(void)
{
  unsigned int uiIdx;
  INTPHY_CTRL_T tPhyCtrl;

  /* lock IRQs during the initialization */
  IRQFIQ_LOCK

  ArmIrq_Deinit();

  /* initialize SYSTIME unit */
  SYSTIME_SetBorder(0, SYSTIME_BORDER - 1, 0);
  SYSTIME_SetSpeed(0, SYSTIME_SPEED, 0);

  /* Setup Multiplex Matrix */
  for(uiIdx = 0; uiIdx < (sizeof(s_atMmio)/sizeof(s_atMmio[0])) ; ++uiIdx)
  {
    MMIO_SetCfg(s_atMmio[uiIdx].uiMmioNo, s_atMmio[uiIdx].eSel, s_atMmio[uiIdx].uiInvOut, s_atMmio[uiIdx].uiInvIn);
  }

  /* different MII timing for external PHYs */
  g_auiMiiCfg[2] = 1;
  g_auiMiiCfg[3] = 1;

  /* reset I/O and LEDs */
  INIT_LEDCOM0
  INIT_LEDCOM1
#if TARGET_HW == _CIFX_PCIE4000_2RE_
  INIT_LEDCOM2
  INIT_LEDCOM3
#endif

#ifndef LED_CHECK_DISABLE
  /* set and clear COM0 LED */
  SET_LEDCOM0_RED
  sys_sleep(500);
  SET_LEDCOM0_GRN
  sys_sleep(500);
  SET_LEDCOM0_OFF

  /* set and clear COM1 LED */
  SET_LEDCOM1_RED
  sys_sleep(500);
  SET_LEDCOM1_GRN
  sys_sleep(500);
  SET_LEDCOM1_OFF

 #if TARGET_HW == _CIFX_PCIE4000_2RE_
  /* set and clear COM0 LED */
  SET_LEDCOM2_RED
  sys_sleep(500);
  SET_LEDCOM2_GRN
  sys_sleep(500);
  SET_LEDCOM2_OFF

  /* set and clear COM1 LED */
  SET_LEDCOM3_RED
  sys_sleep(500);
  SET_LEDCOM3_GRN
  sys_sleep(500);
  SET_LEDCOM3_OFF
#endif

#endif /* ifndef LED_CHECK_DISABLE */

  /* reset PHY */
  INTPHY_Init();

  tPhyCtrl.uiPhyMode = INTPHY_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
  tPhyCtrl.fPhySimBypass = false;
  tPhyCtrl.fPhyReset = false;
  tPhyCtrl.afPhyEnable[0] = true;
  tPhyCtrl.afPhyEnable[1] = true;
  tPhyCtrl.afPhyAutoMdix[0] = true;
  tPhyCtrl.afPhyAutoMdix[1] = true;
  tPhyCtrl.afPhyFxMode[0] = false;
  tPhyCtrl.afPhyFxMode[1] = false;
  INTPHY_Reset(&tPhyCtrl);

  phy_init(0, 0, PHYID_NETX_INTPHY, &s_tIntPhyOps);
  phy_init(1, 1, PHYID_NETX_INTPHY, &s_tIntPhyOps);

#if defined(EXT_MII)

  ext_phy_reset();
  phy_init(2,  0, PHYID_BCM5241, &s_tExtPhyOps);
  phy_init(3,  1, PHYID_BCM5241, &s_tExtPhyOps);

  /* Connect XC1 to external PHY, no TX_ER */
  AsicCtrl_SetIoConfig(2, BF(io_config2_sel_xm10_mii, 6)
                        | BF(io_config2_sel_xm11_mii, 6)
                      );
#endif

  /* reset pointer FIFO borders */
  PFIFO_Reset(0);

  /* initialize the USB port */
  USBCDC_Init();
  sys_sleep(1); /* wait at least 1us between initialization and enabling pull-up */
  USBCDC_Connect(); /* enable Pull-Up */
  uprint_init(sys_ser_putc, sys_ser_flush, NULL);

  /* unlock IRQs */
  IRQFIQ_UNLOCK

  if( main_task(NO_OF_ETH_PORTS) )
  {
    sys_led_rdyrun_set(LED_COLOUR_RED);
  }

  /* do not return */
  while(1);
}

