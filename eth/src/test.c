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


#include "test.h"

#include <string.h>

#include "boot_drv_eth.h"
#include "netx_io_areas.h"
#include "options.h"
#include "rdy_run.h"
#include "systime.h"
#include "uprintf.h"
#include "version.h"

#include "driver/hal_muhkuh.h"


/*-------------------------------------------------------------------------*/


TEST_RESULT_T test(const ETH_PARAMETER_T *ptTestParams)
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
	unsigned long ulIp;
	unsigned long ulTimeout;
	TIMER_HANDLE_T tTimeout;
	int iElapsed;
	unsigned long ulVerbosity;
	unsigned long ulFlags;


	/* Be optimistic. */
	tTestResult = TEST_RESULT_OK;

	ulVerbosity = ptTestParams->ulVerbose;
	if( ulVerbosity!=0 )
	{
		uprintf("Link up timeout: %d\n", ptTestParams->ulLinkUpTimeout);
		uprintf("Maximum transfer time: %d\n", ptTestParams->ulMaximumTransferTime);
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

	/* Loop over all Ethernet ports and prepare the system.
	 * This does things like enabling all clocks.
	 */
	for(uiCnt=0; uiCnt<MAX_NETWORK_INTERFACES; ++uiCnt)
	{
		ptEthCfg = g_t_romloader_options.t_ethernet.atPorts + uiCnt;
		ptNetworkDriver = atNetworkDriver + uiCnt;
		iResult = boot_drv_eth_prepare(uiCnt, ptEthCfg, ptNetworkDriver);
		if( iResult!=0 )
		{
			tTestResult = TEST_RESULT_ERROR_ETH_PREPARE;
			break;
		}
	}
	if( iResult==0 )
	{
		/* Loop over all Ethernet ports and disable them. */
		for(uiCnt=0; uiCnt<MAX_NETWORK_INTERFACES; ++uiCnt)
		{
			ptEthCfg = g_t_romloader_options.t_ethernet.atPorts + uiCnt;
			ptNetworkDriver = atNetworkDriver + uiCnt;
			iResult = boot_drv_eth_disable(uiCnt, ptEthCfg, ptNetworkDriver);
			if( iResult!=0 )
			{
				tTestResult = TEST_RESULT_ERROR_ETH_DISABLE;
				break;
			}
		}
		if( iResult==0 )
		{
			hal_phy_init();

			/* Initialize the XC. */
			hal_pfifo_reset();

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

				ulTimeout = ptTestParams->ulLinkUpTimeout;
				if( ulTimeout!=0 )
				{
					systime_handle_start_ms(&tTimeout, ulTimeout);
				}
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
							tTestResult = TEST_RESULT_ERROR_STARTUP_PROCESS;
							break;
						}
						else
						{
							/* Only check the link state if it must be up all the time. */
							ulFlags  = ptTestParams->atPortConfiguration[uiCnt].ulFlags;
							ulFlags &= ETHERNET_PORT_FLAG_LinkDownAllowed;
							if( ulFlags==0 )
							{
								if( atNetworkDriver[uiCnt].f_is_configured!=0 && atNetworkDriver[uiCnt].tState!=NETWORK_STATE_Ready )
								{
									iAllInterfacesUp = 0;
								}
							}
						}
					}

					if( ulTimeout!=0 )
					{
						iElapsed = systime_handle_is_elapsed(&tTimeout);
						if( iElapsed!=0 )
						{
							uprintf("Timeout waiting for a link.\n");
							tTestResult = TEST_RESULT_ERROR_TIMEOUT_LINK;
							iResult = -1;
						}
					}
				} while( iAllInterfacesUp==0 && iResult==0 );
			}
		}
	}

	if( iResult==0 )
	{
		uprintf("--- All interfaces are up. ---\n");

		/* Expect all ports to be OK. */
		ulTimeout = ptTestParams->ulMaximumTransferTime;
		if( ulTimeout!=0 )
		{
			systime_handle_start_ms(&tTimeout, ulTimeout);
		}
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
					tTestResult = TEST_RESULT_ERROR_TEST_PROCESS;
					break;
				}
			}

			if( ulTimeout!=0 )
			{
				iElapsed = systime_handle_is_elapsed(&tTimeout);
				if( iElapsed!=0 )
				{
					uprintf("The maximum transfer time elapsed.\n");
					tTestResult = TEST_RESULT_ERROR_MAX_TRANSFER_TIME;
					break;
				}
			}
		} while( iAllPortsFinished==0 );
	}

	return tTestResult;
}
