/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_SYSTIMELT(s_aptSystimeLt)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Request Latching
* \description
*   Issues a latch request.
* \class
*   SYSTIMELT
* \params
*   eCpu              [in]  CPU type
*   ulReqMsk          [in]  Latch Request mask
* \return
*                                                                            */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(SYSTIMELT_ReqLatch) ( unsigned int uiInst, uint32_t ulReqMsk )
{
  s_aptSystimeLt[uiInst]->ulIntlogic_lt_systimes_latch = ulReqMsk;
}

/*****************************************************************************/
/*! Get SYSTIME
* \description
*   Read the system time.
* \class
*   SYSTIMELT
* \params
*   eCpu                  [in]  CPU type
*   uInst                 [in]  SYSTIME instance
*   pulSystime_s          [in]  Pointer of SYSTIME Value in Seconds
*   pulSystime_ns         [in]  Pointer of SYSTIME Value in Nanoseconds
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(SYSTIMELT_GetSystime)( unsigned int uiInst, unsigned int uiSystimeIdx, uint32_t* pulSystime_s, uint32_t* pulSystime_ns )
{
  switch( uiSystimeIdx )
  {
  case 0:
    *pulSystime_s  = s_aptSystimeLt[uiInst]->ulIntlogic_lt_systime_com_s;
    *pulSystime_ns = s_aptSystimeLt[uiInst]->ulIntlogic_lt_systime_com_ns;
  break;

  case 1:
    *pulSystime_s  = s_aptSystimeLt[uiInst]->ulIntlogic_lt_systime_com_uc_s;
    *pulSystime_ns = s_aptSystimeLt[uiInst]->ulIntlogic_lt_systime_com_uc_ns;
  break;

  case 2:
    *pulSystime_s  = s_aptSystimeLt[uiInst]->ulIntlogic_lt_systime_app_s;
    *pulSystime_ns = s_aptSystimeLt[uiInst]->ulIntlogic_lt_systime_app_ns;
  break;

  default:
    break;
  }
}

