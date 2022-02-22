/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_SYSTIME(s_aptSystime)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Set SYSTIME Border
* \description
*   Set the border.
* \class
*   SYSTIME
* \params
*   ulBorder              [in]  SYSTIME Border Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void SYSTIME_SetBorder( unsigned int uiInst, uint32_t ulBorder, void* pvUser )
{
  s_aptSystime[uiInst]->ulSystime_border = ulBorder;
}

/*****************************************************************************/
/*! Get SYSTIME Border
* \description
*   Get the border.
* \class
*   SYSTIME
* \params
*   pulBorder             [out] Pointer of SYSTIME Border Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void SYSTIME_GetBorder( unsigned int uiInst, uint32_t* pulBorder, void* pvUser )
{
  *pulBorder = s_aptSystime[uiInst]->ulSystime_border;
}

/*****************************************************************************/
/*! Set SYSTIME Speed
* \description
*   Set the speed.
* \class
*   SYSTIME
* \params
*   ulSpeed               [in]  SYSTIME Speed Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void SYSTIME_SetSpeed( unsigned int uiInst, uint32_t ulSpeed, void* pvUser )
{
  s_aptSystime[uiInst]->ulSystime_count_value = ulSpeed;
}

/*****************************************************************************/
/*! Get SYSTIME Speed
* \description
*   Get the speed.
* \class
*   SYSTIME
* \params
*   pulSpeed              [out] Pointer of SYSTIME Speed Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void SYSTIME_GetSpeed( unsigned int uiInst, uint32_t* pulSpeed, void* pvUser )
{
  *pulSpeed = s_aptSystime[uiInst]->ulSystime_count_value;
}

/*****************************************************************************/
/*! Set SYSTIME
* \description
*   Sets the SYSTIME.
* \class
*   SYSTIME
* \params
*   ulSystime_s           [in]  SYSTIME Value in Seconds
*   ulSystime_ns          [in]  SYSTIME Value in Nanoseconds
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void SYSTIME_SetSystime( unsigned int uiInst, uint32_t ulSystime_s, uint32_t ulSystime_ns, void* pvUser )
{
  s_aptSystime[uiInst]->ulSystime_ns = ulSystime_ns;
  s_aptSystime[uiInst]->ulSystime_s  = ulSystime_s;
}

/*****************************************************************************/
/*! Get SYSTIME
* \description
*   Read the system time.
* \class
*   SYSTIME
* \params
*   pulSystime_s          [in]  Pointer of SYSTIME Value in Seconds
*   pulSystime_ns         [in]  Pointer of SYSTIME Value in Nanoseconds
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void SYSTIME_GetSystime( unsigned int uiInst, uint32_t* pulSystime_s, uint32_t* pulSystime_ns, void* pvUser )
{
  *pulSystime_s  = s_aptSystime[uiInst]->ulSystime_s;
  *pulSystime_ns = s_aptSystime[uiInst]->ulSystime_ns;
}

/*****************************************************************************/
/*! Get SystimeNs
* \description
*   Returns the systime_ns only.
* \class
*   SYSTIME
* \params
*   pvUser                [in]  User specific parameter
* \return
* systime_ns value                                                           */
/*****************************************************************************/
uint32_t SYSTIME_GetSystimeNs( unsigned int uiInst, void* pvUser )
{
  return ( s_aptSystime[uiInst]->ulSystime_ns  );
}

/*****************************************************************************/
/*! Get SystimeS
* \description
*   Returns the systime_s only.
* \class
*   SYSTIME
* \params
*   pvUser                [in]  User specific parameter
* \return
* systime_ns value                                                           */
/*****************************************************************************/
uint32_t SYSTIME_GetSystimeS( unsigned int uiInst, void* pvUser )
{
  return ( s_aptSystime[uiInst]->ulSystime_s  );
}

