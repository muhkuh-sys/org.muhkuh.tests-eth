
/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_armirq.h"
#include "hw_defines.h"
#include <string.h>

/* stolen from CMSIS ... */

#define __NVIC_PRIO_BITS               3            /*!< Number of Bits used for Priority Levels                               */

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */

/** \brief  Structure type to access the Nested Vectored Interrupt Controller (NVIC).
 */
typedef struct
{
  __IO uint32_t ISER[8];                 /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register           */
       uint32_t RESERVED0[24];
  __IO uint32_t ICER[8];                 /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register         */
       uint32_t RSERVED1[24];
  __IO uint32_t ISPR[8];                 /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register          */
       uint32_t RESERVED2[24];
  __IO uint32_t ICPR[8];                 /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register        */
       uint32_t RESERVED3[24];
  __IO uint32_t IABR[8];                 /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register           */
       uint32_t RESERVED4[56];
  __IO uint8_t  IP[240];                 /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
       uint32_t RESERVED5[644];
  __O  uint32_t STIR;                    /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register     */
}  NVIC_Type;

/** \brief  Structure type to access the System Control Block (SCB).
 */
typedef struct
{
  __I  uint32_t CPUID;                   /*!< Offset: 0x000 (R/ )  CPUID Base Register                                   */
  __IO uint32_t ICSR;                    /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register                  */
  __IO uint32_t VTOR;                    /*!< Offset: 0x008 (R/W)  Vector Table Offset Register                          */
  __IO uint32_t AIRCR;                   /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register      */
  __IO uint32_t SCR;                     /*!< Offset: 0x010 (R/W)  System Control Register                               */
  __IO uint32_t CCR;                     /*!< Offset: 0x014 (R/W)  Configuration Control Register                        */
  __IO uint8_t  SHP[12];                 /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IO uint32_t SHCSR;                   /*!< Offset: 0x024 (R/W)  System Handler Control and State Register             */
  __IO uint32_t CFSR;                    /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register                    */
  __IO uint32_t HFSR;                    /*!< Offset: 0x02C (R/W)  HardFault Status Register                             */
  __IO uint32_t DFSR;                    /*!< Offset: 0x030 (R/W)  Debug Fault Status Register                           */
  __IO uint32_t MMFAR;                   /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register                      */
  __IO uint32_t BFAR;                    /*!< Offset: 0x038 (R/W)  BusFault Address Register                             */
  __IO uint32_t AFSR;                    /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register                       */
  __I  uint32_t PFR[2];                  /*!< Offset: 0x040 (R/ )  Processor Feature Register                            */
  __I  uint32_t DFR;                     /*!< Offset: 0x048 (R/ )  Debug Feature Register                                */
  __I  uint32_t ADR;                     /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register                            */
  __I  uint32_t MMFR[4];                 /*!< Offset: 0x050 (R/ )  Memory Model Feature Register                         */
  __I  uint32_t ISAR[5];                 /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register                   */
       uint32_t RESERVED0[5];
  __IO uint32_t CPACR;                   /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register                   */
} SCB_Type;

#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address  */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address  */
#define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address                  */

#define SCB                 ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct           */
#define NVIC                ((NVIC_Type      *)     NVIC_BASE     )   /*!< NVIC configuration struct          */

extern int stack_top; /* provided by linker script */
void start(void); /* provided by initialization code */

/* Vector Table in SRAM */
volatile uint32_t __Vectors[96] __attribute__ ((section(".vectors_ram"))) = {(uint32_t)&stack_top, (uint32_t)start};

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
void ArmIrq_SetExceptionHandler( ARMIRQ_EXCEPTION_E eType, uint32_t ulHandlerAddr )
{
  __Vectors[eType] = ulHandlerAddr;
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
void ArmIrq_IrqHandler(void)
{
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
void ArmIrq_Init(void)
{
  unsigned int uiCnt;

  /* Set priority code point to 6: 1 bit pre-emption (nested) priority, seven bits subpriority */
  SCB->AIRCR = (0x5faUL << SRT_NX90_cm4_scs_aircr_vectkey) | (6UL << SRT_NX90_cm4_scs_aircr_prigroup);

  /* Use default IRQ Handler for all Interrupts */
  for(uiCnt = 2; uiCnt < (sizeof(__Vectors)/sizeof(__Vectors[0])); ++uiCnt)
  {
    __Vectors[uiCnt] = (uint32_t)ArmIrq_IrqHandler;
  }

  /* Data Memory Barrier to ensure write to memory is completed */
  __DMB();

  /* Set VTOR to the new vector table location */
  SCB->VTOR = (uint32_t)__Vectors;

  /* Data Synchronization Barrier to ensure all subsequent instructions
   * use the new configuration
   */
  __DSB();
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
void ArmIrq_Deinit(void)
{
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
void ArmIrq_SetIsr(unsigned int uiIrqId, uint32_t ulIsrAddr, unsigned int uPriority)
{
  __Vectors[uiIrqId + 16] = ulIsrAddr;
  NVIC->IP[((uint32_t)(int32_t)uiIrqId)] = (uint8_t)((uPriority << (8 - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
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
void ArmIrq_SetEnable(unsigned int uiIrqId, bool fEnable)
{
  if(fEnable)
  {
    NVIC->ISER[(((uint32_t)(int32_t)uiIrqId) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)uiIrqId) & 0x1FUL));
  }
  else
  {
    NVIC->ICER[(((uint32_t)(int32_t)uiIrqId) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)uiIrqId) & 0x1FUL));
  }
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
void ArmIrq_SetActive(unsigned int uiIrqId, bool fEnable)
{
  if(fEnable)
  {
    NVIC->ISPR[(((uint32_t)(int32_t)uiIrqId) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)uiIrqId) & 0x1FUL));
  }
  else
  {
    NVIC->ICPR[(((uint32_t)(int32_t)uiIrqId) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)uiIrqId) & 0x1FUL));
  }
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
void ArmIrq_Confirm(unsigned int uiIrqId)
{
  NVIC->ICPR[(((uint32_t)(int32_t)uiIrqId) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)uiIrqId) & 0x1FUL));
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
bool ArmIrq_IsPending(unsigned int uiIrqId)
{
  return((NVIC->ISPR[(((uint32_t)(int32_t)uiIrqId) >> 5UL)] & (1UL << (((uint32_t)(int32_t)uiIrqId) & 0x1FUL))) != 0UL);
}

uint32_t ArmIrq_HighestPendingIrq(void)
{
  return 0;
}
