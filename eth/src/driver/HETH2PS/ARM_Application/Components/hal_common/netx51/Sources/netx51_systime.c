/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_armtimer.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_SYSTIME
__USE_ARM_TIMER

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Set System Time Border
* \description
*   Set the System Time border.
* \class
*   SYSTIME
* \params
*   ulBorder              [in]  System Time Border Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SYSTIME_SetBorder( uint32_t ulBorder,
                             void*    pvUser )
{
  s_ptSystime->ulSystime_border = ulBorder;
}

/*****************************************************************************/
/*! Get System Time Border
* \description
*   Get the System Time border.
* \class
*   SYSTIME
* \params
*   pulBorder             [out] Pointer of System Time Border Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SYSTIME_GetBorder( uint32_t* pulBorder,
                             void*     pvUser )
{
  *pulBorder = s_ptSystime->ulSystime_border;
}

/*****************************************************************************/
/*! Set System Time Speed
* \description
*   Set the System Time speed.
* \class
*   SYSTIME
* \params
*   ulSpeed               [in]  System Time Speed Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SYSTIME_SetSpeed( uint32_t ulSpeed,
                            void*    pvUser )
{
  s_ptSystime->ulSystime_count_value = ulSpeed;
}

/*****************************************************************************/
/*! Get System Time Speed
* \description
*   Get the System Time speed.
* \class
*   SYSTIME
* \params
*   pulSpeed              [out] Pointer of System Time Speed Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SYSTIME_GetSpeed( uint32_t* pulSpeed,
                            void*     pvUser )
{
  *pulSpeed = s_ptSystime->ulSystime_count_value;
}

/*****************************************************************************/
/*! Set System Time
* \description
*   Sets the System Time.
* \class
*   SYSTIME
* \params
*   ulSystime_s           [in]  System Time Value in Seconds
*   ulSystime_ns          [in]  System Time Value in Nanoseconds
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SYSTIME_SetSystime( uint32_t ulSystime_s,
                              uint32_t ulSystime_ns,
                              void*    pvUser )
{
  s_ptSystime->ulSystime_ns = ulSystime_ns;
  s_ptSystime->ulSystime_s  = ulSystime_s;
}

/*****************************************************************************/
/*! Get System Time
* \description
*   Read the System Time via ARM_TIMER channel.
* \class
*   SYSTIME
* \params
*   pulSystime_s          [in]  Pointer of System Time Value in Seconds
*   pulSystime_ns         [in]  Pointer of System Time Value in Nanoseconds
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SYSTIME_GetSystime( uint32_t* pulSystime_s,
                              uint32_t* pulSystime_ns,
                              void*     pvUser )
{
  *pulSystime_s  = s_ptArmTimer->ulArm_timer_systime_s;
  *pulSystime_ns = s_ptArmTimer->ulArm_timer_systime_ns;
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
*   systime_ns value                                                           */
/*****************************************************************************/
uint32_t NX51_SYSTIME_GetSystimeNs( void* pvUser )
{
  volatile uint32_t ulNs;

  ulNs = s_ptArmTimer->ulArm_timer_systime_s;
  ulNs = s_ptArmTimer->ulArm_timer_systime_ns;

  return ulNs;
}
