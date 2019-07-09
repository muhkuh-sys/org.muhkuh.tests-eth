

#include "nec_cb12.h"

#include "miimu.h"
#include "netx_io_areas.h"


void IntDualPhy_Initialize(void)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;
	unsigned int ulDelayCnt;


	/* reset phy configuration */
	ulValue  = INT_DUALPHY1_ADDR << HOSTSRT(phy_control_phy_address);

	ulValue |= PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << HOSTSRT(phy_control_phy0_mode);
	ulValue |= HOSTMSK(phy_control_phy0_automdix);
	ulValue |= HOSTMSK(phy_control_phy0_enable);

	ulValue |= PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << HOSTSRT(phy_control_phy1_mode);
	ulValue |= HOSTMSK(phy_control_phy1_automdix);
	ulValue |= HOSTMSK(phy_control_phy1_enable);

	/* read access key, write back access key */
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;
	/* write value with reset bit = 1 */
	ptAsicCtrlArea->ulPhy_control = ulValue | HOSTMSK(phy_control_phy_reset);

	/* delay for 100us -> do 5 mii transfers of 24us each */
	ulDelayCnt = 5;
	do
	{
		miimu_dummy_read(0);
	} while( ulDelayCnt--!=0 );

	/* read access key, write back access key */
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;
	/* write value with reset bit = 0 */
	ptAsicCtrlArea->ulPhy_control = ulValue;

	/* delay for 20us -> do 1 mii transfers of 24us */
	miimu_dummy_read(0);
}


