#include "hal_muhkuh.h"

#include <string.h>

#include "netx_io_areas.h"
#include "options.h"
#include "uprintf.h"

#if CFG_USE_ETHMAC==1
#       include "hal_ethmac_wrapper.h"
#elif CFG_USE_ETH2PS==1
#       include "hal_eth2ps_wrapper.h"
#endif


#if CFG_USE_ETHMAC==0

int hal_muhkuh_ethmac_prepare(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort __attribute__((unused)))
{
	uprintf("ERROR: ETHMAC is not available in this build.\n");
	return -1;
}



int hal_muhkuh_ethmac_initialize(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort __attribute__((unused)))
{
	uprintf("ERROR: ETHMAC is not available in this build.\n");
	return -1;
}



int hal_muhkuh_ethmac_disable(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort __attribute__((unused)))
{
	uprintf("ERROR: ETHMAC is not available in this build.\n");
	return -1;
}

#else

int hal_muhkuh_ethmac_prepare(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort)
{
	HOSTDEF(ptAsicCtrlArea);
	HOSTDEF(ptSystimeComArea);
	int iResult;
	unsigned long ulMask;
	unsigned long ulEnable;
	unsigned long ulValue;
	unsigned int uiXcPort;


	/* Be pessimistic. */
	iResult = -1;

	/* Set the systime. */
	ptSystimeComArea->ulSystime_border = 1000000000U - 1U;
	ptSystimeComArea->ulSystime_count_value = 0xa0000000U;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		uiXcPort = uiPort & 1U;

		/* Check if all necessary clocks can be enabled. */
		ulMask = HOSTMSK(clock_enable0_mask_xc_misc);
		ulEnable  = HOSTMSK(clock_enable0_xc_misc);
#if ASIC_TYP==ASIC_TYP_NETX90
		ulEnable |= HOSTMSK(clock_enable0_xc_misc_wm);
#endif
		if( uiXcPort==0U )
		{
			ulMask |= HOSTMSK(clock_enable0_mask_xmac0);
			ulMask |= HOSTMSK(clock_enable0_mask_tpec0);
			ulMask |= HOSTMSK(clock_enable0_mask_rpec0);

			ulEnable |= HOSTMSK(clock_enable0_xmac0);
			ulEnable |= HOSTMSK(clock_enable0_tpec0);
			ulEnable |= HOSTMSK(clock_enable0_rpec0);
#if ASIC_TYP==ASIC_TYP_NETX90
			ulEnable |= HOSTMSK(clock_enable0_xmac0_wm);
			ulEnable |= HOSTMSK(clock_enable0_tpec0_wm);
			ulEnable |= HOSTMSK(clock_enable0_rpec0_wm);
#endif
		}
		else
		{
			ulMask |= HOSTMSK(clock_enable0_mask_xmac1);
			ulMask |= HOSTMSK(clock_enable0_mask_tpec1);
			ulMask |= HOSTMSK(clock_enable0_mask_rpec1);

			ulEnable |= HOSTMSK(clock_enable0_xmac1);
			ulEnable |= HOSTMSK(clock_enable0_tpec1);
			ulEnable |= HOSTMSK(clock_enable0_rpec1);
#if ASIC_TYP==ASIC_TYP_NETX90
			ulEnable |= HOSTMSK(clock_enable0_xmac1_wm);
			ulEnable |= HOSTMSK(clock_enable0_tpec1_wm);
			ulEnable |= HOSTMSK(clock_enable0_rpec1_wm);
#endif
		}
		ulValue  = ptAsicCtrlArea->asClock_enable[0].ulMask;
		ulValue &= ulMask;
		ulValue ^= ulMask;
		if( ulValue!=0U )
		{
			uprintf("The Ethernet clocks are masked out.\n");
		}
		else
		{
			/* Enable the clocks. */
			ulValue  = ptAsicCtrlArea->asClock_enable[0].ulEnable;
			ulValue |= ulEnable;
			ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
			ptAsicCtrlArea->asClock_enable[0].ulEnable = ulValue;

			iResult = 0;
		}
	}

	return iResult;
}



static int ethmac_get_link_status(NETWORK_DRIVER_T *ptNetworkDriver, LINK_STATE_T *ptLinkState)
{
	unsigned int uiPort;
	int iResult;
	unsigned int uiLinkState;
	LINK_STATE_T tLinkState;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;

	tLinkState = LINK_STATE_Unknown;
	iResult = hal_ethmac_get_link_state(uiPort, &uiLinkState, NULL, NULL);
	if( iResult==0 )
	{
		if( uiLinkState==0 )
		{
			tLinkState = LINK_STATE_Down;
		}
		else
		{
			tLinkState = LINK_STATE_Up;
		}
	}

	if( ptLinkState!=NULL )
	{
		*ptLinkState = tLinkState;
	}

	return iResult;
}



static int ethmac_get_empty_packet(NETWORK_DRIVER_T *ptNetworkDriver, void **ppvPacket, void **pphPacket)
{
	unsigned int uiPort;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;
	return hal_ethmac_get_empty_packet(uiPort, ppvPacket, pphPacket);
}



static void ethmac_release_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, void *phPacket)
{
	unsigned int uiPort;
	int iResult;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;
	iResult = hal_ethmac_release_packet(uiPort, pvPacket, phPacket);
	if( iResult!=0 )
	{
		uprintf("WARNING: failed to release packet %08x/%08x\n", (unsigned long)pvPacket, (unsigned long)phPacket);
	}
}



static void ethmac_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, void *phPacket, unsigned int uiPacketSize)
{
	unsigned int uiPort;
	int iResult;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;
	iResult = hal_ethmac_send_packet(uiPort, pvPacket, phPacket, uiPacketSize);
	if( iResult!=0 )
	{
		uprintf("WARNING: failed to send packet %08x/%08x\n", (unsigned long)pvPacket, (unsigned long)phPacket);
	}
}



static int ethmac_get_received_packet(NETWORK_DRIVER_T *ptNetworkDriver, void **ppvPacket, void **pphPacket, unsigned int *puiPacketSize)
{
	unsigned int uiPort;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;
	return hal_ethmac_get_received_packet(uiPort, ppvPacket, pphPacket, puiPacketSize);
}



static void ethmac_deactivate(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)))
{
	/* TODO: deactivate all. */
}



static const NETWORK_IF_T tNetworkIfEthMac =
{
	.pfnGetLinkStatus = ethmac_get_link_status,
	.pfnGetEmptyPacket = ethmac_get_empty_packet,
	.pfnReleasePacket = ethmac_release_packet,
	.pfnSendPacket = ethmac_send_packet,
	.pfnGetReceivedPacket = ethmac_get_received_packet,
	.pfnDeactivate = ethmac_deactivate
};



int hal_muhkuh_ethmac_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort)
{
	int iResult;
//	DRV_ETH_XC_HANDLE_T *ptHandle;
//	unsigned int uiXcPort;
	const uint8_t *pucMAC;


	/* Be pessimistic. */
	iResult = -1;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		/* Get a shortcut to the handle. */
//		ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);

//		uiXcPort = uiPort & 1;

		/* Initialize the internal handle. */
//		ptHandle->uiEthPortNr = uiPort;
//		ptHandle->uiXcPort = uiXcPort;
//		ptHandle->auiExtPhyCtrlInst[0] = 0;
//		ptHandle->auiExtPhyCtrlInst[1] = 0;
//		ptHandle->auiExtPhyAddress[0] = 0;
//		ptHandle->auiExtPhyAddress[1] = 0;

		ptNetworkDriver->uiPort = uiPort;
		pucMAC = ptNetworkDriver->tEthernetPortCfg.aucMac;
		iResult = hal_ethmac_xc_init(uiPort, pucMAC);

		memcpy(&(ptNetworkDriver->tNetworkIf), &tNetworkIfEthMac, sizeof(NETWORK_IF_T));
	}

	return iResult;
}



int hal_muhkuh_ethmac_disable(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort)
{
	int iResult;
	unsigned int uiXcPort;


	/* Be pessimistic. */
	iResult = -1;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		uiXcPort = uiPort & 1U;

		/* reset the hardware block */
		iResult = hal_ethmac_xc_reset(uiXcPort);
	}

	return iResult;
}

#endif







#if CFG_USE_ETH2PS==0

int hal_muhkuh_eth2ps_prepare(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)))
{
	uprintf("ERROR: ETH2PS is not available in this build.\n");
	return -1;
}



int hal_muhkuh_eth2ps_initialize(NETWORK_DRIVER_T *ptNetworkDriver0 __attribute__((unused)), NETWORK_DRIVER_T *ptNetworkDriver1 __attribute__((unused)))
{
	uprintf("ERROR: ETH2PS is not available in this build.\n");
	return -1;
}



int hal_muhkuh_eth2ps_disable(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)))
{
	uprintf("ERROR: ETH2PS is not available in this build.\n");
	return -1;
}

#else

int hal_muhkuh_eth2ps_prepare(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)))
{
	HOSTDEF(ptAsicCtrlArea);
	HOSTDEF(ptSystimeComArea);
	int iResult;
	unsigned long ulMask;
	unsigned long ulEnable;
	unsigned long ulValue;


	/* Be pessimistic. */
	iResult = -1;

	/* Set the systime. */
	ptSystimeComArea->ulSystime_border = 1000000000U - 1U;
	ptSystimeComArea->ulSystime_count_value = 0xa0000000U;

	/* Check if all necessary clocks can be enabled. */
	ulMask = HOSTMSK(clock_enable0_mask_xc_misc);
	ulEnable  = HOSTMSK(clock_enable0_xc_misc);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulEnable |= HOSTMSK(clock_enable0_xc_misc_wm);
#endif

	ulMask |= HOSTMSK(clock_enable0_mask_xmac0);
	ulMask |= HOSTMSK(clock_enable0_mask_tpec0);
	ulMask |= HOSTMSK(clock_enable0_mask_rpec0);
	ulMask |= HOSTMSK(clock_enable0_mask_xmac1);
	ulMask |= HOSTMSK(clock_enable0_mask_tpec1);
	ulMask |= HOSTMSK(clock_enable0_mask_rpec1);

	ulEnable |= HOSTMSK(clock_enable0_xmac0);
	ulEnable |= HOSTMSK(clock_enable0_tpec0);
	ulEnable |= HOSTMSK(clock_enable0_rpec0);
	ulEnable |= HOSTMSK(clock_enable0_xmac1);
	ulEnable |= HOSTMSK(clock_enable0_tpec1);
	ulEnable |= HOSTMSK(clock_enable0_rpec1);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulEnable |= HOSTMSK(clock_enable0_xmac0_wm);
	ulEnable |= HOSTMSK(clock_enable0_tpec0_wm);
	ulEnable |= HOSTMSK(clock_enable0_rpec0_wm);
	ulEnable |= HOSTMSK(clock_enable0_xmac1_wm);
	ulEnable |= HOSTMSK(clock_enable0_tpec1_wm);
	ulEnable |= HOSTMSK(clock_enable0_rpec1_wm);
#endif

	ulValue  = ptAsicCtrlArea->asClock_enable[0].ulMask;
	ulValue &= ulMask;
	ulValue ^= ulMask;
	if( ulValue!=0U )
	{
		uprintf("The Ethernet clocks are masked out.\n");
	}
	else
	{
		/* Enable the clocks. */
		ulValue  = ptAsicCtrlArea->asClock_enable[0].ulEnable;
		ulValue |= ulEnable;
		ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
		ptAsicCtrlArea->asClock_enable[0].ulEnable = ulValue;

		/*---------------------------------------------------------
		 * FIXME: This part is board-specific.
		 *        Maybe this can become an option file someday?
		 *        Would be also a nice way to enable the LINK and ACT LEDs.
		 */

		hal_ethps2_prepare();

		/* End of board-specific part.
		 *---------------------------------------------------------
		 */

		iResult = 0;
	}

	return iResult;
}



static int eth2ps_get_link_status(NETWORK_DRIVER_T *ptNetworkDriver, LINK_STATE_T *ptLinkState)
{
	unsigned int uiPort;
	int iResult;
	unsigned int uiLinkState;
	LINK_STATE_T tLinkState;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;

	tLinkState = LINK_STATE_Unknown;
	iResult = hal_eth2ps_get_link_state(uiPort, &uiLinkState, NULL, NULL);
	if( iResult==0 )
	{
		if( uiLinkState==0 )
		{
			tLinkState = LINK_STATE_Down;
		}
		else
		{
			tLinkState = LINK_STATE_Up;
		}
	}

	if( ptLinkState!=NULL )
	{
		*ptLinkState = tLinkState;
	}

	return iResult;
}



static int eth2ps_get_empty_packet(NETWORK_DRIVER_T *ptNetworkDriver, void **ppvPacket, void **pphPacket)
{
	unsigned int uiPort;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;
	return hal_eth2ps_get_empty_packet(uiPort, ppvPacket, pphPacket);
}



static void eth2ps_release_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, void *phPacket)
{
	unsigned int uiPort;
	int iResult;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;
	iResult = hal_eth2ps_release_packet(uiPort, pvPacket, phPacket);
	if( iResult!=0 )
	{
		uprintf("WARNING: failed to release packet %08x/%08x\n", (unsigned long)pvPacket, (unsigned long)phPacket);
	}
}



static void eth2ps_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, void *phPacket, unsigned int uiPacketSize)
{
	unsigned int uiPort;
	int iResult;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;
	iResult = hal_eth2ps_send_packet(uiPort, pvPacket, phPacket, uiPacketSize);
	if( iResult!=0 )
	{
		uprintf("WARNING: failed to send packet %08x/%08x\n", (unsigned long)pvPacket, (unsigned long)phPacket);
	}
}



static int eth2ps_get_received_packet(NETWORK_DRIVER_T *ptNetworkDriver, void **ppvPacket, void **pphPacket, unsigned int *puiPacketSize)
{
	unsigned int uiPort;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->uiPort;
	return hal_eth2ps_get_received_packet(uiPort, ppvPacket, pphPacket, puiPacketSize);
}



static void eth2ps_deactivate(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)))
{
	/* TODO: deactivate all. */
}



static const NETWORK_IF_T tNetworkIfEth2ps =
{
	.pfnGetLinkStatus = eth2ps_get_link_status,
	.pfnGetEmptyPacket = eth2ps_get_empty_packet,
	.pfnReleasePacket = eth2ps_release_packet,
	.pfnSendPacket = eth2ps_send_packet,
	.pfnGetReceivedPacket = eth2ps_get_received_packet,
	.pfnDeactivate = eth2ps_deactivate
};



int hal_muhkuh_eth2ps_initialize(NETWORK_DRIVER_T *ptNetworkDriver0, NETWORK_DRIVER_T *ptNetworkDriver1)
{
	int iResult;
	const uint8_t *pucMAC0;
	const uint8_t *pucMAC1;


	ptNetworkDriver0->uiPort = 0;
	ptNetworkDriver1->uiPort = 1;

	pucMAC0 = ptNetworkDriver0->tEthernetPortCfg.aucMac;
	pucMAC1 = ptNetworkDriver1->tEthernetPortCfg.aucMac;
	iResult = hal_eth2ps_init(pucMAC0, pucMAC1);
	if( iResult==0 )
	{
		memcpy(&(ptNetworkDriver0->tNetworkIf), &tNetworkIfEth2ps, sizeof(NETWORK_IF_T));
		memcpy(&(ptNetworkDriver1->tNetworkIf), &tNetworkIfEth2ps, sizeof(NETWORK_IF_T));
	}

	return iResult;
}



int hal_muhkuh_eth2ps_disable(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)))
{
	/* FIXME: add a reset here. */

	return 0;
}

#endif
