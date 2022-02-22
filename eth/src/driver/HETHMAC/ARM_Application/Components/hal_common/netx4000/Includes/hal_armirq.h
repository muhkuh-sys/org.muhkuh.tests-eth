#ifndef __HAL_ARMIRQ_H
#define __HAL_ARMIRQ_H

#include "compiler_spec.h"

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define lock_irqfiq_save(x) (x = ArmIrq_LockIrqSave())
#define lock_irqfiq_restore(x)  (ArmIrq_LockIrqRestore(x))

#ifdef __ARM_COMPILER__

/* Interrupt attribute for ARM compiler */
#define IRQFIQ_LOCK     __asm { MSR CPSR_c, #0xdf }
#define IRQFIQ_UNLOCK   __asm { MSR CPSR_c, #0x1f }

#else

/* Interrupt attribute for GNU compiler */
#define IRQFIQ_LOCK     asm ( "msr cpsr_c, #0xdf" );
#define IRQFIQ_UNLOCK   asm ( "msr cpsr_c, #0x1f" );

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

#ifndef HALDEC
#define HALDEC(name) name
#endif

unsigned int      HALDEC(ArmIrq_LockIrqSave)           ( void );
void              HALDEC(ArmIrq_LockIrqRestore)        ( unsigned int uiState );
void              HALDEC(ArmIrq_SetExceptionHandler)   ( ARMIRQ_EXCEPTION_E eType, uint32_t ulHandlerAddr );
void INTERRUPT    HALDEC(ArmIrq_IrqHandler)            ( void );
void              HALDEC(ArmIrq_Init)                  ( void );
void              HALDEC(ArmIrq_Deinit)                ( void );
uint32_t          HALDEC(ArmIrq_HighestPendingIrq)     ( void );
void              HALDEC(ArmIrq_SetIsr)                ( unsigned int uIrqId, uint32_t ulIsrAddr, unsigned int uPriority );
void              HALDEC(ArmIrq_SetEnable)             ( unsigned int uIrqId, bool fEnable );
void              HALDEC(ArmIrq_SetActive)             ( unsigned int uIrqId, bool fEnable );
bool              HALDEC(ArmIrq_IsPending)             ( unsigned int uIrqId );
void              HALDEC(ArmIrq_Confirm)               ( unsigned int uIrqId );

#endif /* #ifndef __HAL_ARMIRQ_H */
