#ifndef __NETX51_MMIO_H
#define __NETX51_MMIO_H

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum NX51_MMIO_CONFIG_Etag {
  NX51_MMIO_CONFIG_XM0_IO0         = 0x00,
  NX51_MMIO_CONFIG_XM0_IO1         = 0x01,
  NX51_MMIO_CONFIG_XM0_IO2         = 0x02,
  NX51_MMIO_CONFIG_XM0_IO3         = 0x03,
  NX51_MMIO_CONFIG_XM0_IO4         = 0x04,
  NX51_MMIO_CONFIG_XM0_IO5         = 0x05,
  NX51_MMIO_CONFIG_XM0_RX          = 0x06,
  NX51_MMIO_CONFIG_XM0_TX_OUT      = 0x07,
  NX51_MMIO_CONFIG_XM1_IO0         = 0x08,
  NX51_MMIO_CONFIG_XM1_IO1         = 0x09,
  NX51_MMIO_CONFIG_XM1_IO2         = 0x0a,
  NX51_MMIO_CONFIG_XM1_IO3         = 0x0b,
  NX51_MMIO_CONFIG_XM1_IO4         = 0x0c,
  NX51_MMIO_CONFIG_XM1_IO5         = 0x0d,
  NX51_MMIO_CONFIG_XM1_RX          = 0x0e,
  NX51_MMIO_CONFIG_XM1_TX_OUT      = 0x0f,
  NX51_MMIO_CONFIG_GPIO0           = 0x10,
  NX51_MMIO_CONFIG_IO_LINK0_WU     = 0x10,
  NX51_MMIO_CONFIG_GPIO1           = 0x11,
  NX51_MMIO_CONFIG_IO_LINK0_OUT    = 0x11,
  NX51_MMIO_CONFIG_GPIO2           = 0x12,
  NX51_MMIO_CONFIG_IO_LINK0_OE     = 0x12,
  NX51_MMIO_CONFIG_GPIO3           = 0x13,
  NX51_MMIO_CONFIG_IO_LINK0_IN     = 0x13,
  NX51_MMIO_CONFIG_GPIO4           = 0x14,
  NX51_MMIO_CONFIG_IO_LINK1_WU     = 0x14,
  NX51_MMIO_CONFIG_GPIO5           = 0x15,
  NX51_MMIO_CONFIG_IO_LINK1_OUT    = 0x15,
  NX51_MMIO_CONFIG_GPIO6           = 0x16,
  NX51_MMIO_CONFIG_IO_LINK1_OE     = 0x16,
  NX51_MMIO_CONFIG_GPIO7           = 0x17,
  NX51_MMIO_CONFIG_IO_LINK1_IN     = 0x17,
  NX51_MMIO_CONFIG_GPIO8           = 0x18,
  NX51_MMIO_CONFIG_IO_LINK2_WU     = 0x18,
  NX51_MMIO_CONFIG_GPIO9           = 0x19,
  NX51_MMIO_CONFIG_IO_LINK2_OUT    = 0x19,
  NX51_MMIO_CONFIG_GPIO10          = 0x1a,
  NX51_MMIO_CONFIG_IO_LINK2_OE     = 0x1a,
  NX51_MMIO_CONFIG_GPIO11          = 0x1b,
  NX51_MMIO_CONFIG_IO_LINK2_IN     = 0x1b,
  NX51_MMIO_CONFIG_GPIO12          = 0x1c,
  NX51_MMIO_CONFIG_IO_LINK3_WU     = 0x1c,
  NX51_MMIO_CONFIG_GPIO13          = 0x1d,
  NX51_MMIO_CONFIG_IO_LINK3_OUT    = 0x1d,
  NX51_MMIO_CONFIG_GPIO14          = 0x1e,
  NX51_MMIO_CONFIG_IO_LINK3_OE     = 0x1e,
  NX51_MMIO_CONFIG_GPIO15          = 0x1f,
  NX51_MMIO_CONFIG_IO_LINK3_IN     = 0x1f,
  NX51_MMIO_CONFIG_GPIO16          = 0x20,
  NX51_MMIO_CONFIG_IO_LINK4_WU     = 0x20,
  NX51_MMIO_CONFIG_GPIO17          = 0x21,
  NX51_MMIO_CONFIG_IO_LINK4_OUT    = 0x21,
  NX51_MMIO_CONFIG_GPIO18          = 0x22,
  NX51_MMIO_CONFIG_IO_LINK4_OE     = 0x22,
  NX51_MMIO_CONFIG_GPIO19          = 0x23,
  NX51_MMIO_CONFIG_IO_LINK4_IN     = 0x23,
  NX51_MMIO_CONFIG_GPIO20          = 0x24,
  NX51_MMIO_CONFIG_IO_LINK5_WU     = 0x24,
  NX51_MMIO_CONFIG_GPIO21          = 0x25,
  NX51_MMIO_CONFIG_IO_LINK5_OUT    = 0x25,
  NX51_MMIO_CONFIG_GPIO22          = 0x26,
  NX51_MMIO_CONFIG_IO_LINK5_OE     = 0x26,
  NX51_MMIO_CONFIG_GPIO23          = 0x27,
  NX51_MMIO_CONFIG_IO_LINK5_IN     = 0x27,
  NX51_MMIO_CONFIG_GPIO24          = 0x28,
  NX51_MMIO_CONFIG_IO_LINK6_WU     = 0x28,
  NX51_MMIO_CONFIG_GPIO25          = 0x29,
  NX51_MMIO_CONFIG_IO_LINK6_OUT    = 0x29,
  NX51_MMIO_CONFIG_GPIO26          = 0x2a,
  NX51_MMIO_CONFIG_IO_LINK6_OE     = 0x2a,
  NX51_MMIO_CONFIG_GPIO27          = 0x2b,
  NX51_MMIO_CONFIG_IO_LINK6_IN     = 0x2b,
  NX51_MMIO_CONFIG_GPIO28          = 0x2c,
  NX51_MMIO_CONFIG_IO_LINK7_WU     = 0x2c,
  NX51_MMIO_CONFIG_GPIO29          = 0x2d,
  NX51_MMIO_CONFIG_IO_LINK7_OUT    = 0x2d,
  NX51_MMIO_CONFIG_GPIO30          = 0x2e,
  NX51_MMIO_CONFIG_IO_LINK7_OE     = 0x2e,
  NX51_MMIO_CONFIG_GPIO31          = 0x2f,
  NX51_MMIO_CONFIG_IO_LINK7_IN     = 0x2f,
  NX51_MMIO_CONFIG_PHY0_LED0       = 0x30,
  NX51_MMIO_CONFIG_PHY0_LED1       = 0x31,
  NX51_MMIO_CONFIG_PHY0_LED2       = 0x32,
  NX51_MMIO_CONFIG_PHY0_LED3       = 0x33,
  NX51_MMIO_CONFIG_PHY1_LED0       = 0x34,
  NX51_MMIO_CONFIG_PHY1_LED1       = 0x35,
  NX51_MMIO_CONFIG_PHY1_LED2       = 0x36,
  NX51_MMIO_CONFIG_PHY1_LED3       = 0x37,
  NX51_MMIO_CONFIG_MII_MDC         = 0x38,
  NX51_MMIO_CONFIG_MII_MDIO        = 0x39,
  NX51_MMIO_CONFIG_SPI0_CS2N       = 0x3a,
  NX51_MMIO_CONFIG_SPI0_SIO2_MMIO  = 0x3b,
  NX51_MMIO_CONFIG_SPI0_SIO3_MMIO  = 0x3c,
  NX51_MMIO_CONFIG_SPI1_CLK        = 0x3d,
  NX51_MMIO_CONFIG_SPI1_CS0N       = 0x3e,
  NX51_MMIO_CONFIG_SPI1_CS1N       = 0x3f,
  NX51_MMIO_CONFIG_SPI1_CS2N       = 0x40,
  NX51_MMIO_CONFIG_SPI1_MISO       = 0x41,
  NX51_MMIO_CONFIG_SPI1_MOSI       = 0x42,
  NX51_MMIO_CONFIG_I2C0_SCL_MMIO   = 0x43,
  NX51_MMIO_CONFIG_I2C0_SDA_MMIO   = 0x44,
  NX51_MMIO_CONFIG_I2C1_SCL        = 0x45,
  NX51_MMIO_CONFIG_I2C1_SDA        = 0x46,
  NX51_MMIO_CONFIG_XC_SAMPLE0      = 0x47,
  NX51_MMIO_CONFIG_XC_SAMPLE1      = 0x48,
  NX51_MMIO_CONFIG_XC_TRIGGER0     = 0x49,
  NX51_MMIO_CONFIG_XC_TRIGGER1     = 0x4a,
  NX51_MMIO_CONFIG_UART0_CTS       = 0x4b,
  NX51_MMIO_CONFIG_UART0_RTS       = 0x4c,
  NX51_MMIO_CONFIG_UART0_RXD       = 0x4d,
  NX51_MMIO_CONFIG_UART0_TXD       = 0x4e,
  NX51_MMIO_CONFIG_UART1_CTS       = 0x4f,
  NX51_MMIO_CONFIG_UART1_RTS       = 0x50,
  NX51_MMIO_CONFIG_UART1_RXD       = 0x51,
  NX51_MMIO_CONFIG_UART1_TXD       = 0x52,
  NX51_MMIO_CONFIG_UART2_CTS       = 0x53,
  NX51_MMIO_CONFIG_UART2_RTS       = 0x54,
  NX51_MMIO_CONFIG_UART2_RXD       = 0x55,
  NX51_MMIO_CONFIG_UART2_TXD       = 0x56,
  NX51_MMIO_CONFIG_CAN_RX          = 0x57,
  NX51_MMIO_CONFIG_CAN_TX          = 0x58,
  NX51_MMIO_CONFIG_MEM_RDY         = 0x59,
  NX51_MMIO_CONFIG_PIO0            = 0x5a,
  NX51_MMIO_CONFIG_PIO1            = 0x5b,
  NX51_MMIO_CONFIG_PIO2            = 0x5c,
  NX51_MMIO_CONFIG_PIO3            = 0x5d,
  NX51_MMIO_CONFIG_PIO4            = 0x5e,
  NX51_MMIO_CONFIG_PIO5            = 0x5f,
  NX51_MMIO_CONFIG_PIO6            = 0x60,
  NX51_MMIO_CONFIG_PIO7            = 0x61,
  NX51_MMIO_CONFIG_PIO_MODE        = 0x7f,
  NX51_MMIO_CONFIG_INPUT           = 0x7f
} NX51_MMIO_CONFIG_E;

typedef enum NX51_MMIO_MODE_Etag {
  NX51_MMIO_MODE_IN  = 0,
  NX51_MMIO_MODE_OUT = 1
} NX51_MMIO_MODE_E;


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void         NX51_MMIO_SetCfg( unsigned int uMmioNum, NX51_MMIO_CONFIG_E eFunction, unsigned int uInvertOut, unsigned int uInvertIn );
void         NX51_MMIO_GetCfg( unsigned int uMmioNum, NX51_MMIO_CONFIG_E* peFunction, unsigned int* puInvertOut, unsigned int* puInvertIn );
void         NX51_MMIO_SetMode( unsigned int uMmioNum, NX51_MMIO_MODE_E eMode );
unsigned int NX51_MMIO_GetInput( unsigned int uMmioNum );
uint32_t     NX51_MMIO_GetIn( void );
unsigned int NX51_MMIO_GetOutput( unsigned int uMmioNum );
void         NX51_MMIO_SetOutput( unsigned int uMmioNum, unsigned int uEnable );
void         NX51_MMIO_GetLine( uint32_t* pulLine0, uint32_t* pulLine1 );
void         NX51_MMIO_SetLine( uint32_t ulLine0, uint32_t ulLine1 );


#endif /* #ifndef __NETX51_MMIO_H */
