
#include "usb_regmap.h"
#include "usb_io.h"               /* USB commands */
#include "usb_requests_cdc.h"     /* USB CDC commands */
#include "usb_requests_common.h"  /* USB request commands */
#include "usb_requests_top.h"     /* USB request commands */
#include "usb_globals.h"          /* Defines the global USB parameters */

/* --------------------------------------------------------------------------- */

USB_State_t globalState;

unsigned int currentConfig;

unsigned int uiCurrentInterface;

/* buffer for setup and data packets */
unsigned char setupBuffer[Usb_Ep0_PacketSize];
unsigned char receiveBuffer[Usb_Ep2_PacketSize];
unsigned char sendBuffer[Usb_Ep3_PacketSize];

/* decoded packet */
setupPacket_t tSetupPkt;
/* out transaction needed */
USB_SetupTransaction_t tOutTransactionNeeded;

USB_ReceiveEndpoint_t tReceiveEpState;
USB_SendEndpoint_t tSendEpState;
unsigned int uiLastPacketSize;

/* new address for pending address change */
unsigned int uiNewAddress;

USB_EndpointState_t endpoint0State;
USB_EndpointState_t endpoint1State;
USB_EndpointState_t endpoint2State;
USB_EndpointState_t endpoint3State;

/* CDC Connection State */
USB_CDC_ConnectionState_t tCdcConnectionState;


/***************************/
/* Initialize the USB port */
/***************************/

void usb_init(void)
{
  globalState = USB_State_Attached;

  endpoint0State = USB_EndpointState_Unconfigured;
  endpoint1State = USB_EndpointState_Unconfigured;
  endpoint2State = USB_EndpointState_Unconfigured;
  endpoint3State = USB_EndpointState_Unconfigured;

  /* no configuration set */
  currentConfig = 0;

  /* default interface is 0 */
  uiCurrentInterface = 0;

  /* init cdc layer */
  usb_cdc_init();

   /* no connection */
  tCdcConnectionState = USB_CDC_ConnectionState_Idle;

  /* soft reset */
  POKE(NETX_USB_CORE_CTRL, 0x0001);
  /* release reset and set ID Func */
  POKE(NETX_USB_CORE_CTRL, 0x0008);

  /* read usb id, must be a device */
  PEEK(ID);

  /* set ID pullup and read connector ID value */
  POKE(PORT_CTRL, (PEEK(PORT_CTRL) | bmID_PU));
  POKE(PORT_CTRL, 0);
}

/* --------------------------------------------------------------------------- */

USB_CDC_ConnectionState_t usb_pollConnection(void)
{
  unsigned int portStatus;
  unsigned int portEvent;
  USB_CDC_ConnectionState_t tState;


  /* default is 'not connected' */
  tState = USB_CDC_ConnectionState_Idle;

  switch( globalState ) {
  case USB_State_Attached:

    /* wait until bus power is detected and set termination */
    portStatus = PEEK(PORT_STAT);
    if( (portStatus&bmVBUS_VLD)!=0 ) {
      globalState = USB_State_Powered;

      /* set termination */
      POKE(PORT_CTRL, 0x180);
    }
  break;

  case USB_State_Powered:

    /* wait for reset event */
    portEvent = PEEK(MAIN_EV);
    if( (portEvent&bmGPORT_EV)!=0 ) {
      usb_handleReset();
    }
  break;

  case USB_State_Default:
  case USB_State_AddressChangePending:
  case USB_State_Address:
  case USB_State_Configured:
  case USB_State_Suspended:
    usb_pingpong();
    tState = tCdcConnectionState;
    break;
  }

  return tState;
}

/* --------------------------------------------------------------------------- */

/******************************/
/* Sent the pending packets   */
/******************************/

void usb_sendPendingPacket(void)
{
  unsigned int uiPacketSize;


  uiPacketSize = usb_cdc_buf_send_get(sendBuffer, Usb_Ep3_PacketSize);
  if( uiPacketSize!=0 ) {
    usb_io_write_fifo(Usb_Ep3_Buffer>>2, uiPacketSize, sendBuffer);
    POKE(PIPE_SEL, 0x03);
    POKE(PIPE_CTRL, (bmACT | TPID_IN));
    POKE(NETX_USB_PIPE_DATA_PTR, Usb_Ep3_Buffer>>2);
    uiLastPacketSize = uiPacketSize;
    POKE(PIPE_DATA_TBYTES, uiPacketSize|bmDBV);
    tSendEpState = USB_SendEndpoint_Running;

    /* Wait until the packet is sent. */
    while(0 == (PEEK(PIPE_EV) & (1<<3)));

  } else {
    /* no more data waiting and last packet was full? -> send 0 packet */
    if( tSendEpState==USB_SendEndpoint_Running && uiLastPacketSize==Usb_Ep3_PacketSize ) {
      POKE(PIPE_SEL, 0x03);
      POKE(PIPE_CTRL, (bmACT | TPID_IN));
      POKE(PIPE_DATA_TBYTES, bmDBV);
    }
    tSendEpState = USB_SendEndpoint_Idle;
  }
}

/* --------------------------------------------------------------------------- */

/******************************/
/* Keep the connection alive  */
/******************************/

void usb_pingpong(void)
{
  unsigned int event1;
  unsigned int event2;
  unsigned int packetSize;
  unsigned long ulDataToggle;
  tUsbCdc_BufferState tBufState;


  event1 = PEEK(MAIN_EV);
  if( (event1&bmGPORT_EV)!=0 ) {
    usb_handleReset();
  }

  /* Check if it is a pipe event? */
  if( (event1&bmGPIPE_EV)!=0 ) {

    /* yes -> test all relevant bits (0-2) of the pipe_ev register */
    event2 = PEEK(PIPE_EV);

    /* test for pipe0 event */
    if (event2 & bmBit0) {

      /* clear the event */
      POKE(PIPE_EV, bmBit0);

      /* select pipe 0 */
      POKE(PIPE_SEL, 0x00);

      switch (PEEK(PIPE_CTRL) & bmTPID) {
      case TPID_SETUP:

        /* Pipe Event detected: SETUP */
        /* read Request */

        /* get the packetsize (in dwords) and convert it to bytes */
        packetSize = PEEK(PIPE_DATA_PTR) << 2;

        /* datasize must be at least standard header */
        if( packetSize==0x08 ) {
          usb_io_read_fifo(Usb_Ep0_Buffer>>2, packetSize, setupBuffer);
          usb_requests_handle_setup(setupBuffer, &tSetupPkt);
        }
        /* else ignore packet (strange size) */
        break;

      case TPID_IN:

        /* Pipe Event detected: IN */
        /* just make the data buffer again valid to enable USB Status stage */

        /* address change pending? */
        if( globalState==USB_State_AddressChangePending ) {

          /* set the new address */
          POKE(PIPE_ADDR, uiNewAddress);

          /* address set */
          globalState = USB_State_Address;
        }

        POKE(PIPE_DATA_TBYTES, Usb_Ep0_PacketSize|bmDBV);

        tOutTransactionNeeded = USB_SetupTransaction_NoOutTransaction;

        break;

      case TPID_OUT:

        /* pipe out event */

        /* data wanted? */
        if( tOutTransactionNeeded==USB_SetupTransaction_OutTransaction ) {

          /* get the packetsize in bytes */
          packetSize = Usb_Ep2_PacketSize - (PEEK(PIPE_DATA_TBYTES)&(~bmDBV));
          if( packetSize<=Usb_Ep0_PacketSize ) {
            tSetupPkt.uiDataLen = packetSize;
            usb_io_read_fifo(Usb_Ep0_Buffer>>2, packetSize, tSetupPkt.abData);

            /* correct data toggle */
            ulDataToggle  = PEEK(PIPE_STAT);
            ulDataToggle ^= (1<<2);
            POKE(PIPE_STAT, ulDataToggle);
            usb_requests_handle_request_top(&tSetupPkt);
          }
        }

        break;
      }
    }

    /* test for pipe 1 event */
    if( (event2&bmBit1)!=0 ) {

      /* clear the event */
      POKE(PIPE_EV, bmBit1);

      /* select pipe 1 */
      POKE(PIPE_SEL, 0x01);

      switch (PEEK(PIPE_CTRL) & bmTPID) {
      case TPID_SETUP:

        /* should never happen -> ignore packet */
        break;

      case TPID_IN:

        /* activate the pipe and set the direction to 'input' */
        POKE(NETX_USB_PIPE_CTRL, (1<<2)|1 );

        /* IN transaction finished, reenable the input buffer */

        /* data buffer valid, ready to receive 0x40 bytes */
        POKE(NETX_USB_PIPE_DATA_TOT, 0x80000000 | Usb_Ep1_PacketSize);

        break;

      case TPID_OUT:
         break;
      }
    }

    /* test for pipe 2 event (data from host arrived?) */
    if( (event2&bmBit2)!=0 ) {

      /* clear the event */
      POKE(PIPE_EV, bmBit2);

      /* select pipe 2 */
      POKE(PIPE_SEL, 0x02);

      switch (PEEK(PIPE_CTRL) & bmTPID) {
      case TPID_SETUP:

        /* should never happen -> ignore packet */
        break;

      case TPID_IN:
        break;

      case TPID_OUT:

        /* get the packetsize in bytes */
        packetSize = Usb_Ep2_PacketSize - (PEEK(PIPE_DATA_TBYTES)&(~bmDBV));
        if( packetSize<=Usb_Ep2_PacketSize ) {
          usb_io_read_fifo((Usb_Ep2_Buffer>>2), packetSize, receiveBuffer);

          /* fill the bytes into the buffer */
          tBufState = usb_cdc_buf_rec_put(receiveBuffer, packetSize, Usb_Ep2_PacketSize);

          switch( tBufState ) {
          case tUsbCdc_BufferState_Ok:

            /* reenable the data buffer */
            usb_activateInputPipe();
            tReceiveEpState = USB_ReceiveEndpoint_Running;
            break;
          case tUsbCdc_BufferState_Full:

            /* keep the buffer disabled */
            tReceiveEpState = USB_ReceiveEndpoint_Blocked;
            break;
          case tUsbCdc_BufferState_Rejected:

            /* should never happen, the buffer will be blocked before */
            // TODO: signal the discard with 'serial state' notification
            break;
          }
        }
        break;
      }
    }

    /* test for pipe 3 event (data to host has been sent?) */
    if( (event2&bmBit3)!=0 ) {

      /* clear the event */
      POKE(PIPE_EV, bmBit3);

      /* select pipe 3 */
      POKE(PIPE_SEL, 0x03);

      switch (PEEK(PIPE_CTRL) & bmTPID) {
      case TPID_SETUP:

        /* should never happen -> ignore the packet */
        break;

      case TPID_IN:

        /* is there more data waiting? */
        usb_sendPendingPacket();
        break;

      case TPID_OUT:
        break;
      }
    }
  }
}

/* --------------------------------------------------------------------------- */

/******************************/
/* Activate the INPUT Pipe    */
/******************************/

void usb_activateInputPipe(void)
{
  /* select pipe 2 */
  POKE(PIPE_SEL, 0x02);

  /* set data pointer to Usb_Ep2_Buffer */
  POKE(NETX_USB_PIPE_DATA_PTR, Usb_Ep2_Buffer>>2);

  /* enable pipe 2 */
  POKE(NETX_USB_PIPE_DATA_TOT, 0x80000000 | Usb_Ep2_PacketSize);
}

/* --------------------------------------------------------------------------- */


/******************************/
/* Reset the USB Port         */
/******************************/

void usb_handleReset(void)
{
  unsigned int event;


  /* get the pending port events */
  event = PEEK(PSC_EV);

  /* check for USB Reset event */
  if( (event&bmURES_EV)!=0 ) {

    /* clear the reset event */
    POKE(PSC_EV, bmURES_EV);

    /* set endpoint 0 packet size */
    POKE (PIPE_CFG, Usb_Ep0_PacketSize);

    /* go to the default state */
    globalState = USB_State_Default;

    /* reset current config (as if we had more than one) */
    currentConfig = 0;

    /* reset current interface */
    uiCurrentInterface = 0;

    tOutTransactionNeeded = USB_SetupTransaction_NoOutTransaction;
    tReceiveEpState = USB_ReceiveEndpoint_Running;
    tSendEpState = USB_SendEndpoint_Idle;

    /* configure the pipes */
    usb_configurePipes();
  }
}

/* --------------------------------------------------------------------------- */


/******************************/
/* Configuration of the  Pipes*/
/******************************/

void usb_configurePipes(void)
{
  /* setup one interrupt pipe */

  /* select pipe #1 */
  POKE(NETX_USB_PIPE_SEL, 1);

  /* set the max packet size */
  POKE(NETX_USB_PIPE_CFG, Usb_Ep1_PacketSize);

  /* set the endpoint number */
  POKE(NETX_USB_PIPE_ADDR, 1);

  /* activate the pipe and set direction to 'input' */
  POKE(NETX_USB_PIPE_CTRL, (1<<2)|1 );

  /* select pipe #2 */
  POKE(NETX_USB_PIPE_SEL, 2);

  /* set the max packet size */
  POKE(NETX_USB_PIPE_CFG, Usb_Ep2_PacketSize);

  /* set the endpoint number */
  POKE(NETX_USB_PIPE_ADDR, 2);

  /* set the data pointer to Usb_Ep2_Buffer */
  POKE(NETX_USB_PIPE_DATA_PTR, Usb_Ep2_Buffer>>2);

  /* data buffer valid, ready to receive bytes */
  POKE(NETX_USB_PIPE_DATA_TOT, 0x80000000 | Usb_Ep2_PacketSize);

  /* activate the pipe and set the direction to 'output' */
  POKE(NETX_USB_PIPE_CTRL, (1<<2)|0 );


  /* select pipe #3 */
  POKE(NETX_USB_PIPE_SEL, 3);

  /* set the max packet size */
  POKE(NETX_USB_PIPE_CFG, Usb_Ep3_PacketSize);

  /* set the endpoint number */
  POKE(NETX_USB_PIPE_ADDR, 3);

  /* activate the pipe and set the direction to 'input' */
  POKE(NETX_USB_PIPE_CTRL, (1<<2)|1 );
}

/* --------------------------------------------------------------------------- */

/*********************************/
/* Unconfiguration of the  Pipes */
/*********************************/

void usb_unconfigurePipes(void)
{
  /* select pipe #3 */
  POKE(NETX_USB_PIPE_SEL, 3);

  /* deactivate the pipe */
  POKE(NETX_USB_PIPE_CTRL, 0);

  /* data buffer invalid */
  POKE(NETX_USB_PIPE_DATA_TOT, 0);

  /* select pipe #2 */
  POKE(NETX_USB_PIPE_SEL, 2);

  /* deactivate the pipe */
  POKE(NETX_USB_PIPE_CTRL, 0);

  /* data buffer invalid */
  POKE(NETX_USB_PIPE_DATA_TOT, 0);


  /* select pipe #1 */
  POKE(NETX_USB_PIPE_SEL, 1);

  /* deactivate the pipe */
  POKE(NETX_USB_PIPE_CTRL, 0);

  /* data buffer invalid */
  POKE(NETX_USB_PIPE_DATA_TOT, 0);
}

/* --------------------------------------------------------------------------- */

