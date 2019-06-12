#include "boot_drv_eth.h"

#include <string.h>

#include "stack/buckets.h"
#include "stack/arp.h"
#include "stack/dhcp.h"
#include "stack/eth.h"
#include "stack/icmp.h"
#include "stack/ipv4.h"
#include "options.h"
#include "uprintf.h"


#if ASIC_TYP==ASIC_TYP_NETX90_MPW || ASIC_TYP==ASIC_TYP_NETX90
#       include "driver/netx90/drv_eth_xc.h"
#elif ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
#       include "driver/netx4000/drv_eth_xc.h"
#endif

static void echo_server_process_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvData, unsigned int sizPacket, void *pvUser __attribute__((unused)))
{
	ETH2_PACKET_T *ptPkt;
	UDP_ASSOCIATION_T *ptAssoc;


	if( sizPacket>0 )
	{
		/* The user data is the pointer to the network driver. */
		ptAssoc = ptNetworkDriver->tFunctionHandle.tServer.ptUdpAssoc;

		/* Cast the data to a eth2 packet. */
		ptPkt = (ETH2_PACKET_T*)pvData;

		/* Maybe do something with the data, like an XOR? */

		/* Send the data back. */
		ptAssoc->ulRemoteIp = ptPkt->uEth2Data.tIpPkt.tIpHdr.ulSrcIp;
		ptAssoc->uiRemotePort = ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usSrcPort;
		udp_send_packet(ptNetworkDriver, ptPkt, sizPacket, ptAssoc);
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
	ETH2_PACKET_T *ptPkt;
	unsigned int sizPacket;
	unsigned char *pucDataCnt;
	unsigned char *pucDataEnd;
	unsigned long ulData;
	int iIsElapsed;
	const char *pcName;


	/* Get a shortcut to the handle. */
	ptHandle = &(ptNetworkDriver->tFunctionHandle.tClient);

	pcName = ptNetworkDriver->tEthernetPortCfg.pcName;

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
			ptPkt = ptNetworkDriver->tNetworkIf.pfnGetEmptyPacket(ptNetworkDriver);
			if( ptPkt==NULL )
			{
				uprintf("%s: No free Ethernet packet available.\n", pcName);
				tState = ECHO_CLIENT_STATE_Error;
			}
			else
			{
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
				udp_send_packet(ptNetworkDriver, ptPkt, sizPacket, ptHandle->ptUdpAssoc);

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


	pcName = ptNetworkDriver->tEthernetPortCfg.pcName;

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
	unsigned short usRemotePort;


	usLocalPort = ptEthCfg->usLocalPort;
	ulRemoteIp = ptEthCfg->ulRemoteIp;
	usRemotePort = ptEthCfg->usRemotePort;

	ptHandle = &(ptNetworkDriver->tFunctionHandle.tClient);

	ptHandle->tState = ECHO_CLIENT_STATE_Idle;
	ptHandle->uiPacketsLeft = 4096;
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



int boot_drv_eth_init(unsigned int uiInterfaceIndex, ETHERNET_PORT_CONFIGURATION_T *ptEthCfg, NETWORK_DRIVER_T *ptNetworkDriver)
{
	int iResult;
	const char *pcName;
	INTERFACE_T tInterface;


	iResult = -1;

	uprintf("Processing interface %d.\n", uiInterfaceIndex);

	tInterface = ptEthCfg->tInterface;
	if( tInterface==INTERFACE_None )
	{
		/* This interface is not in use. */
		uprintf("Interface %d is not in use.\n", uiInterfaceIndex);
		memset(ptNetworkDriver, 0, sizeof(NETWORK_DRIVER_T));
		iResult = 0;
	}
	else
	{
		pcName = ptEthCfg->pcName;
		if( pcName==NULL )
		{
			uprintf("ERROR: the name of interface %d is not set.\n", uiInterfaceIndex);
		}
		else
		{
			uprintf("%s: Initializing interface...\n", pcName);

			switch(tInterface)
			{
			case INTERFACE_None:
				break;

			case INTERFACE_INTPHY0:
				/* INTPHY port 0 */
				iResult = drv_eth_xc_initialize(ptNetworkDriver, 0);
				break;

			case INTERFACE_INTPHY1:
				/* INTPHY port 1 */
				iResult = drv_eth_xc_initialize(ptNetworkDriver, 1);
				break;

			case INTERFACE_EXTPHY0:
				/* EXTPHY port 0 */
				iResult = drv_eth_xc_initialize(ptNetworkDriver, 2);
				break;

			case INTERFACE_EXTPHY1:
				/* EXTPHY port 1 */
				iResult = drv_eth_xc_initialize(ptNetworkDriver, 3);
				break;

			case INTERFACE_LVDS0:
				/* LVDS port 0 */
				iResult = drv_eth_xc_initialize_lvds(ptNetworkDriver, 0);
				break;

			case INTERFACE_LVDS1:
				/* LVDS port 1 */
				iResult = drv_eth_xc_initialize_lvds(ptNetworkDriver, 1);
				break;
			}

			if( iResult!=0 )
			{
				uprintf("%s: ERROR: failed to initialize the Ethernet port.\n", pcName);
			}
			else
			{
				buckets_init();
				arp_init(ptNetworkDriver);
				ipv4_init(ptNetworkDriver);
				udp_init(ptNetworkDriver);
				dhcp_init(ptNetworkDriver);

				ptNetworkDriver->f_is_configured = 1;
				memcpy(&(ptNetworkDriver->tEthernetPortCfg), ptEthCfg, sizeof(ETHERNET_PORT_CONFIGURATION_T));
				ptNetworkDriver->tState = NETWORK_STATE_NoLink;
				systime_handle_start_ms(&(ptNetworkDriver->tLinkUpTimer), 0);
				systime_handle_start_ms(&(ptNetworkDriver->tEthernetHandlerTimer), 1000);

				switch( ptEthCfg->tFunction )
				{
				case INTERFACE_FUNCTION_None:
					break;

				case INTERFACE_FUNCTION_EchoServer:
					ptNetworkDriver->tFunctionHandle.tServer.ptUdpAssoc = udp_registerPort(
						ptNetworkDriver,
						MUS2NUS(53280),
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

				uprintf("%s: Interface initialized.\n", pcName);
			}
		}
	}

	return iResult;
}



static void ethernet_cyclic_process(NETWORK_DRIVER_T *ptNetworkDriver)
{
	/* Process waiting packets. */
	eth_process_packet(ptNetworkDriver);

	if( systime_handle_is_elapsed(&(ptNetworkDriver->tEthernetHandlerTimer))!=0 )
	{
		/* process cyclic events here */
		arp_timer(ptNetworkDriver);
		dhcp_timer(ptNetworkDriver);

		systime_handle_start_ms(&(ptNetworkDriver->tEthernetHandlerTimer), 1000);
	}
}



int ethernet_startup_process(NETWORK_DRIVER_T *ptNetworkDriver)
{
	int iResult;
	NETWORK_STATE_T tState;
	unsigned int uiLinkState;
	unsigned long ulDelay;
	DHCP_STATE_T tDhcpState;
	const char *pcName;
	unsigned long ulIp;


	/* Be optimistic. */
	iResult = 0;

	if( ptNetworkDriver->f_is_configured!=0 )
	{
		pcName = ptNetworkDriver->tEthernetPortCfg.pcName;

		/* Get the current link state. */
		uiLinkState = eth_get_link_status(ptNetworkDriver);

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
			if( uiLinkState!=0U )
			{
				uprintf("%s: link up\n", pcName);

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
			break;


		case NETWORK_STATE_LinkUp_Delay:
			/* Delay for the time specified in usLinkUpDelay.
			 * Move to LinkUp_Ready after the delay.
			 */
			if( uiLinkState==0U )
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
			if( uiLinkState==0U )
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
			if( uiLinkState==0U )
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
			if( uiLinkState==0U )
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
	ETHERNET_TEST_RESULT_T tResult;
	ECHO_CLIENT_STATE_T tActionResult;
	NETWORK_STATE_T tState;
	unsigned int uiLinkState;
	const char *pcName;
	unsigned long ulFlags;


	/* Be pessimistic. */
	tResult = ETHERNET_TEST_RESULT_Error;

	if( ptNetworkDriver->f_is_configured==0 )
	{
		/* Unconfigured devices are always finished. */
		tResult = ETHERNET_TEST_RESULT_FinishedOk;
	}
	else
	{
		pcName = ptNetworkDriver->tEthernetPortCfg.pcName;

		/* Get the current link state. */
		uiLinkState = eth_get_link_status(ptNetworkDriver);

		ethernet_cyclic_process(ptNetworkDriver);

		ulFlags = ptNetworkDriver->tEthernetPortCfg.ulFlags;

		tState = ptNetworkDriver->tState;
		switch(tState)
		{
		case NETWORK_STATE_NoLink:
		case NETWORK_STATE_LinkUp_Delay:
		case NETWORK_STATE_LinkUp_Ready:
		case NETWORK_STATE_Dhcp:
			/* During the test process the interface should never end up here. */
			uprintf("%s: invalid state during test process: %d\n", pcName, tState);
			tState = NETWORK_STATE_Error;
			break;


		case NETWORK_STATE_Ready:
			/* The link should never go down during a test. */
			if( uiLinkState==0U )
			{
				uprintf("%s: link went down during test.\n", pcName);
				tState = NETWORK_STATE_Error;
			}
			else
			{
				switch(ptNetworkDriver->tEthernetPortCfg.tFunction)
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
