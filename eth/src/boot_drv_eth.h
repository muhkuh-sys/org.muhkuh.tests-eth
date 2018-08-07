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



typedef struct PHY_MACRO_STRUCT
{
	unsigned char  aucMacro[512];
} PHY_MACRO_T;



typedef struct STRUCT_ETHERNET_CONFIGURATION
{
	unsigned char aucMac[6];
	unsigned long ulIp;
	unsigned long ulGatewayIp;
	unsigned long ulNetmask;
	unsigned long ulPhyControl;
	unsigned short usLinkUpDelay;
	unsigned short usArpTimeout;
	unsigned short usDhcpTimeout;
	unsigned char ucArpRetries;
	unsigned char ucDhcpRetries;
	unsigned char aucMmioCfg[2];
	unsigned char aucPadCtrlExtCommon[4];
	unsigned char aucPadCtrlExt0[16];
	unsigned char aucPadCtrlExt1[16];
	PHY_MACRO_T tPhyMacroIntern;
	PHY_MACRO_T tPhyMacroExtern;
} ETHERNET_CONFIGURATION_T;



typedef enum INTERFACE_ENUM
{
	INTERFACE_INTPHY0  = 0,
	INTERFACE_INTPHY1  = 1,
	INTERFACE_EXTPHY0  = 2,
	INTERFACE_EXTPHY1  = 3,
	INTERFACE_LVDS0    = 4,
	INTERFACE_LVDS1    = 5
} INTERFACE_T;



typedef enum NETWORK_STATE_ENUM
{
	NETWORK_STATE_NoLink             = 0,
	NETWORK_STATE_LinkUp_Delay       = 1,
	NETWORK_STATE_LinkUp_Ready       = 2,
	NETWORK_STATE_Dhcp               = 3,
	NETWORK_STATE_Ready              = 4,
	NETWORK_STATE_Error              = 5
} NETWORK_STATE_T;



typedef struct NETWORK_DRIVER_STRUCT
{
	int f_is_configured;
	const char *pcName;
	const NETWORK_IF_T *ptNetworkIf;
	void *pvUser;
	NETWORK_STATE_T tState;
	TIMER_HANDLE_T tLinkUpTimer;
	TIMER_HANDLE_T tEthernetHandlerTimer;
	UDP_ASSOCIATION_T *ptEchoUdpAssoc;
} NETWORK_DRIVER_T;



int boot_drv_eth_init(NETWORK_DRIVER_T *ptNetworkDriver, INTERFACE_T tInterface, const char *pcName);
void ethernet_cyclic_process(NETWORK_DRIVER_T *ptNetworkDriver);


#endif  /* __BOOT_DRV_ETH_H__ */
