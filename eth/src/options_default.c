/***************************************************************************
 *   Copyright (C) 2016 by Hilscher GmbH                                   *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "driver/drv_eth_xc.h"
#include "driver/phy.h"
#include "stack/ipv4.h"
#include "netx_io_areas.h"
#include "options.h"


const ROMLOADER_OPTIONS_T t_default_options =
{
	.t_ethernet =
	{
		.atPorts =
		{
			/* Port 0 */
			{
				.pcName = "CH0",
				.tInterface = INTERFACE_INTPHY0,
				.tFunction = INTERFACE_FUNCTION_EchoServer,
				.aucMac = { 0x00U, 0x02U, 0xa2U, 0x20U, 0x20U, 0x00U },
				.ulIp = IP_ADR(192,168,64,20),
				.ulGatewayIp = 0,
				.ulNetmask = IP_ADR(255,255,255,0),
				.usLinkUpDelay = 1000
			},
			/* Port 1 */
			{
				.pcName = "CH1",
				.tInterface = INTERFACE_INTPHY1,
				.tFunction = INTERFACE_FUNCTION_EchoClient,
				.aucMac = { 0x00U, 0x02U, 0xa2U, 0x20U, 0x20U, 0x01U },
				.ulIp = IP_ADR(192,168,64,21),
				.ulGatewayIp = 0,
				.ulNetmask = IP_ADR(255,255,255,0),
				.usLinkUpDelay = 1000
			}
		},
		.usArpTimeout = 1000,
		.usDhcpTimeout = 3000,
		.ucArpRetries = 10,
		.ucDhcpRetries = 5,
		.tPhyMacroIntern =
		{
			.aucMacro =
			{
				/******************************************************************
				 *
				 * Reset the INTPHY.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(2),

				/******************************************************************
				 *
				 * Set the configuration while reset is active.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 7U, 0U, 1U, 0U, 1U, 7U, 0U, 1U, 0U, 1U, 1U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(2),

				/******************************************************************
				 *
				 * Remove the reset.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 7U, 0U, 1U, 0U, 1U, 7U, 0U, 1U, 0U, 1U, 0U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(2),

				/******************************************************************
				 *
				 * Set PHY0 to the test mode.
				 *
				 ******************************************************************/
				PHY_SETUP_SYS_PHY_WRITE(0U, 20U, 0x0000U),
				PHY_SETUP_SYS_PHY_WRITE(0U, 20U, 0x0400U),
				PHY_SETUP_SYS_PHY_WRITE(0U, 20U, 0x0000U),
				PHY_SETUP_SYS_PHY_WRITE(0U, 20U, 0x0400U),

				/* sys_phy_write_bank(uiPhy, REG_BANK_BIST, 0x1c, 0x0000);
				 * sys_phy_write_bank(uiPhy, REG_BANK_BIST, 0x1d, 0x0280);
				 */
				PHY_SETUP_SYS_PHY_WRITE_BANK(0U, REG_BANK_BIST, 0x1cU, 0x0000U),
				PHY_SETUP_SYS_PHY_WRITE_BANK(0U, REG_BANK_BIST, 0x1dU, 0x0280U),

				PHY_SETUP_DELAY(2),

				/* sys_phy_write(uiPhy, 17, 0x0040);
				 * sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x02, 0xF07B);
				 * sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x03, 0x0004);
				 */
				PHY_SETUP_SYS_PHY_WRITE(0U, 17U, 0x0040U),
				PHY_SETUP_SYS_PHY_WRITE_BANK(0U, REG_BANK_VMDAC, 0x02U, 0xF07BU),
				PHY_SETUP_SYS_PHY_WRITE_BANK(0U, REG_BANK_VMDAC, 0x03U, 0x0004U),

				/* restart auto-neg
				 * sys_phy_write(uiPhy, 0, 0x1200);
				 */
				PHY_SETUP_SYS_PHY_WRITE(0U, 0U, 0x1200U),



				/* uiPhy = 1; */

				/* sys_phy_write(uiPhy, 20, 0x0000);
				 * sys_phy_write(uiPhy, 20, 0x0400);
				 * sys_phy_write(uiPhy, 20, 0x0000);
				 * sys_phy_write(uiPhy, 20, 0x0400);
				 */
				PHY_SETUP_SYS_PHY_WRITE(1U, 20U, 0x0000U),
				PHY_SETUP_SYS_PHY_WRITE(1U, 20U, 0x0400U),
				PHY_SETUP_SYS_PHY_WRITE(1U, 20U, 0x0000U),
				PHY_SETUP_SYS_PHY_WRITE(1U, 20U, 0x0400U),

				/* sys_phy_write_bank(uiPhy, REG_BANK_BIST, 0x1c, 0x0000);
				 * sys_phy_write_bank(uiPhy, REG_BANK_BIST, 0x1d, 0x0280);
				 */
				PHY_SETUP_SYS_PHY_WRITE_BANK(1U, REG_BANK_BIST, 0x1cU, 0x0000U),
				PHY_SETUP_SYS_PHY_WRITE_BANK(1U, REG_BANK_BIST, 0x1dU, 0x0280U),

				PHY_SETUP_DELAY(2),

				/* sys_phy_write(uiPhy, 17, 0x0040);
				 * sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x02, 0xF07B);
				 * sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x03, 0x0004);
				 */
				PHY_SETUP_SYS_PHY_WRITE(1U, 17U, 0x0040U),
				PHY_SETUP_SYS_PHY_WRITE_BANK(1U, REG_BANK_VMDAC, 0x02U, 0xF07BU),
				PHY_SETUP_SYS_PHY_WRITE_BANK(1U, REG_BANK_VMDAC, 0x03U, 0x0004U),

				/* restart auto-neg
				 * sys_phy_write(uiPhy, 0, 0x1200);
				 */
				PHY_SETUP_SYS_PHY_WRITE(1U, 0U, 0x1200U)
			}
		},
		.tPhyMacroExtern =
		{
			.aucMacro =
			{
				PHY_SETUP_COMMAND_End
			}
		}
	}
};

