#ifndef __NETX51_PFIFO_H
#define __NETX51_PFIFO_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void      NX51_PFIFO_Reset(void);
int       NX51_PFIFO_SetBorders(const unsigned int *auiPFifoDepth);
int       NX51_PFIFO_GetBorders(unsigned int *auiPFifoDepth);
uint32_t  NX51_PFIFO_GetFillLevel(unsigned int uFifoNum);
uint32_t  NX51_PFIFO_GetFifoResetVector(void);
uint32_t  NX51_PFIFO_GetFifoFullVector(void);
uint32_t  NX51_PFIFO_GetFifoEmptyVector(void);
uint32_t  NX51_PFIFO_GetFifoOverflowVector(void);
uint32_t  NX51_PFIFO_GetFifoUnderrunVector(void);


#endif /* #ifndef __NETX51_PFIFO_H */
