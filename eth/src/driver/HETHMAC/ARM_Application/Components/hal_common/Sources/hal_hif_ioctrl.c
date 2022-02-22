/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_hif_ioctrl.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_ASIC_CTRL(s_ptAsicCtrl)
HW_PTR_HIF_IO_CTRL(s_ptHifIoCtrl)


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
#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(HIF_IOCTRL_DeInit) ( void )
{
  uint32_t ulAccessKey;

  /* reset HIF IO configuration */
  ulAccessKey = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulAsic_ctrl_access_key = ulAccessKey;
  s_ptHifIoCtrl->ulHif_io_cfg = HW_DFLT_VAL(hif_io_cfg);

  /* reset HIF PIO configuration */
  s_ptHifIoCtrl->ulHif_pio_cfg = HW_DFLT_VAL(hif_pio_cfg);

  /* reset HIF PIO OE */
  s_ptHifIoCtrl->aulHif_pio_oe[0] = HW_DFLT_VAL(hif_pio_oe0);
  s_ptHifIoCtrl->aulHif_pio_oe[1] = HW_DFLT_VAL(hif_pio_oe1);

  /* reset HIF PIO OUT */
  s_ptHifIoCtrl->aulHif_pio_out[0] = HW_DFLT_VAL(hif_pio_out0);
  s_ptHifIoCtrl->aulHif_pio_out[1] = HW_DFLT_VAL(hif_pio_out1);
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
int HALDEF(HIF_IOCTRL_SetIoCfg) ( HIF_IOCTRL_HIF_MI_CFG_E      eDataWidth,
                                 HIF_IOCTRL_SEL_HIF_A_WIDTH_E eAddrWidth,
                                 unsigned int                      uEnHifRdyPioMi,
                                 unsigned int                      uEnHifSdram
                               )
{
  uint32_t ulAccessKey;

  /* Plausibility check */
  if( eDataWidth > HIF_IOCTRL_HIF_MI_CFG_NO_MI )
    return -1;
  if( eAddrWidth > HIF_IOCTRL_SEL_HIF_A_WIDTH_32M )
    return -1;

  /* set HIF IO configuration */
  ulAccessKey = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulAsic_ctrl_access_key = ulAccessKey;

  s_ptHifIoCtrl->ulHif_io_cfg = ((uint32_t)eDataWidth     << HW_SRT(hif_io_cfg_hif_mi_cfg))
                               |((uint32_t)eAddrWidth     << HW_SRT(hif_io_cfg_sel_hif_a_width))
                               |((uint32_t)uEnHifRdyPioMi << HW_SRT(hif_io_cfg_en_hif_rdy_pio_mi))
                               |((uint32_t)uEnHifSdram    << HW_SRT(hif_io_cfg_en_hif_sdram_mi));

  return 0;
}

/*****************************************************************************/
/*! HIF_IOCTRL Set IO Configuration Register
* \description
*   This function sets the HIF IO Configuration Register.
* \class
*   HIF_IOCTRL Service Class
* \params
*   eDataWidth        [in] 0/1/2: HIF IOs are used as 8/16/32 bit MI, 3: No MI usage. HIF IOs can be used as PIOs or for parallel DPM.
*   eAddrWidth        [in] Select HIF MI address width
*   uEnHifRdyPioMi    [in] 1: Enable HIF_RDY for PIO usage
*   uEnHifSdram       [in] 1: Enable HIF IOs for SDRAM memory interface configuration
*   eSelData          [in] 0/1/2/3: 0: HIF/MEM IOs are undriven, 1: Only MEM MI is available, 2: Only HIF MI is available, 
*                                   3: HIF_D0..15 for HIF MI and HIF_D16..31 for MEM MI available.
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(HIF_IOCTRL_SetIoCfgReg)( HIF_IOCTRL_HIF_MI_CFG_E      eDataWidth,
                                   HIF_IOCTRL_SEL_HIF_A_WIDTH_E eAddrWidth,
                                   unsigned int                        uEnHifRdyPioMi,
                                   unsigned int                        uEnHifSdram,
                                   HIF_IOCTRL_SEL_DATA_E        eSelData
                                 )
{
  uint32_t ulAccessKey;

#if defined(__NETX4000)
  /* select mem_d IOs for HIF or MEM usage */
  uint32_t ulSelMemData = 0;
  switch(eSelData)
  {
  case HIF_IOCTRL_SEL_DATA_DISABLE:    ulSelMemData = 0x0; break;
  case HIF_IOCTRL_SEL_DATA_MEM_ONLY:   ulSelMemData = 0xf; break;
  case HIF_IOCTRL_SEL_DATA_HIF_ONLY:   ulSelMemData = 0x0; break;
  case HIF_IOCTRL_SEL_DATA_HIF_MEM:    ulSelMemData = 0xc; break;
  default: return -1;
  }
#endif

  /* Plausibility check */
  if( eDataWidth > HIF_IOCTRL_HIF_MI_CFG_NO_MI )
    return -1;
  if( eAddrWidth > HIF_IOCTRL_SEL_HIF_A_WIDTH_32M )
    return -1;

  /* set HIF IO configuration */
  ulAccessKey = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulAsic_ctrl_access_key = ulAccessKey;

  s_ptHifIoCtrl->ulHif_io_cfg = ((uint32_t)eDataWidth     << HW_SRT(hif_io_cfg_hif_mi_cfg))
                               |((uint32_t)eAddrWidth     << HW_SRT(hif_io_cfg_sel_hif_a_width))
                               |((uint32_t)uEnHifRdyPioMi << HW_SRT(hif_io_cfg_en_hif_rdy_pio_mi))
                               |((uint32_t)uEnHifSdram    << HW_SRT(hif_io_cfg_en_hif_sdram_mi))
#if defined(__NETX4000)
                               |(ulSelMemData             << HW_SRT(hif_io_cfg_sel_mem_d))
#endif
                               ;

  return 0;
}

/*****************************************************************************/
/*! HIF_IOCTRL Get IO Configuration
* \description
*   This function gets the HIF IO Configuration.
* \class
*   HIF_IOCTRL Service Class
* \params
* \return
*   Current HIF_IO_CFG value                                                 */
/*****************************************************************************/
uint32_t HALDEF(HIF_IOCTRL_GetIoConfig) ( void )
{
  return s_ptHifIoCtrl->ulHif_io_cfg;
}

/*****************************************************************************/
/*! HIF_IOCTRL Set IO Configuration Register
* \description
*   This function sets the HIF IO Configuration Register.
* \class
*   HIF_IOCTRL Service Class
* \params
*   ulHifIoCfg [in] HIF-IO configuration value
* \return                                                                    
*                                                                            */
/*****************************************************************************/
void HALDEF(HIF_IOCTRL_SetIoConfig)( uint32_t ulHifIoCfg )
{
  uint32_t ulAccessKey;

  /* set HIF IO configuration */
  ulAccessKey = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulAsic_ctrl_access_key = ulAccessKey;

  s_ptHifIoCtrl->ulHif_io_cfg = ulHifIoCfg;
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
*   eIrqD12Cfg        [in] HIF_D12 (DPM_SPI_DIRQ/SPM_DIRQ) IRQ input configuration
*   uFilterIrqs       [in] Filtering of HIF PIO inputs for IRQ generation
*   uNetx50PioRegComp [in] netX50 HIF-PIO configuration register compatibility
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(HIF_IOCTRL_SetPioCfg) ( HIF_IOCTRL_IN_CTRL_E    eInCtrl,
                                  unsigned int             uSelEnIn,
                                  unsigned int             uFilterEnIn,
                                  HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio35Cfg,
                                  HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio36Cfg,
                                  HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio40Cfg,
                                  HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio47Cfg,
                                  HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio72Cfg,
                                  HIF_IOCTRL_IRQ_PIO_CFG_E eIrqD12Cfg,
                                  unsigned int             uFilterIrqs,
                                  unsigned int             uNetx50PioRegComp
                               )
{
  /* Plausibility check */
  if( eInCtrl>HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_EN_IN_HI )
    return -1;
  if( eIrqPio35Cfg>HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( eIrqPio36Cfg>HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( eIrqPio40Cfg>HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( eIrqPio47Cfg>HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( eIrqPio72Cfg>HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;
  if( eIrqD12Cfg>HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE )
    return -1;

  /* set HIF PIO configuration */
  s_ptHifIoCtrl->ulHif_pio_cfg = ((uint32_t)eInCtrl      << HW_SRT(hif_pio_cfg_in_ctrl))
#if defined(__NETX56) || defined(__NETX4000)
                                |((uint32_t)uSelEnIn     << HW_SRT(hif_pio_cfg_sel_en_in))
#endif
                                |((uint32_t)uFilterEnIn  << HW_SRT(hif_pio_cfg_filter_en_in))
                                |((uint32_t)eIrqPio47Cfg << HW_SRT(hif_pio_cfg_irq_hif_dirq_cfg))
#if defined(__NETX4000)
                                |((uint32_t)eIrqPio35Cfg << HW_SRT(hif_pio_cfg_irq_hif_d17_cfg))
                                |((uint32_t)eIrqPio36Cfg << HW_SRT(hif_pio_cfg_irq_hif_d16_cfg))
                                |((uint32_t)eIrqPio40Cfg << HW_SRT(hif_pio_cfg_irq_hif_d18_cfg))
                                |((uint32_t)eIrqPio72Cfg << HW_SRT(hif_pio_cfg_irq_hif_d27_cfg))
#else
                                |((uint32_t)eIrqPio35Cfg << HW_SRT(hif_pio_cfg_irq_hif_a17_cfg))
                                |((uint32_t)eIrqPio36Cfg << HW_SRT(hif_pio_cfg_irq_hif_a16_cfg))
#endif
                                |((uint32_t)eIrqD12Cfg   << HW_SRT(hif_pio_cfg_irq_hif_d12_cfg))
                                |((uint32_t)uFilterIrqs  << HW_SRT(hif_pio_cfg_filter_irqs));

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
void HALDEF(HIF_IOCTRL_SetLine)( unsigned int uOutRegNum,
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
uint32_t HALDEF(HIF_IOCTRL_GetLine)( unsigned int uOutRegNum )
{
  if( uOutRegNum>1 )
    return 0;
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
void HALDEF(HIF_IOCTRL_SetOe)( unsigned int uOeRegNum,
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
*   0 on erroneous
*   Current HIF PIO_OE register value                                        */
/*****************************************************************************/
uint32_t HALDEF(HIF_IOCTRL_GetOe)( unsigned int uOeRegNum )
{
  if( uOeRegNum>1 )
    return 0;
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
uint32_t HALDEF(HIF_IOCTRL_GetIn)( unsigned int uInRegNum )
{
  if( uInRegNum>1 )
    return 0;
  else
    return ( s_ptHifIoCtrl->aulHif_pio_in[uInRegNum] );
}

/*****************************************************************************/
/*! HIF_IOCTRL ARM Interrupt Enable
* \description
*   Enable ARM interrupt request for the selected HIF PIO.
* \class
*   HIF_IOCTRL
* \params
*   ulPioNum          [in]   Selected HIF PIO
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(HIF_IOCTRL_IrqEnable)( uint32_t ulPioNum )
{
  if( ulPioNum<=HW_SRT(hif_pio_irq_arm_mask_set_irq_hif_dirq) )
    s_ptHifIoCtrl->ulHif_pio_irq_arm_mask_set |= (uint32_t)1 << ulPioNum;
}

/*****************************************************************************/
/*! HIF_IOCTRL ARM Interrupt Disable
* \description
*   Disable ARM interrupt request for the selected PIO.
* \class
*   HIF_IOCTRL
* \params
*   ulPioNum          [in]   Selected HIF PIO
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(HIF_IOCTRL_IrqDisable)( uint32_t ulPioNum )
{
  if( ulPioNum<=HW_SRT(hif_pio_irq_arm_mask_reset_irq_hif_dirq) )
    s_ptHifIoCtrl->ulHif_pio_irq_arm_mask_reset |= (uint32_t)1 << ulPioNum;
}

/*****************************************************************************/
/*! HIF_IOCTRL Get ARM Interrupt
* \description
*   Read ARM interrupt status of the selected HIF PIO.
* \class
*   HIF_IOCTRL
* \params
*   ulPioNum          [in]   Selected HIF PIO
* \return
*   0 on erroneous
*   Current HIF PIO_IRQ_ARM_MASKED value of selected HIF_PIO                 */
/*****************************************************************************/
int HALDEF(HIF_IOCTRL_GetIrq)( uint32_t ulPioNum )
{
  if( ulPioNum>HW_SRT(hif_pio_irq_arm_masked_irq_hif_dirq) )
    return 0;
  else
    return (s_ptHifIoCtrl->ulHif_pio_irq_arm_masked & ((uint32_t)1 << ulPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! HIF_IOCTRL Get ARM Interrupt Masked
* \description
*   Get complete ARM interrupt masked status.
* \class
*   HIF_IOCTRL
* \params
* \return
*   Current HIF PIO_IRQ_ARM_MASKED value                                     */
/*****************************************************************************/
uint32_t HALDEF(HIF_IOCTRL_GetIrqMasked)( void )
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
void HALDEF(HIF_IOCTRL_IrqReset)( uint32_t ulPioNum )
{
  if( ulPioNum<=HW_SRT(hif_pio_irq_raw_irq_hif_dirq) )
    s_ptHifIoCtrl->ulHif_pio_irq_raw = (uint32_t)1 << ulPioNum;
}

/*****************************************************************************/
/*! HIF_IOCTRL Set Parameter
* \description
*   This function writes parameter.
* \class
*   HIF_IOCTRL
* \params
*   ePrmID   [in] Parameter ID
*   ulPrmVal [in] Parameter value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(HIF_IOCTRL_SetParam)( HIF_IOCTRL_PRM_E ePrmID,
                                uint32_t                ulPrmVal )
{
  switch( ePrmID )
  {
    case HIF_IOCTRL_PRM_HIF_PIO_CFG:                 s_ptHifIoCtrl->ulHif_pio_cfg                 = ulPrmVal; break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_ARM_MASK_SET:    s_ptHifIoCtrl->ulHif_pio_irq_arm_mask_set    = ulPrmVal; break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_ARM_MASK_RESET:  s_ptHifIoCtrl->ulHif_pio_irq_arm_mask_reset  = ulPrmVal; break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_XPIC_MASK_SET:   s_ptHifIoCtrl->ulHif_pio_irq_xpic_mask_set   = ulPrmVal; break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_XPIC_MASK_RESET: s_ptHifIoCtrl->ulHif_pio_irq_xpic_mask_reset = ulPrmVal; break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_RAW:             s_ptHifIoCtrl->ulHif_pio_irq_raw             = ulPrmVal; break;

    default: return -1; break;
  }

  return 0;
}

/*****************************************************************************/
/*! HIF_IOCTRL Get Parameter
* \description
*   This function reads parameter and returns it.
* \class
*   HIF_IOCTRL
* \params
*   ePrmID  [in] Parameter ID
* \return
*   Read Parameter Value                                                     */
/*****************************************************************************/
uint32_t HALDEF(HIF_IOCTRL_GetParam)( HIF_IOCTRL_PRM_E ePrmID )
{
  uint32_t ulRetVal;

  switch( ePrmID )
  {
    case HIF_IOCTRL_PRM_HIF_PIO_CFG:                 ulRetVal = s_ptHifIoCtrl->ulHif_pio_cfg;                 break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_ARM_MASK_SET:    ulRetVal = s_ptHifIoCtrl->ulHif_pio_irq_arm_mask_set;    break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_ARM_MASK_RESET:  ulRetVal = s_ptHifIoCtrl->ulHif_pio_irq_arm_mask_reset;  break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_ARM_MASKED:      ulRetVal = s_ptHifIoCtrl->ulHif_pio_irq_arm_masked;      break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_XPIC_MASK_SET:   ulRetVal = s_ptHifIoCtrl->ulHif_pio_irq_xpic_mask_set;   break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_XPIC_MASK_RESET: ulRetVal = s_ptHifIoCtrl->ulHif_pio_irq_xpic_mask_reset; break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_XPIC_MASKED:     ulRetVal = s_ptHifIoCtrl->ulHif_pio_irq_xpic_masked;     break;
    case HIF_IOCTRL_PRM_HIF_PIO_IRQ_RAW:             ulRetVal = s_ptHifIoCtrl->ulHif_pio_irq_raw;             break;

    default: return ulRetVal=0; break;
  }

  return ulRetVal;
}
