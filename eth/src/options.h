/***************************************************************************
 *   Copyright (C) 2016 by Hilscher GmbH                                   *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "boot_drv_eth.h"


typedef struct STRUCT_ROMLOADER_OPTIONS
{
	/* Ethernet settings */
	ETHERNET_CONFIGURATION_T t_ethernet;
} ROMLOADER_OPTIONS_T;


extern ROMLOADER_OPTIONS_T g_t_romloader_options;


void options_initialize(void);


#endif  /* __OPTIONS_H__ */

