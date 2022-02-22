/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include <string.h>

#include "arp.h"

#include "stack/ipv4.h"
#include "options.h"
#include "systime.h"



static const unsigned char aucArpFixedHdr[6] =
{
	0x00, 0x01,		/* usHardwareTyp; */
	0x08, 0x00,		/* usProtocolTyp; */
	0x06,			/* ucHardwareSize; */
	0x04			/* ucProtocolSize; */
};



static ARP_CACHE_ENTRY_T *arp_get_cache_entry(NETWORK_DRIVER_T *ptNetworkDriver, unsigned long ulIp)
{
	ARP_CACHE_ENTRY_T *ptCnt;
	ARP_CACHE_ENTRY_T *ptEnd;
	ARP_CACHE_ENTRY_T *ptHit;
	ARP_STATE_T tState;


	/* Loop over the ARP cache. */
	ptCnt = ptNetworkDriver->tNetworkDriverData.auiArpCache;
	ptEnd = ptCnt + ARP_CACHE_MAX_ENTRIES;
	ptHit = NULL;
	while(ptCnt<ptEnd)
	{
		tState = ptCnt->tState;
		/* Is this state somehow used? */
		if( tState!=ARPSTATE_Unused )
		{
			/* Yes, it's used -> check the IP. */
			if( ptCnt->ulIp==ulIp )
			{
				/* IP found. */
				ptCnt->iAge = 0;
				ptHit = ptCnt;
				break;
			}
			else if( ptCnt->iAge<ARP_CACHE_MAX_AGE )
			{
				/* Increment the age. */
				++ptCnt->iAge;
			}
		}

		/* Next entry. */
		++ptCnt;
	}

	return ptHit;
}


static PACKET_QUEUE_ENTRY_T *arp_get_waiting_packet(NETWORK_DRIVER_T *ptNetworkDriver, unsigned long ulIp)
{
	PACKET_QUEUE_ENTRY_T *ptCnt;
	PACKET_QUEUE_ENTRY_T *ptEnd;
	PACKET_QUEUE_ENTRY_T *ptHit;


	/* Loop over all queue entries. */
	ptCnt = ptNetworkDriver->tNetworkDriverData.atPacketQueue;
	ptEnd = ptCnt + ARP_PACKET_QUEUE_MAX_ENTRIES;
	ptHit = NULL;
	while( ptCnt<ptEnd )
	{
		/* Is this entry valid and does the IP match? */
		if( ptCnt->sizPacket!=0 && ptCnt->ulIp==ulIp )
		{
			/* Yes -> entry found. */
			ptHit = ptCnt;
			break;
		}
		/* Try the next entry. */
		++ptCnt;
	}

	return ptHit;
}


static void arp_process_waiting_entries(NETWORK_DRIVER_T *ptNetworkDriver, unsigned long ulNewIp, MAC_ADR_T *ptMac)
{
	PACKET_QUEUE_ENTRY_T *ptEntry;


	/* Look for waiting packets. */
	do
	{
		/* Look for a packet in the queue which was waiting for the new IP. */
		ptEntry = arp_get_waiting_packet(ptNetworkDriver, ulNewIp);
		if( ptEntry!=NULL )
		{
			/* Found a waiting packet -> send it. */
			eth_send_packet(ptNetworkDriver, ptEntry->ptPkt, ptEntry->phPkt, ptEntry->sizPacket, ptMac, ETH2HEADER_TYP_IP);
			/* Mark the queue entry as "processed". */
			ptEntry->sizPacket = 0;
		}
	} while( ptEntry!=NULL );
}



static PACKET_QUEUE_ENTRY_T *arp_add_waiting_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPkt, void *phPkt, unsigned int sizPacket, unsigned long ulDstIp)
{
	PACKET_QUEUE_ENTRY_T *ptCnt;
	PACKET_QUEUE_ENTRY_T *ptEnd;
	PACKET_QUEUE_ENTRY_T *ptHit;


	/* Loop over all queue entries. */
	ptCnt = ptNetworkDriver->tNetworkDriverData.atPacketQueue;
	ptEnd = ptCnt + ARP_PACKET_QUEUE_MAX_ENTRIES;
	ptHit = NULL;
	while( ptCnt<ptEnd )
	{
		/* Is this entry valid and does the IP match? */
		if( ptCnt->sizPacket==0 )
		{
			/* Yes -> entry found. */
			ptHit = ptCnt;
			break;
		}
		/* Try the next entry. */
		++ptCnt;
	}

	if( ptHit!=NULL )
	{
		ptHit->sizPacket = sizPacket;
		ptHit->ulIp = ulDstIp;
		ptHit->ptPkt = ptPkt;
		ptHit->phPkt = phPkt;
	}

	return ptHit;
}



static void arp_set_mac_request(NETWORK_DRIVER_T *ptNetworkDriver, unsigned long ulIp)
{
	ARP_CACHE_ENTRY_T *ptCnt;
	ARP_CACHE_ENTRY_T *ptEnd;
	int iAge;
	int iTopAge;
	ARP_CACHE_ENTRY_T *ptFree;


	/* force update with first free */
	iTopAge = -1;

	/* loop over the ARP cache */
	ptCnt = ptNetworkDriver->tNetworkDriverData.auiArpCache;
	ptEnd = ptCnt + ARP_CACHE_MAX_ENTRIES;
	ptFree = ptCnt;

	while(ptCnt<ptEnd)
	{
		if( ptCnt->tState==ARPSTATE_Valid )
		{
			iAge = ptCnt->iAge;
			if( iAge>iTopAge )
			{
				iTopAge = iAge;
				ptFree = ptCnt;
			}
		}
		else if( ptCnt->tState==ARPSTATE_Unused )
		{
			ptFree = ptCnt;
			break;
		}

		/* next entry */
		++ptCnt;
	}

	ptFree->tState = ARPSTATE_Requesting;
	ptFree->iAge = 0;
	ptFree->ulIp = ulIp;
	ptFree->ulRequestTimeStamp = systime_get_ms();
	ptFree->uiRetryCnt = g_t_romloader_options.t_ethernet.ucArpRetries;
}


static void arp_send_request(NETWORK_DRIVER_T *ptNetworkDriver, unsigned long ulIp)
{
	int iResult;
	ETH2_PACKET_T *ptSendPacket;
	void *phSendPacket;


	/* get a free frame for sending */
	iResult = eth_get_empty_packet(ptNetworkDriver, &ptSendPacket, &phSendPacket);
	if( iResult==0 )
	{
		/* copy common ARP header */
		memcpy(&ptSendPacket->uEth2Data.tArpPkt, aucArpFixedHdr, 6);
		/* this is a reply */
		ptSendPacket->uEth2Data.tArpPkt.usOpcode = ARP_OPCODE_REQUEST;
		/* the sender of the request is me */
		memcpy(ptSendPacket->uEth2Data.tArpPkt.tSrcMacAdr.aucMac, ptNetworkDriver->tEthernetPortCfg.aucMac, 6);
		ptSendPacket->uEth2Data.tArpPkt.ulSrcIpAdr = ptNetworkDriver->tEthernetPortCfg.ulIp;
		/* i'm looking for the MAC */
		memset(&ptSendPacket->uEth2Data.tArpPkt.tDstMacAdr, 0, 6);
		ptSendPacket->uEth2Data.tArpPkt.ulDstIpAdr = ulIp;

		/* send the packet */
		eth_send_packet(ptNetworkDriver, ptSendPacket, phSendPacket, sizeof(ARP_PACKET_T), &g_tBroadcastMac, ETH2HEADER_TYP_ARP);
	}
}



void arp_init(NETWORK_DRIVER_T *ptNetworkDriver)
{
	ARP_CACHE_ENTRY_T *ptCacheCnt;
	ARP_CACHE_ENTRY_T *ptCacheEnd;
	PACKET_QUEUE_ENTRY_T *ptQueueCnt;
	PACKET_QUEUE_ENTRY_T *ptQueueEnd;


	/* invalidate all queue entries */
	ptQueueCnt = ptNetworkDriver->tNetworkDriverData.atPacketQueue;
	ptQueueEnd = ptQueueCnt + ARP_PACKET_QUEUE_MAX_ENTRIES;
	while( ptQueueCnt<ptQueueEnd )
	{
		ptQueueCnt->sizPacket = 0;
		++ptQueueCnt;
	}

	/* invalidate all cache entries */
	ptCacheCnt = ptNetworkDriver->tNetworkDriverData.auiArpCache;
	ptCacheEnd = ptCacheCnt + ARP_CACHE_MAX_ENTRIES;
	while( ptCacheCnt<ptCacheEnd )
	{
		ptCacheCnt->tState = ARPSTATE_Unused;
		++ptCacheCnt;
	}
}


static void arp_process_request(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptEthPkt)
{
	int iResult;
	ETH2_PACKET_T *ptSendPacket;
	void *phSendPacket;
	unsigned long ulReqIp;
	unsigned int uiMacOr;
	int iCnt;


	ulReqIp = ptEthPkt->uEth2Data.tArpPkt.ulDstIpAdr;
	if( ulReqIp==ptNetworkDriver->tEthernetPortCfg.ulIp )
	{
		/* The MAC field must not be filled. */
		uiMacOr = 0;
		for(iCnt=0; iCnt<5; ++iCnt)
		{
			uiMacOr |= ptEthPkt->uEth2Data.tArpPkt.tDstMacAdr.aucMac[iCnt];
		}
		if( uiMacOr==0 )
		{
			/* get a free frame for sending */
			iResult = eth_get_empty_packet(ptNetworkDriver, &ptSendPacket, &phSendPacket);
			if( iResult==0 )
			{
				/* copy common ARP header */
				memcpy(&ptSendPacket->uEth2Data.tArpPkt, aucArpFixedHdr, 6);
				/* this is a reply */
				ptSendPacket->uEth2Data.tArpPkt.usOpcode = ARP_OPCODE_REPLY;
				/* the sender of the reply is me */
				memcpy(ptSendPacket->uEth2Data.tArpPkt.tSrcMacAdr.aucMac, ptNetworkDriver->tEthernetPortCfg.aucMac, 6);
				ptSendPacket->uEth2Data.tArpPkt.ulSrcIpAdr = ptNetworkDriver->tEthernetPortCfg.ulIp;
				/* the receiver of the reply is the sender of the request */
				ptSendPacket->uEth2Data.tArpPkt.tDstMacAdr = ptEthPkt->uEth2Data.tArpPkt.tSrcMacAdr;
				ptSendPacket->uEth2Data.tArpPkt.ulDstIpAdr = ptEthPkt->uEth2Data.tArpPkt.ulSrcIpAdr;

				/* send the packet */
				eth_send_packet(ptNetworkDriver, ptSendPacket, phSendPacket, sizeof(ARP_PACKET_T), &ptEthPkt->tEth2Hdr.tSrcMac, ETH2HEADER_TYP_ARP);
			}
		}
	}
}



static void arp_process_reply(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptEthPkt)
{
	ARP_CACHE_ENTRY_T *ptCacheEntry;
	unsigned long ulSrcIp;
	MAC_ADR_T *ptSrcMac;


	/* Destination IP must be my IP. */
	if( ptEthPkt->uEth2Data.tArpPkt.ulDstIpAdr==ptNetworkDriver->tEthernetPortCfg.ulIp )
	{
		/* The destination MAC address must be my MAC address. */
		if( memcmp(&ptEthPkt->uEth2Data.tArpPkt.tDstMacAdr, ptNetworkDriver->tEthernetPortCfg.aucMac, 6)==0 )
		{
			/* Look for the supplied IP in the "wanted" list. */
			ulSrcIp = ptEthPkt->uEth2Data.tArpPkt.ulSrcIpAdr;
			ptCacheEntry = arp_get_cache_entry(ptNetworkDriver, ulSrcIp);
			if( ptCacheEntry!=NULL )
			{
				if( ptCacheEntry->tState==ARPSTATE_Requesting )
				{
					/* this is the response to one of my requests */
					ptSrcMac = &ptEthPkt->uEth2Data.tArpPkt.tSrcMacAdr;
					ptCacheEntry->tState = ARPSTATE_Valid;
					ptCacheEntry->iAge = 0;
					memcpy(&ptCacheEntry->tMac, ptSrcMac, 6);

					/* process waiting packets */
					arp_process_waiting_entries(ptNetworkDriver, ulSrcIp, ptSrcMac);
				}
			}
		}
	}
}


void arp_process_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptEthPkt, unsigned int sizPacket)
{
	unsigned int uiOpcode;


	/* common checks */
	if( sizPacket>=sizeof(ETH2_HEADER_T)+sizeof(ARP_PACKET_T) )
	{
		/* Do the first 6 bytes match the Ethernet IP ARP? */
		if( memcmp(&ptEthPkt->uEth2Data.tArpPkt, aucArpFixedHdr, 6)==0 )
		{
			uiOpcode = ptEthPkt->uEth2Data.tArpPkt.usOpcode;
			switch( uiOpcode )
			{
			case ARP_OPCODE_REQUEST:
				arp_process_request(ptNetworkDriver, ptEthPkt);
				break;

			case ARP_OPCODE_REPLY:
				arp_process_reply(ptNetworkDriver, ptEthPkt);
				break;

			default:
				break;
			}
		}
	}
}



void arp_send_ipv4_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPkt, void *phPkt, unsigned int sizPacket, unsigned long ulDstIp)
{
	ARP_CACHE_ENTRY_T *ptCacheEntry;
	PACKET_QUEUE_ENTRY_T *ptQueueEntry;
	const MAC_ADR_T *ptMac;
	MAC_ADR_T tMac;


	ptMac = NULL;

	/* is this a broadcast packet? */
	if( ulDstIp==IP_ADR(255, 255, 255, 255) )
	{
		ptMac = &g_tBroadcastMac;
	}
	/* Is this a multicast packet? */
	else if( (ulDstIp&IP_ADR(0xf0, 0x00, 0x00, 0x00))==IP_ADR(0xe0, 0x00, 0x00, 0x00) )
	{
		/* Construct the MAC from the IP. */
		tMac.aucMac[0] = 0x01;
		tMac.aucMac[1] = 0x00;
		tMac.aucMac[2] = 0x5e;
		tMac.aucMac[3] = (unsigned char)((ulDstIp>>16U) & 0xffU);
		tMac.aucMac[4] = (unsigned char)((ulDstIp>> 8U) & 0xffU);
		tMac.aucMac[5] = (unsigned char)( ulDstIp       & 0xffU);

		ptMac = &tMac;
	}
	else
	{
		/* look for the requested IP in the cache */
		ptCacheEntry = arp_get_cache_entry(ptNetworkDriver, ulDstIp);
		if( ptCacheEntry==NULL )
		{
			/* queue packet */
			ptQueueEntry = arp_add_waiting_packet(ptNetworkDriver, ptPkt, phPkt, sizPacket, ulDstIp);
			if( ptQueueEntry!=NULL )
			{
				/* enter the request in the cache */
				arp_set_mac_request(ptNetworkDriver, ulDstIp);
				/* send request for the IP */
				arp_send_request(ptNetworkDriver, ulDstIp);
			}
		}
		else if( ptCacheEntry->tState==ARPSTATE_Requesting )
		{
			/* This IP is already in the request list.
			 * Just add the packet to the queue. */
			arp_add_waiting_packet(ptNetworkDriver, ptPkt, phPkt, sizPacket, ulDstIp);
		}
		else
		{
			/* OK, found the IP. */
			ptMac = &ptCacheEntry->tMac;
		}
	}

	if( ptMac!=NULL )
	{
		eth_send_packet(ptNetworkDriver, ptPkt, phPkt, sizPacket, ptMac, ETH2HEADER_TYP_IP);
	}
}



void arp_timer(NETWORK_DRIVER_T *ptNetworkDriver)
{
	ARP_CACHE_ENTRY_T *ptCnt;
	ARP_CACHE_ENTRY_T *ptEnd;
	int iRes;
	PACKET_QUEUE_ENTRY_T *ptEntry;
	unsigned long ulIp;
	TIMER_HANDLE_T tHandle;


	/* Loop over the ARP cache. */
	ptCnt = ptNetworkDriver->tNetworkDriverData.auiArpCache;
	ptEnd = ptCnt + ARP_CACHE_MAX_ENTRIES;
	while(ptCnt<ptEnd)
	{
		/* Is the entry a request? */
		if( ptCnt->tState==ARPSTATE_Requesting )
		{
			/* Did the request already time out? */
			tHandle.ulStart = ptCnt->ulRequestTimeStamp;
			tHandle.ulDuration = g_t_romloader_options.t_ethernet.usArpTimeout;
			iRes = systime_handle_is_elapsed(&tHandle);
			if( iRes!=0 )
			{
				ulIp = ptCnt->ulIp;

				/* Yes -> is at least one retry left? */
				if( ptCnt->uiRetryCnt>0 )
				{
					/* Yes -> re-send the packet. */
					arp_send_request(ptNetworkDriver, ulIp);
					ptCnt->ulRequestTimeStamp = systime_get_ms();
					--ptCnt->uiRetryCnt;
				}
				else
				{
					/* No -> remove request from list. */
					ptCnt->tState = ARPSTATE_Unused;

					/* Remove all pending packets for this IP. */
					do
					{
						/* Look for a packet in the queue which was waiting for the new IP. */
						ptEntry = arp_get_waiting_packet(ptNetworkDriver, ulIp);
						if( ptEntry!=NULL )
						{
							/* Free the packet. */
							eth_release_packet(ptNetworkDriver, ptEntry->ptPkt, ptEntry->phPkt);
							/* Mark the queue entry as "processed". */
							ptEntry->sizPacket = 0;
						}
					} while( ptEntry!=NULL );
				}
			}
		}

		/* Next entry. */
		++ptCnt;
	}
}

