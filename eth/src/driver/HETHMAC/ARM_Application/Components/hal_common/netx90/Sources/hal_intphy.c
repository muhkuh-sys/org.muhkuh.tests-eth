/* Includes */
#include "hal_intphy.h"
#include "hal_miimu.h"
#include "hw_defines.h"
#include <string.h>

/* Definitions */

#define MSK_TSTCNTL_READ                0x8000U
#define MSK_TSTCNTL_WRITE               0x4000U
#define MSK_TSTCNTL_TEST_MODE           0x0400U

/* bit masks for register 0 (BASIC CONTROL) */
#define MSK_PHYREG_BASIC_CONTROL_SPEED_SELECT    0x2000
#define MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE  0x1000

#define SLEEP_MS(ms) SYS_SLEEP_CC(ms * 100000)

#if defined(__NETX90_MPW) /* netX90MPW */
#define DFLT_TXSLOPE_100 0x3 /* 4 ns (default is b111..3 ns) */
#define DFLT_BIAS10      0x20
#define DFLT_ADC         0x1
#define DFLT_BLWDAC      0x1
#define DFLT_RX_GAIN1    0x8821 // 0xa941
#define DFLT_RX_GAIN2    0xcba9 // 0xedcb
#define DFLT_RXITRIM     0x7

typedef struct PHY_CTRL_Ttag
{
  unsigned int uiMiimFreq : 1;
  unsigned int uiMiimPreamble : 1;
  unsigned int uiIBIAS10_valid : 1;
  unsigned int uiIBIAS10 : 6;
  unsigned int uiIBIAS100_valid : 1;
  unsigned int uiIBIAS100 : 6;
  unsigned int uiADC_valid : 1;
  unsigned int uiADC : 2;
  unsigned int uiBLWDAC_valid : 1;
  unsigned int uiBLWDAC : 2;
  unsigned int uiSQ_valid : 1;
  unsigned int uiSQ : 3;
  unsigned int uiPLL_valid : 1;
  unsigned int uiPLL : 2;
} PHY_CTRL_T;

#else /* netX90 */
#define DFLT_TXSLOPE_100               0x7 /* 3 ns */
#define DFLT_VMDAC_CTRL0            0xf1e3
#define DFLT_VMDAC_CTRL1            0x71e4
#define DFLT_PHYCFG_EXT         0xf8000800
#define DFLT_PHYCFG_GAIN_TABLE  0xcba98321
#define DFLT_PHYCFG_GAIN_THRSH1       1000
#define DFLT_PHYCFG_GAIN_THRSH2        750
#define DFLT_PHYCFG_GAIN_THRSH3        610
#define DFLT_PHYCFG_GAIN_THRSH4        570
#define DFLT_PHYCFG_GAIN_THRSH5        535
#define DFLT_PHYCFG_GAIN_THRSH6        460
#define DFLT_PHYCFG_GAIN_THRSH7        407

typedef struct PHY_CTRL_Ttag
{
  unsigned int uiMiimFreq : 1;
  unsigned int uiMiimPreamble : 1;
  unsigned int uiTXAMP10_valid : 1;
  unsigned int uiTXAMP10 : 6;
  unsigned int uiTXAMP100_valid : 1;
  unsigned int uiTXAMP100 : 6;
} PHY_CTRL_T;

#endif

/* Variables */
static PHY_CTRL_T s_atPhyCtrl[] =
{
  {MIIMU_MDCFREQ_HIGH, MIIMU_SUPPRESS_PREAMBLE, 0},
  {MIIMU_MDCFREQ_HIGH, MIIMU_SUPPRESS_PREAMBLE, 0},
};

HW_PTR_INT_PHY_CFG(s_ptIntPhyCfg)

/* Functions */

/*****************************************************************************/
/*! Initialize Internal PHY
* \description
*   Initialize internal PHY.
*   Note: This function is deprecated, do not use it in new software.
* \class 
*   INTPHY 
* \params
*   ulPhyCtrlInit [in]  PHY Control Value
* \return
*                                                                            */
/*****************************************************************************/

#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(INTPHY_Init)(void)
{
  s_atPhyCtrl[0].uiMiimFreq     = MIIMU_MDCFREQ_HIGH;
  s_atPhyCtrl[1].uiMiimFreq     = MIIMU_MDCFREQ_HIGH;
  s_atPhyCtrl[0].uiMiimPreamble = MIIMU_SUPPRESS_PREAMBLE;
  s_atPhyCtrl[1].uiMiimPreamble = MIIMU_SUPPRESS_PREAMBLE;

  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl = HW_DFLT_VAL(int_phy_cfg_phy_ctrl);

#if !defined(__NETX90_MPW)
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0         = DFLT_PHYCFG_EXT;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_gain_table0       = DFLT_PHYCFG_GAIN_TABLE;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_1     = DFLT_PHYCFG_GAIN_THRSH1;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_2     = DFLT_PHYCFG_GAIN_THRSH2;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_3     = DFLT_PHYCFG_GAIN_THRSH3;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_4     = DFLT_PHYCFG_GAIN_THRSH4;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_5     = DFLT_PHYCFG_GAIN_THRSH5;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_6     = DFLT_PHYCFG_GAIN_THRSH6;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_7     = DFLT_PHYCFG_GAIN_THRSH7;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_off_energy_level0 = HW_DFLT_VAL(int_phy_cfg_phy_off_energy_level0);
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_binit0            = HW_DFLT_VAL(int_phy_cfg_phy_binit0);

  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1         = DFLT_PHYCFG_EXT;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_gain_table1       = DFLT_PHYCFG_GAIN_TABLE;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_1     = DFLT_PHYCFG_GAIN_THRSH1;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_2     = DFLT_PHYCFG_GAIN_THRSH2;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_3     = DFLT_PHYCFG_GAIN_THRSH3;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_4     = DFLT_PHYCFG_GAIN_THRSH4;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_5     = DFLT_PHYCFG_GAIN_THRSH5;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_6     = DFLT_PHYCFG_GAIN_THRSH6;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_7     = DFLT_PHYCFG_GAIN_THRSH7;
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_off_energy_level1 = HW_DFLT_VAL(int_phy_cfg_phy_off_energy_level1);
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_binit1            = HW_DFLT_VAL(int_phy_cfg_phy_binit1);
#endif
}

/*****************************************************************************/
/*! Set PHY Control Register
* \description
*   Sets PHY Control Register
* \class 
*   INTPHY 
* \params
*   ptPhyCtrl          [in]  Pointer to PHY Control structure
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(INTPHY_SetPhyCtrl)( const INTPHY_CTRL_T* ptPhyCtrl )
{
  /* Note: address = 0 */
  uint32_t ulVal = 0;

  ulVal |= (ptPhyCtrl->uiPhyMode << HW_SRT(int_phy_cfg_phy_ctrl_phy0_mode));
  if( ptPhyCtrl->afPhyFxMode[0] ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy0_fxmode);
  if( ptPhyCtrl->afPhyAutoMdix[0] ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy0_automdix);
  if( ptPhyCtrl->afPhyEnable[0] ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy0_enable);

  ulVal |= (ptPhyCtrl->uiPhyMode << HW_SRT(int_phy_cfg_phy_ctrl_phy1_mode));
  if( ptPhyCtrl->afPhyFxMode[1] ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy1_fxmode);
  if( ptPhyCtrl->afPhyAutoMdix[1] ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy1_automdix);
  if( ptPhyCtrl->afPhyEnable[1] ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy1_enable);

  if( ptPhyCtrl->fPhyReset ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy_reset);

  /* write value */
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl = ulVal;
}


#if !defined(__NETX90_MPW)
/* Enable Auto-MDIX when AutoNeg is disabled (forced modes 100BT or 10BT ) */
void HALDEF(INTPHY_CorrectAutoMdix)(unsigned int uiPhy)
{
  unsigned int val;

  val = INTPHY_Read(uiPhy, REG_BASIC_CONTROL);

  /* AutoNeg disabled? */
  if( 0 == (val & MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE) )
  {
    /* 10Mbit? */
    if( 0 == (val & MSK_PHYREG_BASIC_CONTROL_SPEED_SELECT) )
    {
      /* fix MDIX bug in 10BT forced modes */
      val = INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_CONFIG);
      val |= 1U << 11; /* set force_dsp_init_state bit */
      INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_CONFIG, val);
    }
    else
    {
      /* fix MDIX bug in 100BT forced modes */
      INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A9_CONFIG, 1); /* enable squelch */
    }
  }
}
#endif

void HALDEF(INTPHY_Fixup)(unsigned int uiPhy)
{
#if defined(__NETX90_MPW)
  unsigned int val, cspare;
  unsigned int uiMiimPreamble = s_atPhyCtrl[uiPhy].uiMiimPreamble;

  s_atPhyCtrl[uiPhy].uiMiimPreamble = MIIMU_SEND_PREAMBLE;

  /* PLL register: This is to program the PLL divider value for reference clock of 25MHz. Default value was for reference freq of 24MHz */
  INTPHY_WriteBank(uiPhy, REG_BANK_BIST, 0x1c, 0x0000); /* FR_PLL_DIV0 */
  INTPHY_WriteBank(uiPhy, REG_BANK_BIST, 0x1d, 0x0280); /* FR_PLL_DIV1 */

  SLEEP_MS(1); /* 1ms */

  /*** DSP FixUp ***/

  /* A2_CONFIG */
  val = (DFLT_RXITRIM << 12); /* set RXITRIM */
  INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A2_CONFIG, val);

  /* A4_CONFIG */
  if( s_atPhyCtrl[uiPhy].uiSQ_valid != 0 )
  {
    unsigned int cbrxcmadj = s_atPhyCtrl[uiPhy].uiSQ;
    val = INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A4_CONFIG);
    val &= ~(0x7U << 4); /* clear CBRXCMADJ */
    val |= (cbrxcmadj << 4); /* set CBRXCMADJ */
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A4_CONFIG, val);
  }

  /* A5_CONFIG */
  val = INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A5_CONFIG);
  val &= ~(0x7U << 1); /* clear TX_SLOPE_100 */
  val |= (DFLT_TXSLOPE_100 << 1); /* set TX_SLOPE_100 */
  INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A5_CONFIG, val);

  /* A6_CONFIG */
  /* Note: PLLINTRATIO is global for both ports, we double the parameter for simplification */
  if( s_atPhyCtrl[uiPhy].uiPLL_valid )
  {
    unsigned int pllintratio = s_atPhyCtrl[uiPhy].uiPLL;
    val = INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A6_CONFIG);
    val &= ~(0x3U << 12); /* clear PLLINTRATIO */
    val |= (pllintratio << 12); /* set PLLINTRATIO */
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A6_CONFIG, val);
  }

  /* A7_CONFIG write only */
  /* set TXAMP */
  val = s_atPhyCtrl[uiPhy].uiIBIAS10_valid ? s_atPhyCtrl[uiPhy].uiIBIAS10 : DFLT_BIAS10;
  INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A7_CONFIG, val);

  /* A10_CONFIG write only */
  if( s_atPhyCtrl[uiPhy].uiIBIAS100_valid )
  {
    unsigned int txamp_100bt = s_atPhyCtrl[uiPhy].uiIBIAS100;
    val = txamp_100bt << 10;
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A10_CONFIG, val);
  }

  /* A11_CONFIG write only
   * - get Port1 out of low swing EE mode (set CSPARE1<6> = 0 )
   * - fix reset value of PSPARE0<7> (sometimes 1 after SW reset)
   */
  cspare = 0;
  if( uiPhy == 0 )
  {
    unsigned int uiIPTRN0 = s_atPhyCtrl[0].uiBLWDAC_valid ? s_atPhyCtrl[0].uiBLWDAC : DFLT_BLWDAC;
    unsigned int uiIPTRN1 = s_atPhyCtrl[1].uiBLWDAC_valid ? s_atPhyCtrl[1].uiBLWDAC : DFLT_BLWDAC;
    unsigned int uiADC0   = s_atPhyCtrl[0].uiADC_valid    ? s_atPhyCtrl[0].uiADC    : DFLT_ADC;
    unsigned int uiADC1   = s_atPhyCtrl[1].uiADC_valid    ? s_atPhyCtrl[1].uiADC    : DFLT_ADC;
    cspare |= uiIPTRN0 << 0;
    cspare |= uiADC0   << 2;
    cspare |= uiIPTRN1 << 4;
    cspare |= uiADC1   << 6;
  }
  else
  {
    cspare = 0x5;
  }
  val = cspare << 8;
  INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A11_CONFIG, val);

  /* configure RXGAIN profile */
  INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_GAIN1, DFLT_RX_GAIN1);
  INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_GAIN2, DFLT_RX_GAIN2);

  /*** DSP FixUp end ***/

  /* disable auto-MDIX because it is not working for netX90MPW, default to MDI */
  /* set Management Data Preamble Bypass = 1 */
  INTPHY_Write(uiPhy, 17, (1U << 10)); /* MODE CONTROL / STATUS */

  /* Termination control register: This is to set proper termination of 100ohm across the RX lines in MDI mode */
  INTPHY_WriteBank(uiPhy, REG_BANK_VMDAC, REG_VM_DAC_CONTROL0, 0x8003); /* VM_DAC_CONTROL0 */
  INTPHY_WriteBank(uiPhy, REG_BANK_VMDAC, REG_VM_DAC_CONTROL1, 0x7184); /* VM_DAC_CONTROL1 */

  /* now we can go on with suppressed preamble */
  s_atPhyCtrl[uiPhy].uiMiimPreamble = uiMiimPreamble;
#else
  unsigned int uiVal;

  /* A5_CONFIG */
  uiVal = INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A5_CONFIG);
  uiVal &= ~(0x7U << 1); /* clear TX_SLOPE_100 */
  uiVal |= (DFLT_TXSLOPE_100 << 1); /* set TX_SLOPE_100 */
  INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A5_CONFIG, uiVal);

  /* set tx_vd_amp_itrim = 0 */
  uiVal = INTPHY_ReadBank(uiPhy, REG_BANK_VMDAC, REG_MISC_PHY_CONTROL0);
  uiVal &= 0x9fff; /* clear tx_vd_amp_itrim bits [14:13] */
  INTPHY_WriteBank(uiPhy, REG_BANK_VMDAC, REG_MISC_PHY_CONTROL0, uiVal);

  /* Termination control register */
  INTPHY_WriteBank(uiPhy, REG_BANK_VMDAC, REG_VM_DAC_CONTROL0, DFLT_VMDAC_CTRL0);
  INTPHY_WriteBank(uiPhy, REG_BANK_VMDAC, REG_VM_DAC_CONTROL1, DFLT_VMDAC_CTRL1);

  INTPHY_CorrectAutoMdix(uiPhy);
#endif

  /* clear IRQs */
  /*lint -e(534) we ignore the register value because we just want
   * the side effect of interrupt confirmation */
  INTPHY_Read(uiPhy, 30);
}

/*****************************************************************************/
/*! Reset PHY
* \description
*   Sets PHY Control Register
* \class
*   INTPHY
* \params
*   ptPhyCtrl          [in]  Pointer to PHY Control structure
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(INTPHY_Reset) ( const INTPHY_CTRL_T* ptPhyCtrl )
{
  INTPHY_CTRL_T tPhyCtrl;
  unsigned int uiPort;
#if !defined(__NETX90_MPW)
  uint32_t ulVal;
#endif

  tPhyCtrl.fPhySimBypass = false;
  tPhyCtrl.fPhyReset = true;

  /* reset INT_PHY */
  tPhyCtrl.uiPhyMode = 0;
  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    tPhyCtrl.afPhyFxMode[uiPort]   = false;
    tPhyCtrl.afPhyAutoMdix[uiPort] = false;
    tPhyCtrl.afPhyEnable[uiPort]   = false;
  }
  INTPHY_SetPhyCtrl(&tPhyCtrl);

  /* Wait 10ms for PHY reset */
  SLEEP_MS(10);

  /* initialize INTPHY to desired state */
  tPhyCtrl.uiPhyMode = ptPhyCtrl->uiPhyMode;
  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    tPhyCtrl.afPhyFxMode[uiPort]   = ptPhyCtrl->afPhyFxMode[uiPort];
    tPhyCtrl.afPhyAutoMdix[uiPort] = ptPhyCtrl->afPhyAutoMdix[uiPort];
    tPhyCtrl.afPhyEnable[uiPort]   = ptPhyCtrl->afPhyEnable[uiPort];
  }

  /* set configuration with reset set */
  tPhyCtrl.fPhyReset = true;
  INTPHY_SetPhyCtrl(&tPhyCtrl);

  /* Wait 1ms for PHY reset */
  SLEEP_MS(1);

  if( ptPhyCtrl->fPhyReset )
  {
    return;
  }

#if !defined(__NETX90_MPW)
  /* set trimming parameters port 0 */
  ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0;
  if( s_atPhyCtrl[0].uiTXAMP10_valid )
  {
    ulVal &= ~HW_MSK(int_phy_cfg_phy_ctrl_ext0_txamp_10);
    ulVal |= s_atPhyCtrl[0].uiTXAMP10 << HW_SRT(int_phy_cfg_phy_ctrl_ext0_txamp_10);
  }
  if( s_atPhyCtrl[0].uiTXAMP100_valid )
  {
    ulVal &= ~HW_MSK(int_phy_cfg_phy_ctrl_ext0_txamp_100);
    ulVal |= s_atPhyCtrl[0].uiTXAMP100 << HW_SRT(int_phy_cfg_phy_ctrl_ext0_txamp_100);
  }
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0 = ulVal;

  /* set trimming parameters port 1 */
  ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1;
  if( s_atPhyCtrl[1].uiTXAMP10_valid )
  {
    ulVal &= ~HW_MSK(int_phy_cfg_phy_ctrl_ext1_txamp_10);
    ulVal |= s_atPhyCtrl[1].uiTXAMP10 << HW_SRT(int_phy_cfg_phy_ctrl_ext1_txamp_10);
  }
  if( s_atPhyCtrl[1].uiTXAMP100_valid )
  {
    ulVal &= ~HW_MSK(int_phy_cfg_phy_ctrl_ext1_txamp_100);
    ulVal |= s_atPhyCtrl[1].uiTXAMP100 << HW_SRT(int_phy_cfg_phy_ctrl_ext1_txamp_100);
  }
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1 = ulVal;
#endif

  /* set configuration with reset gone */
  tPhyCtrl.fPhyReset = false;
  INTPHY_SetPhyCtrl(&tPhyCtrl);

  /* Wait 1ms for PHY reset extension */
  SLEEP_MS(100);

  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    INTPHY_Fixup(uiPort);
  }

  /* clear IRQs */
  s_ptIntPhyCfg->ulInt_phy_cfg_irq_raw = 0x3;
  s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_set = 0x3;

  if( ( tPhyCtrl.uiPhyMode == INTPHY_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN )
    ||( tPhyCtrl.uiPhyMode == INTPHY_MODE_REPEATER_AUTONEG_100BASE_TX_HD_ADV_CRSRX )
    ||( tPhyCtrl.uiPhyMode == INTPHY_MODE_100BASE_TX_HD_ADV_AUTONEG_CRSRXTX )
    )
  {
    /* restart AutoNeg */
    INTPHY_Write(INTPHY_ADDR0, 0, 0x1200);
    INTPHY_Write(INTPHY_ADDR1, 0, 0x1200);
  }
}

/*****************************************************************************/
/*! Get INTPHY Parameter
* \description
*   Get any INTPHY Parameter
* \class
*   INTPHY
* \params
*   ePrmID      [in]    Wanted Parameter
* \return
*   uint32_t                                                                 */
/*****************************************************************************/
uint32_t HALDEF(INTPHY_PrmGet) ( INTPHY_PRM_E ePrmID )
{
  uint32_t ulVal = 0;

  /* Get the selected parameter */
  switch(ePrmID)
  {
    case INTPHY_PRM_PHY_CTRL:       ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl; break;
#if !defined(__NETX90_MPW)
    case INTPHY_PRM_PHY_CTRL_EXT0:   ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0; break;
    case INTPHY_PRM_PHY_CTRL_EXT1:   ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1; break;
    case INTPHY_PRM_PHY_GAIN_TABLE0: ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_gain_table0; break;
    case INTPHY_PRM_PHY_GAIN_TABLE1: ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_gain_table1; break;
#endif
    case INTPHY_PRM_STATUS:         ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_status; break;
    case INTPHY_PRM_I2C_WRITE:      ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write; break;
    case INTPHY_PRM_I2C_READ:       ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_i2c_read; break;
    case INTPHY_PRM_DEBUG:          ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_debug;  break;

    case INTPHY_PRM_MIIM_PREAMBLE_0: ulVal = s_atPhyCtrl[0].uiMiimPreamble; break;
    case INTPHY_PRM_MIIM_PREAMBLE_1: ulVal = s_atPhyCtrl[1].uiMiimPreamble; break;
    case INTPHY_PRM_MIIM_FREQ_0:     ulVal = s_atPhyCtrl[0].uiMiimFreq; break;
    case INTPHY_PRM_MIIM_FREQ_1:     ulVal = s_atPhyCtrl[1].uiMiimFreq; break;

    case INTPHY_PRM_IRQ_RAW:        ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_irq_raw; break;
    case INTPHY_PRM_IRQ_MASKED:     ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_irq_masked; break;
    case INTPHY_PRM_IRQ_MASK_SET:   ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_set; break;
    case INTPHY_PRM_IRQ_MASK_RESET: ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_reset; break;

    /* Default */
    default: break;
  }

  return ulVal;
}



/*****************************************************************************/
/*! Set INTPHY Parameter
* \description
*   Set any INTPHY Parameter
* \class
*   INTPHY
* \params
*   ePrmID      [in]   Wanted Parameter
*   pulStatus   [in]   Parameter
* \return
*   0 on success
*   1 on failure                                                             */
/*****************************************************************************/
int HALDEF(INTPHY_PrmSet) ( INTPHY_PRM_E ePrmID, uint32_t ulPrmVal )
{
  int iErr = 0;

  /* Get the selected parameter */
  switch(ePrmID)
  {
    case INTPHY_PRM_PHY_CTRL:          s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl = ulPrmVal; break;

#if !defined(__NETX90_MPW)
    case INTPHY_PRM_PHY_CTRL_EXT0:     s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0     = ulPrmVal; break;
    case INTPHY_PRM_PHY_GAIN_TABLE0:   s_ptIntPhyCfg->ulInt_phy_cfg_phy_gain_table0   = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH0_1: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_1 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH0_2: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_2 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH0_3: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_3 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH0_4: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_4 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH0_5: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_5 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH0_6: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_6 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH0_7: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh0_7 = ulPrmVal; break;
    case INTPHY_PRM_OFF_ENERGY_LEVEL0: s_ptIntPhyCfg->ulInt_phy_cfg_phy_off_energy_level0 = ulPrmVal; break;
    case INTPHY_PRM_BINIT0:            s_ptIntPhyCfg->ulInt_phy_cfg_phy_binit0        = ulPrmVal; break;

    case INTPHY_PRM_PHY_CTRL_EXT1:     s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1     = ulPrmVal; break;
    case INTPHY_PRM_PHY_GAIN_TABLE1:   s_ptIntPhyCfg->ulInt_phy_cfg_phy_gain_table1   = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH1_1: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_1 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH1_2: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_2 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH1_3: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_3 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH1_4: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_4 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH1_5: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_5 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH1_6: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_6 = ulPrmVal; break;
    case INTPHY_PRM_PHY_AMP_THRESH1_7: s_ptIntPhyCfg->ulInt_phy_cfg_phy_amp_thresh1_7 = ulPrmVal; break;
    case INTPHY_PRM_OFF_ENERGY_LEVEL1: s_ptIntPhyCfg->ulInt_phy_cfg_phy_off_energy_level1 = ulPrmVal; break;
    case INTPHY_PRM_BINIT1:            s_ptIntPhyCfg->ulInt_phy_cfg_phy_binit1 = ulPrmVal; break;
#endif

    case INTPHY_PRM_I2C_WRITE:         s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulPrmVal; break;
    case INTPHY_PRM_I2C_READ:          s_ptIntPhyCfg->ulInt_phy_cfg_i2c_read = ulPrmVal; break;
    case INTPHY_PRM_DEBUG:             s_ptIntPhyCfg->ulInt_phy_cfg_debug = ulPrmVal; break;

    case INTPHY_PRM_IRQ_RAW:           s_ptIntPhyCfg->ulInt_phy_cfg_irq_raw = ulPrmVal; break;
    case INTPHY_PRM_IRQ_MASK_SET:      s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_set = ulPrmVal; break;
    case INTPHY_PRM_IRQ_MASK_RESET:    s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_reset = ulPrmVal; break;

    case INTPHY_PRM_MIIM_PREAMBLE_0:   s_atPhyCtrl[0].uiMiimPreamble   = 1U & ulPrmVal; break;
    case INTPHY_PRM_MIIM_PREAMBLE_1:   s_atPhyCtrl[1].uiMiimPreamble   = 1U & ulPrmVal; break;
    case INTPHY_PRM_MIIM_FREQ_0:       s_atPhyCtrl[0].uiMiimFreq       = 1U & ulPrmVal; break;
    case INTPHY_PRM_MIIM_FREQ_1:       s_atPhyCtrl[1].uiMiimFreq       = 1U & ulPrmVal; break;
#if defined(__NETX90_MPW)
    case INTPHY_PRM_TRIM_ADC_0:
      s_atPhyCtrl[0].uiADC_valid      = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[0].uiADC            = 0x03U & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_ADC_1:
      s_atPhyCtrl[1].uiADC_valid      = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[1].uiADC            = 0x03U & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_BLWDAC_0:
      s_atPhyCtrl[0].uiBLWDAC_valid   = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[0].uiBLWDAC         = 0x03U & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_BLWDAC_1:
      s_atPhyCtrl[1].uiBLWDAC_valid   = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[1].uiBLWDAC         = 0x03U & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_SQ_0:
      s_atPhyCtrl[0].uiSQ_valid       = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[0].uiSQ             = 0x07U & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_SQ_1:
      s_atPhyCtrl[1].uiSQ_valid       = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[1].uiSQ             = 0x07U & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_IBIAS10_0:
      s_atPhyCtrl[0].uiIBIAS10_valid  = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[0].uiIBIAS10        = 0x3fU & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_IBIAS10_1:
      s_atPhyCtrl[1].uiIBIAS10_valid  = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[1].uiIBIAS10        = 0x3fU & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_IBIAS100_0:
      s_atPhyCtrl[0].uiIBIAS100_valid = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[0].uiIBIAS100       = 0x3fU & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_IBIAS100_1:
      s_atPhyCtrl[1].uiIBIAS100_valid = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[1].uiIBIAS100       = 0x3fU & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_PLL:
      s_atPhyCtrl[0].uiPLL_valid      = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[0].uiPLL            = 0x3U & ulPrmVal;
      s_atPhyCtrl[1].uiPLL_valid      = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[1].uiPLL            = 0x3U & ulPrmVal;
      break;
#else
    case INTPHY_PRM_TRIM_TXAMP10_0:
      s_atPhyCtrl[0].uiTXAMP10_valid  = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[0].uiTXAMP10        = 0x3fU & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_TXAMP10_1:
      s_atPhyCtrl[1].uiTXAMP10_valid  = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[1].uiTXAMP10        = 0x3fU & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_TXAMP100_0:
      s_atPhyCtrl[0].uiTXAMP100_valid  = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[0].uiTXAMP100        = 0x3fU & ulPrmVal;
      break;
    case INTPHY_PRM_TRIM_TXAMP100_1:
      s_atPhyCtrl[1].uiTXAMP100_valid  = 1U & (~ulPrmVal >> 7);
      s_atPhyCtrl[1].uiTXAMP100        = 0x3fU & ulPrmVal;
      break;
#endif

    /* Default */
    default:
      iErr = 1;
      break;
  } /* end switch */

  return iErr;
}

/*****************************************************************************/
/*! INTPHY I2C Select/DeSelect
* \description
*   Write INTPHY register via I2C bus interface
* \class
*   INTPHY
* \params
*   todo
* \return
*                                                                            */
/*****************************************************************************/
static void HALDEF(INTPHY_I2cSelect) ( unsigned int uiPhy, bool fSelect )
{
  uint32_t ulVal = 0;

  if(fSelect)
  {
    if(0 == uiPhy) {
      ulVal |= HW_MSK(int_phy_cfg_i2c_write_sel_phy0);
    }
    if(1 == uiPhy) {
      ulVal |= HW_MSK(int_phy_cfg_i2c_write_sel_phy1);
    }
  }

  /* write value */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulVal;
}

/*****************************************************************************/
/*! INTPHY I2C Write Register
* \description
*   Write INTPHY register via I2C bus interface
* \class
*   INTPHY
* \params
*   todo
* \return
*                                                                            */
/*****************************************************************************/
static void HALDEF(INTPHY_I2cWriteReg) ( unsigned int uiAddr, uint16_t usWriteData )
{
  uint32_t ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write;

  ulVal |= ((uint32_t)uiAddr << HW_SRT(int_phy_cfg_i2c_write_addr));
  ulVal |= ((uint32_t)usWriteData << HW_SRT(int_phy_cfg_i2c_write_wr_data));

  /* set address / data and wait 100 ns */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulVal;
  SYS_SLEEP_CC(10);

  /* set write request and wait 100 ns */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulVal | HW_MSK(int_phy_cfg_i2c_write_wr_req);
  SYS_SLEEP_CC(10);

  /* release write request and wait 100 ns */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulVal;
  SYS_SLEEP_CC(10);

  /* release address / data and wait 100 ns */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = (ulVal & ~(HW_MSK(int_phy_cfg_i2c_write_addr) | HW_MSK(int_phy_cfg_i2c_write_wr_data)) );
  SYS_SLEEP_CC(10);
}

/*****************************************************************************/
/*! INTPHY I2C Read Register
* \description
*   Read INTPHY register via I2C bus interface
* \class
*   INTPHY
* \params
*   todo
* \return
*   uiRdDataValid                                                            */
/*****************************************************************************/
static uint16_t HALDEF(INTPHY_I2cReadReg) ( unsigned int uiAddr )
{
  uint32_t ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write;
  uint32_t ulRdData = 0;

  ulVal |= ((uint32_t)uiAddr << HW_SRT(int_phy_cfg_i2c_write_addr));

  /* set address and wait 100 ns */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulVal;
  SYS_SLEEP_CC(10);

  /* set read request and wait 100 ns */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulVal | HW_MSK(int_phy_cfg_i2c_write_rd_req);
  SYS_SLEEP_CC(10);

  /* poll for ready */
  do {
    ulRdData = s_ptIntPhyCfg->ulInt_phy_cfg_i2c_read;
  } while( 0 == (ulRdData & HW_MSK(int_phy_cfg_i2c_read_rd_data_valid) ));

  /* release read request and wait 100 ns */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulVal;
  SYS_SLEEP_CC(10);

  /* release address and wait 100 ns */
  s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = (ulVal & ~(HW_MSK(int_phy_cfg_i2c_write_addr)) );
  SYS_SLEEP_CC(10);

  return (uint16_t)(ulRdData & HW_MSK(int_phy_cfg_i2c_read_rd_data));
}


/*****************************************************************************/
/*! INTPHY I2C Write
* \description
*   Write INTPHY register via I2C bus interface
* \class
*   INTPHY
* \params
*   todo
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(INTPHY_I2cWrite) ( unsigned int uiPhy, unsigned int uiAddr, uint16_t usWriteData )
{
  INTPHY_I2cSelect(uiPhy, true);
  INTPHY_I2cWriteReg(uiAddr, usWriteData);
  INTPHY_I2cSelect(uiPhy, false);
}

/*****************************************************************************/
/*! INTPHY I2C Read
* \description
*   Read INTPHY register via I2C bus interface
* \class
*   INTPHY
* \params
*   todo
* \return
*   uiRdDataValid                                                            */
/*****************************************************************************/
uint16_t HALDEF(INTPHY_I2cRead) ( unsigned int uiPhy, unsigned int uiAddr )
{
  uint16_t usVal;

  INTPHY_I2cSelect(uiPhy, true);
  usVal = INTPHY_I2cReadReg(uiAddr);
  INTPHY_I2cSelect(uiPhy, false);

  return usVal;
}

/*****************************************************************************/
/*! INTPHY Write Bank
* \description
*   Write INTPHY register of a special bank via I2C bus interface
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiBank              [in]  Bank number
*   uiAddr              [in]  Register Address
*   uiVal               [in]  Register value
* \return
*   0 on success                                                             */
/*****************************************************************************/
void HALDEF(INTPHY_I2cWriteBank) ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal )
{
  unsigned int uiReg20Val = MSK_TSTCNTL_WRITE | (uiBank << 11) | (uiAddr << 0);

  /* select PHY */
  INTPHY_I2cSelect(uiPhy, true);

  if( uiBank == REG_BANK_DSP )
  {
    /* Note: entering test mode not required when access via I2C */
    INTPHY_I2cWriteReg(23, (uint16_t)uiVal);
    INTPHY_I2cWriteReg(20, (uint16_t)(uiReg20Val | MSK_TSTCNTL_TEST_MODE) );
  }
  else
  {
    INTPHY_I2cWriteReg(23, (uint16_t)uiVal);
    INTPHY_I2cWriteReg(20, (uint16_t)uiReg20Val);
  }

  /* leave indirect address mode */
  INTPHY_I2cWriteReg(20, 0x0000);

  /* de-select PHY */
  INTPHY_I2cSelect(uiPhy, false);
}

/*****************************************************************************/
/*! INTPHY Read Bank
* \description
*   Read INTPHY register of a special bank via I2C bus interface
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiBank              [in]  Bank number
*   uiAddr              [in]  Register Address
* \return
*   Register value                                                           */
/*****************************************************************************/
uint32_t HALDEF(INTPHY_I2cReadBank) ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr )
{
  uint32_t ulVal;
  unsigned int uiReg20Val = MSK_TSTCNTL_READ | (uiBank << 11) | (uiAddr << 5);

  /* select PHY */
  INTPHY_I2cSelect(uiPhy, true);

  if( uiBank == REG_BANK_DSP )
  {
    /* Note: entering test mode not required when access via I2C */
    INTPHY_I2cWriteReg(20, (uint16_t)(uiReg20Val | MSK_TSTCNTL_TEST_MODE) );

    /* read value from TST_READ1/2 */
    ulVal  = INTPHY_I2cReadReg(21);
    ulVal |= ((uint32_t)INTPHY_I2cReadReg(22)) << 16;
  }
  else
  {
#if defined(__NETX90_MPW)
    uiReg20Val |= uiAddr; /* bug: also set write address (required when reading VM_DAC_CONTROL1) */
#endif
    INTPHY_I2cWriteReg(20, (uint16_t)uiReg20Val );

    /* read value from TST_READ1 */
    ulVal = INTPHY_I2cReadReg(21);
  }

  /* leave indirect address mode */
  INTPHY_I2cWriteReg(20, 0x0000);

  /* de-select PHY */
  INTPHY_I2cSelect(uiPhy, false);

  return ulVal;
}

/*****************************************************************************/
/*! INTPHY Read Register
* \description
*   Read INTPHY register of the default bank
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiAddr              [in]  Register Address
* \return
*   Register value                                                           */
/*****************************************************************************/
unsigned int HALDEF(INTPHY_Read)(unsigned int uiPhy, unsigned int uiAddr)
{
  uint16_t usVal = 0;
  /*lint -e(534) We ignore the result code for sake of API simplicity */
  MIIMU_ReadPhyReg(s_atPhyCtrl[uiPhy].uiMiimPreamble, s_atPhyCtrl[uiPhy].uiMiimFreq, MIIMU_RTA_1_BITS, uiPhy, uiAddr, &usVal, 0);
  return (unsigned int)usVal;
}

/*****************************************************************************/
/*! INTPHY Write Register
* \description
*   Write INTPHY register of the default bank
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiAddr              [in]  Register Address
*   uiVal               [in]  Register value
* \return
*   0 on success                                                             */
/*****************************************************************************/
void HALDEF(INTPHY_Write)(unsigned int uiPhy, unsigned int uiAddr, unsigned int uiVal)
{
  /*lint -e(534) We ignore the result code for sake of API simplicity */
  MIIMU_WritePhyReg(s_atPhyCtrl[uiPhy].uiMiimPreamble, s_atPhyCtrl[uiPhy].uiMiimFreq, MIIMU_RTA_1_BITS, uiPhy, uiAddr, (uint16_t)uiVal, 0);
}

/*****************************************************************************/
/*! INTPHY Read Bank
* \description
*   Read INTPHY register of a special bank
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiBank              [in]  Bank number
*   uiAddr              [in]  Register Address
* \return
*   Register value                                                           */
/*****************************************************************************/
uint32_t HALDEF(INTPHY_ReadBank) ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr )
{
  uint32_t ulVal;
  unsigned int uiReg20Val = MSK_TSTCNTL_READ | (uiBank << 11) | (uiAddr << 5);

  if( uiBank == REG_BANK_DSP )
  {
#if defined(__NETX90_MPW)
    /* enter test mode */
    INTPHY_Write(uiPhy, 20, MSK_TSTCNTL_TEST_MODE);
    INTPHY_Write(uiPhy, 20, 0x0000);
    INTPHY_Write(uiPhy, 20, MSK_TSTCNTL_TEST_MODE);
#endif
    /* make sure that byp_tstmode_en is set due we do not do test mode procedure (not netx90_mpw) */
    INTPHY_Write(uiPhy, 20, uiReg20Val | MSK_TSTCNTL_TEST_MODE);

    /* read value from TST_READ1/2 */
    ulVal  = INTPHY_Read(uiPhy, 21);
    ulVal |= ((uint32_t)INTPHY_Read(uiPhy, 22)) << 16;
  }
  else
  {
#if defined(__NETX90_MPW)
    uiReg20Val |= uiAddr; /* bug: also set write address (required when reading VM_DAC_CONTROL1) */
#endif
    INTPHY_Write(uiPhy, 20, uiReg20Val);

    /* read value from TST_READ1 */
    ulVal = INTPHY_Read(uiPhy, 21);
  }

  /* leave indirect address mode */
  INTPHY_Write(uiPhy, 20, 0x0000);

  return ulVal;
}

/*****************************************************************************/
/*! INTPHY Write Bank
* \description
*   Write INTPHY register of a special bank
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiBank              [in]  Bank number
*   uiAddr              [in]  Register Address
*   uiVal               [in]  Register value
* \return
*   0 on success                                                             */
/*****************************************************************************/
void HALDEF(INTPHY_WriteBank) ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal )
{
  unsigned int uiReg20Val = MSK_TSTCNTL_WRITE | (uiBank << 11) | (uiAddr << 0);

  if( uiBank == REG_BANK_DSP )
  {
#if defined(__NETX90_MPW)
    /* enter test mode */
    INTPHY_Write(uiPhy, 20, MSK_TSTCNTL_TEST_MODE);
    INTPHY_Write(uiPhy, 20, 0x0000);
    INTPHY_Write(uiPhy, 20, MSK_TSTCNTL_TEST_MODE);
#endif

    /* make sure that byp_tstmode_en is set due we do not do test mode procedure (not netx90_mpw) */
    INTPHY_Write(uiPhy, 23, uiVal);
    INTPHY_Write(uiPhy, 20, uiReg20Val | MSK_TSTCNTL_TEST_MODE);
  }
  else
  {
    INTPHY_Write(uiPhy, 23, uiVal);
    INTPHY_Write(uiPhy, 20, uiReg20Val);
  }

  /* leave indirect address mode */
  INTPHY_Write(uiPhy, 20, 0x0000);
}
