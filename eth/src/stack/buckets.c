/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008, 2009 by Hilscher GmbH           *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include <stddef.h>
#include <string.h>

#include "buckets.h"
#include "options.h"



unsigned char pucBuckets[BUCKET_SIZE];

unsigned int sizBucket_VaildDataBytes;


void buckets_init(void)
{
	sizBucket_VaildDataBytes = 0;
}


// is at least one bucket free
unsigned int buckets_getFreeBytes(void)
{
	unsigned int sizFree;


	if( sizBucket_VaildDataBytes==0 )
	{
		sizFree = BUCKET_SIZE;
	}
	else
	{
		sizFree = 0;
	}

	return sizFree;
}


unsigned int buckets_getValidBytes(void)
{
	return sizBucket_VaildDataBytes;
}


int buckets_write(unsigned char *pucData, unsigned int sizLen)
{
	int iResult;


	/* Expect failure. */
	iResult = -1;

	/* Are enough buckets free? */
	if( sizLen<=buckets_getFreeBytes() )
	{
		/* Copy the data to the bucket. */
		memcpy(pucBuckets, pucData, sizLen);

		sizBucket_VaildDataBytes = sizLen;

		iResult = 0;
	}

	return iResult;
}


unsigned char *buckets_getPtr(void)
{
	return pucBuckets;
}


int buckets_bytesProcessed(void)
{
	sizBucket_VaildDataBytes = 0;
	return 0;
}

