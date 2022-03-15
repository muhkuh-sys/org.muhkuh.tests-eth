#include <stdint.h>


#ifndef __HAL_ETHMAC_WRAPPER_H__
#define __HAL_ETHMAC_WRAPPER_H__


void hal_ethmac_phy_init(unsigned int uiEnabledPorts);

void hal_ethmac_pfifo_reset(void);

int hal_ethmac_xc_reset(unsigned int uiXcPort);

int hal_ethmac_xc_init(unsigned int uiPort, const uint8_t *pucMAC);

int hal_ethmac_get_link_state(unsigned int uiPort, unsigned int *puiLinkState, unsigned int *puiSpeed, unsigned int *puiIsFullDuplex);
int hal_ethmac_get_empty_packet(unsigned int uiPort, void **ppvPacket, void **pphPacket);
int hal_ethmac_release_packet(unsigned int uiPort, void *pvPacket __attribute__ ((unused)), void *phPacket);
int hal_ethmac_send_packet(unsigned int uiPort, void *pvPacket, void *phPacket, unsigned int uiPacketSize);
int hal_ethmac_get_received_packet(unsigned int uiPort, void **ppvPacket, void **pphPacket, unsigned int *puiPacketSize);


#endif  /* __HAL_ETHMAC_WRAPPER_H__ */
