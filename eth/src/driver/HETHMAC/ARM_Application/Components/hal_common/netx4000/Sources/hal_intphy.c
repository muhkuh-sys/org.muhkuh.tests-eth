/* Includes */
#include "hal_intphy.h"
#include "hal_miimu.h"
#include "hw_defines.h"

#define MSK_TSTCNTL_READ                0x8000U
#define MSK_TSTCNTL_WRITE               0x4000U
#define MSK_TSTCNTL_TEST_MODE           0x0400U

#define SLEEP_MS(ms) SYS_SLEEP_CC(ms * 100000)

typedef struct PHY_CTRL_Ttag
{
  unsigned int uiMiimFreq : 1;
  unsigned int uiMiimPreamble : 1;
} PHY_CTRL_T;

/* Variables */
static PHY_CTRL_T s_atPhyCtrl[] =
{
  {MIIMU_MDCFREQ_HIGH, MIIMU_SEND_PREAMBLE},
  {MIIMU_MDCFREQ_HIGH, MIIMU_SEND_PREAMBLE},
};

/* Variables */
HW_PTR_XC_EXTERNAL_CFG(s_ptXcExtCfg)


/* Functions */

/*****************************************************************************/
/*! Initialize Internal PHY
* \description
*   Deprecated, use INTPHY_Reset instead!
*   Initialize internal PHY.
* \class 
*   INTPHY 
* \params
*   ptPhyCtrlInit          [in]  Pointer of PHY Control Value
* \return
*                                                                            */
/*****************************************************************************/

#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(INTPHY_Init)( void )
{
  unsigned int uiPort;

  s_ptXcExtCfg->ulPhy_control = HW_DFLT_VAL(phy_control);

  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort)
  {
    s_atPhyCtrl[uiPort].uiMiimFreq     = MIIMU_MDCFREQ_HIGH;
    s_atPhyCtrl[uiPort].uiMiimPreamble = MIIMU_SEND_PREAMBLE;
  }
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
    case INTPHY_PRM_MIIM_PREAMBLE_0: ulVal = s_atPhyCtrl[0].uiMiimPreamble; break;
    case INTPHY_PRM_MIIM_PREAMBLE_1: ulVal = s_atPhyCtrl[1].uiMiimPreamble; break;
    case INTPHY_PRM_MIIM_FREQ_0:     ulVal = s_atPhyCtrl[0].uiMiimFreq; break;
    case INTPHY_PRM_MIIM_FREQ_1:     ulVal = s_atPhyCtrl[1].uiMiimFreq; break;

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
  /* Get the selected parameter */
  switch(ePrmID)
  {
    case INTPHY_PRM_MIIM_PREAMBLE_0: s_atPhyCtrl[0].uiMiimPreamble   = 1U & ulPrmVal; break;
    case INTPHY_PRM_MIIM_PREAMBLE_1: s_atPhyCtrl[1].uiMiimPreamble   = 1U & ulPrmVal; break;
    case INTPHY_PRM_MIIM_FREQ_0:     s_atPhyCtrl[0].uiMiimFreq       = 1U & ulPrmVal; break;
    case INTPHY_PRM_MIIM_FREQ_1:     s_atPhyCtrl[1].uiMiimFreq       = 1U & ulPrmVal; break;

    /* Default */
    default:
      return 1;
    break;
  } /* end switch */

  return 0;
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
  uint16_t usVal;
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
void HALDEF(INTPHY_WriteBank) ( unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal )
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
