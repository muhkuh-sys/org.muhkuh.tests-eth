#ifndef __HAL_ARMIRQ_H
#define __HAL_ARMIRQ_H

#include "compiler_spec.h"

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum
{
  ARMIRQ_EXCEPTION_RESET          = 0,
  ARMIRQ_EXCEPTION_UNDEFINED_INST = 1,
  ARMIRQ_EXCEPTION_SWI            = 2,
  ARMIRQ_EXCEPTION_PREFETCH_ABORT = 3,
  ARMIRQ_EXCEPTION_DATA_ABORT     = 4,
  ARMIRQ_EXCEPTION_RESERVED       = 5,
  ARMIRQ_EXCEPTION_IRQ            = 6,
  ARMIRQ_EXCEPTION_FIQ            = 7
} ARMIRQ_EXCEPTION_E;


/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/

void              ArmIrq_SetExceptionHandler   ( ARMIRQ_EXCEPTION_E eType, uint32_t ulHandlerAddr );
void INTERRUPT    ArmIrq_IrqHandler            ( void );
void              ArmIrq_Init                  ( void );
void              ArmIrq_Deinit                ( void );
uint32_t          ArmIrq_HighestPendingIrq     ( void );
void              ArmIrq_SetIsr                ( unsigned int uiIrqId, uint32_t ulIsrAddr, unsigned int uiPriority );
void              ArmIrq_SetEnable             ( unsigned int uiIrqId, bool fEnable );
void              ArmIrq_SetActive             ( unsigned int uiIrqId, bool fEnable );
bool              ArmIrq_IsPending             ( unsigned int uiIrqId );
void              ArmIrq_Confirm               ( unsigned int uiIrqId );

#endif /* #ifndef __HAL_ARMIRQ_H */
