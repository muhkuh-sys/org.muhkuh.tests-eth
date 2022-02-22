/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_hif_ioctrl.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_ASIC_CTRL
__USE_HIF_IO_CTRL


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! HIF_IOCTRL DeInitialize
* \description
*   This function resets the HIF IO Control Area to default values.
* \class
*   HIF_IOCTRL Service Class
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX51_HIF_IOCTRL_DeInit ( void )
{
  uint32_t ulAccessKey;

  /* reset HIF IO configuration */
  ulAccessKey = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulAsic_ctrl_access_key = ulAccessKey;
  s_ptHifIoCtrl->ulHif_io_cfg = DFLT_VAL_NX51_hif_io_cfg;

  /* reset HIF PIO configuration */
  s_ptHifIoCtrl->ulHif_pio_cfg = DFLT_VAL_NX51_hif_pio_cfg;

  /* reset HIF PIO OE */
  s_ptHifIoCtrl->aulHif_pio_oe[0] = DFLT_VAL_NX51_hif_pio_oe0;
  s_ptHifIoCtrl->aulHif_pio_oe[1] = DFLT_VAL_NX51_hif_pio_oe1;

  /* reset HIF PIO OUT */
  s_ptHifIoCtrl->aulHif_pio_out[0] = DFLT_VAL_NX51_hif_pio_out0;
  s_ptHifIoCtrl->aulHif_pio_out[1] = DFLT_VAL_NX51_hif_pio_out1;
}

/*****************************************************************************/
/*! HIF_IOCTRL Set IO Configuration
* \description
*   This function sets the HIF IO Configuration.
* \class
*   HIF_IOCTRL Service Class
* \params
*   eDataWidth        [in] 0/1/2: HIF IOs are used as 8/16/32 bit MI, 3: No MI usage. HIF IOs can be used as PIOs or for parallel DPM.
*   eAddrWidth        [in] Select HIF MI address width
*   uEnHifRdyPioMi    [in] 1: Enable HIF_RDY for PIO usage
*   uEnHifSdram       [in] 1: Enable HIF IOs for SDRAM memory interface configuration
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HIF_IOCTRL_SetIoCfg ( NX51_HIF_IOCTRL_HIF_MI_CFG_E      eDataWidth,
                               NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_E eAddrWidth,
                               unsigned int                      uEnHifRdyPioMi,
                               unsigned int                      uEnHifSdram
                             )
{
  uint32_t ulAccessKey;

  /* Plausibility check */
  if( (int)eDataWidth > NX51_HIF_IOCTRL_HIF_MI_CFG_NO_MI )
    return -1;
  if( (int)eAddrWidth > NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_32M )
    return -1;

  /* set HIF IO configuration */
  ulAccessKey = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulAsic_ctrl_access_key = ulAccessKey;

  s_ptHifIoCtrl->ulHif_io_cfg = ((uint32_t)eDataWidth     << SRT_NX51_hif_io_cfg_hif_mi_cfg)
                               |((uint32_t)eAddrWidth     << SRT_NX51_hif_io_cfg_sel_hif_a_width)
                               |((uint32_t)uEnHifRdyPioMi << SRT_NX51_hif_io_cfg_en_hif_rdy_pio_mi)
                               |((uint32_t)uEnHifSdram    << SRT_NX51_hif_io_cfg_en_hif_sdram_mi);

  return 0;
}

/*****************************************************************************/
/*! HIF_IOCTRL Set PIO Configuration
* \description
*   This function sets the HIF PIO Configuration.
* \class
*   HIF_IOCTRL Service Class
* \params
*   eInCtrl           [in] 0/1/2/3: PIOs sampled at POR/CONTINUOUSLY/EN_IN_LO/EN_IN_HI.
*   uSelEnIn          [in] HIF PIO Input sampling enable select
*   uFilterEnIn       [in] HIF PIO Input sampling enable (EN_IN) filter
*   eIrqPio35Cfg      [in] HIF PIO35 configuration (netx56: HIF_D17) IRQ
*   eIrqPio36Cfg      [in] HIF PIO36 configuration (netx56: HIF_D16) IRQ
*   eIrqPio40Cfg      [in] HIF PIO40 configuration (netx51: HIF_D18) IRQ
*   eIrqPio47Cfg      [in] HIF PIO47 configuration (netx51: HIF_DIRQ) IRQ
*   eIrqPio72Cfg      [in] HIF PIO72 configuration (netx51: HIF_D27) IRQ
*   uFilterIrqs       [in] Filtering of HIF PIO inputs for IRQ generation
*   uNetx50PioRegComp [in] netX50 HIF-PIO configuration register compatibility
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HIF_IOCTRL_SetPioCfg ( NX51_HIF_IOCTRL_IN_CTRL_E     eInCtrl,
                                unsigned int                  uSelEnIn,
                                unsigned int                  uFilterEnIn,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio35Cfg,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio36Cfg,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio40Cfg,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio47Cfg,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio72Cfg,
                                unsigned int                  uFilterIrqs,
                                unsigned int                  uNetx50PioRegComp
                             )
{
  /* Plausibility check */
  if( (int)eInCtrl > NX51_HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_EN_IN_HI )
    return -1;
  if( (int)eIrqPio35Cfg > NX51_HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( (int)eIrqPio36Cfg > NX51_HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( (int)eIrqPio40Cfg > NX51_HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( (int)eIrqPio47Cfg > NX51_HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( (int)eIrqPio72Cfg > NX51_HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;

  /* set HIF PIO configuration */
  s_ptHifIoCtrl->ulHif_pio_cfg = ((uint32_t)eInCtrl      << SRT_NX51_hif_pio_cfg_in_ctrl)
                                |((uint32_t)uSelEnIn     << SRT_NX51_hif_pio_cfg_sel_en_in)
                                |((uint32_t)uFilterEnIn  << SRT_NX51_hif_pio_cfg_filter_en_in)
                                |((uint32_t)eIrqPio35Cfg << SRT_NX51_hif_pio_cfg_irq_pio35_cfg)
                                |((uint32_t)eIrqPio36Cfg << SRT_NX51_hif_pio_cfg_irq_pio36_cfg)
                                |((uint32_t)eIrqPio40Cfg << SRT_NX51_hif_pio_cfg_irq_pio40_cfg)
                                |((uint32_t)eIrqPio47Cfg << SRT_NX51_hif_pio_cfg_irq_pio47_cfg)
                                |((uint32_t)eIrqPio72Cfg << SRT_NX51_hif_pio_cfg_irq_pio72_cfg)
                                |((uint32_t)uFilterIrqs  << SRT_NX51_hif_pio_cfg_filter_irqs)
                                |((uint32_t)uNetx50PioRegComp << SRT_NX51_hif_pio_cfg_netX50_pio_reg_comp);

  return 0;
}

/*****************************************************************************/
/*! HIF_IOCTRL Set Line
* \description
*  Write an OUT register.
* \class
*   HIF_IOCTRL
* \params
*   uOutRegNum     [in] HIF PIO_OUT register number
*   ulVal          [in] HIF PIO_OUT register value
* \return
*                                                                            */
/*****************************************************************************/
void NX51_HIF_IOCTRL_SetLine( unsigned int uOutRegNum,
                              uint32_t     ulVal )
{
  if( uOutRegNum<2 )
    s_ptHifIoCtrl->aulHif_pio_out[uOutRegNum]=ulVal;
}

/*****************************************************************************/
/*! HIF_IOCTRL Get Line
* \description
*  Read an OUT register.
* \class
*   HIF_IOCTRL
* \params
*   uOutRegNum     [in] HIF PIO_OUT register number
* \return
*   0 on erroneous
*   Current HIF PIO_OUT register value                                       */
/*****************************************************************************/
uint32_t NX51_HIF_IOCTRL_GetLine( unsigned int uOutRegNum )
{
  if( uOutRegNum>1 )
    return ~0UL;
  else
    return ( s_ptHifIoCtrl->aulHif_pio_out[uOutRegNum] );
}

/*****************************************************************************/
/*! HIF_IOCTRL Set Output enable
* \description
*  Write an OE register.
* \class
*   HIF_IOCTRL
* \params
*   uOeRegNum     [in] HIF PIO_OE register number
*   ulVal         [in] HIF PIO_OE register value
* \return
*                                                                            */
/*****************************************************************************/
void NX51_HIF_IOCTRL_SetOe( unsigned int uOeRegNum,
                            uint32_t     ulVal )
{
  if( uOeRegNum<2 )
    s_ptHifIoCtrl->aulHif_pio_oe[uOeRegNum]=ulVal;
}

/*****************************************************************************/
/*! HIF_IOCTRL Get Output enable
* \description
*  Read an OE register.
* \class
*   HIF_IOCTRL
* \params
*   uOeRegNum     [in] HIF PIO_OE register number
* \return
*   -1 on erroneous
*   Current HIF PIO_OE register value                                        */
/*****************************************************************************/
uint32_t NX51_HIF_IOCTRL_GetOe( unsigned int uOeRegNum )
{
  if( uOeRegNum>1 )
    return ~0UL;
  else
    return ( s_ptHifIoCtrl->aulHif_pio_oe[uOeRegNum] );
}

/*****************************************************************************/
/*! HIF_IOCTRL Get In
* \description
*  Read an IN register.
* \class
*   HIF_IOCTRL
* \params
*   uInRegNum     [in] HIF PIO_IN register number
* \return
*   0 on erroneous
*   Current HIF PIO_IN register value                                        */
/*****************************************************************************/
uint32_t NX51_HIF_IOCTRL_GetIn( unsigned int uInRegNum )
{
  if( uInRegNum>1 )
    return 0;
  else
    return ( s_ptHifIoCtrl->aulHif_pio_in[uInRegNum] );
}

/*****************************************************************************/
/*! HIF_IOCTRL Interrupt Enable
* \description
*   Enable interrupt request for the selected HIF PIO.
* \class
*   HIF_IOCTRL
* \params
*   ulPioNum          [in]   Selected HIF PIO
* \return
*                                                                            */
/*****************************************************************************/
void NX51_HIF_IOCTRL_IrqEnable( uint32_t ulPioNum )
{
  if( ulPioNum<=SRT_NX51_hif_pio_irq_arm_mask_set_irq_pio72 )
    s_ptHifIoCtrl->ulHif_pio_irq_arm_mask_set |= (uint32_t)1 << ulPioNum;
}

/*****************************************************************************/
/*! HIF_IOCTRL Interrupt Disable
* \description
*   Disable interrupt request for the selected PIO.
* \class
*   HIF_IOCTRL
* \params
*   ulPioNum          [in]   Selected HIF PIO
* \return
*                                                                            */
/*****************************************************************************/
void NX51_HIF_IOCTRL_IrqDisable( uint32_t ulPioNum )
{
  if( ulPioNum<=SRT_NX51_hif_pio_irq_arm_mask_reset_irq_pio72 )
    s_ptHifIoCtrl->ulHif_pio_irq_arm_mask_reset |= (uint32_t)1 << ulPioNum;
}

/*****************************************************************************/
/*! HIF_IOCTRL Get Interrupt
* \description
*   Read interrupt status of the selected HIF PIO.
* \class
*   HIF_IOCTRL
* \params
*   ulPioNum          [in]   Selected HIF PIO
* \return
*   0 on erroneous
*   Current HIF PIO_IRQ_ARM_MASKED value of selected HIF_PIO                 */
/*****************************************************************************/
int NX51_HIF_IOCTRL_GetIrq( uint32_t ulPioNum )
{
  if( ulPioNum>SRT_NX51_hif_pio_irq_arm_masked_irq_pio72 )
    return 0;
  else
    return (s_ptHifIoCtrl->ulHif_pio_irq_arm_masked & ((uint32_t)1 << ulPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! HIF_IOCTRL Get Interrupt Masked
* \description
*   Get complete interrupt masked status.
* \class
*   HIF_IOCTRL
* \params
* \return
*   Current HIF PIO_IRQ_ARM_MASKED value                                     */
/*****************************************************************************/
uint32_t NX51_HIF_IOCTRL_GetIrqMasked( void )
{
  return s_ptHifIoCtrl->ulHif_pio_irq_arm_masked;
}

/*****************************************************************************/
/*! HIF_IOCTRL Interrupt Reset
* \description
*   Writes 1 to reset the interrupt of the selected HIF PIO.
* \class
*   HIF_IOCTRL
* \params
*   ulPioNum          [in]   Selected PIO
* \return
*                                                                            */
/*****************************************************************************/
void NX51_HIF_IOCTRL_IrqReset( uint32_t ulPioNum )
{
  if( ulPioNum<=SRT_NX51_hif_pio_irq_raw_irq_pio72 )
    s_ptHifIoCtrl->ulHif_pio_irq_raw = (uint32_t)1 << ulPioNum;
}

