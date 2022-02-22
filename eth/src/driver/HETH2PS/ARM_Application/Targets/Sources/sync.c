#include "sync.h"
#include "ip_stack_cfg.h"
#include "hal_eth2ps.h"
#include <stdlib.h>
#include <string.h>

/****************************************************************************************
*  Definitions
****************************************************************************************/

typedef struct SYNC_TEST_CFG_Ttag
{
  uint32_t            ulCycleStartNs;
  ETH2PS_CYCLIC_CFG_T tCyclicCfg;
} SYNC_TEST_CFG_T;


#define SYNC_TEST(start,ctrl,pulse,t0,dt0,irq0,en0,pol0,oe0,t1,dt1,irq1,en1,pol1,oe1) {start,{\
    ctrl,pulse,{{t0,dt0,irq0,en0,pol0,oe0},{t1,dt1,irq1,en1,pol1,oe1}}\
}}

/****************************************************************************************
*  Variables
****************************************************************************************/

SYNC_T g_tSync;

static SYNC_TEST_CFG_T s_atSyncTestCases[] =
{
    /*        cyc start, hostCtl, pulse, offset0,    period0, irq0,  trg0,  pol0,  oe0,   offset1,    period1, irq1,  trg1,  pol1,  oe1   */
    SYNC_TEST(0,         false,   100,   1000000000, 500000,  true,  true,  true,  true,  0,          0,       false, false, false, false),
    SYNC_TEST(1,         false,   200,   1000000000, 500000,  true,  true,  false, true,  0,          0,       false, false, false, false),
    SYNC_TEST(2,         false,   100,   0,          500000,  true,  true,  false, false, 0,          0,       false, false, false, false),
    SYNC_TEST(3,         false,   100,   0,          500000,  true,  false, true,  true,  0,          0,       false, false, false, false),
    SYNC_TEST(4,         false,   100,   0,          1000000, false, true,  true,  true,  0,          0,       false, false, false, false),
    SYNC_TEST(5,         true,    100,   0,          500000,  true,  true,  true,  true,  0,          0,       false, false, false, false),
    SYNC_TEST(6,         false,   200,   0,          0,       false, false, false, false, 999999994,  500000,  true,  true,  true,  true ),
    SYNC_TEST(7,         false,   100,   0,          0,       false, false, false, false, 999999994,  500000,  true,  true,  false, true ),
    SYNC_TEST(8,         false,   100,   0,          0,       false, false, false, false, 0,          500000,  true,  true,  false, false),
    SYNC_TEST(9,         false,   100,   0,          0,       false, false, false, false, 0,          500000,  true,  false, true,  true ),
    SYNC_TEST(10,        false,   100,   0,          0,       false, false, false, false, 0,          1000000, false, true,  true,  true ),
    SYNC_TEST(11,        true,    100,   0,          0,       false, false, false, false, 0,          500000,  true,  true,  true,  true ),
    SYNC_TEST(999999999, false,   100,   0,          500000,  true,  true,  true,  true,  251200,     100000,  true,  true,  true,  true ),
    SYNC_TEST(999999999, false,   100,   251200,     100000,  true,  true,  true,  true,  0,          500000,  true,  true,  true,  true ),
    SYNC_TEST(999999999, false,   100,   251200,     100000,  true,  true,  true,  true,  251200,     500000,  true,  true,  true,  true ),
};


static const char* s_pszModule = __FILE__;


/****************************************************************************************
*  Functions
****************************************************************************************/

void sync_init( void )
{
  memset(&g_tSync, 0, sizeof(g_tSync));

  if( ETH2PS_OKAY != Eth2PS_CyclicInitialize(NULL) )
    FATAL_ERROR("%s:%d:Eth2PS_CyclicInitialize() failed!", s_pszModule, __LINE__);
}

void sync_test( void )
{
  unsigned int uTestCase = g_tSync.uTestCase++;

  /* stop cyclic events if running */
  if( uTestCase > 0 )
    Eth2PS_CyclicStop();

  if( uTestCase == (sizeof(s_atSyncTestCases)/sizeof(s_atSyncTestCases[0])) )
  {
    g_tSync.uTestCase = 0;
  }
  else
  {
    /* configure cyclic events */
    if( ETH2PS_OKAY != Eth2PS_CyclicConfig(&s_atSyncTestCases[uTestCase].tCyclicCfg) )
      FATAL_ERROR("%s:%d:Eth2Ps_CyclicConfig() failed!", s_pszModule, __LINE__);

    /* start cyclic events */
    Eth2PS_CyclicStart(s_atSyncTestCases[uTestCase].ulCycleStartNs);
  }
}

void sync_irq_handler( void )
{
  uint32_t ulIrq;

  /* Get interrupt status */
  ulIrq = Eth2PS_CyclicGetCnfIrq();

  /* Check now the different interrupt sources */
  if( (ulIrq & MSK_ETH2PS_SYNC_IRQ_TRG0) != 0 )
  {
    /* increment sync0 counter */
    g_tSync.aulIrqCnt[0]++;
  }

  if( (ulIrq & MSK_ETH2PS_SYNC_IRQ_TRG1) != 0 )
  {
    /* increment sync1 counter */
    g_tSync.aulIrqCnt[1]++;
  }
}


