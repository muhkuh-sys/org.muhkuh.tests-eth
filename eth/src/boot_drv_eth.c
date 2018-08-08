#include "boot_drv_eth.h"

#include "stack/buckets.h"
#include "stack/arp.h"
#include "stack/dhcp.h"
#include "stack/eth.h"
#include "stack/icmp.h"
#include "stack/ipv4.h"
#include "options.h"
#include "uprintf.h"


static void process_echo_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvData, unsigned int sizPacket, void *pvUser __attribute__((unused)))
{
	ETH2_PACKET_T *ptPkt;
	UDP_ASSOCIATION_T *ptAssoc;


	if( sizPacket>0 )
	{
		/* The user data is the pointer to the network driver. */
		ptAssoc = ptNetworkDriver->ptEchoUdpAssoc;

		/* Cast the data to a eth2 packet. */
		ptPkt = (ETH2_PACKET_T*)pvData;

		/* Maybe do something with the data, like an XOR? */

		/* Send the data back. */
		ptAssoc->ulRemoteIp = ptPkt->uEth2Data.tIpPkt.tIpHdr.ulSrcIp;
		ptAssoc->uiRemotePort = ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usSrcPort;
		udp_send_packet(ptNetworkDriver, ptPkt, sizPacket, ptAssoc);
	}
}



int boot_drv_eth_init(NETWORK_DRIVER_T *ptNetworkDriver, INTERFACE_T tInterface, const char *pcName)
{
	int iResult;


	uprintf("%s: Initializing interface...\n", pcName);

	iResult = -1;
	switch(tInterface)
	{
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
		ipv4_init();
		udp_init();
		dhcp_init(ptNetworkDriver);

		ptNetworkDriver->f_is_configured = 1;
		ptNetworkDriver->pcName = pcName;
		ptNetworkDriver->tState = NETWORK_STATE_NoLink;
		systime_handle_start_ms(&(ptNetworkDriver->tLinkUpTimer), 0);
		systime_handle_start_ms(&(ptNetworkDriver->tEthernetHandlerTimer), 1000);
		ptNetworkDriver->ptEchoUdpAssoc = udp_registerPort(MUS2NUS(53280), IP_ADR(0, 0, 0, 0), 0, process_echo_packet, NULL);

		uprintf("%s: Interface initialized.\n", pcName);
	}

	return iResult;
}



void ethernet_cyclic_process(NETWORK_DRIVER_T *ptNetworkDriver)
{
	NETWORK_STATE_T tState;
	unsigned int uiLinkState;
	unsigned long ulDelay;
	DHCP_STATE_T tDhcpState;
	const char *pcName;
	unsigned long ulIp;


	pcName = ptNetworkDriver->pcName;

	/* Get the current link state. */
	uiLinkState = eth_get_link_status(ptNetworkDriver);

	/* Process waiting packets. */
        eth_process_packet(ptNetworkDriver);

        if( systime_handle_is_elapsed(&(ptNetworkDriver->tEthernetHandlerTimer))!=0 )
        {
                /* process cyclic events here */
                arp_timer(ptNetworkDriver);
                dhcp_timer(ptNetworkDriver);

                systime_handle_start_ms(&(ptNetworkDriver->tEthernetHandlerTimer), 1000);
        }

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

			ulDelay = g_t_romloader_options.t_ethernet.usLinkUpDelay;
			if( ulDelay==0 )
			{
				tState = NETWORK_STATE_LinkUp_Ready;
			}
			else
			{
				uprintf("%s: link up delay of %dms.\n", pcName, ulDelay);
				systime_handle_start_ms(&(ptNetworkDriver->tLinkUpTimer), g_t_romloader_options.t_ethernet.usLinkUpDelay);
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
			ulIp = g_t_romloader_options.t_ethernet.ulIp;
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
				ulIp = g_t_romloader_options.t_ethernet.ulIp;
				uprintf("%s: DHCP finished with IP %d.%d.%d.%d. The interface is ready.\n", pcName, ulIp&0xff, (ulIp>>8U)&0xff, (ulIp>>16U)&0xff, (ulIp>>24U)&0xff);
				tState = NETWORK_STATE_Ready;
				break;
			}
		}
		break;


	case NETWORK_STATE_Ready:
		/* Echo packets until link goes down. */
		if( uiLinkState==0U )
		{
			uprintf("%s: link down\n", pcName);
			tState = NETWORK_STATE_NoLink;
		}
		break;


	case NETWORK_STATE_Error:
		break;
	}

	/* Remember the current state. */
	ptNetworkDriver->tState = tState;
}
