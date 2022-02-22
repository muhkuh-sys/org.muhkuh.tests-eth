#ifndef __NETX6_PFIFO_H
#define __NETX6_PFIFO_H

#include <stdint.h>

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void      NX6_PFIFO_Reset( void );
int       NX6_PFIFO_SetBorders( const unsigned int *auiPFifoDepth );
int       NX6_PFIFO_GetBorders( unsigned int *auiPFifoDepth );
uint32_t  NX6_PFIFO_GetFillLevel( unsigned int uFifoNum );
uint32_t  NX6_PFIFO_GetFifoResetVector( void );
uint32_t  NX6_PFIFO_GetFifoFullVector( void );
uint32_t  NX6_PFIFO_GetFifoEmptyVector( void );
uint32_t  NX6_PFIFO_GetFifoOverflowVector( void );
uint32_t  NX6_PFIFO_GetFifoUnderrunVector( void );

#endif /* #ifndef __NETX6_PFIFO_H */
