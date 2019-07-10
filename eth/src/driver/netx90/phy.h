#ifndef __PHY_H__
#define __PHY_H__


typedef enum PHY_SETUP_COMMAND_ENUM
{
	PHY_SETUP_COMMAND_End              =  0,
	PHY_SETUP_COMMAND_WriteIntPhyCfg   =  1,
	PHY_SETUP_COMMAND_WriteXc0PhyCtrl0 =  2,
	PHY_SETUP_COMMAND_WriteXc0PhyCtrl1 =  3,
	PHY_SETUP_COMMAND_WriteEthSystem   =  4,
	PHY_SETUP_COMMAND_WriteAbs         =  5,
	PHY_SETUP_COMMAND_PollIntPhyCfg    =  6,
	PHY_SETUP_COMMAND_PollXc0PhyCtrl0  =  7,
	PHY_SETUP_COMMAND_PollXc0PhyCtrl1  =  8,
	PHY_SETUP_COMMAND_PollEthSystem    =  9,
	PHY_SETUP_COMMAND_PollAbs          = 10,
	PHY_SETUP_COMMAND_MiiEthSystem     = 11,
	PHY_SETUP_COMMAND_MiiXc0PhyCtrl0   = 12,
	PHY_SETUP_COMMAND_MiiXc0PhyCtrl1   = 13,
	PHY_SETUP_COMMAND_Delay            = 14,
	PHY_SETUP_COMMAND_Execute          = 15,
	PHY_SETUP_COMMAND_Execute01        = 16,
	PHY_SETUP_COMMAND_Execute0123      = 17
} PHY_SETUP_COMMAND_T;



#define REG_BANK_DSP   0
#define REG_BANK_WOL   1
#define REG_BANK_BIST  3
#define REG_BANK_VMDAC 7

/* Default bank */
#define REG_BASIC_CTRL                       0

/* DSP bank */
#define REG_DCBLW                            0
#define REG_DBGCNTL                          1
#define REG_AMP_THRESH_1                     2
#define REG_AMP_THRESH_2                     3
#define REG_AMP_THRESH_3                     4
#define REG_AMP_THRESH_4                     5
#define REG_AMP_THRESH_5                     6
#define REG_AMP_THRESH_6                     7
#define REG_AMP_THRESH_7                     8
#define REG_ON_ENERGY_LEVEL                  9
#define REG_OFF_ENERGY_LEVEL_0              10
#define REG_OFF_ENERGY_LEVEL_1              11
#define REG_ENERGY_CNT_THRSH                12
#define REG_BINIT_0123                      13
#define REG_BINIT_4567                      14
#define REG_DP_BASE_QTH                     15
#define REG_DIFFZ                           16
#define REG_CONFIG                          17
#define REG_A1_CONFIG                       18
#define REG_A2_CONFIG                       19
#define REG_A3_CONFIG                       20
#define REG_A4_CONFIG                       21
#define REG_A5_CONFIG                       22
#define REG_A6_CONFIG                       23
#define REG_A7_CONFIG                       24
#define REG_CONFIG2                         25
#define REG_A8_CONFIG                       26
#define REG_A9_CONFIG                       27
#define REG_A10_CONFIG                      28
#define REG_A11_CONFIG                      29
#define REG_GAIN1                           30
#define REG_GAIN2                           31


/* VMDAC bank */
#define REG_ZQ_CAL_CONTROL                   0
#define REG_ZQ_CAL_STATUS                    1
#define REG_VM_DAC_CONTROL0                  2
#define REG_VM_DAC_CONTROL1                  3
#define REG_MISC_PHY_CONTROL0                4
#define REG_MISC_PCS_CONTROL0                5
#define REG_MISC_PCS_CONTROL1                6
#define REG_MISC_STATUS                      7
#define REG_MISC_STATUS1                     8
#define REG_TXDAC_CODE_01                    9
#define REG_TXDAC_CODE_23                   10
#define REG_TXDAC_CODE_45                   11
#define REG_TXDAC_CODE_6                    12


#define PHY_SETUP_DW(a) ((a)&0xffU), (((a)>>8U)&0xffU), (((a)>>16U)&0xffU), (((a)>>24U)&0xffU)


#define PHY_SETUP_INTPHY_SetPhyCtrl(PHY_ADDRESS, PHY0_MODE, PHY0_FXMODE, PHY0_AUTOMDIX, PHY0_NP_MSG_CODE, PHY0_ENABLE, PHY1_MODE, PHY1_FXMODE, PHY1_AUTOMDIX, PHY1_NP_MSG_CODE, PHY1_ENABLE, PHY_RESET) \
	PHY_SETUP_COMMAND_WriteIntPhyCfg, REL_Adr_NX90_int_phy_cfg_phy_ctrl/sizeof(unsigned long), \
	PHY_SETUP_DW( \
		(PHY_ADDRESS) << HOSTSRT(int_phy_cfg_phy_ctrl_phy_address) | \
		(PHY0_MODE) << HOSTSRT(int_phy_cfg_phy_ctrl_phy0_mode) | \
		(PHY0_FXMODE) << HOSTSRT(int_phy_cfg_phy_ctrl_phy0_fxmode) | \
		(PHY0_AUTOMDIX) << HOSTSRT(int_phy_cfg_phy_ctrl_phy0_automdix) | \
		(PHY0_NP_MSG_CODE) << HOSTSRT(int_phy_cfg_phy_ctrl_phy0_np_msg_code) | \
		(PHY0_ENABLE) << HOSTSRT(int_phy_cfg_phy_ctrl_phy0_enable) | \
		(PHY1_MODE) << HOSTSRT(int_phy_cfg_phy_ctrl_phy1_mode) | \
		(PHY1_FXMODE) << HOSTSRT(int_phy_cfg_phy_ctrl_phy1_fxmode) | \
		(PHY1_AUTOMDIX) << HOSTSRT(int_phy_cfg_phy_ctrl_phy1_automdix) | \
		(PHY1_NP_MSG_CODE) << HOSTSRT(int_phy_cfg_phy_ctrl_phy1_np_msg_code) | \
		(PHY1_ENABLE) << HOSTSRT(int_phy_cfg_phy_ctrl_phy1_enable) | \
		(PHY_RESET) << HOSTSRT(int_phy_cfg_phy_ctrl_phy_reset) \
	)



#define PHY_SETUP_SYS_PHY_WRITE(PHY, REG, VAL) \
	PHY_SETUP_COMMAND_MiiXc0PhyCtrl0, \
	PHY_SETUP_DW( \
		    1 << HOSTSRT(int_phy_ctrl_miimu_snrdy) | \
		    1 << HOSTSRT(int_phy_ctrl_miimu_preamble) | \
		    1 << HOSTSRT(int_phy_ctrl_miimu_opmode) | \
		    0 << HOSTSRT(int_phy_ctrl_miimu_mdc_period) | \
		    1 << HOSTSRT(int_phy_ctrl_miimu_rta) | \
		(REG) << HOSTSRT(int_phy_ctrl_miimu_regaddr) | \
		(PHY) << HOSTSRT(int_phy_ctrl_miimu_phyaddr) | \
		(VAL) << HOSTSRT(int_phy_ctrl_miimu_data) \
	), \
	PHY_SETUP_DW(1000)



#define PHY_SETUP_SYS_PHY_WRITE_BANK_DSP(PHY, BANK, REG, VAL) \
	PHY_SETUP_SYS_PHY_WRITE((PHY), 23, (VAL)), \
	PHY_SETUP_SYS_PHY_WRITE((PHY), 20, (0x4400 | ((BANK) << 11) | (REG))), \
	PHY_SETUP_SYS_PHY_WRITE((PHY), 20, 0x0000)

#define PHY_SETUP_SYS_PHY_WRITE_BANK_NOTDSP(PHY, BANK, REG, VAL) \
	PHY_SETUP_SYS_PHY_WRITE((PHY), 23, (VAL)), \
	PHY_SETUP_SYS_PHY_WRITE((PHY), 20, (0x4000 | ((BANK) << 11) | (REG))), \
	PHY_SETUP_SYS_PHY_WRITE((PHY), 20, 0x0000)


#define PHY_SETUP_DELAY(DELAY) \
	PHY_SETUP_COMMAND_Delay, PHY_SETUP_DW((DELAY))



int phy_setup_execute_sequence(const unsigned char *pucSequence, unsigned int sizSequence);


#endif  /* __PHY_H__ */
