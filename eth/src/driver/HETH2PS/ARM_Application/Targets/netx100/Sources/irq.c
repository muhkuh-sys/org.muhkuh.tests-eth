#include "common_func_netx500.h"
#include "eth_2ps.h"
#include "sync.h"
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

void irq_init(void)
{
  irq_setup(0, SRT_NX500_vic_int_enable_com0, irq_isr_com0);
  irq_setup(1, SRT_NX500_vic_int_enable_com1, irq_isr_com1);
  irq_setup(2, SRT_NX500_vic_int_enable_com3, sync_irq_handler);
}
