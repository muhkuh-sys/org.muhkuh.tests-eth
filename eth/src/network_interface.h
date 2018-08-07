#include "systime.h"

#ifndef __NETWORK_INTERFACE_H__
#define __NETWORK_INTERFACE_H__


struct NETWORK_DRIVER_STRUCT;

typedef unsigned int (*PFN_NETWORK_FN_GET_LINK_STATUS)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver);
typedef void *(*PFN_NETWORK_FN_GET_EMPTY_PACKET)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver);
typedef void (*PFN_NETWORK_FN_RELEASE_PACKET)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, void *pvPacket);
typedef void (*PFN_NETWORK_FN_SEND_PACKET)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, void *pvPacket, unsigned int sizPacket);
typedef void *(*PFN_NETWORK_FN_GET_RECEIVED_PACKET)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, unsigned int *psizPacket);
typedef void (*PFN_NETWORK_FN_DEACTIVATE)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver);
#if CFG_DEBUGMSG==1
typedef void (*PFN_NETWORK_FN_STATISTICS)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver);
#endif


typedef struct STRUCT_NETWORK_IF
{
	PFN_NETWORK_FN_GET_LINK_STATUS pfnGetLinkStatus;
	PFN_NETWORK_FN_GET_EMPTY_PACKET pfnGetEmptyPacket;
	PFN_NETWORK_FN_RELEASE_PACKET pfnReleasePacket;
	PFN_NETWORK_FN_SEND_PACKET pfnSendPacket;
	PFN_NETWORK_FN_GET_RECEIVED_PACKET pfnGetReceivedPacket;
	PFN_NETWORK_FN_DEACTIVATE pfnDeactivate;
#if CFG_DEBUGMSG==1
	PFN_NETWORK_FN_STATISTICS pfnStatistics;
#endif
} NETWORK_IF_T;



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



typedef struct DRV_ETH_XC_HANDLE_STRUCT
{
	unsigned int uiEthPortNr;       /* The Ethernet port 0-3. */
	unsigned int uiXC;              /* The XC instance 0-1. */
	unsigned int auiExtPhyCtrlInst[2];
	unsigned int auiExtPhyAddress[2];
} DRV_ETH_XC_HANDLE_T;



typedef void (*PFN_UDP_RECEIVE_HANDLER)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, void *pvData, unsigned int sizLength, void *pvUser);

typedef struct
{
	unsigned int uiLocalPort;
	unsigned long ulRemoteIp;
	unsigned int uiRemotePort;
	PFN_UDP_RECEIVE_HANDLER pfn_recHandler;
	void *pvUser;
} UDP_ASSOCIATION_T;



typedef struct NETWORK_DRIVER_DATA_STRUCT
{
	DRV_ETH_XC_HANDLE_T tDrvEthXcHandle;
} NETWORK_DRIVER_DATA_T;



typedef struct NETWORK_DRIVER_STRUCT
{
	int f_is_configured;
	const char *pcName;
	NETWORK_IF_T tNetworkIf;
	NETWORK_STATE_T tState;
	TIMER_HANDLE_T tLinkUpTimer;
	TIMER_HANDLE_T tEthernetHandlerTimer;
	UDP_ASSOCIATION_T *ptEchoUdpAssoc;
	NETWORK_DRIVER_DATA_T tNetworkDriverData;
} NETWORK_DRIVER_T;


#endif  /* __NETWORK_INTERFACE_H__ */
