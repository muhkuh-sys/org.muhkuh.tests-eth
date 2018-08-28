/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "udp.h"

#include "stack/checksum.h"
#include "stack/ipv4.h"
#include "options.h"



static unsigned short udp_buildChecksum(ETH2_PACKET_T *ptPkt, unsigned int sizUdpPacketSize)
{
	unsigned int uiIpChecksum;
	unsigned int uiUdpChecksum;
	unsigned int uiMyUdpChecksum;


	/* Save the IP checksum. */
	uiIpChecksum = ptPkt->uEth2Data.tIpPkt.tIpHdr.usChecksum;
	/* Use IP checksum field for UDP length. */
	ptPkt->uEth2Data.tIpPkt.tIpHdr.usChecksum = ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usLength;

	/* Save the UDP checksum. */
	uiUdpChecksum = ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usChecksum;
	/* Clear the UDP checksum. */
	ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usChecksum = 0;

	/* Generate the checksum. */
	uiMyUdpChecksum = checksum_add_complement(&ptPkt->uEth2Data.tIpPkt.tIpHdr.ucProtocol, sizUdpPacketSize+11);

	/* Restore the IP checksum and the UDP checksum. */
	ptPkt->uEth2Data.tIpPkt.tIpHdr.usChecksum = (unsigned short)uiIpChecksum;
	ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usChecksum = (unsigned short)uiUdpChecksum;

	return (unsigned short)uiMyUdpChecksum;
}



void udp_init(NETWORK_DRIVER_T *ptNetworkDriver)
{
	UDP_ASSOCIATION_T *ptAssocCnt;
	UDP_ASSOCIATION_T *ptAssocEnd;


	ptAssocCnt = ptNetworkDriver->tNetworkDriverData.tUdpData.atUdpPortAssoc;
	ptAssocEnd = ptAssocCnt + UDP_PORTLIST_MAX;
	while( ptAssocCnt<ptAssocEnd )
	{
		ptAssocCnt->uiLocalPort = 0;
		++ptAssocCnt;
	}
}



void udp_process_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPkt, unsigned int sizPacket)
{
	unsigned int sizUdpPacketSize;
	unsigned int sizTransferedSize;
	unsigned int uiDstPort;
	unsigned int uiMyUdpChecksum;
	unsigned int uiUdpChecksum;
	UDP_ASSOCIATION_T *ptAssocCnt;
	UDP_ASSOCIATION_T *ptAssocEnd;
	PFN_UDP_RECEIVE_HANDLER pfnRecHandler;


	/* check size */
	if( sizPacket>=(sizeof(ETH2_HEADER_T)+sizeof(IPV4_HEADER_T)+sizeof(UDP_HEADER_T)) )
	{
		/* The size field in UDP packet must not exceed the transfered data. */
		sizUdpPacketSize = NUS2MUS(ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usLength);
		sizTransferedSize = sizPacket - sizeof(ETH2_HEADER_T) - sizeof(IPV4_HEADER_T);
		if( sizUdpPacketSize<=sizTransferedSize )
		{
			/* test UDP checksum */
			uiMyUdpChecksum = udp_buildChecksum(ptPkt, sizUdpPacketSize);
			uiUdpChecksum = ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usChecksum;
			if( uiMyUdpChecksum==uiUdpChecksum )
			{
				/* check destination port */
				uiDstPort = ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usDstPort;

				/* loop over all port associations */
				pfnRecHandler = NULL;
				ptAssocCnt = ptNetworkDriver->tNetworkDriverData.tUdpData.atUdpPortAssoc;
				ptAssocEnd = ptAssocCnt + UDP_PORTLIST_MAX;
				while( ptAssocCnt<ptAssocEnd )
				{
					/* does the local port match? */
//					uprintf("%s: check %d %d\n", ptNetworkDriver->tEthernetPortCfg.pcName, NUS2MUS(ptAssocCnt->uiLocalPort), NUS2MUS(uiDstPort));
					if( ptAssocCnt->uiLocalPort==uiDstPort )
					{
						/* yes -> pass packet data to the callback */
						pfnRecHandler = ptAssocCnt->pfn_recHandler;
						break;
					}
					++ptAssocCnt;
				}
				if( pfnRecHandler==NULL )
				{
//					uprintf("%s: drop UPD packet\n", ptNetworkDriver->tEthernetPortCfg.pcName);
				}
				else
				{
					pfnRecHandler(ptNetworkDriver, ptPkt, sizUdpPacketSize-sizeof(UDP_HEADER_T), ptAssocCnt->pvUser);
				}
			}
		}
	}
}



void udp_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPkt, unsigned int sizUdpUserData, UDP_ASSOCIATION_T *ptAssoc)
{
	unsigned int sizPacketSize;
	UDP_ASSOCIATION_T *atUdpPortAssoc;


	/* Is the pointer inside the array? */
	atUdpPortAssoc = ptNetworkDriver->tNetworkDriverData.tUdpData.atUdpPortAssoc;
	if( ptAssoc>=atUdpPortAssoc && ptAssoc<(atUdpPortAssoc+UDP_PORTLIST_MAX) )
	{
		/* Get the size of the complete packet. */
		sizPacketSize = sizeof(UDP_HEADER_T) + sizUdpUserData;

		/* Fill the part of the IP header which will be used for the checksum. */

		/* Set the requested protocol. */
		ptPkt->uEth2Data.tIpPkt.tIpHdr.ucProtocol = IP_PROTOCOL_UDP;
		/* Source IP is my IP. */
		ptPkt->uEth2Data.tIpPkt.tIpHdr.ulSrcIp = ptNetworkDriver->tEthernetPortCfg.ulIp;
		/* Set the requested destination IP. */
		ptPkt->uEth2Data.tIpPkt.tIpHdr.ulDstIp = ptAssoc->ulRemoteIp;


		ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usSrcPort = (unsigned short)ptAssoc->uiLocalPort;
		ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usDstPort = (unsigned short)ptAssoc->uiRemotePort;
		ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usLength = MUS2NUS(sizPacketSize);
		ptPkt->uEth2Data.tIpPkt.uIpData.tUdpPkt.tUdpHdr.usChecksum = udp_buildChecksum(ptPkt, sizPacketSize);

		ipv4_send_packet(ptNetworkDriver, ptPkt, ptAssoc->ulRemoteIp, IP_PROTOCOL_UDP, sizPacketSize);
	}
}



UDP_ASSOCIATION_T *udp_registerPort(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiLocalPort, unsigned long ulRemoteIp, unsigned int uiRemotePort, PFN_UDP_RECEIVE_HANDLER pfn_recHandler, void *pvUser)
{
	UDP_ASSOCIATION_T *ptAssocCnt;
	UDP_ASSOCIATION_T *ptAssocEnd;
	UDP_ASSOCIATION_T *ptAssocHit;


	/* Find a free port table entry. */
	ptAssocCnt = ptNetworkDriver->tNetworkDriverData.tUdpData.atUdpPortAssoc;
	ptAssocEnd = ptAssocCnt + UDP_PORTLIST_MAX;
	ptAssocHit = NULL;
	while( ptAssocCnt<ptAssocEnd )
	{
		if( ptAssocCnt->uiLocalPort==0 )
		{
			/* Found free port association. */
			ptAssocHit = ptAssocCnt;
			ptAssocHit->uiLocalPort = uiLocalPort;
			ptAssocHit->ulRemoteIp = ulRemoteIp;
			ptAssocHit->uiRemotePort = uiRemotePort;
			ptAssocHit->pfn_recHandler = pfn_recHandler;
			ptAssocHit->pvUser = pvUser;
			break;
		}
		++ptAssocCnt;
	}

	return ptAssocHit;
}



void udp_unregisterPort(NETWORK_DRIVER_T *ptNetworkDriver, UDP_ASSOCIATION_T *ptAssoc)
{
	UDP_ASSOCIATION_T *atUdpPortAssoc;


	atUdpPortAssoc = ptNetworkDriver->tNetworkDriverData.tUdpData.atUdpPortAssoc;
	if( ptAssoc>=atUdpPortAssoc && ptAssoc<(atUdpPortAssoc+UDP_PORTLIST_MAX) )
	{
		ptAssoc->uiLocalPort = 0;
	}
}

