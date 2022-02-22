#ifndef __HAL_USBCDC_H
#define __HAL_USBCDC_H

#include <stdbool.h>

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void USBCDC_Init(unsigned int uiPort);
void USBCDC_Connect(unsigned int uiPort);
bool USBCDC_IsConnected(unsigned int uiPort);

int USBCDC_BufSpace(unsigned int uiPort);
int USBCDC_BufLevel(unsigned int uiPort);

int USBCDC_BufRecPeek(unsigned int uiPort);
int USBCDC_BufRecGet(unsigned int uiPort);
int USBCDC_BufSendPut(unsigned int uiPort, int iChar);
int USBCDC_BufSendFlush(unsigned int uiPort);

#endif  /* #ifndef __HAL_USBCDC_H */
