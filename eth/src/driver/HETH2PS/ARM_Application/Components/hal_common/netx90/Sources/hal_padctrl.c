/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_padctrl.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_PADCTRL(s_paulPadCtrl)
HW_PTR_ASIC_CTRL(s_ptAsicCtrl_padctrl)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! PAD_CTRL Set Configuration
* \description
*   Configures ports pins
* \class
*   PADCTRL
* \params
*   paulConfigTable   [in]  Pointer to configuration array
*   uNumEntries       [in]  Number of entries within array
* \return
*  PADCTRL_RESULT_E                                                          */
/*****************************************************************************/
PADCTRL_RESULT_E PadCtrl_SetCfg( const uint32_t* paulConfigTable, unsigned int uiNumEntries )
{
  unsigned int uiTableIdx;
  uint32_t ulRegIdx, ulRegVal;

  /* set all registers defined in the table */
  for(uiTableIdx = 0; uiTableIdx < uiNumEntries; ++uiTableIdx)
  {
    ulRegIdx = paulConfigTable[uiTableIdx] >> SRT_PADCTRL_CFG_PORT;
    ulRegVal = paulConfigTable[uiTableIdx] &  MSK_PADCTRL_CFG_VAL;

    if( ulRegIdx < PAD_CTRL_PORT_CNT )
    {
      s_ptAsicCtrl_padctrl->ulAsic_ctrl_access_key = s_ptAsicCtrl_padctrl->ulAsic_ctrl_access_key;
      s_paulPadCtrl[ulRegIdx] = ulRegVal;
    }
    else
    {
      return PADCTRL_RESULT_INVAL_PARAM;
    }
  }

  return PADCTRL_RESULT_OK;
}

/*****************************************************************************/
/*! PAD_CTRL Get Configuration
* \description
*   Returns configuration of a port.
* \class
*   PADCTRL
* \params
*   ePort             [in]   Selected port
* \return
*   uint32_t                                                                 */
/*****************************************************************************/
uint32_t PadCtrl_GetCfg( PADCTRL_PORT_E  ePort )
{
  return s_paulPadCtrl[(unsigned int)ePort];
}

/*****************************************************************************/
/*! PAD_CTRL Set Pull Enable
* \description
*   Set Pull enable
* \class
*   PADCTRL
* \params
*   ePort             [in] Selected port
*   fEn               [in] false/true: disable/enable
* \return
*  PADCTRL_RESULT_E                                                          */
/*****************************************************************************/
PADCTRL_RESULT_E PadCtrl_SetPullEn( PADCTRL_PORT_E ePort, bool fEn )
{
  uint32_t ulRegVal;

  if( (unsigned int)ePort < PAD_CTRL_PORT_CNT )
  {
    ulRegVal = s_paulPadCtrl[(unsigned int)ePort];
    if(fEn) {
      ulRegVal |= 1UL << SRT_PADCTRL_CFG_PE;
    } else {
      ulRegVal &= ~(1UL << SRT_PADCTRL_CFG_PE);
    }
    s_ptAsicCtrl_padctrl->ulAsic_ctrl_access_key = s_ptAsicCtrl_padctrl->ulAsic_ctrl_access_key;
    s_paulPadCtrl[(unsigned int)ePort] = ulRegVal;
  }
  else {
    return PADCTRL_RESULT_INVAL_PARAM;
  }

  return PADCTRL_RESULT_OK;
}

