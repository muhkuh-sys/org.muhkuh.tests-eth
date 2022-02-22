/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_systime_uc.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_SYSTIME_UC

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Set System Time Border
* \description
*   Set the System Time border.
* \class 
*   SYSTIME_UC
* \params
*   ulBorder              [in]  System Time Border Value
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX6_SYSTIMEUC_SetBorder( uint32_t ulBorder,
                              void*    pvUser )
{
  NX_WRITE32(s_ptSystimeUc->ulSystime_border, ulBorder);
}

/*****************************************************************************/
/*! Get System Time Border
* \description
*   Get the System Time border.
* \class 
*   SYSTIME_UC
* \params
*   pulBorder             [out] Pointer of System Time Border Value
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX6_SYSTIMEUC_GetBorder( uint32_t* pulBorder,
                              void*     pvUser )
{
  *pulBorder = NX_READ32(s_ptSystimeUc->ulSystime_border);
}

/*****************************************************************************/
/*! Set System Time Speed
* \description
*   Set the System Time speed.
* \class 
*   SYSTIME_UC
* \params
*   ulSpeed               [in]  System Time Speed Value
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX6_SYSTIMEUC_SetSpeed( uint32_t ulSpeed,
                             void*    pvUser )
{
  NX_WRITE32(s_ptSystimeUc->ulSystime_count_value, ulSpeed);
}

/*****************************************************************************/
/*! Get System Time Speed
* \description
*   Get the System Time speed.
* \class 
*   SYSTIME_UC
* \params
*   pulSpeed              [out] Pointer of System Time Speed Value
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX6_SYSTIMEUC_GetSpeed( uint32_t* pulSpeed,
                             void*     pvUser )
{
  *pulSpeed = NX_READ32(s_ptSystimeUc->ulSystime_count_value);
}

/*****************************************************************************/
/*! Set System Time
* \description
*   Sets the System Time.
* \class 
*   SYSTIME_UC
* \params
*   ulSystime_s           [in]  System Time Value in Seconds
*   ulSystime_ns          [in]  System Time Value in Nanoseconds
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX6_SYSTIMEUC_SetSystime( uint32_t ulSystime_s,
                               uint32_t ulSystime_ns,
                               void*    pvUser )
{
  NX_WRITE32(s_ptSystimeUc->ulSystime_ns, ulSystime_ns);
  NX_WRITE32(s_ptSystimeUc->ulSystime_s, ulSystime_s);
}

/*****************************************************************************/
/*! Get System Time
* \description
*   Read the System Time.
* \class 
*   SYSTIME_UC
* \params
*   pulSystime_s          [out]  Pointer of System Time Value in Seconds
*   pulSystime_ns         [out]  Pointer of System Time Value in Nanoseconds
*   pvUser                [in]   User specific parameter
* \return 
*                                                                            */
/*****************************************************************************/
void NX6_SYSTIMEUC_GetSystime( uint32_t* pulSystime_s,
                               uint32_t* pulSystime_ns,
                               void* pvUser )
{
  *pulSystime_s  = NX_READ32(s_ptSystimeUc->ulSystime_s);
  *pulSystime_ns = NX_READ32(s_ptSystimeUc->ulSystime_ns);
}

/*****************************************************************************/
/*! Get SystimeNs
* \description
*   Returns the systime_ns only.
* \class 
*   SYSTIME_UC
* \params
*   pvUser                [in]  User specific parameter 
* \return 
*   systime_ns value                                                           */
/*****************************************************************************/
uint32_t NX6_SYSTIMEUC_GetSystimeNs( void* pvUser )
{
  volatile uint32_t ulNs;
  
  ulNs = NX_READ32(s_ptSystimeUc->ulSystime_s);
  ulNs = NX_READ32(s_ptSystimeUc->ulSystime_ns);
  
  return ulNs; 
}
