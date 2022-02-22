#ifndef __HAL_ARMIRQ_H
#define __HAL_ARMIRQ_H

#include "compiler_spec.h"
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum
{
  ARMIRQ_EXCEPTION_RESET          = 1,
  ARMIRQ_EXCEPTION_NMI            = 2,
  ARMIRQ_EXCEPTION_HARD_FAULT     = 3,
  ARMIRQ_EXCEPTION_MEM_MANAGE     = 4,
  ARMIRQ_EXCEPTION_BUS_FAULT      = 5,
  ARMIRQ_EXCEPTION_USAGE_FAULT    = 6,
  ARMIRQ_EXCEPTION_SVC            = 11,
  ARMIRQ_EXCEPTION_DEBUG_MONITOR  = 12,
  ARMIRQ_EXCEPTION_PENDING_SV     = 14,
  ARMIRQ_EXCEPTION_SYSTICK        = 15
} ARMIRQ_EXCEPTION_E;


/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/

void              ArmIrq_SetExceptionHandler   ( ARMIRQ_EXCEPTION_E eType, uint32_t ulHandlerAddr );
void              ArmIrq_IrqHandler            ( void );
void              ArmIrq_Init                  ( void );
void              ArmIrq_Deinit                ( void );
uint32_t          ArmIrq_HighestPendingIrq     ( void );
void              ArmIrq_SetIsr                ( unsigned int uiIrqId, uint32_t ulIsrAddr, unsigned int uiPriority );
void              ArmIrq_SetEnable             ( unsigned int uiIrqId, bool fEnable );
void              ArmIrq_SetActive             ( unsigned int uiIrqId, bool fEnable );
bool              ArmIrq_IsPending             ( unsigned int uiIrqId );
void              ArmIrq_Confirm               ( unsigned int uiIrqId );

#endif /* #ifndef __HAL_ARMIRQ_H */
