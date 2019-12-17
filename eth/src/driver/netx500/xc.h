

#include <stddef.h>


#ifndef __XC_H__
#define __XC_H__


void xc_reset(unsigned int uiPortNr);

void xc_init(unsigned int uiPortNr, const unsigned char *aucMAC, unsigned char *pucFrameBuffer, unsigned char *pucFrameBufferEnd);
void xc_init_fifo_borders(void);
void xc_init_empty_pointer_fifo(unsigned int uiPortNr, unsigned char *pucBufferStart, unsigned char *pucBufferEnd);
void xc_deinit(unsigned int uiPortNr);

void *xc_get_empty_frame(unsigned int uiPortNr);
void xc_release_frame(unsigned int uiPortNr, void *pvFrame);

void *xc_get_rx_frame(unsigned int uiPortNr, size_t *psizFrame);
void xc_send_frame(unsigned int uiPortNr, void *pvPacket, size_t sizPacket);


#endif  /* __XC_H__ */
