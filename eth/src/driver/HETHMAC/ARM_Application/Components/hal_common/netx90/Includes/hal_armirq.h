#ifndef __HAL_ARMIRQ_H
#define __HAL_ARMIRQ_H

#include "compiler_spec.h"
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define lock_irqfiq_save(x) (x = ArmIrq_LockIrqSave())
#define lock_irqfiq_restore(x)  (ArmIrq_LockIrqRestore(x))

#ifdef __ARM_COMPILER__

/* Interrupt attribute for ARM compiler */
#define IRQFIQ_LOCK     __asm { CPSID i }
#define IRQFIQ_UNLOCK   __asm { CPSIE i }

#else

/* Interrupt attribute for GNU compiler */
#define IRQFIQ_LOCK     __asm__ __volatile__("CPSID i");
#define IRQFIQ_UNLOCK   __asm__ __volatile__("CPSIE i");

#define ABORT_UNLOCK                             \
                __asm__ __volatile__(                   \
                        "mrs r12,     cpsr\n"           \
                        "bic r12,     r12, #0x100\n"    \
                        "msr cpsr_x,  r12\n"            \
                        ::: "r12", "cc"                 \
                );

#endif

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

#ifndef HALDEC
#define HALDEC(name) name
#endif

unsigned int      HALDEC(ArmIrq_LockIrqSave)           ( void );
void              HALDEC(ArmIrq_LockIrqRestore)        ( unsigned int x );
void              HALDEC(ArmIrq_SetExceptionHandler)   ( ARMIRQ_EXCEPTION_E eType, uint32_t ulHandlerAddr );
void              HALDEC(ArmIrq_IrqHandler)            ( void );
void              HALDEC(ArmIrq_Init)                  ( void );
void              HALDEC(ArmIrq_Deinit)                ( void );
uint32_t          HALDEC(ArmIrq_HighestPendingIrq)     ( void );
void              HALDEC(ArmIrq_SetIsr)                ( unsigned int uIrqId, uint32_t ulIsrAddr, unsigned int uPriority );
void              HALDEC(ArmIrq_SetEnable)             ( unsigned int uIrqId, bool fEnable );
void              HALDEC(ArmIrq_SetActive)             ( unsigned int uIrqId, bool fEnable );
bool              HALDEC(ArmIrq_IsPending)             ( unsigned int uIrqId );
void              HALDEC(ArmIrq_Confirm)               ( unsigned int uIrqId );

#endif /* #ifndef __HAL_ARMIRQ_H */
