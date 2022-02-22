#ifndef __NETX6_PFIFO_H
#define __NETX6_PFIFO_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void      NX6_PFIFO_Reset( void );
int       NX6_PFIFO_SetBorders( uint32_t *aulPFifoDepth );
uint32_t  NX6_PFIFO_GetFillLevel( unsigned int uFifoNum );
uint32_t  NX6_PFIFO_GetFifoResetVector( void );
uint32_t  NX6_PFIFO_GetFifoFullVector( void );
uint32_t  NX6_PFIFO_GetFifoEmptyVector( void );
uint32_t  NX6_PFIFO_GetFifoOverflowVector( void );
uint32_t  NX6_PFIFO_GetFifoUnderrunVector( void );


#endif /* #ifndef __NETX6_PFIFO_H */
