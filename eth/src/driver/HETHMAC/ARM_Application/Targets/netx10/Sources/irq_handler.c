#include "common_func_netx10.h"
#include "hal_ethmac.h"
#include "eth_mac.h"

/* interrupt service routine for port 0 */
void VIC_ISR_COM0( void )
{
  eth_mac_isr(0);
}

void irq_init(void)
{
  /* initialize IRQ vector */
  NX10_VIC_InstallIsrVector(NX10_VIC_IRQVectorHandler_C);

  /* Set the vectors for the IRQs */
  NX10_VIC_SetDefaultVector(NX10_VIC_ISR_Default);
  NX10_VIC_SetIrqVector(0, SRT_NX10_vic_int_enable_com0 | MSK_NX10_vic_vect_cntl0_ENABLE, VIC_ISR_COM0);
  NX10_VIC_EnableIrqSources(MSK_NX10_vic_int_enable_com0);
}
