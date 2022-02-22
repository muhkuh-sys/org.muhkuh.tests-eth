
/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_armirq.h"
#include "hw_defines.h"
#include <string.h>

/* Interrupt controller interface registers */
#define GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_128_LEVELS   0xFE /* Bit[0] = 0b0 -> 128 supported levels */
#define GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_64_LEVELS    0xFC /* Bit[1:0] = 0b00 -> 64 supported levels */
#define GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_32_LEVELS    0xF8 /* Bit[2:0] = 0b000 -> 32 supported levels */
#define GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_16_LEVELS    0xF0 /* Bit[3:0] = 0b0000 -> 16 supported levels */
#define GIC_CONTROLLER_PRIORITY_MASK_HIGHEST_PRIORITY     0x00 /* Highest priority */
#define GIC_CONTROLLER_PRIORITY_MASK_LOWEST_PRIORITY      0x80 /* Lowest priority */

/* Interrupt distributor interface registers */
#define GIC_DISTRIBUTOR_INT_SET_ENABLE_INTERRUPT           0x1 /* Set enable interrupt */
#define GIC_DISTRIBUTOR_INT_CLEAR_ENABLE_INTERRUPT         0x0 /* Clear enable interrupt */

#define GIC_DISTRIBUTOR_INT_PRIORITY_LOWEST_CONFIG        0x00 /* Lower configuration border */
#define GIC_DISTRIBUTOR_INT_PRIORITY_HIGHEST_CONFIG       0xFF /* Upper configuration border */

/* Generic values */
#define GIC_CPU_INTERRUPT_ID_LOWER_BORDER                   31 /* Interrupt IDs 00..31 are reserved - SPI 00..31 */
#define GIC_CPU_INTERRUPT_ID_UPPER_BORDER                  255 /* Interrupt IDs 32..511 can be used - PPI 00..479 */
#define GIC_TOTAL_NUMBER_OF_INTERRUPTS                     256 /* 0..255 */
#define GIC_NO_INTERRUPT_IS_PENDING                      0x3ff /* No interrupt is pending */
#define GIC_SET_BIT                                        0x1 /* Set one bit */
#define GIC_ENABLE                                         0x1 /* Generic enable bit */
#define GIC_DISABLE                                        0x0 /* Generic disable bit */


/* 2 bit configuration for each peripheral IRQ */
static const uint32_t s_aulIcdCfg[] =
{
//         0          16          32          48
  0xff7f55ff, 0x55555557, 0x55555555, 0x55555555,
//        64          80          96         112
  0x5555fd55, 0x55555555, 0xf5555555, 0xffffffff,
//       128         144         160         176
  0x55555fff, 0x5555fd55, 0x55555555, 0x55555555,
//       192         208
  0x55555555, 0x55555555,
};

/* Access pointers */
HW_PTR_IC_CPU_IF(s_ptGicIf)   /* Interrupt controller CPU interface */
HW_PTR_IC_DIST(s_ptGicDist)     /* Interrupt distributor */

uint32_t ppa_gic_func_array [ GIC_TOTAL_NUMBER_OF_INTERRUPTS ] = {4UL};

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

/*lint -e{413} intended use of null-pointer */
void ArmIrq_SetExceptionHandler( ARMIRQ_EXCEPTION_E eType, uint32_t ulHandlerAddr )
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
void INTERRUPT ArmIrq_IrqHandler(void)
{
  uint32_t ulIrqId, ulIrqAck;
  void (*fnIsr)(uint32_t ulIrqNo);

  /* Get the highest pending interrupt id */
  ulIrqAck = s_ptGicIf->ulIcci_int_ack;
  ulIrqId  = ulIrqAck & HW_MSK(icci_int_ack_intid);

  /* Check if an interrupt is pending */
  while( ulIrqId != GIC_NO_INTERRUPT_IS_PENDING )
  {
    /* Call the specific interrupt service routine */
    fnIsr = (void (*)(uint32_t))ppa_gic_func_array[ulIrqId];
    fnIsr(ulIrqAck);

    /* Deactivate IRQ */
    s_ptGicIf->ulIcci_int_end = ulIrqAck;

    /* Check if there is one more interrupt pending (get the highest pending interrupt id) */
    ulIrqAck = s_ptGicIf->ulIcci_int_ack;
    ulIrqId  = ulIrqAck & HW_MSK(icci_int_ack_intid);
  }
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
  /* Enable interrupt handling */
  unsigned int uiIdx;

  /* Enable the CPU interface */
  s_ptGicIf->ulIcci_ctrl = 3;

  /* Reset priority mask */
  s_ptGicIf->ulIcci_prio_msk = GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_128_LEVELS;

  /* Enable the interrupt distributor interface */
  s_ptGicDist->ulIcd_ctrl = 3;

  /* configure edge triggered IRQs */
  for(uiIdx = 0; uiIdx < (sizeof(s_aulIcdCfg)/sizeof(s_aulIcdCfg[0])); ++uiIdx)
  {
    s_ptGicDist->aulIcd_cfg[uiIdx + 2] = s_aulIcdCfg[uiIdx];
  }

  /* set all IRQ targets to all CPUs */
  /* Note: This enables the IRQs to trigger the pending state on CA9 */
  for(uiIdx = 0; uiIdx < 64; ++uiIdx)
  {
    s_ptGicDist->aulIcd_processor_trg[uiIdx] = 0xffffffffUL;
  }

  ArmIrq_SetExceptionHandler(ARMIRQ_EXCEPTION_IRQ, (uint32_t)ArmIrq_IrqHandler);
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
  unsigned int uiIdx;
  uint32_t ulIrqAck;

  /* Disable the interrupt distributor interface */
  s_ptGicDist->ulIcd_ctrl = 0;

  for(uiIdx = 0; uiIdx < (sizeof(s_ptGicDist->aulIcd_clr_en)/sizeof(s_ptGicDist->aulIcd_clr_en[0])); ++uiIdx)
  {
    s_ptGicDist->aulIcd_clr_en[uiIdx] = 0xffffffff;
  }

  /* Disable the CPU interface */
  s_ptGicIf->ulIcci_ctrl = 0;

  /* end all active IRQs */
  for(ulIrqAck = 0; ulIrqAck < GIC_TOTAL_NUMBER_OF_INTERRUPTS; ++ulIrqAck)
  {
    s_ptGicIf->ulIcci_int_end = ulIrqAck;
  }
}

/*****************************************************************************/
/*! Set Interrupt Service Routine
* \description
*   Setup an interrupt service routine and enable it for the given interrupt source and priority.
* \class
*   IrqCtrl
* \params
*   uiIrqId       [in] Interrupt Source
*   ulIsrAddr     [in] Address (entry point) of the ISR (NULL disables the interrupt for the given priority)
*   uiPriority    [in] Interrupt priority (0 is highest priority)
* \return
*                                                                            */
/*****************************************************************************/
void ArmIrq_SetIsr(unsigned int uiIrqId, uint32_t ulIsrAddr, unsigned int uiPriority)
{
  /* Setup the interrupt -> Must be level sensitive here (because of setting and resetting interrupt source) */

  unsigned int uiSrt;
  unsigned int uiIdx;
  uint32_t ulVal;

  /* Check interrupt number (uIrqNo) parameter */
  if( uiIrqId > GIC_CPU_INTERRUPT_ID_UPPER_BORDER )
  {
    return;
  }

  /* Check priority number (ulPriority) parameter */
  if( uiPriority > GIC_DISTRIBUTOR_INT_PRIORITY_HIGHEST_CONFIG )
  {
    return;
  }

  uiSrt = uiIrqId & 0x1f;
  uiIdx = uiIrqId >> 5;

  /* Disable the interrupt */
  s_ptGicDist->aulIcd_clr_en[uiIdx] = 1UL << uiSrt;

  /* Configure the priority */
  uiSrt = (uiIrqId & 0x3) << 3;
  uiIdx = uiIrqId >> 2;

  /* Configuration 4 bit registers - read -> modify -> write is needed here */
  ulVal = s_ptGicDist->aulIcd_prio[uiIdx] & ~(0xfUL << uiSrt);   /* Read and clear selected bits */
  s_ptGicDist->aulIcd_prio[uiIdx] = ulVal | ((uint32_t)uiPriority << uiSrt);

  /* Configure the processor target */
  /* Configuration 4 bit registers - read -> modify -> write is needed here */
  ulVal = s_ptGicDist->aulIcd_processor_trg[uiIdx] & ~(0xffUL << uiSrt);   /* Read and clear selected bits */
  s_ptGicDist->aulIcd_processor_trg[uiIdx] = ulVal | (0xff << uiSrt);

  /* Install the service routine */
  ppa_gic_func_array[uiIrqId] = ulIsrAddr;
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
  unsigned int uiSrt = uiIrqId & 0x1f;
  unsigned int uiIdx = uiIrqId >> 5;

  if( fEnable )
  {
    s_ptGicDist->aulIcd_set_en[uiIdx] = 1UL << uiSrt;
  }
  else
  {
    s_ptGicDist->aulIcd_clr_en[uiIdx] = 1UL << uiSrt;
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
*   uiIrqId       [in] Interrupt source
*   fEnable       [in] true/false: Activate/deactivate interrupt source
* \return
*                                                                            */
/*****************************************************************************/
void ArmIrq_SetActive(unsigned int uiIrqId, bool fEnable)
{
  unsigned int uiSrt = uiIrqId & 0x1f;
  unsigned int uiIdx = uiIrqId >> 5;

  if( fEnable )
  {
    s_ptGicDist->aulIcd_set_pending[uiIdx] = 1UL << uiSrt;
  }
  else
  {
    s_ptGicDist->aulIcd_clr_pending[uiIdx] = 1UL << uiSrt;
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
  ArmIrq_SetActive(uiIrqId, false);
}


/*****************************************************************************/
/*! Get Raw Interrupt
* \description
*   Checks whether the given interrupt source is pending.
* \class
*   IrqCtrl
* \params
*   uiIrqId       [in] Interrupt source
* \return
*   true/false: IRQ is currently active/inactive                             */
/*****************************************************************************/
bool ArmIrq_IsPending(unsigned int uiIrqId)
{
  /* Return status in boolean format */
  unsigned int uiSrt = uiIrqId & 0x1f;
  unsigned int uiIdx = uiIrqId >> 5;
  uint32_t ulVal = s_ptGicDist->aulIcd_set_pending[uiIdx];

  /* Shift the value to position 0 (to return 0 or 1)*/
  return ( 0 != ((ulVal >> uiSrt) & 1) );
}

uint32_t ArmIrq_HighestPendingIrq(void)
{
  return s_ptGicIf->ulIcci_highest_pending;
}
