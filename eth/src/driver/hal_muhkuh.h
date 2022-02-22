#include "network_interface.h"


#ifndef __HAL_MUHKUH_H__
#define __HAL_MUHKUH_H__


void setup_phy_internal(void);
void setup_phy_external(void);

int hal_xc_prepare(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int hal_xc_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int hal_xc_disable(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);

void hal_phy_init(void);
void hal_pfifo_reset(void);


#endif  /* __HAL_MUHKUH_H__ */
