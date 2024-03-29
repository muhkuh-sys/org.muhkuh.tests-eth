/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "network_interface.h"
#include "systime.h"
#include "stack/udp.h"


#ifndef __BOOT_DRV_ETH_H__
#define __BOOT_DRV_ETH_H__



typedef struct ETHERNET_CONFIGURATION_STRUCT
{
	ETHERNET_PORT_CONFIGURATION_T atPorts[MAX_NETWORK_INTERFACES];

	unsigned short usArpTimeout;
	unsigned short usDhcpTimeout;
	unsigned char ucArpRetries;
	unsigned char ucDhcpRetries;
} ETHERNET_CONFIGURATION_T;



typedef enum ETHERNET_TEST_RESULT_ENUM
{
	ETHERNET_TEST_RESULT_InProgress = 0,
	ETHERNET_TEST_RESULT_FinishedOk = 1,
	ETHERNET_TEST_RESULT_Error = 2
} ETHERNET_TEST_RESULT_T;



int ethernet_init(ETHERNET_PORT_CONFIGURATION_T *atEthCfg, NETWORK_DRIVER_T *atNetworkDriver);
int ethernet_deinit(ETHERNET_PORT_CONFIGURATION_T *atEthCfg, NETWORK_DRIVER_T *atNetworkDriver);
int ethernet_startup_process(NETWORK_DRIVER_T *ptNetworkDriver);
ETHERNET_TEST_RESULT_T ethernet_test_process(NETWORK_DRIVER_T *ptNetworkDriver);


#endif  /* __BOOT_DRV_ETH_H__ */
