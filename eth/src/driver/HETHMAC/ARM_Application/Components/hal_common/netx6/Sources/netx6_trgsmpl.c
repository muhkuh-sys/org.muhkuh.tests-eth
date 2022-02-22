/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_trgsmpl.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_TRIGGER_SAMPLE

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! TRIGGER_SAMPLE Set Parameter
* \description
*   This function writes parameter.
* \class 
*   TRIGGER_SAMPLE
* \params
*   ePrmID          [in]  Parameter ID
*   ulPrmVal        [in]  Parameter value
* \return
*   NX6_TRGSMPL_OKAY                                                             
*   NX6_TRGSMPL_ERR_INVAL_PARAM                                             */
/*****************************************************************************/
NX6_TRGSMPL_RESULT_E NX6_TriggerSample_SetPrm( NX6_TRGSMPL_PRM_E ePrmID,
                                               uint32_t ulPrmVal )
{
  NX6_TRGSMPL_RESULT_E eRslt = NX6_TRGSMPL_OKAY;
  switch( ePrmID )
  { 
    case NX6_TRGSMPL_PRM_TRIGGER_SAMPLE_CONFIG:   NX_WRITE32(s_ptTriggerSample->ulTrigger_sample_config, (ulPrmVal & ~MSK_NX51_trigger_sample_config_latch_unit_activate)); break;
    case NX6_TRGSMPL_PRM_TRIGGER_ACTIVATE:        NX_WRITE32(s_ptTriggerSample->ulTrigger_activate, (ulPrmVal & ~MSK_NX51_trigger_activate_trigger_unit_activate)); break;
    case NX6_TRGSMPL_PRM_TRIGGER_IMPULSE_LENGTH:  NX_WRITE32(s_ptTriggerSample->ulTrigger_impulse_length, ulPrmVal); break;
    case NX6_TRGSMPL_PRM_TRIGGER_0_STARTTIME_NS:  NX_WRITE32(s_ptTriggerSample->aulTrigger__starttime_ns[0], ulPrmVal); break;
    case NX6_TRGSMPL_PRM_TRIGGER_1_STARTTIME_NS:  NX_WRITE32(s_ptTriggerSample->aulTrigger__starttime_ns[1], ulPrmVal); break;
    case NX6_TRGSMPL_PRM_TRIGGER_0_CYC_TIME:      NX_WRITE32(s_ptTriggerSample->aulTrigger__cyc_time[0], ulPrmVal); break;
    case NX6_TRGSMPL_PRM_TRIGGER_1_CYC_TIME:      NX_WRITE32(s_ptTriggerSample->aulTrigger__cyc_time[1], ulPrmVal); break;
    case NX6_TRGSMPL_PRM_SAMPLE_MODE:             NX_WRITE32(s_ptTriggerSample->ulSample_mode, ulPrmVal); break;
    case NX6_TRGSMPL_PRM_SAMPLE_0_POS_SYSTIME_NS: NX_WRITE32(s_ptTriggerSample->asSample_[0].ulPos_systime_ns, ulPrmVal); break;
    case NX6_TRGSMPL_PRM_SAMPLE_0_NEG_SYSTIME_NS: NX_WRITE32(s_ptTriggerSample->asSample_[1].ulNeg_systime_ns, ulPrmVal); break;
    case NX6_TRGSMPL_PRM_SAMPLE_1_POS_SYSTIME_NS: NX_WRITE32(s_ptTriggerSample->asSample_[0].ulPos_systime_ns, ulPrmVal); break;
    case NX6_TRGSMPL_PRM_SAMPLE_1_NEG_SYSTIME_NS: NX_WRITE32(s_ptTriggerSample->asSample_[1].ulNeg_systime_ns, ulPrmVal); break;
    case NX6_TRGSMPL_PRM_TRIGGER_OFFSET:          NX_WRITE32(s_ptTriggerSample->ulTrigger_offset, ulPrmVal); break;
    case NX6_TRGSMPL_PRM_SYNC1_INTERVAL:          NX_WRITE32(s_ptTriggerSample->ulSync1_interval, ulPrmVal); break;

    /* Default */
    default:
      eRslt = NX6_TRGSMPL_ERR_INVAL_PARAM;
    break;
  } /* end switch */

  return eRslt;
}

/*****************************************************************************/
/*! TRIGGER_SAMPLE Get Parameter
* \description
*   This function reads parameter and returns it.
* \class 
*   TRIGGER_SAMPLE
* \params
*   ePrmID          [in]  Parameter ID
* \return
*   Read Parameter Value                                                     */
/*****************************************************************************/
uint32_t NX6_TriggerSample_GetPrm( NX6_TRGSMPL_PRM_E ePrmID )
{
  uint32_t ulRetVal;
  
  switch( ePrmID )
  { 
    case NX6_TRGSMPL_PRM_TRIGGER_SAMPLE_CONFIG:   ulRetVal = NX_READ32(s_ptTriggerSample->ulTrigger_sample_config); break;
    case NX6_TRGSMPL_PRM_TRIGGER_SAMPLE_STATUS:   ulRetVal = NX_READ32(s_ptTriggerSample->ulTrigger_sample_status); break;
    case NX6_TRGSMPL_PRM_TRIGGER_SAMPLE_IRQ:      ulRetVal = NX_READ32(s_ptTriggerSample->ulTrigger_sample_irq); break;
    case NX6_TRGSMPL_PRM_TRIGGER_ACTIVATE:        ulRetVal = NX_READ32(s_ptTriggerSample->ulTrigger_activate); break;
    case NX6_TRGSMPL_PRM_TRIGGER_IMPULSE_LENGTH:  ulRetVal = NX_READ32(s_ptTriggerSample->ulTrigger_impulse_length); break;
    case NX6_TRGSMPL_PRM_TRIGGER_0_STARTTIME_NS:  ulRetVal = NX_READ32(s_ptTriggerSample->aulTrigger__starttime_ns[0]); break;
    case NX6_TRGSMPL_PRM_TRIGGER_1_STARTTIME_NS:  ulRetVal = NX_READ32(s_ptTriggerSample->aulTrigger__starttime_ns[1]); break;
    case NX6_TRGSMPL_PRM_TRIGGER_0_CYC_TIME:      ulRetVal = NX_READ32(s_ptTriggerSample->aulTrigger__cyc_time[0]); break;
    case NX6_TRGSMPL_PRM_TRIGGER_1_CYC_TIME:      ulRetVal = NX_READ32(s_ptTriggerSample->aulTrigger__cyc_time[1]); break;
    case NX6_TRGSMPL_PRM_SAMPLE_MODE:             ulRetVal = NX_READ32(s_ptTriggerSample->ulSample_mode); break;
    case NX6_TRGSMPL_PRM_SAMPLE_0_POS_SYSTIME_NS: ulRetVal = NX_READ32(s_ptTriggerSample->asSample_[0].ulPos_systime_ns); break;
    case NX6_TRGSMPL_PRM_SAMPLE_0_NEG_SYSTIME_NS: ulRetVal = NX_READ32(s_ptTriggerSample->asSample_[0].ulNeg_systime_ns); break;
    case NX6_TRGSMPL_PRM_SAMPLE_1_POS_SYSTIME_NS: ulRetVal = NX_READ32(s_ptTriggerSample->asSample_[1].ulPos_systime_ns); break;
    case NX6_TRGSMPL_PRM_SAMPLE_1_NEG_SYSTIME_NS: ulRetVal = NX_READ32(s_ptTriggerSample->asSample_[1].ulNeg_systime_ns); break;
    case NX6_TRGSMPL_PRM_TRIGGER_OFFSET:          ulRetVal = NX_READ32(s_ptTriggerSample->ulTrigger_offset); break;
    case NX6_TRGSMPL_PRM_SYNC1_INTERVAL:          ulRetVal = NX_READ32(s_ptTriggerSample->ulSync1_interval); break;
    default: ulRetVal = 0; break;
  } /* end switch */

  return ulRetVal;
}

/*****************************************************************************/
/*! TRIGGER_SAMPLE Start Trigger unit
* \description
*   This function starts the trigger unit.
* \class 
*   TRIGGER_SAMPLE
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX6_TriggerSample_StartTriggerUnit( void )
{
  NX_WRITE32(s_ptTriggerSample->ulTrigger_activate,  NX_READ32(s_ptTriggerSample->ulTrigger_activate)
                                                    |MSK_NX51_trigger_activate_trigger_unit_activate
            );
}

/*****************************************************************************/
/*! TRIGGER_SAMPLE Stop Trigger unit
* \description
*   This function stops the trigger unit.
* \class 
*   TRIGGER_SAMPLE
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX6_TriggerSample_StopTriggerUnit( void )
{
  NX_WRITE32(s_ptTriggerSample->ulTrigger_activate, NX_READ32(s_ptTriggerSample->ulTrigger_activate)
                                                   & ~MSK_NX51_trigger_activate_trigger_unit_activate
            );
}

/*****************************************************************************/
/*! TRIGGER_SAMPLE Start Sample unit
* \description
*   This function starts the sample unit.
* \class 
*   TRIGGER_SAMPLE
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX6_TriggerSample_StartSampleUnit( void )
{
  NX_WRITE32(s_ptTriggerSample->ulTrigger_sample_config, NX_READ32(s_ptTriggerSample->ulTrigger_sample_config)
                                                        |MSK_NX51_trigger_sample_config_latch_unit_activate
            );
}

/*****************************************************************************/
/*! TRIGGER_SAMPLE Stop Sample unit
* \description
*   This function stops the sample unit.
* \class 
*   TRIGGER_SAMPLE
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX6_TriggerSample_StopSampleUnit( void )
{
  NX_WRITE32(s_ptTriggerSample->ulTrigger_sample_config,  NX_READ32(s_ptTriggerSample->ulTrigger_sample_config)
                                                         & ~MSK_NX51_trigger_sample_config_latch_unit_activate
            );
}

/*****************************************************************************/
/*! TRIGGER_SAMPLE Reset Status
* \description
*   This function resets status flags.
* \class 
*   TRIGGER_SAMPLE
* \params
*   ulRstMsk          [in]  Reset flags mask
* \return
*                                                                            */
/*****************************************************************************/
void NX6_TriggerSample_ResetStatus( uint32_t ulRstMsk )
{
  NX_WRITE32(s_ptTriggerSample->ulTrigger_sample_status, ulRstMsk);
}

/*****************************************************************************/
/*! TRIGGER_SAMPLE Get Interrupt
* \description
*   Read interrupt status.
* \class 
*   TRIGGER_SAMPLE 
* \params
* \return
*   Current interrupt statuses                                                */
/*****************************************************************************/
uint32_t NX6_TriggerSample_GetIrq( void )
{
  return NX_READ32(s_ptTriggerSample->ulTrigger_sample_irq);
}

/*****************************************************************************/
/*! TRIGGER_SAMPLE Confirm Interrupt
* \description
*   This function confirms interrupt(s)
* \class 
*   TRIGGER_SAMPLE
* \params
*   ulConfirmMsk          [in]  Confirm mask
* \return
*                                                                            */
/*****************************************************************************/
void NX6_TriggerSample_ConfirmIrq( uint32_t ulConfirmMsk )
{
  NX_WRITE32(s_ptTriggerSample->ulTrigger_sample_irq, ulConfirmMsk);
}
