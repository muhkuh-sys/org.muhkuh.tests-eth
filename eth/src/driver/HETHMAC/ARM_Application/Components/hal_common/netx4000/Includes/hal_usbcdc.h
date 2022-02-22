#ifndef __HAL_USBCDC_H
#define __HAL_USBCDC_H

#include <stdbool.h>

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
#ifndef HALDEC
#define HALDEC(name) name
#endif

void HALDEC(USBCDC_Init)(void);
void HALDEC(USBCDC_Connect)(void);
bool HALDEC(USBCDC_IsConnected)(void);

int HALDEC(USBCDC_BufSpace)(void);
int HALDEC(USBCDC_BufLevel)(void);

unsigned int HALDEC(USBCDC_BufRecPeek)(void);
unsigned char HALDEC(USBCDC_BufRecGet)(void);
void HALDEC(USBCDC_BufSendPut)(unsigned char bValue);
void HALDEC(USBCDC_BufSendFlush)(void);

#endif  /* #ifndef __HAL_USBCDC_H */

