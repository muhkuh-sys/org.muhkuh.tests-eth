/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include <string.h>

#include "stack/eth.h"

#include "stack/arp.h"
#include "stack/ipv4.h"
#include "options.h"
#include "rdy_run.h"
#include "uprintf.h"

#define CFG_DEBUGMSG 0
#if CFG_DEBUGMSG==1
static const unsigned long ul_networking_eth = 0xffffffff;
#       include "uprintf.h"

#       define DEBUGZONE(n)  (ul_networking_eth&(0x00000001<<(n)))

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

#       define DEBUGMSG(cond,...) ((void)((cond)?(uprintf(__VA_ARGS__)),1:0))
#else
#       define DEBUGMSG(cond,...) ((void)0)
#endif


const MAC_ADR_T g_tBroadcastMac =
{
	.aucMac = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

const MAC_ADR_T g_tEmptyMac =
{
	.aucMac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};



void eth_process_one_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, void *phPacket, unsigned int sizPacket)
{
	ETH2_PACKET_T *ptPacket;
	unsigned int uiEth2Typ;


	ptPacket = (ETH2_PACKET_T *)pvPacket;

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
			ipv4_process_packet(ptNetworkDriver, ptPacket, sizPacket);
			break;

		case ETH2HEADER_TYP_ARP:
			arp_process_packet(ptNetworkDriver, ptPacket, sizPacket);
			break;

		default:
			DEBUGMSG(ZONE_VERBOSE, "[ETH] unknown packet! type 0x%04x\n", uiEth2Typ);
			break;
		}
	}

	ptNetworkDriver->tNetworkIf.pfnReleasePacket(ptNetworkDriver, ptPacket, phPacket);
}



void eth_process_packets(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int sizNetworkDriver)
{
	NETWORK_DRIVER_T *ptCnt;
	NETWORK_DRIVER_T *ptEnd;


	/* Loop over all drivers. */
	ptCnt = ptNetworkDriver;
	ptEnd = ptNetworkDriver + sizNetworkDriver;
	while( ptCnt<ptEnd )
	{
		/* Process only configured drivers. */
		if( ptCnt->f_is_configured!=0 )
		{
			ptCnt->tNetworkIf.pfnProcessReceivedPackets(ptCnt, ptNetworkDriver, sizNetworkDriver);
		}
		++ptCnt;
	}
}



void eth_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPacket, void *phPacket, unsigned int sizEthUserData, const MAC_ADR_T *ptDstMac, unsigned int uiTyp)
{
	unsigned int sizPacket;


	/* Fill the ETH2 header. */

	/* Set the remote MAC. */
	memcpy(&ptPacket->tEth2Hdr.tDstMac, ptDstMac, sizeof(MAC_ADR_T));
	/* Set my MAC. */
	memcpy(&ptPacket->tEth2Hdr.tSrcMac, ptNetworkDriver->tEthernetPortCfg.aucMac, sizeof(MAC_ADR_T));
	/* Set the packet type. */
	ptPacket->tEth2Hdr.usTyp = (unsigned short)uiTyp;

	/* Get the packet length. */
	sizPacket = sizeof(ETH2_HEADER_T) + sizEthUserData;

	ptNetworkDriver->tNetworkIf.pfnSendPacket(ptNetworkDriver, ptPacket, phPacket, sizPacket);

	DEBUGMSG(ZONE_VERBOSE, "[ETH] sent frame\n");
}



int eth_get_empty_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T **pptPacket, void **pphPacket)
{
	int iResult;
	void *pvEmptyPacket;
	ETH2_PACKET_T *ptEmptyPacket;
	void *phEmptyPacket;


	/* Get a free frame for sending. */
	iResult = ptNetworkDriver->tNetworkIf.pfnGetEmptyPacket(ptNetworkDriver, &pvEmptyPacket, &phEmptyPacket);
	if( iResult!=0 )
	{
		DEBUGMSG(ZONE_ERROR, "[ETH] failed to get empty frame!\n");
	}
	else
	{
		ptEmptyPacket = (ETH2_PACKET_T *)pvEmptyPacket;

		*pptPacket = ptEmptyPacket;
		*pphPacket = phEmptyPacket;
	}

	return iResult;
}



void eth_release_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPacket, void *phPacket)
{
	ptNetworkDriver->tNetworkIf.pfnReleasePacket(ptNetworkDriver, ptPacket, phPacket);
}



int eth_get_link_status(NETWORK_DRIVER_T *ptNetworkDriver, LINK_STATE_T *ptLinkState, LINK_SPEED_T *ptLinkSpeed, LINK_DUPLEX_T *ptLinkDuplex)
{
	return ptNetworkDriver->tNetworkIf.pfnGetLinkStatus(ptNetworkDriver, ptLinkState, ptLinkSpeed, ptLinkDuplex);
}
