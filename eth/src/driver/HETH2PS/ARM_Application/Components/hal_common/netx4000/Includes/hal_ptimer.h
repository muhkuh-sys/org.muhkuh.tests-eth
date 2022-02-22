#ifndef __HAL_PTIMER_H
#define __HAL_PTIMER_H

#include <stdint.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*! Configuration for ARM timer */
typedef enum
{
  PTIMER_CFG_MODE_STOP_AT_0      = 0, /**< 0: Timer stops at 0                                       */
  PTIMER_CFG_MODE_PRELOAD_AT_0   = 1  /**< 1: Timer is preload with value from preload register at 0 */
} PTIMER_CFG_MODE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void      PTimer_Start     ( unsigned int uiTimer, PTIMER_CFG_MODE_E eMode, uint32_t ulVal );
void      PTimer_Stop      ( unsigned int uiTimer );
void      PTimer_Sleep     ( unsigned int uiTimer, uint32_t ulTimeout10Ns );
uint32_t  PTimer_GetTimer  ( unsigned int uiTimer );
uint32_t  PTimer_GetIrqRaw ( unsigned int uiTimer );
void      PTimer_CnfIrq    ( unsigned int uiTimer, uint32_t ulIrq );

void      PWdg_Start     ( uint32_t ulVal );
void      PWdg_Trigger   ( void );
void      PWdg_Stop      ( void );
uint32_t  PWdg_GetIrqRaw ( void );
void      PWdg_CnfIrq    ( uint32_t ulIrq );

#endif /* #ifndef __HAL_PTIMER_H */
