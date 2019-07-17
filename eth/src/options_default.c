/***************************************************************************
 *   Copyright (C) 2016 by Hilscher GmbH                                   *
 *                                                                         *
 *   Author: Christoph Thelen (cthelen@hilscher.com)                       *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/


#include "stack/ipv4.h"
#include "netx_io_areas.h"
#include "options.h"

#if ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
#       include "driver/netx4000/drv_eth_xc.h"
#       include "driver/netx4000/nec_cb12.h"
#elif ASIC_TYP==ASIC_TYP_NETX90_MPW || ASIC_TYP==ASIC_TYP_NETX90
#       include "driver/netx90/drv_eth_xc.h"
#       include "driver/netx90/phy.h"
#endif


const ROMLOADER_OPTIONS_T t_default_options =
{
	.t_ethernet =
	{
		.atPorts =
		{
			/* Port 0 */
			{
				.acName = "CH0",
				.tInterface = INTERFACE_INTPHY0,
				.tFunction = INTERFACE_FUNCTION_EchoClient,
				.ulFlags = 0,
				.aucMac = { 0x00U, 0x02U, 0xa2U, 0x20U, 0x20U, 0x00U },
				.ulIp = IP_ADR(192,168,64,20),
				.ulGatewayIp = 0,
				.ulNetmask = IP_ADR(255,255,255,0),
				.usLinkUpDelay = 1000,
				.usLocalPort = MUS2NUS(1024),
				.ulRemoteIp = IP_ADR(192,168,64,1),
				.usRemotePort = MUS2NUS(5555)
			},
			/* Port 1 */
			{
				.acName = "CH1",
				.tInterface = INTERFACE_INTPHY1,
				.tFunction = INTERFACE_FUNCTION_EchoClient,
				.ulFlags = 0,
				.aucMac = { 0x00U, 0x02U, 0xa2U, 0x20U, 0x20U, 0x01U },
				.ulIp = IP_ADR(192,168,65,20),
				.ulGatewayIp = 0,
				.ulNetmask = IP_ADR(255,255,255,0),
				.usLinkUpDelay = 1000,
				.usLocalPort = MUS2NUS(1025),
				.ulRemoteIp = IP_ADR(192,168,65,1),
				.usRemotePort = MUS2NUS(5555)
			}
		},
		.usArpTimeout = 1000,
		.usDhcpTimeout = 3000,
		.ucArpRetries = 10,
		.ucDhcpRetries = 5,
#if ASIC_TYP==ASIC_TYP_NETX4000_RELAXED || ASIC_TYP==ASIC_TYP_NETX4000
		.ulPhyControl = (PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << HOSTSRT(phy_control_phy_mode)) |
		                HOSTMSK(phy_control_phy0_automdixen) |
		                HOSTMSK(phy_control_phy1_automdixen) |
		                HOSTMSK(phy_control_phy0_enable) |
		                HOSTMSK(phy_control_phy1_enable)
#elif ASIC_TYP==ASIC_TYP_NETX90_MPW
		.ulPhyControl = 0,
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
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(0U, REG_BANK_BIST, 0x1cU, 0x0000U),
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(0U, REG_BANK_BIST, 0x1dU, 0x0280U),

				PHY_SETUP_DELAY(2),

				/* sys_phy_write(uiPhy, 17, 0x0040);
				 * sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x02, 0xF07B);
				 * sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x03, 0x0004);
				 */
				PHY_SETUP_SYS_PHY_WRITE(0U, 17U, 0x0040U),
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(0U, REG_BANK_VMDAC, 0x02U, 0xF07BU),
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(0U, REG_BANK_VMDAC, 0x03U, 0x0004U),

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
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(1U, REG_BANK_BIST, 0x1cU, 0x0000U),
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(1U, REG_BANK_BIST, 0x1dU, 0x0280U),

				PHY_SETUP_DELAY(2),

				/* sys_phy_write(uiPhy, 17, 0x0040);
				 * sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x02, 0xF07B);
				 * sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x03, 0x0004);
				 */
				PHY_SETUP_SYS_PHY_WRITE(1U, 17U, 0x0040U),
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(1U, REG_BANK_VMDAC, 0x02U, 0xF07BU),
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(1U, REG_BANK_VMDAC, 0x03U, 0x0004U),

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
#elif ASIC_TYP==ASIC_TYP_NETX90
		.ulPhyControl = 0,
		.tPhyMacroIntern =
		{
			.aucMacro =
			{
#if 0

				/******************************************************************
				 *
				 * Reset the INTPHY.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(10),
#       if 0
				/******************************************************************
				 *
				 * Set the configuration while reset is active.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 7U, 0U, 1U, 0U, 1U, 7U, 0U, 1U, 0U, 1U, 1U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(2),
#       else
				/******************************************************************
				 *
				 * Set the configuration while reset is active.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 7U, 0U, 1U, 0U, 1U, 0U, 0U, 0U, 0U, 0U, 1U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(2),
#       endif
				/* Port 0 */
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_ctrl_ext0/sizeof(unsigned long), PHY_SETUP_DW(0xf8000800U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_gain_table0/sizeof(unsigned long), PHY_SETUP_DW(0xcba98321U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_1/sizeof(unsigned long), PHY_SETUP_DW(1100),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_2/sizeof(unsigned long), PHY_SETUP_DW(900),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_3/sizeof(unsigned long), PHY_SETUP_DW(750),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_4/sizeof(unsigned long), PHY_SETUP_DW(600),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_5/sizeof(unsigned long), PHY_SETUP_DW(520),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_6/sizeof(unsigned long), PHY_SETUP_DW(450),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_7/sizeof(unsigned long), PHY_SETUP_DW(400),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_off_energy_level0/sizeof(unsigned long), PHY_SETUP_DW(0x87766558U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_binit0/sizeof(unsigned long), PHY_SETUP_DW(0x0eca8876U),
#       if 0
				/* Port 1 */
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_ctrl_ext1/sizeof(unsigned long), PHY_SETUP_DW(0xf8000800U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_gain_table1/sizeof(unsigned long), PHY_SETUP_DW(0xcba98321U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_1/sizeof(unsigned long), PHY_SETUP_DW(1100),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_2/sizeof(unsigned long), PHY_SETUP_DW(900),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_3/sizeof(unsigned long), PHY_SETUP_DW(750),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_4/sizeof(unsigned long), PHY_SETUP_DW(600),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_5/sizeof(unsigned long), PHY_SETUP_DW(520),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_6/sizeof(unsigned long), PHY_SETUP_DW(450),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_7/sizeof(unsigned long), PHY_SETUP_DW(400),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_off_energy_level1/sizeof(unsigned long), PHY_SETUP_DW(0x87766558U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_binit1/sizeof(unsigned long), PHY_SETUP_DW(0x0eca8876U),
#       endif

#       if 0
				/******************************************************************
				 *
				 * Remove the reset.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 7U, 0U, 1U, 0U, 1U, 7U, 0U, 1U, 0U, 1U, 0U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(2),
#       else
				/******************************************************************
				 *
				 * Remove the reset.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 7U, 0U, 1U, 0U, 1U, 0U, 0U, 0U, 0U, 0U, 0U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(2),
#       endif
				/* PHY Fixup. */
				/* DCBLW (Direct Current for Base Line Wander Canceler) */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_DCBLW, 0x1674U),
				/* CONFIG2 */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_CONFIG2, 0x10d0U),
				/* A3_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_A3_CONFIG, 0x7500U),
				/* A5_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_A5_CONFIG, 0x840eU),
				/* A11_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_A11_CONFIG, 0x5400U),
				/* MISC_PHY_CONTROL0: set tx_vd_amp_itrim = 0 */
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(0, REG_BANK_VMDAC, REG_MISC_PHY_CONTROL0, 0x0a08U),
#       if 0
				/* DCBLW (Direct Current for Base Line Wander Canceler) */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_DCBLW, 0x1674U),
				/* CONFIG2 */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_CONFIG2, 0x10d0U),
				/* A3_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_A3_CONFIG, 0x7500U),
				/* A5_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_A5_CONFIG, 0x840eU),
				/* A11_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_A11_CONFIG, 0x5400U),
				/* MISC_PHY_CONTROL0: set tx_vd_amp_itrim = 0 */
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(1, REG_BANK_VMDAC, REG_MISC_PHY_CONTROL0, 0x0a08U),
#       endif
				/* Restart AutoNeg (due to we reset PHY in AutoNeg mode) */
				PHY_SETUP_SYS_PHY_WRITE(0U, 0U, 0x1200U),
#       if 0
				PHY_SETUP_SYS_PHY_WRITE(1U, 0U, 0x1200U),
#       endif

#else

				/******************************************************************
				 *
				 * Reset the INTPHY.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(100),

				/* Port 0 */
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_ctrl_ext0/sizeof(unsigned long), PHY_SETUP_DW(0xf8000800U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_gain_table0/sizeof(unsigned long), PHY_SETUP_DW(0xcba98321U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_1/sizeof(unsigned long), PHY_SETUP_DW(1000),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_2/sizeof(unsigned long), PHY_SETUP_DW(750),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_3/sizeof(unsigned long), PHY_SETUP_DW(610),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_4/sizeof(unsigned long), PHY_SETUP_DW(570),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_5/sizeof(unsigned long), PHY_SETUP_DW(535),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_6/sizeof(unsigned long), PHY_SETUP_DW(460),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh0_7/sizeof(unsigned long), PHY_SETUP_DW(407),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_off_energy_level0/sizeof(unsigned long), PHY_SETUP_DW(0x87766558U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_binit0/sizeof(unsigned long), PHY_SETUP_DW(0x0eca8876U),

				/* Port 1 */
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_ctrl_ext1/sizeof(unsigned long), PHY_SETUP_DW(0xf8000800U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_gain_table1/sizeof(unsigned long), PHY_SETUP_DW(0xcba98321U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_1/sizeof(unsigned long), PHY_SETUP_DW(1000),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_2/sizeof(unsigned long), PHY_SETUP_DW(750),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_3/sizeof(unsigned long), PHY_SETUP_DW(610),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_4/sizeof(unsigned long), PHY_SETUP_DW(570),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_5/sizeof(unsigned long), PHY_SETUP_DW(535),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_6/sizeof(unsigned long), PHY_SETUP_DW(460),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_amp_thresh1_7/sizeof(unsigned long), PHY_SETUP_DW(407),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_off_energy_level1/sizeof(unsigned long), PHY_SETUP_DW(0x87766558U),
				PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_binit1/sizeof(unsigned long), PHY_SETUP_DW(0x0eca8876U),

				/******************************************************************
				 *
				 * Set the configuration while reset is active.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 7U, 0U, 1U, 0U, 1U, 7U, 0U, 1U, 0U, 1U, 1U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(100),

				/******************************************************************
				 *
				 * Remove the reset.
				 *
				 ******************************************************************/
				PHY_SETUP_INTPHY_SetPhyCtrl(0U, 7U, 0U, 1U, 0U, 1U, 7U, 0U, 1U, 0U, 1U, 0U),
				/* Delay for at least 1ms. */
				PHY_SETUP_DELAY(100),

				/* PHY Fixup. */
				/* DCBLW (Direct Current for Base Line Wander Canceler) */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_DCBLW, 0x1674U),
				/* CONFIG2 */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_CONFIG2, 0x10d0U),
				/* A3_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_A3_CONFIG, 0x7500U),
				/* A5_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_A5_CONFIG, 0x840eU),
				/* A11_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(0, REG_BANK_DSP, REG_A11_CONFIG, 0x5400U),
				/* MISC_PHY_CONTROL0: set tx_vd_amp_itrim = 0 */
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(0, REG_BANK_VMDAC, REG_MISC_PHY_CONTROL0, 0x0a08U),

				/* DCBLW (Direct Current for Base Line Wander Canceler) */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_DCBLW, 0x1674U),
				/* CONFIG2 */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_CONFIG2, 0x10d0U),
				/* A3_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_A3_CONFIG, 0x7500U),
				/* A5_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_A5_CONFIG, 0x840eU),
				/* A11_CONFIG */
				PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(1, REG_BANK_DSP, REG_A11_CONFIG, 0x5400U),
				/* MISC_PHY_CONTROL0: set tx_vd_amp_itrim = 0 */
				PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(1, REG_BANK_VMDAC, REG_MISC_PHY_CONTROL0, 0x0a08U),

				/* Restart AutoNeg (due to we reset PHY in AutoNeg mode) */
				PHY_SETUP_SYS_PHY_WRITE(0U, 0U, 0x1200U),
				PHY_SETUP_SYS_PHY_WRITE(1U, 0U, 0x1200U),

#endif
			}
		},
		.tPhyMacroExtern =
		{
			.aucMacro =
			{
				PHY_SETUP_COMMAND_End
			}
		}
#endif
	}
};

