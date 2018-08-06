/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include <stddef.h>
#include <string.h>

#include "stack/eth.h"

#include "stack/arp.h"
#include "stack/ipv4.h"
#include "options.h"
#include "rdy_run.h"


#if CFG_DEBUGMSG==1
#       include "uprintf_debug.h"

#       define DEBUGZONE(n)  (g_t_romloader_options.t_debug_settings.ul_networking_eth&(0x00000001<<(n)))

	/*
	 * These defines must match the ZONE_* defines
	 */
#       define DBG_ZONE_ERROR           0
#       define DBG_ZONE_WARNING         1
#       define DBG_ZONE_FUNCTION        2
#       define DBG_ZONE_INIT            3
#       define DBG_ZONE_VERBOSE         7

#       define ZONE_ERROR               DEBUGZONE(DBG_ZONE_ERROR)
#       define ZONE_WARNING             DEBUGZONE(DBG_ZONE_WARNING)
#       define ZONE_FUNCTION            DEBUGZONE(DBG_ZONE_FUNCTION)
#       define ZONE_INIT                DEBUGZONE(DBG_ZONE_INIT)
#       define ZONE_VERBOSE             DEBUGZONE(DBG_ZONE_VERBOSE)

#       define DEBUGMSG(cond,...) ((void)((cond)?(uprintf_debug(__VA_ARGS__)),1:0))
#else
#       define DEBUGMSG(cond,...) ((void)0)
#endif


static NETWORK_IF_T tNetworkIf;
static void *pvDrvEthUser;


const tMacAdr g_tBroadcastMac =
{
	.aucMac = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

const tMacAdr g_tEmptyMac =
{
	.aucMac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};


void eth_init(const NETWORK_IF_T *ptNetworkIf, void *pvUser)
{
	/* Copy the interface structure. */
	memcpy(&tNetworkIf, ptNetworkIf, sizeof(NETWORK_IF_T));
	/* Set the user data pointer. */
	pvDrvEthUser = pvUser;
}


void eth_process_packet(void)
{
	unsigned int sizPacket;
	ETH2_PACKET_T *ptPacket;
	unsigned int uiEth2Typ;


	/* Process incoming frames. */
	ptPacket = tNetworkIf.pfnGetReceivedPacket(&sizPacket, pvDrvEthUser);
	if( ptPacket!=NULL )
	{
//		hexdump_debug(ptPacket, sizPacket);
		/* Check the size of the received packet. */
		if( sizPacket<sizeof(ETH2_HEADER_T) )
		{
			DEBUGMSG(ZONE_VERBOSE, "[ETH] packet is too small! size: %d\n", sizPacket);
		}
		else
		{
			uiEth2Typ = ptPacket->tEth2Hdr.usTyp;
			DEBUGMSG(ZONE_VERBOSE, "[ETH] received packet type 0x%04x, size %d\n", uiEth2Typ, sizPacket);

			switch( uiEth2Typ )
			{
			case ETH2HEADER_TYP_IP:
				ipv4_process_packet(ptPacket, sizPacket);
				break;

			case ETH2HEADER_TYP_ARP:
				arp_process_packet(ptPacket, sizPacket);
				break;

			default:
				DEBUGMSG(ZONE_VERBOSE, "[ETH] unknown packet! type 0x%04x\n", uiEth2Typ);
				break;
			}
		}

		tNetworkIf.pfnReleasePacket(ptPacket, pvDrvEthUser);
	}
#if 0
	else
	{
		tNetworkIf.pfnStatistics(pvDrvEthUser);
	}
#endif
}



void eth_send_packet(ETH2_PACKET_T *ptPacket, unsigned int sizEthUserData, const tMacAdr *ptDstMac, unsigned int uiTyp)
{
	unsigned int sizPacket;


	/* Fill the ETH2 header. */

	/* Set the remote MAC. */
	memcpy(&ptPacket->tEth2Hdr.tDstMac, ptDstMac, sizeof(tMacAdr));
	/* Set my MAC. */
	memcpy(&ptPacket->tEth2Hdr.tSrcMac, g_t_romloader_options.t_ethernet.aucMac, sizeof(tMacAdr));
	/* Set the packet type. */
	ptPacket->tEth2Hdr.usTyp = (unsigned short)uiTyp;

	/* Get the packet length. */
	sizPacket = sizeof(ETH2_HEADER_T) + sizEthUserData;

	tNetworkIf.pfnSendPacket(ptPacket, sizPacket, pvDrvEthUser);

	DEBUGMSG(ZONE_VERBOSE, "[ETH] sent frame\n");
}



ETH2_PACKET_T *eth_get_empty_packet(void)
{
	ETH2_PACKET_T *ptEmptyPacket;


	/* Get a free frame for sending. */
	ptEmptyPacket = tNetworkIf.pfnGetEmptyPacket(pvDrvEthUser);
	if( ptEmptyPacket==NULL )
	{
		DEBUGMSG(ZONE_ERROR, "[ETH] failed to get empty frame!\n");
	}

	return ptEmptyPacket;
}



void eth_release_packet(ETH2_PACKET_T *ptPacket)
{
	tNetworkIf.pfnReleasePacket(ptPacket, pvDrvEthUser);
}



unsigned int eth_get_link_status(void)
{
	unsigned int uiLinkStatus;


	uiLinkStatus = tNetworkIf.pfnGetLinkStatus(pvDrvEthUser);
	return uiLinkStatus;
}



void eth_deactivate(void)
{
	tNetworkIf.pfnDeactivate(pvDrvEthUser);
}

