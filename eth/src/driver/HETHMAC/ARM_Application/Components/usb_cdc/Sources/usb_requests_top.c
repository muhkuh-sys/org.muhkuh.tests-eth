
#include "usb_regmap.h"
#include "usb_io.h"
#include "usb_requests_common.h"
#include "usb_requests_top.h"
#include "usb_requests_std.h"
#include "usb_requests_cdc.h"
#include "usb_globals.h"



void usb_requests_handle_setup(unsigned char* pbBuffer, setupPacket_t *ptSetupPkt)
{
        /* convert the setup packet into the structure for faster access */
        ptSetupPkt->tHeader.reqDir         = (setup_requestDir_t)((pbBuffer[0]&0x80)>>7);
        ptSetupPkt->tHeader.reqType        = (setup_requestType_t)((pbBuffer[0]&0x60)>>5);
        ptSetupPkt->tHeader.reqRec         = (setup_RequestRec_t)(pbBuffer[0]&0x1f);
        ptSetupPkt->tHeader.reqId          = pbBuffer[1];
        ptSetupPkt->tHeader.wValue         = pbBuffer[2] | (pbBuffer[3]<<8);
        ptSetupPkt->tHeader.wIndex         = pbBuffer[4] | (pbBuffer[5]<<8);
        ptSetupPkt->tHeader.wLength        = pbBuffer[6] | (pbBuffer[7]<<8);

        /* does the request have an out transaction? */
        if( ptSetupPkt->tHeader.reqDir==SETUP_REQDIR_HostToDevice && ptSetupPkt->tHeader.wLength!=0 ) {
                POKE(NETX_USB_PIPE_CTRL, (1<<2)|0 );
                POKE(NETX_USB_PIPE_DATA_PTR, Usb_Ep0_Buffer>>2);
                POKE(PIPE_DATA_TBYTES, Usb_Ep0_PacketSize|bmDBV);	
                tOutTransactionNeeded = USB_SetupTransaction_OutTransaction;
        } else {
                tOutTransactionNeeded = USB_SetupTransaction_NoOutTransaction;
                usb_requests_handle_request_top(ptSetupPkt);
        }
}


int iDebugCatch = 0;
volatile int iFreezeDummy;


static pfn_packetHandler afnRequests[4] = {
        usb_requests_handle_std,                /* standard requests */
        usb_requests_handle_cdc,                /* class specific requests */
        usb_requests_handle_unsupported,        /* vendor specific requests */
        usb_requests_handle_unsupported         /* reserved */
};


void usb_requests_handle_request_top(setupPacket_t *ptSetupPkt)
{
        packet_handler_stall_req_t tSendStall;

        /* distinguish the type */
        tSendStall = afnRequests[ptSetupPkt->tHeader.reqType](ptSetupPkt);

        /* send stall on request */
        if( tSendStall==PKT_HANDLER_Send_Stall ) {
                usb_io_sendStall();
        }
}


