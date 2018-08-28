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
#include "driver/drv_eth_xc.h"
#include "stack/udp.h"


#ifndef __BOOT_DRV_ETH_H__
#define __BOOT_DRV_ETH_H__



typedef struct PHY_MACRO_STRUCT
{
	unsigned char  aucMacro[512];
} PHY_MACRO_T;



typedef struct ETHERNET_CONFIGURATION_STRUCT
{
	ETHERNET_PORT_CONFIGURATION_T atPorts[MAX_NETWORK_INTERFACES];

	unsigned short usArpTimeout;
	unsigned short usDhcpTimeout;
	unsigned char ucArpRetries;
	unsigned char ucDhcpRetries;

	PHY_MACRO_T tPhyMacroIntern;
	PHY_MACRO_T tPhyMacroExtern;
} ETHERNET_CONFIGURATION_T;



int boot_drv_eth_init(unsigned int uiInterfaceIndex, ETHERNET_PORT_CONFIGURATION_T *ptEthCfg, NETWORK_DRIVER_T *ptNetworkDriver);
int ethernet_startup_process(NETWORK_DRIVER_T *ptNetworkDriver);
int ethernet_test_process(NETWORK_DRIVER_T *ptNetworkDriver);


#endif  /* __BOOT_DRV_ETH_H__ */
