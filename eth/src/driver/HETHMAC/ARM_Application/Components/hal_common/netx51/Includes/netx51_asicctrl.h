#ifndef __NETX51_ASICCTRL_H
#define __NETX51_ASICCTRL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
typedef enum {
  NX51_RDYRUN_LED_OFF = 0,
  NX51_RDYRUN_LED_GREEN,
  NX51_RDYRUN_LED_RED,
  NX51_RDYRUN_LED_INV
} NX51_RDYRUN_LED_E;

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
void     NX51_AsicCtrl_SetRdyRunLed( NX51_RDYRUN_LED_E eMode );

#endif /* #ifndef __NETX56_ASICCTRL_H */
