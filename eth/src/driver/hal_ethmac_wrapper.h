#include <stdint.h>


#ifndef __HAL_ETHMAC_WRAPPER_H__
#define __HAL_ETHMAC_WRAPPER_H__


typedef void (*PFN_HAL_HANDLE_RECEIVED_PACKET)(void *pvNetworkDriver, void *pvPacket, void *phPacket, unsigned int sizPacket);

void hal_ethmac_phy_init(unsigned int uiEnabledPorts);
void hal_ethmac_phy_deinit(unsigned int uiEnabledPorts);

void hal_ethmac_pfifo_reset(void);

int hal_ethmac_xc_reset(unsigned int uiXcPort);

int hal_ethmac_xc_init(unsigned int uiPort, const uint8_t *pucMAC);

int hal_ethmac_get_link_state(unsigned int uiPort, unsigned int *puiLinkState, unsigned int *puiSpeed, unsigned int *puiIsFullDuplex);
int hal_ethmac_get_empty_packet(unsigned int uiPort, void **ppvPacket, void **pphPacket);
int hal_ethmac_release_packet(unsigned int uiPort, void *pvPacket, void *phPacket);
int hal_ethmac_send_packet(unsigned int uiPort, void *pvPacket, void *phPacket, unsigned int uiPacketSize);
void hal_ethmac_get_received_packet(unsigned int uiPort, void *pvNetworkDriver, PFN_HAL_HANDLE_RECEIVED_PACKET pfnReceiveHandler);


#endif  /* __HAL_ETHMAC_WRAPPER_H__ */
