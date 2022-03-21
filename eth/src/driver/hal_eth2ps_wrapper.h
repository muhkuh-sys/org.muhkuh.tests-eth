#include <stdint.h>


#ifndef __HAL_ETH2PS_WRAPPER_H__
#define __HAL_ETH2PS_WRAPPER_H__


typedef void (*PFN_HAL_HANDLE_RECEIVED_PACKET)(void *pvNetworkDriver, void *pvPacket, void *phPacket, unsigned int sizPacket);


typedef enum HAL_ETH2PS_RESULT_ENUM
{
	HAL_ETH2PS_RESULT_Ok = 0,
	HAL_ETH2PS_RESULT_0SPEPortsAreNotSupportedYet = 1,
	HAL_ETH2PS_RESULT_2SPEPortsAreNotSupportedYet = 2,
	HAL_ETH2PS_RESULT_UnknownSetup = 3,
	HAL_ETH2PS_RESULT_FailedToAddRTEPhy0 = 4,
	HAL_ETH2PS_RESULT_FailedToAddRTEPhy1 = 5,
	HAL_ETH2PS_RESULT_FailedToAddSPEPhy0 = 6,
	HAL_ETH2PS_RESULT_FailedToAddSPEPhy1 = 7,
	HAL_ETH2PS_RESULT_FailedToInitializeSPEPhy0 = 8,
	HAL_ETH2PS_RESULT_FailedToInitializeSPEPhy1 = 9,
	HAL_ETH2PS_RESULT_FailedToRegisterExtLinkApiSPEPhy0 = 10,
	HAL_ETH2PS_RESULT_FailedToRegisterExtLinkApiSPEPhy1 = 11,
	HAL_ETH2PS_RESULT_FailedToReleaseSPEPhy0 = 12,
	HAL_ETH2PS_RESULT_FailedToReleaseSPEPhy1 = 13,
} HAL_ETH2PS_RESULT_T;


void hal_ethps2_prepare(void);

HAL_ETH2PS_RESULT_T hal_eth2ps_phy_init(unsigned int uiNumberOfSpePorts);
HAL_ETH2PS_RESULT_T hal_eth2ps_phy_deinit(unsigned int uiNumberOfSpePorts);

void hal_eth2ps_pfifo_reset(void);

int hal_eth2ps_reset(unsigned int uiXcPort);

int hal_eth2ps_init(uint8_t *pucMAC0, uint8_t *pucMAC1);
int hal_eth2ps_deinit(void);

int hal_eth2ps_get_link_state(unsigned int uiPort, unsigned int *puiLinkState, unsigned int *puiSpeed, unsigned int *puiIsFullDuplex);
int hal_eth2ps_get_empty_packet(unsigned int uiPort, void **ppvPacket, void **pphPacket);
int hal_eth2ps_release_packet(unsigned int uiPort, void *pvPacket, void *phPacket);
int hal_eth2ps_send_packet(unsigned int uiPort, void *pvPacket, void *phPacket, unsigned int uiPacketSize);
void hal_eth2ps_get_received_packet(void *pvNetworkDriver0, PFN_HAL_HANDLE_RECEIVED_PACKET pfnPort0, void *pvNetworkDriver1, PFN_HAL_HANDLE_RECEIVED_PACKET pfnPort1);
int hal_eth2ps_get_statistics(unsigned int uiPort, void *pvBuffer, unsigned int sizBuffer);


#endif  /* __HAL_ETH2PS_WRAPPER_H__ */
