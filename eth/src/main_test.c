/***************************************************************************
 *   Copyright (C) 2018 by Christoph Thelen                                *
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


#include "main_test.h"

#include <string.h>

#include "boot_drv_eth.h"
#include "netx_io_areas.h"
#include "options.h"
#include "rdy_run.h"
#include "systime.h"
#include "uprintf.h"
#include "version.h"

#if ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
#       include "netx4000/cr7_global_timer.h"
#       include "driver/netx4000/drv_eth_xc.h"
#elif ASIC_TYP==ASIC_TYP_NETX500
#       include "driver/netx500/interface.h"
#elif ASIC_TYP==ASIC_TYP_NETX90_MPW || ASIC_TYP==ASIC_TYP_NETX90
#       include "driver/netx90/drv_eth_xc.h"
#endif


static unsigned long s_ulVerbosity;

/*-------------------------------------------------------------------------*/


TEST_RESULT_T test(ETH_PARAMETER_T *ptTestParams)
{
	TEST_RESULT_T tTestResult;
	int iResult;
	NETWORK_DRIVER_T atNetworkDriver[MAX_NETWORK_INTERFACES];
	ETHERNET_PORT_CONFIGURATION_T *ptEthCfg;
	NETWORK_DRIVER_T *ptNetworkDriver;
	unsigned int uiCnt;
	int iAllInterfacesUp;
	ETHERNET_TEST_RESULT_T tResult;
	int iAllPortsFinished;
	int iAllPortsOk;
	unsigned long ulIp;


	systime_init();
	options_initialize();
#if ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
	cr7_global_timer_initialize();
#endif

	uprintf("\f. *** Ethernet test by doc_bacardi@users.sourceforge.net ***\n");
	uprintf("V" VERSION_ALL "\n\n");

	/* Get the test parameter. */
	uprintf("Parameters: 0x%08x\n", (unsigned long)ptTestParams);
	uprintf("  Verbose: 0x%08x\n", ptTestParams->ulVerbose);

	/* Copy the configuration to the option structure. */
	memcpy(&g_t_romloader_options.t_ethernet.atPorts, ptTestParams->atPortConfiguration, sizeof(g_t_romloader_options.t_ethernet.atPorts));

	/* Set the verbose mode. */
	s_ulVerbosity = ptTestParams->ulVerbose;
	if( s_ulVerbosity!=0 )
	{
		for(uiCnt=0; uiCnt<(sizeof(g_t_romloader_options.t_ethernet.atPorts)/sizeof(g_t_romloader_options.t_ethernet.atPorts[0])); uiCnt++)
		{
			ptEthCfg = &(g_t_romloader_options.t_ethernet.atPorts[uiCnt]);
			uprintf("  Port %d\n", uiCnt);
			uprintf("    name: '%s'\n", ptEthCfg->acName);
			uprintf("    interface: %d\n", ptEthCfg->ulInterface);
			uprintf("    function: %d\n", ptEthCfg->ulFunction);
			uprintf("    flags: 0x%08x\n", ptEthCfg->ulFlags);
			ulIp = ptEthCfg->ulIp;
			uprintf("    IP: %d.%d.%d.%d\n", ulIp&0xffU, (ulIp>>8U)&0xffU, (ulIp>>16U)&0xffU, (ulIp>>24U)&0xffU);
			ulIp = ptEthCfg->ulGatewayIp;
			uprintf("    gateway IP: %d.%d.%d.%d\n", ulIp&0xffU, (ulIp>>8U)&0xffU, (ulIp>>16U)&0xffU, (ulIp>>24U)&0xffU);
			ulIp = ptEthCfg->ulNetmask;
			uprintf("    netmask: %d.%d.%d.%d\n", ulIp&0xffU, (ulIp>>8U)&0xffU, (ulIp>>16U)&0xffU, (ulIp>>24U)&0xffU);
			ulIp = ptEthCfg->ulRemoteIp;
			uprintf("    remote IP: %d.%d.%d.%d\n", ulIp&0xffU, (ulIp>>8U)&0xffU, (ulIp>>16U)&0xffU, (ulIp>>24U)&0xffU);
			uprintf("    link up delay: %d\n", ptEthCfg->usLinkUpDelay);
			uprintf("    local port: %d\n", ptEthCfg->usLocalPort);
			uprintf("    remote port: %d\n", ptEthCfg->usRemotePort);
			uprintf("    MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", ptEthCfg->aucMac[0], ptEthCfg->aucMac[1], ptEthCfg->aucMac[2], ptEthCfg->aucMac[3], ptEthCfg->aucMac[4], ptEthCfg->aucMac[5]);
		}
	}

	/* Initialize the XC. */
	pfifo_reset();

	/* Loop over all Ethernet ports and try to configure them. */
	for(uiCnt=0; uiCnt<MAX_NETWORK_INTERFACES; ++uiCnt)
	{
		ptEthCfg = g_t_romloader_options.t_ethernet.atPorts + uiCnt;
		ptNetworkDriver = atNetworkDriver + uiCnt;
		iResult = boot_drv_eth_init(uiCnt, ptEthCfg, ptNetworkDriver);
		if( iResult!=0 )
		{
			break;
		}
	}

	if( iResult==0 )
	{
		/* Setup the PHYs.
		 * FIXME: this is hard-coded for now, but it should depend on the configured interfaces.
		 */
#if ASIC_TYP==ASIC_TYP_NETX90_MPW || ASIC_TYP==ASIC_TYP_NETX90
		setup_phy_internal();
#elif ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
		/* TODO: add this for the netX4000. */
#endif

		do
		{
			/* Be optimistic. */
			iAllInterfacesUp = 1;

			for(uiCnt=0; uiCnt<MAX_NETWORK_INTERFACES; ++uiCnt)
			{
				iResult = ethernet_startup_process(atNetworkDriver + uiCnt);
				if( iResult!=0 )
				{
					uprintf("Error on port %d\n", uiCnt);
					break;
				}
				else if( atNetworkDriver[uiCnt].f_is_configured!=0 && atNetworkDriver[uiCnt].tState!=NETWORK_STATE_Ready )
				{
					iAllInterfacesUp = 0;
				}
			}
		} while( iAllInterfacesUp==0 && iResult==0 );
	}

	iAllPortsOk = 0;
	if( iResult==0 )
	{
		uprintf("--- All interfaces are up. ---\n");

		/* Expect all ports to be OK. */
		iAllPortsOk = 1;

		do
		{
			/* Expect all ports to be finished. */
			iAllPortsFinished = 1;

			for(uiCnt=0; uiCnt<MAX_NETWORK_INTERFACES; ++uiCnt)
			{
				tResult = ethernet_test_process(atNetworkDriver + uiCnt);
				switch(tResult)
				{
				case ETHERNET_TEST_RESULT_InProgress:
					iAllPortsFinished = 0;
					break;

				case ETHERNET_TEST_RESULT_FinishedOk:
					break;

				case ETHERNET_TEST_RESULT_Error:
					uprintf("Error on port %d\n", uiCnt);
					iAllPortsOk = 0;
					break;
				}
			}
		} while( iAllPortsFinished==0 );
	}

	if( iAllPortsOk!=0 )
	{
		tTestResult = TEST_RESULT_OK;
		rdy_run_setLEDs(RDYRUN_GREEN);
	}
	else
	{
		tTestResult = TEST_RESULT_ERROR;
		rdy_run_setLEDs(RDYRUN_YELLOW);
	}

	return tTestResult;
}

/*-----------------------------------*/

