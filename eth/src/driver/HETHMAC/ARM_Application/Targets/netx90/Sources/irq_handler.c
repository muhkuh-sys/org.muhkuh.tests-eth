#include "hal_armirq.h"
#include "eth_mac.h"
#include "hw_irqid_arm_com.h"

/* interrupt service routine for port 0 */
static void isr_com0( void )
{
  eth_mac_isr(0);
}

/* interrupt service routine for port 1 */
static void isr_com1( void )
{
  eth_mac_isr(1);
}

static void irq_setup(unsigned int uiIrqNo, void(*isr)(void))
{
  ArmIrq_SetIsr(uiIrqNo, (uint32_t)isr, 0);
  ArmIrq_SetEnable(uiIrqNo, true);
}

void irq_init(void)
{
  ArmIrq_Init();
  irq_setup(IRQID_ARM_COM0, isr_com0);
  irq_setup(IRQID_ARM_COM1, isr_com1);
}
