#include "common_func_netx51.h"
#include "common_func_netx6.h"
#include "regdef_netx51.h"
#include "sync.h"
#include "eth_2ps.h"
#include "asic.h"

extern ETH_T* g_ptEth;

/****************************************************************************************
*  Functions
****************************************************************************************/

static void irq_isr_nx6( void )
{
  uint32_t ulNetx6IrqStatus;

  /* read the netx6 IRQ status register */
  NX6_DPM_GetIrq(NX6_DPM_IRQ_STATUS_IRQ_MASKED, &ulNetx6IrqStatus);

  /* check DPM Error IRQ */
  if( (ulNetx6IrqStatus & MSK_NX51_dpm_irq_irq_masked_dpm_err) != 0 )
  {
    RDYRUN_SetRdyRunLed(RDYRUN_LED_RED);
    while(1) {};
  }

  /* check COM IRQs */
  if( (ulNetx6IrqStatus & MSK_NX51_dpm_irq_irq_masked_com0) != 0 )
  {
    /* handle COM0 IRQ */
    eth_2ps_irq_handler(g_ptEth, 0, false);
  }

  if( (ulNetx6IrqStatus & MSK_NX51_dpm_irq_irq_masked_com1) != 0 )
  {
    /* handle COM1 IRQ */
    eth_2ps_irq_handler(g_ptEth, 1, false);
  }

  if( (ulNetx6IrqStatus & MSK_NX51_dpm_irq_irq_masked_trigger_lt) != 0 )
  {
    /* handle TRIGGER_LT IRQ */
    sync_irq_handler();
  }
}


/* interrupt service routine for HIF */
void irq_isr_hif( void )
{
  uint32_t ulIrqSource;

  /* get netX51 HIF IRQ register masked */
  ulIrqSource = NX51_HIF_IOCTRL_GetIrqMasked();

  /* check the PIO47 IRQ flag (Flag of external interrupt status for PIO 47) */
  if( (ulIrqSource & MSK_NX51_hif_pio_irq_arm_masked_irq_pio47) != 0 )
  {
    /* Acknowledge the netx51 pio47 IRQ */
    NX51_HIF_IOCTRL_IrqReset( SRT_NX51_hif_pio_irq_raw_irq_pio47 );

    /* Handle netX6 IRQs */
    irq_isr_nx6();
  }
}


void irq_init( void )
{
  /* setup netx51 HIF PIO interrupt, connected to HIF PIO 47 */
  if( 0 != NX51_HIF_IOCTRL_SetPioCfg ( NX51_HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_CONTINUOUSLY, /* ulInCtrl */
                                       0, /* fSelEnIn (Default) */
                                       1, /* fFilterEnIn (Default) */
                                       NX51_HIF_IOCTRL_IRQ_PIO_CFG_LOW_LEVEL_ACTIVE, /* ulIrqPio35Cfg (unused) */
                                       NX51_HIF_IOCTRL_IRQ_PIO_CFG_LOW_LEVEL_ACTIVE, /* ulIrqPio36Cfg (unused) */
                                       NX51_HIF_IOCTRL_IRQ_PIO_CFG_LOW_LEVEL_ACTIVE, /* ulIrqPio40Cfg (unused) */
                                       NX51_HIF_IOCTRL_IRQ_PIO_CFG_LOW_LEVEL_ACTIVE, /* ulIrqPio47Cfg */
                                       NX51_HIF_IOCTRL_IRQ_PIO_CFG_LOW_LEVEL_ACTIVE, /* ulIrqPio72Cfg (unused) */
                                       1, /* fFilterIrqs (Default) */
                                       1 /* fNetx50PioRegComp (Default) */ ) )
   {
     RDYRUN_SetRdyRunLed(RDYRUN_LED_RED);
     while(1) {}
   }

  /* Acknowledge the netx51 pio47 IRQ (in case that an interrupt is active) */
  NX51_HIF_IOCTRL_IrqReset( SRT_NX51_hif_pio_irq_raw_irq_pio47 );

  /* enable HIF PIO 47 interrupt for VIC */
  NX51_HIF_IOCTRL_IrqEnable( SRT_NX51_hif_pio_irq_arm_mask_set_irq_pio47 );

  /* configure netX 6 interrupts (enable Com0/1, trigger_lt and DPM Error) */
  NX6_DPM_SetupIrq( ( MSK_NX51_vic_int_enable_trigger_lt
                    | MSK_NX51_dpm_irq_irq_mask_set_com0
                    | MSK_NX51_dpm_irq_irq_mask_set_com1
                    | MSK_NX51_dpm_irq_irq_mask_set_dpm_err
                    ), 0, 0, 0, 0 );

  /* COM0 and COM1 IRQs are connected to netX51 HIF IRQ */
  irq_setup(0, SRT_NX51_vic_int_enable_hif, irq_isr_hif);
}
