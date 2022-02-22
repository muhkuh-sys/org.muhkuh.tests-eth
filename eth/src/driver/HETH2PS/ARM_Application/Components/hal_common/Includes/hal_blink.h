#ifndef __HAL_BLINK_H
#define __HAL_BLINK_H

#include <stdint.h>
#include <stdbool.h>

/* Definitions */

/* Function prototypes */

int  BLINK_ConfigureChannel (unsigned int uiInst, unsigned int uiChannel, unsigned int uiPeriod10ms, unsigned int uiBlinkLen, uint32_t ulSeq);
void BLINK_StartStop        (unsigned int uiInst, bool fStartStop, bool fCh0, bool fCh1, bool fCh2, bool fCh3);

#endif /* #ifndef __HAL_BLINK_H */
