#ifndef __HAL_PFIFO_H
#define __HAL_PFIFO_H

#include <stdint.h>

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

void      HALDEC(PFIFO_Reset)                    ( unsigned int uInstNo );
int       HALDEC(PFIFO_SetBorders)               ( unsigned int uInstNo, uint32_t* auPFifoDepth );
uint32_t  HALDEC(PFIFO_GetBorder)                ( unsigned int uInstNo, unsigned int uFifoNum );
uint32_t  HALDEC(PFIFO_GetFillLevel)             ( unsigned int uInstNo, unsigned int uFifoNum );
int       HALDEC(PFIFO_SetFifoResetVector)       ( unsigned int uInstNo, unsigned long ulVal );
uint32_t  HALDEC(PFIFO_GetFifoResetVector)       ( unsigned int uInstNo );
uint32_t  HALDEC(PFIFO_GetFifoFullVector)        ( unsigned int uInstNo );
uint32_t  HALDEC(PFIFO_GetFifoEmptyVector)       ( unsigned int uInstNo );
uint32_t  HALDEC(PFIFO_GetFifoOverflowVector)    ( unsigned int uInstNo );
uint32_t  HALDEC(PFIFO_GetFifoUnderrunVector)    ( unsigned int uInstNo );
uint32_t  HALDEC(PFIFO_ReadFifo)                 ( unsigned int uInstNo, unsigned int uFifoNum );
uint32_t  HALDEC(PFIFO_WriteFifo)                ( unsigned int uInstNo, unsigned int uFifoNum, uint32_t ulVal );


#endif /* #ifndef __HAL_PFIFO_H */
