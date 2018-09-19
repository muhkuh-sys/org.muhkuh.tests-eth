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


	systime_init();
	options_initialize();
#if ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
	cr7_global_timer_initialize();
#endif

	/* Set the verbose mode. */
	s_ulVerbosity = ptTestParams->ulVerbose;
	if( s_ulVerbosity!=0 )
	{
		uprintf("\f. *** Ethernet test by doc_bacardi@users.sourceforge.net ***\n");
		uprintf("V" VERSION_ALL "\n\n");

		/* Get the test parameter. */
		uprintf(". Parameters: 0x%08x\n", (unsigned long)ptTestParams);
		uprintf(".   Verbose: 0x%08x\n", ptTestParams->ulVerbose);
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
				else if( atNetworkDriver[uiCnt].tState!=NETWORK_STATE_Ready )
				{
					iAllInterfacesUp = 0;
				}
			}
		} while( iAllInterfacesUp==0 && iResult==0 );
	}

	tResult = ETHERNET_TEST_RESULT_Error;
	if( iResult==0 )
	{
		uprintf("--- All interfaces are up. ---\n");

		do
		{
			for(uiCnt=0; uiCnt<MAX_NETWORK_INTERFACES; ++uiCnt)
			{
				tResult = ethernet_test_process(atNetworkDriver + uiCnt);
				if( tResult==ETHERNET_TEST_RESULT_Error )
				{
					uprintf("Error on port %d\n", uiCnt);
					break;
				}
			}
		} while( tResult==ETHERNET_TEST_RESULT_InProgress );
	}

	if( tResult==ETHERNET_TEST_RESULT_FinishedOk )
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

