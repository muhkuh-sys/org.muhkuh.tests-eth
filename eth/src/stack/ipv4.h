/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include <stddef.h>

#include "eth.h"


#ifndef __IP_H__
#define __IP_H__

#define IP_ADR(ip3,ip2,ip1,ip0) ((unsigned long)((ip0<<24)|(ip1<<16)|(ip2<<8)|ip3))

#define IP_VERSION 0x45

#define IP_PROTOCOL_ICMP 0x01
#define IP_PROTOCOL_UDP 0x11


void ipv4_init(NETWORK_DRIVER_T *ptNetworkDriver);

void ipv4_process_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptEthPkt, unsigned int sizPacket);

void ipv4_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPkt, void *phPkt, unsigned long ulDstIp, unsigned int uiProtocol, unsigned int sizIpUserData);


#endif	/* __IP_H__ */
