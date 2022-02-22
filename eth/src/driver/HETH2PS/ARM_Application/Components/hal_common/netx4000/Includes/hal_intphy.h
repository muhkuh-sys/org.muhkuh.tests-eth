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
  INTPHY_PRM_PHY_CTRL
} INTPHY_PRM_E;

typedef enum
{
  INTPHY_MAUTYPE_10HD  = 0x0020,
  INTPHY_MAUTYPE_10FD  = 0x0040,
  INTPHY_MAUTYPE_100HD = 0x0080,
  INTPHY_MAUTYPE_100FD = 0x0100
} INTPHY_MAUTYPE_E;

#define MSK_INTPHY_AUTONEG_ADV_ALL_CAPABLE (INTPHY_MAUTYPE_10HD | INTPHY_MAUTYPE_10FD | INTPHY_MAUTYPE_100HD | INTPHY_MAUTYPE_100FD)

typedef enum
{
  INTPHY_MDIXMODE_MDI,
  INTPHY_MDIXMODE_MDIX,
  INTPHY_MDIXMODE_AUTO
} INTPHY_MDIXMODE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void         INTPHY_Init       (void);
void         INTPHY_SetupDualPhy(void); /* Mode 7, both PHYs enabled (copper), PowerDown via PHY register access */
void         INTPHY_SetModePowerDown(unsigned int uiPhy);
void         INTPHY_SetModeAutoNeg(unsigned int uiPhy, unsigned int uiAutonegAdv);
void         INTPHY_SetModeForced(unsigned int uiPhy, INTPHY_MAUTYPE_E eMauType, INTPHY_MDIXMODE_E eMdixMode);

void         INTPHY_SetPhyCtrl (const INTPHY_CTRL_T* ptPhyCtrl);
void         INTPHY_Reset      (const INTPHY_CTRL_T* ptPhyCtrl);
uint32_t     INTPHY_PrmGet     (INTPHY_PRM_E ePrmID);
int          INTPHY_PrmSet     (INTPHY_PRM_E ePrmID, uint32_t ulPrmVal);
unsigned int INTPHY_Read       (unsigned int uiPhy, unsigned int uiAddr);
void         INTPHY_Write      (unsigned int uiPhy, unsigned int uiAddr, unsigned int uiVal);
uint32_t     INTPHY_ReadBank   (unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr);
void         INTPHY_WriteBank  (unsigned int uiPhy, unsigned int uiBank, unsigned int uiAddr, unsigned int uiVal);

#endif /* #ifndef __HAL_INTPHY_H */
