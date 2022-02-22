#include "common_func_netx51.h"
#include "hal_ethmac.h"
#include "eth_mac.h"
#include "regdef_netx51.h"

#if defined(HAL_ETHMAC_TARGET_NX6)
#include "common_func_netx6.h"
#endif

/* interrupt service routine for port 0 */
void VIC_ISR_COM0( void )
{
  eth_mac_isr(0);
}

/* interrupt service routine for port 1 */
void VIC_ISR_COM1( void )
{
  eth_mac_isr(1);
}

#if defined(HAL_ETHMAC_TARGET_NX6)
/* interrupt service routine for HIF */
void VIC_ISR_Hif( void )
{
  uint32_t ulIrqSource;
  uint32_t ulNetx6IrqStatus;

  /* get netX51 HIF IRQ register masked */
  ulIrqSource = NX51_HIF_IOCTRL_GetIrqMasked();

  /* check the PIO47 IRQ flag (Flag of external interrupt status for PIO 47) */
  if( (ulIrqSource & (1<<SRT_NX51_hif_pio_irq_arm_masked_irq_pio47)) != 0 )
  {
    /* read the netx6 IRQ status register */
    NX6_DPM_GetIrq( NX6_DPM_IRQ_STATUS_IRQ_MASKED, &ulNetx6IrqStatus );

    /* check DPM Error IRQ */
    if( (ulNetx6IrqStatus & MSK_NX51_dpm_irq_irq_masked_dpm_err) !=0 ) {
      NX51_IRQFIQ_LOCK
      NX51_AsicCtrl_SetRdyRunLed( NX51_RDYRUN_LED_RED );
      while(1);
    }

    /* check COM IRQs */
    if( (ulNetx6IrqStatus & MSK_NX51_dpm_irq_irq_masked_com0) !=0 ) {
      VIC_ISR_COM0(); /* handle COM0 IRQ */
    }
    if( (ulNetx6IrqStatus & MSK_NX51_dpm_irq_irq_masked_com1) !=0 ) {
      VIC_ISR_COM1(); /* handle COM1 IRQ */
    }
    /* Acknowledge the netx51 pio47 IRQ */
    NX51_HIF_IOCTRL_IrqReset( SRT_NX51_hif_pio_irq_raw_irq_pio47 );

  }
}
#endif /* #if HAL_ETHMAC_TARGET_NX6 */

void irq_init(void)
{
  /* initialize IRQ vector */
  NX51_VIC_InstallIsrVector( NX51_VIC_IRQVectorHandler_C );
  NX51_VIC_SetDefaultVector( NX51_VIC_ISR_Default );

#if defined(HAL_ETHMAC_TARGET_NX51)
  /* Set the vectors for the IRQs */
  NX51_VIC_SetIrqVector( 0, SRT_NX51_vic_int_enable_com0 | MSK_NX51_vic_vect_cntl0_ENABLE, VIC_ISR_COM0 );
  NX51_VIC_SetIrqVector( 1, SRT_NX51_vic_int_enable_com1 | MSK_NX51_vic_vect_cntl1_ENABLE, VIC_ISR_COM1 );

  /* enable IRQs: COM0 highest priority */
  NX51_VIC_EnableIrqSources(  MSK_NX51_vic_int_enable_com0
                             |MSK_NX51_vic_int_enable_com1 );

#elif defined(HAL_ETHMAC_TARGET_NX6)
  /* COM0 and COM1 IRQs are connected to netX51 HIF IRQ */
  NX51_VIC_SetIrqVector( 0, SRT_NX51_vic_int_enable_hif | MSK_NX51_vic_vect_cntl0_ENABLE, VIC_ISR_Hif );

  /* enable IRQs: HIF highest priority */
  NX51_VIC_EnableIrqSources( MSK_NX51_vic_int_enable_hif );

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
                                       1 /* fNetx50PioRegComp (Default) */
                               )
     )
   {
     NX51_AsicCtrl_SetRdyRunLed( NX51_RDYRUN_LED_RED ); /* ERROR: initialize failed */
     while(1) {}
   }

  /* Acknowledge the netx51 pio47 IRQ (in case that an interrupt is active) */
  NX51_HIF_IOCTRL_IrqReset( SRT_NX51_hif_pio_irq_raw_irq_pio47 );

  /* enable HIF PIO 47 interrupt for VIC */
  NX51_HIF_IOCTRL_IrqEnable( SRT_NX51_hif_pio_irq_arm_mask_set_irq_pio47 );

  /* configure netX 6 interrupts (enable Com0, Com1 and DPM Error) */
  NX6_DPM_SetupIrq( ( MSK_NX51_dpm_irq_irq_mask_set_com0
                    | MSK_NX51_dpm_irq_irq_mask_set_com1
                    | MSK_NX51_dpm_irq_irq_mask_set_dpm_err
                    ), 0, 0, 0, 0 );

#endif
}
