#ifndef __USB_IO_H
#define __USB_IO_H

#include "usb_types.h"


void usb_io_read_fifo(unsigned int uiDwOffset, unsigned int uiByteCount, unsigned char *pucBuffer);
void usb_io_write_fifo(unsigned int uiDwOffset, unsigned int uiByteCount, const unsigned char *pucBuffer);

void usb_io_sendStall(void);
void usb_io_sendZeroPacket(void);
void usb_io_sendDataPacket(unsigned int data_length, const unsigned char *data);

#endif /* __USB_IO_H */
