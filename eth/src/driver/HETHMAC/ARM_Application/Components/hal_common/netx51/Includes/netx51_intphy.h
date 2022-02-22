#ifndef __NETX51_INTPHY_H
#define __NETX51_INTPHY_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX51_INT_PHY1_ADDR                          0x00
#define NX51_INT_PHY2_ADDR                          0x01
#define NX51_PHY_CTRL_PHY_ADDR                       0x0 /* bits 4..1: PHY address, bit 0 indicate internal PHY 0 or 1 */
#define NX51_PHY_CTRL_PHY_FX_MODE                      0
#define NX51_PHY_CTRL_PHY_AUTOMDIX                     1
#define NX51_PHY_CTRL_PHY_ENABLE                       1
#define NX51_RESET_PHY_DELAY                    80000000 /* reset delay in 100us units */

/*****************************************************************************/
/*! PHY Control Register Mode                                                */
/*****************************************************************************/
typedef enum NX51_PHYCTRL_MODE_Etag {
  NX51_PHYCTRL_MODE_10BASE_T_HD_NOAUTONEG                     =  0x0,
  NX51_PHYCTRL_MODE_10BASE_T_FD_NOAUTONEG                     =  0x1,
  NX51_PHYCTRL_MODE_100BASE_TXFX_HD_NOAUTONEG_CRSTXRX         =  0x2,
  NX51_PHYCTRL_MODE_100BASE_TXFX_FD_NOAUTONEG_CRSRX           =  0x3,
  NX51_PHYCTRL_MODE_100BASE_TX_HD_ADV_AUTONEG_CRSRXTX         =  0x4,
  NX51_PHYCTRL_MODE_REPEATER_AUTONEG_100BASE_TX_HD_ADV_CRSRX  =  0x5,
  NX51_PHYCTRL_MODE_POWER_DOWN_MODE                           =  0x6,
  NX51_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN            =  0x7,
  NX51_PHYCTRL_MODE_SPECIAL_MODE_8                            =  0x8,
  NX51_PHYCTRL_MODE_SPECIAL_MODE_9                            =  0x9,
  NX51_PHYCTRL_MODE_SPECIAL_MODE_10                           =  0xa,
  NX51_PHYCTRL_MODE_SPECIAL_MODE_11                           =  0xb,
  NX51_PHYCTRL_MODE_SPECIAL_MODE_12                           =  0xc,
  NX51_PHYCTRL_MODE_SPECIAL_MODE_13                           =  0xd,
  NX51_PHYCTRL_MODE_SPECIAL_MODE_14                           =  0xe,
  NX51_PHYCTRL_MODE_LOOPBACK_ISOLATE                          =  0xf
} NX51_PHYCTRL_MODE_E;

/*****************************************************************************/
/*! PHY LED Flashing Mode                                                    */
/*****************************************************************************/
typedef enum NX51_INT_PHYCTRL_LED_MODE_Etag {
  NX51_INT_PHYCTRL_LED_MODE_MANUAL    = 0,
  NX51_INT_PHYCTRL_LED_MODE_STATIC    = 1,
  NX51_INT_PHYCTRL_LED_MODE_FLASHING  = 2,
  NX51_INT_PHYCTRL_LED_MODE_COMBINED  = 3
} NX51_INT_PHYCTRL_LED_MODE_E;


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void      NX51_INTPHY_Init( uint32_t ulPhyCtrlInit );
uint32_t  NX51_INTPHY_GetLedStatus( unsigned int uPhyNum );
int       NX51_INTPHY_ConfigLedBlink( unsigned int uPhyNum, NX51_INT_PHYCTRL_LED_MODE_E eBlinkMode, unsigned int uBlinkInterval );
int       NX51_INTPHY_ManualLedSet( unsigned int uPhyNum, bool fLed0, bool fLed1 );


#endif /* #ifndef __NETX51_INTPHY_H */
