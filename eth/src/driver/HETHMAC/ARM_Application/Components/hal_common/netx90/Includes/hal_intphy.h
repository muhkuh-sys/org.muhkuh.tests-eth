#ifndef __HAL_INTPHY_H
#define __HAL_INTPHY_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/* Definitions */
#define INTPHY_PORT_CNT 2
#define INTPHY_ADDR0 0
#define INTPHY_ADDR1 1

/* ### EPHY specific ### */

/* Register banks */
#define REG_BANK_DSP    0
#define REG_BANK_WOL    1
#define REG_BANK_BIST   3
#define REG_BANK_VMDAC  7

/* MDIO register set */
#define REG_BASIC_CONTROL                    0
#define REG_BASIC_STATUS                     1
#define REG_PHY_ID1                          2
#define REG_PHY_ID2                          3
#define REG_AUTONEG_ADV                      4
#define REG_AUTONEG_ADV_LP_ABILITY           5
#define REG_AUTONEG_EXPANSION                6
#define REG_NEXT_PAGE_TRANSMIT               7
#define REG_SILICON_REVISION                16
#define REG_MODE_CONTROL_STATUS             17
#define REG_SPECIAL_MODES                   18
#define REG_SMII_CONFIG_STATUS              19
#define REG_TSTCNTL                         20
#define REG_TSTREAD1                        21
#define REG_TSTREAD2                        22
#define REG_TSTWRITE                        23
#define REG_POWER_DOWN_CONTROL              24
#define REG_MDIO_TIMEOUT_CONTROL            25
#define REG_RX_ERROR_CNT_100MBIT            26
#define REG_CONTROL_STATUS_INDICATION       27
#define REG_SPECIAL_INT_TESTABILITY_CTRL    28
#define REG_INTERRUPT_SOURCE                29
#define REG_INTERRUPT_MASK                  30
#define REG_PHY_SPECIAL_CONTROL_STATUS      31

/* Analog/DSP register set */
#define REG_DCBLW                            0 /* WO */
#define REG_DBGCNTL                          1 /* WO */
#define REG_AMP_THRESH_1                     2 /* WO */
#define REG_AMP_THRESH_2                     3 /* WO */
#define REG_AMP_THRESH_3                     4 /* WO */
#define REG_AMP_THRESH_4                     5 /* WO */
#define REG_AMP_THRESH_5                     6 /* WO */
#define REG_AMP_THRESH_6                     7 /* WO */
#define REG_AMP_THRESH_7                     8 /* WO */
#define REG_ON_ENERGY_LEVEL                  9 /* WO */
#define REG_OFF_ENERGY_LEVEL_0              10 /* WO */
#define REG_OFF_ENERGY_LEVEL_1              11 /* WO */
#define REG_ENERGY_CNT_THRSH                12 /* WO */
#define REG_BINIT_0123                      13 /* WO */
#define REG_BINIT_4567                      14 /* WO */
#define REG_DP_BASE_QTH                     15 /* WO */
#define REG_DIFFZ                           16 /* WO */
#define REG_CONFIG                          17 /* WO */
#define REG_A1_CONFIG                       18 /* WO */
#define REG_A2_CONFIG                       19 /* WO */
#define REG_A3_CONFIG                       20 /* RW */
#define REG_A4_CONFIG                       21 /* RW */
#define REG_A5_CONFIG                       22 /* RW */
#define REG_A6_CONFIG                       23 /* RW */
#define REG_A7_CONFIG                       24 /* WO */
#define REG_CONFIG2                         25 /* WO */
#define REG_A8_CONFIG                       26 /* WO */
#define REG_A9_CONFIG                       27 /* WO */
#define REG_A10_CONFIG                      28 /* WO */
#define REG_A11_CONFIG                      29 /* WO */
#define REG_GAIN1                           30 /* WO */
#define REG_GAIN2                           31 /* WO */

/* WOL register set */
#define REG_MAC_DA_ADDR_15_TO_0              0 /* */
#define REG_MAC_DA_ADDR_31_TO_16             1 /* */
#define REG_MAC_DA_ADDR_47_TO_32             2 /* */
#define REG_WOL_CONTROL                      3 /* */
#define REG_EPHY_LPI_CTRL                    4 /* */
#define REG_EPHY_LPI_RX_TW_TS_TIMER          5 /* */
#define REG_EPHY_LPI_RX_TI_TQ_TIMER          6 /* */
#define REG_EPHY_LPI_RX_LINK_FAIL_TIMER      7 /* */
#define REG_EPHY_LPI_TX_TIMER                8 /* */
#define REG_WOL_STATUS                      10 /* */
#define REG_EPHY_LPI_RX_WAKE_ERR_CNTR       11 /* */
#define REG_EPHY_LPI_STATUS                 12 /* */
#define REG_SIGDET_HYST_CTRL                14 /* */
#define REG_SIGDET_CTL                      15 /* */
#define REG_SIGDET_HYST_WIN_CTRL            16 /* */

/* BIST register set */
#define REG_BIST_CTRL                        0 /* RW */
#define REG_BIST_STATUS                      1 /* RO */
#define REG_BIST_PREAMBLE_SFD_IPG_LENGTH     2 /* RW */
#define REG_BIST_PACKET_DATA_LENGTH          3 /* RW */
#define REG_BIST_DA_0                        4 /* RW */
#define REG_BIST_DA_1                        5 /* RW */
#define REG_BIST_DA_2                        6 /* RW */
#define REG_BIST_SA_0                        7 /* RW */
#define REG_BIST_SA_1                        8 /* RW */
#define REG_BIST_SA_2                        9 /* RW */
#define REG_BIST_NIBBLE_CNT                 10 /* RO */
#define REG_BIST_ERROR_COUNT                11 /* RO */
#define REG_BIST_RECEIVED_NIBBLE_CNT        12 /* RO */
#define REG_BIST_MISC_CTRL                  13 /* RW */
#define REG_CLK_COMPARE_CTRL                14 /* ?? */
#define REG_CLK_COMPARE_CLK1_CNT            16 /* ?? */
#define REG_CLK_COMPARE_CLK2_CNT            17 /* ?? */
#define REG_CLK1_CNT_IN_REF_CLK_0           18 /* ?? */
#define REG_CLK1_CNT_IN_REF_CLK_1           19 /* ?? */
#define REG_CLK2_CNT_IN_REF_CLK_0           20 /* ?? */
#define REG_CLK2_CNT_IN_REF_CLK_1           21 /* ?? */
#define REG_CBL_DG_TDR_CTRL                 22 /* ?? */
#define REG_CBL_DG_TDR_MON_WIN_CTRL         23 /* ?? */
#define REG_CBL_DG_TDR_MAX_MIN_PEAK_THR     24 /* ?? */
#define REG_CBL_DG_TDR_MAX_MIN_PEAK         25 /* ?? */
#define REG_CBL_DG_TDR_PROPAGATION_DELAY    26 /* ?? */
#define REG_FR_PLL_CONTROL                  27 /* RW */
#define REG_FR_PLL_DIV0                     28 /* RW */
#define REG_FR_PLL_DIV1                     29 /* RW */
#define REG_FR_PLL_OUT_MON                  30 /* RO */
#define REG_RC_CAL_MON                      31 /* RO */

/* VMDAC register set */
#define REG_ZQ_CAL_CONTROL                   0 /* RW */
#define REG_ZQ_CAL_STATUS                    1 /* RO */
#define REG_VM_DAC_CONTROL0                  2 /* RW */
#define REG_VM_DAC_CONTROL1                  3 /* RW */
#define REG_MISC_PHY_CONTROL0                4 /* RW */
#define REG_MISC_PCS_CONTROL0                5 /* RW */
#define REG_MISC_PCS_CONTROL1                6 /* RW */
#define REG_MISC_STATUS                      7 /* RO */
#define REG_MISC_STATUS1                     8 /* RO */
#define REG_TXDAC_CODE_01                    9 /* RW */
#define REG_TXDAC_CODE_23                   10 /* RW */
#define REG_TXDAC_CODE_45                   11 /* RW */
#define REG_TXDAC_CODE_6                    12 /* RW */


/*****************************************************************************/
/*! PHY Control Register Mode                                                */
/*****************************************************************************/
#define INTPHY_MODE_10BASE_T_HD_NOAUTONEG                     0x0
#define INTPHY_MODE_10BASE_T_FD_NOAUTONEG                     0x1
#define INTPHY_MODE_100BASE_TXFX_HD_NOAUTONEG_CRSTXRX         0x2
#define INTPHY_MODE_100BASE_TXFX_FD_NOAUTONEG_CRSRX           0x3
#define INTPHY_MODE_100BASE_TX_HD_ADV_AUTONEG_CRSRXTX         0x4
#define INTPHY_MODE_REPEATER_AUTONEG_100BASE_TX_HD_ADV_CRSRX  0x5
#define INTPHY_MODE_POWER_DOWN_MODE                           0x6
#define INTPHY_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN            0x7

typedef enum
{
  INTPHY_MDIXMODE_MDI,
  INTPHY_MDIXMODE_MDIX,
  INTPHY_MDIXMODE_AUTO
} INTPHY_MDIXMODE_E;

/*****************************************************************************/
/*! PHY Control Register                                                     */
/*****************************************************************************/
typedef struct INTPHY_CTRL_Ttag
{
  unsigned int uiPhyMode;
  bool         fPhySimBypass; /* unused, just for compatibility with legacy netX */
  bool         fPhyReset;
  bool         afPhyFxMode[INTPHY_PORT_CNT];
  bool         afPhyAutoMdix[INTPHY_PORT_CNT];
  bool         afPhyEnable[INTPHY_PORT_CNT];
} INTPHY_CTRL_T;

/*****************************************************************************/
/*! Parameters                                                               */
/*****************************************************************************/
typedef enum INTPHY_PRM_Etag
{
  /* PHY 0 */
  INTPHY_PRM_PHY_CTRL_EXT0,
  INTPHY_PRM_PHY_GAIN_TABLE0,
  INTPHY_PRM_PHY_AMP_THRESH0_1,
  INTPHY_PRM_PHY_AMP_THRESH0_2,
  INTPHY_PRM_PHY_AMP_THRESH0_3,
  INTPHY_PRM_PHY_AMP_THRESH0_4,
  INTPHY_PRM_PHY_AMP_THRESH0_5,
  INTPHY_PRM_PHY_AMP_THRESH0_6,
  INTPHY_PRM_PHY_AMP_THRESH0_7,
  INTPHY_PRM_OFF_ENERGY_LEVEL0,
  INTPHY_PRM_BINIT0,
  INTPHY_PRM_MIIM_PREAMBLE_0,
  INTPHY_PRM_MIIM_FREQ_0,
  INTPHY_PRM_TRIM_ADC_0,
  INTPHY_PRM_TRIM_BLWDAC_0,
  INTPHY_PRM_TRIM_SQ_0,
  INTPHY_PRM_TRIM_IBIAS10_0,
  INTPHY_PRM_TRIM_IBIAS100_0,
  INTPHY_PRM_TRIM_TXAMP10_0,
  INTPHY_PRM_TRIM_TXAMP100_0,

  /* PHY 1 */
  INTPHY_PRM_PHY_CTRL_EXT1,
  INTPHY_PRM_PHY_GAIN_TABLE1,
  INTPHY_PRM_PHY_AMP_THRESH1_1,
  INTPHY_PRM_PHY_AMP_THRESH1_2,
  INTPHY_PRM_PHY_AMP_THRESH1_3,
  INTPHY_PRM_PHY_AMP_THRESH1_4,
  INTPHY_PRM_PHY_AMP_THRESH1_5,
  INTPHY_PRM_PHY_AMP_THRESH1_6,
  INTPHY_PRM_PHY_AMP_THRESH1_7,
  INTPHY_PRM_OFF_ENERGY_LEVEL1,
  INTPHY_PRM_BINIT1,
  INTPHY_PRM_MIIM_PREAMBLE_1,
  INTPHY_PRM_MIIM_FREQ_1,
  INTPHY_PRM_TRIM_ADC_1,
  INTPHY_PRM_TRIM_BLWDAC_1,
  INTPHY_PRM_TRIM_SQ_1,
  INTPHY_PRM_TRIM_IBIAS10_1,
  INTPHY_PRM_TRIM_IBIAS100_1,
  INTPHY_PRM_TRIM_TXAMP10_1,
  INTPHY_PRM_TRIM_TXAMP100_1,

  /* Common */
  INTPHY_PRM_PHY_CTRL,
  INTPHY_PRM_STATUS,
  INTPHY_PRM_I2C_WRITE,
  INTPHY_PRM_I2C_READ,
  INTPHY_PRM_DEBUG,
  INTPHY_PRM_IRQ_RAW,
  INTPHY_PRM_IRQ_MASKED,
  INTPHY_PRM_IRQ_MASK_SET,
  INTPHY_PRM_IRQ_MASK_RESET,
  INTPHY_PRM_TRIM_PLL,
} INTPHY_PRM_E;

/* Function prototypes */
#ifndef HALDEC
#define HALDEC(name) name
#endif

void         HALDEC(INTPHY_Init)(void);
void         HALDEC(INTPHY_SetPhyCtrl)(const INTPHY_CTRL_T* ptPhyCtrl);
void         HALDEC(INTPHY_Reset)(const INTPHY_CTRL_T* ptPhyCtrl);

uint32_t     HALDEC(INTPHY_PrmGet)(INTPHY_PRM_E ePrmID );
int          HALDEC(INTPHY_PrmSet)(INTPHY_PRM_E ePrmID, uint32_t ulPrmVal);

unsigned int HALDEC(INTPHY_Read)(unsigned int uiPhy, unsigned int uiAddr);
void         HALDEC(INTPHY_Write)(unsigned int uiPhy, unsigned int uiAddr, unsigned int uiVal);
uint32_t     HALDEC(INTPHY_ReadBank)(unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr);
void         HALDEC(INTPHY_WriteBank)(unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal);

uint16_t     HALDEC(INTPHY_I2cRead)(unsigned int uiPhy, unsigned int uiAddr);
void         HALDEC(INTPHY_I2cWrite)(unsigned int uiPhy, unsigned int uiAddr, uint16_t usWriteData);
uint32_t     HALDEC(INTPHY_I2cReadBank)(unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr);
void         HALDEC(INTPHY_I2cWriteBank)(unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal);

void         HALDEC(INTPHY_Fixup)(unsigned int uiPhy);
void         HALDEC(INTPHY_CorrectAutoMdix)(unsigned int uiPhy);

#endif /* #ifndef __HAL_INTPHY_H */
