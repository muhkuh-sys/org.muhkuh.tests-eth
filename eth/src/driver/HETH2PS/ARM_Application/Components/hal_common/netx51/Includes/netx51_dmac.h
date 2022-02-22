#ifndef __NETX51_DMAC_H
#define __NETX51_DMAC_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*! Result codes for functions */
typedef enum NX51_DMAC_RESULT_Etag {
  NX51_DMAC_OKAY              = 0, /**< Successful                          */
  NX51_DMAC_ERR_INVAL_CHANNEL,     /**< Invalid DMA channel specified       */
  NX51_DMAC_ERR_INVAL_PARAM        /**< Invalid parameter specified         */
} NX51_DMAC_RESULT_E;

/*! DMA channel configuration */
typedef struct NX51_DMAC_CHCFGtag {
  uint32_t ulDmacChSrcAd;                        /**< Channel source address registers */
  uint32_t ulDmacChDestAd;                       /**< Channel destination address registers */
  uint32_t ulDmacChLink;                         /**< Channel linked list item register */
  uint32_t ulDmacChCtrl;                         /**< Channel control registers */
  uint32_t ulDmacChCfg;                          /**< Channel configuration registers, Bits "Halt" and "Enable" of the channel configuration register are ignored within Dmac_ConfigChannel(). */
} NX51_DMAC_CHCFG;

/*! DMAC Linked List Item */
typedef struct NX51_DMAC_LLI_Ttag {
  volatile uint32_t ulSrcAdr;    /**< Transfer source address          */
  volatile uint32_t ulDstAdr;    /**< Transfer destination address     */
  volatile uint32_t ulNextLLI;   /**< Address of next linked list item */
  volatile uint32_t ulChctrl;    /**< Channel control register value   */
} NX51_DMAC_LLI_T;


#define NX51_DMAC_PER_NO_ETH_RX   10
#define NX51_DMAC_PER_NO_ETH_TX   11

typedef enum NX51_DMA_FLOW_CTRL_Etag {
  NX51_DMAC_FLOW_CTRL_MEM_MEM_DMA = 0,
  NX51_DMAC_FLOW_CTRL_MEM_PER_DMA = 1,
  NX51_DMAC_FLOW_CTRL_PER_MEM_DMA = 2,
  NX51_DMAC_FLOW_CTRL_PER_PER_DMA = 3,
  NX51_DMAC_FLOW_CTRL_PER_PER_DST_PER = 4,
  NX51_DMAC_FLOW_CTRL_MEM_PER_PER = 5,
  NX51_DMAC_FLOW_CTRL_PER_MEM_PER = 6,
  NX51_DMAC_FLOW_CTRL_PER_PER_SRC_PER = 7
} NX51_DMA_FLOW_CTRL_E;


/*! DMA channel peripheral configuration */
typedef enum NX51_DMA_TRANSWIDTH_Etag {
  NX51_DMA_TRANSWIDTH_8BIT = 0,
  NX51_DMA_TRANSWIDTH_16BIT,
  NX51_DMA_TRANSWIDTH_32BIT
} NX51_DMA_TRANSWIDTH_E;


typedef enum NX51_DMA_PER_Etag {
  NX51_DMA_PER_SQI_RX = 0,
  NX51_DMA_PER_SQI_TX,
  NX51_DMA_PER_SPI1_RX,
  NX51_DMA_PER_SPI1_TX,
  NX51_DMA_PER_UART0_RX,
  NX51_DMA_PER_UART0_TX,
  NX51_DMA_PER_UART1_RX,
  NX51_DMA_PER_UART1_TX,
  NX51_DMA_PER_UART2_RX,
  NX51_DMA_PER_UART2_TX,
  NX51_DMA_PER_ETH_RX,
  NX51_DMA_PER_ETH_TX,
  NX51_DMA_PER_I2C_MASTER,
  NX51_DMA_PER_I2C_SLAVE,
  NX51_DMA_PER_USB_DEV_UART_RX,
  NX51_DMA_PER_USB_DEV_UART_TX
} NX51_DMA_PER_E;


/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/

NX51_DMAC_RESULT_E NX51_Dmac_Config( bool fEnable );
NX51_DMAC_RESULT_E NX51_Dmac_GetIrq( uint32_t* pulIrqTc, uint32_t* pulIrqErr );
NX51_DMAC_RESULT_E NX51_Dmac_ConfirmIrq( uint32_t ulConfirmIrqTcMask, uint32_t ulConfirmIrqErrMask );
NX51_DMAC_RESULT_E NX51_Dmac_ConfigChannel( unsigned int uChannel, NX51_DMAC_CHCFG* ptDmaChCfg );
NX51_DMAC_RESULT_E NX51_Dmac_EnableChannel( unsigned int uChannel );
NX51_DMAC_RESULT_E NX51_Dmac_DisableChannel( unsigned int uChannel );
NX51_DMAC_RESULT_E NX51_Dmac_SetupMem2PerTransfer( unsigned int          uChannel,
                                                 uint32_t              ulSrcAddr,
                                                 NX51_DMA_TRANSWIDTH_E eTransWidth,
                                                 uint32_t              ulTransSize,
                                                 NX51_DMA_PER_E        eDstPer );
NX51_DMAC_RESULT_E NX51_Dmac_SetupPer2MemTransfer( unsigned int          uChannel,
                                                 NX51_DMA_PER_E        eSrcPer,
                                                 NX51_DMA_TRANSWIDTH_E eTransWidth,
                                                 uint32_t              ulTransSize,
                                                 uint32_t              ulDstAddr );

#endif /* #ifndef __NETX51_DMAC_H */
