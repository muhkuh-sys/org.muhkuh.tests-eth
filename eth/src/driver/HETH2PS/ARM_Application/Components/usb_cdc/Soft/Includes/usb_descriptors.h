#ifndef __USB_DESCRIPTORS_H
#define __USB_DESCRIPTORS_H

/* --------------------------------------------------------------------------- */

/* NOTE: this struct is gcc specific, it uses the empty array */
typedef struct
{
        unsigned char length;
        unsigned char descriptor_type;
        char text[];
} stringDescriptor_t;

/* --------------------------------------------------------------------------- */

void sendDescriptor(unsigned int reqiredLength, unsigned int descriptorLength, const unsigned char *descriptor);
packet_handler_stall_req_t getDescriptor(setupPacket_t *pPacket);
void sendStringDescriptor(unsigned int languageId, unsigned int stringIdx, unsigned int wLength);

/* --------------------------------------------------------------------------- */


#endif  /* __USB_DESCRIPTORS_H */
