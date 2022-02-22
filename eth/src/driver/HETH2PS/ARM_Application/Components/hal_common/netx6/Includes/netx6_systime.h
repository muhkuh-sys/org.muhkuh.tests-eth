#ifndef __NETX6_SYSTIME_H
#define __NETX6_SYSTIME_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX6_SYSTIME_SetBorder( uint32_t ulBorder, void* pvUser );
void NX6_SYSTIME_GetBorder( uint32_t* pulBorder, void* pvUser );
void NX6_SYSTIME_SetSpeed( uint32_t ulSpeed, void* pvUser );
void NX6_SYSTIME_GetSpeed( uint32_t* pulSpeed, void* pvUser );
void NX6_SYSTIME_SetSystime( uint32_t ulSystime_s, uint32_t ulSystime_ns, void* pvUser );
void NX6_SYSTIME_GetSystime( uint32_t* pulSystime_s, uint32_t* pulSystime_ns, void* pvUser );
uint32_t NX6_SYSTIME_GetSystimeNs( void* pvUser );

#endif /* #ifndef __NETX6_SYSTIME_H */
