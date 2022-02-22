#ifndef __COMMON_FUNC_NETX50_H
#define __COMMON_FUNC_NETX50_H

#include "defines.h" // for PEEK, POKE, INTERRUPT
#include "regdef_netx50.h"

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX50_DEV_FREQUENCY 100000000L // 100 Mhz

/*
  _   _      _      ____    _____ 
 | | | |    / \    |  _ \  |_   _|
 | | | |   / _ \   | |_) |   | |  
 | |_| |  / ___ \  |  _ <    | |  
  \___/  /_/   \_\ |_| \_\   |_|  

*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX50_GPIO_CFG_IN            0
#define NX50_GPIO_CFG_EXT_0         2
#define SerialPutChar(c) NX50_UART_PutCharacter(0,c)

typedef enum UART_BAUDRATEtag {
        UART_BAUDRATE_300    =    3,
        UART_BAUDRATE_600    =    6,
        UART_BAUDRATE_1200   =   12,
        UART_BAUDRATE_2400   =   24,
        UART_BAUDRATE_4800   =   48,
        UART_BAUDRATE_9600   =   96,
        UART_BAUDRATE_19200  =  192,
        UART_BAUDRATE_38400  =  384,
        UART_BAUDRATE_57600  =  576,
        UART_BAUDRATE_115200 = 1152
} NX50_UART_BAUDRATE;

#define NX50_DEV_BAUDRATE UART_BAUDRATE_115200

// DEV_BAUDRATE is 100 times to small -> multiply with 100 (or divide by DEV_FREQUENCY/100)
#define NX50_DEV_BAUDRATE_DIV_LO (((NX50_DEV_BAUDRATE*16*65536)/(NX50_DEV_FREQUENCY/100))&0xff)
#define NX50_DEV_BAUDRATE_DIV_HI (((NX50_DEV_BAUDRATE*16*65536)/(NX50_DEV_FREQUENCY/100))>>8) 

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_UART_InitSerial(unsigned int ulUartNr);
void NX50_UART_CloseSerial(unsigned int ulUartNr);
void NX50_UART_PutCharacter(unsigned int ulUartNr, const unsigned char bChar);
int  NX50_UART_GetCharacter(unsigned int ulUartNr);
int  NX50_UART_Peek(unsigned int ulUartNr);
void NX50_UART_UPrintf(const char* pchFmt, ...);
void NX50_UART_UPrintfDec(const char* pchFmt, ...);

/*
 __     __  ___    ____ 
 \ \   / / |_ _|  / ___|
  \ \ / /   | |  | |    
   \ V /    | |  | |___ 
    \_/    |___|  \____|
                        
*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
typedef void (*pp_func)(void);

#define MAX_IRQ_VECTORS             16
#define ARM9_Vect_Undefined 0x00000004    
#define ARM9_Vect_SWI       0x00000008    
#define ARM9_Vect_Prefetch  0x0000000c    
#define ARM9_Vect_Abort     0x00000010    
#define ARM9_Vect_IRQ       0x00000018
#define ARM9_Vect_FIQ       0x0000001c    

#define NX50_IRQFIQ_LOCK     asm ( "msr cpsr_c, #0xdf" );
#define NX50_IRQFIQ_UNLOCK   asm ( "msr cpsr_c, #0x1f" );

#define NX50_lock_irqfiq_save(x)     (x = NX50_LockIrqSave())
#define NX50_lock_irqfiq_restore(x)  (NX50_LockIrqRestore(x))

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_VIC_InstallIsrVector( pp_func ulVector);
void NX50_VIC_ClearVic(void);
void INTERRUPT NX50_VIC_IRQVectorHandler_C(void);
void NX50_VIC_ISR_Default(void);
void NX50_VIC_SetDefaultVector(pp_func default_handler);
void NX50_VIC_SetIrqVector(unsigned int vec, unsigned long src, pp_func irq_handler);
void NX50_VIC_EnableIrqSources(unsigned long src);

unsigned int   NX50_LockIrqSave(void);
void           NX50_LockIrqRestore(unsigned int uiState);

/*
   ____   ____    ___    ___  
  / ___| |  _ \  |_ _|  / _ \ 
 | |  _  | |_) |  | |  | | | |
 | |_| | |  __/   | |  | |_| |
  \____| |_|     |___|  \___/ 
                              
*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX50_GPIO_MODE_NOINVERT                   0x0
#define NX50_GPIO_MODE_INVERT                    0x10
#define NX50_GPIO_MODE_INPUT_READ                 0x0
#define NX50_GPIO_MODE_OUTPUT_SET_TO_0            0x4
#define NX50_GPIO_MODE_OUTPUT_SET_TO_1            0x5
#define NX50_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE    0x6

#define MSK_GPIO_COUNTER0_2  (MSK_NX50_cnt_irq_raw_cnt0 | \
                              MSK_NX50_cnt_irq_raw_cnt1 | \
                              MSK_NX50_cnt_irq_raw_cnt2)

#define MSK_GPIO_COUNTER3_4  (MSK_NX50_cnt_irq_raw_cnt3 | \
                              MSK_NX50_cnt_irq_raw_cnt4)

#define NX50_GPIO_ConfirmTimerIrq(cnt) NX50_GPIO_ConfirmIrq(Adr_NX50_gpio_cnt_irq_raw, (1 << (cnt)))
#define NX50_GPIO_SetupTimer(cno, time) NX50_GPIO_PrepareTimer(cno, time * (NX50_DEV_FREQUENCY/1000000), 1); NX50_GPIO_StartTimer(cno)

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int   NX50_GPIO_GetIrq (unsigned long ulGpioNum);
int   NX50_GPIO_GetInput (unsigned long ulGpioNum);
unsigned long NX50_GPIO_GetIn( void );
void  NX50_GPIO_SetupMode (unsigned long ulGpioNum, unsigned long ulMode, unsigned long ulInvert);
void  NX50_GPIO_SetOutput (unsigned long ulGpioNum, int fEnable);
unsigned long NX50_GPIO_GetLine( void );
void  NX50_GPIO_SetLine( unsigned long ulVal );
void  NX50_GPIO_IrqEnable (unsigned long ulGPIOnum);
void NX50_GPIO_ConfirmIrq( unsigned long ulConfirm, unsigned long ulMask );
void  NX50_GPIO_Sleep (unsigned long ulCounter, unsigned int uiTimeout);
void NX50_GPIO_ResetTimer( unsigned int uCounter );
void NX50_GPIO_PrepareTimer( unsigned int uCounter, unsigned int uiTimeout, unsigned int uiOnce );
void NX50_GPIO_StartTimer( unsigned int uCounter );
int NX50_GPIO_IsTimerRunning( unsigned long ulCounter );
void NX50_GPIO_EnableTimerIrq( unsigned int uCounter );


/*
  ____    ___    ___  
 |  _ \  |_ _|  / _ \ 
 | |_) |  | |  | | | |
 |  __/   | |  | |_| |
 |_|     |___|  \___/ 
                      
*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX50_PIO_MODE_IN  0
#define NX50_PIO_MODE_OUT 1

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_PIO_SetMode (unsigned long ulPioNum, unsigned long ulMode);
int  NX50_PIO_GetInput (unsigned long ulPioNum);
unsigned long NX50_PIO_GetIn( void );
int  NX50_PIO_GetOutput (unsigned long ulPioNum);
void NX50_PIO_SetOutput (unsigned long ulPioNum, int fEnable);
unsigned long NX50_PIO_GetLine ( void );
void NX50_PIO_SetLine( unsigned long ulVal );

/*
  __  __   __  __   ___    ___  
 |  \/  | |  \/  | |_ _|  / _ \ 
 | |\/| | | |\/| |  | |  | | | |
 | |  | | | |  | |  | |  | |_| |
 |_|  |_| |_|  |_| |___|  \___/ 
                                
*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define MMIO_CONFIG_XM0_IO0            0x00
#define MMIO_CONFIG_XM0_IO1            0x01
#define MMIO_CONFIG_XM0_IO2            0x02
#define MMIO_CONFIG_XM0_IO3            0x03
#define MMIO_CONFIG_XM0_IO4            0x04
#define MMIO_CONFIG_XM0_IO5            0x05
#define MMIO_CONFIG_XM0_RX             0x06
#define MMIO_CONFIG_XM0_TX_OE          0x07
#define MMIO_CONFIG_XM0_TX_OUT         0x08
#define MMIO_CONFIG_XM1_IO0            0x09
#define MMIO_CONFIG_XM1_IO1            0x0a
#define MMIO_CONFIG_XM1_IO2            0x0b
#define MMIO_CONFIG_XM1_IO3            0x0c
#define MMIO_CONFIG_XM1_IO4            0x0d
#define MMIO_CONFIG_XM1_IO5            0x0e
#define MMIO_CONFIG_XM1_RX             0x0f
#define MMIO_CONFIG_XM1_TX_OE          0x10
#define MMIO_CONFIG_XM1_TX_OUT         0x11
#define MMIO_CONFIG_GPIO0              0x12
#define MMIO_CONFIG_GPIO1              0x13
#define MMIO_CONFIG_GPIO2              0x14
#define MMIO_CONFIG_GPIO3              0x15
#define MMIO_CONFIG_GPIO4              0x16
#define MMIO_CONFIG_GPIO5              0x17
#define MMIO_CONFIG_GPIO6              0x18
#define MMIO_CONFIG_GPIO7              0x19
#define MMIO_CONFIG_GPIO8              0x1a
#define MMIO_CONFIG_GPIO9              0x1b
#define MMIO_CONFIG_GPIO10             0x1c
#define MMIO_CONFIG_GPIO11             0x1d
#define MMIO_CONFIG_GPIO12             0x1e
#define MMIO_CONFIG_GPIO13             0x1f
#define MMIO_CONFIG_GPIO14             0x20
#define MMIO_CONFIG_GPIO15             0x21
#define MMIO_CONFIG_GPIO16             0x22
#define MMIO_CONFIG_GPIO17             0x23
#define MMIO_CONFIG_GPIO18             0x24
#define MMIO_CONFIG_GPIO19             0x25
#define MMIO_CONFIG_GPIO20             0x26
#define MMIO_CONFIG_GPIO21             0x27
#define MMIO_CONFIG_GPIO22             0x28
#define MMIO_CONFIG_GPIO23             0x29
#define MMIO_CONFIG_GPIO24             0x2a
#define MMIO_CONFIG_GPIO25             0x2b
#define MMIO_CONFIG_GPIO26             0x2c
#define MMIO_CONFIG_GPIO27             0x2d
#define MMIO_CONFIG_GPIO28             0x2e
#define MMIO_CONFIG_GPIO29             0x2f
#define MMIO_CONFIG_GPIO30             0x30
#define MMIO_CONFIG_GPIO31             0x31
#define MMIO_CONFIG_PHY0_LED0          0x32
#define MMIO_CONFIG_PHY0_LED1          0x33
#define MMIO_CONFIG_PHY0_LED2          0x34
#define MMIO_CONFIG_PHY0_LED3          0x35
#define MMIO_CONFIG_PHY1_LED0          0x36
#define MMIO_CONFIG_PHY1_LED1          0x37
#define MMIO_CONFIG_PHY1_LED2          0x38
#define MMIO_CONFIG_PHY1_LED3          0x39
#define MMIO_CONFIG_MII_MDC            0x3a
#define MMIO_CONFIG_MII_MDIO           0x3b
#define MMIO_CONFIG_MII0_COL           0x3c
#define MMIO_CONFIG_MII0_CRS           0x3d
#define MMIO_CONFIG_MII0_LED0          0x3e
#define MMIO_CONFIG_MII0_LED1          0x3f
#define MMIO_CONFIG_MII0_LED2          0x40
#define MMIO_CONFIG_MII0_LED3          0x41
#define MMIO_CONFIG_MII0_RXCLK         0x42
#define MMIO_CONFIG_MII0_RXD0          0x43
#define MMIO_CONFIG_MII0_RXD1          0x44
#define MMIO_CONFIG_MII0_RXD2          0x45
#define MMIO_CONFIG_MII0_RXD3          0x46
#define MMIO_CONFIG_MII0_RXDV          0x47
#define MMIO_CONFIG_MII0_RXER          0x48
#define MMIO_CONFIG_MII0_TXCLK         0x49
#define MMIO_CONFIG_MII0_TXD0          0x4a
#define MMIO_CONFIG_MII0_TXD1          0x4b
#define MMIO_CONFIG_MII0_TXD2          0x4c
#define MMIO_CONFIG_MII0_TXD3          0x4d
#define MMIO_CONFIG_MII0_TXEN          0x4e
#define MMIO_CONFIG_MII0_TXER          0x4f
#define MMIO_CONFIG_MII1_COL           0x50
#define MMIO_CONFIG_MII1_CRS           0x51
#define MMIO_CONFIG_MII1_LED0          0x52
#define MMIO_CONFIG_MII1_LED1          0x53
#define MMIO_CONFIG_MII1_LED2          0x54
#define MMIO_CONFIG_MII1_LED3          0x55
#define MMIO_CONFIG_MII1_RXCLK         0x56
#define MMIO_CONFIG_MII1_RXD0          0x57
#define MMIO_CONFIG_MII1_RXD1          0x58
#define MMIO_CONFIG_MII1_RXD2          0x59
#define MMIO_CONFIG_MII1_RXD3          0x5a
#define MMIO_CONFIG_MII1_RXDV          0x5b
#define MMIO_CONFIG_MII1_RXER          0x5c
#define MMIO_CONFIG_MII1_TXCLK         0x5d
#define MMIO_CONFIG_MII1_TXD0          0x5e
#define MMIO_CONFIG_MII1_TXD1          0x5f
#define MMIO_CONFIG_MII1_TXD2          0x60
#define MMIO_CONFIG_MII1_TXD3          0x61
#define MMIO_CONFIG_MII1_TXEN          0x62
#define MMIO_CONFIG_MII1_TXER          0x63
#define MMIO_CONFIG_PIO0               0x64
#define MMIO_CONFIG_PIO1               0x65
#define MMIO_CONFIG_PIO2               0x66
#define MMIO_CONFIG_PIO3               0x67
#define MMIO_CONFIG_PIO4               0x68
#define MMIO_CONFIG_PIO5               0x69
#define MMIO_CONFIG_PIO6               0x6a
#define MMIO_CONFIG_PIO7               0x6b
#define MMIO_CONFIG_SPI0_CS2N          0x6c
#define MMIO_CONFIG_SPI1_CLK           0x6d
#define MMIO_CONFIG_SPI1_CS0N          0x6e
#define MMIO_CONFIG_SPI1_CS1N          0x6f
#define MMIO_CONFIG_SPI1_CS2N          0x70
#define MMIO_CONFIG_SPI1_MISO          0x71
#define MMIO_CONFIG_SPI1_MOSI          0x72
#define MMIO_CONFIG_I2C_SCL_MMIO       0x73
#define MMIO_CONFIG_I2C_SDA_MMIO       0x74
#define MMIO_CONFIG_XC_SAMPLE0         0x75
#define MMIO_CONFIG_XC_SAMPLE1         0x76
#define MMIO_CONFIG_XC_TRIGGER0        0x77
#define MMIO_CONFIG_XC_TRIGGER1        0x78
#define MMIO_CONFIG_UART0_CTS          0x79
#define MMIO_CONFIG_UART0_RTS          0x7a
#define MMIO_CONFIG_UART0_RXD          0x7b
#define MMIO_CONFIG_UART0_TXD          0x7c
#define MMIO_CONFIG_UART1_CTS          0x7d
#define MMIO_CONFIG_UART1_RTS          0x7e
#define MMIO_CONFIG_UART1_RXD          0x7f
#define MMIO_CONFIG_UART1_TXD          0x80
#define MMIO_CONFIG_UART2_CTS          0x81
#define MMIO_CONFIG_UART2_RTS          0x82
#define MMIO_CONFIG_UART2_RXD          0x83
#define MMIO_CONFIG_UART2_TXD          0x84
#define MMIO_CONFIG_USB_ID_DIG         0x85
#define MMIO_CONFIG_USB_ID_PULLUP_CTRL 0x86
#define MMIO_CONFIG_USB_RPD_ENA        0x87
#define MMIO_CONFIG_USB_RPU_ENA        0x88
#define MMIO_CONFIG_CCD_DATA0          0x89
#define MMIO_CONFIG_CCD_DATA1          0x8a
#define MMIO_CONFIG_CCD_DATA2          0x8b
#define MMIO_CONFIG_CCD_DATA3          0x8c
#define MMIO_CONFIG_CCD_DATA4          0x8d
#define MMIO_CONFIG_CCD_DATA5          0x8e
#define MMIO_CONFIG_CCD_DATA6          0x8f
#define MMIO_CONFIG_CCD_DATA7          0x90
#define MMIO_CONFIG_CCD_PIXCLK         0x91
#define MMIO_CONFIG_CCD_LINE_VALID     0x92
#define MMIO_CONFIG_CCD_FRAME_VALID    0x93
#define MMIO_CONFIG_INPUT              0xff

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_MMIO_SetCfg( unsigned int uMmioNum, unsigned char bFunction, int fInvertOut, int fInvertIn );

/*
  ____        _           ____                  
 |  _ \    __| |  _   _  |  _ \   _   _   _ __  
 | |_) |  / _` | | | | | | |_) | | | | | | '_ \ 
 |  _ <  | (_| | | |_| | |  _ <  | |_| | | | | |
 |_| \_\  \__,_|  \__, | |_| \_\  \__,_| |_| |_|
                  |___/                         
*/


/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
typedef enum {
  NX50_RDYRUN_LED_OFF = 0,
  NX50_RDYRUN_LED_GREEN,
  NX50_RDYRUN_LED_RED,
  NX50_RDYRUN_LED_INV
} NX50_RDYRUN_LED_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_E tMode);

/*
  ___           _                                   _           ____    _   _  __   __
 |_ _|  _ __   | |_    ___   _ __   _ __     __ _  | |         |  _ \  | | | | \ \ / /
  | |  | '_ \  | __|  / _ \ | '__| | '_ \   / _` | | |         | |_) | | |_| |  \ V / 
  | |  | | | | | |_  |  __/ | |    | | | | | (_| | | |         |  __/  |  _  |   | |  
 |___| |_| |_|  \__|  \___| |_|    |_| |_|  \__,_| |_|  _____  |_|     |_| |_|   |_|  
                                                       |_____|                        
*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX50_INT_PHY1_ADDR                     0x00
#define NX50_INT_PHY2_ADDR                     0x01
#define PHY_CTRL_PHY_ADDR                       0x0 // bits 4..1: phyaddr, bit 0 indicate internal phy 0 or 1
#define PHY_CTRL_PHY1_FX_MODE                     0
#define PHY_CTRL_PHY2_FX_MODE                     0
#define PHY_CTRL_PHY1_AUTOMDIX                    1
#define PHY_CTRL_PHY2_AUTOMDIX                    1
#define PHY_CTRL_PHY1_NP_MSG_CODE                 0
#define PHY_CTRL_PHY2_NP_MSG_CODE                 0
#define PHY_CTRL_PHY1_ENABLE                      1
#define PHY_CTRL_PHY2_ENABLE                      1
#define RESET_PHY_DELAY                    80000000 // reset delay in 100us units


// PHY Control Register
#define PHYCTRL_10BASE_T_HD_NOAUTONEG                       0x00
#define PHYCTRL_10BASE_T_FD_NOAUTONEG                       0x01
#define PHYCTRL_100BASE_TXFX_HD_NOAUTONEG_CRSTXRX           0x02
#define PHYCTRL_100BASE_TXFX_FD_NOAUTONEG_CRSRX             0x03
#define PHYCTRL_100BASE_TX_HD_ADV_AUTONEG_CRSRXTX           0x04
#define PHYCTRL_REPEATER_AUTONEG_100BASE_TX_HD_ADV_CRSRX    0x05
#define PHYCTRL_POWER_DOWN_MODE                             0x06
#define PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN              0x07

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_INTPHY_Init( NX50_PHY_CONTROL_T *ptPhyCtrlInit );
void NX50_INTPHY_CorrectAutoMdix100MBit( unsigned int uMiimuPhyAddr );

/*
  ____            _           _                           _____   _    __         
 |  _ \    ___   (_)  _ __   | |_    ___   _ __          |  ___| (_)  / _|   ___  
 | |_) |  / _ \  | | | '_ \  | __|  / _ \ | '__|         | |_    | | | |_   / _ \ 
 |  __/  | (_) | | | | | | | | |_  |  __/ | |            |  _|   | | |  _| | (_) |
 |_|      \___/  |_| |_| |_|  \__|  \___| |_|     _____  |_|     |_| |_|    \___/ 
                                                 |_____|                          
*/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_PFIFO_Reset( void );
unsigned long NX50_PFIFO_GetFillLevel( unsigned int uFifoNum );
int NX50_PFIFO_SetBorders(unsigned int *auPFifoDepth);
unsigned long NX50_PFIFO_GetFifoResetVector( void );
unsigned long NX50_PFIFO_GetFifoFullVector( void );
unsigned long NX50_PFIFO_GetFifoEmptyVector( void );
unsigned long NX50_PFIFO_GetFifoOverflowVector( void );
unsigned long NX50_PFIFO_GetFifoUnderrunVector( void );


//                                                                  
// ######  #    #  #    #   ####   #####  #   ####   #    #   ####  
// #       #    #  ##   #  #    #    #    #  #    #  ##   #  #      
// #####   #    #  # #  #  #         #    #  #    #  # #  #   ####  
// #       #    #  #  # #  #         #    #  #    #  #  # #       # 
// #       #    #  #   ##  #    #    #    #  #    #  #   ##  #    # 
// #        ####   #    #   ####     #    #   ####   #    #   ####  
//                                                                  
//                                                        #     #     #     #       
// #    #   ####   ######  #####       #####   #   #      #     #    # #    #       
// #    #  #       #       #    #      #    #   # #       #     #   #   #   #       
// #    #   ####   #####   #    #      #####     #        #######  #     #  #       
// #    #       #  #       #    #      #    #    #        #     #  #######  #       
// #    #  #    #  #       #    #      #    #    #        #     #  #     #  #       
//  ####    ####   ######  #####       #####     #        #     #  #     #  ####### 
//                                                                                  

// __  __   ____           _                           _               
// \ \/ /  / ___|         | |       ___     __ _    __| |   ___   _ __ 
//  \  /  | |             | |      / _ \   / _` |  / _` |  / _ \ | '__|
//  /  \  | |___          | |___  | (_) | | (_| | | (_| | |  __/ | |   
// /_/\_\  \____|  _____  |_____|  \___/   \__,_|  \__,_|  \___| |_|   
//                |_____|                                              

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX50_XMAC_RPU_DWORD_RAMSIZE 0x0100
#define NX50_XMAC_TPU_DWORD_RAMSIZE 0x0100
#define NX50_XPEC_DWORD_RAMSIZE     0x0800

typedef enum {
  NX50_XC_TYPE_RPU  = 0,
  NX50_XC_TYPE_TPU  = 1,
  NX50_XC_TYPE_XPEC = 2
} NX50_XC_TYPE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX50_XC_Reset(unsigned int uPortNo, void* pvUser);
int NX50_XC_Load(unsigned int uPortNo, NX50_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser);
int NX50_XC_Start(unsigned int uPortNo, void* pvUser);

//  __  __   ___   ___   __  __   _   _ 
// |  \/  | |_ _| |_ _| |  \/  | | | | |
// | |\/| |  | |   | |  | |\/| | | | | |
// | |  | |  | |   | |  | |  | | | |_| |
// |_|  |_| |___| |___| |_|  |_|  \___/ 
//                                      

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX50_MIIMU_SUPPRESS_PREAMBLE              (0)             
#define NX50_MIIMU_SEND_PREAMBLE                  (1)             
#define NX50_MIIMU_MDCFREQ_HIGH                   (0)             
#define NX50_MIIMU_MDCFREQ_LOW                    (1)             
#define NX50_MIIMU_RTA_0_BITS                     (0)             
#define NX50_MIIMU_RTA_1_BITS                     (1)             

/* NEC CB12 registers */
#define DRV_CB12_CONTROL                     (0)             /* Control                                         */
#define DRV_CB12_STATUS                      (1)             /* Status #1                                       */
#define DRV_CB12_PHY_ID_1                    (2)             /* PHY Identification 1                            */
#define DRV_CB12_PHY_ID_2                    (3)             /* PHY Identification 2                            */
#define DRV_CB12_AUTO_NEG_ADVERTISEMENT      (4)             /* Auto-Negotiation Advertisement                  */
#define DRV_CB12_AUTO_NEG_LINK_PARTNER_ABIL  (5)             /* Auto-Negotiation Link Partner Ability           */
#define DRV_CB12_AUTO_NEG_EXPANSION          (6)             /* Auto-Negotiation Expansion                      */
#define DRV_CB12_AUTO_NEG_NEXT_PAGE_TX       (7)             /* Auto-Negotiation Next Page Transmit             */
#define DRV_CB12_SILICON_REVISION            (16)            /* Silicon Revision                                */
#define DRV_CB12_MODE_CONTROL_STATUS         (17)            /* Mode Control/Status                             */
#define DRV_CB12_SPECIAL_MODES               (18)            /* Special Modes                                   */
#define DRV_CB12_SMII_CONFIGURATION_STATUS   (19)            /* SMII configuration status                       */
#define DRV_CB12_CONTROL_STATUS_INDICATION   (27)            /* Control/Status Indication                       */
#define DRV_CB12_SPECIAL_INT_TEST_CONTROLS   (28)            /* Special internal testability controls           */
#define DRV_CB12_INTERRUPT_SOURCE            (29)            /* Interrupt source                                */
#define DRV_CB12_INTERRUPT_MASK              (30)            /* Interrupt mask                                  */
#define DRV_CB12_PHY_SPECIAL_CONTROL_STATUS  (31)            /* PHY special control/status                      */

/* Register 0 - Basic Control Register Bit Definition */
#define DRV_CB12_CONTROL_RESET                  (0x8000)        /* PHY reset                          */
#define DRV_CB12_CONTROL_LOOPBACK               (0x4000)        /* Enable loopback                    */
#define DRV_CB12_CONTROL_SPEED_SELECT_100       (0x2000)        /* Select Speed 100MBit               */
#define DRV_CB12_CONTROL_AUTO_NEG_ENABLE        (0x1000)        /* Auto-Negotiation Enable            */
#define DRV_CB12_CONTROL_POWER_DOWN             (0x0800)        /* Power-down                         */
#define DRV_CB12_CONTROL_ISOLATE                (0x0400)        /* Electrically Isolate PHY from MII  */
#define DRV_CB12_CONTROL_AUTO_NEG_RESTART       (0x0200)        /* Restart Auto-Negotiation           */
#define DRV_CB12_CONTROL_FULL_DUPLEX            (0x0100)        /* Full Duplex Mode                   */
#define DRV_CB12_CONTROL_COL_TEST               (0x0080)        /* Enable COL signal test             */

/* Register 1 - Basic Status Register Bit Definition */
#define DRV_CB12_STATUS_100_BASE_T4             (0x8000)        /* 100BASE-T4 support                 */
#define DRV_CB12_STATUS_100_BASE_X_FDX          (0x4000)        /* 100BASE-X full duplex support      */
#define DRV_CB12_STATUS_100_BASE_X_HDX          (0x2000)        /* 100BASE-X half duplex support      */
#define DRV_CB12_STATUS_10_MBPS_FDX             (0x1000)        /* 10 Mbps full duplex support        */
#define DRV_CB12_STATUS_10_MBPS_HDX             (0x0800)        /* 10 Mbps half duplex support        */
#define DRV_CB12_STATUS_AUTO_NEG_COMPLETE       (0x0020)        /* Auto-Negotiation complete          */
#define DRV_CB12_STATUS_REMOTE_FAULT            (0x0010)        /* Remote fault detected              */
#define DRV_CB12_STATUS_AUTO_NEG_ABILITY        (0x0008)        /* Auto-Negotiation ability           */
#define DRV_CB12_STATUS_LINK_UP                 (0x0004)        /* Link status                        */
#define DRV_CB12_STATUS_JABBER_DETECT           (0x0002)        /* Jabber detected                    */
#define DRV_CB12_STATUS_EXTENDED_CAPABILITY     (0x0001)        /* Basic/extended register capability */

/* Register 4 - Auto Negotiation Advertisement Register Bit Definition */
#define DRV_CB12_ADV_NEXT_PAGE                  (0x8000)        /* Ability to send multiple pages        */
#define DRV_CB12_ADV_REMOTE_FAULT               (0x2000)        /* Remote fault                          */
#define DRV_CB12_ADV_PAUSE_1                    (0x0800)        /* <11> = Symmetric and Asymmetric pause */
#define DRV_CB12_ADV_PAUSE_0                    (0x0400)        /* <10> = Symmetric pause                */
                                                                /* <01> = Asymmetric PAUSE               */                                                               /* <00> = NO PAUSE                       */
#define DRV_CB12_ADV_100_BASE_T4                (0x0200)        /* 100BASE-T4 capability (not supp.)     */
#define DRV_CB12_ADV_100_BASE_TX_FDX            (0x0100)        /* 100BASE-TX full-duplex capability     */
#define DRV_CB12_ADV_100_BASE_TX                (0x0080)        /* 100BASE-TX capability                 */
#define DRV_CB12_ADV_10_BASE_T_FDX              (0x0040)        /* 10BASE-T full-duplex capability       */
#define DRV_CB12_ADV_10_BASE_T                  (0x0020)        /* 10BASE-T capability                   */
#define DRV_CB12_ADV_SELECT_4                   (0x0010)        /* Selector Field:                       */
#define DRV_CB12_ADV_SELECT_3                   (0x0008)        /*  <00001> = IEEE 802.3                 */
#define DRV_CB12_ADV_SELECT_2                   (0x0004)        /*  <00010> = IEEE 802.9 ISLAN-16T       */
#define DRV_CB12_ADV_SELECT_1                   (0x0002)        /*  <00000> = Reserved                   */
#define DRV_CB12_ADV_SELECT_0                   (0x0001)        /*  <11111> = Reserved                   */

/* Register 17 - Mode Control/Status */
#define DRV_CB12_MODE_CTRL_STAT_FASTRIP                    (0x4000)        /* 10BASE-T fast mode                       */
#define DRV_CB12_MODE_CTRL_STAT_EDPWRDOWN                  (0x2000)        /* Enable the Energy Detect Power-Down mode */
#define DRV_CB12_MODE_CTRL_STAT_LOWSQEN                    (0x0800)        /* Low squelch signal                       */
#define DRV_CB12_MODE_CTRL_STAT_MDPREBP                    (0x0400)        /* Management Data Preamble bypass          */
#define DRV_CB12_MODE_CTRL_STAT_FARLOOPBACK                (0x0200)        /* Remote Loopback enable                   */
#define DRV_CB12_MODE_CTRL_STAT_FASTEST                    (0x0100)        /* Auto-Negotiation Test mode               */
#define DRV_CB12_MODE_CTRL_STAT_AUTO_MDIX_EN               (0x0080)        /* AutoMDIX enable bit                      */
#define DRV_CB12_MODE_CTRL_STAT_MDI_MODE                   (0x0040)        /* Manual set of MDIX/MDI mode <1> = MDIX   */
#define DRV_CB12_MODE_CTRL_STAT_DCD_PAT_GEN                (0x0010)        /* Enables DCD measuring pattern generation */
#define DRV_CB12_MODE_CTRL_STAT_PHYADBP                    (0x0008)        /* PYH disregards PHY address in SMI access */
#define DRV_CB12_MODE_CTRL_STAT_FORCE_GOOD_LINK_STATUS     (0x0004)        /* Force 100BASE-X link active              */
#define DRV_CB12_MODE_CTRL_STAT_ENERGYON                   (0x0002)        /* Energy detected on line                  */

/* Register 18 - Special Modes */
#define DRV_CB12_SPECIAL_MODE_MIIMODE_1                    (0x8000)      /* <11> = res, <10> = SMII                              */
#define DRV_CB12_SPECIAL_MODE_MIIMODE_0                    (0x4000)      /* <01> = RMII, <00> = MII                              */                       */
#define DRV_CB12_SPECIAL_MODE_CLKSELFREQ                   (0x2000)      /* Clock frequency selection <0> = 25 MHz, <1> = 50 MHz */
#define DRV_CB12_SPECIAL_MODE_FX_MODE                      (0x0400)      /* Enable 100BASE-FX mode                      */
#define DRV_CB12_SPECIAL_MODE_FARLOOPBACK                  (0x0200)      /* Remote Loopback enable                      */
#define DRV_CB12_SPECIAL_MODE_PYH_MODE_2                   (0x0080)      /* PHY mode of operation Refer to 7.2 in spec  */
#define DRV_CB12_SPECIAL_MODE_PYH_MODE_1                   (0x0040)      /* PHY mode of operation Refer to 7.2 in spec  */
#define DRV_CB12_SPECIAL_MODE_PYH_MODE_0                   (0x0020)      /* PHY mode of operation Refer to 7.2 in spec  */
#define DRV_CB12_SPECIAL_MODE_PHY_ADD_4                    (0x0010)      /* PHY address Refer to 7.1 in spec            */
#define DRV_CB12_SPECIAL_MODE_PHY_ADD_3                    (0x0008)      /* PHY address Refer to 7.1 in spec            */
#define DRV_CB12_SPECIAL_MODE_PHY_ADD_2                    (0x0004)      /* PHY address Refer to 7.1 in spec            */
#define DRV_CB12_SPECIAL_MODE_PHY_ADD_1                    (0x0002)      /* PHY address Refer to 7.1 in spec            */
#define DRV_CB12_SPECIAL_MODE_PHY_ADD_0                    (0x0001)      /* PHY address Refer to 7.1 in spec            */

/* Register 27 - Special Control/Status indications */
#define DRV_CB12_SPECIAL_CTRL_STAT_SWRST_FAST              (0x1000)      /* SW reset counter testing                           */                          
#define DRV_CB12_SPECIAL_CTRL_STAT_SQEOFF                  (0x0800)      /* Disable SQE (Heartbeat) test                       */                      
#define DRV_CB12_SPECIAL_CTRL_STAT_FEFIEN                  (0x0020)      /* Far end fault indication enable                    */                   
#define DRV_CB12_SPECIAL_CTRL_STAT_XPOL                    (0x0010)      /* Polarity state of 10BASE-T <1> = reversed polarity */

/* Register 31 - PHY Special Control/Status indications */
#define DRV_CB12_PYH_SPECIAL_CTRL_STAT_AUTODONE            (0x1000)      /* Auto-Negotiation done         */
#define DRV_CB12_PYH_SPECIAL_CTRL_STAT_4B5BEN              (0x0040)      /* Enable 4B5B Encoding/Decoding */
#define DRV_CB12_PYH_SPECIAL_CTRL_STAT_DATA_SCRAMBLING_DIS (0x0001)      /* Scramble disabling            */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX50_MIIMU_ReadPhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser);
int NX50_MIIMU_WritePhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser);


/*
  ____                        _              ____   _            _ 
 |  _ \    ___   ___    ___  | |_           / ___| | |_   _ __  | |
 | |_) |  / _ \ / __|  / _ \ | __|         | |     | __| | '__| | |
 |  _ <  |  __/ \__ \ |  __/ | |_          | |___  | |_  | |    | |
 |_| \_\  \___| |___/  \___|  \__|  _____   \____|  \__| |_|    |_|
                                   |_____|                         

*/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_RSTCTRL_SetCfg( unsigned long ulRstCtrlVal );
unsigned long NX50_RSTCTRL_GetCfg( void );

/*
  ____                  _     _                    
 / ___|   _   _   ___  | |_  (_)  _ __ ___     ___ 
 \___ \  | | | | / __| | __| | | | '_ ` _ \   / _ \
  ___) | | |_| | \__ \ | |_  | | | | | | | | |  __/
 |____/   \__, | |___/  \__| |_| |_| |_| |_|  \___|
          |___/                                    

*/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_SYSTIME_SetBorder( unsigned long ulBorder, void* pvUser );
void NX50_SYSTIME_SetSpeed( unsigned long ulSpeed, void* pvUser );
void NX50_SYSTIME_SetSystime( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser );
void NX50_SYSTIME_GetSystime( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser );

#endif // #define __COMMON_FUNC_NETX50_H
                                  
