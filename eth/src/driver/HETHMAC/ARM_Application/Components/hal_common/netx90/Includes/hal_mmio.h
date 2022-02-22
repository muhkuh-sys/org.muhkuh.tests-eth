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

#ifndef HALDEC
#define HALDEC(name) name
#endif

void          HALDEC(MMIO_SetCfg)              ( unsigned int uMmioNo, MMIO_CONFIG_E eFunction, unsigned int uInvertOut, unsigned int uInvertIn );
void          HALDEC(MMIO_GetCfg)              ( unsigned int uMmioNo, MMIO_CONFIG_E* peFunction, unsigned int* puInvertOut, unsigned int* puInvertIn );
void          HALDEC(MMIO_SetMode)             ( unsigned int uMmioNo, MMIO_MODE_E eMode );
unsigned int  HALDEC(MMIO_GetInput)            ( unsigned int uMmioNo );
unsigned int  HALDEC(MMIO_GetOutput)           ( unsigned int uMmioNo );
void          HALDEC(MMIO_SetOutput)           ( unsigned int uMmioNo, unsigned int uEnable );
void          HALDEC(MMIO_SetOeLine)           ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          HALDEC(MMIO_GetOeLine)           ( uint32_t* pulLine0, uint32_t* pulLine1, uint32_t* pulLine2, uint32_t* pulLine3 );
void          HALDEC(MMIO_SetOutLineCfg)       ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          HALDEC(MMIO_GetOutLineCfg)       ( uint32_t* pulLine0, uint32_t* pulLine1, uint32_t* pulLine2, uint32_t* pulLine3 );
void          HALDEC(MMIO_GetInLineStatus)     ( uint32_t* pulLine0, uint32_t* pulLine1, uint32_t* pulLine2, uint32_t* pulLine3 );
uint32_t      HALDEC(MMIO_GetInLine)           ( unsigned int uiLine );
void          HALDEC(MMIO_SetCfgReg)           ( unsigned int uMmioNo, uint32_t ulSetCfg );
void          HALDEC(MMIO_GetCfgReg)           ( unsigned int uMmioNo, uint32_t* pulGetCfg );
void          HALDEC(MMIO_SetCfgRaw)           ( unsigned int uMmioNo, uint32_t ulSetCfg );
void          HALDEC(MMIO_PioOutLineCfgSet)    ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          HALDEC(MMIO_PioOutLineCfgRst)    ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          HALDEC(MMIO_PioOeLineCfgSet)     ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );
void          HALDEC(MMIO_PioOeLineCfgRst)     ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 );

#define MMIO_SetLine  MMIO_SetOutLineCfg
#define MMIO_GetLine  MMIO_GetInLineStatus

#endif /* #ifndef __HAL_MMIO_H */
