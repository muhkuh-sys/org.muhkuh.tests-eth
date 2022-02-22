#ifndef __NETX6_PIO_H
#define __NETX6_PIO_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! PIO Mode                                                                 */
/*****************************************************************************/
typedef enum NX6_PIO_MODE_E {
  NX6_PIO_MODE_IN  = 0,
  NX6_PIO_MODE_OUT = 1
} NX6_PIO_MODE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX6_PIO_SetMode (unsigned int uPioNum, NX6_PIO_MODE_E eMode);
unsigned int NX6_PIO_GetInput (unsigned int uPioNum);
uint32_t NX6_PIO_GetIn( void );
unsigned int NX6_PIO_GetOutput (unsigned int uPioNum);
void NX6_PIO_SetOutput (unsigned int uPioNum, unsigned int uEnable);
uint32_t NX6_PIO_GetLine ( void );
void NX6_PIO_SetLine( uint32_t ulVal );


#endif /* #ifndef __NETX6_PIO_H */
