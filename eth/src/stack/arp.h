/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "eth.h"


#ifndef __ARP_H__
#define __ARP_H__


#define ARP_OPCODE_REQUEST	MUS2NUS(0x0001)
#define ARP_OPCODE_REPLY	MUS2NUS(0x0002)


void arp_init(void);

void arp_process_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptEthPkt, unsigned int sizPacket);

void arp_send_ipv4_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPkt, unsigned int sizPacket, unsigned long ulDstIp);

void arp_timer(NETWORK_DRIVER_T *ptNetworkDriver);

#endif	/* __ARP_H__ */
