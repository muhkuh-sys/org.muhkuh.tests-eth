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
#include "board.h"
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

const ETH_PARAMETER_T tEthernetParameter __attribute__ ((section(".params"))) =
{
	.ulMagic = ETHTEST_PARAMETER_BLOCK_MAGIC,
	.ulStructureVersion = ETHTEST_PARAMETER_BLOCK_VERSION,
};


void main_standalone(const ETH_PARAMETER_T *ptTestParams)
{
	TEST_RESULT_T tTestResult;
	int iResult;
	EXIT_METHOD_T tExitMethod;


	systime_init();
	uart_standalone_initialize();
	options_initialize();
#if ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
	cr7_global_timer_initialize();
#endif

	uprintf("\f. *** Ethernet test by doc_bacardi@users.sourceforge.net ***\n");
	uprintf("V" VERSION_ALL "\n\n");
	uprintf("This test includes the following software components:\n");
	uprintf(" * HAL ETH MAC V%d.%d.%d.%d\n", ETHMAC_VERSION_MAJOR, ETHMAC_VERSION_MINOR, ETHMAC_VERSION_BUILD, ETHMAC_VERSION_REVISION);
	uprintf(" * HAL ETH 2PS V%d.%d.%d.%d\n", ETH2PS_VERSION_MAJOR, ETH2PS_VERSION_MINOR, ETH2PS_VERSION_BUILD, ETH2PS_VERSION_REVISION);

	/* Get the test parameter. */
	uprintf("Parameters: 0x%08x\n", (unsigned long)ptTestParams);
	uprintf("  Verbose: 0x%08x\n", ptTestParams->ulVerbose);

	/* Parse the exit method. */
	tExitMethod = (EXIT_METHOD_T)(ptTestParams->ulExitMethod);
	iResult = -1;
	switch(tExitMethod)
	{
		case EXIT_METHOD_Stop:
		case EXIT_METHOD_Return:
		case EXIT_METHOD_Reset:
			iResult = 0;
			break;
	}
	if( iResult!=0 )
	{
		uprintf("Invalid exit mode: 0x%08x. Defaulting to 'STOP'.\n");
		tExitMethod = EXIT_METHOD_Stop;
	}

	/* Copy the configuration to the option structure. */
	memcpy(&g_t_romloader_options.t_ethernet.atPorts, ptTestParams->atPortConfiguration, sizeof(g_t_romloader_options.t_ethernet.atPorts));

	iResult = board_initialize();
	if( iResult!=0 )
	{
		uprintf("Failed to initialize the board.\n");
		tTestResult = TEST_RESULT_ERROR_BOARD_INIT;
	}
	else
	{
		/* Run the test. */
		tTestResult = test(ptTestParams);
	}

	/* Show the result on the SYS LED. */
	if( tTestResult==TEST_RESULT_OK )
	{
		rdy_run_setLEDs(RDYRUN_GREEN);
	}
	else
	{
		rdy_run_setLEDs(RDYRUN_YELLOW);
	}


	switch(tExitMethod)
	{
		case EXIT_METHOD_Stop:
			/* Stop in an endless loop. */
			uprintf("Stopped.\n");
			board_stop_cpu();
			break;

		case EXIT_METHOD_Return:
			break;

		case EXIT_METHOD_Reset:
			uprintf("Reset.\n");
			board_reset();
			break;
	}
}
