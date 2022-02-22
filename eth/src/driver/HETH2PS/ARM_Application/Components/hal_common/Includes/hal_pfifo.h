#ifndef __HAL_PFIFO_H
#define __HAL_PFIFO_H

#include <stdint.h>

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void      PFIFO_Reset                    ( unsigned int uiInst );
int       PFIFO_SetBorders               ( unsigned int uiInst, const unsigned int* auiPFifoDepth );
int       PFIFO_GetBorders               ( unsigned int uiInst, unsigned int* auiPFifoDepth );
uint32_t  PFIFO_GetBorder                ( unsigned int uiInst, unsigned int uiFifoNum );
uint32_t  PFIFO_GetFillLevel             ( unsigned int uiInst, unsigned int uiFifoNum );
int       PFIFO_SetFifoResetVector       ( unsigned int uiInst, unsigned long ulVal );
uint32_t  PFIFO_GetFifoResetVector       ( unsigned int uiInst );
uint32_t  PFIFO_GetFifoFullVector        ( unsigned int uiInst );
uint32_t  PFIFO_GetFifoEmptyVector       ( unsigned int uiInst );
uint32_t  PFIFO_GetFifoOverflowVector    ( unsigned int uiInst );
uint32_t  PFIFO_GetFifoUnderrunVector    ( unsigned int uiInst );
uint32_t  PFIFO_ReadFifo                 ( unsigned int uiInst, unsigned int uiFifoNum );
uint32_t  PFIFO_WriteFifo                ( unsigned int uiInst, unsigned int uiFifoNum, uint32_t ulVal );


#endif /* #ifndef __HAL_PFIFO_H */
