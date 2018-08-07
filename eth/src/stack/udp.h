/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "eth.h"


#ifndef __UDP_H__
#define __UDP_H__


void udp_init(void);

void udp_process_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptEthPkt, unsigned int sizPacket);

void udp_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, ETH2_PACKET_T *ptPkt, unsigned int sizUdpUserData, UDP_ASSOCIATION_T *ptAssoc);

UDP_ASSOCIATION_T *udp_registerPort(unsigned int uiLocalPort, unsigned long ulRemoteIp, unsigned int uiRemotePort, PFN_UDP_RECEIVE_HANDLER pfn_recHandler, void *pvUser);
void udp_unregisterPort(UDP_ASSOCIATION_T *ptAssoc);

#endif	/* __UDP_H__ */
