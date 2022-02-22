/**
 * \file adin1100_regdef.h
 * \brief Register definition of ADIN1100 external PHY chip revision U1
 *
 * \author Marvin Mager - mmager@hilscher.com
 * \date 04.01.2021
 */

#ifndef ADIN1100_REGDEF_H_
#define ADIN1100_REGDEF_H_

/* BASE-T1 PMA/PMD Extention Ability Register */
#define AD1100_PMA_PMD_BT1_ABILITY_REG 0x010012U
#define AD1100_B100_ABILITY 0                   /* 100BASE-T1 Ability, 1 bit RO */
#define AD1100_B1000_ABILITY 1                  /* 1000BASE-T1 Ability, 1 bit RO */
#define AD1100_B10L_ABILITY 2                   /* 10BASE-T1L Ability, 1 bit RO */
#define AD1100_B10S_ABILITY 3                   /* 10BASE-T1S Ability, 1 bit RO */

/* BASE-T1 PMA/PMD Control Register */
#define AD1100_PMA_PMD_BT1_CONTROL_REG 0x010834U
#define AD1100_TYPE_SEL 0                       /* Type Selection, 4 bits R/W */
#define AD1100_CFG_MST 14                       /* Master-Slave Configuration, 1 bit R/W */

/* 10BASE-T1L PMA Control Register */
#define AD1100_B10L_PMA_CNTRL_REG 0x0108F6U
#define AD1100_B10L_LB_PMA_LOC_EN 0             /* 10BASE-T1L PMA Loopback, 1 bit R/W */
#define AD1100_B10L_EEE 10                      /* 10BASE-T1L EEE Enable, 1 bit R/W */
#define AD1100_B10L_TX_LVL_HI 12                /* 10BASE-T1L Transmit Voltage Amplitude Control,
                                                   1 bit R/W*/
#define AD1100_B10L_TX_DIS_MODE_EN 14           /* 10BASE-T1L Transmit Disable Mode, 1 bit R/W */

/* 10BASE-T1L PMA Status Register */
#define AD1100_B10L_PMA_STAT_REG 0x0108F7U
#define AD1100_B10L_POL_INV 2                   /* 10BASE-T1L Polarity Inverse, 1 bit RO */
#define AD1100_B10L_EEE_ABLE 10                 /* 10BASE-T1L EEE Ability, 1 bit RO */
#define AD1100_B10L_TX_LVL_HI_ABLE 12           /* 10BASE-T1L High Voltage Tx Ability, 1 bit RO */
#define AD1100_B10L_LB_PMA_LOC_ABLE 13          /* 10BASE-T1L PMA Loopback Ability, 1 bit RO */

/* 10BASE-T1L Test Mode Control Register */
#define AD1100_B10L_TEST_MODE_CNTRL_REG 0x0108F8U
#define AD1100_B10L_TX_TEST_MODE 13             /* 10BASE-T1L Transmitter Test Mode, 3 bit R/W */

/* 10BASE-T1L PMA Link Status Register */
#define AD1100_B10L_PMA_LINK_STAT_REG 0x018302U
#define AD1100_B10L_LINK_STAT_OK 0              /* 10BASE-T1L Link Status Okay, 1 bit RO */
#define AD1100_B10L_LINK_STAT_OK_LL 1           /* 10BASE-T1L Link Status Okay, 1 bit RO, latching low */
#define AD1100_B10L_DSCR_STAT_OK 8              /* 10BASE-T1L Descrambler Status Okay, 1 bit RO */
#define AD1100_B10L_DSCR_STAT_OK_LL 9           /* 10BASE-T1L Descrambler Status Okay, 1 bit RO,
                                                   latching low */
#define AD1100_B10L_LOC_RCVR_STAT_OK 10         /* 10BASE-T1L Local Receiver Status Okay, 1 bit RO */
#define AD1100_B10L_LOC_RCVR_STAT_OK_LL 11      /* 10BASE-T1L Local Receiver Status Okay, 1 bit RO,
                                                   latching low */
#define AD1100_B10L_REM_RCVR_STAT_OK 12         /* 10BASE-T1L Remote Receiver Status Okay, 1 bit RO */
#define AD1100_B10L_REM_RCVR_STAT_OK_LL 13      /* 10BASE-T1L Remote Receiver Status Okay, 1 bit RO,
                                                   latching low */

/* 10BASE-T1L PCS Control Register */
#define AD1100_B10L_PCS_CNTRL_REG 0x0308E6U
#define AD1100_B10L_LB_PCS_EN 14                /* PCS Loopback Enable, 1 bit R/W */

/* BASE-T1 Auto-Negotiation Control Register */
#define AD1100_AN_CONTROL_REG 0x070200U
#define AD1100_AN_RESTART 9                     /* Auto-Negotiation Restart, 1 bit R/W, self clearing */
#define AD1100_AN_EN 12                         /* Auto-Negotiation Enable, 1 bit R/W */

/* BASE-T1 Auto-Negotiation Status Register */
#define AD1100_AN_STATUS_REG 0x070201U
#define AD1100_AN_LINK_STATUS 2                 /* Auto-Negotiation Link Status, 1 bit RO, latching low */
#define AD1100_AN_ABLE 3                        /* Auto-Negotiation Ability, 1 bit RO */
#define AD1100_AN_COMPLETE 5                    /* Auto-Negotiation Complete, 1 bit RO */
#define AD1100_AN_PAGE_RX 6                     /* Page Received, 1 bit RO, latching high */

/* BASE-T1 Auto-Negotiation Advertisement [15:0] Register */
#define AD1100_AN_ADV_ABILITY_L_REG 0x070202U
#define AD1100_AN_ADV_SELECTOR 0                /* AN Adv Selector, 5 bit RO */
#define AD1100_AN_ADV_PAUSE 10                  /* AN Adv Pause Ability, 2 bit R/W */
#define AD1100_AN_ADV_FORCE_MS 12               /* AN Adv Force Master/Slave Configuration, 1 bit R/W */
#define AD1100_AN_ADV_REMOTE_FAULT 13           /* AN Adv Remote Fault, 1 bit R/W */
#define AD1100_AN_ADV_ACK 14                    /* AN Adv Acknowledge, 1 bit R/W */
#define AD1100_AN_ADV_NEXT_PAGE_REQ 15          /* AN Adv Next Page Request, 1 bit R/W */

/* BASE-T1 Auto-Negotiation Advertisement [31:16] Register */
#define AD1100_AN_ADV_ABILITY_M_REG 0x070203U
#define AD1100_AN_ADV_MST 4                     /* AN Adv Master/Slave Configuration, 1 bit R/W */
#define AD1100_AN_ADV_B10L 14                   /* AN Adv 10BASE-T1L Ability, 1 bit R/W */

/* BASE-T1 Auto-Negotiation Advertisement [47:32] Register */
#define AD1100_AN_ADV_ABILITY_H_REG 0x070204U
#define AD1100_AN_ADV_B10L_TX_LVL_HI_REQ 12     /* AN Adv High Level Transmit Operating Mode
                                                   Request, 1 bit R/W */
#define AD1100_AN_ADV_B10L_TX_LVL_HI_ABL 13     /* AN Adv High Level Transmit Operating Mode
                                                   Ability, 1 bit R/W */

/* BASE-T1 Auto-Negotiation Link Partner Base Page Ability [15:0] Register */
#define AD1100_AN_LP_ADV_ABILITY_L_REG 0x070205U
#define AD1100_AN_LP_SELECTOR 0                 /* AN Link Partner Selector, 5 bit RO */
#define AD1100_AN_LP_ADV_PAUSE 10               /* AN Link Partner Pause Ability, 2 bit RO */
#define AD1100_AN_LP_ADV_FORCE_MS 12            /* AN Link Partner Force Master/Slave Configuration,
                                                   1 bit RO */
#define AD1100_AN_LP_ADV_REMOTE_FAULT 13        /* AN Link Partner Remote Fault, 1 bit RO */
#define AD1100_AN_LP_ADV_ACK 14                 /* AN Link Partner Acknowledge, 1 bit RO */
#define AD1100_AN_LP_ADV_NEXT_PAGE_REQ 15       /* AN Link Partner Next Page Request, 1 bit RO */

/* BASE-T1 Auto-Negotiation Link Partner Base Page Ability [31:16] Register */
#define AD1100_AN_LP_ADV_ABILITY_M_REG 0x070206U
#define AD1100_AN_LP_ADV_MSTR 4                 /* AN Link Partner Master/Slave Configuration,
                                                   1 bit RO */
#define AD1100_AN_LP_ADV_B100 5                 /* AN Link Partner 100BASE-T1 Ability, 1 bit RO */
#define AD1100_AN_LP_ADV_B10S_FD 6              /* AN Link Partner 10BASE-T1S Full Duplex Ability,
                                                   1 bit RO */
#define AD1100_AN_LP_ADV_B1000 7                /* AN Link Partner 1000BASE-T1 Ability, 1 bit RO */
#define AD1100_AN_LP_ADV_B10L 14                /* AN Link Partner 10BASE-T1L Ability, 1 bit RO */

/* BASE-T1 Auto-Negotiation Link Partner Base Page Ability [47:32] Register */
#define AD1100_AN_LP_ADV_ABILITY_H_REG 0x070207U
#define AD1100_AN_LP_ADV_B10S_HD 11             /* AN Link Partner 10BASE-T1S Half Duplex Ability,
                                                   1 bit RO */
#define AD1100_AN_LP_ADV_B10L_TX_LVL_HI_REQ 12  /* AN Link Partner 10BASE-T1L High Level Transmit
                                                   Operating Mode Request, 1 bit RO */
#define AD1100_AN_LP_ADV_B10L_TX_LVL_HI_ABL 13  /* AN Link Partner 10BASE-T1L High Level Transmit
                                                   Operating Mode Ability, 1 bit RO */
#define AD1100_AN_LP_ADV_B10L_EEE_ABL 14        /* AN Link Partner 10BASE-T1L EEE Ability, 1 bit RO */

/* BASE-T1 Auto-Negotiation Next Page Transmit [15:0] Register */
#define AD1100_AN_NEXT_PAGE_L_REG 0x070208U
#define AD1100_AN_NP_MESSAGE_CODE 0             /* AN Next Page Message/Unformatted Code Field,
                                                   11 bits R/W */
#define AD1100_AN_NP_TOOGLE 11                  /* AN Next Page Toggle Bit, 1 bit RO */
#define AD1100_AN_NP_ACK2 12                    /* AN Next Page Acknowledge 2, 1 bit R/W */
#define AD1100_AN_NP_MESSAGE_PAGE 13            /* AN Next Page Encoding, 1 bit R/W */
#define AD1100_AN_NP_ACK 14                     /* AN Next Page Acknowledge, 1 bit RO */
#define AD1100_AN_NP_NEXT_PAGE_REQ 15           /* AN Next Page Request, 1 bit R/W */

/* BASE-T1 Auto-Negotiation Next Page Transmit [31:16] Register */
#define AD1100_AN_NEXT_PAGE_M_REG 0x070209U
#define AD1100_AN_NP_UNFORMATTED1 0             /* AN Next Page Unformatted Code Field 1, 16 bits R/W */

/* BASE-T1 Auto-Negotiation Next Page Transmit [47:32] Register */
#define AD1100_AN_NEXT_PAGE_H_REG 0x07020AU
#define AD1100_AN_NP_UNFORMATTED2 0             /* AN Next Page Unformatted Code Field 2, 16 bits R/W */

/* BASE-T1 Auto-Negotiation Link Partner Next Page Transmit [15:0] Register */
#define AD1100_AN_LP_NEXT_PAGE_L_REG 0x07020BU
#define AD1100_AN_LP_NP_MESSAGE_CODE 0          /* AN Link Partner Next Page Message/Unformatted
                                                   Code Field, 11 bits RO */
#define AD1100_AN_LP_NP_TOOGLE 11               /* AN Link Partner Next Page Toggle Bit, 1 bit RO */
#define AD1100_AN_LP_NP_ACK2 12                 /* AN Link Partner Next Page Acknowledge 2, 1 bit RO */
#define AD1100_AN_LP_NP_MESSAGE_PAGE 13         /* AN Link Partner Next Page Encoding, 1 bit RO */
#define AD1100_AN_LP_NP_ACK 14                  /* AN Link Partner Next Page Acknowledge, 1 bit RO */
#define AD1100_AN_LP_NP_NEXT_PAGE_REQ 15        /* AN Link Partner Next Page Request, 1 bit RO */

/* BASE-T1 Auto-Negotiation Link Partner Next Page Transmit [31:16] Register */
#define AD1100_AN_LP_NEXT_PAGE_M_REG 0x07020CU
#define AD1100_AN_LP_NP_UNFORMATTED1 0          /* AN Link Partner Next Page Unformatted Code Field 1,
                                                   16 bits RO */

/* BASE-T1 Auto-Negotiation Link Partner Next Page Transmit [47:32] Register */
#define AD1100_AN_LP_NEXT_PAGE_H_REG 0x07020DU
#define AD1100_AN_LP_NP_UNFORMATTED2 0          /* AN Link Partner Next Page Unformatted Code Field 2,
                                                   16 bits RO */

/* Extra Auto-Negotiation Status Register */
#define AD1100_AN_STATUS_EXTRA_REG 0x078001U
#define AD1100_AN_LINK_GOOD 0                   /* AN Complete Indication, 1 bit RO */
#define AD1100_AN_HCD_TECH 1                    /* AN Highest Common Denominator PHY Technology,
                                                   4 bits RO */
#define AD1100_AN_MS_CONFIG_RSLTN 5             /* AN Master/Slave Resolution Result, 2 bits RO */
#define AD1100_AN_INC_LINK 7                    /* AN Incompatible Link Indication, 1 bit RO */
#define AD1100_AN_LP_NP_RX 8                    /* AN Next Page Request Received from Link Partner,
                                                   1 bit RO, latching high */

/* Vendor Specific MMD 1 Device Identifier High Register */
#define AD1100_MMD1_DEV_ID1_REG 0x1E0002U
#define AD1100_MMD1_DEV_ID1 0                   /* Organizationally Unique Identifier Bits [3:18],
                                                   16 bits RO */

/* Vendor Specific MMD 1 Device Identifier Low Register */
#define AD1100_MMD1_DEV_ID2_REG 0x1E0003U
#define AD1100_MMD1_REV_NUM 0                   /* Revision Number, 4 bits RO */
#define AD1100_MMD1_MODEL_NUM 4                 /* Model Number, 6 bits RO */
#define AD1100_MMD1_DEV_ID2_OUI 10              /* Organizationally Unique Identifier Bits[19:24],
                                                   6 bits RO */

/* CRSM Interrupt Status Register */
#define AD1100_CRSM_IRQ_STATUS_REG 0x1E0010U
#define AD1100_CRSM_MDIO_ERR_IRQ_LH 0           /* CRSM MDIO Frame Error Interrupt, 1 bit RO,
                                                   latching high, obsolete in datasheet rev F!*/
#define AD1100_CRSM_HRD_REST_IRQ_LH 11          /* CRSM Hardware Reset Interrupt, 1 bit RO,
                                                   latching high */
#define AD1100_CRSM_SW_IRQ_LH 15                /* CRSM Software Requested Interrupt Event,
                                                   1 bit RO, latching high*/

/* PHY Sub-system Interrupt Status Register */
#define AD1100_PHY_SUBSYS_IRQ_STATUS_REG 0x1E0011U
#define AD1100_PHY_SUBSYS_MDIO_ERR_LH 0         /* PHY Sub-system MDIO Framing Error, 1 bit RO,
                                                   latching high*/
#define AD1100_LINK_STAT_CHNG_LH 1              /* Link Status Change, 1 bit RO, latching high */
#define AD1100_AN_STAT_CHNG_IRQ_LH 11           /* Auto-Negotiation Status Change Interrupt, 1 bit RO,
                                                   latching high */
#define AD1100_MAC_IF_EBUF_ERR_IRQ_LH 13        /* MAC Interface Buffers Overflow/Underflow
                                                   Interrupt, 1 bit RO, latching high */
#define AD1100_MAC_IF_FC_FG_IRQ_LH 14           /* MAC Interface Frame Checker/Generator
                                                   Interrupt, 1 bit RO, latching high */

/* CRSM Interrupt Mask Register */
#define AD1100_CRSM_IRQ_MASK_REG 0x1E0020U
#define AD1100_CRSM_MDIO_ERR_IRQ_EN 0           /* Enable CRSM MDIO Frame Error Interrupt, 1 bit R/W */
#define AD1100_CRSM_HRD_REST_IRQ_EN 11          /* Enable CRSM Hardware Reset Interrupt, 1 bit R/W */
#define AD1100_CRSM_SW_IRQ_REQ 15               /* CRSM Software Requested Interrupt, 1 bit R/W
                                                   This bit can be set to generate an Interrupt
                                                   for System Level Testing. This Bit Always
                                                   Reads back as 0 as it is self clearing */

/* PHY Sub-system Interrupt Mask Register */
#define AD1100_PHY_SUBSYS_IRQ_MASK_REG 0x1E0021U
#define AD1100_PHY_SUBSYS_MDIO_ERR_IRQ_EN 0     /* Enable PHY Sub-system MDIO Framing Error
                                                   Interrupt, 1 bit R/W */
#define AD1100_LINK_STAT_CHNG_IRQ_EN 1          /* Enable Link Status Change Interrupt, 1 bit R/W */
#define AD1100_AN_STAT_CHNG_IRQ_EN 11           /* Enable Auto-Negotiation Status Change Interrupt,
                                                   1 bit R/W */
#define AD1100_MAC_IF_EBUF_ERR_IRQ_EN 13        /* Enable MAC Interface Buffer Overflow/Underflow
                                                   Interrupt, 1 bit R/W */
#define AD1100_MAC_IF_FC_FG_IRQ_EN 14           /* Enable MAC Interface Frame Checker/Generator
                                                   Interrupt, 1 bit R/W */

/* Frame Checker Enable Register */
#define AD1100_FC_EN_REG 0x1E8001U
#define AD1100_FC_EN 0                          /* Frame Checker Enable, 1 bit R/W */

/* Frame Checker Interrupt Enable Register */
#define AD1100_FC_IRQ_EN_REG 0x1E8004U
#define AD1100_FC_IRQ_EN 0                      /* Frame Checker Interrupt Enable, 1 bit R/W */

/* Frame Checker Transmit Select Register */
#define AD1100_FC_TX_SEL_REG 0x1E8005U
#define AD1100_FC_TX_SEL 0                      /* Frame Checker Transmit Select, 1 bit R/W */

/* Receive Error Count Register */
#define AD1100_RX_ERR_CNT_REG 0x1E8008U
#define AD1100_RX_ERR_CNT 0                     /* Receive Error Count, 16 bits RO, self clearing */

/* Frame Checker Count High Register */
#define AD1100_FC_FRM_CNT_H_REG 0x1E8008U
#define AD1100_FC_FRM_CNT_H 0                   /* Bits [31:16] of Latched Copy of the Number of
                                                   Received Frames, 16 bits RO */

/* Frame Checker Count Low Register */
#define AD1100_FC_FRM_CNT_L_REG 0x1E8009U
#define AD1100_FC_FRM_CNT_L 0                   /* Bits [15:0] of Latched Copy of the Number of
                                                   Received Frames, 16 bits RO */

/* Frame Checker Length Error Count Register */
#define AD1100_FC_LEN_ERR_CNT_REG 0x1E800BU
#define AD1100_FC_LEN_ERR_CNT 0                 /* Latched Copy of the Frame Length Error Counter,
                                                   16 bits RO */

/* Frame Checker Alignment Error Count Register */
#define AD1100_FC_ALGN_ERR_CNT_REG 0x1E800CU
#define AD1100_FC_ALGN_ERR_CNT 0                /* Latched Copy of the Frame Alignment Error Counter,
                                                   16 bits RO */

/* Frame Checker Symbol Error Count Register */
#define AD1100_FC_SYMB_ERR_CNT_REG 0x1E800DU
#define AD1100_FC_SYMB_ERR_CNT 0                /* Latched Copy of the Symbol Error Counter,
                                                   16 bits RO */

/* Frame Checker Oversized Frame Count Register */
#define AD1100_FC_OSZ_CNT_REG 0x1E800EU
#define AD1100_FC_OSZ_CNT 0                     /* Latched Copy of the Oversized Frame Error
                                                   Counter, 16 bits RO */

/* Frame Checker Undersized Frame Count Register */
#define AD1100_FC_USZ_CNT_REG 0x1E800FU
#define AD1100_FC_USZ_CNT 0                     /* Latched Copy of the Undersized Frame Error
                                                   Counter, 16 bits RO */

/* Frame Checker Odd Nibble Frame Count Register */
#define AD1100_FC_ODD_CNT_REG 0x1E8010U
#define AD1100_FC_ODD_CNT 0                     /* Latched Copy of the Odd Nibble Error Counter,
                                                   16 bits RO */

/* Frame Checker Odd Preamble Packet Count Register */
#define AD1100_FC_ODD_PRE_CNT_REG 0x1E8011U
#define AD1100_FC_ODD_PRE_CNT 0                 /* Latched Copy of the Odd Preamble Error Counter,
                                                   16 bits RO */

/* Frame Checker False Carrier Count Register */
#define AD1100_FC_FALSE_CARRIER_CNT_REG 0x1E8013U
#define AD1100_FC_FALSE_CARRIER_CNT 0           /* Latched Copy of the False Carrier Error Counter,
                                                   16 bits RO */

/* Frame Generator Enable Register */
#define AD1100_FG_EN_REG 0x1E8015U
#define AD1100_FG_EN 0                          /* Frame Generator Enable, 1 bit R/W */

/* Frame Generator Control/Restart Register */
#define AD1100_FG_CNTRL_RSTRT 0x1E8016U
#define AD1100_FG_CNTRL 0                       /* Frame Generator Control, 3 bits R/W */
#define AD1100_FG_RSTRT 3                       /* Frame Generator Restart, 1 bit R/W, self clearing */

/* Frame Generator Continuous Mode Enable Register */
#define AD1100_FG_CONT_MODE_EN_REG 0x1E8017U
#define AD1100_FG_CONT_MODE_EN 0                /* Frame Generator Continuous/Burst Mode, 1 bit R/W */

/* Frame Generator Interrupt Enable Register */
#define AD1100_FG_IRQ_EN_REG 0x1E8018U
#define AD1100_FG_IRQ_EN 0                      /* Frame Generator Interrupt Enable, 1 bit R/W */

/* Frame Generator Frame Length Register */
#define AD1100_FG_FRM_LEN_REG 0x1E801AU
#define AD1100_FG_FRM_LEN 0                     /* Data Field Frame Length, 16 bit R/W */

/* Frame Generator Number of Frames High Register */
#define AD1100_FG_NFRM_H_REG 0x1E801CU
#define AD1100_FG_NFRM_H 0                      /* Bits [31:16] of the Number of Frames to be
                                                   generated, 16 bits R/W */

/* Frame Generator Number of Frames Low Register */
#define AD1100_FG_NFRM_L_REG 0x1E801DU
#define AD1100_FG_NFRM_L 0                      /* Bits [15:0] of the Number of Frames to be
                                                   generated, 16 bits R/W */

/* Frame Generator Done Register */
#define AD1100_FG_DONE_REG 0x1E801EU
#define AD1100_FG_DONE 0                        /* Frame Generator Done, 1 bit RO, latching high */

/* RMII Configuration Register */
#define AD1100_RMII_CFG 0x1E8038U
#define AD1100_RMII_TXD_CHK_EN 0                /* RMII TXD Check Enable, 1 bit R/W */

/* MAC Interface Loopback Configuration Register */
#define AD1100_MAC_IF_LOOPBACK_REG 0x1E803DU
#define AD1100_MAC_IF_LB_EN 0                   /* MAC Interface Loopback Enable, 1 bit R/W */
#define AD1100_MAC_IF_LB_TX_SUP_EN 1            /* Suppress Transmission Enable Enable, 1 bit R/W */
#define AD1100_MAC_IF_REM_LB_EN 2               /* MAC Interface Remote Loopback Enable, 1 bit R/W */
#define AD1100_MAC_IF_REM_LBRX_SUP_EN 3         /* Suppress RX Enable, 1 bit R/W */

/* CRSM Software Reset Register */
#define AD1100_CRSM_SFT_RST_REG 0x1E8810U
#define AD1100_CRSM_SFT_RST 0                   /* CRSM Software Reset Register, 1 bit R/W, self
                                                   clearing */

/* CRSM Software Power-Down Control Register */
#define AD1100_CRSM_SFT_PD_CNTRL_REG 0x1E8812U
#define AD1100_CRSM_SFT_PD 0                    /* CRSM Software Powerdown, 1 bit R/W */

/* CRSM PHY Subsystem Reset Register */
#define AD1100_CRSM_PHY_SUBSYS_RST_REG 0x1E8814U
#define AD1100_CRSM_PHY_SUBSYS_RST 0            /* CRSM PHY Subsystem Reset, 1 bit R/W */

/* CRSM PHY MAC Interface Reset Register */
#define AD1100_CRSM_MAC_IF_RST_REG 0x1E8815U
#define AD1100_CRSM_MAC_IF_RST 0                /* CRSM PHY MAC Interface Reset, 1 bit R/W, self
                                                   clearing */

/* CRSM Status Register */
#define AD1100_CRSM_STAT_REG 0x1E8818U
#define AD1100_CRSM_SYS_RDY 0                   /* CRSM System Ready, 1 bit RO */
#define AD1100_CRSM_SFT_PD_RDY 1                /* CRSM Software Powerdown Status, 1 bit RO */

/* MAC Interface Configuration Register */
#define AD1100_CRSM_MAC_IF_CFG_REG 0x1E882BU
#define AD1100_CRSM_RMII_MEDIA_CNV_EN 8         /* Media Converter Enable, 1 bit R/W */

/* LED Blink Time Control Register */
#define AD1100_LED_BLINK_TIME_CNTRL_REG 0x1E8C80U
#define AD1100_LED_OFF_N4MS 0                   /* LED off Blink Time x 4ms, 8 bits R/W */
#define AD1100_LED_ON_N4MS 8                    /* LED on Blink Time x 4ms, 8 bits R/W */

/* LED Control Register */
#define AD1100_LED_CNTRL_REG 0x1E8C81U
#define AD1100_LED_EN 0                         /* LED Enable, 1 bit R/W */


#endif /* ADIN1100_REGDEF_H_ */
