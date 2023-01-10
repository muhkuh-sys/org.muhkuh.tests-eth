/***************************************************************************
 *   Copyright (C) 2022 by Christoph Thelen                                *
 *   doc_bacardi@users.sourceforge.net                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "main_standalone.h"

#include <string.h>

#include "asic_types.h"
#include "options.h"
#include "rdy_run.h"
#include "systime.h"
#include "test.h"
#include "uart_standalone.h"
#include "uprintf.h"
#include "version.h"


#include "driver/HETHMAC/ARM_Application/Components/hal_ethmac/Includes/eth_mac_version.h"
#include "driver/HETH2PS/ARM_Application/Components/hal_eth2ps/Includes/eth2ps_version.h"


/*-------------------------------------------------------------------------*/


UART_STANDALONE_DEFINE_GLOBALS

#include "stack/ipv4.h"
static const ETH_PARAMETER_T tEthernetParameter =
{
	.ulMagic = ETHTEST_PARAMETER_BLOCK_MAGIC,
	.ulStructureVersion = ETHTEST_PARAMETER_BLOCK_VERSION,
	.ulVerbose = 1,
	.ulLinkUpTimeout = 0,
	.ulMaximumTransferTime = 0,
	.atPortConfiguration =
		{
			{
				.acName = "CH0",
				.ulInterface = INTERFACE_ETHMAC_INTPHY0,
				.ulFunction = INTERFACE_FUNCTION_EchoServer,
				.ulFlags = 0x00000003,
				.ulExpectedLinkAttributes = 0,
				.ulIp = IP_ADR(192,168,64,240),
				.ulGatewayIp = 0,
				.ulNetmask = IP_ADR(255,255,255,0),
				.ulRemoteIp = IP_ADR(0,0,0,0),
				.ulNumberOfTestPackets = 2048,
				.usLinkUpDelay = 1000,
				.usLocalPort = 1024,
				.usRemotePort = 0,
				.aucMac = { 0x00U,0x02U,0xa2U,0x20U,0x20U,0x04U }
			},
			{
				.acName = "CH1",
				.ulInterface = INTERFACE_ETHMAC_INTPHY1,
				.ulFunction = INTERFACE_FUNCTION_EchoServer,
				.ulFlags = 0x00000003,
				.ulExpectedLinkAttributes = 0,
				.ulIp = IP_ADR(192,168,64,241),
				.ulGatewayIp = 0,
				.ulNetmask = IP_ADR(255,255,255,0),
				.ulRemoteIp = IP_ADR(0,0,0,0),
				.ulNumberOfTestPackets = 2048,
				.usLinkUpDelay = 1000,
				.usLocalPort = 1025,
				.usRemotePort = 0,
				.aucMac = { 0x00U,0x02U,0xa2U,0x20U,0x20U,0x05U }
			}
		}
};



void main_standalone(void)
{
	TEST_RESULT_T tTestResult;
	const ETH_PARAMETER_T *ptTestParams;


	systime_init();
	uart_standalone_initialize();
	options_initialize();
#if ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
	cr7_global_timer_initialize();
#endif

	/* Use the static Ethernet parameters. */
	ptTestParams = &tEthernetParameter;

	uprintf("\f. *** Ethernet test by doc_bacardi@users.sourceforge.net ***\n");
	uprintf("V" VERSION_ALL "\n\n");
	uprintf("This test includes the following software components:\n");
	uprintf(" * HAL ETH MAC V%d.%d.%d.%d\n", ETHMAC_VERSION_MAJOR, ETHMAC_VERSION_MINOR, ETHMAC_VERSION_BUILD, ETHMAC_VERSION_REVISION);
	uprintf(" * HAL ETH 2PS V%d.%d.%d.%d\n", ETH2PS_VERSION_MAJOR, ETH2PS_VERSION_MINOR, ETH2PS_VERSION_BUILD, ETH2PS_VERSION_REVISION);

	/* Get the test parameter. */
	uprintf("Parameters: 0x%08x\n", (unsigned long)ptTestParams);
	uprintf("  Verbose: 0x%08x\n", ptTestParams->ulVerbose);

	/* Copy the configuration to the option structure. */
	memcpy(&g_t_romloader_options.t_ethernet.atPorts, ptTestParams->atPortConfiguration, sizeof(g_t_romloader_options.t_ethernet.atPorts));

	/* Run the test. */
	tTestResult = test(ptTestParams);

	/* Show the result on the SYS LED. */
	if( tTestResult==TEST_RESULT_OK )
	{
		rdy_run_setLEDs(RDYRUN_GREEN);
	}
	else
	{
		rdy_run_setLEDs(RDYRUN_YELLOW);
	}

	/* Stop in an endless loop. */
	uprintf("Stopped.\n");
	while(1)
	{
#if ASIC_TYP==ASIC_TYP_NETX500
		__asm__("NOP");
#elif ASIC_TYP==ASIC_TYP_NETX50
		__asm__("NOP");
#elif ASIC_TYP==ASIC_TYP_NETX10
		__asm__("NOP");
#elif ASIC_TYP==ASIC_TYP_NETX56
		__asm__("NOP");
#elif ASIC_TYP==ASIC_TYP_NETX6
#       error "netX6 is not yet supported"

#elif ASIC_TYP==ASIC_TYP_NETX4000
		__asm__("WFE");
#elif ASIC_TYP==ASIC_TYP_NETX90_MPW
		__asm__("WFE");
#elif ASIC_TYP==ASIC_TYP_NETX90_MPW_APP
		__asm__("WFE");
#elif ASIC_TYP==ASIC_TYP_NETX90
		__asm__("WFE");
#elif ASIC_TYP==ASIC_TYP_NETX90_APP
		__asm__("WFE");
#else
#       error "Invalid ASIC_TYP!"
#endif
	}
}
