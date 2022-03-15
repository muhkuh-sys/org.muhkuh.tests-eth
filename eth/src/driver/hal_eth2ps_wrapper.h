#include <stdint.h>


#ifndef __HAL_ETH2PS_WRAPPER_H__
#define __HAL_ETH2PS_WRAPPER_H__


void hal_ethps2_prepare(void);

int hal_eth2ps_phy_init(unsigned int uiNumberOfSpePorts);

void hal_eth2ps_pfifo_reset(void);

int hal_eth2ps_reset(unsigned int uiXcPort);

int hal_eth2ps_init(const uint8_t *pucMAC0, const uint8_t *pucMAC1);

int hal_eth2ps_get_link_state(unsigned int uiPort, unsigned int *puiLinkState, unsigned int *puiSpeed, unsigned int *puiIsFullDuplex);
int hal_eth2ps_get_empty_packet(unsigned int uiPort, void **ppvPacket, void **pphPacket);
int hal_eth2ps_release_packet(unsigned int uiPort, void *pvPacket, void *phPacket);
int hal_eth2ps_send_packet(unsigned int uiPort, void *pvPacket, void *phPacket, unsigned int uiPacketSize);
int hal_eth2ps_get_received_packet(unsigned int uiPort, void **ppvPacket, void **pphPacket, unsigned int *puiPacketSize);


#endif  /* __HAL_ETH2PS_WRAPPER_H__ */
