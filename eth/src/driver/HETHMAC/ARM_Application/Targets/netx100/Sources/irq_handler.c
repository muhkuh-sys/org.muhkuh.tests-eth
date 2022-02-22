#include "common_func_netx500.h"
#include "hal_ethmac.h"
#include "eth_mac.h"

/* interrupt service routine for port 0 */
static void VIC_ISR_COM0(void)
{
  eth_mac_isr(0);
}

/* interrupt service routine for port 1 */
static void VIC_ISR_COM1(void)
{
  eth_mac_isr(1);
}

/* interrupt service routine for port 2 */
static void VIC_ISR_COM2(void)
{
  eth_mac_isr(2);
}

/* interrupt service routine for port 3 */
static void VIC_ISR_COM3(void)
{
  eth_mac_isr(3);
}

void irq_init(void)
{
  /* initialize IRQ vector */
  NX500_VIC_InstallIsrVector(NX500_VIC_IRQVectorHandler_C);

  /* Set the vectors for the IRQs */
  NX500_VIC_SetDefaultVector(NX500_VIC_ISR_Default);
  NX500_VIC_SetIrqVector(0, SRT_NX500_vic_int_enable_com0 | MSK_NX500_vic_vect_cntl0_ENABLE, VIC_ISR_COM0);
  NX500_VIC_SetIrqVector(1, SRT_NX500_vic_int_enable_com1 | MSK_NX500_vic_vect_cntl1_ENABLE, VIC_ISR_COM1);
  NX500_VIC_SetIrqVector(2, SRT_NX500_vic_int_enable_com2 | MSK_NX500_vic_vect_cntl0_ENABLE, VIC_ISR_COM2);
  NX500_VIC_SetIrqVector(3, SRT_NX500_vic_int_enable_com3 | MSK_NX500_vic_vect_cntl1_ENABLE, VIC_ISR_COM3);

  /* enable IRQs */
  NX500_VIC_EnableIrqSources(MSK_NX500_vic_int_enable_com0
                            |MSK_NX500_vic_int_enable_com1
                            |MSK_NX500_vic_int_enable_com2
                            |MSK_NX500_vic_int_enable_com3);
}
