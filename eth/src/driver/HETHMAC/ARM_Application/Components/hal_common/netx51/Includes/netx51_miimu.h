#ifndef __NETX51_MIIMU_H
#define __NETX51_MIIMU_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define NX51_MIIMU_SUPPRESS_PREAMBLE              (0)
#define NX51_MIIMU_SEND_PREAMBLE                  (1)
#define NX51_MIIMU_MDCFREQ_HIGH                   (0)
#define NX51_MIIMU_MDCFREQ_LOW                    (1)
#define NX51_MIIMU_RTA_0_BITS                     (0)
#define NX51_MIIMU_RTA_1_BITS                     (1)

/* NEC CB12 registers */
#define NX51_DRV_CB12_CONTROL                     (0)             /* Control                                         */
#define NX51_DRV_CB12_STATUS                      (1)             /* Status #1                                       */
#define NX51_DRV_CB12_PHY_ID_1                    (2)             /* PHY Identification 1                            */
#define NX51_DRV_CB12_PHY_ID_2                    (3)             /* PHY Identification 2                            */
#define NX51_DRV_CB12_AUTO_NEG_ADVERTISEMENT      (4)             /* Auto-Negotiation Advertisement                  */
#define NX51_DRV_CB12_AUTO_NEG_LINK_PARTNER_ABIL  (5)             /* Auto-Negotiation Link Partner Ability           */
#define NX51_DRV_CB12_AUTO_NEG_EXPANSION          (6)             /* Auto-Negotiation Expansion                      */
#define NX51_DRV_CB12_AUTO_NEG_NEXT_PAGE_TX       (7)             /* Auto-Negotiation Next Page Transmit             */
#define NX51_DRV_CB12_SILICON_REVISION            (16)            /* Silicon Revision                                */
#define NX51_DRV_CB12_MODE_CONTROL_STATUS         (17)            /* Mode Control/Status                             */
#define NX51_DRV_CB12_SPECIAL_MODES               (18)            /* Special Modes                                   */
#define NX51_DRV_CB12_SMII_CONFIGURATION_STATUS   (19)            /* SMII configuration status                       */
#define NX51_DRV_CB12_CONTROL_STATUS_INDICATION   (27)            /* Control/Status Indication                       */
#define NX51_DRV_CB12_SPECIAL_INT_TEST_CONTROLS   (28)            /* Special internal testability controls           */
#define NX51_DRV_CB12_INTERRUPT_SOURCE            (29)            /* Interrupt source                                */
#define NX51_DRV_CB12_INTERRUPT_MASK              (30)            /* Interrupt mask                                  */
#define NX51_DRV_CB12_PHY_SPECIAL_CONTROL_STATUS  (31)            /* PHY special control/status                      */

/* Register 0 - Basic Control Register Bit Definition */
#define NX51_DRV_CB12_CONTROL_RESET                  (0x8000)        /* PHY reset                          */
#define NX51_DRV_CB12_CONTROL_LOOPBACK               (0x4000)        /* Enable loopback                    */
#define NX51_DRV_CB12_CONTROL_SPEED_SELECT_100       (0x2000)        /* Select Speed 100MBit               */
#define NX51_DRV_CB12_CONTROL_AUTO_NEG_ENABLE        (0x1000)        /* Auto-Negotiation Enable            */
#define NX51_DRV_CB12_CONTROL_POWER_DOWN             (0x0800)        /* Power-down                         */
#define NX51_DRV_CB12_CONTROL_ISOLATE                (0x0400)        /* Electrically Isolate PHY from MII  */
#define NX51_DRV_CB12_CONTROL_AUTO_NEG_RESTART       (0x0200)        /* Restart Auto-Negotiation           */
#define NX51_DRV_CB12_CONTROL_FULL_DUPLEX            (0x0100)        /* Full Duplex Mode                   */
#define NX51_DRV_CB12_CONTROL_COL_TEST               (0x0080)        /* Enable COL signal test             */

/* Register 1 - Basic Status Register Bit Definition */
#define NX51_DRV_CB12_STATUS_100_BASE_T4             (0x8000)        /* 100BASE-T4 support                 */
#define NX51_DRV_CB12_STATUS_100_BASE_X_FDX          (0x4000)        /* 100BASE-X full duplex support      */
#define NX51_DRV_CB12_STATUS_100_BASE_X_HDX          (0x2000)        /* 100BASE-X half duplex support      */
#define NX51_DRV_CB12_STATUS_10_MBPS_FDX             (0x1000)        /* 10 Mbps full duplex support        */
#define NX51_DRV_CB12_STATUS_10_MBPS_HDX             (0x0800)        /* 10 Mbps half duplex support        */
#define NX51_DRV_CB12_STATUS_AUTO_NEG_COMPLETE       (0x0020)        /* Auto-Negotiation complete          */
#define NX51_DRV_CB12_STATUS_REMOTE_FAULT            (0x0010)        /* Remote fault detected              */
#define NX51_DRV_CB12_STATUS_AUTO_NEG_ABILITY        (0x0008)        /* Auto-Negotiation ability           */
#define NX51_DRV_CB12_STATUS_LINK_UP                 (0x0004)        /* Link status                        */
#define NX51_DRV_CB12_STATUS_JABBER_DETECT           (0x0002)        /* Jabber detected                    */
#define NX51_DRV_CB12_STATUS_EXTENDED_CAPABILITY     (0x0001)        /* Basic/extended register capability */

/* Register 4 - Auto Negotiation Advertisement Register Bit Definition */
#define NX51_DRV_CB12_ADV_NEXT_PAGE                  (0x8000)        /* Ability to send multiple pages        */
#define NX51_DRV_CB12_ADV_REMOTE_FAULT               (0x2000)        /* Remote fault                          */
#define NX51_DRV_CB12_ADV_PAUSE_1                    (0x0800)        /* <11> = Symmetric and Asymmetric pause */
#define NX51_DRV_CB12_ADV_PAUSE_0                    (0x0400)        /* <10> = Symmetric pause                */
                                                                /* <01> = Asymmetric PAUSE               */                                                               /* <00> = NO PAUSE                       */
#define NX51_DRV_CB12_ADV_100_BASE_T4                (0x0200)        /* 100BASE-T4 capability (not supp.)     */
#define NX51_DRV_CB12_ADV_100_BASE_TX_FDX            (0x0100)        /* 100BASE-TX full-duplex capability     */
#define NX51_DRV_CB12_ADV_100_BASE_TX                (0x0080)        /* 100BASE-TX capability                 */
#define NX51_DRV_CB12_ADV_10_BASE_T_FDX              (0x0040)        /* 10BASE-T full-duplex capability       */
#define NX51_DRV_CB12_ADV_10_BASE_T                  (0x0020)        /* 10BASE-T capability                   */
#define NX51_DRV_CB12_ADV_SELECT_4                   (0x0010)        /* Selector Field:                       */
#define NX51_DRV_CB12_ADV_SELECT_3                   (0x0008)        /*  <00001> = IEEE 802.3                 */
#define NX51_DRV_CB12_ADV_SELECT_2                   (0x0004)        /*  <00010> = IEEE 802.9 ISLAN-16T       */
#define NX51_DRV_CB12_ADV_SELECT_1                   (0x0002)        /*  <00000> = Reserved                   */
#define NX51_DRV_CB12_ADV_SELECT_0                   (0x0001)        /*  <11111> = Reserved                   */

/* Register 17 - Mode Control/Status */
#define NX51_DRV_CB12_MODE_CTRL_STAT_FASTRIP                    (0x4000)        /* 10BASE-T fast mode                       */
#define NX51_DRV_CB12_MODE_CTRL_STAT_EDPWRDOWN                  (0x2000)        /* Enable the Energy Detect Power-Down mode */
#define NX51_DRV_CB12_MODE_CTRL_STAT_LOWSQEN                    (0x0800)        /* Low squelch signal                       */
#define NX51_DRV_CB12_MODE_CTRL_STAT_MDPREBP                    (0x0400)        /* Management Data Preamble bypass          */
#define NX51_DRV_CB12_MODE_CTRL_STAT_FARLOOPBACK                (0x0200)        /* Remote Loopback enable                   */
#define NX51_DRV_CB12_MODE_CTRL_STAT_FASTEST                    (0x0100)        /* Auto-Negotiation Test mode               */
#define NX51_DRV_CB12_MODE_CTRL_STAT_AUTO_MDIX_EN               (0x0080)        /* AutoMDIX enable bit                      */
#define NX51_DRV_CB12_MODE_CTRL_STAT_MDI_MODE                   (0x0040)        /* Manual set of MDIX/MDI mode <1> = MDIX   */
#define NX51_DRV_CB12_MODE_CTRL_STAT_DCD_PAT_GEN                (0x0010)        /* Enables DCD measuring pattern generation */
#define NX51_DRV_CB12_MODE_CTRL_STAT_PHYADBP                    (0x0008)        /* PYH disregards PHY address in SMI access */
#define NX51_DRV_CB12_MODE_CTRL_STAT_FORCE_GOOD_LINK_STATUS     (0x0004)        /* Force 100BASE-X link active              */
#define NX51_DRV_CB12_MODE_CTRL_STAT_ENERGYON                   (0x0002)        /* Energy detected on line                  */

/* Register 18 - Special Modes */
#define NX51_DRV_CB12_SPECIAL_MODE_MIIMODE_1                    (0x8000)      /* <11> = res, <10> = SMII                              */
#define NX51_DRV_CB12_SPECIAL_MODE_MIIMODE_0                    (0x4000)      /* <01> = RMII, <00> = MII                              */
#define NX51_DRV_CB12_SPECIAL_MODE_CLKSELFREQ                   (0x2000)      /* Clock frequency selection <0> = 25 MHz, <1> = 50 MHz */
#define NX51_DRV_CB12_SPECIAL_MODE_FX_MODE                      (0x0400)      /* Enable 100BASE-FX mode                      */
#define NX51_DRV_CB12_SPECIAL_MODE_FARLOOPBACK                  (0x0200)      /* Remote Loopback enable                      */
#define NX51_DRV_CB12_SPECIAL_MODE_PYH_MODE_2                   (0x0080)      /* PHY mode of operation Refer to 7.2 in spec  */
#define NX51_DRV_CB12_SPECIAL_MODE_PYH_MODE_1                   (0x0040)      /* PHY mode of operation Refer to 7.2 in spec  */
#define NX51_DRV_CB12_SPECIAL_MODE_PYH_MODE_0                   (0x0020)      /* PHY mode of operation Refer to 7.2 in spec  */
#define NX51_DRV_CB12_SPECIAL_MODE_PHY_ADD_4                    (0x0010)      /* PHY address Refer to 7.1 in spec            */
#define NX51_DRV_CB12_SPECIAL_MODE_PHY_ADD_3                    (0x0008)      /* PHY address Refer to 7.1 in spec            */
#define NX51_DRV_CB12_SPECIAL_MODE_PHY_ADD_2                    (0x0004)      /* PHY address Refer to 7.1 in spec            */
#define NX51_DRV_CB12_SPECIAL_MODE_PHY_ADD_1                    (0x0002)      /* PHY address Refer to 7.1 in spec            */
#define NX51_DRV_CB12_SPECIAL_MODE_PHY_ADD_0                    (0x0001)      /* PHY address Refer to 7.1 in spec            */

/* Register 27 - Special Control/Status indications */
#define NX51_DRV_CB12_SPECIAL_CTRL_STAT_SWRST_FAST              (0x1000)      /* SW reset counter testing                           */
#define NX51_DRV_CB12_SPECIAL_CTRL_STAT_SQEOFF                  (0x0800)      /* Disable SQE (Heartbeat) test                       */
#define NX51_DRV_CB12_SPECIAL_CTRL_STAT_FEFIEN                  (0x0020)      /* Far end fault indication enable                    */
#define NX51_DRV_CB12_SPECIAL_CTRL_STAT_XPOL                    (0x0010)      /* Polarity state of 10BASE-T <1> = reversed polarity */

/* Register 31 - PHY Special Control/Status indications */
#define NX51_DRV_CB12_PYH_SPECIAL_CTRL_STAT_AUTODONE            (0x1000)      /* Auto-Negotiation done         */
#define NX51_DRV_CB12_PYH_SPECIAL_CTRL_STAT_4B5BEN              (0x0040)      /* Enable 4B5B Encoding/Decoding */
#define NX51_DRV_CB12_PYH_SPECIAL_CTRL_STAT_DATA_SCRAMBLING_DIS (0x0001)      /* Scramble disabling            */


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

int NX51_MIIMU_ReadPhyReg   ( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, uint16_t* pusData, void* pvUser);
int NX51_MIIMU_WritePhyReg  ( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuWtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, uint16_t usData, void* pvUser);


#endif /* #ifndef __NETX56_MIIMU_H */
