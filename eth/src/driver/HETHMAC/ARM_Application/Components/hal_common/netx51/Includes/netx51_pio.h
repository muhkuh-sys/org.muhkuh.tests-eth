#ifndef __NETX51_PIO_H
#define __NETX51_PIO_H

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! PIO Mode                                                                 */
/*****************************************************************************/
typedef enum NX51_PIO_MODE_Etag {
  NX51_PIO_MODE_IN  = 0,
  NX51_PIO_MODE_OUT = 1
} NX51_PIO_MODE_E;


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void         NX51_PIO_SetMode (unsigned int uPioNum, NX51_PIO_MODE_E eMode);
unsigned int NX51_PIO_GetInput (unsigned int uPioNum);
uint32_t     NX51_PIO_GetIn( void );
unsigned int NX51_PIO_GetOutput (unsigned int uPioNum);
void         NX51_PIO_SetOutput (unsigned int uPioNum, unsigned int uEnable);
uint32_t     NX51_PIO_GetLine ( void );
void         NX51_PIO_SetLine( uint32_t ulVal );


#endif /* #ifndef __NETX51_PIO_H */
