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

#include "network_interface.h"


#ifndef __TEST_INTERFACE_H__
#define __TEST_INTERFACE_H__

#define IPV4(ip3,ip2,ip1,ip0) ((unsigned long)((ip0<<24)|(ip1<<16)|(ip2<<8)|ip3))

#define ETHTEST_PARAMETER_BLOCK_MAGIC 0x41504152
#define ETHTEST_PARAMETER_BLOCK_VERSION 0x00010000

typedef struct ETH_PARAMETER_STRUCT
{
	unsigned long ulMagic;
	unsigned long ulStructureVersion;
	unsigned long ulVerbose;
	unsigned long ulLinkUpTimeout;
	unsigned long ulMaximumTransferTime;
	ETHERNET_PORT_CONFIGURATION_T atPortConfiguration[MAX_NETWORK_INTERFACES];
} ETH_PARAMETER_T;


typedef enum TEST_RESULT_ENUM
{
	TEST_RESULT_OK = 0,
	// TEST_RESULT_ERROR = 1,
	TEST_RESULT_ERROR_MAX_TRANSFER_TIME = 1,
	TEST_RESULT_ERROR_TEST_PROCESS = 2,
	TEST_RESULT_ERROR_TIMEOUT_LINK = 3,
	TEST_RESULT_ERROR_STARTUP_PROCESS = 4,
	TEST_RESULT_ERROR_ETH_DISABLE = 5,
	TEST_RESULT_ERROR_ETH_PREPARE = 6,
	TEST_RESULT_ERROR_BOARD_INIT = 7
} TEST_RESULT_T;


#endif  /* __TEST_INTERFACE_H__ */
