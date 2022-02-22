#ifndef __HAL_GTIMER_H
#define __HAL_GTIMER_H

#include <stdint.h>

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

/* CPU granularity
 * Calling these functions in one core has impact on all other cores
 */
void      HALDEC(GTimer_Start)     ( uint64_t ulStartVal );
void      HALDEC(GTimer_Stop)      ( void );

/* CPU Core granularity
 * Calling these functions in one core has no impact on the other cores
 */
uint64_t  HALDEC(GTimer_GetTimer)  ( void );
void      HALDEC(GTimer_StartCmp)  ( uint64_t ulCmpVal, uint32_t ulAutoInc );
void      HALDEC(GTimer_StopCmp)   ( void );
uint32_t  HALDEC(GTimer_GetIrqRaw) ( void );
void      HALDEC(GTimer_CnfIrq)    ( uint32_t ulIrq );

#endif /* #ifndef __HAL_GTIMER_H */
