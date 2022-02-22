#ifndef __HAL_SYSTIME_LT_H
#define __HAL_SYSTIME_LT_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void SYSTIMELT_ReqLatch    ( unsigned int uiInst, uint32_t ulReqMsk );
void SYSTIMELT_GetSystime  ( unsigned int uiInst, unsigned int uiSystimeIdx, uint32_t* pulSystime_s, uint32_t* pulSystime_ns );

#endif /* #ifndef __HAL_SYSTIME_LT_H */
