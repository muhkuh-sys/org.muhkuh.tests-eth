#ifndef __NETX6_RDYRUN_H
#define __NETX6_RDYRUN_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
typedef enum NX6_RDYRUN_LED_Etag {
  NX6_RDYRUN_LED_OFF   = 0,
  NX6_RDYRUN_LED_GREEN,
  NX6_RDYRUN_LED_RED,
  NX6_RDYRUN_LED_INV
} NX6_RDYRUN_LED_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX6_RDYRUN_SetRdyRunLed( NX6_RDYRUN_LED_E tMode );

#endif /* #ifndef __NETX6_RDYRUN_H */
