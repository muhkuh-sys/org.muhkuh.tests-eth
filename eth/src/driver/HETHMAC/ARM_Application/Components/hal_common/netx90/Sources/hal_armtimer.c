/* Includes */
#include "hal_armtimer.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_ARM_TIMER(s_ptArmTimer)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! ARMTIMER Configure Systime
* \description
*   This function configures the systime input of an ARM timer.
* \class
*   ARMTIMER
* \params
* \return
*   ARMTIMER_RESULT_E
*   ARMTIMER_ERR_INVAL_PARAM                                            */
/*****************************************************************************/

#ifndef HALDEF
#define HALDEF(name) name
#endif

ARMTIMER_RESULT_E HALDEF(ArmTimer_ConfigureTimerSystime)( unsigned int uInst, unsigned int uSystimeInst )
{
  if( uInst >= ARMTIMER_CHANNELS )
    return ARMTIMER_ERR_INVAL_PARAM;

  s_ptArmTimer->aulTimer_config_timer[uInst]  &=  ~HW_MSK(timer_config_timer0_systime_config);
  s_ptArmTimer->aulTimer_config_timer[uInst]  |=  uSystimeInst<<HW_SRT(timer_config_timer0_systime_config);

  return ARMTIMER_OKAY;
}

/*****************************************************************************/
/*! ARMTIMER Start
* \description
*   This function starts an ARM timer..
* \class 
*   ARMTIMER
* \params
*   uInst [in] timer instance 0..2
*   eMode [in] timer mode (single-shot, continuous or systime_ns compare)
*   ulVal [in] timer duration in 10ns steps
* \return
*   ARMTIMER_RESULT_E
*   ARMTIMER_ERR_INVAL_PARAM                                            */
/*****************************************************************************/

ARMTIMER_RESULT_E HALDEF(ArmTimer_Start)( unsigned int uInst, ARMTIMER_CFG_MODE_E eMode, uint32_t ulVal )
{
  ARMTIMER_RESULT_E eRslt = ARMTIMER_OKAY;
  uint32_t ulSystimeConfig;

  if( uInst>=ARMTIMER_CHANNELS )
    return ARMTIMER_ERR_INVAL_PARAM;

  /* leave systime_config as it is */
  ulSystimeConfig = s_ptArmTimer->aulTimer_config_timer[uInst]
                  & HW_MSK(timer_config_timer0_systime_config);

  switch( eMode )
  {
    case ARMTIMER_CFG_MODE_STOP_AT_0:
      s_ptArmTimer->aulTimer_config_timer[uInst]  = ulSystimeConfig | ((uint32_t)eMode << HW_SRT(timer_config_timer0_mode));
      s_ptArmTimer->aulTimer_timer[uInst]         = ulVal;
    break;
    
    case ARMTIMER_CFG_MODE_PRELOAD_AT_0:
      s_ptArmTimer->aulTimer_config_timer[uInst]  = ulSystimeConfig | ((uint32_t)eMode << HW_SRT(timer_config_timer0_mode));
      s_ptArmTimer->aulTimer_preload_timer[uInst] = ulVal;
      s_ptArmTimer->aulTimer_timer[uInst]         = ulVal;
    break;
    
    case ARMTIMER_CFG_MODE_SYSTIME:
      s_ptArmTimer->aulTimer_config_timer[uInst]  = ulSystimeConfig | ((uint32_t)eMode << HW_SRT(timer_config_timer0_mode));
      s_ptArmTimer->aulTimer_timer[uInst]         = ulVal;
    break;
 
    default:
      eRslt = ARMTIMER_ERR_INVAL_PARAM;
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
* \return
*   ARMTIMER_RESULT_E
*   ARMTIMER_ERR_INVAL_PARAM                                            */
/*****************************************************************************/
ARMTIMER_RESULT_E HALDEF(ArmTimer_Stop)( unsigned int uInst  )
{
  if( uInst>=ARMTIMER_CHANNELS )
    return ARMTIMER_ERR_INVAL_PARAM;

  /* disable interrupt */
  s_ptArmTimer->ulTimer_irq_msk_reset = HW_MSK(timer_irq_msk_reset_timer0_irq) << uInst;

  /* reset timer registers */
  s_ptArmTimer->aulTimer_config_timer[uInst] &= ~HW_MSK(timer_config_timer0_mode); /* don't touch systime config */
  s_ptArmTimer->aulTimer_preload_timer[uInst] = 0;
  s_ptArmTimer->aulTimer_timer[uInst]         = 0;
  s_ptArmTimer->aulTimer_timer[uInst]         = 0;

  /* confirm pending interrupt */
  s_ptArmTimer->ulTimer_irq_raw = HW_MSK(timer_irq_raw_timer0_irq) << uInst;

  return ARMTIMER_OKAY;  
}

/*****************************************************************************/
/*! ARMTIMER Start Systime S compare
* \description
*   This function starts the systime_s compare machine.
* \class 
*   ARMTIMER
* \params
* \return
*   ARMTIMER_RESULT_E                                                     */
/*****************************************************************************/
ARMTIMER_RESULT_E HALDEF(ArmTimer_StartSystimeSCmp)( uint32_t ulVal )
{
  /* write value */
  s_ptArmTimer->ulTimer_compare_systime_s_value = ulVal;
  
  /* reset irq_raw.systime_s_irq to activate compare machine */
  s_ptArmTimer->ulTimer_irq_raw = HW_MSK(timer_irq_raw_systime_s_irq);

  return ARMTIMER_OKAY;  
}

/*****************************************************************************/
/*! ARMTIMER Enable Interrupt(s)
* \description
*   This function enables interrupts of the ARMTIMER.
* \class 
*   ARMTIMER
* \params
   ulIrqMask     [in] Interrupt enable mask
* \return
*   ARMTIMER_RESULT_E                                                     */
/*****************************************************************************/
ARMTIMER_RESULT_E HALDEF(ArmTimer_EnIrq)( uint32_t ulIrqMsk )
{
  /* enable interrupts */
  s_ptArmTimer->ulTimer_irq_msk_set = ulIrqMsk;
  return ARMTIMER_OKAY;
}

/*****************************************************************************/
/*! ARMTIMER Disable Interrupt(s)
* \description
*   This function disables interrupts.
* \class 
*   ARMTIMER
* \params
   ulIrqMask     [in] Interrupt disable mask
* \return
*   ARMTIMER_OKAY                                                       */
/*****************************************************************************/
ARMTIMER_RESULT_E HALDEF(ArmTimer_DisIrq)( uint32_t ulIrqMsk )
{
  /* disable interrupts */
  s_ptArmTimer->ulTimer_irq_msk_reset = ulIrqMsk;
  return ARMTIMER_OKAY;
}

/*****************************************************************************/
/*! ARMTIMER Get masked Interrupt(s)
* \description
*   This function retrieves the current interrupt masked requests.
* \class 
*   ARMTIMER
* \params
* \return
*   masked IRQ vector                                                        */
/*****************************************************************************/
uint32_t HALDEF(ArmTimer_GetIrq)( void )
{
  /* get IRQ requests */
  return s_ptArmTimer->ulTimer_irq_masked;
}

/*****************************************************************************/
/*! ARMTIMER Get raw Interrupt(s)
* \description
*   This function retrieves the current interrupt raw requests.
* \class 
*   ARMTIMER
* \params
* \return
*   ARMTIMER_OKAY                                                       */
/*****************************************************************************/
uint32_t HALDEF(ArmTimer_GetIrqRaw)( void )
{
  /* get IRQ requests */
  return s_ptArmTimer->ulTimer_irq_raw;
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
*   ARMTIMER_OKAY                                                       */
/*****************************************************************************/
void HALDEF(ArmTimer_ConfirmIrq)( uint32_t ulConfirmIrqMask )
{
  /* confirm the set of irqs */
  s_ptArmTimer->ulTimer_irq_raw = ulConfirmIrqMask;
}

/*****************************************************************************/
/*! ARMTIMER Get Parameter
* \description
*   This function reads parameter.
* \class 
*   PWM
* \params
*   ulPrmID          [in]  Parameter ID
*   pulPrmVal        [in]  Pointer to parameter value
* \return
*   ARMTIMER_OKAY                                                             
*   ARMTIMER_ERR_INVAL_PARAM                                          */
/*****************************************************************************/
ARMTIMER_RESULT_E HALDEF(ArmTimer_GetPrm)( ARMTIMER_PRM_E ePrmID, uint32_t* pulPrmVal )
{
  ARMTIMER_RESULT_E eRslt = ARMTIMER_OKAY;

  switch( ePrmID )
  { 
    case ARMTIMER_PRM_CONFIG_TIMER0:     *pulPrmVal = s_ptArmTimer->aulTimer_config_timer[0];        break;
    case ARMTIMER_PRM_CONFIG_TIMER1:     *pulPrmVal = s_ptArmTimer->aulTimer_config_timer[1];        break;
    case ARMTIMER_PRM_CONFIG_TIMER2:     *pulPrmVal = s_ptArmTimer->aulTimer_config_timer[2];        break;
    case ARMTIMER_PRM_PRELOAD_TIMER0:    *pulPrmVal = s_ptArmTimer->aulTimer_preload_timer[0];       break;
    case ARMTIMER_PRM_PRELOAD_TIMER1:    *pulPrmVal = s_ptArmTimer->aulTimer_preload_timer[1];       break;
    case ARMTIMER_PRM_PRELOAD_TIMER2:    *pulPrmVal = s_ptArmTimer->aulTimer_preload_timer[2];       break;
    case ARMTIMER_PRM_TIMER0:            *pulPrmVal = s_ptArmTimer->aulTimer_timer[0];               break;
    case ARMTIMER_PRM_TIMER1:            *pulPrmVal = s_ptArmTimer->aulTimer_timer[1];               break;
    case ARMTIMER_PRM_TIMER2:            *pulPrmVal = s_ptArmTimer->aulTimer_timer[2];               break;
    case ARMTIMER_PRM_SYSTIME_S_COMPARE: *pulPrmVal = s_ptArmTimer->ulTimer_compare_systime_s_value; break;
    case ARMTIMER_PRM_IRQMSKSET:         *pulPrmVal = s_ptArmTimer->ulTimer_irq_msk_set;             break;
    case ARMTIMER_PRM_IRQMSKRST:         *pulPrmVal = s_ptArmTimer->ulTimer_irq_msk_reset;           break;
    case ARMTIMER_PRM_SYSTIME_S:         *pulPrmVal = s_ptArmTimer->ulTimer_systime_s;               break;
    case ARMTIMER_PRM_SYSTIME_NS:        *pulPrmVal = s_ptArmTimer->ulTimer_systime_ns;              break;
    default:
      eRslt = ARMTIMER_ERR_INVAL_PARAM;
    break;
  } /* end switch */

  return eRslt;
}

/*****************************************************************************/
/*! ARMTIMER Set Parameter
* \description
*   This function reads parameter.
* \class 
*   ARMTIMER
* \params
*   ePrmID           [in]  Parameter ID
*   pulPrmVal        [in]  New parameter value
* \return
*   ARMTIMER_OKAY                                                             
*   ARMTIMER_ERR_INVAL_PARAM                                            */
/*****************************************************************************/
ARMTIMER_RESULT_E HALDEF(ArmTimer_SetPrm) ( ARMTIMER_PRM_E ePrmID, uint32_t ulPrmVal )
{
  ARMTIMER_RESULT_E eRslt = ARMTIMER_OKAY;

  switch( ePrmID )
  { 
    case ARMTIMER_PRM_CONFIG_TIMER0:         s_ptArmTimer->aulTimer_config_timer[0]        = ulPrmVal; break;
    case ARMTIMER_PRM_CONFIG_TIMER1:         s_ptArmTimer->aulTimer_config_timer[1]        = ulPrmVal; break;
    case ARMTIMER_PRM_CONFIG_TIMER2:         s_ptArmTimer->aulTimer_config_timer[2]        = ulPrmVal; break;
    case ARMTIMER_PRM_PRELOAD_TIMER0:        s_ptArmTimer->aulTimer_preload_timer[0]       = ulPrmVal; break;
    case ARMTIMER_PRM_PRELOAD_TIMER1:        s_ptArmTimer->aulTimer_preload_timer[1]       = ulPrmVal; break;
    case ARMTIMER_PRM_PRELOAD_TIMER2:        s_ptArmTimer->aulTimer_preload_timer[2]       = ulPrmVal; break;
    case ARMTIMER_PRM_TIMER0:                s_ptArmTimer->aulTimer_timer[0]               = ulPrmVal; break;
    case ARMTIMER_PRM_TIMER1:                s_ptArmTimer->aulTimer_timer[1]               = ulPrmVal; break;
    case ARMTIMER_PRM_TIMER2:                s_ptArmTimer->aulTimer_timer[2]               = ulPrmVal; break;
    case ARMTIMER_PRM_SYSTIME_S_COMPARE:     s_ptArmTimer->ulTimer_compare_systime_s_value = ulPrmVal; break;
    case ARMTIMER_PRM_IRQMSKSET:             s_ptArmTimer->ulTimer_irq_msk_set             = ulPrmVal; break;
    case ARMTIMER_PRM_IRQMSKRST:             s_ptArmTimer->ulTimer_irq_msk_reset           = ulPrmVal; break;
    default:
      eRslt = ARMTIMER_ERR_INVAL_PARAM;
    break;
  } /* end switch */

  return eRslt;
}

/*****************************************************************************/
/*! ARMTIMER Sleep 10 ns
* \description
*   Sleep a while using an ARM timer.
* \class 
*   ARMTIMER
* \params
*   uInst              [in]   Selected Timer
*   ulTimeout10Ns      [out]  The Time to wait in 10 ns
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmTimer_Sleep)( unsigned int uInst, uint32_t ulTimeout10Ns )
{
  s_ptArmTimer->aulTimer_config_timer[uInst]  =  ARMTIMER_CFG_MODE_STOP_AT_0 << HW_SRT(timer_config_timer0_mode);
  s_ptArmTimer->aulTimer_timer[uInst]         =  ulTimeout10Ns;
                                                
  /* poll timer finished */
  while ( s_ptArmTimer->aulTimer_timer[uInst] != 0 );
}

/*****************************************************************************/
/*! ARMTIMER Set Systime Configuration
* \description
*   Set systime instance.
* \class 
*   ARMTIMER
* \params
*   uSysimeInst              [in]   Selected instance
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmTimer_SetSystimeConfig) ( unsigned int uSysimeInst )
{
  s_ptArmTimer->ulTimer_systime_config = uSysimeInst;
}

/*****************************************************************************/
/*! ARMTIMER Get Timer
* \description
*   This function reads the current timer value.
* \class
*   PWM
* \params
*   uInst              [in]   Selected Timer
* \return
*   timer value                                                              */
/*****************************************************************************/
uint32_t HALDEF(ArmTimer_GetTimer)( unsigned int uInst )
{
  return s_ptArmTimer->aulTimer_timer[uInst];
}

/*****************************************************************************/
/*! Get SYSTIME
* \description
*   Read the system time.
* \class
*   ARMTIMER
* \params
*   pulSystime_s          [in]  Pointer of Value in Seconds
*   pulSystime_ns         [in]  Pointer of Value in Nanoseconds
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(ArmTimer_GetSystime)( uint32_t* pulSystime_s, uint32_t* pulSystime_ns )
{
  *pulSystime_s  = s_ptArmTimer->ulTimer_systime_s;
  *pulSystime_ns = s_ptArmTimer->ulTimer_systime_ns;
}
