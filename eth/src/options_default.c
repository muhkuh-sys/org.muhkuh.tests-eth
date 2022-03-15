/***************************************************************************
 *   Copyright (C) 2016 by Hilscher GmbH                                   *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "stack/ipv4.h"
#include "netx_io_areas.h"
#include "options.h"


const ROMLOADER_OPTIONS_T t_default_options =
{
	.t_ethernet =
	{
		.atPorts =
		{
			/* Port 0 */
			{
				.acName = "CH0",
				.ulInterface = INTERFACE_None,
				.ulFunction = INTERFACE_FUNCTION_None,
				.ulFlags = 0,
				.ulIp = IP_ADR(0,0,0,0),
				.ulGatewayIp = 0,
				.ulNetmask = IP_ADR(0,0,0,0),
				.ulRemoteIp = IP_ADR(0,0,0,0),
				.usLinkUpDelay = 1000,
				.usLocalPort = 0,
				.usRemotePort = 0,
				.aucMac = { 0x00U, 0x02U, 0xa2U, 0x20U, 0x20U, 0x00U }
			},
			/* Port 1 */
			{
				.acName = "CH1",
				.ulInterface = INTERFACE_None,
				.ulFunction = INTERFACE_FUNCTION_None,
				.ulFlags = 0,
				.ulIp = IP_ADR(0,0,0,0),
				.ulGatewayIp = 0,
				.ulNetmask = IP_ADR(255,255,255,0),
				.usLinkUpDelay = 1000,
				.usLocalPort = 1025,
				.ulRemoteIp = IP_ADR(192,168,64,20),
				.usRemotePort = 1024,
				.aucMac = { 0x00U, 0x02U, 0xa2U, 0x20U, 0x20U, 0x01U }
			}
		},
		.usArpTimeout = 100,
		.usDhcpTimeout = 3000,
		.ucArpRetries = 10,
		.ucDhcpRetries = 5
	}
};

