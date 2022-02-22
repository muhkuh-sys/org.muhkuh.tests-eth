#ifndef __NETX6_ASICCTRL_H
#define __NETX6_ASICCTRL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void     NX6_AsicCtrl_SetIoConfig( uint32_t ulVal );
uint32_t NX6_AsicCtrl_GetIoConfig( void );
void     NX6_AsicCtrl_SetIoConfig2( uint32_t ulVal );
uint32_t NX6_AsicCtrl_GetIoConfig2( void );
void     NX6_AsicCtrl_SetClockEnable( uint32_t ulVal );
uint32_t NX6_AsicCtrl_GetClockEnable( void );
void     NX6_AsicCtrl_SetResetCtrl( uint32_t ulVal );
uint32_t NX6_AsicCtrl_GetResetCtrl( void );

#endif /* #ifndef __NETX56_ASICCTRL_H */
