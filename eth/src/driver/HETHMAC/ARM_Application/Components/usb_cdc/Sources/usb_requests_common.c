
#include "usb_requests_common.h"

packet_handler_stall_req_t usb_requests_handle_unsupported(setupPacket_t *pPacket)
{
        return PKT_HANDLER_Send_Stall;
}
