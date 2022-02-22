#ifndef __NETX51_ASICCTRL_H
#define __NETX51_ASICCTRL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void     NX51_AsicCtrl_SetIoConfig( uint32_t ulVal );
uint32_t NX51_AsicCtrl_GetIoConfig( void );
void     NX51_AsicCtrl_SetIoConfig2( uint32_t ulVal );
uint32_t NX51_AsicCtrl_GetIoConfig2( void );
void     NX51_AsicCtrl_SetClockEnable( uint32_t ulVal );
uint32_t NX51_AsicCtrl_GetClockEnable( void );
void     NX51_AsicCtrl_SetResetCtrl( uint32_t ulVal );
uint32_t NX51_AsicCtrl_GetResetCtrl( void );

#endif /* #ifndef __NETX56_ASICCTRL_H */
