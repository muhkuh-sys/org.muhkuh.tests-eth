
#include "interface.h"

#include <string.h>

#include "netx_io_areas.h"
#include "options.h"
#include "uprintf.h"
#include "nec_cb12.h"
#include "xc.h"
#include "miimu.h"
#include "nec_cb12.h"


extern unsigned char aucEthernetBuffer0_start[];
extern unsigned char aucEthernetBuffer0_end[];
extern unsigned char aucEthernetBuffer1_start[];
extern unsigned char aucEthernetBuffer1_end[];


static void interface_configure(NETWORK_DRIVER_T *ptNetworkDriver)
{
	unsigned int uiXcPort;
	unsigned long ulData;
	unsigned long ulDebug;
	unsigned int uiPhyAddress;
	unsigned char *pucMac;
	unsigned char *pucFrameBufferStart;
	unsigned char *pucFrameBufferEnd;


	uiXcPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort;

	/* Get the PHY address. */
	if( uiXcPort==0 )
	{
		uiPhyAddress = 0;
		pucFrameBufferStart = aucEthernetBuffer0_start;
		pucFrameBufferEnd = aucEthernetBuffer0_end;
	}
	else
	{
		uiPhyAddress = 1;
		pucFrameBufferStart = aucEthernetBuffer1_start;
		pucFrameBufferEnd = aucEthernetBuffer1_end;
	}
	uprintf("uiXcPort:%d uiPhyAddress:%d\n", uiXcPort, uiPhyAddress);

	/* DEBUG: Read the PHY ID. */
	ulDebug = Miimu_ReadPhy(uiPhyAddress, DRV_CB12_PHY_ID_1);
	uprintf("[ETH] PHY_REG_ID_1: 0x%04x\n", ulDebug);
	ulDebug = Miimu_ReadPhy(uiPhyAddress, DRV_CB12_PHY_ID_2);
	uprintf("[ETH] PHY_REG_ID_2: 0x%04x\n", ulDebug);

	/* Set the control register. */
	ulData = DFLT_PHY_CTRL;
	Miimu_WritePhy(uiPhyAddress, DRV_CB12_CONTROL, ulData);

	/* Set the auto negotiation features. */
	ulData = DFLT_PHY_ANADV;
	Miimu_WritePhy(uiPhyAddress, DRV_CB12_AUTO_NEG_ADVERTISEMENT, ulData);

	ulDebug = Miimu_ReadPhy(uiPhyAddress, DRV_CB12_CONTROL);
	uprintf("[ETH] PHY_REG_CONTROL: 0x%04x\n", ulDebug);
	ulDebug = Miimu_ReadPhy(uiPhyAddress, DRV_CB12_AUTO_NEG_ADVERTISEMENT);
	uprintf("[ETH] DRV_CB12_AUTO_NEG_ADVERTISEMENT: 0x%04x\n", ulDebug);

	/*
	 * Reset the XC unit.
	 */
	pucMac = ptNetworkDriver->tEthernetPortCfg.aucMac;
	uprintf("[ETH] using MAC %02x:%02x:%02x:%02x:%02x:%02x\n", pucMac[0], pucMac[1], pucMac[2], pucMac[3], pucMac[4], pucMac[5]);
	xc_init(uiXcPort, pucMac, pucFrameBufferStart, pucFrameBufferEnd);
}



static void *interface_get_empty_packet(NETWORK_DRIVER_T *ptNetworkDriver)
{
	unsigned int uiXcPort;
	void *pvEmptyPacket;


	/* Get a free frame for sending. */
	uiXcPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort;
	pvEmptyPacket = xc_get_empty_frame(uiXcPort);
	if( pvEmptyPacket==NULL )
	{
		uprintf("[ETH] failed to get empty frame!\n");
	}
//	uprintf("Empty packet: 0x%08x\n", (unsigned long)pvEmptyPacket);

	return pvEmptyPacket;
}



static void interface_release_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket)
{
	unsigned int uiXcPort;


	uiXcPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort;
	xc_release_frame(uiXcPort, pvPacket);
}



static void *interface_receive_packet(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int *psizPacket)
{
	unsigned int uiXcPort;
	void *pvPacket;


	uiXcPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort;
	pvPacket = xc_get_rx_frame(uiXcPort, psizPacket);

//	uprintf("Rec packet: 0x%08x\n", (unsigned long)pvPacket);
	return pvPacket;
}



static void interface_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, unsigned int sizPacket)
{
	unsigned int uiXcPort;


	uiXcPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort;
	xc_send_frame(uiXcPort, pvPacket, sizPacket);
}



static unsigned int interface_get_link_status(NETWORK_DRIVER_T *ptNetworkDriver)
{
	unsigned int uiXcPort;
	unsigned int uiPhyAddress;
	unsigned long ulData;
	unsigned int uiLinkStatus;


	uiXcPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort;

	/* Get the PHY address. */
	if( uiXcPort==0 )
	{
		uiPhyAddress = 0;
	}
	else
	{
		uiPhyAddress = 1;
	}

	ulData = Miimu_ReadPhy(uiPhyAddress, DRV_CB12_STATUS);
	ulData &= DRV_CB12_STATUS_LINK_UP;
	uiLinkStatus = 0U;
	if( ulData!=0U )
	{
		uiLinkStatus = 1U;
	}

	return uiLinkStatus;
}



static void interface_uninitialize(NETWORK_DRIVER_T *ptNetworkDriver)
{
	unsigned int uiXcPort;
	unsigned int uiPhyAddress;
	unsigned long ulData;


	uiXcPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort;

	/* Get the PHY address. */
	if( uiXcPort==0 )
	{
		uiPhyAddress = 0;
	}
	else
	{
		uiPhyAddress = 1;
	}

	/* De-activate ethernet */
	xc_deinit(uiXcPort);

	/*
	 * deactivate PHY
	 */

	/* set power down bit in control register */
	ulData  = DFLT_PHY_CTRL;
	ulData |= DRV_CB12_CONTROL_POWER_DOWN;
	Miimu_WritePhy(uiPhyAddress, DRV_CB12_CONTROL, ulData);
}



void pfifo_reset(void)
{
	xc_init_fifo_borders();
}



static const NETWORK_IF_T tNetworkIfXc =
{
	.pfnGetLinkStatus = interface_get_link_status,
	.pfnGetEmptyPacket = interface_get_empty_packet,
	.pfnReleasePacket = interface_release_packet,
	.pfnSendPacket = interface_send_packet,
	.pfnGetReceivedPacket = interface_receive_packet,
	.pfnDeactivate = interface_uninitialize,
};



int drv_eth_xc_prepare(NETWORK_DRIVER_T *ptNetworkDriver __attribute((unused)), unsigned int uiPort)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;
	int iResult;


	/* Expect failure. */
	iResult = 1;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		ulValue  = HOSTMSK(clock_enable_xpec0) | HOSTMSK(clock_enable_xmac0);
		ulValue |= HOSTMSK(clock_enable_xpec1) | HOSTMSK(clock_enable_xmac1);
		ulValue |= HOSTMSK(clock_enable_xpec2) | HOSTMSK(clock_enable_xmac2);
		ulValue |= HOSTMSK(clock_enable_xpec3) | HOSTMSK(clock_enable_xmac3);
		if( (ptAsicCtrlArea->ulClock_enable_mask&ulValue)!=ulValue )
		{
			uprintf("[main] No ethernet boot possible. The XC clocks are masked to 0.\n");
		}
		else
		{
			/*
			 * Enable the clocks.
			 */
			ulValue  = ptAsicCtrlArea->ulClock_enable;
			ulValue |= HOSTMSK(clock_enable_xpec0) | HOSTMSK(clock_enable_xmac0);
			ulValue |= HOSTMSK(clock_enable_xpec1) | HOSTMSK(clock_enable_xmac1);
			ulValue |= HOSTMSK(clock_enable_xpec2) | HOSTMSK(clock_enable_xmac2);
			ulValue |= HOSTMSK(clock_enable_xpec3) | HOSTMSK(clock_enable_xmac3);

			/* ulValue holds the bits which must be enabled. Add the already enabled clocks from the register. */
			ulValue |= ptAsicCtrlArea->ulClock_enable;
			/* Unlock the asic ctrl access key for the next operation. */
			ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;
			/* NOTE: Do not just add the new bits here with a '|=' operator. This will
			 * not work as the unlocked asic_ctrl_access_key only works for the next
			 * operation - whether it is a read or write. '|=' reads the register first,
			 * and then writes it back. This would waste the unlocked state for the read
			 * operation.
			 */
			ptAsicCtrlArea->ulClock_enable = ulValue;

			iResult = 0;
		}
	}

	return iResult;
}



int drv_eth_xc_disable(NETWORK_DRIVER_T *ptNetworkDriver __attribute((unused)), unsigned int uiPort)
{
	int iResult;


	/* Expect failure. */
	iResult = 1;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		xc_reset(uiPort);
		iResult = 0;
	}

	return iResult;
}



int drv_eth_xc_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort)
{
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned int uiXcPort;
	int iResult;


	/* Expect failure. */
	iResult = 1;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		/* Get a shortcut to the handle. */
		ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);

		uiXcPort = uiPort;

		/* Initialize the internal handle. */
		ptHandle->uiEthPortNr = uiPort;
		ptHandle->uiXcPort = uiXcPort;
		ptHandle->auiExtPhyCtrlInst[0] = 0;
		ptHandle->auiExtPhyCtrlInst[1] = 0;
		ptHandle->auiExtPhyAddress[0] = 0;
		ptHandle->auiExtPhyAddress[1] = 0;

		/* Initialize the FIFO unit. */
		/* Do not initialize it here. This routine will be called
		 * once for each port, so the FIFO will be reset
		 * 2 times and all frames already inserted are lost.
		 */
/*		xc_init_fifo_borders(); */

		IntDualPhy_Initialize();

		interface_configure(ptNetworkDriver);

		memcpy(&(ptNetworkDriver->tNetworkIf), &tNetworkIfXc, sizeof(NETWORK_IF_T));

		iResult = 0;
	}

	return iResult;
}



int drv_eth_xc_prepare_lvds(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort __attribute__((unused)))
{
	/* netX500 has no LVDS. */
	return -1;
}



int drv_eth_xc_disable_lvds(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort __attribute__((unused)))
{
	/* netX500 has no LVDS. */
	return -1;
}



int drv_eth_xc_initialize_lvds(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort __attribute__((unused)))
{
	/* netX500 has no LVDS. */
	return -1;
}
