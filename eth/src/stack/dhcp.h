/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "network_interface.h"

#ifndef __DHCP_H__
#define __DHCP_H__


void dhcp_init(NETWORK_DRIVER_T *ptNetworkDriver);

DHCP_STATE_T dhcp_getState(NETWORK_DRIVER_T *ptNetworkDriver);

void dhcp_request(NETWORK_DRIVER_T *ptNetworkDriver);

void dhcp_timer(NETWORK_DRIVER_T *ptNetworkDriver);


#endif	/* __DHCP_H__ */
