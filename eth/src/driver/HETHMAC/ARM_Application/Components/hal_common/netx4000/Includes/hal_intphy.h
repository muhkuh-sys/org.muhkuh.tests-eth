#ifndef __HAL_INTPHY_H
#define __HAL_INTPHY_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define INTPHY_PORT_CNT 2
#define INTPHY_ADDR0 0
#define INTPHY_ADDR1 1

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
#define INTPHY_MODE_SPECIAL_MODE_8                            0x8
#define INTPHY_MODE_SPECIAL_MODE_9                            0x9
#define INTPHY_MODE_SPECIAL_MODE_10                           0xa
#define INTPHY_MODE_SPECIAL_MODE_11                           0xb
#define INTPHY_MODE_SPECIAL_MODE_12                           0xc
#define INTPHY_MODE_SPECIAL_MODE_13                           0xd
#define INTPHY_MODE_SPECIAL_MODE_14                           0xe
#define INTPHY_MODE_LOOPBACK_ISOLATE                          0xf

/*****************************************************************************/
/*! PHY Control Register                                                     */
/*****************************************************************************/
typedef struct INTPHY_CTRL_Ttag
{
  unsigned int uiPhyMode;
  bool         fPhySimBypass;
  bool         fPhyReset;
  bool         afPhyFxMode[INTPHY_PORT_CNT];
  bool         afPhyAutoMdix[INTPHY_PORT_CNT];
  bool         afPhyEnable[INTPHY_PORT_CNT];
} INTPHY_CTRL_T;

/*****************************************************************************/
/*! Parameters                                                               */
/*****************************************************************************/
typedef enum INTPHY_CTRL_PRM_Etag
{
  INTPHY_PRM_MIIM_PREAMBLE_0,
  INTPHY_PRM_MIIM_PREAMBLE_1,
  INTPHY_PRM_MIIM_FREQ_0,
  INTPHY_PRM_MIIM_FREQ_1,
} INTPHY_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

void         HALDEC(INTPHY_Init)       (void);
void         HALDEC(INTPHY_SetPhyCtrl) (const INTPHY_CTRL_T* ptPhyCtrl);
void         HALDEC(INTPHY_Reset)      (const INTPHY_CTRL_T* ptPhyCtrl);
uint32_t     HALDEC(INTPHY_PrmGet)     (INTPHY_PRM_E ePrmID);
int          HALDEC(INTPHY_PrmSet)     (INTPHY_PRM_E ePrmID, uint32_t ulPrmVal);
unsigned int HALDEC(INTPHY_Read)       (unsigned int uiPhy, unsigned int uiAddr);
void         HALDEC(INTPHY_Write)      (unsigned int uiPhy, unsigned int uiAddr, unsigned int uiVal);
uint32_t     HALDEC(INTPHY_ReadBank)   (unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr);
void         HALDEC(INTPHY_WriteBank)  (unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal);

#endif /* #ifndef __HAL_INTPHY_H */
