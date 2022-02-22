#ifndef __NETX51_ARMTIMER_H
#define __NETX51_ARMTIMER_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: bool definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX51_ARMTIMER_CHANNELS      2 /**< Number of timers */

/*! Result codes for functions */
typedef enum NX51_ARMTIMER_RESULT_Etag {
  NX51_ARMTIMER_OKAY            = 0, /**< Successful          */
  NX51_ARMTIMER_ERR_INVAL_PARAM = 1  /**< Invalid parameter   */
} NX51_ARMTIMER_RESULT_E;

/*! Configuration for ARM timer */
typedef enum NX51_ARMTIMER_CFG_MODE_Etag {
  NX51_ARMTIMER_CFG_MODE_STOP_AT_0    = 0, /**< 0: Timer stops at 0                                         */
  NX51_ARMTIMER_CFG_MODE_PRELOAD_AT_0 = 1, /**< 1: Timer is PRE-load with value from PRE-load register at 0 */
  NX51_ARMTIMER_CFG_MODE_SYSTIME      = 2, /**< 2: Timer (value) compare with system time (once)            */
  NX51_ARMTIMER_CFG_MODE_RESERVED     = 3  /**< 3: reserved                                                 */
} NX51_ARMTIMER_CFG_MODE_E;

/*! Parameters ID that can be get by ArmTimer_GetPrm() */
typedef enum NX51_ARMTIMER_PRM_Etag {
  NX51_ARMTIMER_PRM_CONFIG_TIMER0      = 0, /**< Register arm_timer_config_timer0      */
  NX51_ARMTIMER_PRM_CONFIG_TIMER1         ,  /**< Register arm_timer_config_timer1      */
  NX51_ARMTIMER_PRM_PRELOAD_TIMER0        ,  /**< Register arm_timer_preload_timer0     */
  NX51_ARMTIMER_PRM_PRELOAD_TIMER1        ,  /**< Register arm_timer_preload_timer1     */
  NX51_ARMTIMER_PRM_TIMER0                ,  /**< Register arm_timer_timer0             */
  NX51_ARMTIMER_PRM_TIMER1                ,  /**< Register arm_timer_timer1             */
  NX51_ARMTIMER_PRM_SYSTIME_NS_COMPARE    ,  /**< Register arm_timer_systime_ns_compare */
  NX51_ARMTIMER_PRM_SYSTIME_S_COMPARE     ,  /**< Register arm_timer_systime_s_compare  */
  NX51_ARMTIMER_PRM_IRQMSKSET             ,  /**< interrupt mask enable                 */
  NX51_ARMTIMER_PRM_IRQMSKRST                /**< interrupt mask disable                */
} NX51_ARMTIMER_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
NX51_ARMTIMER_RESULT_E NX51_ArmTimer_Start( unsigned int uInst, NX51_ARMTIMER_CFG_MODE_E eMode, uint32_t ulVal );
NX51_ARMTIMER_RESULT_E NX51_ArmTimer_Stop( unsigned int uInst );
void NX51_ArmTimer_StartSystimeNsCmp( uint32_t ulVal );
void NX51_ArmTimer_StartSystimeSecCmp( uint32_t ulVal );
void NX51_ArmTimer_EnIrq( unsigned int uTimer0IrqEn, unsigned int uTimer1IrqEn, unsigned int uSystimeNsIrqEn, unsigned int uSystimeSecIrqEn );
void NX51_ArmTimer_DisIrq( unsigned int uTimer0IrqDis, unsigned int uTimer1IrqDis, unsigned int uSystimeNsIrqDis, unsigned int uSystimeSecIrqDis );
void NX51_ArmTimer_GetIrq( uint32_t* pulIrq );
void NX51_ArmTimer_GetIrqRaw( uint32_t* pulIrq );
void NX51_ArmTimer_ConfirmIrq( uint32_t ulConfirmIrqMask );
NX51_ARMTIMER_RESULT_E NX51_ArmTimer_GetPrm( NX51_ARMTIMER_PRM_E ePrmID, uint32_t* pulPrmVal );

#endif /* #ifndef __NETX51_ARMTIMER_H */
