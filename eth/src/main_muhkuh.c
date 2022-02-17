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


#include "main_muhkuh.h"

#include <string.h>

#include "options.h"
#include "rdy_run.h"
#include "systime.h"
#include "test.h"
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


/*-------------------------------------------------------------------------*/


TEST_RESULT_T main_muhkuh(ETH_PARAMETER_T *ptTestParams)
{
	TEST_RESULT_T tTestResult;


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

	return tTestResult;
}

/*-----------------------------------*/

