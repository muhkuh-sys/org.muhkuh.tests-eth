#ifndef __NETX6_TRGSMPL_H
#define __NETX6_TRGSMPL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! Parameters ID that can be set/get by TrgSmpl_SetPrm()/TrgSmpl_GetPrm()   */
/*****************************************************************************/
typedef enum NX6_TRGSMPL_PRM_Etag {
  NX6_TRGSMPL_PRM_TRIGGER_SAMPLE_CONFIG   =  1,
  NX6_TRGSMPL_PRM_TRIGGER_SAMPLE_STATUS   =  2,
  NX6_TRGSMPL_PRM_TRIGGER_SAMPLE_IRQ      =  3,
  NX6_TRGSMPL_PRM_TRIGGER_ACTIVATE        =  4,
  NX6_TRGSMPL_PRM_TRIGGER_IMPULSE_LENGTH  =  5,
  NX6_TRGSMPL_PRM_TRIGGER_0_STARTTIME_NS  =  6,
  NX6_TRGSMPL_PRM_TRIGGER_1_STARTTIME_NS  =  7,
  NX6_TRGSMPL_PRM_TRIGGER_0_CYC_TIME      =  8,
  NX6_TRGSMPL_PRM_TRIGGER_1_CYC_TIME      =  9,
  NX6_TRGSMPL_PRM_SAMPLE_MODE             = 10,
  NX6_TRGSMPL_PRM_SAMPLE_0_POS_SYSTIME_NS = 11,
  NX6_TRGSMPL_PRM_SAMPLE_0_NEG_SYSTIME_NS = 12,
  NX6_TRGSMPL_PRM_SAMPLE_1_POS_SYSTIME_NS = 13,
  NX6_TRGSMPL_PRM_SAMPLE_1_NEG_SYSTIME_NS = 14,
  NX6_TRGSMPL_PRM_TRIGGER_OFFSET          = 15,
  NX6_TRGSMPL_PRM_SYNC1_INTERVAL          = 16
} NX6_TRGSMPL_PRM_E;

/*****************************************************************************/
/*! Result codes for functions                                               */
/*****************************************************************************/
typedef enum NX6_TRGSMPL_RESULT_Etag {
  NX6_TRGSMPL_OKAY            =  0, /* Successful          */
  NX6_TRGSMPL_ERR_INVAL_PARAM =  1  /* Invalid parameter   */
} NX6_TRGSMPL_RESULT_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
NX6_TRGSMPL_RESULT_E NX6_TriggerSample_SetPrm( NX6_TRGSMPL_PRM_E ePrmID, uint32_t ulPrmVal );
uint32_t NX6_TriggerSample_GetPrm( NX6_TRGSMPL_PRM_E ePrmID );
void NX6_TriggerSample_StartTriggerUnit( void );
void NX6_TriggerSample_StopTriggerUnit( void );
void NX6_TriggerSample_StartSampleUnit( void );
void NX6_TriggerSample_StopSampleUnit( void );
void NX6_TriggerSample_ResetStatus( uint32_t ulRstMsk );
uint32_t NX6_TriggerSample_GetIrq( void );
void NX6_TriggerSample_ConfirmIrq( uint32_t ulConfirmMsk );

#endif /* #ifndef __NETX6_TRGSMPL_H */
