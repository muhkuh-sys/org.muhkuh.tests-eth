#include "common_func_netx4000.h"
#include "hw_defines.h"
#include "sync.h"
#include "eth_2ps.h"
#include "asic.h"

#ifndef EXT_MII
#define EXT_MII 0
#endif

extern ETH_T* g_ptEth;

/****************************************************************************************
*  Functions
****************************************************************************************/

/* interrupt service routine for port 0 */
static void irq_isr_com0( void )
{
  eth_2ps_irq_handler(g_ptEth, 0, false);
}


static void irq_isr_com1( void )
{
  eth_2ps_irq_handler(g_ptEth, 1, false);
}

void irq_init(void)
{
#if EXT_MII==0
  irq_setup(0, IRQID_ARM_COM0, irq_isr_com0);
  irq_setup(0, IRQID_ARM_COM1, irq_isr_com1);
  irq_setup(0, IRQID_ARM_TRIGGER_LT0, sync_irq_handler);
#else
  irq_setup(0, IRQID_ARM_COM2, irq_isr_com0);
  irq_setup(0, IRQID_ARM_COM3, irq_isr_com1);
  irq_setup(0, IRQID_ARM_TRIGGER_LT1, sync_irq_handler);
#endif
}
