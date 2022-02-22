/****************************************************************************************
*  Serial Byte I/O
*  API
****************************************************************************************/

#ifndef __SERIAL_H
#define __SERIAL_H

/****************************************************************************************
*  Functions
****************************************************************************************/

int serial_add(unsigned int uiPort,
               unsigned int uiImpl,
               int (*fnPoll)(unsigned int uiImpl),
               int (*fnPeek)(unsigned int uiImpl),
               int (*fnRecv)(unsigned int uiImpl),
               int (*fnSend)(unsigned int uiImpl, int iChar),
               int (*fnFlush)(unsigned int uiImpl));

int serial_poll(unsigned int uiPort);
int serial_recv(unsigned int uiPort);
int serial_send(unsigned int uiPort, int iChar);

#endif /* __SERIAL_H */
