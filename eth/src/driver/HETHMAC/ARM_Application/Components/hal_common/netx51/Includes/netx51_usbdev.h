#ifndef __NETX51_USBDEV_H
#define __NETX51_USBDEV_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: bool definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*! Result codes for functions */
typedef enum NX51_USBDEV_RESULT_Etag {
  NX51_USBDEV_OKAY                     = 1, /**< Successful               */
  NX51_USBDEV_ERR_INVAL_PARAM          = 2, /**< Invalid parameter        */
  NX51_USBDEV_ERR_DISCONNECTED         = 3, /**< USB connection lost      */
  NX51_USBDEV_ERR_DMA_CHANNEL_ACTIVE   = 4  /**< DMA transfer in progress */
} NX51_USBDEV_RESULT_E;

/*! USB End point modes */
typedef enum NX51_USBDEV_EP_MODE_Etag {
  NX51_USBDEV_EP_MODE_STREAM           = 0, /**< Streaming mode, all valid data will be transferred directly */
  NX51_USBDEV_EP_MODE_STREAM_ZLP       = 1, /**< Streaming mode and a zero-length-packet is sent once if transmit FIFO is empty and last transmit packet was 64 byte */
  NX51_USBDEV_EP_MODE_PACKET           = 2, /**< Packet oriented transfers (max. 64 Byte per transfer) */
  NX51_USBDEV_EP_MODE_TRANSACTION      = 3 /**< Transfer is finished at Packet <> 64 Bytes */
} NX51_USBDEV_EP_MODE_E;

/*! USB Device Enumeration Descriptors */
typedef union {
  uint8_t ab[64];
  uint32_t aul[16];
} NX51_USBDEV_DESCRIPTORS_T;


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

NX51_USBDEV_RESULT_E NX51_UsbDev_Init( NX51_USBDEV_DESCRIPTORS_T* ptDescriptors, /**< Memory image to be copied into usb_dev_enum_ram */
                                       NX51_USBDEV_EP_MODE_E      eRxMode,       /**< RX transfer mode of the UART endpoint           */
                                       NX51_USBDEV_EP_MODE_E      eTxMode,       /**< TX transfer mode of the UART endpoint           */
                                       bool                       fEnableRxDma,  /**< Use DMAC unit for RX transfers of UART endpoint */
                                       bool                       fEnableTxDma   /**< Use DMAC unit for TX transfers of UART endpoint */ );

NX51_USBDEV_RESULT_E NX51_UsbDev_Reset( void );
NX51_USBDEV_RESULT_E NX51_UsbDev_GetIrq( uint32_t* pulIrq );
NX51_USBDEV_RESULT_E NX51_UsbDev_GetIrqRaw( uint32_t* pulIrq );
NX51_USBDEV_RESULT_E NX51_UsbDev_ConfirmIrq( uint32_t ulIrq );
NX51_USBDEV_RESULT_E NX51_UsbDev_IrqEnable( uint32_t ulIrqMsk );
NX51_USBDEV_RESULT_E NX51_UsbDev_IrqDisable( uint32_t ulIrqMsk );
NX51_USBDEV_RESULT_E NX51_UsbDev_SetRxMode( NX51_USBDEV_EP_MODE_E eRxMode,
                                            uint8_t*              pbDataBuffer,
                                            uint32_t*             pulRxLen );
NX51_USBDEV_RESULT_E NX51_UsbDev_TxTransactionDma( unsigned int uDmaChannel,
                                                   uint8_t*     pbData,
                                                   uint32_t     ulNumBytes );
NX51_USBDEV_RESULT_E NX51_UsbDev_StartRxTransactionDma( unsigned int uDmaChannel,
                                                        uint8_t*     pbDataBuffer );
NX51_USBDEV_RESULT_E NX51_UsbDev_FinishRxTransactionDma( unsigned int uDmaChannel,
                                                         uint32_t*    pulNumBytes );

#endif /* #ifndef __NETX51_USBDEV_H */
