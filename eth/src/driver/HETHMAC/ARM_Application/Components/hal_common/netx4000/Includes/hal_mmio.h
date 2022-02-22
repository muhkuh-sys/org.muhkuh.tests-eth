#ifndef __HAL_MMIO_H
#define __HAL_MMIO_H

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum
{
  MMIO_CONFIG_GPIO0 = 0x00,                   
  MMIO_CONFIG_GPIO1,                   
  MMIO_CONFIG_GPIO2,                  
  MMIO_CONFIG_GPIO3,                   
  MMIO_CONFIG_GPIO4,                   
  MMIO_CONFIG_GPIO5,                   
  MMIO_CONFIG_GPIO6,                   
  MMIO_CONFIG_GPIO7,                   
  MMIO_CONFIG_GPIO8,                   
  MMIO_CONFIG_GPIO9,                   
  MMIO_CONFIG_GPIO10,                  
  MMIO_CONFIG_GPIO11,                  
  MMIO_CONFIG_GPIO12,                  
  MMIO_CONFIG_GPIO13,                  
  MMIO_CONFIG_GPIO14,                  
  MMIO_CONFIG_GPIO15,                  
  MMIO_CONFIG_IOLINK0_IN,              
  MMIO_CONFIG_IOLINK0_OUT,            
  MMIO_CONFIG_IOLINK0_OE,              
  MMIO_CONFIG_IOLINK0_WAKEUP,          
  MMIO_CONFIG_IOLINK1_IN,              
  MMIO_CONFIG_IOLINK1_OUT,             
  MMIO_CONFIG_IOLINK1_OE,              
  MMIO_CONFIG_IOLINK1_WAKEUP,          
  MMIO_CONFIG_IOLINK2_IN,              
  MMIO_CONFIG_IOLINK2_OUT,             
  MMIO_CONFIG_IOLINK2_OE,              
  MMIO_CONFIG_IOLINK2_WAKEUP,          
  MMIO_CONFIG_IOLINK3_IN,              
  MMIO_CONFIG_IOLINK3_OUT,            
  MMIO_CONFIG_IOLINK3_OE,              
  MMIO_CONFIG_IOLINK3_WAKEUP,          
  MMIO_CONFIG_IOLINK4_IN,              
  MMIO_CONFIG_IOLINK4_OUT,             
  MMIO_CONFIG_IOLINK4_OE,              
  MMIO_CONFIG_IOLINK4_WAKEUP,          
  MMIO_CONFIG_IOLINK5_IN,              
  MMIO_CONFIG_IOLINK5_OUT,             
  MMIO_CONFIG_IOLINK5_OE,              
  MMIO_CONFIG_IOLINK5_WAKEUP,          
  MMIO_CONFIG_IOLINK6_IN,              
  MMIO_CONFIG_IOLINK6_OUT,             
  MMIO_CONFIG_IOLINK6_OE,              
  MMIO_CONFIG_IOLINK6_WAKEUP,          
  MMIO_CONFIG_IOLINK7_IN,              
  MMIO_CONFIG_IOLINK7_OUT,             
  MMIO_CONFIG_IOLINK7_OE,              
  MMIO_CONFIG_IOLINK7_WAKEUP,          
  MMIO_CONFIG_PIO0,                    
  MMIO_CONFIG_PIO1,                    
  MMIO_CONFIG_PIO2,                    
  MMIO_CONFIG_PIO3,                    
  MMIO_CONFIG_PIO4,                    
  MMIO_CONFIG_PIO5,                    
  MMIO_CONFIG_PIO6,                    
  MMIO_CONFIG_PIO7,                    
  MMIO_CONFIG_WDG_ACTIVE,              
  MMIO_CONFIG_EN_IN,                   
  MMIO_CONFIG_SPI0_CLK,                
  MMIO_CONFIG_SPI0_CS0N,               
  MMIO_CONFIG_SPI0_CS1N,               
  MMIO_CONFIG_SPI0_CS2N,              
  MMIO_CONFIG_SPI0_MISO,               
  MMIO_CONFIG_SPI0_MOSI,               
  MMIO_CONFIG_SPI1_CLK,                
  MMIO_CONFIG_SPI1_CS0N,               
  MMIO_CONFIG_SPI1_CS1N,               
  MMIO_CONFIG_SPI1_CS2N,               
  MMIO_CONFIG_SPI1_MISO,               
  MMIO_CONFIG_SPI1_MOSI,               
  MMIO_CONFIG_I2C0_SCL,                
  MMIO_CONFIG_I2C0_SDA,                
  MMIO_CONFIG_I2C1_SCL,                
  MMIO_CONFIG_I2C1_SDA,                
  MMIO_CONFIG_I2C2_SCL,                
  MMIO_CONFIG_I2C2_SDA,                
  MMIO_CONFIG_UART0_CTSN,              
  MMIO_CONFIG_UART0_RTSN,              
  MMIO_CONFIG_UART0_RXD,               
  MMIO_CONFIG_UART0_TXD,               
  MMIO_CONFIG_UART1_CTSN,              
  MMIO_CONFIG_UART1_RTSN,              
  MMIO_CONFIG_UART1_RXD,               
  MMIO_CONFIG_UART1_TXD,               
  MMIO_CONFIG_UART2_CTSN,              
  MMIO_CONFIG_UART2_RTSN,              
  MMIO_CONFIG_UART2_RXD,               
  MMIO_CONFIG_UART2_TXD,               
  MMIO_CONFIG_UART_XPIC3_CTSN,
  MMIO_CONFIG_UART_XPIC3_RTSN,
  MMIO_CONFIG_UART_XPIC3_RXD,
  MMIO_CONFIG_UART_XPIC3_TXD,
  MMIO_CONFIG_CAN_RX,                  
  MMIO_CONFIG_CAN_TX,                  
  MMIO_CONFIG_PWM_FAILURE_N,           
  MMIO_CONFIG_POS_ENC0_A,              
  MMIO_CONFIG_POS_ENC0_B,              
  MMIO_CONFIG_POS_ENC0_N,              
  MMIO_CONFIG_POS_ENC1_A,             
  MMIO_CONFIG_POS_ENC1_B,              
  MMIO_CONFIG_POS_ENC1_N,              
  MMIO_CONFIG_POS_MP0,                 
  MMIO_CONFIG_POS_MP1,                 
  MMIO_CONFIG_XC0_SAMPLE0,             
  MMIO_CONFIG_XC0_SAMPLE1,             
  MMIO_CONFIG_XC0_TRIGGER0,            
  MMIO_CONFIG_XC0_TRIGGER1,            
  MMIO_CONFIG_XC1_SAMPLE0,             
  MMIO_CONFIG_XC1_SAMPLE1,             
  MMIO_CONFIG_XC1_TRIGGER0,            
  MMIO_CONFIG_XC1_TRIGGER1,            
  MMIO_CONFIG_MII_MDC,                 
  MMIO_CONFIG_MII_MDIO,                
  MMIO_CONFIG_XM10_MII_MDC,            
  MMIO_CONFIG_XM10_MII_MDIO,           
  MMIO_CONFIG_XM11_MII_MDC,            
  MMIO_CONFIG_XM11_MII_MDIO,           
  MMIO_CONFIG_XM10_MII_IRQ,            
  MMIO_CONFIG_XM11_MII_IRQ,            
  MMIO_CONFIG_PHY0_LED_PHY_CTRL_LNK,   
  MMIO_CONFIG_PHY0_LED_PHY_CTRL_ACT,   
  MMIO_CONFIG_PHY0_LED_SPD,            
  MMIO_CONFIG_PHY0_LED_DPX,            
  MMIO_CONFIG_PHY1_LED_PHY_CTRL_LNK,   
  MMIO_CONFIG_PHY1_LED_PHY_CTRL_ACT,  
  MMIO_CONFIG_PHY1_LED_SPD,            
  MMIO_CONFIG_PHY1_LED_DPX,            
  MMIO_CONFIG_PHY2_LED_PHY_CTRL_LNK,   
  MMIO_CONFIG_PHY2_LED_PHY_CTRL_ACT,   
  MMIO_CONFIG_PHY2_LED_LNK,            
  MMIO_CONFIG_PHY2_LED_ACT,            
  MMIO_CONFIG_PHY2_LED_SPD,           
  MMIO_CONFIG_PHY2_LED_DPX,            
  MMIO_CONFIG_PHY3_LED_PHY_CTRL_LNK,   
  MMIO_CONFIG_PHY3_LED_PHY_CTRL_ACT,   
  MMIO_CONFIG_PHY3_LED_LNK,            
  MMIO_CONFIG_PHY3_LED_ACT,            
  MMIO_CONFIG_PHY3_LED_SPD,           
  MMIO_CONFIG_PHY3_LED_DPX,            
  MMIO_CONFIG_XC00_IO0,                
  MMIO_CONFIG_XC00_IO1,                
  MMIO_CONFIG_XC00_IO2,                
  MMIO_CONFIG_XC00_IO3,                
  MMIO_CONFIG_XC00_IO4,                
  MMIO_CONFIG_XC00_IO5,                
  MMIO_CONFIG_XC00_RX,                 
  MMIO_CONFIG_XC00_TX_OUT,             
  MMIO_CONFIG_XC01_IO0,                
  MMIO_CONFIG_XC01_IO1,                
  MMIO_CONFIG_XC01_IO2,                
  MMIO_CONFIG_XC01_IO3,                
  MMIO_CONFIG_XC01_IO4,                
  MMIO_CONFIG_XC01_IO5,                
  MMIO_CONFIG_XC01_RX,                 
  MMIO_CONFIG_XC01_TX_OUT,             
  MMIO_CONFIG_XC10_IO0,                
  MMIO_CONFIG_XC10_IO1,                
  MMIO_CONFIG_XC10_IO2,                
  MMIO_CONFIG_XC10_IO3,                
  MMIO_CONFIG_XC10_IO4,                
  MMIO_CONFIG_XC10_IO5,                
  MMIO_CONFIG_XC10_RX,                 
  MMIO_CONFIG_XC10_TX_OUT,             
  MMIO_CONFIG_XC11_IO0,                
  MMIO_CONFIG_XC11_IO1,                
  MMIO_CONFIG_XC11_IO2,                
  MMIO_CONFIG_XC11_IO3,                
  MMIO_CONFIG_XC11_IO4,                
  MMIO_CONFIG_XC11_IO5,                
  MMIO_CONFIG_XC11_RX,                 
  MMIO_CONFIG_XC11_TX_OUT,             
  MMIO_CONFIG_PIO_MODE = 0xff
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
