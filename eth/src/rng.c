/***************************************************************************
 *   Copyright (C) 2016 by Hilscher GmbH                                   *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "rng.h"

#include "netx_io_areas.h"


void rng_init(void)
{
	HOSTDEF(ptRandomArea);
	HOSTDEF(ptSampleAtPornStatArea);
	HOSTDEF(ptSystimeUcComArea);
	unsigned long ulValue;


	/* Get the sample at porn values and mix them with the current systime to get a good start for the random generator. */
	ulValue  = ptSampleAtPornStatArea->aulSample_at_porn_stat_in[0];
	ulValue ^= ptSampleAtPornStatArea->aulSample_at_porn_stat_in[1];
	ulValue ^= ptSystimeUcComArea->ulSystime_s;
	ulValue ^= ptSystimeUcComArea->ulSystime_ns;
	ptRandomArea->ulRandom_init = ulValue;
}



unsigned long rng_get_value(void)
{
	HOSTDEF(ptRandomArea);


	return ptRandomArea->ulRandom_random;
}

