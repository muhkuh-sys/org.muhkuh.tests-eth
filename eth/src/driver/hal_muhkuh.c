#include "hal_muhkuh.h"

#include <string.h>

#include "hal_ethmac_wrapper.h"
#include "netx_io_areas.h"
#include "options.h"
#include "uprintf.h"
#include "netx90/phy.h"
#include "netx90/phy_std_regs.h"



void hal_phy_init(void)
{
	hal_ethmac_phy_init();
}



static int intphy_reset(void)
{
	int iResult;
	const unsigned char *pucMacro;


	/* Get a pointer to the macro data. */
	pucMacro = g_t_romloader_options.t_ethernet.tPhyMacroIntern.aucMacro;

	/* There is no default for the initialization of the internal PHY.
	 * An empty macro is an error.
	 */
	if( *pucMacro==0x00U )
	{
		iResult = -1;
	}
	else
	{
		iResult = phy_setup_execute_sequence(pucMacro, sizeof(PHY_MACRO_T));
	}

	return iResult;
}



static int extphy_reset(void)
{
	const unsigned char *pucMacro;
	int iResult;


	/* Get a pointer to the macro data. */
	pucMacro = g_t_romloader_options.t_ethernet.tPhyMacroExtern.aucMacro;

	/* Use the standard setup if the macro is empty.
	 * A macro starting with a "0" is empty.
	 */
	if( *pucMacro==0x00U )
	{
		iResult = -1;
	}
	else
	{
		iResult = phy_setup_execute_sequence(pucMacro, sizeof(PHY_MACRO_T));
	}

	return iResult;
}



void hal_pfifo_reset(void)
{
	hal_ethmac_pfifo_reset();
}



void setup_phy_internal(void)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;
	unsigned long ulXmMiiCfg;
	unsigned long ulXcMdioCfg;
	unsigned long ulPhyCtrl;


	/* Connect to internal PHY. */
	ulXmMiiCfg = 8U;
	ulXcMdioCfg = 2U;
//	ulPhyCtrl = HOSTDFLT(phy_ctrl0);
	ulPhyCtrl  = HOSTMSK(phy_ctrl0_phy0_led_invert);
	ulPhyCtrl |= HOSTMSK(phy_ctrl0_phy1_led_invert);

	ulValue  = 0U << HOSTSRT(io_config0_sel_xm0_tx);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_tx_wm);
#endif
	ulValue  = 0U << HOSTSRT(io_config0_sel_xm0_txoe);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_txoe_wm);
#endif
	ulValue  = 0U << HOSTSRT(io_config0_sel_xm0_eclk);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_eclk_wm);
#endif
	ulValue  = 0U << HOSTSRT(io_config0_sel_xm0_io);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_io_wm);
#endif
	ulValue  = 0U << HOSTSRT(io_config0_sel_fb0clk);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_fb0clk_wm);
#endif
	/* Disable LVDS connections. */
	ulValue |= ulXmMiiCfg << HOSTSRT(io_config0_sel_xm0_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config0_sel_xc0_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xc0_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[0].ulConfig = ulValue;

	ulValue  = 0U << HOSTSRT(io_config1_sel_xm1_tx);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_tx_wm);
#endif
	ulValue  = 0U << HOSTSRT(io_config1_sel_xm1_txoe);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_txoe_wm);
#endif
	ulValue  = 0U << HOSTSRT(io_config1_sel_xm1_eclk);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_eclk_wm);
#endif
	ulValue  = 0U << HOSTSRT(io_config1_sel_xm1_io);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_io_wm);
#endif
	ulValue  = 0U << HOSTSRT(io_config1_sel_fb1clk);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_fb1clk_wm);
#endif
	ulValue |= ulXmMiiCfg << HOSTSRT(io_config1_sel_xm1_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_mii_cfg_wm);
#endif
	ulValue |= 0U << HOSTSRT(io_config1_sel_xc1_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xc1_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[1].ulConfig = ulValue;

	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulPhy_ctrl0 = ulPhyCtrl;

	intphy_reset();
}



void setup_phy_external(void)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;
	unsigned long ulXmMiiCfg;
	unsigned long ulXcMdioCfg;
	unsigned long ulPhyCtrl;


	/* Connect to external PHY. */
	ulXmMiiCfg = 6U;
	ulXcMdioCfg = 1U;
	ulPhyCtrl  = 4U << HOSTSRT(phy_ctrl0_phy0_led_invert);
	ulPhyCtrl |= 4U << HOSTSRT(phy_ctrl0_phy1_led_invert);

	/* Disable LVDS connections. */
	ulValue  = ulXmMiiCfg << HOSTSRT(io_config0_sel_xm0_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config0_sel_xc0_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xc0_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[0].ulConfig = ulValue;

	ulValue  = ulXmMiiCfg << HOSTSRT(io_config1_sel_xm1_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config1_sel_xc1_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xc1_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[1].ulConfig = ulValue;

	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulPhy_ctrl0 = ulPhyCtrl;

	ulValue  = ptAsicCtrlArea->asIo_config[2].ulConfig;
	ulValue |= HOSTMSK(io_config2_clk25out_oe);
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[2].ulConfig = ulValue;

	extphy_reset();
}



int hal_xc_prepare(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort)
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



/*-------------------------------------------------------------------------*/


static int ethmac_get_link_status(NETWORK_DRIVER_T *ptNetworkDriver, LINK_STATE_T *ptLinkState)
{
	unsigned int uiPort;
	int iResult;
	unsigned int uiLinkState;
	LINK_STATE_T tLinkState;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiEthPortNr;

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
	uiPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiEthPortNr;
	return hal_ethmac_get_empty_packet(uiPort, ppvPacket, pphPacket);
}



static void ethmac_release_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, void *phPacket)
{
	unsigned int uiPort;
	int iResult;


	/* Get the index of the Ethernet port. */
	uiPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiEthPortNr;
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
	uiPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiEthPortNr;
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
	uiPort = ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiEthPortNr;
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



/*-------------------------------------------------------------------------*/


int hal_xc_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort)
{
	int iResult;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned int uiXcPort;
	const uint8_t *pucMAC;


	/* Be pessimistic. */
	iResult = -1;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		/* Get a shortcut to the handle. */
		ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);

		uiXcPort = uiPort & 1;

		/* Initialize the internal handle. */
		ptHandle->uiEthPortNr = uiPort;
		ptHandle->uiXcPort = uiXcPort;
		ptHandle->auiExtPhyCtrlInst[0] = 0;
		ptHandle->auiExtPhyCtrlInst[1] = 0;
		ptHandle->auiExtPhyAddress[0] = 0;
		ptHandle->auiExtPhyAddress[1] = 0;

		pucMAC = ptNetworkDriver->tEthernetPortCfg.aucMac;
		iResult = hal_ethmac_xc_init(uiPort, pucMAC);

		memcpy(&(ptNetworkDriver->tNetworkIf), &tNetworkIfEthMac, sizeof(NETWORK_IF_T));
	}

	return iResult;
}



int hal_xc_disable(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)), unsigned int uiPort)
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
