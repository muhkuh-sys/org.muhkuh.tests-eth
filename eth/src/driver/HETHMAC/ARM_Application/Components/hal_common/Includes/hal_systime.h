#ifndef __HAL_SYSTIME_H
#define __HAL_SYSTIME_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
#ifndef HALDEC
#define HALDEC(name) name
#endif

void        HALDEC(SYSTIME_SetBorder)              ( unsigned int uInst, uint32_t ulBorder, void* pvUser );
void        HALDEC(SYSTIME_GetBorder)              ( unsigned int uInst, uint32_t* pulBorder, void* pvUser );
void        HALDEC(SYSTIME_SetSpeed)               ( unsigned int uInst, uint32_t ulSpeed, void* pvUser );
void        HALDEC(SYSTIME_GetSpeed)               ( unsigned int uInst, uint32_t* pulSpeed, void* pvUser );
void        HALDEC(SYSTIME_SetSystime)             ( unsigned int uInst, uint32_t ulSystime_s, uint32_t ulSystime_ns, void* pvUser );
void        HALDEC(SYSTIME_GetSystime)             ( unsigned int uInst, uint32_t* pulSystime_s, uint32_t* pulSystime_ns, void* pvUser );
uint32_t    HALDEC(SYSTIME_GetSystimeNs)           ( unsigned int uInst, void* pvUser );
uint32_t    HALDEC(SYSTIME_GetSystimeS)            ( unsigned int uInst, void* pvUser );

#endif /* #ifndef __HAL_SYSTIME_H */
