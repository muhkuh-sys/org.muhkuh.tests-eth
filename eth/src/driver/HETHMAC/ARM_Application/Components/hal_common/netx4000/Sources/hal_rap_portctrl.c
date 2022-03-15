/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_rap_portctrl.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_RAP_PORT_CTRL(s_paulRapPortCtrl)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! RAP PORT_CONTROL Set Configuration
* \description
*   Configures ports pins
* \class
*   RAP_PORT_CTRL
* \params
*   paulConfigTable   [in]  Pointer to configuration array
*   uNumEntries       [in]  Number of entries within array
* \return
*                                                                            */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

RAP_PORTCTRL_RESULT_E HALDEF(RAP_PortCtrl_SetCfg)( const uint32_t* paulConfigTable, unsigned int uNumEntries )
{
  unsigned int uTableIndex;
  uint32_t ulRegIdx, ulRegVal;

  /* set all registers defined in the table */
  for(uTableIndex = 0; uTableIndex < uNumEntries; ++uTableIndex)
  {
    ulRegIdx = paulConfigTable[uTableIndex] >> SRT_RAP_PORTCTRL_CFG_PORT;
    ulRegVal = paulConfigTable[uTableIndex] &  MSK_RAP_PORTCTRL_CFG_VAL;

    if( ulRegIdx < RAP_PORTCTRL_CFG_PORT_CNT )
    {
      s_paulRapPortCtrl[ulRegIdx] = ulRegVal;
    }
    else
    {
      return RAP_PORTCTRL_RESULT_INVAL_PARAM;
    }
  }

  return RAP_PORTCTRL_RESULT_OK;
}

/*****************************************************************************/
/*! RAP PORT_CONTROL Get Configuration
* \description
*   Returns configuration of a port.
* \class
*   RAP_PORT_CTRL
* \params
*   ePort             [in]   Selected port
* \return
*   uint32_t                                                                 */
/*****************************************************************************/
uint32_t HALDEF(RAP_PortCtrl_GetCfg)( RAP_PORTCTRL_PORT_E  ePort )
{
  return s_paulRapPortCtrl[(unsigned int)ePort];
}
