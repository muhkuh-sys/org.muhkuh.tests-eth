
#ifndef __usb_requests_top_h__
#define __usb_requests_top_h__

void usb_requests_handle_setup(unsigned char* pbBuffer, setupPacket_t *ptSetupPkt);
void usb_requests_handle_request_top(setupPacket_t *ptSetupPkt);

#endif  // __usb_requests_top_h__

