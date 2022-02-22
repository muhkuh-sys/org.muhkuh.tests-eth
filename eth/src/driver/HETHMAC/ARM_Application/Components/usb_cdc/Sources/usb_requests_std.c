
#include "usb_requests_common.h"
#include "usb_requests_std.h"
#include "usb_descriptors.h"
#include "usb_globals.h"
#include "usb_io.h"

/* ------------------------------------- */

/* jumptable for the 11 standard requests (and 2 reserved ones) */
static pfn_packetHandler afnStdRequests[13] = {
        setup_std_get_status,                   /* SETUP_STD_REQID_Get_Status           (0x00) */
        setup_std_clear_feature,                /* SETUP_STD_REQID_Clear_Feature        (0x01) */
        usb_requests_handle_unsupported,        /* reserved                             (0x02) */
        setup_std_set_feature,                  /* SETUP_STD_REQID_Set_Feature          (0x03) */
        usb_requests_handle_unsupported,        /* reserved                             (0x04) */
        setup_std_set_address,                  /* SETUP_STD_REQID_Set_Address          (0x05) */
        setup_std_get_descriptor,               /* SETUP_STD_REQID_Get_Descriptor       (0x06) */
        setup_std_set_descriptor,               /* SETUP_STD_REQID_Set_Descriptor       (0x07) */
        setup_std_get_configuration,            /* SETUP_STD_REQID_Get_Configuration    (0x08) */
        setup_std_set_configuration,            /* SETUP_STD_REQID_Set_Configuration    (0x09) */
        setup_std_get_interface,                /* SETUP_STD_REQID_Get_Interface        (0x0a) */
        setup_std_set_interface,                /* SETUP_STD_REQID_Set_Interface        (0x0b) */
        setup_std_synch_frame                   /* SETUP_STD_REQID_Synch_Frame          (0x0c) */
};

/* call the jumptable */
packet_handler_stall_req_t usb_requests_handle_std(setupPacket_t *pPacket)
{
        unsigned int uiReqId;


        uiReqId = pPacket->tHeader.reqId;
        if( uiReqId>=(sizeof(afnStdRequests)/sizeof(afnStdRequests[0])) ) {
                /* unknown request */
                return PKT_HANDLER_Send_Stall;
        }
        else {
                return afnStdRequests[uiReqId](pPacket);
        }
}

//-------------------------------------


packet_handler_stall_req_t setup_std_get_status(setupPacket_t *pPacket)
{
        unsigned char abStatus[2];
        unsigned int uiIdx;


        /* data source must be device for this request */
        if( pPacket->tHeader.reqDir!=SETUP_REQDIR_DeviceToHost ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* the request is only valid in addressed or configured state */
        if( globalState!=USB_State_Address && globalState==USB_State_Configured ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* in addressed state, only index 0 is allowed */
        if( globalState==USB_State_Address && pPacket->tHeader.wIndex!=0 ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* get status */
        uiIdx = pPacket->tHeader.wIndex;
        switch( pPacket->tHeader.reqRec ) {
        case SETUP_REQREC_Device:
                /* index must be 0 for the device */
                if( uiIdx!=0 ) {
                        return PKT_HANDLER_Send_Stall;
                }

                /* device status is fixed */
                abStatus[0] = 1;     /* self-powered and no remote wakeup */
                abStatus[1] = 0;
                break;

        case SETUP_REQREC_Interface:
                /* the device has 2 interfaces: 0 and 1 */
                if( uiIdx>1 ) {
                        return PKT_HANDLER_Send_Stall;
                }

                /* all interface status bits are reserved */
                abStatus[0] = 0;
                abStatus[1] = 0;
                break;

        case SETUP_REQREC_Endpoint:
                /* endpoint */
                abStatus[1] = 0;

                if( uiIdx==0x00 ) {
                        abStatus[0] = ((endpoint0State==USB_EndpointState_Halted) ? 1 : 0);
                }
                else if( uiIdx==0x81 ) {
                        abStatus[0] = ((endpoint1State==USB_EndpointState_Halted) ? 1 : 0);
                }
                else if( uiIdx==0x02 ) {
                        abStatus[0] = ((endpoint2State==USB_EndpointState_Halted) ? 1 : 0);
                }
                else if( uiIdx==0x83 ) {
                        abStatus[0] = ((endpoint3State==USB_EndpointState_Halted) ? 1 : 0);
                } else {
                        return PKT_HANDLER_Send_Stall;
                }
                break;

        default:
                return PKT_HANDLER_Send_Stall;
        }

        sendDescriptor(pPacket->tHeader.wLength, 2, abStatus);
        return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_std_clear_feature(setupPacket_t *pPacket)
{
        unsigned int uiIdx;


        /* the request is only valid in addressed or configured state */
        if( globalState!=USB_State_Address && globalState==USB_State_Configured ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* in addressed state, only index 0 is allowed */
        if( globalState==USB_State_Address && pPacket->tHeader.wIndex!=0 ) {
                return PKT_HANDLER_Send_Stall;
        }

        uiIdx = pPacket->tHeader.wIndex;
        switch( pPacket->tHeader.reqRec ) {
        case SETUP_REQREC_Device:
                /* no features supported */
                return PKT_HANDLER_Send_Stall;
        case SETUP_REQREC_Interface:
                /* no features supported */
                return PKT_HANDLER_Send_Stall;
        case SETUP_REQREC_Endpoint:
                /* only feature 'ENDPOINT_HALT' supported */
                if( pPacket->tHeader.wValue!=0 ) {
                        return PKT_HANDLER_Send_Stall;
                }

                if( uiIdx==0x00 ) {
                        /* remove halt from ep0 */
                        endpoint0State = USB_EndpointState_Running;
                }
                else if( uiIdx==0x81 ) {
                        /* remove halt from ep1 */
                        endpoint1State = USB_EndpointState_Running;
                }
                else if( uiIdx==0x02 ) {
                        /* remove halt from ep2 */
                        endpoint2State = USB_EndpointState_Running;
                }
                else if( uiIdx==0x83 ) {
                        /* remove halt from ep3 */
                        endpoint3State = USB_EndpointState_Running;
                } else {
                        return PKT_HANDLER_Send_Stall;
                }
                break;

        default:
                return PKT_HANDLER_Send_Stall;
        }

        usb_io_sendZeroPacket();
        return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_std_set_feature(setupPacket_t *pPacket)
{
        unsigned int uiIdx;


        /* the request is only valid in addressed or configured state */
        if( globalState!=USB_State_Address && globalState==USB_State_Configured ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* in addressed state, only index 0 is allowed */
        if( globalState==USB_State_Address && pPacket->tHeader.wIndex!=0 ) {
                return PKT_HANDLER_Send_Stall;
        }

        uiIdx = pPacket->tHeader.wIndex;
        switch( pPacket->tHeader.reqRec ) {
        case SETUP_REQREC_Device:
                /* no features supported */
                return PKT_HANDLER_Send_Stall;
        case SETUP_REQREC_Interface:
                /* no features supported */
                return PKT_HANDLER_Send_Stall;
        case SETUP_REQREC_Endpoint:
                /* only feature 'ENDPOINT_HALT' supported */
                if( pPacket->tHeader.wValue!=0 ) {
                        return PKT_HANDLER_Send_Stall;
                }

                if( uiIdx==0x00 ) {
                        /* set halt on ep0 */
                        endpoint0State = USB_EndpointState_Halted;
                }
                else if( uiIdx==0x81 ) {
                        /* set halt on ep1 */
                        endpoint1State = USB_EndpointState_Halted;
                }
                else if( uiIdx==0x02 ) {
                        /* set halt on ep2 */
                        endpoint2State = USB_EndpointState_Halted;
                }
                else if( uiIdx==0x83 ) {
                        /* set halt on ep3 */
                        endpoint3State = USB_EndpointState_Halted;
                } else {
                        return PKT_HANDLER_Send_Stall;
                }
                break;

        default:
                return PKT_HANDLER_Send_Stall;
        }

        usb_io_sendZeroPacket();
        return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_std_set_address(setupPacket_t *pPacket)
{
        /* this request is only allowed for the device */
        if( pPacket->tHeader.reqRec!=SETUP_REQREC_Device ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* only allowed in default and addressed state */
        if( globalState!=USB_State_Default && globalState!=USB_State_Address ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* SetAddress Request, send zero byte */
        usb_io_sendZeroPacket();

        /* remember new address */
        uiNewAddress = pPacket->tHeader.wValue << 4;

        /* now we're in address state */
        globalState = USB_State_AddressChangePending;

        return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_std_get_descriptor(setupPacket_t *pPacket)
{
        /* data source must be device for this request */
        if( pPacket->tHeader.reqDir!=SETUP_REQDIR_DeviceToHost ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* this request is only valid for the device */
        if( pPacket->tHeader.reqRec!=SETUP_REQREC_Device ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* this request is only valid in Default, Address and Configured state */
        if( globalState!=USB_State_Default && globalState!=USB_State_Address && globalState!=USB_State_Configured ) {
                return PKT_HANDLER_Send_Stall;
        }

        return getDescriptor(pPacket);
}


packet_handler_stall_req_t setup_std_set_descriptor(setupPacket_t *pPacket)
{
        /* descriptor changes are not supported */
        return PKT_HANDLER_Send_Stall;
}


packet_handler_stall_req_t setup_std_get_configuration(setupPacket_t *pPacket)
{
        unsigned char bConfigIdx;


        /* data source must be device for this request */
        if( pPacket->tHeader.reqDir!=SETUP_REQDIR_DeviceToHost ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* this request is only valid for the device */
        if( pPacket->tHeader.reqRec!=SETUP_REQREC_Device ) {
                return PKT_HANDLER_Send_Stall;
        }


        if( globalState==USB_State_Address ) {
                bConfigIdx = 0;
        }
        else if( globalState==USB_State_Configured ) {
                bConfigIdx = currentConfig;
        }
        else {
                /* not supported in other states */
                return PKT_HANDLER_Send_Stall;
        }

        sendDescriptor(pPacket->tHeader.wLength, 1, &bConfigIdx);
        return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_std_set_configuration(setupPacket_t *pPacket)
{
        unsigned int uiConfigIdx;


        /* this request is only valid for the device */
        if( pPacket->tHeader.reqRec!=SETUP_REQREC_Device ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* this request is only valid in address and configured state */
        if( globalState!=USB_State_Address && globalState!=USB_State_Configured ) {
                return PKT_HANDLER_Send_Stall;
        }

        uiConfigIdx = (pPacket->tHeader.wValue & 0xff);

        if( uiConfigIdx==0 ) {
                /* set config to 0 */
                currentConfig = 0;

                /* go back to address state */
                globalState = USB_State_Address;

        }
        else if( uiConfigIdx==1 ) {
                /* select config #1 */
                currentConfig = 1;

                /* go to configured state */
                globalState = USB_State_Configured;

        }
        else {
                /* configuration is not supported => send STALL */
                return PKT_HANDLER_Send_Stall;
        }

        usb_io_sendZeroPacket();
        return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_std_get_interface(setupPacket_t *pPacket)
{
        unsigned int uiIdx;
        unsigned char bInterface;


        /* data source must be device for this request */
        if( pPacket->tHeader.reqDir!=SETUP_REQDIR_DeviceToHost ) {
                return PKT_HANDLER_Send_Stall;
        }

        if( pPacket->tHeader.reqRec!=SETUP_REQREC_Interface ) {
                return PKT_HANDLER_Send_Stall;
        }

        if( globalState!=USB_State_Configured ) {
                return PKT_HANDLER_Send_Stall;
        }

        uiIdx = pPacket->tHeader.wIndex;

        if( uiIdx==0 ) {
                /* send interface descriptor #0 */
                bInterface = 0;
        }
        else if( uiIdx==1 ) {
                /* send interface descriptor #1 */
                bInterface = 1;
        }
        else {
                /* interface not supported => send STALL */
                return PKT_HANDLER_Send_Stall;
        }

        sendDescriptor(pPacket->tHeader.wLength, 1, &bInterface);                                
        return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_std_set_interface(setupPacket_t *pPacket)
{
        unsigned int uiIdx;


        if( pPacket->tHeader.reqRec!=SETUP_REQREC_Interface ) {
                return PKT_HANDLER_Send_Stall;
        }

        if( globalState!=USB_State_Configured ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* set interface */
        uiIdx = pPacket->tHeader.wIndex;

        if( uiIdx!=0 && uiIdx!=1 ) {
                return PKT_HANDLER_Send_Stall;
        }

        /* ok */
        usb_io_sendZeroPacket();
        return PKT_HANDLER_Do_Not_Send_Stall;
}


packet_handler_stall_req_t setup_std_synch_frame(setupPacket_t *pPacket)
{
        return PKT_HANDLER_Send_Stall;
}


