
#include "usb_io.h"
#include "usb_regmap.h"


void usb_io_read_fifo(unsigned int uiDwOffset, unsigned int uiByteCount, unsigned char *pucBuffer)
{
  const unsigned char *pucSc;
  unsigned char *pucDc, *pucDe;

  pucSc = (const unsigned char*)(NETX_USB_FIFO + (uiDwOffset<<2));
  pucDc = pucBuffer;
  pucDe = pucDc + uiByteCount;

  while( pucDc<pucDe ) {
    *pucDc++ = *pucSc++;
  }
}


void usb_io_write_fifo(unsigned int uiDwOffset, unsigned int uiByteCount, const unsigned char *pucBuffer)
{
  /*
    NOTE: the ahbl switch can not write bytewise to registers or all other
    bytes in the dword are set to the same value
  */
	const unsigned char *pucSc;
	unsigned long *pulDc, *pulDe;
	unsigned int uiByteCnt;
	unsigned long ulValue;


  /* round up the bytecount */
  uiByteCount += 3;

	pucSc = pucBuffer;
	pulDc = (unsigned long*)(NETX_USB_FIFO + (uiDwOffset<<2));
	pulDe = pulDc + (uiByteCount>>2);

  uiByteCnt = 0;
  ulValue = 0;
  while( pulDc<pulDe ) {
    ulValue >>= 8;
    ulValue  |= (*pucSc++)<<24;
    uiByteCnt++;
    if( (uiByteCnt&3)==0 ) {
            *pulDc++ = ulValue;
    }
  }
}


void usb_io_sendStall(void)
{
  POKE(PIPE_CFG,(PEEK(PIPE_CFG) | bmSTALL));
}


void usb_io_sendZeroPacket(void)
{
  POKE(PIPE_CTRL, (bmACT | TPID_IN));
  POKE(PIPE_DATA_TBYTES, bmDBV);
}


void usb_io_sendDataPacket(unsigned int data_length, const unsigned char *data)
{
  usb_io_write_fifo(0x00, data_length, data);

  POKE(PIPE_CTRL, (bmACT | TPID_IN));
  POKE(PIPE_DATA_PTR, 0);
  POKE(PIPE_DATA_TBYTES, (data_length | bmDBV));
}

