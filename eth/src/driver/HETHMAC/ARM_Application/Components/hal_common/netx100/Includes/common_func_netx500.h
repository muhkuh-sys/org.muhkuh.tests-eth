#ifndef __COMMON_FUNC_NETX500_H
#define __COMMON_FUNC_NETX500_H

#include "defines.h" // for PEEK, POKE, INTERRUPT
#include "regdef_netx500.h"

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX500_DEV_FREQUENCY 100000000L // 100 MHz

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
#define NX500_GPIO_CFG_IN            0
#define NX500_GPIO_CFG_EXT_0         2
#define SerialPutChar(c) NX500_UART_PutCharacter(0,c)

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
} NX500_UART_BAUDRATE;

#define NX500_DEV_BAUDRATE UART_BAUDRATE_115200

// DEV_BAUDRATE is 100 times to small -> multiply with 100 (or divide by DEV_FREQUENCY/100)
#define NX500_DEV_BAUDRATE_DIV_LO (((NX500_DEV_BAUDRATE*16*65536)/(NX500_DEV_FREQUENCY/100))&0xff)
#define NX500_DEV_BAUDRATE_DIV_HI (((NX500_DEV_BAUDRATE*16*65536)/(NX500_DEV_FREQUENCY/100))>>8) 

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX500_UART_InitSerial(unsigned int ulUartNr);
void NX500_UART_CloseSerial(unsigned int ulUartNr);
void NX500_UART_PutCharacter(unsigned int ulUartNr, const unsigned char bChar);
int  NX500_UART_GetCharacter(unsigned int ulUartNr);
int  NX500_UART_Peek(unsigned int ulUartNr);
void NX500_UART_UPrintf(const char* pchFmt, ...);
void NX500_UART_UPrintfDec(const char* pchFmt, ...);

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

#define MAX_IRQ_VECTORS                  16
#define ARM926EJS_Vect_Undefined 0x00000004    
#define ARM926EJS_Vect_SWI       0x00000008    
#define ARM926EJS_Vect_Prefetch  0x0000000c    
#define ARM926EJS_Vect_Abort     0x00000010    
#define ARM926EJS_Vect_IRQ       0x00000018
#define ARM926EJS_Vect_FIQ       0x0000001c    

#define NX500_IRQFIQ_LOCK     asm ( "msr cpsr_c, #0xdf" );
#define NX500_IRQFIQ_UNLOCK   asm ( "msr cpsr_c, #0x1f" );

#define NX500_lock_irqfiq_save(x)     (x = NX500_LockIrqSave())
#define NX500_lock_irqfiq_restore(x)  (NX500_LockIrqRestore(x))

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX500_VIC_InstallIsrVector( pp_func ulVector);
void NX500_VIC_ClearVic(void);
void INTERRUPT NX500_VIC_IRQVectorHandler_C(void);
void NX500_VIC_ISR_Default(void);
void NX500_VIC_SetDefaultVector(pp_func default_handler);
void NX500_VIC_SetIrqVector(unsigned int vec, unsigned long src, pp_func irq_handler);
void NX500_VIC_EnableIrqSources(unsigned long src);

unsigned int   NX500_LockIrqSave(void);
void           NX500_LockIrqRestore(unsigned int uiState);

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
#define NX500_GPIO_MODE_NOINVERT                   0x0
#define NX500_GPIO_MODE_INVERT                     0x4
#define NX500_GPIO_MODE_INPUT_READ                 0x0
#define NX500_GPIO_MODE_OUTPUT_SET_TO_0            0x1
#define NX500_GPIO_MODE_OUTPUT_SET_TO_1            0x9
#define NX500_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE   0x11

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int   NX500_GPIO_GetIrq (unsigned long ulGpioNum);
int   NX500_GPIO_GetInput (unsigned long ulGpioNum);
unsigned long NX500_GPIO_GetIn( void );
void  NX500_GPIO_SetupMode (unsigned long ulGpioNum, unsigned long ulMode, unsigned long ulInvert);
void  NX500_GPIO_SetOutput (unsigned long ulGpioNum, int fEnable);
unsigned long NX500_GPIO_GetLine( void );
void  NX500_GPIO_SetLine( unsigned long ulVal );
void  NX500_GPIO_IrqEnable (unsigned long ulGPIOnum);
void  NX500_GPIO_IrqReset (unsigned long ulGpioNum);
void  NX500_GPIO_Sleep (unsigned long ulCounter, unsigned int uiTimeout);
void NX500_GPIO_SetupTimer( unsigned long ulCounter, unsigned int uiTimeout );
int NX500_GPIO_IsTimerRunning( unsigned long ulCounter );
void NX500_GPIO_PrepareTimer( unsigned int uCounter, unsigned int uiTimeout, unsigned int uiOnce );
void NX500_GPIO_StartTimer( unsigned int uCounter );
void NX500_GPIO_ResetTimer( unsigned int uCounter );
unsigned int NX500_GPIO_GetTimerIrq( unsigned int uCounter );
void NX500_GPIO_EnableTimerIrq( unsigned int uCounter );
void NX500_GPIO_DisableTimerIrq( unsigned int uCounter );
void NX500_GPIO_ConfirmTimerIrq( unsigned long ulCounter );

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
#define NX500_PIO_MODE_IN  0
#define NX500_PIO_MODE_OUT 1

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX500_PIO_SetMode (unsigned long ulPioNum, unsigned long ulMode);
int  NX500_PIO_GetInput (unsigned long ulPioNum);
unsigned long NX500_PIO_GetIn( void );
int  NX500_PIO_GetOutput (unsigned long ulPioNum);
void NX500_PIO_SetOutput (unsigned long ulPioNum, int fEnable);
unsigned long NX500_PIO_GetLine ( void );
void NX500_PIO_SetLine( unsigned long ulVal );

void NX500_HIF_SetPioLine( unsigned long ulPio32to63, unsigned long ulPio64to84 );
void NX500_HIF_SetIoMode( unsigned long ulHifModeEnable32to63, unsigned long ulHifModeEnable64to84 );
void NX500_HIF_GetPioIn( unsigned long* pulPio32to63, unsigned long* pulPio64to84 );

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX500_MMIO_SetCfg( unsigned int uMmioNum, unsigned char bFunction, int fInvertOut, int fInvertIn );

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
  NX500_RDYRUN_LED_OFF = 0,
  NX500_RDYRUN_LED_GREEN,
  NX500_RDYRUN_LED_RED,
  NX500_RDYRUN_LED_INV
} NX500_RDYRUN_LED_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_E tMode);

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
#define NX500_INT_PHY1_ADDR                    0x00
#define NX500_INT_PHY2_ADDR                    0x01
#define PHY_CTRL_PHY_ADDR                       0x0 // bits 4..1: PHY address, bit 0 indicate internal PHY 0 or 1
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
void NX500_INTPHY_Init( NX500_PHY_CONTROL_T *ptPhyCtrlInit );
void NX500_INTPHY_CorrectAutoMdix100MBit( unsigned int uMiimuPhyAddr );

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
void NX500_PFIFO_Reset( void );
unsigned long NX500_PFIFO_GetFillLevel( unsigned int uFifoNum );
int NX500_PFIFO_SetBorders(unsigned int *auPFifoDepth);
unsigned long NX500_PFIFO_GetFifoResetVector( void );
unsigned long NX500_PFIFO_GetFifoFullVector( void );
unsigned long NX500_PFIFO_GetFifoEmptyVector( void );
unsigned long NX500_PFIFO_GetFifoOverflowVector( void );
unsigned long NX500_PFIFO_GetFifoUnderrunVector( void );


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
#define NX500_XMAC_RPU_DWORD_RAMSIZE 0x0100
#define NX500_XMAC_TPU_DWORD_RAMSIZE 0x0100
#define NX500_XPEC_DWORD_RAMSIZE     0x0800

typedef enum {
  NX500_XC_TYPE_RPU  = 0,
  NX500_XC_TYPE_TPU  = 1,
  NX500_XC_TYPE_XPEC = 2,
} NX500_XC_TYPE_E;

#define MSK_NX500_XC_UNIT_RPU  (1U << NX500_XC_TYPE_RPU)
#define MSK_NX500_XC_UNIT_TPU  (1U << NX500_XC_TYPE_TPU)
#define MSK_NX500_XC_UNIT_XPEC (1U << NX500_XC_TYPE_XPEC)
#define MSK_NX500_XC_UNIT_XMAC (MSK_NX500_XC_UNIT_RPU | MSK_NX500_XC_UNIT_TPU)
#define MSK_NX500_XC_UNIT_ALL  (MSK_NX500_XC_UNIT_XMAC | MSK_NX500_XC_UNIT_XPEC)

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX500_XC_Reset(unsigned int uPortNo, void* pvUser);
int NX500_XC_ResetUnits(unsigned int uPortNo, unsigned int uUnitVec, void* pvUser);
int NX500_XC_Load(unsigned int uPortNo, NX500_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser);
int NX500_XC_Start(unsigned int uPortNo, void* pvUser);
int NX500_XC_StartUnits(unsigned int uPortNo, unsigned int uUnitVec, void* pvUser);

//  __  __   ___   ___   __  __   _   _ 
// |  \/  | |_ _| |_ _| |  \/  | | | | |
// | |\/| |  | |   | |  | |\/| | | | | |
// | |  | |  | |   | |  | |  | | | |_| |
// |_|  |_| |___| |___| |_|  |_|  \___/ 
//                                      

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX500_MIIMU_SUPPRESS_PREAMBLE              (0)             
#define NX500_MIIMU_SEND_PREAMBLE                  (1)             
#define NX500_MIIMU_MDCFREQ_HIGH                   (0)             
#define NX500_MIIMU_MDCFREQ_LOW                    (1)             
#define NX500_MIIMU_RTA_0_BITS                     (0)             
#define NX500_MIIMU_RTA_1_BITS                     (1)             

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
int NX500_MIIMU_ReadPhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser);
int NX500_MIIMU_WritePhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser);


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
void NX500_RSTCTRL_SetCfg( unsigned long ulRstCtrlVal );
unsigned long NX500_RSTCTRL_GetCfg( void );

/*
   __  __ __  __ _   _
  |  \/  |  \/  | | | |
  | |\/| | |\/| | | | |
  | |  | | |  | | |_| |
  |_|  |_|_|  |_|\___/

*/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX500_MMU_Init( unsigned long* pulTTBBase );


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
void NX500_SYSTIME_SetBorder( unsigned long ulBorder, void* pvUser );
void NX500_SYSTIME_SetSpeed( unsigned long ulSpeed, void* pvUser );
void NX500_SYSTIME_SetSystime( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser );
void NX500_SYSTIME_GetSystime( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser );

//....###.....######..####..######...........######..########.########..##......
//...##.##...##....##..##..##....##.........##....##....##....##.....##.##......
//..##...##..##........##..##...............##..........##....##.....##.##......
//.##.....##..######...##..##...............##..........##....########..##......
//.#########.......##..##..##...............##..........##....##...##...##......
//.##.....##.##....##..##..##....##.........##....##....##....##....##..##......
//.##.....##..######..####..######..#######..######.....##....##.....##.########

/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/
void NX500_AsicCtrl_SetIoConfig( unsigned int uiIdx, unsigned long ulVal );
unsigned long NX500_AsicCtrl_GetIoConfig( unsigned int uiIdx );

#endif // #define __COMMON_FUNC_NETX500_H
                                  










