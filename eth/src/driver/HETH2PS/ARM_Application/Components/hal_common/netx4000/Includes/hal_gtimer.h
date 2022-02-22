#ifndef __HAL_GTIMER_H
#define __HAL_GTIMER_H

#include <stdint.h>

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

/* CPU granularity
 * Calling these functions in one core has impact on all other cores
 */
void      GTimer_Start     ( uint64_t ulStartVal );
void      GTimer_Stop      ( void );

/* CPU Core granularity
 * Calling these functions in one core has no impact on the other cores
 */
uint64_t  GTimer_GetTimer  ( void );
void      GTimer_StartCmp  ( uint64_t ulCmpVal, uint32_t ulAutoInc );
void      GTimer_StopCmp   ( void );
uint32_t  GTimer_GetIrqRaw ( void );
void      GTimer_CnfIrq    ( uint32_t ulIrq );

#endif /* #ifndef __HAL_GTIMER_H */
