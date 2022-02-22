#include "hal_ethmac.h"
#include "sys.h"
#include "common_func_netx90.h"
#include "hw_defines.h"
#include "phy.h"
#include "uprintf.h"

#define NO_OF_ETH_PORTS 2

/* Select target hardware by defining TARGET_HW */
/* Possible values for TARGET_HW */
#define _NXEB90_ETM_     0
#define _NRP_H90_        1
#define _NXHX90_JTAG_    2

/* Default hardware is _NXEB90_ETM_ */
#ifndef TARGET_HW
#define TARGET_HW _NXEB90_ETM_
#endif

#if TARGET_HW==_NXEB90_ETM_

#elif TARGET_HW == _NRP_H90_

#elif TARGET_HW == _NXHX90_JTAG_

#else
#error "Unknown Target Hardware (TARGET_HW)"
#endif

#ifndef EXT_MII
#define EXT_MII 0
#endif

#ifndef FX_MODE
#define FX_MODE 0
#endif

#define CONCAT(a,b) a##b
#define BF(name, val) ((uint32_t)val << CONCAT(SRT_NX90_, name))
#define BM(name) (CONCAT(MSK_NX90_, name))

#define INTPHY_ADDR 0
#define INTPHY_PORT_CNT 2
#define MIIMU_READ(phy,reg,data)  MIIMU_ReadPhyReg(MIIMU_SEND_PREAMBLE, MIIMU_MDCFREQ_HIGH, MIIMU_RTA_1_BITS, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) MIIMU_WritePhyReg(MIIMU_SEND_PREAMBLE, MIIMU_MDCFREQ_HIGH, MIIMU_RTA_1_BITS, phy, reg, data, NULL)

#define REG_BANK_DSP   0
#define REG_BANK_WOL   1
#define REG_BANK_BIST  3
#define REG_BANK_VMDAC 7

#define EPHY_MODE_CTRL_FARLOOPBACK 0x0200

#define SYSTIME_INST 0

static uint32_t const s_aulPadCtrlTable[] =
{
  /*          PORT                     IE DS              PE */

  /* set driver strength to 8mA for brighter LEDs */
  PADCTRL_CFG(PAD_CTRL_PORT_MLED0,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED1,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED2,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED3,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),

#if EXT_MII
  /* Note: Enable inputs at MII.RXD */
  PADCTRL_CFG(PAD_CTRL_PORT_MII0_RXD0, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII0_RXD1, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII0_RXD2, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII0_RXD3, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD0, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD1, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD2, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD3, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),

//  /* Note: Use high output driver strength of 8mA to achieve small rise and fall times. Errors have been observed with 4mA drivers! */
//  PADCTRL_CFG(PAD_CTRL_PORT_MII0_TXD0, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII0_TXD1, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII0_TXD2, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII0_TXD3, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII0_TXEN, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD0, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD1, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD2, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD3, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
//  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXEN, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
#endif

#if FX_MODE
  /* Note: Enable inputs at MII.RXD */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD0, 1, PADCTRL_DS_4MA, PADCTRL_PE_NONE),
#endif
};

/* print one character to STDOUT */
static int sys_ser_putc(int iChar, void* pvStream)
{
  UART_PutCharacter(0, (unsigned char)iChar);
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

  MIIMU_READ(uiPhy, uiReg,  &usMiimuData);

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


#if EXT_MII

static void ext_phy_reset( void )
{
  /* apply reset to external PHYs */
  AsicCtrl_SetResetCtrl(MSK_NX90_reset_ctrl_EN_RES_REQ_OUT);

  /* Wait 1ms (reset pulse) */
  sys_sleep(1);

  /* release reset */
  AsicCtrl_SetResetCtrl(MSK_NX90_reset_ctrl_EN_RES_REQ_OUT
                       |MSK_NX90_reset_ctrl_RES_REQ_OUT);

  /* Wait 10ms for PHY recovers reset */
  sys_sleep(10);

  EXTPHY_Init(0, 0, PHY_MODE_AUTONEG_AUTOMDIXEN);
  EXTPHY_Init(0, 1, PHY_MODE_AUTONEG_AUTOMDIXEN);
}

#else


/* Reset internal Dual-PHY */
static void int_phy_reset(void)
{
  INTPHY_CTRL_T tPhyCtrl;
  unsigned int uiPort;
  bool fFxMode = ( FX_MODE != 0 );

  INTPHY_Init();

  tPhyCtrl.uiPhyMode = fFxMode ? INTPHY_MODE_100BASE_TXFX_FD_NOAUTONEG_CRSRX
                               : INTPHY_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;

  tPhyCtrl.fPhySimBypass = false;
  tPhyCtrl.fPhyReset = false;
  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort) {
    tPhyCtrl.afPhyFxMode[uiPort]   = fFxMode;
    tPhyCtrl.afPhyAutoMdix[uiPort] = !fFxMode;
    tPhyCtrl.afPhyEnable[uiPort]   = true;
  }
  INTPHY_Reset(&tPhyCtrl);

  /* Wait 1ms for PHY reset */
  sys_sleep(1);
}
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
  ArmTimer_Sleep(0, uiTimeMs * 1000000);
}

int main(void)
{
  unsigned int uiIdx;

  /* lock IRQs during the initialization */
  IRQFIQ_LOCK

  ArmIrq_Deinit();

  /* initialize SYSTIME unit */
  SYSTIME_SetBorder(SYSTIME_INST, SYSTIME_BORDER - 1, 0);
  SYSTIME_SetSpeed(SYSTIME_INST, SYSTIME_SPEED, 0);

#if EXT_MII
  /* overwrite default MII configuration to fit external MII timing */
  /* Note: 10Mbit does not work with netX90mpw and external PHYs */
  g_auiMiiCfg[0] = 1;
  g_auiMiiCfg[1] = 1;

  /* Connect XC to external PHY, no TX_ER */
  AsicCtrl_SetIoConfig(1, BF(io_config0_sel_xm0_mii_cfg, 6)
                        | BF(io_config0_mask_sel_xc0_mdio, 1));
  AsicCtrl_SetIoConfig(2, BF(io_config1_sel_xm1_mii_cfg, 6));

  /* only low-active LINK LED is connected, activity is generated internally by MII signals */
  AsicCtrl_SetPhyCtrl0(BF(phy_ctrl0_phy0_led_invert, 1 << 2)
                     | BF(phy_ctrl0_phy1_led_invert, 1 << 2)
                      );

#else
  /* Connect XC to internal PHY */
  AsicCtrl_SetIoConfig(1, BF(io_config0_sel_xm0_mii_cfg, 8)
                        | BF(io_config0_mask_sel_xc0_mdio, 2));
  AsicCtrl_SetIoConfig(2, BF(io_config1_sel_xm1_mii_cfg, 8));

  /* PHY LEDs from internal PHY are all low-active */
  AsicCtrl_SetPhyCtrl0(MSK_NX90_phy_ctrl0_phy0_led_invert
                     | MSK_NX90_phy_ctrl0_phy1_led_invert
                      );
#endif

  /* enable clk25 */
  AsicCtrl_SetIoConfig(3, BF(io_config2_clk25out_oe, 1)
                        | BF(io_config2_sel_fo0, FX_MODE)
                        | BF(io_config2_sel_fo1, FX_MODE)
                      );

  /* Setup Multiplex Matrix */
  for(uiIdx = 0; uiIdx < 16; ++uiIdx)
  {
    MMIO_SetCfg(uiIdx, MMIO_CONFIG_PIO_MODE, 0, 0);
  }

  MLEDCTRL_Stop(0);

#if TARGET_HW == _NXEB90_ETM_
  /* Map MLED2 to INTPHY LEDs of port 0 */
  MLEDCTRL_ConfigureOutput(0, 4, MLED_CTRL_SEL_PHY_CTRL0_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 5, MLED_CTRL_SEL_PHY_CTRL0_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);

  /* Map MLED3 to INTPHY LEDs of port 1 */
  MLEDCTRL_ConfigureOutput(0, 6, MLED_CTRL_SEL_PHY_CTRL1_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 7, MLED_CTRL_SEL_PHY_CTRL1_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);

#elif TARGET_HW == _NRP_H90_
  /* Map MLED2 to PHY LEDs of port 0 */
  MLEDCTRL_ConfigureOutput(0, 4, MLED_CTRL_SEL_PHY_CTRL0_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 5, MLED_CTRL_SEL_PHY_CTRL0_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);

  /* Map MLED3 to PHY LEDs of port 1 */
  MLEDCTRL_ConfigureOutput(0, 6, MLED_CTRL_SEL_PHY_CTRL1_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 7, MLED_CTRL_SEL_PHY_CTRL1_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);

#elif TARGET_HW == _NXHX90_JTAG_
  /* Map MLED2 to PHY LEDs of port 0 */
  MLEDCTRL_ConfigureOutput(0, 4, MLED_CTRL_SEL_PHY_CTRL0_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 5, MLED_CTRL_SEL_PHY_CTRL0_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);

  /* Map MLED3 to PHY LEDs of port 1 */
  MLEDCTRL_ConfigureOutput(0, 6, MLED_CTRL_SEL_PHY_CTRL1_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 7, MLED_CTRL_SEL_PHY_CTRL1_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);

#else
#error "Unknown Target Hardware (TARGET_HW)"
#endif

  MLEDCTRL_Start(0, 3, 0);

  if( PADCTRL_RESULT_OK != PadCtrl_SetCfgTable(s_aulPadCtrlTable) )
  {
    while(1);
  }

  /* reset PHY */
#if EXT_MII
  ext_phy_reset();
  phy_init(0,  0, PHYID_BCM5241, &s_tPhyOps);
  phy_init(1,  1, PHYID_BCM5241, &s_tPhyOps);
#else
  int_phy_reset();
  phy_init(0,  0, PHYID_OMNIPHY, &s_tPhyOps);
  phy_init(1,  1, PHYID_OMNIPHY, &s_tPhyOps);
#endif

  /* reset pointer FIFO borders */
  PFIFO_Reset(0);

  /* initialize the STDOUT port */
  UART_Init(0, 115200, 8, 0, 1);
  UART_PutCharacter(0, '\n'); /* do initial new line */
  uprint_init(sys_ser_putc, NULL, NULL);

  /* unlock IRQs */
  IRQFIQ_UNLOCK

  if( main_task(NO_OF_ETH_PORTS) )
  {
    sys_led_rdyrun_set(LED_COLOUR_RED);
  }

  /* do not return */
  while(1);
}
