#include "hal_gic.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

/* Jump table (function pointer array) */
uint32_t ppa_gic_func_array [ GIC_TOTAL_NUMBER_OF_INTERRUPTS ] = {4UL};


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


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/


#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(Gic_CpuiInit)(void)
{
  /* Enable the CPU interface */
  s_ptGicIf->ulIcci_ctrl = 3;
}

void HALDEF(Gic_CpuiReset)(void)
{
  uint32_t ulIrqAck;

  /* Disable the CPU interface */
  s_ptGicIf->ulIcci_ctrl = 0;

  /* end all active IRQs */
  for(ulIrqAck = 0; ulIrqAck < GIC_TOTAL_NUMBER_OF_INTERRUPTS; ++ulIrqAck)
  {
    s_ptGicIf->ulIcci_int_end = ulIrqAck;
  }
}


void HALDEF(Gic_DistInit)(void)
{
  unsigned int uIdx;

  /* Enable the interrupt distributor interface */
  s_ptGicDist->ulIcd_ctrl = 3;

  /* configure edge triggered IRQs */
  for(uIdx = 0; uIdx < (sizeof(s_aulIcdCfg)/sizeof(s_aulIcdCfg[0])); ++uIdx)
  {
    s_ptGicDist->aulIcd_cfg[uIdx + 2] = s_aulIcdCfg[uIdx];
  }

  /* set all IRQ targets to all CPUs */
  /* Note: This enables the IRQs to trigger the pending state on CA9 */
  for(uIdx = 0; uIdx < 64; ++uIdx)
  {
    s_ptGicDist->aulIcd_processor_trg[uIdx] = 0xffffffffUL;
  }
}

void HALDEF(Gic_DistReset)(void)
{
  unsigned int uiIdx;

  /* Disable the interrupt distributor interface */
  s_ptGicDist->ulIcd_ctrl = 0;

  for(uiIdx = 0; uiIdx < (sizeof(s_ptGicDist->aulIcd_clr_en)/sizeof(s_ptGicDist->aulIcd_clr_en[0])); ++uiIdx)
  {
    s_ptGicDist->aulIcd_clr_en[uiIdx] = 0xffffffff;
  }

}

/*****************************************************************************/
/*! HALDEF(Gic_CpuiSetPriorityMask): Select priority support/level
* \description
*   The priority mask level for the CPU interface. If the priority of an 
*   interrupt is higher than the value indicated by this field, the interface 
*   signals the interrupt to the processor. Lower priority value means higher 
*   interrupt priority. E.g. 0 is the highest priority.
* \class 
*   GIC 
* \params
*   ulPriority    [in] Select priority
* \return
*   GIC_RESULT_E                                                      */
/*****************************************************************************/
GIC_RESULT_E HALDEF(Gic_CpuiSetPriorityMask)(uint32_t ulPriorityMsk)
{
  /* Check priority mask (ulPriorityConfiguration) parameter */
  if(
      ulPriorityMsk != GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_128_LEVELS    &&
      ulPriorityMsk != GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_64_LEVELS     &&
      ulPriorityMsk != GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_32_LEVELS     &&
      ulPriorityMsk != GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_16_LEVELS
    )
  {
    return GIC_ERR_INVAL_PARAM;
  }

  /* Do access */
  s_ptGicIf->ulIcci_prio_msk = ulPriorityMsk;
  
  /* Controller was successfully configured */
  return GIC_OKAY;
}


/*****************************************************************************/
/*! HALDEF(Gic_DistSetEnable): Enable interrupt N
* \description
*   This functions writes a 1 into the appropriate register (and position).
*   Writing 1 to a Set-enable bit enables forwarding of the corresponding 
*   interrupt from the Distributor to the CPU interfaces. Reading a bit 
*   identifies whether the interrupt is enabled.
* \class 
*   GIC 
* \params
*   uIrqNo       [in] Interrupt number to enable
* \return
*   GIC_RESULT_E                                                      */
/*****************************************************************************/
GIC_RESULT_E HALDEF(Gic_DistSetEnable)(unsigned int uIrqNo, bool fEnable)
{
  unsigned int uSrt = uIrqNo & 0x1f;
  unsigned int uIdx = uIrqNo >> 5;

  /* Check interrupt number (uIrqNo) parameter */
  if( uIrqNo > GIC_CPU_INTERRUPT_ID_UPPER_BORDER )
  {
    return GIC_ERR_INVAL_PARAM;
  }
  
  if( fEnable )
  {
    s_ptGicDist->aulIcd_set_en[uIdx] = 1UL << uSrt;
  }
  else
  {
    s_ptGicDist->aulIcd_clr_en[uIdx] = 1UL << uSrt;
  }

  /* Controller was successfully configured */
  return GIC_OKAY;
}


/*****************************************************************************/
/*! HALDEF(Gic_DistSetPending): Set interrupt N to pending
* \description
*   This functions writes a 1 into the appropriate register (and position).
*   Writing 1 to a Set-pending bit sets the status of the corresponding 
*   peripheral interrupt to pending. Reading a bit identifies whether the
*   interrupt is pending.
* \class 
*   GIC 
* \params
*   uIrqNo       [in] Set this interrupt to pending
* \return
*   GIC_RESULT_E                                                      */
/*****************************************************************************/
GIC_RESULT_E HALDEF(Gic_DistSetPending)(unsigned int uIrqNo, bool fEnable)
{
  unsigned int uSrt = uIrqNo & 0x1f;
  unsigned int uIdx = uIrqNo >> 5;
  
  /* Check interrupt number (uIrqNo) parameter */
  if( uIrqNo > GIC_CPU_INTERRUPT_ID_UPPER_BORDER )
  {
    return GIC_ERR_INVAL_PARAM;
  }
  
  if( fEnable )
  {
    s_ptGicDist->aulIcd_set_pending[uIdx] = 1UL << uSrt;
  }
  else
  {
    s_ptGicDist->aulIcd_clr_pending[uIdx] = 1UL << uSrt;
  }

  /* Controller was successfully configured */
  return GIC_OKAY;
}


/*****************************************************************************/
/*! HALDEF(Gic_DistSetPriority): Configure interrupt priority
* \description
*   The distributor provides an 8-bit priority field for each interrupt
*   supported by the GIC. This field stores the priority of the corresponding
*   interrupt.
* \class 
*   GIC 
* \params
*   uIrqNo       [in] Interrupt number to configure
*   ulPriority   [in] Priority for Interrupt N
* \return
*   GIC_RESULT_E                                                      */
/*****************************************************************************/
GIC_RESULT_E HALDEF(Gic_DistSetPriority)(unsigned int uIrqNo,
    uint32_t ulPriority)
{
  unsigned int uSrt = (uIrqNo & 0x3) << 3;
  unsigned int uIdx = uIrqNo >> 2;
  uint32_t ulVal;

  /* Check priority number (ulPriority) parameter */
  if( ulPriority > GIC_DISTRIBUTOR_INT_PRIORITY_HIGHEST_CONFIG )
  {
    return GIC_ERR_INVAL_PARAM;
  }

  /* Check interrupt number (uIrqNo) parameter */
  if( uIrqNo > GIC_CPU_INTERRUPT_ID_UPPER_BORDER )
  {
    return GIC_ERR_INVAL_PARAM;
  }

  /* Configuration 4 bit registers - read -> modify -> write is needed here */
  ulVal = s_ptGicDist->aulIcd_prio[uIdx] & ~(0xfUL << uSrt);   /* Read and clear selected bits */
  s_ptGicDist->aulIcd_prio[uIdx] = ulVal | (ulPriority << uSrt);
  
  /* Controller was successfully configured */
  return GIC_OKAY;
}


/*****************************************************************************/
/*! HALDEF(Gic_DistSetProcessorTarget): Configure interrupt target CPU
* \description
*   The distributor provides an 8-bit CPU targets field for each interrupt
*   supported by the GIC. This field stores the list of target processors for
*   the interrupt. Each interrupt can be connected to multiple CPUs.
* \class 
*   GIC 
* \params
*   uIrqNo               [in] Interrupt number to configure
*   ulTargetCoreMsk      [in] CPU target for Interrupt N
* \return
*   GIC_RESULT_E                                                      */
/*****************************************************************************/
GIC_RESULT_E HALDEF(Gic_DistSetProcessorTarget)(unsigned int uIrqNo,
    uint32_t ulTargetCoreMsk)
{
  unsigned int uSrt = (uIrqNo & 0x3) << 3;
  unsigned int uIdx = uIrqNo >> 2;
  uint32_t ulVal;

  /* Check interrupt number (uIrqNo) parameter */
  if( uIrqNo > GIC_CPU_INTERRUPT_ID_UPPER_BORDER )
  {
    return GIC_ERR_INVAL_PARAM;
  }

  /* Configuration 4 bit registers - read -> modify -> write is needed here */
  ulVal = s_ptGicDist->aulIcd_processor_trg[uIdx] & ~(0xffUL << uSrt);   /* Read and clear selected bits */
  s_ptGicDist->aulIcd_processor_trg[uIdx] = ulVal | (ulTargetCoreMsk << uSrt);

  /* Controller was successfully configured */
  return GIC_OKAY;
}


/*****************************************************************************/
/*! Gic_DistSetConfiguration: Configure interrupt level
* \description
*   The distributor provides a 2-bit Int_config field for each interrupt 
*   supported by the GIC. This field identifies whether the corresponding
*   interrupt is edge-triggered or level-sensitive
* \class 
*   GIC 
* \params
*   uIrqNo                  [in] Interrupt number to configure
*   fEdge                   [in] High Level (false) or Rising Edge (true)
* \return
*   GIC_RESULT_E                                                      */
/*****************************************************************************/
GIC_RESULT_E HALDEF(Gic_DistCfg)(unsigned int uIrqNo, bool fEdge)
{
  unsigned int uSrt = (uIrqNo & 0xf) << 1;
  unsigned int uIdx = uIrqNo >> 4;
  uint32_t ulVal;

  /* Check interrupt number (uIrqNo) parameter */
  if( uIrqNo > GIC_CPU_INTERRUPT_ID_UPPER_BORDER )
  {
    return GIC_ERR_INVAL_PARAM;
  }

  /* Configuration 2 bit registers - read -> modify -> write is needed here */
  ulVal = s_ptGicDist->aulIcd_cfg[uIdx] & ~(0x3UL << uSrt);   /* Read and clear selected bits */

  if( fEdge )
  {
    ulVal |= (0x2UL << uSrt);
  }
  else
  {
    ulVal &= ~(0x2UL << uSrt);
  }

  s_ptGicDist->aulIcd_cfg[uIdx] = ulVal;

  /* Controller was successfully configured */
  return GIC_OKAY;
}


/*****************************************************************************/
/*! HALDEF(Gic_DistSetIsr): Install ISRs
* \description
*   Installs a interrupt service routine for a specific interrupt id
* \class 
*   GIC 
* \params
*   uIrqNo                [in] uIrqNo - Interrupt number
*   pp_InterruptServiceRoutine    [in] pp_InterruptServiceRoutine - Pointer to ISR
* \return
*   GIC_RESULT_E                                                      */
/*****************************************************************************/
GIC_RESULT_E HALDEF(Gic_DistSetIsr)(unsigned int uIrqNo,
    uint32_t ulIsrAddr)
{
  /* Check interrupt number (uIrqNo) parameter */
  if( uIrqNo >= GIC_CPU_INTERRUPT_ID_UPPER_BORDER )
  {
    return GIC_ERR_INVAL_PARAM;
  }

  /* Do access */
  ppa_gic_func_array[uIrqNo] = ulIsrAddr;

  /* Controller was successfully configured */
  return GIC_OKAY;
}


/*****************************************************************************/
/*! Gic_GenericGetInterruptStatus: Get status of interrupt N
* \description
*   Return the status of the specific interrupt
* \class 
*   GIC 
* \params
*   uIrqNo                [in]  Interrupt number
* \return
*   true if interrupt is set
*   false if interrupt is not set                                            */
/*****************************************************************************/
bool HALDEF(Gic_DistIsPending)(unsigned int uIrqNo)
{
  unsigned int uSrt = uIrqNo & 0x1f;
  unsigned int uIdx = uIrqNo >> 5;
  uint32_t ulVal = s_ptGicDist->aulIcd_set_pending[uIdx];
  
  /* Shift the value to position 0 (to return 0 or 1)*/
  return ( 0 != ((ulVal >> uSrt) & 1) );
}

/*****************************************************************************/
/*! Gic_GenericGetInterruptStatus: Get status of interrupt N
* \description
*   Return the status of the specific interrupt
* \class
*   GIC
* \params
*   uIrqNo                [in]  Interrupt number
* \return
*   true if interrupt is set
*   false if interrupt is not set                                            */
/*****************************************************************************/
bool HALDEF(Gic_DistIsActive)(unsigned int uIrqNo)
{
  unsigned int uSrt = uIrqNo & 0x1f;
  unsigned int uIdx = uIrqNo >> 5;
  uint32_t ulVal = s_ptGicDist->aulIcd_active[uIdx];

  /* Shift the value to position 0 (to return 0 or 1)*/
  return ( 0 != ((ulVal >> uSrt) & 1) );
}


/*****************************************************************************/
/*! Gic_GenericDefaultInterruptHandler: Simple interrupt handler
* \description
*   Simple interrupt handler (jump to ISR and confirm IRQ)
* \class 
*   GIC 
* \params
*   -
* \return
*   GIC_RESULT_E                                                      */
/*****************************************************************************/
GIC_RESULT_E HALDEF(Gic_DefaultIrqHandler)(void)
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
  
  /* Interrupt service routine was executed */  
  return GIC_OKAY;
}

uint32_t HALDEF(Gic_CpuiHighestPendingIrq)(void)
{
  return s_ptGicIf->ulIcci_highest_pending;
}
