#ifndef __NETX51_TRGSMPL_H
#define __NETX51_TRGSMPL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! Parameters ID that can be set/get by TrgSmpl_SetPrm()/TrgSmpl_GetPrm()   */
/*****************************************************************************/
typedef enum NX51_TRGSMPL_PRM_Etag {
  NX51_TRGSMPL_PRM_TRIGGER_SAMPLE_CONFIG   = 1,
  NX51_TRGSMPL_PRM_TRIGGER_SAMPLE_STATUS   ,
  NX51_TRGSMPL_PRM_TRIGGER_SAMPLE_IRQ      ,
  NX51_TRGSMPL_PRM_TRIGGER_ACTIVATE        ,
  NX51_TRGSMPL_PRM_TRIGGER_IMPULSE_LENGTH  ,
  NX51_TRGSMPL_PRM_TRIGGER_0_STARTTIME_NS  ,
  NX51_TRGSMPL_PRM_TRIGGER_1_STARTTIME_NS  ,
  NX51_TRGSMPL_PRM_TRIGGER_0_CYC_TIME      ,
  NX51_TRGSMPL_PRM_TRIGGER_1_CYC_TIME      ,
  NX51_TRGSMPL_PRM_SAMPLE_MODE             ,
  NX51_TRGSMPL_PRM_SAMPLE_0_POS_SYSTIME_NS ,
  NX51_TRGSMPL_PRM_SAMPLE_0_NEG_SYSTIME_NS ,
  NX51_TRGSMPL_PRM_SAMPLE_1_POS_SYSTIME_NS ,
  NX51_TRGSMPL_PRM_SAMPLE_1_NEG_SYSTIME_NS ,
  NX51_TRGSMPL_PRM_TRIGGER_OFFSET          ,
  NX51_TRGSMPL_PRM_SYNC1_INTERVAL
} NX51_TRGSMPL_PRM_E;

/*****************************************************************************/
/*! Result codes for functions                                               */
/*****************************************************************************/
typedef enum NX51_TRGSMPL_RESULT_Etag {
  NX51_TRGSMPL_OKAY                      =  0, /* Successful          */
  NX51_TRGSMPL_ERR_INVAL_PARAM           =  1  /* Invalid parameter   */
} NX51_TRGSMPL_RESULT_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

NX51_TRGSMPL_RESULT_E NX51_TriggerSample_SetPrm( NX51_TRGSMPL_PRM_E ePrmID, uint32_t ulPrmVal );
uint32_t  NX51_TriggerSample_GetPrm( NX51_TRGSMPL_PRM_E ePrmID );
void      NX51_TriggerSample_StartTriggerUnit( void );
void      NX51_TriggerSample_StopTriggerUnit( void );
void      NX51_TriggerSample_StartSampleUnit( void );
void      NX51_TriggerSample_StopSampleUnit( void );
void      NX51_TriggerSample_ResetStatus( uint32_t ulRstMsk );
uint32_t  NX51_TriggerSample_GetIrq( void );
void      NX51_TriggerSample_ConfirmIrq( uint32_t ulConfirmMsk );

#endif /* #ifndef __NETX51_TRGSMPL_H */
