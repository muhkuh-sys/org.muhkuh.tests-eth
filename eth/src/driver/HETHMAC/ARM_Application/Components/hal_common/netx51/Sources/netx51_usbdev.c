/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_usbdev.h"
#include "netx51_dmac.h"
#include "hal_resources_defines_netx51.h"


/*****************************************************************************/
/*  Definitions                                                              */
/*****************************************************************************/

#define MSK_NX51_usb_dev_fifo_ctrl_conf_reset_uart_rx (1 << (3 + SRT_NX51_usb_dev_fifo_ctrl_conf_reset))


/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_USB_DEV_CTRL
__USE_USB_DEV_ENUM_RAM
__USE_USB_DEV_FIFO_CTRL
__USE_USB_DEV_FIFO
__USE_DMAC_CH

typedef struct NX51_USB_DMAC_RSC_Ttag
{
  NX51_DMAC_LLI_T    tList;
  volatile uint32_t  ulLength;
} NX51_USB_DMAC_RSC_T;

/*lint -e551 accessed by DMA */
static NX51_USB_DMAC_RSC_T s_tUsbDevDmaTx;
static NX51_USB_DMAC_RSC_T s_tUsbDevDmaRx;

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! USBDEV Initialize
* \description
*   This function resets and configures the USB Device unit
* \class
*   USBDEV
* \params
*   ptDescriptors        [in] Memory image to be copied into usb_dev_enum_ram
*   eRxMode              [in] RX transfer mode of the UART end point
*   eTxMode              [in] TX transfer mode of the UART end point
*   fEnableRxDma         [in] Use DMAC unit for RX transfers of UART end point
*   fEnableTxDma         [in] Use DMAC unit for TX transfers of UART end point
* \return
*   NX51_USBDEV_OKAY
*   NX51_USBDEV_ERR_DISCONNECTED
*   NX51_USBDEV_ERR_DMA_CHANNEL_ACTIVE
*   NX51_USBDEV_ERR_INVAL_PARAM                                              */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_Init( NX51_USBDEV_DESCRIPTORS_T* ptDescriptors,
                                       NX51_USBDEV_EP_MODE_E      eRxMode,
                                       NX51_USBDEV_EP_MODE_E      eTxMode,
                                       bool                       fEnableRxDma,
                                       bool                       fEnableTxDma )
{
  uint32_t ulDevCfg;
  uint32_t ulFifoCfg;

  int i;
  volatile uint32_t* pulUsbDevEnumDst;

  if( ( (int)eRxMode > NX51_USBDEV_EP_MODE_TRANSACTION )
   || ( (int)eTxMode > NX51_USBDEV_EP_MODE_TRANSACTION ) )
  {
    return NX51_USBDEV_ERR_INVAL_PARAM;
  }

  /* Reset USB device and disable USB device during configuration */
  ulDevCfg  =  DFLT_VAL_NX51_usb_dev_cfg;
  ulDevCfg |=  MSK_NX51_usb_dev_cfg_usb_dev_reset;
  ulDevCfg &= ~MSK_NX51_usb_dev_cfg_usb_core_enable;
  s_ptUsbDevCtrl->ulUsb_dev_cfg = ulDevCfg;

  ulDevCfg &= ~MSK_NX51_usb_dev_cfg_usb_dev_reset;
  s_ptUsbDevCtrl->ulUsb_dev_cfg = ulDevCfg;

  /* Reset FIFOs and config USB endpoints */
  ulFifoCfg  =  DFLT_VAL_NX51_usb_dev_fifo_ctrl_conf
             & ~MSK_NX51_usb_dev_fifo_ctrl_conf_mode_uart_rx
             & ~MSK_NX51_usb_dev_fifo_ctrl_conf_mode_uart_tx;
  ulFifoCfg |=  (uint32_t)127 << SRT_NX51_usb_dev_fifo_ctrl_conf_max_transaction_len;
  ulFifoCfg |=  (uint32_t)eRxMode << SRT_NX51_usb_dev_fifo_ctrl_conf_mode_uart_rx;
  ulFifoCfg |=  (uint32_t)eTxMode << SRT_NX51_usb_dev_fifo_ctrl_conf_mode_uart_tx;
  if( fEnableRxDma )
    ulFifoCfg |= MSK_NX51_usb_dev_fifo_ctrl_conf_dma_en_uart_rx;
  if( fEnableTxDma )
    ulFifoCfg |= MSK_NX51_usb_dev_fifo_ctrl_conf_dma_en_uart_tx;
  ulFifoCfg |=  MSK_NX51_usb_dev_fifo_ctrl_conf_reset;
  s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_conf = ulFifoCfg;

  ulFifoCfg &= ~MSK_NX51_usb_dev_fifo_ctrl_conf_reset;
  s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_conf = ulFifoCfg;

  /* Set device enumeration descriptors */
  pulUsbDevEnumDst = s_pulUsbDevEnumRam;
  for(i = 0; i < 16; i++)
  {
    *pulUsbDevEnumDst++ = ptDescriptors->aul[i];
  }

  /* Clear all interrupts */
  s_ptUsbDevCtrl->ulUsb_dev_irq_mask = 0;
  s_ptUsbDevCtrl->ulUsb_dev_irq_raw  = 0xffffffff;

  /* Enable USB device */
  ulDevCfg |= MSK_NX51_usb_dev_cfg_usb_core_enable;
  s_ptUsbDevCtrl->ulUsb_dev_cfg   = ulDevCfg;

  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Reset
* \description
*   This function resets the usb device.
* \class
*   USBDEV
* \params
* \return
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_Reset( void )
{
  s_ptUsbDevCtrl->ulUsb_dev_cfg = 0;
  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Get Irq
* \description
*   This function reads irq masked register.
* \class
*   USBDEV
* \params
*   pulIrq  [out] Irq value
* \return
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_GetIrq( uint32_t* pulIrq )
{
  *pulIrq = s_ptUsbDevCtrl->ulUsb_dev_irq_masked;
  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Get Irq Raw
* \description
*   This function reads irq raw register.
* \class
*   USBDEV
* \params
*   pulIrq  [out] Irq value
* \return
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_GetIrqRaw( uint32_t* pulIrq )
{
  *pulIrq = s_ptUsbDevCtrl->ulUsb_dev_irq_raw;
  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Confirm Irq
* \description
*   This function clears irq.
* \class
*   USBDEV
* \params
*   ulIrq  [in] Irq value
* \return
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_ConfirmIrq( uint32_t ulIrq )
{
  s_ptUsbDevCtrl->ulUsb_dev_irq_raw = ulIrq;
  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Enable Irq
* \description
*   This function enables irq.
* \class
*   USBDEV
* \params
*   ulIrqMsk  [in] Irq mask
* \return
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_IrqEnable( uint32_t ulIrqMsk )
{
  s_ptUsbDevCtrl->ulUsb_dev_irq_mask |= ulIrqMsk;
  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Disable Irq
* \description
*   This function disables irq.
* \class
*   USBDEV
* \params
*   ulIrqMsk  [in] Irq mask
* \return
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_IrqDisable( uint32_t ulIrqMsk )
{
  s_ptUsbDevCtrl->ulUsb_dev_irq_mask &= ~ulIrqMsk;
  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Set Rx Mode
* \description
*   This function sets rx mode.
* \class
*   USBDEV
* \params
*   ulIrqMsk  [in] Irq mask
* \return
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_SetRxMode( NX51_USBDEV_EP_MODE_E eRxMode,
                                            uint8_t*              pbDataBuffer,
                                            uint32_t*             pulRxLen )
{
  uint32_t ulNumBytes;
  uint32_t ulFifoCfg;
  uint32_t ulRxStat;
  uint32_t ulDmaEn;

  /* Get configuration register value */
  ulFifoCfg  =  s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_conf;
  ulDmaEn    =  ulFifoCfg & MSK_NX51_usb_dev_fifo_ctrl_conf_dma_en_uart_rx;

  /* Decline new packets */
  ulFifoCfg |=  MSK_NX51_usb_dev_fifo_ctrl_conf_uart_rx_nak_all;
  ulFifoCfg &= ~MSK_NX51_usb_dev_fifo_ctrl_conf_dma_en_uart_rx;
  s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_conf = ulFifoCfg;

  /* Wait for current packet received */
  do
  {
    ulRxStat = s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_uart_ep_rx_stat;
  } while ( (ulRxStat & MSK_NX51_usb_dev_fifo_ctrl_uart_ep_rx_stat_packet_transfer_active) != 0 );

  /* Read fifo empty */
  ulNumBytes  = (ulRxStat & MSK_NX51_usb_dev_fifo_ctrl_uart_ep_rx_stat_fill_level)
                         >> SRT_NX51_usb_dev_fifo_ctrl_uart_ep_rx_stat_fill_level;
  *pulRxLen   = ulNumBytes;
  while( ulNumBytes-- )
  {
    *pbDataBuffer++ = 0xff & s_ptUsbDevFifo->ulUsb_dev_uart_rx_data;
  }

  /* Switch mode, enable packet reception, restore DMA enable bit, reset rx fifo */
  ulFifoCfg &= ~MSK_NX51_usb_dev_fifo_ctrl_conf_mode_uart_rx;
  ulFifoCfg |=  (uint32_t)eRxMode << SRT_NX51_usb_dev_fifo_ctrl_conf_mode_uart_rx;
  ulFifoCfg |=  ulDmaEn;
  ulFifoCfg |= MSK_NX51_usb_dev_fifo_ctrl_conf_reset_uart_rx;
  s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_conf  = ulFifoCfg;

  ulFifoCfg &= ~MSK_NX51_usb_dev_fifo_ctrl_conf_uart_rx_nak_all;
  ulFifoCfg &= ~MSK_NX51_usb_dev_fifo_ctrl_conf_reset_uart_rx;
  s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_conf  = ulFifoCfg;

  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Tx Transaction DMA
* \description
*
* \class
*   USBDEV
* \params
*   uDmaChannel   [in] DMA channel
*   pbData        [in] Pointer to data
*   ulNumBytes    [in] Number of bytes
* \return
*   NX51_USBDEV_ERR_DISCONNECTED
*   NX51_USBDEV_ERR_DMA_CHANNEL_ACTIVE
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_TxTransactionDma( unsigned int   uDmaChannel,
                                                   uint8_t*       pbData,
                                                   uint32_t       ulNumBytes )
{
  uint32_t ulDmaChCfg;
  uint32_t ulDmaChCtrl;
  NX51_DMAC_CH_AREA_T* ptCh = s_aptDmacCh[uDmaChannel];

  if( s_ptUsbDevCtrl->ulUsb_dev_irq_raw & MSK_NX51_usb_dev_irq_raw_device_disconnected )
    return NX51_USBDEV_ERR_DISCONNECTED;

  if( ptCh->ulDmac_chcfg & MSK_NX51_dmac_chcfg_E )
    return NX51_USBDEV_ERR_DMA_CHANNEL_ACTIVE;

  /* Setup linked list item to transfer the data */
  ulDmaChCtrl  =  0;
  ulDmaChCtrl |=  (uint32_t)1 << SRT_NX51_dmac_chctrl_SBSize;
  ulDmaChCtrl |=  (uint32_t)1 << SRT_NX51_dmac_chctrl_DBSize;
  ulDmaChCtrl |=  MSK_NX51_dmac_chctrl_ARM_EQ;
  ulDmaChCtrl |=  MSK_NX51_dmac_chctrl_SI;
  ulDmaChCtrl |=  MSK_NX51_dmac_chctrl_I;

  s_tUsbDevDmaTx.tList.ulSrcAdr        = (uint32_t)pbData;
  s_tUsbDevDmaTx.tList.ulDstAdr        = Adr_NX51_usb_dev_fifo_usb_dev_uart_tx_data;
  s_tUsbDevDmaTx.tList.ulNextLLI       = 0;
  s_tUsbDevDmaTx.tList.ulChctrl        = ulDmaChCtrl;

  /* setup channel to transfer transaction length information and link the data transfer request */
  ulDmaChCtrl  =  0;
  ulDmaChCtrl |= (uint32_t)2 << SRT_NX51_dmac_chctrl_SWidth; /* read  32 bit at once */
  ulDmaChCtrl |= (uint32_t)2 << SRT_NX51_dmac_chctrl_DWidth; /* write 32 bit at once */
  ulDmaChCtrl |=  MSK_NX51_dmac_chctrl_ARM_EQ;

  ulDmaChCfg   =  0;
  ulDmaChCfg  |= MSK_NX51_dmac_chcfg_ITC;
  ulDmaChCfg  |= (uint32_t)NX51_DMAC_FLOW_CTRL_MEM_PER_PER << SRT_NX51_dmac_chcfg_FlowCntrl;
  ulDmaChCfg  |= (uint32_t)NX51_DMA_PER_USB_DEV_UART_TX << SRT_NX51_dmac_chcfg_DestPeripheral;
  ulDmaChCfg  |= MSK_NX51_dmac_chcfg_E;

  s_tUsbDevDmaTx.ulLength       = ulNumBytes << SRT_NX51_usb_dev_fifo_ctrl_uart_ep_tx_len_transaction_len;

  ptCh->ulDmac_chsrc_ad       = (uint32_t)&s_tUsbDevDmaTx.ulLength;
  ptCh->ulDmac_chdest_ad      = Adr_NX51_usb_dev_fifo_ctrl_uart_ep_tx_len;
  ptCh->ulDmac_chlink         = (uint32_t)&s_tUsbDevDmaTx.tList;
  ptCh->ulDmac_chctrl         = ulDmaChCtrl;
  ptCh->ulDmac_chcfg          = ulDmaChCfg;

  return NX51_USBDEV_OKAY;
}


/*****************************************************************************/
/*! USBDEV Rx Transaction DMA
* \description
*
* \class
*   USBDEV
* \params
*   uDmaChannel         [in] DMA channel
*   pbDataBuffer        [in] Pointer to data buffer
* \return
*   NX51_USBDEV_ERR_DISCONNECTED
*   NX51_USBDEV_ERR_DMA_CHANNEL_ACTIVE
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_StartRxTransactionDma( unsigned int   uDmaChannel,
                                                        uint8_t*       pbDataBuffer )
{
  uint32_t ulDmaChCfg;
  uint32_t ulDmaChCtrl;
  NX51_DMAC_CH_AREA_T* ptCh = s_aptDmacCh[uDmaChannel];

  if( s_ptUsbDevCtrl->ulUsb_dev_irq_raw & MSK_NX51_usb_dev_irq_raw_device_disconnected )
    return NX51_USBDEV_ERR_DISCONNECTED;

  if( ptCh->ulDmac_chcfg & MSK_NX51_dmac_chcfg_E )
    return NX51_USBDEV_ERR_DMA_CHANNEL_ACTIVE;

  /* Setup linked list item to transfer the transaction length */
  ulDmaChCtrl  =  0;
  ulDmaChCtrl |= (uint32_t)2 << SRT_NX51_dmac_chctrl_SWidth; /* read  32 bit at once */
  ulDmaChCtrl |= (uint32_t)2 << SRT_NX51_dmac_chctrl_DWidth; /* write 32 bit at once */
  ulDmaChCtrl |=  MSK_NX51_dmac_chctrl_ARM_EQ;
  ulDmaChCtrl |=  MSK_NX51_dmac_chctrl_I;

  s_tUsbDevDmaRx.tList.ulSrcAdr        = Adr_NX51_usb_dev_fifo_ctrl_uart_ep_rx_len;
  s_tUsbDevDmaRx.tList.ulDstAdr        = (uint32_t)&s_tUsbDevDmaRx.ulLength;
  s_tUsbDevDmaRx.tList.ulNextLLI       = 0;
  s_tUsbDevDmaRx.tList.ulChctrl        = ulDmaChCtrl;

  /* setup channel to transfer the data */
  ulDmaChCtrl  =  0;
  ulDmaChCtrl |=  (uint32_t)1 << SRT_NX51_dmac_chctrl_SBSize;
  ulDmaChCtrl |=  (uint32_t)1 << SRT_NX51_dmac_chctrl_DBSize;
  ulDmaChCtrl |=  MSK_NX51_dmac_chctrl_ARM_EQ;
  ulDmaChCtrl |=  MSK_NX51_dmac_chctrl_DI;

  ulDmaChCfg  =  0;
  ulDmaChCfg  |= MSK_NX51_dmac_chcfg_ITC;
  ulDmaChCfg  |= (uint32_t)NX51_DMAC_FLOW_CTRL_PER_MEM_PER << SRT_NX51_dmac_chcfg_FlowCntrl;
  ulDmaChCfg  |= (uint32_t)NX51_DMA_PER_USB_DEV_UART_RX << SRT_NX51_dmac_chcfg_DestPeripheral;
  ulDmaChCfg  |= MSK_NX51_dmac_chcfg_E;

  ptCh->ulDmac_chsrc_ad       = Adr_NX51_usb_dev_fifo_usb_dev_uart_rx_data;
  ptCh->ulDmac_chdest_ad      = (uint32_t)pbDataBuffer;
  ptCh->ulDmac_chlink         = (uint32_t)&s_tUsbDevDmaRx.tList;
  ptCh->ulDmac_chctrl         = ulDmaChCtrl;
  ptCh->ulDmac_chcfg          = ulDmaChCfg;

  return NX51_USBDEV_OKAY;
}

/*****************************************************************************/
/*! USBDEV Finish Transaction DMA
* \description
*
* \class
*   USBDEV
* \params
*   uDmaChannel         [in]  DMA channel
*   pulNumBytes         [out] Pointer of number of bytes
* \return
*   NX51_USBDEV_ERR_DISCONNECTED
*   NX51_USBDEV_ERR_DMA_CHANNEL_ACTIVE
*   NX51_USBDEV_OKAY                                                         */
/*****************************************************************************/
NX51_USBDEV_RESULT_E NX51_UsbDev_FinishRxTransactionDma( unsigned int uDmaChannel,
                                                         uint32_t*    pulNumBytes )
{
  NX51_DMAC_CH_AREA_T* ptCh = s_aptDmacCh[uDmaChannel];
  uint32_t ulNumBytes;

  if( s_ptUsbDevCtrl->ulUsb_dev_irq_raw & MSK_NX51_usb_dev_irq_raw_device_disconnected )
    return NX51_USBDEV_ERR_DISCONNECTED;

  if( ptCh->ulDmac_chcfg & MSK_NX51_dmac_chcfg_E )
    return NX51_USBDEV_ERR_DMA_CHANNEL_ACTIVE;

  ulNumBytes = (s_tUsbDevDmaRx.ulLength & MSK_NX51_usb_dev_fifo_ctrl_uart_ep_rx_len_transaction_len)
                                   >> SRT_NX51_usb_dev_fifo_ctrl_uart_ep_rx_len_transaction_len;

  if( ulNumBytes == 0 )
  {
    ulNumBytes = (s_tUsbDevDmaRx.ulLength & MSK_NX51_usb_dev_fifo_ctrl_uart_ep_rx_len_packet_len)
                                       >> SRT_NX51_usb_dev_fifo_ctrl_uart_ep_rx_len_packet_len;
  }

  *pulNumBytes = ulNumBytes;

  return NX51_USBDEV_OKAY;
}
