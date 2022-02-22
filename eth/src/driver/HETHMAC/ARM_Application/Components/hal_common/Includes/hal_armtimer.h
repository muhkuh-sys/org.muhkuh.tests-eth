#ifndef __HAL_ARMTIMER_H
#define __HAL_ARMTIMER_H

#include <stdint.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define ARMTIMER_CHANNELS     3 /**< Number of timers */

/*! Result codes for functions */
typedef enum
{
  ARMTIMER_OKAY             = 0, /**< Successful          */
  ARMTIMER_ERR_INVAL_PARAM  = 1  /**< Invalid parameter   */
} ARMTIMER_RESULT_E;

/*! Configuration for ARM timer */
typedef enum
{
  ARMTIMER_CFG_MODE_STOP_AT_0      = 0, /**< 0: Timer stops at 0                                       */
  ARMTIMER_CFG_MODE_PRELOAD_AT_0   = 1, /**< 1: Timer is preload with value from preload register at 0 */
  ARMTIMER_CFG_MODE_SYSTIME        = 2  /**< 2: Timer (value) compare with systime (once)              */
} ARMTIMER_CFG_MODE_E;

/*! Parameters ID that can be get by ArmTimer_GetPrm() */
typedef enum
{
  ARMTIMER_PRM_CONFIG_TIMER0          =  1, /**< Register arm_timer_config_timer0          */
  ARMTIMER_PRM_CONFIG_TIMER1          =  2, /**< Register arm_timer_config_timer1          */
  ARMTIMER_PRM_CONFIG_TIMER2          =  3, /**< Register arm_timer_config_timer2          */
  ARMTIMER_PRM_PRELOAD_TIMER0         =  4, /**< Register arm_timer_preload_timer0         */
  ARMTIMER_PRM_PRELOAD_TIMER1         =  5, /**< Register arm_timer_preload_timer1         */
  ARMTIMER_PRM_PRELOAD_TIMER2         =  6, /**< Register arm_timer_preload_timer2         */
  ARMTIMER_PRM_TIMER0                 =  7, /**< Register arm_timer_timer0                 */
  ARMTIMER_PRM_TIMER1                 =  8, /**< Register arm_timer_timer1                 */
  ARMTIMER_PRM_TIMER2                 =  9, /**< Register arm_timer_timer2                 */
  ARMTIMER_PRM_SYSTIME_S_COMPARE      = 10, /**< Register arm_timer_systime_s_compare      */
  ARMTIMER_PRM_IRQMSKSET              = 11, /**< interrupt mask enable                     */
  ARMTIMER_PRM_IRQMSKRST              = 12, /**< interrupt mask disable                    */
  ARMTIMER_PRM_SYSTIME_S              = 13, /**< Register arm_timer_systime_s              */
  ARMTIMER_PRM_SYSTIME_NS             = 14  /**< Register arm_timer_systime_ns             */
} ARMTIMER_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

ARMTIMER_RESULT_E         HALDEC(ArmTimer_ConfigureTimerSystime) ( unsigned int uInst, unsigned int uSystimeInst );
ARMTIMER_RESULT_E         HALDEC(ArmTimer_Start)                 ( unsigned int uInst, ARMTIMER_CFG_MODE_E eMode, uint32_t ulVal );
ARMTIMER_RESULT_E         HALDEC(ArmTimer_Stop)                  ( unsigned int uInst  );
ARMTIMER_RESULT_E         HALDEC(ArmTimer_StartSystimeSCmp)      ( uint32_t ulVal );
ARMTIMER_RESULT_E         HALDEC(ArmTimer_EnIrq)                 ( uint32_t ulIrqMsk );
ARMTIMER_RESULT_E         HALDEC(ArmTimer_DisIrq)                ( uint32_t ulIrqMsk );
uint32_t                  HALDEC(ArmTimer_GetIrq)                ( void );
uint32_t                  HALDEC(ArmTimer_GetIrqRaw)             ( void );
void                      HALDEC(ArmTimer_ConfirmIrq)            ( uint32_t ulConfirmIrqMask );
ARMTIMER_RESULT_E         HALDEC(ArmTimer_GetPrm)                ( ARMTIMER_PRM_E ePrmID, uint32_t* pulPrmVal );
ARMTIMER_RESULT_E         HALDEC(ArmTimer_SetPrm)                ( ARMTIMER_PRM_E ePrmID, uint32_t ulPrmVal );
void                      HALDEC(ArmTimer_Sleep)                 ( unsigned int uInst, uint32_t ulTimeout10Ns );
void                      HALDEC(ArmTimer_SetSystimeConfig)      ( unsigned int uSysimeInst );
uint32_t                  HALDEC(ArmTimer_GetTimer)              ( unsigned int uInst );
void                      HALDEC(ArmTimer_GetSystime)            ( uint32_t* pulSystime_s, uint32_t* pulSystime_ns );

#endif /* #ifndef __HAL_ARMTIMER_H */
