#include "network_interface.h"


#ifndef __HAL_MUHKUH_H__
#define __HAL_MUHKUH_H__


int hal_muhkuh_ethmac_prepare(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int hal_muhkuh_ethmac_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int hal_muhkuh_ethmac_disable(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);

int hal_muhkuh_eth2ps_prepare(NETWORK_DRIVER_T *ptNetworkDriver);
int hal_muhkuh_eth2ps_initialize(NETWORK_DRIVER_T *ptNetworkDriver0, NETWORK_DRIVER_T *ptNetworkDriver1);
int hal_muhkuh_eth2ps_disable(NETWORK_DRIVER_T *ptNetworkDriver);


#endif  /* __HAL_MUHKUH_H__ */
