#include "common_func_netx50.h"
#include "eth_2ps.h"
#include "sync.h"
#include "hal_resources_defines_netx5.h"
#include "asic.h"

extern ETH_T* g_ptEth;

/****************************************************************************************
*  Functions
****************************************************************************************/

/* interrupt service routine for HIF */
static void irq_isr_hif(void)
{
  unsigned long ulIrqSource;
  unsigned long ulNetx5IrqStatus;

  /* get netx50 HIF IRQ register */
  NX50_DPM_GetDpmIrq(&ulIrqSource);

  /* confirm the netx50 pio47 IRQ */
  NX50_DPM_AckDpmIrq(ulIrqSource);

  /* check the global IRQ request flag */
  if( (ulIrqSource & (1<<31)) != 0 )
  {
    /* check the PIO47 IRQ flag (Flag of external interrupt status for PIO47)) */
    if( (ulIrqSource & (1<<27)) != 0 )
    {
      /* read the netx5 IRQ status register */
      NX5_DPM_GetIrq(&ulNetx5IrqStatus);

      /* check netX IRQs */
      if( (ulNetx5IrqStatus & MSK_NX5_dpm_irq_irq_masked_com0) != 0 )
      {
        eth_2ps_irq_handler(g_ptEth, 0, false);
      }

      if( (ulNetx5IrqStatus & MSK_NX5_dpm_irq_irq_masked_com1) != 0 )
      {
        eth_2ps_irq_handler(g_ptEth, 1, false);
      }

      if( (ulNetx5IrqStatus & MSK_NX5_dpm_irq_irq_masked_trigger_lt) != 0 )
      {
        sync_irq_handler();
      }
    }
  }
}

void irq_init( void )
{
  /* configure netx50 DPM IRQ */
  NX50_DPM_ConfigDpmIrq((1UL<<31) /* global enable */
                       |(1UL<<27) /* pio47 */
                       );

  /* enable netX5 interrupts */
  NX5_DPM_SetupIrq( ( MSK_NX5_dpm_irq_irq_mask_com0
                    | MSK_NX5_dpm_irq_irq_mask_com1
                    | MSK_NX5_dpm_irq_irq_mask_trigger_lt
                    | MSK_NX5_dpm_irq_irq_mask_dpm_err
                    ), 0, 0 );

  irq_setup(0, SRT_NX50_vic_int_enable_hif, irq_isr_hif);
}
