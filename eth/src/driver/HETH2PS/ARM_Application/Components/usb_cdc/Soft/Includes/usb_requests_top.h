#ifndef __USB_REQUESTS_TOP_H
#define __USB_REQUESTS_TOP_H

void usb_requests_handle_setup(unsigned char* pbBuffer, setupPacket_t *ptSetupPkt);
void usb_requests_handle_request_top(setupPacket_t *ptSetupPkt);

#endif /* __USB_REQUESTS_TOP_H */
