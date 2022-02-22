#ifndef __HAL_SYSTIME_H
#define __HAL_SYSTIME_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void     SYSTIME_SetBorder    ( unsigned int uiInst, uint32_t ulBorder, void* pvUser );
void     SYSTIME_GetBorder    ( unsigned int uiInst, uint32_t* pulBorder, void* pvUser );
void     SYSTIME_SetSpeed     ( unsigned int uiInst, uint32_t ulSpeed, void* pvUser );
void     SYSTIME_GetSpeed     ( unsigned int uiInst, uint32_t* pulSpeed, void* pvUser );
void     SYSTIME_SetSystime   ( unsigned int uiInst, uint32_t ulSystime_s, uint32_t ulSystime_ns, void* pvUser );
void     SYSTIME_GetSystime   ( unsigned int uiInst, uint32_t* pulSystime_s, uint32_t* pulSystime_ns, void* pvUser );
uint32_t SYSTIME_GetSystimeNs ( unsigned int uiInst, void* pvUser );
uint32_t SYSTIME_GetSystimeS  ( unsigned int uiInst, void* pvUser );

#endif /* #ifndef __HAL_SYSTIME_H */
