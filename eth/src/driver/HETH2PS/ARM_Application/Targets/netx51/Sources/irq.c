#include "common_func_netx51.h"
#include "regdef_netx51.h"
#include "sync.h"
#include "eth_2ps.h"
#include "asic.h"

extern ETH_T* g_ptEth;

/****************************************************************************************
*  Functions
****************************************************************************************/

/* interrupt service routine for port 0 */
static void irq_isr_com0(void)
{
  eth_2ps_irq_handler(g_ptEth, 0, false);
}


static void irq_isr_com1(void)
{
  eth_2ps_irq_handler(g_ptEth, 1, false);
}


void irq_init( void )
{
  irq_setup(0, SRT_NX51_vic_int_enable_trigger_lt, sync_irq_handler);
  irq_setup(1, SRT_NX51_vic_int_enable_com0      , irq_isr_com0);
  irq_setup(2, SRT_NX51_vic_int_enable_com1      , irq_isr_com1);
}
