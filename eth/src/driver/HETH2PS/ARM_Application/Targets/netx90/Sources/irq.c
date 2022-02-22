#include "common_func_netx90.h"
#include "sync.h"
#include "eth_2ps.h"
#include "asic.h"
#include "io.h"
#include "sys.h"
#include "target_defines.h"

#include "hal_adinphy.h"

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

#if TARGET_HW == _NXEB90_SPE_

/**
 * Interrupt service routine for HIF_DIRQ. This external interrupt is connected
 * to the ADIN1100 INT_N pin.
 *
 * \see adinphy_irq_handler()
 */
static void isr_hif_pio( void )
{
  adinphy_irq_handler();
  ArmIrq_Confirm(IRQID_ARM_HIF_PIO);
}

#endif /* TARGET_HW == _NXEB90_SPE_ */

void irq_init(void)
{
#if TARGET_HW == _NXEB90_SPE_
  irq_setup(0, IRQID_ARM_HIF_PIO, isr_hif_pio);
#endif
  irq_setup(0, IRQID_ARM_TRIGGER_LT, sync_irq_handler);
  irq_setup(0, IRQID_ARM_COM0, irq_isr_com0);
  irq_setup(0, IRQID_ARM_COM1, irq_isr_com1);
}
