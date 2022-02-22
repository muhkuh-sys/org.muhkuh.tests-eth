#ifndef _SYNC_H
#define _SYNC_H

#include <stdint.h>
#include <stdbool.h>

typedef struct SYNC_Ttag
{
  unsigned int uTestCase;
  uint32_t aulIrqCnt[2];
} SYNC_T;

extern SYNC_T g_tSync;

/****************************************************************************************
*  Functions
****************************************************************************************/

void sync_init( void );
void sync_test( void );
void sync_irq_handler( void );

#endif /* _SYNC_H */
