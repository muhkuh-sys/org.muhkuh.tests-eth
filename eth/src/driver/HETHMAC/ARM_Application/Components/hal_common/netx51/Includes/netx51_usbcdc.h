#ifndef __NETX51_USBCDC_H
#define __NETX51_USBCDC_H

#include <stdbool.h>

void NX51_USBCDC_Init(void);
bool NX51_USBCDC_IsConnected(void);

unsigned int NX51_USBCDC_BufRecPeek(void);
unsigned char NX51_USBCDC_BufRecGet(void);
void NX51_USBCDC_BufSendPut(unsigned char bValue);
void NX51_USBCDC_BufSendFlush(void);

int NX51_USBCDC_BufSpace(void);
int NX51_USBCDC_BufLevel(void);

#endif  /* #ifndef __NETX51_USBCDC_H */

