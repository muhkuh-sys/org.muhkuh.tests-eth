#include "network_interface.h"
#include <stddef.h>


#ifndef __INTERFACE_H__
#define __INTERFACE_H__

void pfifo_reset(void);

int drv_eth_xc_prepare(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int drv_eth_xc_disable(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int drv_eth_xc_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);

int drv_eth_xc_prepare_lvds(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int drv_eth_xc_disable_lvds(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int drv_eth_xc_initialize_lvds(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);

#endif  /* __INTERFACE_H__ */
