/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_armtimer.h"
#include "hal_resources_defines_netx51.h"


/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_ARM_TIMER


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! ARMTIMER Start
* \description
*   This function starts an ARM timer..
* \class
*   ARMTIMER
* \params
*   uInst   [in] Timer instance (0..2)
*   eMode   [in] Timer mode
*   ulVal   [in] Initial timer value
* \return
*   NX51_ARMTIMER_OKAY
*   NX51_ARMTIMER_ERR_INVAL_PARAM                                            */
/*****************************************************************************/
NX51_ARMTIMER_RESULT_E NX51_ArmTimer_Start( unsigned int             uInst,
                                            NX51_ARMTIMER_CFG_MODE_E eMode,
                                            uint32_t                 ulVal )
{
  NX51_ARMTIMER_RESULT_E eRslt = NX51_ARMTIMER_OKAY;

  if( uInst>=NX51_ARMTIMER_CHANNELS )
    return NX51_ARMTIMER_ERR_INVAL_PARAM;

  switch( eMode )
  {
    case NX51_ARMTIMER_CFG_MODE_STOP_AT_0:
      s_ptArmTimer->aulArm_timer_config_timer[uInst]  =  (uint32_t)eMode << SRT_NX51_arm_timer_config_timer0_mode;
      s_ptArmTimer->aulArm_timer_timer[uInst]         =  ulVal;
    break;

    case NX51_ARMTIMER_CFG_MODE_PRELOAD_AT_0:
      s_ptArmTimer->aulArm_timer_config_timer[uInst]  =  (uint32_t)eMode << SRT_NX51_arm_timer_config_timer0_mode;
      s_ptArmTimer->aulArm_timer_preload_timer[uInst] =  ulVal;
      s_ptArmTimer->aulArm_timer_timer[uInst]         =  ulVal;
    break;

    case NX51_ARMTIMER_CFG_MODE_SYSTIME:
      s_ptArmTimer->aulArm_timer_config_timer[uInst]  =  (uint32_t)eMode << SRT_NX51_arm_timer_config_timer0_mode;
      s_ptArmTimer->aulArm_timer_timer[uInst]         =  ulVal;
    break;

    default: eRslt = NX51_ARMTIMER_ERR_INVAL_PARAM;
    break;
  }

  return eRslt;
}

/*****************************************************************************/
/*! ARMTIMER Stop
* \description
*   This function stops and resets an ARM timer..
* \class
*   ARMTIMER
* \params
*   uInst   [in] Timer instance (0..2)
* \return
*   NX51_ARMTIMER_OKAY
*   NX51_ARMTIMER_ERR_INVAL_PARAM                                            */
/*****************************************************************************/
NX51_ARMTIMER_RESULT_E NX51_ArmTimer_Stop( unsigned int uInst )
{
  if( uInst>=NX51_ARMTIMER_CHANNELS )
    return NX51_ARMTIMER_ERR_INVAL_PARAM;

  /* disable interrupt */
  switch( uInst ) {
    case 0: s_ptArmTimer->ulArm_timer_irq_msk_reset =  (uint32_t)1 << SRT_NX51_arm_timer_irq_msk_reset_timer0_irq; break;
    case 1: s_ptArmTimer->ulArm_timer_irq_msk_reset =  (uint32_t)1 << SRT_NX51_arm_timer_irq_msk_reset_timer1_irq; break;
    default: return NX51_ARMTIMER_ERR_INVAL_PARAM;
  }

  /* reset timer registers */
  s_ptArmTimer->aulArm_timer_config_timer[uInst]  =  0;
  s_ptArmTimer->aulArm_timer_preload_timer[uInst] =  0;
  s_ptArmTimer->aulArm_timer_timer[uInst]         =  0;
  s_ptArmTimer->aulArm_timer_timer[uInst]         =  0;

  /* confirm pending interrupt */
  switch( uInst ) {
    case 0: s_ptArmTimer->ulArm_timer_irq_raw =  (uint32_t)1 << SRT_NX51_arm_timer_irq_raw_timer0_irq; break;
    case 1: s_ptArmTimer->ulArm_timer_irq_raw =  (uint32_t)1 << SRT_NX51_arm_timer_irq_raw_timer1_irq; break;
    default: return NX51_ARMTIMER_ERR_INVAL_PARAM;
  }

  return NX51_ARMTIMER_OKAY;
}

/*****************************************************************************/
/*! ARMTIMER Start Systime NS compare
* \description
*   This function starts the systime_ns compare machine.
* \class
*   ARMTIMER
* \params
*   ulVal    [in] Value to be compared with the systime_ns register
* \return
*                                                                            */
/*****************************************************************************/
void NX51_ArmTimer_StartSystimeNsCmp( uint32_t ulVal )
{
  s_ptArmTimer->ulArm_timer_systime_ns_compare = ulVal;
}

/*****************************************************************************/
/*! ARMTIMER Start Systime S compare
* \description
*   This function starts the systime_s compare machine.
* \class
*   ARMTIMER
* \params
*   ulVal    [in] Value to be compared with the systime_s register
* \return
*   NX51_ARMTIMER_RESULT                                                     */
/*****************************************************************************/
void NX51_ArmTimer_StartSystimeSecCmp( uint32_t ulVal )
{
  /* write value */
  s_ptArmTimer->ulArm_timer_systime_s_compare = ulVal;

  /* reset irq_raw.systime_s_irq to activate compare machine */
  s_ptArmTimer->ulArm_timer_irq_raw = MSK_NX51_arm_timer_irq_raw_systime_s_irq;
}

/*****************************************************************************/
/*! ARMTIMER Enable Interrupt(s)
* \description
*   This function enables interrupts of the ARMTIMER.
* \class
*   ARMTIMER
* \params
*   uTimer0IrqEn         [in] Timer0 interrupt enable
*   uTimer1IrqEn         [in] Timer1 interrupt enable
*   uSystimeNsIrqEn      [in] Systime NanoSecond interrupt enable
*   uSystimeSecIrqEn     [in] Systime Second interrupt enable
* \return
*                                                                            */
/*****************************************************************************/
void NX51_ArmTimer_EnIrq( unsigned int uTimer0IrqEn,
                          unsigned int uTimer1IrqEn,
                          unsigned int uSystimeNsIrqEn,
                          unsigned int uSystimeSecIrqEn )
{
  /* enable interrupts */
  s_ptArmTimer->ulArm_timer_irq_msk_set =  ((uint32_t)uTimer0IrqEn     << SRT_NX51_arm_timer_irq_msk_set_timer0_irq)
                                          |((uint32_t)uTimer1IrqEn     << SRT_NX51_arm_timer_irq_msk_set_timer1_irq)
                                          |((uint32_t)uSystimeNsIrqEn  << SRT_NX51_arm_timer_irq_msk_set_systime_ns_irq)
                                          |((uint32_t)uSystimeSecIrqEn << SRT_NX51_arm_timer_irq_msk_set_systime_s_irq);
}

/*****************************************************************************/
/*! ARMTIMER Disable Interrupt(s)
* \description
*   This function disables interrupts.
* \class
*   ARMTIMER
* \params
*   uTimer0IrqDis         [in] Timer0 interrupt disable
*   uTimer1IrqDis         [in] Timer1 interrupt disable
*   uSystimeNsIrqDis      [in] Systime NanoSecond interrupt disable
*   uSystimeSecIrqDis     [in] Systime Second interrupt disable
* \return
*                                                                            */
/*****************************************************************************/
void NX51_ArmTimer_DisIrq( unsigned int uTimer0IrqDis,
                           unsigned int uTimer1IrqDis,
                           unsigned int uSystimeNsIrqDis,
                           unsigned int uSystimeSecIrqDis )
{
  /* disable interrupts */
  s_ptArmTimer->ulArm_timer_irq_msk_reset =  ((uint32_t)uTimer0IrqDis     << SRT_NX51_arm_timer_irq_msk_reset_timer0_irq)
                                            |((uint32_t)uTimer1IrqDis     << SRT_NX51_arm_timer_irq_msk_reset_timer1_irq)
                                            |((uint32_t)uSystimeNsIrqDis  << SRT_NX51_arm_timer_irq_msk_reset_systime_ns_irq)
                                            |((uint32_t)uSystimeSecIrqDis << SRT_NX51_arm_timer_irq_msk_reset_systime_s_irq);
}

/*****************************************************************************/
/*! ARMTIMER Get masked Interrupt(s)
* \description
*   This function retrieves the current interrupt masked requests.
* \class
*   ARMTIMER
* \params
*   pulIrq     [out] Pointer to interrupt events
* \return
*                                                                            */
/*****************************************************************************/
void NX51_ArmTimer_GetIrq( uint32_t *pulIrq )
{
  /* get IRQ requests */
  *pulIrq = s_ptArmTimer->ulArm_timer_irq_masked;
}

/*****************************************************************************/
/*! ARMTIMER Get raw Interrupt(s)
* \description
*   This function retrieves the current interrupt raw requests.
* \class
*   ARMTIMER
* \params
*   pulIrq     [out] Pointer to interrupt events
* \return
*   NX51_ARMTIMER_OKAY                                                       */
/*****************************************************************************/
void NX51_ArmTimer_GetIrqRaw( uint32_t *pulIrq )
{
  /* get IRQ requests */
  *pulIrq = s_ptArmTimer->ulArm_timer_irq_raw;
}

/*****************************************************************************/
/*! ARMTIMER Confirm interrupt(s)
* \description
*   This function confirms a set of interrupts that were requested.
* \class
*   ARMTIMER
* \params
*   ulConfirmIrqMask  [in] Mask to confirm interrupt events
* \return
*                                                                            */
/*****************************************************************************/
void NX51_ArmTimer_ConfirmIrq( uint32_t ulConfirmIrqMask )
{
  /* confirm the set of IRQs */
  s_ptArmTimer->ulArm_timer_irq_raw = ulConfirmIrqMask;
}

/*****************************************************************************/
/*! ARMTIMER Get Parameter
* \description
*   This function reads parameter.
* \class
*   ARMTIMER
* \params
*   ePrmID           [in]  Parameter ID
*   pulPrmVal        [in]  Pointer to parameter value
* \return
*   NX51_ARMTIMER_OKAY
*   NX51_ARMTIMER_ERR_INVAL_PARAM                                            */
/*****************************************************************************/
NX51_ARMTIMER_RESULT_E NX51_ArmTimer_GetPrm( NX51_ARMTIMER_PRM_E ePrmID,
                                             uint32_t*           pulPrmVal )
{
  NX51_ARMTIMER_RESULT_E eRslt = NX51_ARMTIMER_OKAY;

  switch( ePrmID )
  {
    case NX51_ARMTIMER_PRM_CONFIG_TIMER0:      *pulPrmVal = s_ptArmTimer->aulArm_timer_config_timer[0];   break;
    case NX51_ARMTIMER_PRM_CONFIG_TIMER1:      *pulPrmVal = s_ptArmTimer->aulArm_timer_config_timer[1];   break;
    case NX51_ARMTIMER_PRM_PRELOAD_TIMER0:     *pulPrmVal = s_ptArmTimer->aulArm_timer_preload_timer[0];  break;
    case NX51_ARMTIMER_PRM_PRELOAD_TIMER1:     *pulPrmVal = s_ptArmTimer->aulArm_timer_preload_timer[1];  break;
    case NX51_ARMTIMER_PRM_TIMER0:             *pulPrmVal = s_ptArmTimer->aulArm_timer_timer[0];          break;
    case NX51_ARMTIMER_PRM_TIMER1:             *pulPrmVal = s_ptArmTimer->aulArm_timer_timer[1];          break;
    case NX51_ARMTIMER_PRM_SYSTIME_NS_COMPARE: *pulPrmVal = s_ptArmTimer->ulArm_timer_systime_ns_compare; break;
    case NX51_ARMTIMER_PRM_SYSTIME_S_COMPARE:  *pulPrmVal = s_ptArmTimer->ulArm_timer_systime_s_compare;  break;
    case NX51_ARMTIMER_PRM_IRQMSKSET:          *pulPrmVal = s_ptArmTimer->ulArm_timer_irq_msk_set;        break;
    case NX51_ARMTIMER_PRM_IRQMSKRST:          *pulPrmVal = s_ptArmTimer->ulArm_timer_irq_msk_reset;      break;
    default:
      eRslt = NX51_ARMTIMER_ERR_INVAL_PARAM;
      break;
  } /* end switch */

  return eRslt;
}
