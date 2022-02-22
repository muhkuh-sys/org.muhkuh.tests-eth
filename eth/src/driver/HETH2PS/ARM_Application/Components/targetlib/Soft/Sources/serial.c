#include "serial.h"
#include <stdio.h>

#define SERIAL_PORT_CNT 4

typedef struct SERIAL_Ttag
{
  unsigned int uiImpl;
  int (*fnPoll)(unsigned int uiImpl);
  int (*fnPeek)(unsigned int uiImpl);
  int (*fnRecv)(unsigned int uiImpl);
  int (*fnSend)(unsigned int uiImpl, int iChar);
  int (*fnFlush)(unsigned int uiImpl);
} SERIAL_T;

static SERIAL_T s_atSerial[SERIAL_PORT_CNT];

int serial_add( unsigned int uiPort,
                unsigned int uiImpl,
                int (*fnPoll)(unsigned int uiImpl),
                int (*fnPeek)(unsigned int uiImpl),
                int (*fnRecv)(unsigned int uiImpl),
                int (*fnSend)(unsigned int uiImpl, int iChar),
                int (*fnFlush)(unsigned int uiImpl))
{
  SERIAL_T* ptSerial;

  if( uiPort >= SERIAL_PORT_CNT )
  {
    return -1;
  }

  ptSerial = &s_atSerial[uiPort];

  ptSerial->uiImpl  = uiImpl;
  ptSerial->fnPoll  = fnPoll;
  ptSerial->fnPeek  = fnPeek;
  ptSerial->fnRecv  = fnRecv;
  ptSerial->fnSend  = fnSend;
  ptSerial->fnFlush = fnFlush;

  return 0;
}

int serial_poll(unsigned int uiPort)
{
  int iRslt = 0;
  SERIAL_T* ptSerial;

  if( uiPort >= SERIAL_PORT_CNT )
  {
    return -1;
  }

  ptSerial = &s_atSerial[uiPort];

  if( ptSerial->fnPoll != NULL )
  {
    iRslt = ptSerial->fnPoll(ptSerial->uiImpl);
    if( iRslt != 0 )
    {
      return iRslt;
    }
  }

  if( ptSerial->fnFlush != NULL )
  {
    iRslt = ptSerial->fnFlush(ptSerial->uiImpl);
  }

  return iRslt;
}

int serial_recv(unsigned int uiPort)
{
  SERIAL_T* ptSerial;

  if( uiPort >= SERIAL_PORT_CNT )
  {
    return EOF;
  }

  ptSerial = &s_atSerial[uiPort];

  if( ptSerial->fnPeek != NULL )
  {
    if( ptSerial->fnPeek(ptSerial->uiImpl) == 0 )
    {
      return EOF;
    }
  }

  if( ptSerial->fnRecv == NULL )
  {
    return EOF;
  }

  return ptSerial->fnRecv(ptSerial->uiImpl);
}

int serial_send(unsigned int uiPort, int iChar)
{
  SERIAL_T* ptSerial;

  if( uiPort >= SERIAL_PORT_CNT )
  {
    return EOF;
  }

  ptSerial = &s_atSerial[uiPort];

  if( ptSerial->fnSend != NULL )
  {
    return ptSerial->fnSend(ptSerial->uiImpl, iChar);
  }

  return EOF;
}

