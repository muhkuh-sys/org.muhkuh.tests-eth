/***************************************************************************
 *   Copyright (C) 2016 by Hilscher GmbH                                   *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "driver/drv_eth_xc.h"
#include "netx_io_areas.h"
#include "options.h"
#include "pad_control.h"


const ROMLOADER_OPTIONS_T t_default_options =
{
	.t_ethernet =
	{
		.aucMac = { 0x00U, 0x02U, 0xa2U, 0x20U, 0x20U, 0x00U },
		.ulIp = 0U,
		.ulGatewayIp = 0U,
		.ulNetmask = 0U,
		.ulDnsIp = 0U,
		.ulPhyControl = (PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << HOSTSRT(int_phy_cfg_phy_ctrl_phy0_mode)) |
		                (PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << HOSTSRT(int_phy_cfg_phy_ctrl_phy1_mode)) |
		                HOSTMSK(int_phy_cfg_phy_ctrl_phy0_automdix) |
		                HOSTMSK(int_phy_cfg_phy_ctrl_phy1_automdix) |
		                HOSTMSK(int_phy_cfg_phy_ctrl_phy0_enable) |
		                HOSTMSK(int_phy_cfg_phy_ctrl_phy1_enable),
		.usLinkUpDelay = 1000,
		.usArpTimeout = 1000,
		.usDhcpTimeout = 3000,
		.usDnsTimeout = 3000,
		.ucArpRetries = 10,
		.ucDhcpRetries = 5,
		.ucDnsRetries = 5,
		.aucMmioCfg =
		{
			0xff,   /* Ethernet link LED. */
			0xff    /* Ethernet activity LED. */
		},
		.aucPadCtrlExtCommon =
		{
			PAD_DEFAULT(mii_mdc),                                                               /* pad_ctrl_mii_mdc */
			PAD_DEFAULT(mii_mdio),                                                              /* pad_ctrl_mii_mdio */
			PAD_DEFAULT(rst_out_n),                                                             /* pad_ctrl_rst_out_n */
			PAD_DEFAULT(clk25out)                                                               /* pad_ctrl_clk25out */
		},
		.aucPadCtrlExt0 =
		{
			PAD_DEFAULT(mii0_rxclk),                                                            /* pad_ctrl_mii0_rxclk */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_Low,  PAD_PULL_Enable,  PAD_INPUT_Enable),   /* pad_ctrl_mii0_rxd0 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_Low,  PAD_PULL_Enable,  PAD_INPUT_Enable),   /* pad_ctrl_mii0_rxd1 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_Low,  PAD_PULL_Enable,  PAD_INPUT_Enable),   /* pad_ctrl_mii0_rxd2 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_Low,  PAD_PULL_Enable,  PAD_INPUT_Enable),   /* pad_ctrl_mii0_rxd3 */
			PAD_DEFAULT(mii0_rxdv),                                                             /* pad_ctrl_mii0_rxdv */
			PAD_DEFAULT(mii0_rxer),                                                             /* pad_ctrl_mii0_rxer */
			PAD_DEFAULT(mii0_txclk),                                                            /* pad_ctrl_mii0_txclk */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii0_txd0 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii0_txd1 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii0_txd2 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii0_txd3 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii0_txen */
			PAD_DEFAULT(mii0_col),                                                              /* pad_ctrl_mii0_col */
			PAD_DEFAULT(mii0_crs),                                                              /* pad_ctrl_mii0_crs */
			PAD_DEFAULT(phy0_led_link_in)                                                       /* pad_ctrl_phy0_led_link_in */
		},
		.aucPadCtrlExt1 =
		{
			PAD_DEFAULT(mii1_rxclk),                                                            /* pad_ctrl_mii1_rxclk */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_Low,  PAD_PULL_Enable,  PAD_INPUT_Enable),   /* pad_ctrl_mii1_rxd0 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_Low,  PAD_PULL_Enable,  PAD_INPUT_Enable),   /* pad_ctrl_mii1_rxd1 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_Low,  PAD_PULL_Enable,  PAD_INPUT_Enable),   /* pad_ctrl_mii1_rxd2 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_Low,  PAD_PULL_Enable,  PAD_INPUT_Enable),   /* pad_ctrl_mii1_rxd3 */
			PAD_DEFAULT(mii1_rxdv),                                                             /* pad_ctrl_mii1_rxdv */
			PAD_DEFAULT(mii1_rxer),                                                             /* pad_ctrl_mii1_rxer */
			PAD_DEFAULT(mii1_txclk),                                                            /* pad_ctrl_mii1_txclk */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii1_txd0 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii1_txd1 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii1_txd2 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii1_txd3 */
			PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),  /* pad_ctrl_mii1_txen */
			PAD_DEFAULT(mii1_col),                                                              /* pad_ctrl_mii1_col */
			PAD_DEFAULT(mii1_crs),                                                              /* pad_ctrl_mii1_crs */
			PAD_DEFAULT(phy1_led_link_in)                                                       /* pad_ctrl_phy1_led_link_in */
		}
	}
};

