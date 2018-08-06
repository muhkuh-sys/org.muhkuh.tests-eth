/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "checksum.h"


unsigned int checksum_add_complement(void *pvData, unsigned int sizLen)
{
	unsigned char *pucData;
	unsigned long ulChkSum;
	unsigned int uiData;
	unsigned int uiShift;
	unsigned int sizPos;


	pucData = (unsigned char*)pvData;
	ulChkSum = 0;

	sizPos = 0;
	while(sizPos<sizLen)
	{
		uiData = pucData[sizPos];
		++sizPos;
		// uiShift is 0 for even sizPos, 8 for odd sizPos
		// note that sizPos is already incremented, this means for the byte offset:
		// bytes at even offsets are shifted by 8, bytes at odd offsets are not shifted
		uiShift = (sizPos&1) << 3;
		ulChkSum += uiData << uiShift;
	}

	do
	{
		ulChkSum = (ulChkSum&0xffff) + (ulChkSum>>16);
	} while( ulChkSum>=0x010000 );
	ulChkSum ^= 0xffff;

	return ulChkSum;
}

