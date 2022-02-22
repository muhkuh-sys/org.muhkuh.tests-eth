#include "common_func_netx50.h"
#include "hal_ethmac.h"
#include "eth_mac.h"

#ifdef TARGET_NETX5
#include "dpm_func_netx50.h"
#include "common_func_netx5.h"
#include "regdef_netx5.h"
#endif

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

#ifdef TARGET_NETX5
/* interrupt service routine for HIF */
void VIC_ISR_HIF( void )
{
  unsigned long ulIrqSource;
  unsigned long ulNetx5IrqStatus;

  /* get netx50 HIF IRQ register */
  NX50_DPM_GetDpmIrq ( &ulIrqSource );

  /* check the global IRQ request flag */
  if( (ulIrqSource & (1UL << 31)) != 0 )
  {
    /* check the PIO47 IRQ flag (Flag of external interrupt status for PIO47)) */
    if( (ulIrqSource & (1UL << 27)) != 0 )
    {
      /* read the netx5 IRQ status register */
      NX5_DPM_GetIrq( &ulNetx5IrqStatus );

      /* check COM IRQs */
      if( (ulNetx5IrqStatus & MSK_NX5_dpm_irq_irq_masked_com0) != 0 )
      {
        VIC_ISR_COM0();
      }

      if( (ulNetx5IrqStatus & MSK_NX5_dpm_irq_irq_masked_com1) != 0 )
      {
        VIC_ISR_COM1();
      }

      /* ACK the netx50 pio47 IRQ */
      NX50_DPM_AckDpmIrq( 1<<27 );
    }
    /* ACK the netx50 global IRQ flag */
    NX50_DPM_AckDpmIrq( 1<<31 );
  }
}
#endif

void irq_init(void)
{
  /* initialize IRQ vector */
  NX50_VIC_InstallIsrVector(NX50_VIC_IRQVectorHandler_C);

  /* Set the vectors for the IRQs */
  NX50_VIC_SetDefaultVector(NX50_VIC_ISR_Default);

#ifdef TARGET_NETX5
  /* both netX5 communication irqs are connected to netX50 HIF IRQ */
  NX50_VIC_SetIrqVector( 0, SRT_NX50_vic_int_enable_hif | MSK_NX50_vic_vect_cntl0_ENABLE, VIC_ISR_HIF );

  /* enable IRQs */
  NX50_VIC_EnableIrqSources( MSK_NX50_vic_int_enable_hif );
#else
  NX50_VIC_SetIrqVector(0, SRT_NX50_vic_int_enable_com0 | MSK_NX50_vic_vect_cntl0_ENABLE, VIC_ISR_COM0);
  NX50_VIC_SetIrqVector(1, SRT_NX50_vic_int_enable_com1 | MSK_NX50_vic_vect_cntl1_ENABLE, VIC_ISR_COM1);

  /* enable IRQs */
  NX50_VIC_EnableIrqSources(MSK_NX50_vic_int_enable_com0
                           |MSK_NX50_vic_int_enable_com1);
#endif
}
