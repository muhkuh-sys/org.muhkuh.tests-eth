/* Includes */
#include "hal_intphy.h"
#include "hal_phyctrl.h"
#include "hw_defines.h"
#include <string.h>

/* Definitions */
#define INTPHY_NX90_REV0_SUPPORT

#define MSK_TSTCNTL_READ                           0x8000
#define MSK_TSTCNTL_WRITE                          0x4000
#define MSK_TSTCNTL_TEST_MODE                      0x0400

#define INTPHY_PHYREG_PHY_IDENTIFIER2_ID_MODEL     0x1490 /* ID/model for netX90 */

/* bit masks for register 17 (MODE_CONTROL_STATUS) */
#define MSK_PHYREG_MODE_CONTROL_STATUS_AUTOMDIX_EN 0x0080
#define MSK_PHYREG_MODE_CONTROL_STATUS_MDI_MODE    0x0040
#define MSK_PHYREG_MODE_CONTROL_WRITE_AS_ZERO      0xd022

/* bit masks for register DSP.CONFIG */
#define MSK_PHYREG_DSP_BANK_CONFIG_FORCE_INIT_STATE  0x0800

/* bit masks for register DSP.A9CFG */
#define MSK_PHYREG_DSP_BANK_A9CFG_SQ_SRC 0x0001


#define SLEEP_MS(ms) SYS_SLEEP_CC(ms * 100000)

#define REG_BFSET(reg, bf, val, fail) if((val) > HW_MSK(bf)) {fail} else {reg = ((reg) & ~HW_MSK(bf)) | ((val) << HW_SRT(bf));}

#define MSK_int_phy_cfg_phy_ctrl_phy_clk_freq (1UL << 3)

#define DFLT_MODE_CONTROL_STATUS 0x0480 /* AutoMDIX_en=1, MDPREBP=1 */
#define DFLT_DCBLW               0x0454 /* re-configure BLW_FILT_MU=4 instead of 3, BLW_AVG_MU=0 instead of 1 */
#define DFLT_POWER_DOWN_CONTROL  0x0060 /* used in macro VAL_POWER_DOWN_CONTROL */
#define DFLT_DP_BASE_QTH         0x00ff /* vcnt_th = 255 to avoid re-measuring energy */
#define DFLT_CONFIG              0x0038 /* used for MDIX correction in 10BT */
#define DFLT_A3_CONFIG           0x7500 /* DACSTRBINV=1 (only for EPHY Rev <3) */
#define DFLT_A5_CONFIG           0x840c /* TXSLOPE_100=6 (only for EPHY Rev <3) */
#define DFLT_A5_CONFIG_REV3      0x8402 /* TXSLOPE_100=1 (only for EPHY Rev 3) */
#define DFLT_A9_CONFIG           0x0000 /* used for MDIX correction in 100BT */
#define DFLT_A11_CONFIG          0x5400 /* BLWDAC base Current trimming = 10uA instead of 12.5 uA (only for EPHY Rev <3) */
#define DFLT_VM_DAC_CONTROL1     0x7187 /* set tx_vdvbadj = 3, tx_10bt_vd_en=1 (only for EPHY Rev <3) */
#define DFLT_MISC_PHY_CONTROL0   0x0a08 /* set tx_vd_amp_itrim = 0 (only for EPHY Rev <3) */

#define VAL_POWER_DOWN_CONTROL(port) (DFLT_POWER_DOWN_CONTROL | (port << 4))

/* Since netX90 revision 1 we have to set phy_clk_freq
 * For netX90 revision 0 this changes the PHY address to 0x10
 * so we have to fix the PHY address after reset.
 * We do not check the netx_version register because this does not work when using a debugger.
 */
#define DFLT_PHYCFG_CTRL        (HW_DFLT_VAL(int_phy_cfg_phy_ctrl) | MSK_int_phy_cfg_phy_ctrl_phy_clk_freq)

#define DFLT_PHYCFG_EXT         0xf8000830
#define DFLT_PHYCFG_GAIN_TABLE  0xcba98321
#define DFLT_PHYCFG_GAIN_THRSH1       1100
#define DFLT_PHYCFG_GAIN_THRSH2        950
#define DFLT_PHYCFG_GAIN_THRSH3        800
#define DFLT_PHYCFG_GAIN_THRSH4        610
#define DFLT_PHYCFG_GAIN_THRSH5        580
#define DFLT_PHYCFG_GAIN_THRSH6        510
#define DFLT_PHYCFG_GAIN_THRSH7        450

#define INTPHY_PHYCTRL_INST 0

/* Variables */
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
void INTPHY_Init(void)
{
  unsigned int uiPort;

  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort )
  {
    /* initialize PHY management interface (MIIM) */
    PHYCTRL_Init(uiPort);

    /* suppress MIIM preamble */
    PHYCTRL_PrmSet(uiPort, PHYCTRL_PRM_MIIM_PREAMBLE, 0);

    /* use high speed MDC */
    PHYCTRL_PrmSet(uiPort, PHYCTRL_PRM_MIIM_MDC_FAST, 1);
  }

  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl              = DFLT_PHYCFG_CTRL;

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
void INTPHY_SetPhyCtrl( const INTPHY_CTRL_T* ptPhyCtrl )
{
  /* Note: address = 0 */
  uint32_t ulVal = MSK_int_phy_cfg_phy_ctrl_phy_clk_freq;

  ulVal |= (ptPhyCtrl->uiPhyMode << HW_SRT(int_phy_cfg_phy_ctrl_phy0_mode));
  if( ptPhyCtrl->afPhyFxMode[0] )   ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy0_fxmode);
  if( ptPhyCtrl->afPhyAutoMdix[0] ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy0_automdix);
  if( ptPhyCtrl->afPhyEnable[0] )   ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy0_enable);

  ulVal |= (ptPhyCtrl->uiPhyMode << HW_SRT(int_phy_cfg_phy_ctrl_phy1_mode));
  if( ptPhyCtrl->afPhyFxMode[1] )   ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy1_fxmode);
  if( ptPhyCtrl->afPhyAutoMdix[1] ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy1_automdix);
  if( ptPhyCtrl->afPhyEnable[1] )   ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy1_enable);

  if( ptPhyCtrl->fPhyReset ) ulVal |= HW_MSK(int_phy_cfg_phy_ctrl_phy_reset);

  /* write value */
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl = ulVal;
}

/*****************************************************************************/
/*! INTPHY Correct Auto-MDIX
* \description
*   Fix the PHY specific registers for correct Auto-MDI/MDI-X operation.
*   The mode of operation is passed into the function explicitly.
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiBasicCtrl         [in]  Current value of PHY register 0 (BASIC_CTRL)   */
/*****************************************************************************/
static void INTPHY_FixupAutoMdix(unsigned int uiPhy, unsigned int uiBasicCtrl)
{
  /* Enable Auto-MDIX when AutoNeg is disabled (forced modes 100BT or 10BT ) */

  /* AutoNeg disabled? */
  if( 0 == (uiBasicCtrl & MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE) )
  {
    /* 10Mbit? */
    if( 0 == (uiBasicCtrl & MSK_PHYREG_BASIC_CONTROL_SPEED_SELECT) )
    {
      /* fix MDIX bug in 10BT forced modes */
      INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_CONFIG, DFLT_CONFIG | MSK_PHYREG_DSP_BANK_CONFIG_FORCE_INIT_STATE);
    }
    else
    {
      /* fix MDIX bug in 100BT forced modes */
      INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A9_CONFIG, DFLT_A9_CONFIG | MSK_PHYREG_DSP_BANK_A9CFG_SQ_SRC); /* enable squelch */
    }
  }
}

/*****************************************************************************/
/*! INTPHY Correct Auto-MDIX
* \description
*   Fix the PHY specific registers for correct Auto-MDI/MDI-X operation.
*   The mode of operation is detected automatically via PHY register read.
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address                              */
/*****************************************************************************/
void INTPHY_CorrectAutoMdix(unsigned int uiPhy)
{
/* Enable Auto-MDIX when AutoNeg is disabled (forced modes 100BT or 10BT ) */
  INTPHY_FixupAutoMdix(uiPhy, INTPHY_Read(uiPhy, ADR_PHYREG_BASIC_CONTROL));
}

/*****************************************************************************/
/*! INTPHY Fix-up Mode
* \description
*   Fix the PHY specific registers for correct PHY operation
*   after exit of PowerDown mode or reset.
*   The mode of operation is passed into the function explicitly.
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiBasicCtrl         [in]  Current value of PHY register 0 (BASIC_CTRL)   */
/*****************************************************************************/
static void INTPHY_FixupMode(unsigned int uiPhy, unsigned int uiBasicCtrl)
{
  unsigned int uiPhyRev;

  uiPhyRev = INTPHY_Read(uiPhy, ADR_PHYREG_PHY_IDENTIFIER2) & MSK_PHYREG_PHY_IDENTIFIER2_REVISION;

  if( uiPhyRev < 3 )
  {
    /* receiver fix-ups */
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_DCBLW, DFLT_DCBLW);
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_DP_BASE_QTH, DFLT_DP_BASE_QTH);
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A3_CONFIG, DFLT_A3_CONFIG);
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A11_CONFIG, DFLT_A11_CONFIG);

    /* transmitter fix-ups */
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A5_CONFIG, DFLT_A5_CONFIG);
    INTPHY_WriteBank(uiPhy, REG_BANK_VMDAC, REG_VM_DAC_CONTROL1, DFLT_VM_DAC_CONTROL1);
    INTPHY_WriteBank(uiPhy, REG_BANK_VMDAC, REG_MISC_PHY_CONTROL0, DFLT_MISC_PHY_CONTROL0);
  }
  else
  {
    /* receiver fix-ups */
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_DCBLW, DFLT_DCBLW);
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_DP_BASE_QTH, DFLT_DP_BASE_QTH);

    /* transmitter fix-ups */
    INTPHY_WriteBank(uiPhy, REG_BANK_DSP, REG_A5_CONFIG, DFLT_A5_CONFIG_REV3);
  }

  /* AFE common control select */
  INTPHY_Write(uiPhy, REG_POWER_DOWN_CONTROL, VAL_POWER_DOWN_CONTROL(uiPhy));

  /* Auto-MDIX fix-up */
  INTPHY_FixupAutoMdix(uiPhy, uiBasicCtrl);

  /* clear IRQs */
  /*lint -e(534) we ignore the register value because we just want
   * the side effect of interrupt confirmation */
  INTPHY_Read(uiPhy, 30);
}

/*****************************************************************************/
/*! INTPHY Fix-up
* \description
*   Fix the PHY specific registers for correct PHY operation
*   after exit of PowerDown mode or reset.
*   The mode of operation is detected automatically via PHY register read.
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address                              */
/*****************************************************************************/
void INTPHY_Fixup(unsigned int uiPhy)
{
  INTPHY_FixupMode(uiPhy, INTPHY_Read(uiPhy, ADR_PHYREG_BASIC_CONTROL));
}

/*****************************************************************************/
/*! INTPHY Check Fix-up
* \description
*   Validate the PHY specific register fixes
*   after exit of PowerDown mode or reset.
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
* \return
*   0: Fixup validation ok
*   1: Errors found                                                          */
/*****************************************************************************/
int INTPHY_CheckFixup(unsigned int uiPhy)
{
  unsigned int uiBasicCtrl = INTPHY_Read(uiPhy, ADR_PHYREG_BASIC_CONTROL);

  unsigned int uiPhyRev;

  uiPhyRev = INTPHY_Read(uiPhy, ADR_PHYREG_PHY_IDENTIFIER2) & MSK_PHYREG_PHY_IDENTIFIER2_REVISION;

  if( uiPhyRev < 3 )
  {
    /* receiver fix-ups */
    if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_DCBLW) != DFLT_DCBLW ) { return 1; }
    if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_DP_BASE_QTH) != DFLT_DP_BASE_QTH ) { return 1; }
    if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A3_CONFIG) != DFLT_A3_CONFIG ) { return 1; }
    if( (INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A11_CONFIG) & 0xffff) != DFLT_A11_CONFIG ) { return 1; }

    /* transmitter fix-ups */
    if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A5_CONFIG) != DFLT_A5_CONFIG ) { return 1; }
    if( INTPHY_ReadBank(uiPhy, REG_BANK_VMDAC, REG_VM_DAC_CONTROL1) != DFLT_VM_DAC_CONTROL1 ) { return 1; }
    if( INTPHY_ReadBank(uiPhy, REG_BANK_VMDAC, REG_MISC_PHY_CONTROL0) != DFLT_MISC_PHY_CONTROL0) { return 1; }
  }
  else
  {
    /* receiver fix-ups */
    if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_DCBLW) != DFLT_DCBLW ) { return 1; }
    if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_DP_BASE_QTH) != DFLT_DP_BASE_QTH ) { return 1; }

    /* transmitter fix-ups */
    if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A5_CONFIG) != DFLT_A5_CONFIG_REV3 ) { return 1; }
  }

  /* POWER_DOWN_CONTROL: set afe_common_ctrl_sel = port */
  if( (INTPHY_Read(uiPhy, REG_POWER_DOWN_CONTROL) & 0x7f) != VAL_POWER_DOWN_CONTROL(uiPhy)) { return 1; }

  /* AutoNeg disabled? */
  if( 0 == (uiBasicCtrl & MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE) )
  {
    /* 10Mbit? */
    if( 0 == (uiBasicCtrl & MSK_PHYREG_BASIC_CONTROL_SPEED_SELECT) )
    {
      /* fix MDIX bug in 10BT forced modes */
      if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_CONFIG) != (DFLT_CONFIG | MSK_PHYREG_DSP_BANK_CONFIG_FORCE_INIT_STATE) ) { return 1; }
    }
    else
    {
      /* fix MDIX bug in 100BT forced modes */
      if( INTPHY_ReadBank(uiPhy, REG_BANK_DSP, REG_A9_CONFIG) != (DFLT_A9_CONFIG | MSK_PHYREG_DSP_BANK_A9CFG_SQ_SRC) ) { return 1; }
    }
  }

  return 0;
}

static void INTPHY_HwReset( const INTPHY_CTRL_T* ptPhyCtrl )
{
  INTPHY_CTRL_T tPhyCtrl;
  unsigned int uiPort;

  tPhyCtrl.fPhySimBypass = false;
  tPhyCtrl.fPhyReset = true;

  /* reset INT_PHY */
  s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl = HW_MSK(int_phy_cfg_phy_ctrl_phy_reset);

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

  /* clear IRQs */
  s_ptIntPhyCfg->ulInt_phy_cfg_irq_raw = 0x3;
  s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_set = 0x3;

  if( ptPhyCtrl->fPhyReset )
  {
    return;
  }

  /* set configuration with reset gone */
  tPhyCtrl.fPhyReset = false;
  INTPHY_SetPhyCtrl(&tPhyCtrl);

  /* Wait 1ms for PHY reset extension */
  SLEEP_MS(1);

#ifdef INTPHY_NX90_REV0_SUPPORT
  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    /* check PHY ID/model number */
    unsigned int uiVal = PHYCTRL_MiimRead(INTPHY_PHYCTRL_INST, uiPort, ADR_PHYREG_PHY_IDENTIFIER2);

     /* if it does not match our expectation we assume netX90 revision 0 PHY with address[4] set to one. */
    if( (uiVal & ~MSK_PHYREG_PHY_IDENTIFIER2_REVISION) != INTPHY_PHYREG_PHY_IDENTIFIER2_ID_MODEL )
    {
      /* fix PHY address for netX90 revision 0 */
      uiVal = PHYCTRL_MiimRead(INTPHY_PHYCTRL_INST, 0x10 | uiPort, REG_SPECIAL_MODES);
      uiVal &= 0xffe1; /* set PHYADD[4:1] = 0 */
      PHYCTRL_MiimWrite(INTPHY_PHYCTRL_INST, 0x10 | uiPort, REG_SPECIAL_MODES, uiVal);
    }
  }
#endif
}

/*****************************************************************************/
/*! Reset PHY
* \description
*   Sets PHY Control Register
* \class
*   INTPHY
* \params
*   ptPhyCtrl          [in]  Pointer to PHY Control structure                */
/*****************************************************************************/
void INTPHY_Reset( const INTPHY_CTRL_T* ptPhyCtrl )
{
  unsigned int uiPort;

  INTPHY_HwReset(ptPhyCtrl);

  if( ptPhyCtrl->fPhyReset || ( ptPhyCtrl->uiPhyMode == INTPHY_MODE_POWER_DOWN_MODE ) )
  {
    return;
  }

  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    if( ptPhyCtrl->afPhyEnable[uiPort] && !ptPhyCtrl->afPhyFxMode[uiPort] )
    {
      /* PHY specific initialization that need to be done after each reset */
      INTPHY_Fixup(uiPort);

      if( ( ptPhyCtrl->uiPhyMode == INTPHY_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN )
        ||( ptPhyCtrl->uiPhyMode == INTPHY_MODE_REPEATER_AUTONEG_100BASE_TX_HD_ADV_CRSRX )
        ||( ptPhyCtrl->uiPhyMode == INTPHY_MODE_100BASE_TX_HD_ADV_AUTONEG_CRSRXTX )
        )
      {
        /* restart AutoNeg */
        INTPHY_Write(uiPort, 0, 0x1200);
      }
    }
  }
}

/*****************************************************************************/
/*! Setup Dual PHY.
* \description
*   Dual PHY boot sequence with standard settings.
*   Both ports are set into power-down mode.
* \class
*   INTPHY                                                                   */
/*****************************************************************************/
void INTPHY_SetupDualPhy(void)
{
  INTPHY_CTRL_T tPhyCtrl;
  unsigned int uiPort;

  /* initialize INT_PHY in standard copper mode */
  /* Reset into PowerDown mode has a ton of problems.
   * We work around by resetting into AutoNeg and power down the ports with BASIC_CONTROL register.
   */
  tPhyCtrl.fPhySimBypass = false;
  tPhyCtrl.uiPhyMode = INTPHY_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
  tPhyCtrl.fPhyReset = false;
  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    tPhyCtrl.afPhyFxMode[uiPort]   = false;
    tPhyCtrl.afPhyAutoMdix[uiPort] = true;
    tPhyCtrl.afPhyEnable[uiPort]   = true;
  }
  INTPHY_HwReset(&tPhyCtrl);

  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    /* power down by software */
    INTPHY_Write(uiPort, ADR_PHYREG_BASIC_CONTROL, MSK_PHYREG_BASIC_CONTROL_POWER_DOWN);
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
uint32_t INTPHY_PrmGet ( INTPHY_PRM_E ePrmID )
{
  uint32_t ulVal = 0;

  /* Get the selected parameter */
  switch(ePrmID)
  {
    case INTPHY_PRM_PHY_CTRL:        ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl; break;
    case INTPHY_PRM_PHY_GAIN_TABLE0: ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_gain_table0; break;
    case INTPHY_PRM_PHY_GAIN_TABLE1: ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_phy_gain_table1; break;
    case INTPHY_PRM_STATUS:          ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_status; break;
    case INTPHY_PRM_I2C_WRITE:       ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write; break;
    case INTPHY_PRM_I2C_READ:        ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_i2c_read; break;
    case INTPHY_PRM_DEBUG:           ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_debug;  break;

    case INTPHY_PRM_IRQ_RAW:         ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_irq_raw; break;
    case INTPHY_PRM_IRQ_MASKED:      ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_irq_masked; break;
    case INTPHY_PRM_IRQ_MASK_SET:    ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_set; break;
    case INTPHY_PRM_IRQ_MASK_RESET:  ulVal = s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_reset; break;

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
int INTPHY_PrmSet ( INTPHY_PRM_E ePrmID, uint32_t ulPrmVal )
{
  int iErr = 0;

  /* Get the selected parameter */
  switch(ePrmID)
  {
    case INTPHY_PRM_PHY_CTRL: s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl = ulPrmVal; break;

    case INTPHY_PRM_PHY_CTRL_EXT0_TXAMP10:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0, int_phy_cfg_phy_ctrl_ext0_txamp_10, ulPrmVal, iErr=2;)
      break;

    case INTPHY_PRM_PHY_CTRL_EXT0_TXAMP100:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0, int_phy_cfg_phy_ctrl_ext0_txamp_100, ulPrmVal, iErr=2;)
      break;

    case INTPHY_PRM_PHY_CTRL_EXT0_PHS_CLK_SEL:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0, int_phy_cfg_phy_ctrl_ext0_phs_clk_sel, ulPrmVal, iErr=2;)
      break;

    case INTPHY_PRM_PHY_CTRL_EXT0_GAIN_TABLE_EXT:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0, int_phy_cfg_phy_ctrl_ext0_gain_table_ext, ulPrmVal, iErr=2;)
      break;

    case INTPHY_PRM_PHY_CTRL_EXT0_RXLPF_PEAK:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext0, int_phy_cfg_phy_ctrl_ext0_rxlpf_peak, ulPrmVal, iErr=2;)
      break;

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

    case INTPHY_PRM_PHY_CTRL_EXT1_TXAMP10:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1, int_phy_cfg_phy_ctrl_ext1_txamp_10, ulPrmVal, iErr=2;)
      break;

    case INTPHY_PRM_PHY_CTRL_EXT1_TXAMP100:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1, int_phy_cfg_phy_ctrl_ext1_txamp_100, ulPrmVal, iErr=2;)
      break;

    case INTPHY_PRM_PHY_CTRL_EXT1_PHS_CLK_SEL:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1, int_phy_cfg_phy_ctrl_ext1_phs_clk_sel, ulPrmVal, iErr=2;)
      break;

    case INTPHY_PRM_PHY_CTRL_EXT1_GAIN_TABLE_EXT:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1, int_phy_cfg_phy_ctrl_ext1_gain_table_ext, ulPrmVal, iErr=2;)
      break;

    case INTPHY_PRM_PHY_CTRL_EXT1_RXLPF_PEAK:
      REG_BFSET(s_ptIntPhyCfg->ulInt_phy_cfg_phy_ctrl_ext1, int_phy_cfg_phy_ctrl_ext1_rxlpf_peak, ulPrmVal, iErr=2;)
      break;

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

    case INTPHY_PRM_I2C_WRITE:         s_ptIntPhyCfg->ulInt_phy_cfg_i2c_write = ulPrmVal; break;
    case INTPHY_PRM_I2C_READ:          s_ptIntPhyCfg->ulInt_phy_cfg_i2c_read = ulPrmVal; break;
    case INTPHY_PRM_DEBUG:             s_ptIntPhyCfg->ulInt_phy_cfg_debug = ulPrmVal; break;

    case INTPHY_PRM_IRQ_RAW:           s_ptIntPhyCfg->ulInt_phy_cfg_irq_raw = ulPrmVal; break;
    case INTPHY_PRM_IRQ_MASK_SET:      s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_set = ulPrmVal; break;
    case INTPHY_PRM_IRQ_MASK_RESET:    s_ptIntPhyCfg->ulInt_phy_cfg_irq_mask_reset = ulPrmVal; break;

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
*   uiPhy               [in]  MIIMU PHY Address
*   fSelect             [in]  true/false: select/de-select PHY               */
/*****************************************************************************/
static void INTPHY_I2cSelect ( unsigned int uiPhy, bool fSelect )
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
*   uiAddr              [in]  Register Address
*   usWriteData         [in]  Register value                                 */
/*****************************************************************************/
static void INTPHY_I2cWriteReg ( unsigned int uiAddr, uint16_t usWriteData )
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
*   uiAddr              [in]  Register Address
* \return
*   uiRdDataValid                                                            */
/*****************************************************************************/
static uint16_t INTPHY_I2cReadReg ( unsigned int uiAddr )
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
*   uiPhy               [in]  MIIMU PHY Address
*   uiAddr              [in]  Register Address
*   usWriteData         [in]  Register value                                 */
/*****************************************************************************/
void INTPHY_I2cWrite ( unsigned int uiPhy, unsigned int uiAddr, uint16_t usWriteData )
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
*   uiPhy               [in]  MIIMU PHY Address
*   uiAddr              [in]  Register Address
* \return
*   Register value                                                           */
/*****************************************************************************/
uint16_t INTPHY_I2cRead ( unsigned int uiPhy, unsigned int uiAddr )
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
void INTPHY_I2cWriteBank ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal )
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
uint32_t INTPHY_I2cReadBank ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr )
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
unsigned int INTPHY_Read(unsigned int uiPhy, unsigned int uiAddr)
{
  return PHYCTRL_MiimRead(INTPHY_PHYCTRL_INST, uiPhy, uiAddr);
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
void INTPHY_Write(unsigned int uiPhy, unsigned int uiAddr, unsigned int uiVal)
{
  PHYCTRL_MiimWrite(INTPHY_PHYCTRL_INST, uiPhy, uiAddr, uiVal);
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
uint32_t INTPHY_ReadBank ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr )
{
  uint32_t ulVal;
  unsigned int uiReg20Val = MSK_TSTCNTL_READ | (uiBank << 11) | (uiAddr << 5);

  if( uiBank == REG_BANK_DSP )
  {
    /* make sure that byp_tstmode_en is set due we do not do test mode procedure (not netx90_mpw) */
    INTPHY_Write(uiPhy, 20, uiReg20Val | MSK_TSTCNTL_TEST_MODE);

    /* read value from TST_READ1/2 */
    ulVal  = INTPHY_Read(uiPhy, 21);
    ulVal |= ((uint32_t)INTPHY_Read(uiPhy, 22)) << 16;
  }
  else
  {
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
void INTPHY_WriteBank ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal )
{
  unsigned int uiReg20Val = MSK_TSTCNTL_WRITE | (uiBank << 11) | (uiAddr << 0);

  if( uiBank == REG_BANK_DSP )
  {
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

/*****************************************************************************/
/*! INTPHY Set Mode Power-Down
* \description
*   Sets a PHY port into power-down mode.
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address                              */
/*****************************************************************************/
void INTPHY_SetModePowerDown(unsigned int uiPhy)
{
  /* set POWER_DOWN_CONTROL.afe_common_ctrl_sel = 0
   * Note: This must be done before going into power down.
   *       The afe_common_ctrl_sel bits of both ports are logically ORed and
   *       define which port controls the block that is common for both ports of the Dual PHY.
   *       In case of a powered down port controls the common block then the other port would malfunction.
   */
  INTPHY_Write(uiPhy, REG_POWER_DOWN_CONTROL, DFLT_POWER_DOWN_CONTROL);

  /* set auto-MDIX enable due NASR */
  INTPHY_Write(uiPhy, REG_MODE_CONTROL_STATUS, DFLT_MODE_CONTROL_STATUS);

  /* do SW reset and poll for done */
  INTPHY_Write(uiPhy, ADR_PHYREG_BASIC_CONTROL, MSK_PHYREG_BASIC_CONTROL_RESET);
  while( 0 != (INTPHY_Read(uiPhy, ADR_PHYREG_BASIC_CONTROL) & MSK_PHYREG_BASIC_CONTROL_RESET) );

  /* send PHY into PowerDown mode */
  INTPHY_Write(uiPhy, ADR_PHYREG_BASIC_CONTROL, MSK_PHYREG_BASIC_CONTROL_POWER_DOWN);
}

/*****************************************************************************/
/*! INTPHY Set Mode Auto-Negotiation
* \description
*   Sets a PHY port into auto-negotiation mode.
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   uiAutonegAdv        [in]  AutoNeg advertisement bit vector
*                             Each bit disables/enables the corresponding
*                             MAU type                                       */
/*****************************************************************************/
void INTPHY_SetModeAutoNeg(unsigned int uiPhy, unsigned int uiAutonegAdv)
{
#if defined(INTPHY_PD_AUTONEG_STALL) /* old implementation */

  /* configure AutoNeg advertisement */
  uiAutonegAdv |= 1; /* advertise selector field must be 1 (IEEE802.3) */
  INTPHY_Write(uiPhy, ADR_PHYREG_AUTONEG_ADV, uiAutonegAdv);

  /* restart AutoNeg state machine (port enabled due we exit PowerDown mode) */
  INTPHY_Write(uiPhy, ADR_PHYREG_BASIC_CONTROL, MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE
                                               |MSK_PHYREG_BASIC_CONTROL_RESTART_AUTONEG);

  /* wait for recovery from PowerDown mode */
  SLEEP_MS(1);

  /* Fix-up routine must be called on exit of PowerDown mode */
  INTPHY_FixupMode(uiPhy, MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE);

#else /* fixed implementation */

  /* exit PowerDown mode
   * Note: Do not enter AutoNeg mode directly after PowerDown, for it might cause a PHY stall issue! */
  INTPHY_Write(uiPhy, ADR_PHYREG_BASIC_CONTROL, 0);

  /* wait for recovery from PowerDown mode */
  SLEEP_MS(1);

  /* configure AutoNeg advertisement */
  uiAutonegAdv |= 1; /* advertise selector field must be 1 (IEEE802.3) */
  INTPHY_Write(uiPhy, ADR_PHYREG_AUTONEG_ADV, uiAutonegAdv);

  /* restart AutoNeg state machine */
  INTPHY_Write(uiPhy, ADR_PHYREG_BASIC_CONTROL, MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE
                                               |MSK_PHYREG_BASIC_CONTROL_RESTART_AUTONEG);

  /* Fix-up routine must be called on exit of PowerDown mode */
  INTPHY_FixupMode(uiPhy, MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE);

#endif
}

/*****************************************************************************/
/*! INTPHY Set Mode Forced
* \description
*   Sets a PHY port into forced mode.
*   Auto-negotiation is disabled, the MAU type is set explicitly.
* \class
*   INTPHY
* \params
*   uiPhy               [in]  MIIMU PHY Address
*   eMauType            [in]  MAU type
*   eMdixMode           [in]  MDI-X mode                                     */
/*****************************************************************************/
void INTPHY_SetModeForced(unsigned int uiPhy, INTPHY_MAUTYPE_E eMauType, INTPHY_MDIXMODE_E eMdixMode)
{
  unsigned int uiBasicCtrl = 0;
  unsigned int uiVal;

  /* configure MDI/MDIX/AutoMDIX mode */
  uiVal = INTPHY_Read(uiPhy, REG_MODE_CONTROL_STATUS);
  uiVal &= ~(MSK_PHYREG_MODE_CONTROL_WRITE_AS_ZERO
            |MSK_PHYREG_MODE_CONTROL_STATUS_AUTOMDIX_EN
            |MSK_PHYREG_MODE_CONTROL_STATUS_MDI_MODE);
  switch(eMdixMode)
  {
  case INTPHY_MDIXMODE_MDI:
    break;
  case INTPHY_MDIXMODE_MDIX:
    uiVal |= MSK_PHYREG_MODE_CONTROL_STATUS_MDI_MODE;
    break;
  case INTPHY_MDIXMODE_AUTO:
    uiVal |= MSK_PHYREG_MODE_CONTROL_STATUS_AUTOMDIX_EN;
    break;
  }
  INTPHY_Write(uiPhy, REG_MODE_CONTROL_STATUS, uiVal);

  /* configure MAU type */
  switch(eMauType)
  {
  case INTPHY_MAUTYPE_10HD:
    uiBasicCtrl = 0;
    break;
  case INTPHY_MAUTYPE_10FD:
    uiBasicCtrl = MSK_PHYREG_BASIC_CONTROL_DUPLEX_MODE;
    break;
  case INTPHY_MAUTYPE_100HD:
    uiBasicCtrl = MSK_PHYREG_BASIC_CONTROL_SPEED_SELECT;
    break;
  case INTPHY_MAUTYPE_100FD:
    uiBasicCtrl = MSK_PHYREG_BASIC_CONTROL_SPEED_SELECT
                 |MSK_PHYREG_BASIC_CONTROL_DUPLEX_MODE;
    break;
  }
  /* set forced mode (port enabled due we exit PowerDown mode) */
  INTPHY_Write(uiPhy, ADR_PHYREG_BASIC_CONTROL, uiBasicCtrl);

  /* wait for recovery from PowerDown mode */
  SLEEP_MS(1);

  /* Fix-up routine must be called on exit of PowerDown mode */
  INTPHY_FixupMode(uiPhy, uiBasicCtrl);
}
