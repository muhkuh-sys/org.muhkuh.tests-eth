#ifndef __HAL_MMIO_H
#define __HAL_MMIO_H

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C: "bool" definition */

/* Definitions */

typedef enum
{
  MMIO_CONFIG_XC_SAMPLE0 = 0,
  MMIO_CONFIG_XC_SAMPLE1,
  MMIO_CONFIG_XC_TRIGGER0,
  MMIO_CONFIG_XC_TRIGGER1,
  MMIO_CONFIG_CAN0_APP_RX,
  MMIO_CONFIG_CAN0_APP_TX,
  MMIO_CONFIG_CAN1_APP_RX,
  MMIO_CONFIG_CAN1_APP_TX,
  MMIO_CONFIG_I2C_XPIC_APP_SCL,
  MMIO_CONFIG_I2C_XPIC_APP_SDA,
  MMIO_CONFIG_I2C_ARM_APP_SCL,
  MMIO_CONFIG_I2C_ARM_APP_SDA,
  MMIO_CONFIG_SPI_XPIC_APP_CLK,
  MMIO_CONFIG_SPI_XPIC_APP_CS0N,
  MMIO_CONFIG_SPI_XPIC_APP_CS1N,
  MMIO_CONFIG_SPI_XPIC_APP_CS2N,
  MMIO_CONFIG_SPI_XPIC_APP_MISO,
  MMIO_CONFIG_SPI_XPIC_APP_MOSI,
  MMIO_CONFIG_SPI1_ARM_APP_CLK,
  MMIO_CONFIG_SPI1_ARM_APP_CS0N,
  MMIO_CONFIG_SPI1_ARM_APP_CS1N,
  MMIO_CONFIG_SPI1_ARM_APP_CS2N,
  MMIO_CONFIG_SPI1_ARM_APP_MISO,
  MMIO_CONFIG_SPI1_ARM_APP_MOSI,
  MMIO_CONFIG_UART_XPIC_APP_RXD,
  MMIO_CONFIG_UART_XPIC_APP_TXD,
  MMIO_CONFIG_UART_XPIC_APP_RTSN,
  MMIO_CONFIG_UART_XPIC_APP_CTSN,
  MMIO_CONFIG_UART_ARM_APP_RXD,
  MMIO_CONFIG_UART_ARM_APP_TXD,
  MMIO_CONFIG_UART_ARM_APP_RTSN,
  MMIO_CONFIG_UART_ARM_APP_CTSN,
  MMIO_CONFIG_GPIO0,
  MMIO_CONFIG_GPIO1,
  MMIO_CONFIG_GPIO2,
  MMIO_CONFIG_GPIO3,
  MMIO_CONFIG_GPIO4,
  MMIO_CONFIG_GPIO5,
  MMIO_CONFIG_GPIO6,
  MMIO_CONFIG_GPIO7,
  MMIO_CONFIG_WDG_ACTIVE,
  MMIO_CONFIG_EN_IN,
  MMIO_CONFIG_ETH_MDC,
  MMIO_CONFIG_ETH_MDIO,
  MMIO_CONFIG_PIO_MODE = 0x3f
} MMIO_CONFIG_E;

typedef enum
{
  MMIO_MODE_IN  = 0,
  MMIO_MODE_OUT = 1
} MMIO_MODE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void          MMIO_SetCfg           ( unsigned int uiMmio, MMIO_CONFIG_E eFunction, unsigned int uiInvertOut, unsigned int uiInvertIn );
void          MMIO_GetCfg           ( unsigned int uiMmio, MMIO_CONFIG_E* peFunction, unsigned int* puiInvertOut, unsigned int* puiInvertIn );
void          MMIO_SetMode          ( unsigned int uiMmio, MMIO_MODE_E eMode );
unsigned int  MMIO_GetInput         ( unsigned int uiMmio );
unsigned int  MMIO_GetOutput        ( unsigned int uiMmio );
void          MMIO_SetOutput        ( unsigned int uiMmio, unsigned int uiEnable );
void          MMIO_SetOeLine        ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          MMIO_GetOeLine        ( uint32_t* pulLine0, uint32_t* pulLine1, uint32_t* pulLine2, uint32_t* pulLine3 );
void          MMIO_SetOutLineCfg    ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          MMIO_GetOutLineCfg    ( uint32_t* pulLine0, uint32_t* pulLine1, uint32_t* pulLine2, uint32_t* pulLine3 );
void          MMIO_GetInLineStatus  ( uint32_t* pulLine0, uint32_t* pulLine1, uint32_t* pulLine2, uint32_t* pulLine3 );
uint32_t      MMIO_GetInLine        ( unsigned int uiLine );
void          MMIO_SetCfgReg        ( unsigned int uiMmio, uint32_t ulSetCfg );
void          MMIO_GetCfgReg        ( unsigned int uiMmio, uint32_t* pulGetCfg );
void          MMIO_SetCfgRaw        ( unsigned int uiMmio, uint32_t ulSetCfg );
void          MMIO_PioOutLineCfgSet ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          MMIO_PioOutLineCfgRst ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          MMIO_PioOeLineCfgSet  ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          MMIO_PioOeLineCfgRst  ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );

#define MMIO_SetLine  MMIO_SetOutLineCfg
#define MMIO_GetLine  MMIO_GetInLineStatus

#endif /* #ifndef __HAL_MMIO_H */
