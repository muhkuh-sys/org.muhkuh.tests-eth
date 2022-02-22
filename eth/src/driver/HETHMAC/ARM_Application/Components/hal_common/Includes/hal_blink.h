#ifndef __HAL_BLINK_H
#define __HAL_BLINK_H

#include <stdint.h>
#include <stdbool.h>

/* Definitions */

/* Function prototypes */
#ifndef HALDEC
#define HALDEC(name) name
#endif

int  HALDEC(BLINK_ConfigureChannel) (unsigned int uInst, unsigned int uiChannel, unsigned int uiPeriod10ms, unsigned int uiBlinkLen, uint32_t ulSeq);
void HALDEC(BLINK_StartStop)        (unsigned int uInst, bool fStartStop, bool fCh0, bool fCh1, bool fCh2, bool fCh3);

#endif /* #ifndef __HAL_BLINK_H */
