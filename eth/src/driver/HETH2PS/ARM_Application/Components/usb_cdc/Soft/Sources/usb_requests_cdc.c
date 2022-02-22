#include "usb_regmap.h"
#include "usb_requests_common.h"
#include "usb_requests_cdc.h"
#include "usb_descriptors.h"
#include "usb_globals.h"
#include "usb_io.h"

/* ------------------------------------- */

#define USB_CDC_BUF_REC_LEN 256
#define USB_CDC_BUF_SEND_LEN 256

unsigned char usb_cdc_buf_rec[USB_CDC_BUF_REC_LEN];
unsigned char usb_cdc_buf_send[USB_CDC_BUF_SEND_LEN];

unsigned int usb_cdc_buf_rec_rpos;              /* read position */
unsigned int usb_cdc_buf_rec_wpos;              /* write position */
unsigned int usb_cdc_buf_rec_fill;              /* fill level */

unsigned int usb_cdc_buf_send_rpos;             /* read position */
unsigned int usb_cdc_buf_send_wpos;             /* write position */
unsigned int usb_cdc_buf_send_fill;             /* fill level */

unsigned long ulUsbCdc_Settings_Baudrate;
tUsbCdc_StopBits tUsbCdc_Settings_StopBits;
tUsbCdc_Parity tUsbCdc_Settings_Parity;
unsigned long ulUsbCdc_Settings_DataBits;

/* -------------------------------------*/

void usb_cdc_init(void)
{
  /* set default line settings of 115.2k, 8N1 */
  ulUsbCdc_Settings_Baudrate      = 115200;
  tUsbCdc_Settings_StopBits       = tUsbCdc_StopBits_1;
  tUsbCdc_Settings_Parity         = tUsbCdc_Parity_None;
  ulUsbCdc_Settings_DataBits      = 8;

  /* init receive buffer */
  usb_cdc_buf_rec_rpos = 0;
  usb_cdc_buf_rec_wpos = 0;
  usb_cdc_buf_rec_fill = 0;

  /* init send buffer */
  usb_cdc_buf_send_rpos = 0;
  usb_cdc_buf_send_wpos = 0;
  usb_cdc_buf_send_fill = 0;
}

/* ------------------------------------- */

packet_handler_stall_req_t setup_cdc_set_line_coding(setupPacket_t *pPacket);
packet_handler_stall_req_t setup_cdc_get_line_coding(setupPacket_t *pPacket);
packet_handler_stall_req_t setup_cdc_set_control_line_state(setupPacket_t *pPacket);

packet_handler_stall_req_t usb_requests_handle_cdc(setupPacket_t *pPacket)
{
  setup_cdc_requestId_t tCdcReqId;


  tCdcReqId = (setup_cdc_requestId_t)pPacket->tHeader.reqId;
  switch( tCdcReqId ) {
  case SETUP_CDC_REQID_Set_Line_Coding:
    return setup_cdc_set_line_coding(pPacket);
  case SETUP_CDC_REQID_Get_Line_Coding:
    return setup_cdc_get_line_coding(pPacket);
  case SETUP_CDC_REQID_Set_Control_Line_State:
    return setup_cdc_set_control_line_state(pPacket);

  default:
    return PKT_HANDLER_Send_Stall;
  }
}

packet_handler_stall_req_t setup_cdc_set_line_coding(setupPacket_t *pPacket)
{
  unsigned long ulBdRate;
  tUsbCdc_StopBits tStopBits;
  tUsbCdc_Parity tParity;
  unsigned long ulDataBits;


  if( pPacket->uiDataLen<7 ) {
    /* invalid length, stall packet */
    return PKT_HANDLER_Send_Stall;
  }

  /* get the temp values from the packet */

  ulBdRate  = pPacket->abData[0];
  ulBdRate |= (pPacket->abData[1]<<8);
  ulBdRate |= (pPacket->abData[2]<<16);
  ulBdRate |= (pPacket->abData[3]<<24);

  tStopBits = (tUsbCdc_StopBits)pPacket->abData[4];
  tParity = (tUsbCdc_Parity)pPacket->abData[5];
  ulDataBits = pPacket->abData[6];

  /* check the baudrate */
  if(
    ulBdRate!=300 &&
    ulBdRate!=600 &&
    ulBdRate!=1200 &&
    ulBdRate!=2400 &&
    ulBdRate!=4800 &&
    ulBdRate!=9600 &&
    ulBdRate!=19200 &&
    ulBdRate!=38400 &&
    ulBdRate!=57600 &&
    ulBdRate!=115200
  ) {
    /* invalid baudrate, stall packet */
    return PKT_HANDLER_Send_Stall;
  }
  if( tStopBits>2 ) {
    /* invalid stop bits, stall packet */
    return PKT_HANDLER_Send_Stall;
  }
  if( tParity>4 ) {
    /* invalid parity, stall packet */
    return PKT_HANDLER_Send_Stall;
  }
  if( ulDataBits!=8 ) {
    /* invalid data bits, stall packet */
    return PKT_HANDLER_Send_Stall;
  }

  /* all values ok! */
  ulUsbCdc_Settings_Baudrate      = ulBdRate;
  tUsbCdc_Settings_StopBits       = tStopBits;
  tUsbCdc_Settings_Parity         = tParity;
  ulUsbCdc_Settings_DataBits      = ulDataBits;

  usb_io_sendZeroPacket();

  return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_cdc_get_line_coding(setupPacket_t *pPacket)
{
  unsigned char abLineCoding[7];


  /* get baudrate */
  abLineCoding[0] = ulUsbCdc_Settings_Baudrate&0xff;
  abLineCoding[1] = (ulUsbCdc_Settings_Baudrate>>8)&0xff;
  abLineCoding[2] = (ulUsbCdc_Settings_Baudrate>>16)&0xff;
  abLineCoding[3] = (ulUsbCdc_Settings_Baudrate>>24)&0xff;

  /* get stop bits */
  abLineCoding[4] = (unsigned char)tUsbCdc_Settings_StopBits;

  /* get parity */
  abLineCoding[5] = (unsigned char)tUsbCdc_Settings_Parity;

  /* get data bits */
  abLineCoding[6] = (unsigned char)ulUsbCdc_Settings_DataBits;

  sendDescriptor(pPacket->tHeader.wLength, 7, abLineCoding);
  return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_cdc_set_control_line_state(setupPacket_t *pPacket)
{
	/* eval DTR (bit 0) and RTS (bit 1) to enable or disable send and receive part */
	tCdcConnectionState = (USB_CDC_ConnectionState_t)(pPacket->tHeader.wValue & 1);

  /* just ignore all values for now */
  usb_io_sendZeroPacket();
  return PKT_HANDLER_Do_Not_Send_Stall;
}

/* ------------------------------------- */

/* Buffer handling, User level functions */

/* 'peek' returns the number of chars waiting in the receive buffer */
int usb_cdc_buf_rec_peek(unsigned int uiPort)
{
  return (usb_cdc_buf_rec_fill == 0) ? 0 : 1;
}

/* 'get' waits for a char and returns it */
int usb_cdc_buf_rec_get(unsigned int uiPort)
{
  unsigned char bValue;

  /* is at least one char in the buffer? */
  while( usb_cdc_buf_rec_fill==0 ) {
    usb_pingpong();
  }

  /* get one char from the buffer */
  bValue = usb_cdc_buf_rec[usb_cdc_buf_rec_rpos];

  /* advance read position */
  ++usb_cdc_buf_rec_rpos;

  /* wrap position? */
  if( usb_cdc_buf_rec_rpos>=USB_CDC_BUF_REC_LEN ) {
    usb_cdc_buf_rec_rpos = 0;
  }

  /* decrement the buffer fill value */
  --usb_cdc_buf_rec_fill;

  /* need to reactivate the endpoint? */
  if( tReceiveEpState==USB_ReceiveEndpoint_Blocked && (USB_CDC_BUF_REC_LEN-usb_cdc_buf_rec_fill)>=Usb_Ep2_PacketSize ) {
    usb_activateInputPipe();
  }

  /* return the character */
  return (int)bValue;
}

/* 'put' waits for buffer space and puts the char into the buffer */
int usb_cdc_buf_send_put(unsigned int uiPort, int iValue)
{
  /* is at least one char free in the buffer? */
  while( usb_cdc_buf_send_fill==USB_CDC_BUF_SEND_LEN ) {
    usb_pingpong();

    /* stop if not connected */
    if( tCdcConnectionState!=USB_CDC_ConnectionState_Connected )
      return -1;
  }

  /* put one char in the buffer */
  usb_cdc_buf_send[usb_cdc_buf_send_wpos] = (unsigned char)iValue;

  /* advance wait position */
  ++usb_cdc_buf_send_wpos;

  /* wrap position? */
  if( usb_cdc_buf_send_wpos>=USB_CDC_BUF_SEND_LEN ) {
    usb_cdc_buf_send_wpos = 0;
  }

  /* increment the buffer fill value */
  ++usb_cdc_buf_send_fill;

  /* enough data to reactivate the endpoint? */
  if( tSendEpState==USB_SendEndpoint_Idle && usb_cdc_buf_send_fill>=Usb_Ep3_PacketSize ) {
    usb_sendPendingPacket();
  }

  return 0;
}


int usb_cdc_buf_send_flush(unsigned int uiPort)
{
  /* send all remaining characters in the send buffer */
  if( usb_cdc_buf_send_fill==0 ) {
    return 0;
  }

  /* endpoint idle -> reactivate it */
  if( tSendEpState==USB_SendEndpoint_Idle ) {
    usb_sendPendingPacket();
  }
  /* wait for all bytes to be sent */
  do {
    usb_pingpong();

    /* stop if not connected */
    if( tCdcConnectionState!=USB_CDC_ConnectionState_Connected )
      return -1;
  } while( usb_cdc_buf_send_fill!=0 );

  return 0;
}


/* ------------------------------------- */ 

/* Buffer handling, Usb level functions */

tUsbCdc_BufferState usb_cdc_buf_rec_put(unsigned char *pbBuffer, unsigned int uiBufferLen, unsigned int uiMaxPacketSize)
{
  unsigned int uiBufferLeft;
  unsigned char *pbC, *pbE;


  /* 0 bytes always succeed */
  if( uiBufferLen==0 ) {
    return tUsbCdc_BufferState_Ok;
  }

  /* test number of bytes left in the buffer */
  uiBufferLeft = USB_CDC_BUF_REC_LEN - usb_cdc_buf_rec_fill;
  if( uiBufferLen>uiBufferLeft ) {
    return tUsbCdc_BufferState_Rejected;
  }

  /* fill the bytes into the buffer */

  /* copy the buffer */
  pbC = pbBuffer;
  pbE = pbC + uiBufferLen;
  do {
    usb_cdc_buf_rec[usb_cdc_buf_rec_wpos++] = *(pbC++);
    if( usb_cdc_buf_rec_wpos>=USB_CDC_BUF_REC_LEN ) {
      usb_cdc_buf_rec_wpos = 0;
    }
  } while( pbC<pbE );

  /* increment the fill value */
  usb_cdc_buf_rec_fill += uiBufferLen;

  uiBufferLeft = USB_CDC_BUF_REC_LEN - usb_cdc_buf_rec_fill;
  if( uiBufferLeft<uiMaxPacketSize ) {
    /* ok, but the buffer can't hold another packet */
    return tUsbCdc_BufferState_Full;
  } else {
    /* ok */
    return tUsbCdc_BufferState_Ok;
  }
}


unsigned int usb_cdc_buf_send_get(unsigned char *pbBuffer, unsigned int uiMaxPacketSize)
{
  unsigned int uiBufferLen;
  unsigned char *pbD, *pbE;


  /* get the number of waiting chars in the buffer, but max one packet */
  uiBufferLen = (usb_cdc_buf_send_fill > uiMaxPacketSize) ? uiMaxPacketSize : usb_cdc_buf_send_fill;

  if( uiBufferLen!=0 ) {
    /* copy the buffer */
    pbD = pbBuffer;
    pbE = pbD + uiBufferLen;
    do {
      *(pbD++) = usb_cdc_buf_send[usb_cdc_buf_send_rpos++];
      if( usb_cdc_buf_send_rpos>=USB_CDC_BUF_REC_LEN ) {
              usb_cdc_buf_send_rpos = 0;
      }
    } while( pbD<pbE );

    /* decrement the fill value */
    usb_cdc_buf_send_fill -= uiBufferLen;
  }

  return uiBufferLen;
}
