#ifndef __HAL_EXTPHY_H
#define __HAL_EXTPHY_H

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
} PHY_MODE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

unsigned int HALDEC(EXTPHY_Read)( unsigned int uiPhyCtrlInst,
                                  unsigned int uiPhy,
                                  unsigned int uiReg );

void HALDEC(EXTPHY_Write)( unsigned int uiPhyCtrlInst,
                           unsigned int uiPhy,
                           unsigned int uiReg,
                           unsigned int uiData );

int HALDEC(EXTPHY_GetLinkState)( unsigned int uiPhyCtrlInst,
                                 unsigned int uPhy,
                                 unsigned int* puLink,
                                 unsigned int* puSpeedMbps,
                                 unsigned int* puFullDuplex,
                                 void* pvUser );

int HALDEC(EXTPHY_Init)( unsigned int uiPhyCtrlInst,
                         unsigned int uiPhy,
                         PHY_MODE_E ePhyMode );

#endif /* #ifndef __HAL_EXTPHY_H */
