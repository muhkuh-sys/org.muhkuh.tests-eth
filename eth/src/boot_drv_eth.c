#include "boot_drv_eth.h"

#include <string.h>

#include "driver/hal_muhkuh.h"
#if CFG_USE_ETHMAC==1
#       include "driver/hal_ethmac_wrapper.h"
#elif CFG_USE_ETH2PS==1
#       include "driver/hal_eth2ps_wrapper.h"
#endif
#include "stack/buckets.h"
#include "stack/arp.h"
#include "stack/dhcp.h"
#include "stack/eth.h"
#include "stack/icmp.h"
#include "stack/ipv4.h"
#include "options.h"
#include "uprintf.h"


static void echo_server_process_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvData, unsigned int sizPacket, void *pvUser __attribute__((unused)))
{
	int iResult;
	ETH2_PACKET_T *ptSendPacket;
	void *phSendPacket;
	ETH2_PACKET_T *ptPkt;
	UDP_ASSOCIATION_T *ptAssoc;


	if( sizPacket>0 )
	{
		/* The user data is the pointer to the network driver. */
		ptAssoc = ptNetworkDriver->tFunctionHandle.tServer.ptUdpAssoc;

		/* Cast the data to a eth2 packet. */
		ptPkt = (ETH2_PACKET_T*)pvData;

		/* Allocate a new packet. */
		iResult = eth_get_empty_packet(ptNetworkDriver, &ptSendPacket, &phSendPacket);
		if( iResult==0 )
		{
			/* Send the data back. */
			ptAssoc->ulRemoteIp = ptPkt->uEth2Data.tIpPkt.tIpHdr.ulSrcIp;
			ptAssoc->uiRemotePort = ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usSrcPort;

			/* Copy the packet data. */
			memcpy((unsigned char*)(&(ptSendPacket->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData)), (unsigned char*)(&(ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData)), sizPacket);

			/* Maybe do something with the data, like an XOR? */

			udp_send_packet(ptNetworkDriver, ptSendPacket, phSendPacket, sizPacket, ptAssoc);
		}
	}
}



static unsigned long pseudo_generator(unsigned long ulSeed)
{
	/* Works with a LFSR (linear feedback left shift register)
	 * with 32 Bits with MLS (Max Length sequence)
	 * which reproduces only after 2^32-1 generations
	 */
	ulSeed =
		(
			(
				(
					(ulSeed >> 31U)
					^ (ulSeed >> 6U)
					^ (ulSeed >> 4U)
					^ (ulSeed >> 1U)
					^  ulSeed
				)
				&  0x00000001
			)
			<< 31
		)
		| (ulSeed>>1);

	return ulSeed;
}



static unsigned int get_test_data_size(FUNCTION_ECHO_CLIENT_HANDLE_T *ptHandle)
{
	unsigned int sizPacket;


	/* Send between 64 and 1024 bytes.
	 * Note that the last 2 bits are masked out, so the
	 * size is always a multiple of DWORDS.
	 */
	sizPacket = ptHandle->uiPacketsLeft & 0x03fcU;
	if( sizPacket<64U )
	{
		sizPacket = 64U;
	}
	return sizPacket;
}



static ECHO_CLIENT_STATE_T echo_client_action(NETWORK_DRIVER_T *ptNetworkDriver)
{
	FUNCTION_ECHO_CLIENT_HANDLE_T *ptHandle;
	ECHO_CLIENT_STATE_T tState;
	void *pvPkt;
	ETH2_PACKET_T *ptPkt;
	void *phPkt;
	unsigned int sizPacket;
	unsigned char *pucDataCnt;
	unsigned char *pucDataEnd;
	unsigned long ulData;
	int iIsElapsed;
	const char *pcName;
	int iResult;


	/* Get a shortcut to the handle. */
	ptHandle = &(ptNetworkDriver->tFunctionHandle.tClient);

	pcName = ptNetworkDriver->tEthernetPortCfg.acName;

	tState = ptHandle->tState;
	switch( tState )
	{
	case ECHO_CLIENT_STATE_Idle:
	case ECHO_CLIENT_STATE_PacketReceived:
		/* Check if there are packets left to send. */
		if( ptHandle->uiPacketsLeft==0 )
		{
			/* No packets left to send -> The test has finished. */
			tState = ECHO_CLIENT_STATE_Ok;
		}
		else
		{
			/* Create a new packet. */
			iResult = ptNetworkDriver->tNetworkIf.pfnGetEmptyPacket(ptNetworkDriver, &pvPkt, &phPkt);
			if( iResult!=0 )
			{
				uprintf("%s: No free Ethernet packet available.\n", pcName);
				tState = ECHO_CLIENT_STATE_Error;
			}
			else
			{
				ptPkt = (ETH2_PACKET_T *)pvPkt;
				sizPacket = get_test_data_size(ptHandle);

				/* Fill the packet with pseudo random data. */
				ulData = ptHandle->ulPacketSeed;
				pucDataCnt = (unsigned char*)(&(ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData));
				pucDataEnd = pucDataCnt + sizPacket;
				do
				{
					*(pucDataCnt++) = (unsigned char)( ulData & 0xffU);
					*(pucDataCnt++) = (unsigned char)((ulData >>  8U) & 0xffU);
					*(pucDataCnt++) = (unsigned char)((ulData >> 16U) & 0xffU);
					*(pucDataCnt++) = (unsigned char)((ulData >> 24U) & 0xffU);
					ulData = pseudo_generator(ulData);
				} while( pucDataCnt<pucDataEnd );

				/* Send the packet. */
				udp_send_packet(ptNetworkDriver, ptPkt, phPkt, sizPacket, ptHandle->ptUdpAssoc);

				/* Start the timeout for the receive operation.
				 * A value of 1000ms is much too high, but does not hurt on the other hand.
				 */
				systime_handle_start_ms(&(ptHandle->tReceiveTimer), 1000);

				/* Wait for the response. */
				tState = ECHO_CLIENT_STATE_WaitingForResponse;
			}
		}
		break;

	case ECHO_CLIENT_STATE_WaitingForResponse:
		/* Check for a timeout. */
		iIsElapsed = systime_handle_is_elapsed(&(ptHandle->tReceiveTimer));
		if( iIsElapsed!=0 )
		{
			uprintf("%s: Timeout receiving a packet.\n", pcName);
			tState = ECHO_CLIENT_STATE_Error;
		}
		break;

	case ECHO_CLIENT_STATE_Ok:
		break;

	case ECHO_CLIENT_STATE_Error:
		uprintf("%s: in error state\n", pcName);
		break;
	}
	ptHandle->tState = tState;

	return tState;
}



static void echo_client_process_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvData, unsigned int sizPacket, void *pvUser __attribute__((unused)))
{
	ETH2_PACKET_T *ptPkt;
	FUNCTION_ECHO_CLIENT_HANDLE_T *ptHandle;
	ECHO_CLIENT_STATE_T tState;
	unsigned int sizPacketExpected;
	unsigned char *pucCnt;
	unsigned char *pucEnd;
	unsigned long ulPacketData;
	unsigned long ulExpectedData;
	const char *pcName;


	pcName = ptNetworkDriver->tEthernetPortCfg.acName;

	if( sizPacket>0 )
	{
		/* Get a shortcut to the handle. */
		ptHandle = &(ptNetworkDriver->tFunctionHandle.tClient);

		tState = ptHandle->tState;
		switch(tState)
		{
		case ECHO_CLIENT_STATE_Idle:
			/* Ignore the packet. */
			break;

		case ECHO_CLIENT_STATE_WaitingForResponse:
			/* Cast the data to a eth2 packet. */
			ptPkt = (ETH2_PACKET_T*)pvData;

			/* Get the expected size of the packet. */
			sizPacketExpected = get_test_data_size(ptHandle);
			if( sizPacket!=sizPacketExpected )
			{
				uprintf("%s: The received packet has %d, but it should have %d bytes.\n", pcName, sizPacket, sizPacketExpected);
				tState = ECHO_CLIENT_STATE_Error;
			}
			else
			{
				/* Loop over the complete packet. */
				ulExpectedData = ptHandle->ulPacketSeed;
				pucCnt = (unsigned char*)(&(ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.uUdpData));
				pucEnd = pucCnt + sizPacket;
				do
				{
					/* Get the next DWORD. */
					ulPacketData  =  (unsigned long)*(pucCnt++);
					ulPacketData |= ((unsigned long)*(pucCnt++)) <<  8U;
					ulPacketData |= ((unsigned long)*(pucCnt++)) << 16U;
					ulPacketData |= ((unsigned long)*(pucCnt++)) << 24U;

					if( ulPacketData!=ulExpectedData )
					{
						uprintf("%s: The received data differs from the expected: 0x%08x - 0x%08x\n", pcName, ulPacketData, ulExpectedData);
						tState = ECHO_CLIENT_STATE_Error;
						break;
					}

					ulExpectedData = pseudo_generator(ulExpectedData);
				} while( pucCnt<pucEnd );

				if( tState!=ECHO_CLIENT_STATE_Error )
				{
					/* The packet is OK! */
					--(ptHandle->uiPacketsLeft);
					ptHandle->ulPacketSeed = ulExpectedData;
					tState = ECHO_CLIENT_STATE_PacketReceived;
				}
			}
			break;

		case ECHO_CLIENT_STATE_PacketReceived:
			/* Ignore the packet. */
			break;

		case ECHO_CLIENT_STATE_Ok:
			/* Ignore the packet. */
			break;

		case ECHO_CLIENT_STATE_Error:
			/* Ignore the packet. */
			uprintf("%s: process packet called in error state.\n", pcName);
			break;
		}
		ptHandle->tState = tState;
	}
}



static void echo_client_initialize(NETWORK_DRIVER_T *ptNetworkDriver, ETHERNET_PORT_CONFIGURATION_T *ptEthCfg)
{
	FUNCTION_ECHO_CLIENT_HANDLE_T *ptHandle;
	unsigned long ulSeed;
	unsigned short usLocalPort;
	unsigned long ulRemoteIp;
	unsigned long ulNumberOfTestPackets;
	unsigned short usRemotePort;


	usLocalPort = MUS2NUS(ptEthCfg->usLocalPort);
	ulRemoteIp = ptEthCfg->ulRemoteIp;
	usRemotePort = MUS2NUS(ptEthCfg->usRemotePort);

	/* If the number of packets is set to 0, use a default value. */
	ulNumberOfTestPackets = ptEthCfg->ulNumberOfTestPackets;
	if( ulNumberOfTestPackets==0 )
	{
		ulNumberOfTestPackets = 2048;
	}

	ptHandle = &(ptNetworkDriver->tFunctionHandle.tClient);

	ptHandle->tState = ECHO_CLIENT_STATE_Idle;
	ptHandle->uiPacketsLeft = ulNumberOfTestPackets;
	ptHandle->ulServerIp = ulRemoteIp;
	ptHandle->usServerPort = usRemotePort;
	ptHandle->ptUdpAssoc = udp_registerPort(
		ptNetworkDriver,
		usLocalPort,                 /* The local port. */
		ulRemoteIp,                  /* The IP of the echo server. */
		usRemotePort,                /* The port of the echo server. */
		echo_client_process_packet,  /* The handler routine for received packets. */
		NULL                         /* The user parameter for the receive handler. */
	);
	ptHandle->tReceiveTimer.ulDuration = 0;
	ptHandle->tReceiveTimer.ulStart = 0;

	/* Generate the seed from the MAC. */
	ulSeed  =  (unsigned long)(ptEthCfg->aucMac[0]);
	ulSeed |= ((unsigned long)(ptEthCfg->aucMac[1])) <<  8U;
	ulSeed |= ((unsigned long)(ptEthCfg->aucMac[2])) << 16U;
	ulSeed |= ((unsigned long)(ptEthCfg->aucMac[3])) << 24U;
	ptHandle->ulPacketSeed = ulSeed;
}



#if MAX_NETWORK_INTERFACES!=2
#       error "The function ethernet_init is hard-coded for 2 network interfaces. This has changed. The code must be updated."
#endif
int ethernet_init(ETHERNET_PORT_CONFIGURATION_T *atEthCfg, NETWORK_DRIVER_T *atNetworkDriver)
{
	int iResult;
#if CFG_USE_ETH2PS!=0
	HAL_ETH2PS_RESULT_T tHalEth2psResult;
#endif
	unsigned int uiInterfaceIndex;
	ETHERNET_PORT_CONFIGURATION_T *ptEthCfg;
	NETWORK_DRIVER_T *ptNetworkDriver;
	NETWORK_DRIVER_T *ptNetworkDriver0;
	NETWORK_DRIVER_T *ptNetworkDriver1;
	INTERFACE_T tInterface0;
	INTERFACE_T tInterface1;
	INTERFACE_FUNCTION_T tFunction;


	iResult = 0;

	for(uiInterfaceIndex=0; uiInterfaceIndex<MAX_NETWORK_INTERFACES; ++uiInterfaceIndex)
	{
		ptEthCfg = &(atEthCfg[uiInterfaceIndex]);
		ptNetworkDriver = &(atNetworkDriver[uiInterfaceIndex]);

		if( ptEthCfg->ulInterface==INTERFACE_None )
		{
			/* This interface is not in use. */
			uprintf("Interface %d is not in use.\n", uiInterfaceIndex);
			memset(ptNetworkDriver, 0, sizeof(NETWORK_DRIVER_T));
		}
		else
		{
			if( ptEthCfg->acName==NULL )
			{
				uprintf("ERROR: the name of interface %d is not set.\n", uiInterfaceIndex);
				iResult = -1;
			}
			else
			{
				memcpy(&(ptNetworkDriver->tEthernetPortCfg), ptEthCfg, sizeof(ETHERNET_PORT_CONFIGURATION_T));
			}
		}
	}

	if( iResult==0 )
	{
		/* Get both interfaces. */
		tInterface0 = (INTERFACE_T)(atEthCfg[0].ulInterface);
		tInterface1 = (INTERFACE_T)(atEthCfg[1].ulInterface);
		/* Get both network drivers. */
		ptNetworkDriver0 = atNetworkDriver;
		ptNetworkDriver1 = atNetworkDriver + 1U;

		/* Configuration 1: 1 single port on INTPHY */
		if( tInterface0==INTERFACE_ETHMAC_INTPHY0 && tInterface1==INTERFACE_None )
		{
#if CFG_USE_ETHMAC==0
			uprintf("ERROR: ETHMAC is not available in this build.\n");
			iResult = -1;
#else
			/* Activate only one PHY port. */
			hal_ethmac_phy_init(1);

			hal_ethmac_pfifo_reset();

			iResult = hal_muhkuh_ethmac_prepare(ptNetworkDriver0, 0);
			if( iResult==0 )
			{
				iResult = hal_muhkuh_ethmac_disable(ptNetworkDriver0, 0);
				if( iResult==0 )
				{
					iResult = hal_muhkuh_ethmac_initialize(ptNetworkDriver0, 0);
				}
			}
#endif
		}
		/* Configuration 2: 2 separate ports on INTPHY */
		else if( tInterface0==INTERFACE_ETHMAC_INTPHY0 && tInterface1==INTERFACE_ETHMAC_INTPHY1 )
		{
#if CFG_USE_ETHMAC==0
			uprintf("ERROR: ETHMAC is not available in this build.\n");
			iResult = -1;
#else
			/* Activate both PHY ports. */
			hal_ethmac_phy_init(2);

			hal_ethmac_pfifo_reset();

			iResult = hal_muhkuh_ethmac_prepare(ptNetworkDriver0, 0);
			if( iResult==0 )
			{
				iResult = hal_muhkuh_ethmac_prepare(ptNetworkDriver1, 1);
				if( iResult==0 )
				{
					iResult = hal_muhkuh_ethmac_disable(ptNetworkDriver0, 0);
					if( iResult==0 )
					{
						iResult = hal_muhkuh_ethmac_disable(ptNetworkDriver1, 1);
						if( iResult==0 )
						{
							iResult = hal_muhkuh_ethmac_initialize(ptNetworkDriver0, 0);
							if( iResult==0 )
							{
								iResult = hal_muhkuh_ethmac_initialize(ptNetworkDriver1, 1);
							}
						}
					}
				}
			}
#endif
		}
		/* Configuration 3: 1 port on INTPHY and 1 port on SPE as a switch */
		else if( tInterface0==INTERFACE_ETH2PS_INTPHY0 && tInterface1==INTERFACE_ETH2PS_EXTSPE0 )
		{
#if CFG_USE_ETH2PS==0
			uprintf("ERROR: ETH2PS is not available in this build.\n");
			iResult = -1;
#else
			iResult = hal_muhkuh_eth2ps_prepare(ptNetworkDriver0);
			if( iResult!=0 )
			{
				uprintf("Failed to prepare the ETH2PS HAL: %d\n", iResult);
			}
			else
			{
				/* Activate the internal and external PHY. */
				tHalEth2psResult = hal_eth2ps_phy_init(1);
				if( tHalEth2psResult!=HAL_ETH2PS_RESULT_Ok )
				{
					uprintf("Failed to initialize the PHY for the ETH2PS HAL: %d\n", tHalEth2psResult);
					iResult = -1;
				}
				else
				{
					hal_eth2ps_pfifo_reset();

					iResult = hal_muhkuh_eth2ps_disable(ptNetworkDriver0);
					if( iResult!=0 )
					{
						uprintf("Failed to disable the ETH2PS HAL in the startup procedure: %d\n", iResult);
					}
					else
					{
						iResult = hal_muhkuh_eth2ps_initialize(ptNetworkDriver0, ptNetworkDriver1);
						if( iResult!=0 )
						{
							uprintf("Failed to initialize the ETH2PS HAL: %d\n", iResult);
						}
					}
				}
			}
#endif
		}
		else
		{
			uprintf("ERROR: invalid combination of interface values.\n");
			iResult = -1;
		}

		if( iResult==0 )
		{
			buckets_init();

			for(uiInterfaceIndex=0; uiInterfaceIndex<MAX_NETWORK_INTERFACES; ++uiInterfaceIndex)
			{
				ptEthCfg = &(atEthCfg[uiInterfaceIndex]);
				if( ptEthCfg->ulInterface!=INTERFACE_None )
				{
					ptNetworkDriver = &(atNetworkDriver[uiInterfaceIndex]);

					arp_init(ptNetworkDriver);
					ipv4_init(ptNetworkDriver);
					udp_init(ptNetworkDriver);
					dhcp_init(ptNetworkDriver);

					ptNetworkDriver->f_is_configured = 1;
					ptNetworkDriver->tState = NETWORK_STATE_NoLink;
					systime_handle_start_ms(&(ptNetworkDriver->tLinkUpTimer), 0);
					systime_handle_start_ms(&(ptNetworkDriver->tEthernetHandlerTimer), 1000);

					tFunction = (INTERFACE_FUNCTION_T)(ptEthCfg->ulFunction);
					switch(tFunction)
					{
					case INTERFACE_FUNCTION_None:
						break;

					case INTERFACE_FUNCTION_EchoServer:
						ptNetworkDriver->tFunctionHandle.tServer.ptUdpAssoc = udp_registerPort(
							ptNetworkDriver,
							MUS2NUS(ptEthCfg->usLocalPort),
							IP_ADR(0, 0, 0, 0),
							0,
							echo_server_process_packet,
							NULL
						);
						break;

					case INTERFACE_FUNCTION_EchoClient:
						echo_client_initialize(ptNetworkDriver, ptEthCfg);
						break;
					}

					uprintf("%s: Interface initialized.\n", ptEthCfg->acName);
				}
			}
		}
	}

	return iResult;
}



static void ethernet_cyclic_process(NETWORK_DRIVER_T *ptNetworkDriver)
{
	if( systime_handle_is_elapsed(&(ptNetworkDriver->tEthernetHandlerTimer))!=0 )
	{
		/* process cyclic events here */
		arp_timer(ptNetworkDriver);
		dhcp_timer(ptNetworkDriver);

		systime_handle_start_ms(&(ptNetworkDriver->tEthernetHandlerTimer), 1000);
	}
}



typedef struct LINK_SPEED_TO_STRING_STRUCT
{
	LINK_SPEED_T tLinkSpeed;
	const char *pcName;
} LINK_SPEED_TO_STRING_T;
static const LINK_SPEED_TO_STRING_T atLinkSpeedToString[] =
{
	{ LINK_SPEED_Unknown, "unknown" },
	{ LINK_SPEED_None,    "none" },
	{ LINK_SPEED_10MBit,  "10MBit" },
	{ LINK_SPEED_100MBit, "100MBit" }
};

static const char *link_speed_to_string(LINK_SPEED_T tLinkSpeed)
{
	const LINK_SPEED_TO_STRING_T *ptCnt;
	const LINK_SPEED_TO_STRING_T *ptEnd;
	const char *pcName;


	pcName = "???";
	ptCnt = atLinkSpeedToString;
	ptEnd = atLinkSpeedToString + (sizeof(atLinkSpeedToString)/sizeof(atLinkSpeedToString[0]));
	while( ptCnt<ptEnd )
	{
		if( ptCnt->tLinkSpeed==tLinkSpeed )
		{
			pcName = ptCnt->pcName;
			break;
		}
		++ptCnt;
	}
	return pcName;
}


typedef struct LINK_DUPLEX_TO_STRING_STRUCT
{
	LINK_DUPLEX_T tLinkDuplex;
	const char *pcName;
} LINK_DUPLEX_TO_STRING_T;
static const LINK_DUPLEX_TO_STRING_T atLinkDuplexToString[] =
{
	{ LINK_DUPLEX_Unknown, "unknown" },
	{ LINK_DUPLEX_None,    "none" },
	{ LINK_DUPLEX_Half,    "half duplex" },
	{ LINK_DUPLEX_Full,    "full duplex" }
};

static const char *link_duplex_to_string(LINK_DUPLEX_T tLinkDuplex)
{
	const LINK_DUPLEX_TO_STRING_T *ptCnt;
	const LINK_DUPLEX_TO_STRING_T *ptEnd;
	const char *pcName;


	pcName = "???";
	ptCnt = atLinkDuplexToString;
	ptEnd = atLinkDuplexToString + (sizeof(atLinkDuplexToString)/sizeof(atLinkDuplexToString[0]));
	while( ptCnt<ptEnd )
	{
		if( ptCnt->tLinkDuplex==tLinkDuplex )
		{
			pcName = ptCnt->pcName;
			break;
		}
		++ptCnt;
	}
	return pcName;
}


static int check_link_attributes(unsigned long ulExpectedLinkAttributes, LINK_SPEED_T tLinkSpeed, LINK_DUPLEX_T tLinkDuplex)
{
	EXPECTED_LINK_ATTRIBUTES_T tExpectedLinkAttributes;
	LINK_SPEED_T tExpectedLinkSpeed;
	LINK_DUPLEX_T tExpectedLinkDuplex;
	int iResult;


	/* Be pessimistic. */
	iResult = -1;

	tExpectedLinkAttributes = (EXPECTED_LINK_ATTRIBUTES_T)ulExpectedLinkAttributes;
	switch( tExpectedLinkAttributes )
	{
	case EXPECTED_LINK_ATTRIBUTES_ANY:
	case EXPECTED_LINK_ATTRIBUTES_10_HALF:
	case EXPECTED_LINK_ATTRIBUTES_10_FULL:
	case EXPECTED_LINK_ATTRIBUTES_100_HALF:
	case EXPECTED_LINK_ATTRIBUTES_100_FULL:
		iResult = 0;
		break;
	}
	if( iResult!=0 )
	{
		uprintf("The expected link attributes have an invalid value of %d.\n", ulExpectedLinkAttributes);
	}
	else
	{
		tExpectedLinkSpeed = LINK_SPEED_Unknown;
		tExpectedLinkDuplex = LINK_DUPLEX_Unknown;

		switch( tExpectedLinkAttributes )
		{
		case EXPECTED_LINK_ATTRIBUTES_ANY:
			/* Accept all speed and duplex attributes. */
			break;

		case EXPECTED_LINK_ATTRIBUTES_10_HALF:
			tExpectedLinkSpeed = LINK_SPEED_10MBit;
			tExpectedLinkDuplex = LINK_DUPLEX_Half;
			if( (tLinkSpeed!=tExpectedLinkSpeed) || (tLinkDuplex!=tExpectedLinkDuplex) )
			{
				iResult = -1;
			}
			break;

		case EXPECTED_LINK_ATTRIBUTES_10_FULL:
			tExpectedLinkSpeed = LINK_SPEED_10MBit;
			tExpectedLinkDuplex = LINK_DUPLEX_Full;
			if( (tLinkSpeed!=tExpectedLinkSpeed) || (tLinkDuplex!=tExpectedLinkDuplex) )
			{
				iResult = -1;
			}
			break;

		case EXPECTED_LINK_ATTRIBUTES_100_HALF:
			tExpectedLinkSpeed = LINK_SPEED_100MBit;
			tExpectedLinkDuplex = LINK_DUPLEX_Half;
			if( (tLinkSpeed!=tExpectedLinkSpeed) || (tLinkDuplex!=tExpectedLinkDuplex) )
			{
				iResult = -1;
			}
			break;

		case EXPECTED_LINK_ATTRIBUTES_100_FULL:
			tExpectedLinkSpeed = LINK_SPEED_100MBit;
			tExpectedLinkDuplex = LINK_DUPLEX_Full;
			if( (tLinkSpeed!=tExpectedLinkSpeed) || (tLinkDuplex!=tExpectedLinkDuplex) )
			{
				iResult = -1;
			}
			break;
		}

		if( iResult!=0 )
		{
			uprintf("The established link has unexpected attributes. Expected % %, got %s %s.\n",
				link_speed_to_string(tExpectedLinkSpeed),
				link_duplex_to_string(tExpectedLinkDuplex),
				link_speed_to_string(tLinkSpeed),
				link_duplex_to_string(tLinkDuplex)
			);
		}
	}

	return iResult;
}



int ethernet_startup_process(NETWORK_DRIVER_T *ptNetworkDriver)
{
	int iResult;
	int iEthResult;
	NETWORK_STATE_T tState;
	LINK_STATE_T tLinkState;
	LINK_SPEED_T tLinkSpeed;
	LINK_DUPLEX_T tLinkDuplex;
	unsigned long ulDelay;
	DHCP_STATE_T tDhcpState;
	const char *pcName;
	unsigned long ulIp;


	/* Be optimistic. */
	iResult = 0;

	if( ptNetworkDriver->f_is_configured!=0 )
	{
		pcName = ptNetworkDriver->tEthernetPortCfg.acName;

		/* Get the current link state. */
		iEthResult = eth_get_link_status(ptNetworkDriver, &tLinkState, &tLinkSpeed, &tLinkDuplex);
		if( iEthResult!=0 )
		{
			/* Failed to get the link state. */
			ptNetworkDriver->tState = NETWORK_STATE_Error;
		}

		ethernet_cyclic_process(ptNetworkDriver);

		tState = ptNetworkDriver->tState;
		switch(tState)
		{
		case NETWORK_STATE_NoLink:
			/* In this state there is no link.
			 * If a link was detected, move to...
			 *   LinkUp_Ready if the usLinkUpDelay is 0,
			 *   LinkUp_Delay otherwise.
			 */
			if( tLinkState==LINK_STATE_Up )
			{
				uprintf("%s: link up\n", pcName);

				/* Link is up. Check the attributes. */
				iEthResult = check_link_attributes(ptNetworkDriver->tEthernetPortCfg.ulExpectedLinkAttributes, tLinkSpeed, tLinkDuplex);
				if( iEthResult!=0 )
				{
					tState = NETWORK_STATE_Error;
				}
				else
				{
					ulDelay = ptNetworkDriver->tEthernetPortCfg.usLinkUpDelay;
					if( ulDelay==0 )
					{
						tState = NETWORK_STATE_LinkUp_Ready;
					}
					else
					{
						uprintf("%s: link up delay of %dms.\n", pcName, ulDelay);
						systime_handle_start_ms(&(ptNetworkDriver->tLinkUpTimer), ulDelay);
						tState = NETWORK_STATE_LinkUp_Delay;
					}
				}
			}
			break;


		case NETWORK_STATE_LinkUp_Delay:
			/* Delay for the time specified in usLinkUpDelay.
			 * Move to LinkUp_Ready after the delay.
			 */
			if( tLinkState!=LINK_STATE_Up )
			{
				uprintf("%s: link down\n", pcName);
				tState = NETWORK_STATE_NoLink;
			}
			else
			{
				/* Link is up. Delay a while. */
				if( systime_handle_is_elapsed(&(ptNetworkDriver->tLinkUpTimer))!=0 )
				{
					uprintf("%s: link up delay finished.\n", pcName);
					tState = NETWORK_STATE_LinkUp_Ready;
				}
			}
			break;


		case NETWORK_STATE_LinkUp_Ready:
			/* Check if we need DHCP here.
			 * Move to CONSOLE_ETH_STATE_Dhcp if not all parameters are present and DHCP is necessary.
			 * Move to CONSOLE_ETH_STATE_Ready if all parameters are there and no DHCP is needed.
			 */
			if( tLinkState!=LINK_STATE_Up )
			{
				uprintf("%s: link down\n", pcName);
				tState = NETWORK_STATE_NoLink;
			}
			else
			{
				/* Skip DHCP if the IP layer is already configured.
				 * In general only the IP must be set.
				 * NOTE: the net mask and gateway can be 0.
				 */
				ulIp = ptNetworkDriver->tEthernetPortCfg.ulIp;
				if( ulIp!=0U )
				{
					/* IP layer is already configured, skip DHCP. */
					uprintf("%s: the interface is ready, using IP %d.%d.%d.%d\n", pcName, ulIp&0xff, (ulIp>>8U)&0xff, (ulIp>>16U)&0xff, (ulIp>>24U)&0xff);
					tState = NETWORK_STATE_Ready;
				}
				else
				{
					/* start DHCP request */
					uprintf("%s: starting DHCP.\n", pcName);
					dhcp_request(ptNetworkDriver);
					tState = NETWORK_STATE_Dhcp;
				}
			}
			break;


		case NETWORK_STATE_Dhcp:
			/* Wait until the DHCP request is finished. */
			if( tLinkState!=LINK_STATE_Up )
			{
				uprintf("%s: link down\n", pcName);
				tState = NETWORK_STATE_NoLink;
			}
			else
			{
				/* get state */
				tDhcpState = dhcp_getState(ptNetworkDriver);
				switch(tDhcpState)
				{
				case DHCP_STATE_Idle:
					/* the DHCP state must not be idle */
					uprintf("%s: DHCP is in strange state -> ERROR\n", pcName);
					tState = NETWORK_STATE_Error;
					break;

				case DHCP_STATE_Discover:
				case DHCP_STATE_Request:
					/* the DHCP request is still in progress */
					break;

				case DHCP_STATE_Error:
					/* the DHCP request failed */
					uprintf("%s: DHCP failed -> ERROR\n", pcName);
					tState = NETWORK_STATE_Error;
					break;

				case DHCP_STATE_Ok:
					ulIp = ptNetworkDriver->tEthernetPortCfg.ulIp;
					uprintf("%s: DHCP finished with IP %d.%d.%d.%d. The interface is ready.\n", pcName, ulIp&0xff, (ulIp>>8U)&0xff, (ulIp>>16U)&0xff, (ulIp>>24U)&0xff);
					tState = NETWORK_STATE_Ready;
					break;
				}
			}
			break;


		case NETWORK_STATE_Ready:
			/* This interface is up. Now wait for all others. */
			if( tLinkState!=LINK_STATE_Up )
			{
				uprintf("%s: link down\n", pcName);
				tState = NETWORK_STATE_NoLink;
			}
			break;


		case NETWORK_STATE_Ok:
			break;


		case NETWORK_STATE_Error:
			iResult = -1;
			break;
		}

		/* Remember the current state. */
		ptNetworkDriver->tState = tState;
	}

	return iResult;
}



ETHERNET_TEST_RESULT_T ethernet_test_process(NETWORK_DRIVER_T *ptNetworkDriver)
{
	int iEthResult;
	ETHERNET_TEST_RESULT_T tResult;
	ETHERNET_TEST_RESULT_T tSetupResult;
	ECHO_CLIENT_STATE_T tActionResult;
	NETWORK_STATE_T tState;
	LINK_STATE_T tLinkState;
	LINK_SPEED_T tLinkSpeed;
	LINK_DUPLEX_T tLinkDuplex;
	const char *pcName;
	unsigned long ulFlags;
	unsigned long ulFlagLinkDownAllowed;
	unsigned long ulDelay;
	unsigned long ulIp;
	DHCP_STATE_T tDhcpState;
	INTERFACE_FUNCTION_T tFunction;


	/* Be pessimistic. */
	tResult = ETHERNET_TEST_RESULT_Error;

	/* Get the function and flags of the port. */
	tFunction = (INTERFACE_FUNCTION_T)(ptNetworkDriver->tEthernetPortCfg.ulFunction);
	ulFlags = ptNetworkDriver->tEthernetPortCfg.ulFlags;

	/* Get an "in progress" result for the port.
	 * This is the result for the port before it reaches the "link up" state.
	 * A port with the "permanent" flag is always "InProgress".
	 * A port with the function "None" and no "permanent" flag has an "in progress" result of FinishedOk.
	 * A "Server" without the "permanent" flag also has an "in progress" result of FinishedOk.
	 * All other functions have an "in progress" result of "InProgress".
	 */
	tSetupResult = ETHERNET_TEST_RESULT_InProgress;
	if(
		((ulFlags&ETHERNET_PORT_FLAG_Permanent)==0) &&
		((tFunction==INTERFACE_FUNCTION_None) || (tFunction==INTERFACE_FUNCTION_EchoServer))
	)
	{
		tSetupResult = ETHERNET_TEST_RESULT_FinishedOk;
	}

	if( ptNetworkDriver->f_is_configured==0 )
	{
		/* Unconfigured devices are always finished. */
		tResult = ETHERNET_TEST_RESULT_FinishedOk;
	}
	else
	{
		pcName = ptNetworkDriver->tEthernetPortCfg.acName;

		/* Get the current link state. */
		iEthResult = eth_get_link_status(ptNetworkDriver, &tLinkState, &tLinkSpeed, &tLinkDuplex);
		if( iEthResult!=0 )
		{
			/* Failed to get the link state. */
			ptNetworkDriver->tState = NETWORK_STATE_Error;
		}

		ethernet_cyclic_process(ptNetworkDriver);

		ulFlagLinkDownAllowed = ulFlags & ETHERNET_PORT_FLAG_LinkDownAllowed;

		tState = ptNetworkDriver->tState;
		switch(tState)
		{
		case NETWORK_STATE_NoLink:
			if( ulFlagLinkDownAllowed==0 )
			{
				/* During the test process the interface should never end up here. */
				uprintf("%s: invalid state during test process: %d\n", pcName, tState);
				tState = NETWORK_STATE_Error;
			}
			else
			{
				/* In this state there is no link.
				* If a link was detected, move to...
				*   LinkUp_Ready if the usLinkUpDelay is 0,
				*   LinkUp_Delay otherwise.
				*/
				if( tLinkState==LINK_STATE_Up )
				{
					uprintf("%s: link up\n", pcName);

					/* Link is up. Check the attributes. */
					iEthResult = check_link_attributes(ptNetworkDriver->tEthernetPortCfg.ulExpectedLinkAttributes, tLinkSpeed, tLinkDuplex);
					if( iEthResult!=0 )
					{
						tState = NETWORK_STATE_Error;
					}
					else
					{
						ulDelay = ptNetworkDriver->tEthernetPortCfg.usLinkUpDelay;
						if( ulDelay==0 )
						{
							tState = NETWORK_STATE_LinkUp_Ready;
						}
						else
						{
							uprintf("%s: link up delay of %dms.\n", pcName, ulDelay);
							systime_handle_start_ms(&(ptNetworkDriver->tLinkUpTimer), ulDelay);
							tState = NETWORK_STATE_LinkUp_Delay;
						}
					}
				}
				tResult = tSetupResult;
			}
			break;


		case NETWORK_STATE_LinkUp_Delay:
			if( ulFlagLinkDownAllowed==0 )
			{
				/* During the test process the interface should never end up here. */
				uprintf("%s: invalid state during test process: %d\n", pcName, tState);
				tState = NETWORK_STATE_Error;
			}
			else
			{
				/* Delay for the time specified in usLinkUpDelay.
				* Move to LinkUp_Ready after the delay.
				*/
				if( tLinkState!=LINK_STATE_Up )
				{
					uprintf("%s: link down\n", pcName);
					tState = NETWORK_STATE_NoLink;
				}
				else
				{
					/* Link is up. Delay a while. */
					if( systime_handle_is_elapsed(&(ptNetworkDriver->tLinkUpTimer))!=0 )
					{
						uprintf("%s: link up delay finished.\n", pcName);
						tState = NETWORK_STATE_LinkUp_Ready;
					}
				}
				tResult = tSetupResult;
			}
			break;


		case NETWORK_STATE_LinkUp_Ready:
			if( ulFlagLinkDownAllowed==0 )
			{
				/* During the test process the interface should never end up here. */
				uprintf("%s: invalid state during test process: %d\n", pcName, tState);
				tState = NETWORK_STATE_Error;
			}
			else
			{
				/* Check if we need DHCP here.
				* Move to CONSOLE_ETH_STATE_Dhcp if not all parameters are present and DHCP is necessary.
				* Move to CONSOLE_ETH_STATE_Ready if all parameters are there and no DHCP is needed.
				*/
				if( tLinkState!=LINK_STATE_Up )
				{
					uprintf("%s: link down\n", pcName);
					tState = NETWORK_STATE_NoLink;
				}
				else
				{
					/* Skip DHCP if the IP layer is already configured.
					* In general only the IP must be set.
					* NOTE: the net mask and gateway can be 0.
					*/
					ulIp = ptNetworkDriver->tEthernetPortCfg.ulIp;
					if( ulIp!=0U )
					{
						/* IP layer is already configured, skip DHCP. */
						uprintf("%s: the interface is ready, using IP %d.%d.%d.%d\n", pcName, ulIp&0xff, (ulIp>>8U)&0xff, (ulIp>>16U)&0xff, (ulIp>>24U)&0xff);
						tState = NETWORK_STATE_Ready;
					}
					else
					{
						/* start DHCP request */
						uprintf("%s: starting DHCP.\n", pcName);
						dhcp_request(ptNetworkDriver);
						tState = NETWORK_STATE_Dhcp;
					}
				}
				tResult = tSetupResult;
			}
			break;


		case NETWORK_STATE_Dhcp:
			if( ulFlagLinkDownAllowed==0 )
			{
				/* During the test process the interface should never end up here. */
				uprintf("%s: invalid state during test process: %d\n", pcName, tState);
				tState = NETWORK_STATE_Error;
			}
			else
			{
				/* Wait until the DHCP request is finished. */
				if( tLinkState!=LINK_STATE_Up )
				{
					uprintf("%s: link down\n", pcName);
					tState = NETWORK_STATE_NoLink;
				}
				else
				{
					/* get state */
					tDhcpState = dhcp_getState(ptNetworkDriver);
					switch(tDhcpState)
					{
					case DHCP_STATE_Idle:
						/* the DHCP state must not be idle */
						uprintf("%s: DHCP is in strange state -> ERROR\n", pcName);
						tState = NETWORK_STATE_Error;
						break;

					case DHCP_STATE_Discover:
					case DHCP_STATE_Request:
						/* the DHCP request is still in progress */
						break;

					case DHCP_STATE_Error:
						/* the DHCP request failed */
						uprintf("%s: DHCP failed -> ERROR\n", pcName);
						tState = NETWORK_STATE_Error;
						break;

					case DHCP_STATE_Ok:
						ulIp = ptNetworkDriver->tEthernetPortCfg.ulIp;
						uprintf("%s: DHCP finished with IP %d.%d.%d.%d. The interface is ready.\n", pcName, ulIp&0xff, (ulIp>>8U)&0xff, (ulIp>>16U)&0xff, (ulIp>>24U)&0xff);
						tState = NETWORK_STATE_Ready;
						break;
					}
				}
				tResult = tSetupResult;
			}
			break;


		case NETWORK_STATE_Ready:
			/* The link should never go down during a test. */
			if( tLinkState!=LINK_STATE_Up )
			{
				if( ulFlagLinkDownAllowed==0 )
				{
					uprintf("%s: link went down during test.\n", pcName);
					tState = NETWORK_STATE_Error;
				}
				else
				{
					uprintf("%s: link down\n", pcName);
					tState = NETWORK_STATE_NoLink;
					tResult = ETHERNET_TEST_RESULT_InProgress;
				}
			}
			else
			{
				switch(tFunction)
				{
				case INTERFACE_FUNCTION_None:
					if( (ulFlags & ETHERNET_PORT_FLAG_Permanent)==0 )
					{
						/* An interface with no function is always finished if it is not permanent. */
						tResult = ETHERNET_TEST_RESULT_FinishedOk;
					}
					else
					{
						/* The function should keep running. This is nice for tests where the device can be pinged. */
						tResult = ETHERNET_TEST_RESULT_InProgress;
					}
					break;

				case INTERFACE_FUNCTION_EchoServer:
					if( (ulFlags & ETHERNET_PORT_FLAG_Permanent)==0 )
					{
						/* An echo server is always finished if it is not permanent. */
						tResult = ETHERNET_TEST_RESULT_FinishedOk;
					}
					else
					{
						/* The server should keep running. This is necessary for tests between 2 devices. */
						tResult = ETHERNET_TEST_RESULT_InProgress;
					}
					break;

				case INTERFACE_FUNCTION_EchoClient:
					tActionResult = echo_client_action(ptNetworkDriver);
					switch(tActionResult)
					{
					case ECHO_CLIENT_STATE_Idle:
					case ECHO_CLIENT_STATE_WaitingForResponse:
					case ECHO_CLIENT_STATE_PacketReceived:
						/* The client is still running the test. */
						tResult = ETHERNET_TEST_RESULT_InProgress;
						break;

					case ECHO_CLIENT_STATE_Ok:
						/* The test is finished. */
						tState = NETWORK_STATE_Ok;
						tResult = ETHERNET_TEST_RESULT_FinishedOk;
						break;

					case ECHO_CLIENT_STATE_Error:
						/* The test failed. */
						tState = NETWORK_STATE_Error;
						break;
					}
					break;
				}

			}
			break;


		case NETWORK_STATE_Ok:
			tResult = ETHERNET_TEST_RESULT_FinishedOk;
			break;


		case NETWORK_STATE_Error:
			break;
		}

		/* Remember the current state. */
		ptNetworkDriver->tState = tState;
	}

	return tResult;
}
