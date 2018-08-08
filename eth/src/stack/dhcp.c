/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include <string.h>

#include "dhcp.h"

#include "stack/ipv4.h"
#include "stack/udp.h"
#include "options.h"
#include "systime.h"



#define DHCP_OP_BOOTREQUEST 1
#define DHCP_OP_BOOTREPLY 2

#define DHCP_DISCOVER_SRC_PORT 68
#define DHCP_DISCOVER_DST_PORT 67
#define DHCP_DISCOVER_DST_IP IP_ADR(255,255,255,255)


typedef enum
{
	DHCP_MSGTYP_DHCPDISCOVER	= 1,
	DHCP_MSGTYP_DHCPOFFER		= 2,
	DHCP_MSGTYP_DHCPREQUEST		= 3,
	DHCP_MSGTYP_DHCPDECLINE		= 4,
	DHCP_MSGTYP_DHCPACK		= 5,
	DHCP_MSGTYP_DHCPNAK		= 6,
	DHCP_MSGTYP_DHCPRELEASE		= 7,
	DHCP_MSGTYP_DHCPINFORM		= 8
} DHCP_MSG_TYP_T;


typedef enum
{
	DHCP_OPT_Pad				= 0,
	DHCP_OPT_SubnetMask			= 1,
	DHCP_OPT_TimeOffset			= 2,
	DHCP_OPT_Router				= 3,
	DHCP_OPT_TimeServer			= 4,
	DHCP_OPT_NameServer			= 5,
	DHCP_OPT_DomainNameServer		= 6,
	DHCP_OPT_LogServer			= 7,
	DHCP_OPT_CookieServer			= 8,
	DHCP_OPT_LPRServer			= 9,
	DHCP_OPT_ImpressServer			= 10,
	DHCP_OPT_RessourceLocationServer	= 11,
	DHCP_OPT_HostName			= 12,
	DHCP_OPT_BootFileSize			= 13,
	DHCP_OPT_MeritDumpFile			= 14,
	DHCP_OPT_DomainName			= 15,
	DHCP_OPT_SwapServer			= 16,
	DHCP_OPT_RootPath			= 17,
	DHCP_OPT_ExtensionsPath			= 18,
	DHCP_OPT_IpForwarding			= 19,
	DHCP_OPT_NonLocalSourceRouting		= 20,
	DHCP_OPT_PolicyFilter			= 21,
	DHCP_OPT_MaximumDatagramReassemblySize	= 22,
	DHCP_OPT_DefaultIpTTL			= 23,
	DHCP_OPT_PathMtuAgingTimeout		= 24,
	DHCP_OPT_PathMtuPlateauTable		= 25,
	DHCP_OPT_InterfaceMtu			= 26,
	DHCP_OPT_AllSubnetsAreLocal		= 27,
	DHCP_OPT_BroadcastAddress		= 28,
	DHCP_OPT_PerformMaskDiscovery		= 29,
	DHCP_OPT_MaskSupplier			= 30,
	DHCP_OPT_PerformRouterDiscovery		= 31,
	DHCP_OPT_RouterSolicitationAddress	= 32,
	DHCP_OPT_StaticRoute			= 33,
	DHCP_OPT_TrailerEncapsulation		= 34,
	DHCP_OPT_ARPCacheTimeout		= 35,
	DHCP_OPT_EthernetEncapsulation		= 36,
	DHCP_OPT_TCPDefaultTTL			= 37,
	DHCP_OPT_TCPKeepaliveInterval		= 38,
	DHCP_OPT_TCPKeepaliveGarbage		= 39,
	DHCP_OPT_NISDomain			= 40,
	DHCP_OPT_NISServers			= 41,
	DHCP_OPT_NTPServers			= 42,

	DHCP_OPT_RequestedIPAddress		= 50,
	DHCP_OPT_IPAddressLeaseTime		= 51,
	DHCP_OPT_DHCPMessageType		= 53,
	DHCP_OPT_ServerIdentifier		= 54,
	DHCP_OPT_ParameterRequestList		= 55,
	DHCP_OPT_Message			= 56,
	DHCP_OPT_MaximumDHCPMessageSize		= 57,

	DHCP_OPT_TFTPServerName			= 66,
	DHCP_OPT_BootfileName			= 67,

	DHCP_OPT_End				= 255
} DHCP_OPTION_T;



static const unsigned char aucDhcpMagic[4] =
{
	0x63, 0x82, 0x53, 0x63
};


static const unsigned char aucDhcpOptionDiscover[3] =
{
	DHCP_OPT_DHCPMessageType,  1, DHCP_MSGTYP_DHCPDISCOVER
};


static const unsigned char aucDhcpOptionRequest[3] =
{
	DHCP_OPT_DHCPMessageType,  1, DHCP_MSGTYP_DHCPREQUEST
};


static const unsigned char aucDhcpOptionParamReqList[7] =
{
	DHCP_OPT_ParameterRequestList,
	5,
	DHCP_OPT_SubnetMask,
	DHCP_OPT_Router,
	DHCP_OPT_DomainNameServer,
	DHCP_OPT_TFTPServerName,
	DHCP_OPT_BootfileName
};


static void dhcp_cleanup(DHCP_HANDLE_DATA_T *ptDhcpHandle)
{
	UDP_ASSOCIATION_T *ptAssoc;


	ptAssoc = ptDhcpHandle->ptAssoc;
	if( ptAssoc!=NULL )
	{
		udp_unregisterPort(ptAssoc);
		ptDhcpHandle->ptAssoc = NULL;
	}
}


static int dhcp_send_discover_packet(NETWORK_DRIVER_T *ptNetworkDriver)
{
	int iResult;
	DHCP_HANDLE_DATA_T *ptDhcpHandle;
	ETH2_PACKET_T *ptSendPacket;
	DHCP_PACKET_T *ptDhcpPacket;
	unsigned char *pucOpts;
	unsigned long ulOptsSize;


	/* get a free frame for sending */
	ptSendPacket = eth_get_empty_packet(ptNetworkDriver);
	if( ptSendPacket==NULL )
	{
		iResult = -1;
	}
	else
	{
		ptDhcpHandle = &(ptNetworkDriver->tNetworkDriverData.tDhcpData);

		ptDhcpPacket = &(ptSendPacket->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData.tDhcpPkt);

		ptDhcpPacket->ucOp = DHCP_OP_BOOTREQUEST;
		ptDhcpPacket->ucHType = 1;
		ptDhcpPacket->ucHLen = 6;
		ptDhcpPacket->ucHops = 0;
		ptDhcpPacket->ulXId = ptDhcpHandle->ulXId;
		ptDhcpPacket->usSecs = 0;
		ptDhcpPacket->usFlags = 0;
		ptDhcpPacket->ulCiAddr = 0;
		ptDhcpPacket->ulYiAddr = 0;
		ptDhcpPacket->ulSiAddr = 0;
		ptDhcpPacket->ulGiAddr = 0;

		memcpy(ptDhcpPacket->aucChAddr, g_t_romloader_options.t_ethernet.aucMac, 6);
		memset(ptDhcpPacket->aucChAddr+6, 0, 10);
		memset(ptDhcpPacket->acSName, 0, 64);
		memset(ptDhcpPacket->acFile, 0, 128);

		pucOpts = ETH_USER_DATA_ADR(ptSendPacket->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData.tDhcpPkt);
		memcpy(pucOpts, aucDhcpMagic, sizeof(aucDhcpMagic));
		pucOpts += sizeof(aucDhcpMagic);
		memcpy(pucOpts, aucDhcpOptionDiscover, sizeof(aucDhcpOptionDiscover));
		pucOpts += sizeof(aucDhcpOptionDiscover);
		/* add parameter request list */
		memcpy(pucOpts, aucDhcpOptionParamReqList, sizeof(aucDhcpOptionParamReqList));
		pucOpts += sizeof(aucDhcpOptionParamReqList);
		/* end of list */
		*(pucOpts++) = DHCP_OPT_End;

		/* get the size of the options */
		ulOptsSize = (unsigned long)(pucOpts - ETH_USER_DATA_ADR(ptSendPacket->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData.tDhcpPkt));
		/* fill up to a minimum of 342 bytes */
		if( ulOptsSize<342 )
		{
			memset(pucOpts, 0, 342-ulOptsSize);
			ulOptsSize = 342;
		}

		udp_send_packet(ptNetworkDriver, ptSendPacket, sizeof(DHCP_PACKET_T)+ulOptsSize, ptDhcpHandle->ptAssoc);

		iResult = 0;
	}

	return iResult;
}


static int dhcp_send_request_packet(NETWORK_DRIVER_T *ptNetworkDriver)
{
	int iResult;
	DHCP_HANDLE_DATA_T *ptDhcpHandle;
	ETH2_PACKET_T *ptSendPacket;
	DHCP_PACKET_T *ptDhcpPacket;
	unsigned char *pucOpts;
	unsigned long ulOptsSize;


	/* get a free frame for sending */
	ptSendPacket = eth_get_empty_packet(ptNetworkDriver);
	if( ptSendPacket==NULL )
	{
		iResult = -1;
	}
	else
	{
		ptDhcpHandle = &(ptNetworkDriver->tNetworkDriverData.tDhcpData);

		ptDhcpPacket = &(ptSendPacket->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData.tDhcpPkt);

		ptDhcpPacket->ucOp = DHCP_OP_BOOTREQUEST;
		ptDhcpPacket->ucHType = 1;
		ptDhcpPacket->ucHLen = 6;
		ptDhcpPacket->ucHops = 0;
		ptDhcpPacket->ulXId = ptDhcpHandle->ulXId;
		ptDhcpPacket->usSecs = 0;
		ptDhcpPacket->usFlags = 0;
		ptDhcpPacket->ulCiAddr = 0;
		ptDhcpPacket->ulYiAddr = 0;
		ptDhcpPacket->ulSiAddr = 0;
		ptDhcpPacket->ulGiAddr = 0;

		memcpy(ptDhcpPacket->aucChAddr, g_t_romloader_options.t_ethernet.aucMac, 6);
		memset(ptDhcpPacket->aucChAddr+6, 0, 10);
		memset(ptDhcpPacket->acSName, 0, 64);
		memset(ptDhcpPacket->acFile, 0, 128);

		/* DHCP options start here */
		pucOpts = ETH_USER_DATA_ADR(ptSendPacket->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData.tDhcpPkt);
		/* magic must be first */
		memcpy(pucOpts, aucDhcpMagic, sizeof(aucDhcpMagic));
		pucOpts += sizeof(aucDhcpMagic);
		/* add the packet typ */
		memcpy(pucOpts, aucDhcpOptionRequest, sizeof(aucDhcpOptionRequest));
		pucOpts += sizeof(aucDhcpOptionRequest);
		/* add server identifier */
		*(pucOpts++) = DHCP_OPT_ServerIdentifier;
		*(pucOpts++) = 4;
		memcpy(pucOpts, ptDhcpHandle->aucServerIdentifier, 4);
		pucOpts += 4;
		/* add requested ip */
		*(pucOpts++) = 50;
		*(pucOpts++) = 4;
		memcpy(pucOpts, &(ptDhcpHandle->ulRequestIp), 4);
		pucOpts += 4;
		/* add parameter request list */
		memcpy(pucOpts, aucDhcpOptionParamReqList, sizeof(aucDhcpOptionParamReqList));
		pucOpts += sizeof(aucDhcpOptionParamReqList);
		/* end of list */
		*(pucOpts++) = DHCP_OPT_End;

		/* get the size of the options */
		ulOptsSize = (unsigned long)(pucOpts - ETH_USER_DATA_ADR(ptSendPacket->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData.tDhcpPkt));
		/* fill up to a minimum of 342 bytes */
		if( ulOptsSize<342 )
		{
			memset(pucOpts, 0, 342-ulOptsSize);
			ulOptsSize = 342;
		}

		udp_send_packet(ptNetworkDriver, ptSendPacket, sizeof(DHCP_PACKET_T)+ulOptsSize, ptDhcpHandle->ptAssoc);

		iResult = 0;
	}

	return iResult;
}


static const unsigned char *dhcp_getOption(DHCP_PACKET_T *ptPacket, unsigned int sizPacket, unsigned int uiOption)
{
	const unsigned char *pucCnt;
	const unsigned char *pucOpt;
	const unsigned char *pucEnd;


	pucCnt = ((unsigned char*)ptPacket) + sizeof(DHCP_PACKET_T);
	pucOpt = NULL;
	pucEnd = pucCnt + sizPacket;

	if( pucCnt+4<=pucEnd )
	{
		/* check magic */
		if( memcmp(pucCnt, aucDhcpMagic, sizeof(aucDhcpMagic))==0 )
		{
			pucCnt += 4;

			/* loop over all options */
			while( pucCnt+2<=pucEnd )
			{
				if( *pucCnt==uiOption && (pucCnt+2+pucCnt[1])<=pucEnd )
				{
					pucOpt = pucCnt;
					break;
				}
				pucCnt += 2 + pucCnt[1];
			}
		}
	}

	return pucOpt;
}


static void dhcp_recHandler(NETWORK_DRIVER_T *ptNetworkDriver, void *pvData, unsigned int sizDhcpLength, void *pvUser __attribute__((unused)))
{
	DHCP_HANDLE_DATA_T *ptDhcpHandle;
	ETH2_PACKET_T *ptPkt;
	DHCP_PACKET_T *ptDhcpPacket;
	int iResult;
	const unsigned char *pucOpt;
	unsigned long ulNewIp;
	unsigned long ulNewNetmask;
	unsigned long ulNewGatewayIp;


	/* cast the data to a eth2 packet */
	ptPkt = (ETH2_PACKET_T*)pvData;
	ptDhcpPacket = &(ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData.tDhcpPkt);

	ptDhcpHandle = &(ptNetworkDriver->tNetworkDriverData.tDhcpData);
	switch(ptDhcpHandle->tState)
	{

	case DHCP_STATE_Idle:
	case DHCP_STATE_Error:
	case DHCP_STATE_Ok:
		/* the connection is not open, ignore the packet */
		break;

	case DHCP_STATE_Discover:
		/* the packet must be a boot reply */
		if( ptDhcpPacket->ucOp==DHCP_OP_BOOTREPLY &&
		    ptDhcpPacket->ucHType==1 &&
		    ptDhcpPacket->ucHLen==6 &&
		    ptDhcpPacket->ulXId==ptDhcpHandle->ulXId &&
		    memcmp(ptDhcpPacket->aucChAddr, g_t_romloader_options.t_ethernet.aucMac, 6) == 0 &&
		    ptDhcpPacket->ulYiAddr!=0 )
		{
			/* is this a dhcp offer? */
			pucOpt = dhcp_getOption(ptDhcpPacket, sizDhcpLength, DHCP_OPT_DHCPMessageType);
			if( pucOpt!=NULL && pucOpt[1]==1 && pucOpt[2]==DHCP_MSGTYP_DHCPOFFER )
			{
				/* get the server identifier */
				pucOpt = dhcp_getOption(ptDhcpPacket, sizDhcpLength, DHCP_OPT_ServerIdentifier);
				if( pucOpt!=NULL && pucOpt[1]==4 )
				{
					memcpy(ptDhcpHandle->aucServerIdentifier, pucOpt+2, 4);
				}
				else
				{
					memset(ptDhcpHandle->aucServerIdentifier, 0, 4);
				}

				/* Request the IP. */
				ptDhcpHandle->ulRequestIp = ptDhcpPacket->ulYiAddr;

				iResult = dhcp_send_request_packet(ptNetworkDriver);
				if( iResult==0 )
				{
					/* state is now "request" */
					ptDhcpHandle->tState = DHCP_STATE_Request;

					/* reset timeout */
					ptDhcpHandle->ulLastGoodPacket = systime_get_ms();
					ptDhcpHandle->uiRetryCnt = g_t_romloader_options.t_ethernet.ucDhcpRetries;
				}
				else
				{
					/* cleanup */
					dhcp_cleanup(ptDhcpHandle);
				}
			}
		}
		break;

	case DHCP_STATE_Request:
		/* The packet must be a boot reply. */
		if( ptDhcpPacket->ucOp==DHCP_OP_BOOTREPLY &&
		    ptDhcpPacket->ucHType==1 &&
		    ptDhcpPacket->ucHLen==6 &&
		    ptDhcpPacket->ulXId==ptDhcpHandle->ulXId &&
		    memcmp(ptDhcpPacket->aucChAddr, g_t_romloader_options.t_ethernet.aucMac, 6) == 0 &&
		    ptDhcpPacket->ulYiAddr!=0 )
		{
			/* Store the new IP address. */
			ulNewIp = ptDhcpPacket->ulYiAddr;

			/* Is this a DHCP offer? */
			pucOpt = dhcp_getOption(ptDhcpPacket, sizDhcpLength, DHCP_OPT_DHCPMessageType);
			if( pucOpt!=NULL && pucOpt[1]==1 )
			{
				if( pucOpt[2]==DHCP_MSGTYP_DHCPACK )
				{
					/* check for the subnetmask option */
					pucOpt = dhcp_getOption(ptDhcpPacket, sizDhcpLength, DHCP_OPT_SubnetMask);
					if( pucOpt!=NULL && pucOpt[1]==4 )
					{
						/* store the new netmask */
						ulNewNetmask = IP_ADR(pucOpt[2], pucOpt[3], pucOpt[4], pucOpt[5]);

						/* check for the gateway option (router) */
						pucOpt = dhcp_getOption(ptDhcpPacket, sizDhcpLength, DHCP_OPT_Router);
						if( pucOpt!=NULL && pucOpt[1]==4 )
						{
							/* get the new gateway */
							ulNewGatewayIp = IP_ADR(pucOpt[2], pucOpt[3], pucOpt[4], pucOpt[5]);

							/* now the settings are complete, accept the stored values */
							g_t_romloader_options.t_ethernet.ulIp = ulNewIp;
							g_t_romloader_options.t_ethernet.ulNetmask = ulNewNetmask;
							g_t_romloader_options.t_ethernet.ulGatewayIp = ulNewGatewayIp;

							/* cleanup */
							dhcp_cleanup(ptDhcpHandle);

							ptDhcpHandle->tState = DHCP_STATE_Ok;
						}
						else
						{
							/* cleanup */
							dhcp_cleanup(ptDhcpHandle);

							ptDhcpHandle->tState = DHCP_STATE_Error;
						}
					}
					else
					{
						/* cleanup */
						dhcp_cleanup(ptDhcpHandle);

						ptDhcpHandle->tState = DHCP_STATE_Error;
					}
				}
				else if( pucOpt[2]==DHCP_MSGTYP_DHCPNAK )
				{
					/* cleanup */
					dhcp_cleanup(ptDhcpHandle);

					ptDhcpHandle->tState = DHCP_STATE_Error;
				}
			}
		}
		break;
	}
}


void dhcp_init(NETWORK_DRIVER_T *ptNetworkDriver)
{
	DHCP_HANDLE_DATA_T *ptDhcpHandle;


	ptDhcpHandle = &(ptNetworkDriver->tNetworkDriverData.tDhcpData);

	ptDhcpHandle->tState = DHCP_STATE_Idle;

	/* No DHCP connection open. */
	ptDhcpHandle->ptAssoc = NULL;

	/* Initialize XID. */
	ptDhcpHandle->ulXId = ((unsigned long)(g_t_romloader_options.t_ethernet.aucMac[0])) |
	                      (((unsigned long)(g_t_romloader_options.t_ethernet.aucMac[1])) <<  8U) |
		              (((unsigned long)(g_t_romloader_options.t_ethernet.aucMac[2])) << 16U) |
		              (((unsigned long)(g_t_romloader_options.t_ethernet.aucMac[3])) << 24U);
}


DHCP_STATE_T dhcp_getState(NETWORK_DRIVER_T *ptNetworkDriver)
{
	return ptNetworkDriver->tNetworkDriverData.tDhcpData.tState;
}


void dhcp_request(NETWORK_DRIVER_T *ptNetworkDriver)
{
	int iResult;
	DHCP_HANDLE_DATA_T *ptDhcpHandle;


	ptDhcpHandle = &(ptNetworkDriver->tNetworkDriverData.tDhcpData);

	/* open UDP port and register callback */
	ptDhcpHandle->ptAssoc = udp_registerPort(MUS2NUS(DHCP_DISCOVER_SRC_PORT), DHCP_DISCOVER_DST_IP, MUS2NUS(DHCP_DISCOVER_DST_PORT), dhcp_recHandler, NULL);
	if( ptDhcpHandle->ptAssoc!=NULL )
	{
		++(ptDhcpHandle->ulXId);
		iResult = dhcp_send_discover_packet(ptNetworkDriver);
		if( iResult==0 )
		{
			/* state is now "discover" */
			ptDhcpHandle->tState = DHCP_STATE_Discover;

			/* reset timeout */
			ptDhcpHandle->ulLastGoodPacket = systime_get_ms();
			ptDhcpHandle->uiRetryCnt = g_t_romloader_options.t_ethernet.ucDhcpRetries;

		}
		else
		{
			/* cleanup */
			dhcp_cleanup(ptDhcpHandle);

			ptDhcpHandle->tState = DHCP_STATE_Error;
		}
	}
}


void dhcp_timer(NETWORK_DRIVER_T *ptNetworkDriver)
{
	DHCP_HANDLE_DATA_T *ptDhcpHandle;
	TIMER_HANDLE_T tHandle;
	int iRes;


	ptDhcpHandle = &(ptNetworkDriver->tNetworkDriverData.tDhcpData);

	tHandle.ulStart = ptDhcpHandle->ulLastGoodPacket;
	tHandle.ulDuration = g_t_romloader_options.t_ethernet.usDhcpTimeout;

	switch(ptDhcpHandle->tState)
	{
	case DHCP_STATE_Idle:
	case DHCP_STATE_Error:
	case DHCP_STATE_Ok:
		break;

	case DHCP_STATE_Discover:
		iRes = systime_handle_is_elapsed(&tHandle);
		if( iRes!=0 )
		{
			/* Timeout -> are retries left? */
			if( ptDhcpHandle->uiRetryCnt>0 )
			{
				/* Re-send the last packet. */
				dhcp_send_discover_packet(ptNetworkDriver);

				ptDhcpHandle->ulLastGoodPacket = systime_get_ms();
				--(ptDhcpHandle->uiRetryCnt);
			}
			else
			{
				/* Close the connection. */
				dhcp_cleanup(ptDhcpHandle);

				ptDhcpHandle->tState = DHCP_STATE_Error;
			}
		}
		break;

	case DHCP_STATE_Request:
		iRes = systime_handle_is_elapsed(&tHandle);
		if( iRes!=0 )
		{
			/* Timeout -> are retries left? */
			if( ptDhcpHandle->uiRetryCnt>0 )
			{
				/* Re-send the last packet. */
				dhcp_send_request_packet(ptNetworkDriver);

				ptDhcpHandle->ulLastGoodPacket = systime_get_ms();
				--(ptDhcpHandle->uiRetryCnt);
			}
			else
			{
				/* Close the connection. */
				dhcp_cleanup(ptDhcpHandle);

				ptDhcpHandle->tState = DHCP_STATE_Error;
			}
		}
		break;
	}
}

