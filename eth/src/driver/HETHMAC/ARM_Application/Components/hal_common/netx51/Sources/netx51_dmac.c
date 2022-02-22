/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_dmac.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX51_NUM_DMA_CH 3

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_DMAC_REG
__USE_DMAC_CH

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! DMAC Configuration
* \description
*   This function configures the DMA controller.
* \class
*   DMAC
* \params
*   fEnable  [in]  1: enable, 0: disable the DMA controller
* \return
*   NX51_DMAC_OKAY                                                           */
/*****************************************************************************/
NX51_DMAC_RESULT_E NX51_Dmac_Config( bool fEnable )
{
  /* set Enable */
  s_ptDmacReg->ulDmac_config = fEnable ? MSK_NX51_dmac_config_DMACENABLE : 0;

  return NX51_DMAC_OKAY;
}


/*****************************************************************************/
/*! DMAC Get Interrupt(s)
* \description
*   This function retrieves the current interrupt masked requests of the DMAC.
*   The interrupt mask is specified in the appropriate DMA channel configuration.
* \class
*   DMAC
* \params
*   pulIrqTc     [out] Pointer to interrupt terminal count events
*   pulIrqErr    [out] Pointer to interrupt error events
* \return
*   NX51_DMAC_OKAY                                                           */
/*****************************************************************************/
NX51_DMAC_RESULT_E NX51_Dmac_GetIrq( uint32_t* pulIrqTc,
                                     uint32_t* pulIrqErr )
{
  /* get IRQ requests */
  *pulIrqTc  = s_ptDmacReg->ulDmac_inttc_status;
  *pulIrqErr = s_ptDmacReg->ulDmac_interr_status;

  return NX51_DMAC_OKAY;
}


/*****************************************************************************/
/*! DMAC Confirm Interrupt(s)
* \description
*   This function confirms a set of interrupts that were requested by the DMAC.
* \class
*   DMAC
* \params
*   ulConfirmIrqTcMask  [in] Mask to confirm interrupt terminal count events
*   ulConfirmIrqErrMask  [in] Mask to confirm interrupt error events
* \return
*   NX51_DMAC_OKAY                                                           */
/*****************************************************************************/
NX51_DMAC_RESULT_E NX51_Dmac_ConfirmIrq( uint32_t ulConfirmIrqTcMask,
                                         uint32_t ulConfirmIrqErrMask )
{
  /* get IRQ requests */
  s_ptDmacReg->ulDmac_inttc_clear  = ulConfirmIrqTcMask;
  s_ptDmacReg->ulDmac_interr_clear = ulConfirmIrqErrMask;

  return NX51_DMAC_OKAY;
}


/*****************************************************************************/
/*! DMAC Configure Channel
* \description
*   This function configures one channel of the DMA controller.
*   The bits "Halt" and "Enable" of the channel configuration register
*   are ignored within this routine.
* \class
*   DMAC
* \params
*   uChannel           [in] DMA channel number
*   ptDmaChCfg         [in] Pointer to DMA channel configuration structure
* \return
*   NX51_DMAC_OKAY
*   NX51_DMAC_ERR_INVAL_CHANNEL                                              */
/*****************************************************************************/
NX51_DMAC_RESULT_E NX51_Dmac_ConfigChannel( unsigned int     uChannel,
                                            NX51_DMAC_CHCFG* ptDmaChCfg )
{
  NX51_DMAC_CH_AREA_T* ptDmaCh;

  /* check for valid channel number */
  if (uChannel >= NX51_NUM_DMA_CH)
    return NX51_DMAC_ERR_INVAL_CHANNEL;

  /* get current channel address */
  ptDmaCh = s_aptDmacCh[uChannel];

  ptDmaCh->ulDmac_chsrc_ad  = ptDmaChCfg->ulDmacChSrcAd;
  ptDmaCh->ulDmac_chdest_ad = ptDmaChCfg->ulDmacChDestAd;
  ptDmaCh->ulDmac_chlink    = ptDmaChCfg->ulDmacChLink;
  ptDmaCh->ulDmac_chctrl    = ptDmaChCfg->ulDmacChCtrl;
  ptDmaCh->ulDmac_chcfg     = ptDmaChCfg->ulDmacChCfg & ~(MSK_NX51_dmac_chcfg_E | MSK_NX51_dmac_chcfg_H);

  return NX51_DMAC_OKAY;
}


/*****************************************************************************/
/*! DMAC Setup Memory To Peripheral Transfer
* \description
*   This function configures one channel of the DMA controller for
*   a memory to peripheral transfer.\n
*   The bits "Halt" and "Enable" of the channel configuration register
*   are ignored within this routine.
* \class
*   DMAC
* \params
*   uChannel                [in]  DMA channel number
*   ulSrcAddr               [in]  Address of source memory
*   eTransWidth             [in]  Transfer width
*   ulTransSize             [in]  Transfer size
*   eDstPer                 [in]  Destination peripheral
* \return
*   NX51_DMAC_OKAY
*   NX51_DMAC_ERR_INVAL_CHANNEL
*   NX51_DMAC_ERR_INVAL_PARAM                                                */
/*****************************************************************************/
NX51_DMAC_RESULT_E NX51_Dmac_SetupMem2PerTransfer( unsigned int          uChannel,
                                                   uint32_t              ulSrcAddr,
                                                   NX51_DMA_TRANSWIDTH_E eTransWidth,
                                                   uint32_t              ulTransSize,
                                                   NX51_DMA_PER_E        eDstPer )
{
  NX51_DMAC_CHCFG   tDmaChCfg;
  uint32_t     ulDstAddr;
  uint32_t     ulDstNum;
  uint32_t     ulTransWidth;
  uint32_t     ulDBSize;
  NX51_DMAC_RESULT_E  eRet;
  uint32_t     ulVal;

  ulDBSize = 1;

  switch (eDstPer)
  {
    case NX51_DMA_PER_SQI_TX:
            ulDstAddr = Adr_NX51_sqi_sqi_dr;
            ulDstNum  = 1;
            break;

    case NX51_DMA_PER_SPI1_TX:
            ulDstAddr = Adr_NX51_spi_motion_spi_dr;
            ulDstNum  = 3;
            break;

    case NX51_DMA_PER_UART0_TX:
            ulDstAddr = Adr_NX51_uart0_uartdr;
            ulDstNum  = 5;
            break;

    case NX51_DMA_PER_UART1_TX:
            ulDstAddr = Adr_NX51_uart1_uartdr;
            ulDstNum  = 7;
            break;

    case NX51_DMA_PER_UART2_TX:
            ulDstAddr = Adr_NX51_uart2_uartdr;
            ulDstNum  = 9;
            break;

    case NX51_DMA_PER_ETH_TX:
            ulDstAddr = Adr_NX51_eth_eth_tx_data;
            ulDstNum  = 11;
            break;

    case NX51_DMA_PER_I2C_MASTER:
            ulDstAddr = Adr_NX51_i2c0_i2c_mdr;
            ulDstNum  = 12;
            break;

    case NX51_DMA_PER_I2C_SLAVE:
            ulDstAddr = Adr_NX51_i2c0_i2c_sdr;
            ulDstNum  = 13;
            break;

    case NX51_DMA_PER_USB_DEV_UART_TX:
            ulDstAddr = Adr_NX51_usb_dev_fifo_usb_dev_uart_tx_data;
            ulDstNum  = 15;
            /*ulDBSize  = 0; */
            break;

    default:
            /* transfer not allowed */
            return NX51_DMAC_ERR_INVAL_PARAM;
  }

  switch (eTransWidth)
  {
    case NX51_DMA_TRANSWIDTH_8BIT:
            ulTransWidth = 0;
            break;

    case NX51_DMA_TRANSWIDTH_16BIT:
            ulTransWidth = 1;
            break;

    case NX51_DMA_TRANSWIDTH_32BIT:
            ulTransWidth = 2;
            break;

    default:
      return NX51_DMAC_ERR_INVAL_PARAM;
  }


  tDmaChCfg.ulDmacChSrcAd = ulSrcAddr;
  tDmaChCfg.ulDmacChDestAd = ulDstAddr;
  tDmaChCfg.ulDmacChLink = 0;
  ulVal = MSK_NX51_dmac_chctrl_I | MSK_NX51_dmac_chctrl_SI | MSK_NX51_dmac_chctrl_ARM_EQ |
          (ulTransWidth << SRT_NX51_dmac_chctrl_DWidth) | (ulTransWidth << SRT_NX51_dmac_chctrl_SWidth) |
          (ulDBSize << SRT_NX51_dmac_chctrl_DBSize) | (ulDBSize << SRT_NX51_dmac_chctrl_SBSize) |
          (ulTransSize << SRT_NX51_dmac_chctrl_TransferSize);
  tDmaChCfg.ulDmacChCtrl = ulVal;

  ulVal = MSK_NX51_dmac_chcfg_ITC | MSK_NX51_dmac_chcfg_IE | (1 << SRT_NX51_dmac_chcfg_FlowCntrl) | (ulDstNum << SRT_NX51_dmac_chcfg_DestPeripheral);
  tDmaChCfg.ulDmacChCfg = ulVal;

  eRet = NX51_Dmac_ConfigChannel(uChannel, &tDmaChCfg);

  return eRet;
}


/*****************************************************************************/
/*! DMAC Setup Peripheral To Memory Transfer
* \description
*   This function configures one channel of the DMA controller for
*   a peripheral to memory transfer.\n
*   The bits "Halt" and "Enable" of the channel configuration register
*   are ignored within this routine.
* \class
*   DMAC
* \params
*   uChannel                [in]  DMA channel number
*   eSrcPer                 [in]  Source peripheral
*   eTransWidth             [in]  Transfer width
*   ulTransSize             [in]  Transfer size
*   ulDstAddr               [in]  Address of destination memory
* \return
*   NX51_DMAC_OKAY
*   NX51_DMAC_ERR_INVAL_CHANNEL
*   NX51_DMAC_ERR_INVAL_PARAM                                                */
/*****************************************************************************/
NX51_DMAC_RESULT_E NX51_Dmac_SetupPer2MemTransfer( unsigned int          uChannel,
                                                   NX51_DMA_PER_E        eSrcPer,
                                                   NX51_DMA_TRANSWIDTH_E eTransWidth,
                                                   uint32_t              ulTransSize,
                                                   uint32_t              ulDstAddr )
{
  NX51_DMAC_CHCFG   tDmaChCfg;
  uint32_t     ulSrcAddr;
  uint32_t     ulSrcNum;
  uint32_t     ulTransWidth;
  NX51_DMAC_RESULT_E  eRet;
  uint32_t     ulVal;

  switch (eSrcPer)
  {
    case NX51_DMA_PER_SQI_RX:
            ulSrcAddr = Adr_NX51_sqi_sqi_dr;
            ulSrcNum  = 0;
            break;

    case NX51_DMA_PER_SPI1_RX:
            ulSrcAddr = Adr_NX51_spi_motion_spi_dr;
            ulSrcNum  = 2;
            break;

    case NX51_DMA_PER_UART0_RX:
            ulSrcAddr = Adr_NX51_uart0_uartdr;
            ulSrcNum  = 4;
            break;

    case NX51_DMA_PER_UART1_RX:
            ulSrcAddr = Adr_NX51_uart1_uartdr;
            ulSrcNum  = 6;
            break;

    case NX51_DMA_PER_UART2_RX:
            ulSrcAddr = Adr_NX51_uart2_uartdr;
            ulSrcNum  = 8;
            break;

    case NX51_DMA_PER_ETH_RX:
            ulSrcAddr = Adr_NX51_eth_eth_rx_data;
            ulSrcNum  = 10;
            break;

    case NX51_DMA_PER_I2C_MASTER:
            ulSrcAddr = Adr_NX51_i2c0_i2c_mdr;
            ulSrcNum  = 12;
            break;

    case NX51_DMA_PER_I2C_SLAVE:
            ulSrcAddr = Adr_NX51_i2c0_i2c_sdr;
            ulSrcNum  = 13;
            break;

    case NX51_DMA_PER_USB_DEV_UART_RX:
            ulSrcAddr = Adr_NX51_usb_dev_fifo_usb_dev_uart_rx_data;
            ulSrcNum  = 14;
            break;

    default:
            /* transfer not allowed */
            return NX51_DMAC_ERR_INVAL_PARAM;
  }

  switch (eTransWidth)
  {
    case NX51_DMA_TRANSWIDTH_8BIT:
            ulTransWidth = 0;
            break;

    case NX51_DMA_TRANSWIDTH_16BIT:
            ulTransWidth = 1;
            break;

    case NX51_DMA_TRANSWIDTH_32BIT:
            ulTransWidth = 2;
            break;

    default:
      return NX51_DMAC_ERR_INVAL_PARAM;
  }


  tDmaChCfg.ulDmacChSrcAd = ulSrcAddr;
  tDmaChCfg.ulDmacChDestAd = ulDstAddr;
  tDmaChCfg.ulDmacChLink = 0;
  ulVal = MSK_NX51_dmac_chctrl_I | MSK_NX51_dmac_chctrl_DI | MSK_NX51_dmac_chctrl_ARM_EQ |
          (ulTransWidth << SRT_NX51_dmac_chctrl_DWidth) | (ulTransWidth << SRT_NX51_dmac_chctrl_SWidth) |
          (1 << SRT_NX51_dmac_chctrl_DBSize) | (1 << SRT_NX51_dmac_chctrl_SBSize) |
          (ulTransSize << SRT_NX51_dmac_chctrl_TransferSize);
  tDmaChCfg.ulDmacChCtrl = ulVal;

  ulVal = MSK_NX51_dmac_chcfg_ITC | MSK_NX51_dmac_chcfg_IE | (2 << SRT_NX51_dmac_chcfg_FlowCntrl) | (ulSrcNum << SRT_NX51_dmac_chcfg_SrcPeripheral);
  tDmaChCfg.ulDmacChCfg = ulVal;

  eRet = NX51_Dmac_ConfigChannel(uChannel, &tDmaChCfg);

  return eRet;
}


/*****************************************************************************/
/*! DMAC Enable Channel
* \description
*   This function enables one channel of the DMA controller.
*   The channel configuration must be written before the channel is enabled.
* \class
*   DMAC
* \params
*   uChannel           [in] DMA channel number
* \return
*   NX51_DMAC_OKAY
*   NX51_DMAC_ERR_INVAL_CHANNEL                                                   */
/*****************************************************************************/
NX51_DMAC_RESULT_E NX51_Dmac_EnableChannel( unsigned int uChannel )
{
  NX51_DMAC_CH_AREA_T* ptDmaCh;

  /* check for valid channel number */
  if (uChannel >= NX51_NUM_DMA_CH)
    return NX51_DMAC_ERR_INVAL_CHANNEL;

  /* get current channel address */
  ptDmaCh = s_aptDmacCh[uChannel];

  ptDmaCh->ulDmac_chcfg |= MSK_NX51_dmac_chcfg_E;

  return NX51_DMAC_OKAY;
}


/*****************************************************************************/
/*! DMAC Disable Channel
* \description
*   This function disables one channel of the DMA controller.
* \class
*   DMAC
* \params
*   uChannel           [in] DMA channel number
* \return
*   NX51_DMAC_OKAY
*   NX51_DMAC_ERR_INVAL_CHANNEL                                                   */
/*****************************************************************************/
NX51_DMAC_RESULT_E NX51_Dmac_DisableChannel( unsigned int uChannel )
{
  NX51_DMAC_CH_AREA_T* ptDmaCh;

  /* check for valid channel number */
  if (uChannel >= NX51_NUM_DMA_CH)
    return NX51_DMAC_ERR_INVAL_CHANNEL;

  /* get current channel address */
  ptDmaCh = s_aptDmacCh[uChannel];

  /* halt channel */
  ptDmaCh->ulDmac_chcfg |= MSK_NX51_dmac_chcfg_H;

  /* wait for channel to get inactive */
  while( (ptDmaCh->ulDmac_chcfg & MSK_NX51_dmac_chcfg_A) != 0 );

  /* disable channel */
  ptDmaCh->ulDmac_chcfg &= ~MSK_NX51_dmac_chcfg_E;

  return NX51_DMAC_OKAY;
}
