#ifndef __COMMON_FUNC_NETX5_H
#define __COMMON_FUNC_NETX5_H

/*
  ____    ____    __  __
 |  _ \  |  _ \  |  \/  |
 | | | | | |_) | | |\/| |
 | |_| | |  __/  | |  | |
 |____/  |_|     |_|  |_|

*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
typedef enum {
  NX5_DPM_SIZE_08,
  NX5_DPM_SIZE_16,
  NX5_DPM_SIZE_32
} NX5_DPM_SIZE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
#ifdef NX5_PARALLEL_DPM
int NX5_DPM_InitDpm( int iCnt, NX5_DPM_SIZE_E tSize, unsigned int uRdAddrSetupTime, int fReadyEn );
#endif
#ifdef NX5_SERIAL_DPM
int NX5_DPM_InitSerDpm( );
#endif
int NX5_DPM_ChkDpm( void );
void NX5_DPM_SetupByteAccess( int iWindow, int fByteAccess );
void NX5_DPM_SetupIrq( unsigned long ulIrqMsk, unsigned long ulFiqMsk, unsigned long ulPadCtrl );
void NX5_DPM_GetFiq( unsigned long *pulFiq );
void NX5_DPM_GetIrq( unsigned long *pulIrq );
void NX5_DPM_GetErr( unsigned long *pulErr, unsigned long *pulErrAddr );
void NX5_DPM_ClrErr( void );
void NX5_DPM_GetAndClrErr( unsigned long *pulErr, unsigned long *pulErrAddr );
void NX5_DPM_SetPioOe31_28( unsigned long ulPioOe31_28 );
void NX5_DPM_SetPioOut31_28( unsigned long ulPioOut31_28 );
unsigned int NX5_DPM_WaitUnlocked( void );
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
#define NX5_PIO_MODE_IN  0
#define NX5_PIO_MODE_OUT 1

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX5_PIO_SetMode (unsigned long ulPioNum, unsigned long ulMode);
int  NX5_PIO_GetOutput( unsigned long ulPioNum );
void NX5_PIO_SetOutput (unsigned long ulPioNum, int fEnable);
void NX5_PIO_ToggleOutput( unsigned long ulPioNum );
int  NX5_PIO_GetInput (unsigned long ulPioNum);
int  NX5_PIO_GetIn( void );

/*
  _____          _                                   _           ____    _   _  __   __
 | ____| __  __ | |_    ___   _ __   _ __     __ _  | |         |  _ \  | | | | \ \ / /
 |  _|   \ \/ / | __|  / _ \ | '__| | '_ \   / _` | | |         | |_) | | |_| |  \ V /
 | |___   >  <  | |_  |  __/ | |    | | | | | (_| | | |         |  __/  |  _  |   | |
 |_____| /_/\_\  \__|  \___| |_|    |_| |_|  \__,_| |_|  _____  |_|     |_| |_|   |_|
                                                        |_____|
*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
typedef enum {
  PHY_MODE_SUPER_ISOLATE                         = 0,  /**< Super Isolate PHY from interface                            */
  PHY_MODE_10BASE_T_HD_NOAUTONEG_AUTOMDIXDIS     = 1,  /**< 10BASE-T Half-duplex, AutoNeg disabled, AutoMDIX disabled   */
  PHY_MODE_10BASE_T_FD_NOAUTONEG_AUTOMDIXDIS     = 2,  /**< 10BASE-T Full-duplex, AutoNeg disabled, AutoMDIX disabled   */
  PHY_MODE_100BASE_TX_HD_NOAUTONEG_AUTOMDIXDIS   = 3,  /**< 100BASE-TX Half-duplex, AutoNeg disabled, AutoMDIX disabled */
  PHY_MODE_100BASE_TX_FD_NOAUTONEG_AUTOMDIXDIS   = 4,  /**< 100BASE-TX Full-duplex, AutoNeg disabled, AutoMDIX disabled */
  PHY_MODE_POWER_DOWN_MODE                       = 5,  /**< PowerDown Mode enabled                                      */
  PHY_MODE_AUTONEG_AUTOMDIXDIS                   = 6,  /**< AutoNeg enabled, AutoMDIX disabled                          */
  PHY_MODE_AUTONEG_AUTOMDIXEN                    = 7,  /**< AutoNeg enabled, AutoMDIX enabled                           */
  PHY_MODE_AUTONEG_AUTOMDIXEN_100BASE_TX_FD_ONLY = 8,  /**< AutoNeg enabled, AutoMDIX enabled, only 100BASE-TX FD advertised */
  PHY_MODE_AUTONEG_AUTOMDIXEN_100BASE_TX_HD_ONLY = 9,  /**< AutoNeg enabled, AutoMDIX enabled, only 100BASE-TX HD advertised */
} NX5_PHY_MODE_E;

/*****************************************************************************/
/*! Link status structure                                                    */
/*****************************************************************************/
typedef struct NX5_PHY_STATUS_Ttag
{
  unsigned int uSpeed;             /**< Speed (100/10)                                            */
  unsigned int fIsLinkUp;          /**< Link state   (!=0 -> Link UP)                             */
  unsigned int fIsFullDuplex;      /**< Duplex state (!=0 -> FDX)                                 */
  unsigned int fMdixInUse;         /**< 1/0: MDIX/MDI in use                                      */
  unsigned int fIsAutoNegEn;       /**< 1/0: Auto-Negotiation enabled/disabled                    */
  unsigned int fIsAutoMdixEn;      /**< 1/0: HP-Auto-MDIX enabled/disabled                        */
  unsigned int fIsSuperIsolate;    /**< 1/0: SuperIsolate enabled/disabled                        */
  unsigned int fIsPowerDown;       /**< 1/0: Power down enabled/disabled                          */
  unsigned int fAdv100BaseX_Fdx;   /**< 1/0: Advertise 100BaseX FD enabled/disabled               */
  unsigned int fAdv100BaseX_Hdx;   /**< 1/0: Advertise 100BaseX FD enabled/disabled               */
  unsigned int fAdv10BaseT_Fdx;    /**< 1/0: Advertise 10BaseT FD enabled/disabled                */
  unsigned int fAdv10BaseT_Hdx;    /**< 1/0: Advertise 10BaseT HD enabled/disabled                */
  unsigned int fLpAdv100BaseX_Fdx; /**< 1/0: Link Partner Advertise 100BaseX FD enabled/disabled  */
  unsigned int fLpAdv100BaseX_Hdx; /**< 1/0: Link Partner Advertise 100BaseX FD enabled/disabled  */
  unsigned int fLpAdv10BaseT_Fdx;  /**< 1/0: Link Partner Advertise 10BaseT FD enabled/disabled   */
  unsigned int fLpAdv10BaseT_Hdx;  /**< 1/0: Link Partner Advertise 10BaseT HD enabled/disabled   */
} NX5_PHY_STATUS_T;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX5_EXTPHY_EnablePhyClk( int fPhyClkOutOe, int fDerivePhyClkOutFromClk );
void NX5_EXTPHY_SetPhyReset( int fEnable );
int  NX5_EXTPHY_Init( unsigned int uPhyAddr, NX5_PHY_MODE_E ePhyMode );
int  NX5_EXTPHY_GetStatus( unsigned int uPhyAddr, NX5_PHY_STATUS_T* ptPhyState );
int  NX5_EXTPHY_GetLinkState( unsigned int uPortNo, unsigned int* puLink, unsigned int* puMbps, unsigned int* puFullDuplex, void* pvUser );

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
void NX5_PFIFO_Reset( void );
int NX5_PFIFO_SetBorders(const unsigned int *auiPFifoDepth);
int NX5_PFIFO_GetBorders(unsigned int *auiPFifoDepth);
int NX5_PFIFO_GetFillLevel( unsigned int uFifoNum );
unsigned long NX5_PFIFO_GetFifoResetVector( void );
unsigned long NX5_PFIFO_GetFifoFullVector( void );
unsigned long NX5_PFIFO_GetFifoEmptyVector( void );
unsigned long NX5_PFIFO_GetFifoOverflowVector( void );
unsigned long NX5_PFIFO_GetFifoUnderrunVector( void );

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
#define NX5_XMAC_RPU_DWORD_RAMSIZE 0x0100
#define NX5_XMAC_TPU_DWORD_RAMSIZE 0x0100
#define NX5_XPEC_DWORD_RAMSIZE     0x0800

typedef enum {
  NX5_XC_TYPE_RPU  = 0,
  NX5_XC_TYPE_TPU  = 1,
  NX5_XC_TYPE_XPEC = 2
} NX5_XC_TYPE_E;

#define MSK_NX5_XC_TYPE_RPU  (1U << NX5_XC_TYPE_RPU)
#define MSK_NX5_XC_TYPE_TPU  (1U << NX5_XC_TYPE_TPU)
#define MSK_NX5_XC_TYPE_XPEC (1U << NX5_XC_TYPE_XPEC)
#define MSK_NX5_XC_TYPE_XMAC (MSK_NX5_XC_TYPE_RPU  | MSK_NX5_XC_TYPE_TPU)
#define MSK_NX5_XC_TYPE_ALL  (MSK_NX5_XC_TYPE_XMAC | MSK_NX5_XC_TYPE_XPEC)

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX5_XC_Reset(unsigned int uiPort, void* pvUser);
int NX5_XC_Load(unsigned int uiPort, NX5_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser);
int NX5_XC_Start(unsigned int uiPort, void* pvUser);
int NX5_XC_StartUnits(unsigned int uiPort, unsigned int uUnitVec, void* pvUser);

//  __  __   ___   ___   __  __   _   _
// |  \/  | |_ _| |_ _| |  \/  | | | | |
// | |\/| |  | |   | |  | |\/| | | | | |
// | |  | |  | |   | |  | |  | | | |_| |
// |_|  |_| |___| |___| |_|  |_|  \___/
//

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX5_MIIMU_SUPPRESS_PREAMBLE              (0)
#define NX5_MIIMU_SEND_PREAMBLE                  (1)
#define NX5_MIIMU_MDCFREQ_HIGH                   (0)
#define NX5_MIIMU_MDCFREQ_LOW                    (1)
#define NX5_MIIMU_RTA_0_BITS                     (0)
#define NX5_MIIMU_RTA_1_BITS                     (1)

#define NX5_EXT_PHY1_ADDR                       0x02
#define NX5_EXT_PHY2_ADDR                       0x03

/* BCM5241 registers */
#define BCM5241_CONTROL                             (0)        /* Control                               */
#define BCM5241_STATUS                              (1)        /* Status #1                             */
#define BCM5241_PHY_ID_1                            (2)        /* PHY Identification 1                  */
#define BCM5241_PHY_ID_2                            (3)        /* PHY Identification 2                  */
#define BCM5241_AUTO_NEG_ADVERTISEMENT              (4)        /* Auto-Negotiation Advertisement        */
#define BCM5241_AUTO_NEG_LINK_PARTNER_ABIL          (5)        /* Auto-Negotiation Link Partner Ability */
#define BCM5241_AUXILIARY_STATUS_SUM               (25)        /* Auxiliary status summary register     */
#define BCM5241_AUXILIARY_ERR_GEN_STATUS           (28)        /* Auxiliary error and general status    */
#define BCM5241_AUXILIARY_MODE                     (29)        /* Auxiliary mode                        */
#define BCM5241_AUXILIARY_MULTIPLE                 (30)        /* Auxiliary multiple register           */
#define BCM5241_BCM_TEST                           (31)        /* Broadcom test register                */

/* BCM5241 shadow registers */
#define BCM5241_SHADOW_MISC_CTRL                   (16)        /* Misc Control  register                */
#define BCM5241_SHADOW_MODE_4                      (26)        /* Mode 4 register                       */

/* Register 0 - Basic Control Register Bit Definition */
#define BCM5241_CONTROL_RESET                  (0x8000)        /* PHY reset                          */
#define BCM5241_CONTROL_LOOPBACK               (0x4000)        /* Enable loopback                    */
#define BCM5241_CONTROL_SPEED_SELECT_100       (0x2000)        /* Select Speed 100MBit               */
#define BCM5241_CONTROL_AUTO_NEG_ENABLE        (0x1000)        /* Auto-Negotiation Enable            */
#define BCM5241_CONTROL_POWER_DOWN             (0x0800)        /* Power-down                         */
#define BCM5241_CONTROL_ISOLATE                (0x0400)        /* Electrically Isolate PHY from MII  */
#define BCM5241_CONTROL_AUTO_NEG_RESTART       (0x0200)        /* Restart Auto-Negotiation           */
#define BCM5241_CONTROL_FULL_DUPLEX            (0x0100)        /* Full Duplex Mode                   */

/* Register 1 - Basic Status Register Bit Definition */
#define BCM5241_STATUS_100_BASE_T4             (0x8000)        /* 100BASE-T4 support                 */
#define BCM5241_STATUS_100_BASE_X_FDX          (0x4000)        /* 100BASE-X full duplex support      */
#define BCM5241_STATUS_100_BASE_X_HDX          (0x2000)        /* 100BASE-X half duplex support      */
#define BCM5241_STATUS_10_MBPS_FDX             (0x1000)        /* 10 Mbps full duplex support        */
#define BCM5241_STATUS_10_MBPS_HDX             (0x0800)        /* 10 Mbps half duplex support        */
#define BCM5241_STATUS_MF_PREAMBLE_SUPPRESS    (0x0040)        /* 1: Enable Preamble suppression     */
#define BCM5241_STATUS_AUTO_NEG_COMPLETE       (0x0020)        /* Auto-Negotiation complete          */
#define BCM5241_STATUS_REMOTE_FAULT            (0x0010)        /* Remote fault detected              */
#define BCM5241_STATUS_LINK_UP                 (0x0004)        /* Link status                        */
#define BCM5241_STATUS_JABBER_DETECT           (0x0002)        /* Jabber detected                    */
#define BCM5241_STATUS_EXTENDED_CAPABILITY     (0x0001)        /* Basic/extended register capability */

/* Register 4/5 - Auto Negotiation Advertisement Register Bit Definition, Reg4: own capabilites, Reg5: Link partner capabilities */
#define BCM5241_ADV_NEXT_PAGE                  (0x8000)        /* Ability to send multiple pages     */
#define BCM5241_ADV_REMOTE_FAULT               (0x2000)        /* Remote fault                       */
#define BCM5241_ADV_PAUSE                      (0x0400)        /* PAUSE capability                   */
#define BCM5241_ADV_100_BASE_T4                (0x0200)        /* 100BASE-T4 capability              */
#define BCM5241_ADV_100_BASE_TX_FDX            (0x0100)        /* 100BASE-TX full-duplex capability  */
#define BCM5241_ADV_100_BASE_TX                (0x0080)        /* 100BASE-TX capability              */
#define BCM5241_ADV_10_BASE_T_FDX              (0x0040)        /* 10BASE-T full-duplex capability    */
#define BCM5241_ADV_10_BASE_T                  (0x0020)        /* 10BASE-T capability                */
#define BCM5241_ADV_SELECTOR_FIELD             (0x001f)        /* <00001> = IEEE 802.3, read-only    */

/* Register 25 - Auxiliary Status Summary */
#define BCM5241_AUX_STAT_SUM_SPEED_IND         (0x0008)        /* 1/0: 100Mps/10Mps                  */
#define BCM5241_AUX_STAT_SUM_LINK_STATUS       (0x0004)        /* 1/0: Link up/down                  */
#define BCM5241_AUX_STAT_SUM_AUTONEG_IND       (0x0002)        /* 1: Auto-negotiation enabled        */
#define BCM5241_AUX_STAT_SUM_FDX_IND           (0x0001)        /* 1/0: Full-duplex active/inactive   */

/* Register 28 - Auxiliary Error and General Status */
#define BCM5241_AUX_ERR_MDIX_STATUS            (0x2000)        /* 1/0: MDIX/MDI in use               */
#define BCM5241_AUX_ERR_HP_AUTOMDIXDIS         (0x0800)        /* 1/0: Disable/Enable HP Auto-MDIX   */

/* Register 29 - Auxiliary Mode */
#define BCM5241_AUX_MODE_FORCE_ACT_LED_OFF     (0x0010)        /* 1/0: Disable/Enable XMT/RCV Activity LED outputs */
#define BCM5241_AUX_MODE_FORCE_LNK_LED_OFF     (0x0008)        /* 1/0: Disable/Enable Link LED output              */

/* Register 30 - Auxiliary Multiple */
#define BCM5241_AUX_MUL_RESTART_AUTONEG        (0x0100)        /* 1: Restart autonegotiation process */
#define BCM5241_AUX_MUL_SUPER_ISOLATE          (0x0008)        /* 1/0: Super isolate/Normal mode     */

/* Register 31 - Broadcom Test register */
#define BCM5241_BCM_TEST_SHADOW_REG_EN         (0x0080)        /* 1/0: Enable/Disable shadow registers  */

/* Shadow Register 26 - Mode 4 */
#define BCM5241_SHADOW_MODE_4_STANDBY_PWR      (0x0008)        /* 1/0: Standby power mode/ Normal mode */

/* Shadow Register 10 - Misc Ctrl */
#define BCM5241_SHADOW_MISC_CTRL_FORCED_AUTO_MDIX_EN (0x4000)  /* 1: Enable Auto-MDIX in forced modes */


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX5_MIIMU_ReadPhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser);
int NX5_MIIMU_WritePhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser);

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
void NX5_SYSTIME_SetBorder( unsigned long ulBorder, void* pvUser );
void NX5_SYSTIME_SetSpeed( unsigned long ulSpeed, void* pvUser );
void NX5_SYSTIME_SetSystime( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser );
void NX5_SYSTIME_GetSystime( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser );

#endif // #define __COMMON_FUNC_NETX5_H
