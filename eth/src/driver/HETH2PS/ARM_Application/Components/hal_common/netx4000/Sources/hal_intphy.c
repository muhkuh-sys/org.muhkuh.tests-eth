/* Includes */
#include "hal_phy.h"
#include "hal_intphy.h"
#include "hal_phyctrl.h"
#include "hw_defines.h"

#define MSK_TSTCNTL_READ                0x8000U
#define MSK_TSTCNTL_WRITE               0x4000U
#define MSK_TSTCNTL_TEST_MODE           0x0400U

/* bit masks for register 17 (MODE_CONTROL_STATUS) */
#define MSK_PHYREG_MODE_CONTROL_STATUS_AUTOMDIX_EN 0x0080
#define MSK_PHYREG_MODE_CONTROL_STATUS_MDI_MODE    0x0040
#define MSK_PHYREG_MODE_CONTROL_WRITE_AS_ZERO      0xd022

#define SLEEP_MS(ms) SYS_SLEEP_CC(ms * 100000)

/* Variables */
HW_PTR_XC_EXTERNAL_CFG(s_ptXcExtCfg)

/* Functions */

/*****************************************************************************/
/*! Initialize Internal PHY
* \description
*   Initialize internal PHY.
* \class
*   INTPHY
* \return
*                                                                            */
/*****************************************************************************/
void INTPHY_Init( void )
{
  unsigned int uiPort;

  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort )
  {
    /* initialize PHY management interface (MIIM) */
    PHYCTRL_Init(uiPort);

    /* send MIIM preamble */
    PHYCTRL_PrmSet(uiPort, PHYCTRL_PRM_MIIM_PREAMBLE, 1);

    /* use high speed MDC */
    PHYCTRL_PrmSet(uiPort, PHYCTRL_PRM_MIIM_MDC_FAST, 1);
  }

  s_ptXcExtCfg->ulPhy_control = HW_DFLT_VAL(phy_control);
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
  uint32_t ulVal;

  ulVal = ptPhyCtrl->uiPhyMode << HW_SRT(phy_control_phy_mode);

  if( ptPhyCtrl->afPhyFxMode[0] ) ulVal |= HW_MSK(phy_control_phy0_fxmode);
  if( ptPhyCtrl->afPhyAutoMdix[0] ) ulVal |= HW_MSK(phy_control_phy0_automdixen);
  if( ptPhyCtrl->afPhyEnable[0] ) ulVal |= HW_MSK(phy_control_phy0_enable);

  if( ptPhyCtrl->afPhyFxMode[1] ) ulVal |= HW_MSK(phy_control_phy1_fxmode);
  if( ptPhyCtrl->afPhyAutoMdix[1] ) ulVal |= HW_MSK(phy_control_phy1_automdixen);
  if( ptPhyCtrl->afPhyEnable[1] ) ulVal |= HW_MSK(phy_control_phy1_enable);

  if( ptPhyCtrl->fPhySimBypass ) ulVal |= HW_MSK(phy_control_phy_sim_byp);
  if( ptPhyCtrl->fPhyReset ) ulVal |= HW_MSK(phy_control_phy_reset);

  /* write value */
  s_ptXcExtCfg->ulPhy_control = ulVal;
}

/*****************************************************************************/
/*! Reset PHY
* \description
*   Dual PHY boot sequence.
* \class
*   INTPHY
* \params
*   ptPhyCtrl          [in]  Pointer to PHY Control structure
* \return
*                                                                            */
/*****************************************************************************/
void INTPHY_Reset ( const INTPHY_CTRL_T* ptPhyCtrl )
{
  INTPHY_CTRL_T tPhyCtrl;
  unsigned int uiPort;

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

  /* Wait 1ms for PHY reset */
  SLEEP_MS(1);

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

  /* set configuration with reset gone */
  tPhyCtrl.fPhyReset = false;
  INTPHY_SetPhyCtrl(&tPhyCtrl);

  /* Wait 10ms for PHY reset extension */
  SLEEP_MS(10);
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
  INTPHY_Reset(&tPhyCtrl);

  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    INTPHY_SetModePowerDown(uiPort);
  }
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
  return PHYCTRL_MiimRead(uiPhy, uiPhy, uiAddr);
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
  PHYCTRL_MiimWrite(uiPhy, uiPhy, uiAddr, uiVal);
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
    /* enter test mode */
    INTPHY_Write(uiPhy, 20, MSK_TSTCNTL_TEST_MODE);
    INTPHY_Write(uiPhy, 20, 0x0000);
    INTPHY_Write(uiPhy, 20, MSK_TSTCNTL_TEST_MODE);

    /* make sure that byp_tstmode_en is set due we do not do test mode procedure (not netx90_mpw) */
    INTPHY_Write(uiPhy, 20, uiReg20Val | MSK_TSTCNTL_TEST_MODE);

    /* read value from TST_READ1/2 */
    ulVal  = INTPHY_Read(uiPhy, 21);
    ulVal |= ((uint32_t)INTPHY_Read(uiPhy, 22)) << 16;
  }
  else
  {
    uiReg20Val |= uiAddr; /* bug: also set write address (required when reading VM_DAC_CONTROL1) */
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
    /* enter test mode */
    INTPHY_Write(uiPhy, 20, MSK_TSTCNTL_TEST_MODE);
    INTPHY_Write(uiPhy, 20, 0x0000);
    INTPHY_Write(uiPhy, 20, MSK_TSTCNTL_TEST_MODE);

    /* make sure that byp_tstmode_en is set due we do not do test mode procedure (not netx90_mpw) */
    INTPHY_Write(uiPhy, 22, uiVal);
    INTPHY_Write(uiPhy, 20, uiReg20Val | MSK_TSTCNTL_TEST_MODE);
  }
  else
  {
    INTPHY_Write(uiPhy, 22, uiVal);
    INTPHY_Write(uiPhy, 20, uiReg20Val);
  }

  /* leave indirect address mode */
  INTPHY_Write(uiPhy, 20, 0x0000);
}

/*****************************************************************************/
/*! Set INTPHY Parameter
* \description
*   Set any INTPHY Parameter
* \class
*   INTPHY
* \params
*   ePrmID     [in]   Parameter Identifier
*   ulPrmVal   [in]   Parameter Value
* \return
*   0 on success
*   1 on failure                                                             */
/*****************************************************************************/
int INTPHY_PrmSet ( INTPHY_PRM_E ePrmID, uint32_t ulPrmVal )
{
  int iRslt = 0;

  /* Get the selected parameter */
  switch(ePrmID)
  {
    case INTPHY_PRM_PHY_CTRL:
      s_ptXcExtCfg->ulPhy_control = ulPrmVal;
      break;

    /* Default */
    default:
      iRslt = 1;
      break;
  }

  return iRslt;
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
    case INTPHY_PRM_PHY_CTRL:
      ulVal = s_ptXcExtCfg->ulPhy_control;
      break;

    /* Default */
    default:
      break;
  }

  return ulVal;
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
  /* power down by software (restart AutoNeg, advertise none) */
  INTPHY_Write(uiPhy, ADR_PHYREG_AUTONEG_ADV, 0);
  INTPHY_Write(uiPhy, ADR_PHYREG_BASIC_CONTROL, MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE
                                               |MSK_PHYREG_BASIC_CONTROL_RESTART_AUTONEG);
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
  /* configure AutoNeg advertisement */
  uiAutonegAdv |= 1; /* advertise selector field must be 1 (IEEE802.3) */
  INTPHY_Write(uiPhy, ADR_PHYREG_AUTONEG_ADV, uiAutonegAdv);

  /* restart AutoNeg state machine (port enabled due we exit PowerDown mode) */
  INTPHY_Write(uiPhy, ADR_PHYREG_BASIC_CONTROL, MSK_PHYREG_BASIC_CONTROL_AUTONEG_ENABLE
                                               |MSK_PHYREG_BASIC_CONTROL_RESTART_AUTONEG);
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
}
