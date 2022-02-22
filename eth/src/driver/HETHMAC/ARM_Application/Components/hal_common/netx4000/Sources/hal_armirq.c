
/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_armirq.h"
#include "hal_gic.h"
#include "hw_defines.h"
#include <string.h>


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Install ISR Vector
* \description
*   Installs the ISR vector.
* \class 
*   VIC 
* \params
*   ulVector              [in]  ISR Vector Address
* \return
*                                                                            */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

/*lint -e{413} intended use of null-pointer */
void HALDEF(ArmIrq_SetExceptionHandler)( ARMIRQ_EXCEPTION_E eType, uint32_t ulHandlerAddr )
{
  volatile uint32_t* pulExcTable = (volatile uint32_t*)0;

  pulExcTable[eType] = 0xe59ff014;
  pulExcTable[eType + 7] = ulHandlerAddr;
}


/*****************************************************************************/
/*! IRQ Vector Handler
* \description
*   This function reads the interrupt vector to find out where to jump to.
*   Jump to ISR.
* \class 
*   VIC 
* \params
* \return
*                                                                            */
/*****************************************************************************/
void INTERRUPT HALDEF(ArmIrq_IrqHandler)(void)
{
  /* Jump to default GIC handler */
  Gic_DefaultIrqHandler();
}


/*****************************************************************************/
/*! Initialize IRQ controller
* \description
*   Initializes the interrupt controller.
* \class 
*   IrqCtrl
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmIrq_Init)(void)
{
  /* Enable interrupt handling */
  Gic_DistReset();
  Gic_CpuiInit();
  Gic_CpuiSetPriorityMask(GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_128_LEVELS);
  Gic_DistInit();

  ArmIrq_SetExceptionHandler(ARMIRQ_EXCEPTION_IRQ, (uint32_t)ArmIrq_IrqHandler);

  //Gic_DistCfg();
}

/*****************************************************************************/
/*! Deinitialize IRQ controller
* \description
*   Initializes the interrupt controller.
* \class 
*   IrqCtrl
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmIrq_Deinit)(void)
{
  /* Disable interrupt handling */
  Gic_DistReset();
  Gic_CpuiReset();
}

/*****************************************************************************/
/*! Set Interrupt Service Routine
* \description
*   Setup an interrupt service routine and enable it for the given interrupt source and priority.
* \class 
*   IrqCtrl
* \params
*   uIrqId        [in] Interrupt Source
*   ulIsrAddr     [in] Address (entry point) of the ISR (NULL disables the interrupt for the given priority)
*   uPriority     [in] Interrupt priority (0 is highest priority)
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmIrq_SetIsr)(unsigned int uIrqId, uint32_t ulIsrAddr, unsigned int uPriority)
{
  /* Setup the interrupt -> Must be level sensitive here (because of setting and resetting interrupt source) */

  /* Disable the interrupt */
  Gic_DistSetEnable(uIrqId, false);

  /* Configure the priority */
  Gic_DistSetPriority(uIrqId, uPriority);

  /* Configure the processor target */
  Gic_DistSetProcessorTarget(uIrqId, 0xff);

  /* Install the service routine */
  Gic_DistSetIsr(uIrqId, ulIsrAddr);
}


/*****************************************************************************/
/*! Set Interrupt Enable
* \description
*   Enables or disables the given interrupt source.
* \class 
*   IrqCtrl
* \params
*   uIrqId        [in] Interrupt source
*   fEnable       [in] true/false: Enable/disable interrupt source
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmIrq_SetEnable)(unsigned int uIrqId, bool fEnable)
{
  Gic_DistSetEnable(uIrqId, fEnable);
}


/*****************************************************************************/
/*! Set Interrupt Active
* \description
*   Forces a given interrupt source into pending state.
*   This is used for software interrupts and ISR testing.
* \class
*   IrqCtrl
* \params
*   uIrqId        [in] Interrupt source
*   fEnable       [in] true/false: Activate/deactivate interrupt source
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmIrq_SetActive)(unsigned int uIrqId, bool fEnable)
{
  Gic_DistSetPending( uIrqId, fEnable );
}


/*****************************************************************************/
/*! Confirm Interrupt
* \description
*   Reset a pending IRQ from the given interrupt source.
* \class 
*   IrqCtrl
* \params
*   uIrqId        [in] Interrupt source
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmIrq_Confirm)(unsigned int uIrqId)
{
  Gic_DistSetPending(uIrqId, false);
}


/*****************************************************************************/
/*! Get Raw Interrupt
* \description
*   Checks whether the given interrupt source is pending.
* \class 
*   IrqCtrl
* \params
*   uIrqId        [in] Interrupt source
* \return
*   true/false: IRQ is currently active/inactive                             */
/*****************************************************************************/
bool HALDEF(ArmIrq_IsPending)(unsigned int uIrqId)
{
  /* Return status in boolean format */
  return Gic_DistIsPending(uIrqId);
}

uint32_t HALDEF(ArmIrq_HighestPendingIrq)(void)
{
  return Gic_CpuiHighestPendingIrq();
}

unsigned int ArmIrq_LockIrqSave(void)
{
  register unsigned int uiCPSR;
  register unsigned int uiState;
#ifdef __ARM_COMPILER__
    __asm ( "MRS uiCPSR, CPSR" );
    __asm ( "AND uiState, uiCPSR, #0xC0");
    __asm ( "ORR uiCPSR, uiCPSR, #0xC0");
    __asm ( "MSR CPSR_c, uiCPSR");
#else
    __asm__ __volatile__(
      "MRS %1, cpsr\n"
      "AND %0, %1, #0xC0\n"
      "ORR %1, %1, #0xC0\n"
      "MSR CPSR_c, %1"
      : "=r" (uiState), "=&r" (uiCPSR)
    );
#endif
    return uiState;
}

void ArmIrq_LockIrqRestore(unsigned int uiState)
{
    register unsigned int uiCPSR;
#ifdef __ARM_COMPILER__
    __asm ( "MRS uiCPSR, CPSR");
    __asm ( "EOR uiCPSR, uiCPSR, #0xC0");
    __asm ( "ORR uiCPSR, uiCPSR, (uiState)");
    __asm ( "MSR CPSR_c, uiCPSR");
#else
    __asm__ __volatile__(
      "MRS %0, cpsr\n"
      "EOR %0, %0, #0xC0\n"
      "ORR %0, %0, %1\n"
      "MSR CPSR_c, %0"
      : "=&r" (uiCPSR)
      : "r" (uiState)
    );
#endif
}
