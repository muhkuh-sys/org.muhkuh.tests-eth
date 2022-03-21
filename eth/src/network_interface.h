#include "systime.h"

#ifndef __NETWORK_INTERFACE_H__
#define __NETWORK_INTERFACE_H__


#define MAX_NETWORK_INTERFACES 2


typedef enum LINK_STATE_ENUM
{
	LINK_STATE_Unknown = 0,
	LINK_STATE_Down = 1,
	LINK_STATE_Up = 2
} LINK_STATE_T;


struct NETWORK_DRIVER_STRUCT;

typedef int (*PFN_NETWORK_FN_GET_LINK_STATUS)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, LINK_STATE_T *ptLinkState);
typedef int (*PFN_NETWORK_FN_GET_EMPTY_PACKET)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, void **ppvPacket, void **pphPacket);
typedef void (*PFN_NETWORK_FN_RELEASE_PACKET)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, void *pvPacket, void *phPacket);
typedef void (*PFN_NETWORK_FN_SEND_PACKET)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, void *pvPacket, void *phPacket, unsigned int sizPacket);
typedef void (*PFN_NETWORK_FN_PROCESS_RECEIVED_PACKETS)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, struct NETWORK_DRIVER_STRUCT *ptAllNetworkDrivers, unsigned int sizAllNetworkDrivers);
typedef void (*PFN_NETWORK_FN_DEACTIVATE)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver);
typedef int (*PFN_NETWORK_FN_SHOW_STATISTICS)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver);
#if CFG_DEBUGMSG==1
typedef void (*PFN_NETWORK_FN_STATISTICS)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver);
#endif


typedef struct STRUCT_NETWORK_IF
{
	PFN_NETWORK_FN_GET_LINK_STATUS pfnGetLinkStatus;
	PFN_NETWORK_FN_GET_EMPTY_PACKET pfnGetEmptyPacket;
	PFN_NETWORK_FN_RELEASE_PACKET pfnReleasePacket;
	PFN_NETWORK_FN_SEND_PACKET pfnSendPacket;
	PFN_NETWORK_FN_PROCESS_RECEIVED_PACKETS pfnProcessReceivedPackets;
	PFN_NETWORK_FN_DEACTIVATE pfnDeactivate;
	PFN_NETWORK_FN_SHOW_STATISTICS pfnShowStatistics;
#if CFG_DEBUGMSG==1
	PFN_NETWORK_FN_STATISTICS pfnStatistics;
#endif
} NETWORK_IF_T;



typedef enum INTERFACE_ENUM
{
	INTERFACE_None     = 0,
	INTERFACE_ETHMAC_INTPHY0  = 1,
	INTERFACE_ETHMAC_INTPHY1  = 2,
	INTERFACE_ETHMAC_EXTPHY0  = 3,
	INTERFACE_ETHMAC_EXTPHY1  = 4,
	INTERFACE_ETH2PS_INTPHY0  = 5,
	INTERFACE_ETH2PS_INTPHY1  = 6,
	INTERFACE_ETH2PS_EXTSPE0  = 7
} INTERFACE_T;



typedef enum INTERFACE_FUNCTION_ENUM
{
	INTERFACE_FUNCTION_None = 0,          /* No special function. ARP and ICMP is active. */
	INTERFACE_FUNCTION_EchoServer = 1,    /* Echo all UDP data on the configured port. */
	INTERFACE_FUNCTION_EchoClient = 2     /* Send UDP packets and expect an echo. */
} INTERFACE_FUNCTION_T;



typedef enum NETWORK_STATE_ENUM
{
	NETWORK_STATE_NoLink             = 0,
	NETWORK_STATE_LinkUp_Delay       = 1,
	NETWORK_STATE_LinkUp_Ready       = 2,
	NETWORK_STATE_Dhcp               = 3,
	NETWORK_STATE_Ready              = 4,
	NETWORK_STATE_Ok                 = 5,
	NETWORK_STATE_Error              = 6
} NETWORK_STATE_T;



typedef enum ECHO_CLIENT_STATE_ENUM
{
	ECHO_CLIENT_STATE_Idle               = 0,
	ECHO_CLIENT_STATE_WaitingForResponse = 1,
	ECHO_CLIENT_STATE_PacketReceived     = 2,
	ECHO_CLIENT_STATE_Ok                 = 3,
	ECHO_CLIENT_STATE_Error              = 4
} ECHO_CLIENT_STATE_T;


#if 0
typedef struct DRV_ETH_XC_HANDLE_STRUCT
{
	unsigned int uiEthPortNr;       /* The Ethernet port 0-3. */
	unsigned int uiXcUnit;
	unsigned int uiXcPort;          /* The XC instance 0-1. */
	unsigned int auiExtPhyCtrlInst[2];
	unsigned int auiExtPhyAddress[2];
} DRV_ETH_XC_HANDLE_T;
#endif


typedef struct MAC_ADR_STRUCT
{
	unsigned char aucMac[6];
} __attribute__((packed)) MAC_ADR_T;



typedef void (*PFN_UDP_RECEIVE_HANDLER)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, void *pvData, unsigned int sizLength, void *pvUser);

typedef struct
{
	unsigned int uiLocalPort;
	unsigned long ulRemoteIp;
	unsigned int uiRemotePort;
	PFN_UDP_RECEIVE_HANDLER pfn_recHandler;
	void *pvUser;
} UDP_ASSOCIATION_T;



/* Maximum number of entries in the ARP cache. */
#define ARP_CACHE_MAX_ENTRIES 8

/* Maximum age value of one ARP entry, older entries will *not* be deleted,
 * it just limits the age value to this value.
 */
#define ARP_CACHE_MAX_AGE 0xff

/* Maximum number of waiting packets in the ARP queue. */
#define ARP_PACKET_QUEUE_MAX_ENTRIES 8


typedef enum
{
	ARPSTATE_Unused		= 0,    /* The entry is unused. */
	ARPSTATE_Requesting	= 1,    /* The request for this entry is send out, but no reply yet. */
	ARPSTATE_Valid		= 2     /* The entry is valid. */
} ARP_STATE_T;


typedef struct
{
	ARP_STATE_T tState;                     /* state of this entry */
	int iAge;                               /* age of the entry (i.e. last used time) */
	unsigned long ulIp;                     /* IP address */
	MAC_ADR_T tMac;                         /* MAC address */
	unsigned long ulRequestTimeStamp;       /* system time stamp when the last request was send, only valid for 'requesting' entries */
	unsigned int uiRetryCnt;                /* counts how many retries are left, only valid for 'requesting' entries */
} ARP_CACHE_ENTRY_T;


typedef struct
{
	unsigned int sizPacket;                 /* this is the size of the packet in bytes and the "valid" flag, a size of 0 means "invalid" */
	unsigned long ulIp;                     /* the destination IP for this packet */
	void *ptPkt;                            /* pointer to the packet */
	void *phPkt;                            /* Packet handle. */
} PACKET_QUEUE_ENTRY_T;



typedef enum
{
	DHCP_STATE_Idle			= 0,
	DHCP_STATE_Discover		= 1,
	DHCP_STATE_Request		= 2,
	DHCP_STATE_Error		= 3,
	DHCP_STATE_Ok			= 4
} DHCP_STATE_T;



typedef struct DHCP_HANDLE_DATA_STRUCT
{
	DHCP_STATE_T tState;
	UDP_ASSOCIATION_T *ptAssoc;
	unsigned long ulLastGoodPacket;
	unsigned int uiRetryCnt;
	unsigned long ulXId;
	unsigned long ulRequestIp;
	unsigned long aucServerIdentifier[4];
} DHCP_HANDLE_DATA_T;



typedef struct IPV4_HANDLE_DATA_STRUCT
{
	unsigned int uiIpOutputId;
} IPV4_HANDLE_DATA_T;



#define UDP_PORTLIST_MAX 8

typedef struct UDP_HANDLE_DATA_STRUCT
{
	UDP_ASSOCIATION_T atUdpPortAssoc[UDP_PORTLIST_MAX];
} UDP_HANDLE_DATA_T;



typedef struct NETWORK_DRIVER_DATA_STRUCT
{
//	DRV_ETH_XC_HANDLE_T tDrvEthXcHandle;
	PACKET_QUEUE_ENTRY_T atPacketQueue[ARP_PACKET_QUEUE_MAX_ENTRIES];
	ARP_CACHE_ENTRY_T auiArpCache[ARP_CACHE_MAX_ENTRIES];
	DHCP_HANDLE_DATA_T tDhcpData;
	IPV4_HANDLE_DATA_T tIpv4Data;
	UDP_HANDLE_DATA_T tUdpData;
} NETWORK_DRIVER_DATA_T;



#define ETHERNET_PORT_FLAG_Permanent 0x00000001
#define ETHERNET_PORT_FLAG_LinkDownAllowed 0x00000002


typedef struct ETHERNET_PORT_CONFIGURATION_STRUCT
{
	char acName[16];
	unsigned long ulInterface;
	unsigned long ulFunction;
	unsigned long ulFlags;
	unsigned long ulIp;
	unsigned long ulGatewayIp;
	unsigned long ulNetmask;
	unsigned long ulRemoteIp;
	unsigned long ulNumberOfTestPackets;
	unsigned short usLinkUpDelay;
	unsigned short usLocalPort;
	unsigned short usRemotePort;
	unsigned char aucMac[6];
} ETHERNET_PORT_CONFIGURATION_T;



typedef struct FUNCTION_ECHO_SERVER_HANDLE_STRUCT
{
	UDP_ASSOCIATION_T *ptUdpAssoc;
} FUNCTION_ECHO_SERVER_HANDLE_T;



typedef struct FUNCTION_ECHO_CLIENT_HANDLE_STRUCT
{
	ECHO_CLIENT_STATE_T tState;
	unsigned int uiPacketsLeft;
	unsigned long ulServerIp;
	unsigned short usServerPort;
	UDP_ASSOCIATION_T *ptUdpAssoc;
	TIMER_HANDLE_T tReceiveTimer;
	unsigned long ulPacketSeed;
} FUNCTION_ECHO_CLIENT_HANDLE_T;



typedef union FUNCTION_HANDLE_ENUM
{
	FUNCTION_ECHO_SERVER_HANDLE_T tServer;
	FUNCTION_ECHO_CLIENT_HANDLE_T tClient;
} FUNCTION_HANDLE_T;


typedef void (*PFN_NETWORK_DRIVER_HANDLE_RECEIVED_PACKET)(struct NETWORK_DRIVER_STRUCT *ptNetworkDriver, void *pvPacket, void *phPacket, unsigned int sizPacket);

typedef struct NETWORK_DRIVER_STRUCT
{
	int f_is_configured;
	unsigned int uiPort;
	NETWORK_STATE_T tState;
	NETWORK_IF_T tNetworkIf;
	PFN_NETWORK_DRIVER_HANDLE_RECEIVED_PACKET pfnHandleReceivedPacket;
	ETHERNET_PORT_CONFIGURATION_T tEthernetPortCfg;
	TIMER_HANDLE_T tLinkUpTimer;
	TIMER_HANDLE_T tEthernetHandlerTimer;
	NETWORK_DRIVER_DATA_T tNetworkDriverData;
	FUNCTION_HANDLE_T tFunctionHandle;
} NETWORK_DRIVER_T;


#endif  /* __NETWORK_INTERFACE_H__ */
