#include "network_interface.h"

#ifndef __DRV_ETH_XC_H__
#define __DRV_ETH_XC_H__


typedef enum PHYCTRL_MODE_Etag
{
	PHYCTRL_MODE_10BASE_T_HD_NOAUTONEG                     =  0x0,
	PHYCTRL_MODE_10BASE_T_FD_NOAUTONEG                     =  0x1,
	PHYCTRL_MODE_100BASE_TXFX_HD_NOAUTONEG_CRSTXRX         =  0x2,
	PHYCTRL_MODE_100BASE_TXFX_FD_NOAUTONEG_CRSRX           =  0x3,
	PHYCTRL_MODE_100BASE_TX_HD_ADV_AUTONEG_CRSRXTX         =  0x4,
	PHYCTRL_MODE_REPEATER_AUTONEG_100BASE_TX_HD_ADV_CRSRX  =  0x5,
	PHYCTRL_MODE_POWER_DOWN_MODE                           =  0x6,
	PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN            =  0x7,
	PHYCTRL_MODE_SPECIAL_MODE_8                            =  0x8,
	PHYCTRL_MODE_SPECIAL_MODE_9                            =  0x9,
	PHYCTRL_MODE_SPECIAL_MODE_10                           =  0xa,
	PHYCTRL_MODE_SPECIAL_MODE_11                           =  0xb,
	PHYCTRL_MODE_SPECIAL_MODE_12                           =  0xc,
	PHYCTRL_MODE_SPECIAL_MODE_13                           =  0xd,
	PHYCTRL_MODE_SPECIAL_MODE_14                           =  0xe,
	PHYCTRL_MODE_LOOPBACK_ISOLATE                          =  0xf
} PHYCTRL_MODE_E;


void pfifo_reset(void);
int drv_eth_xc_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);
int drv_eth_xc_initialize_lvds(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort);

void setup_phy_internal(void);
void setup_phy_external(void);
void setup_phy_lvds(void);


#endif  /* __DRV_ETH_XC_H__ */

