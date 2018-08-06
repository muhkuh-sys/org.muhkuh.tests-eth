/***************************************************************************
 *   Copyright (C) 2016 by Hilscher GmbH                                   *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "options.h"
#include <string.h>


ROMLOADER_OPTIONS_T g_t_romloader_options;

/* Copy the default settings to ram. */
extern const ROMLOADER_OPTIONS_T t_default_options;

void options_initialize(void)
{
	/* Copy the default options. */
	memcpy(&g_t_romloader_options, &t_default_options, sizeof(ROMLOADER_OPTIONS_T));
}
