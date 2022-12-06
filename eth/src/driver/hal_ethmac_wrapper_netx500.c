#include "hal_ethmac_wrapper.h"

#include "regdef_netx500.h"

#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_armtimer.h"
#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_pfifo.h"
#include "HETHMAC/ARM_Application/Components/hal_common/netx100/Includes/common_func_netx500.h"
#include "HETHMAC/ARM_Application/Components/ip_stack/Includes/phy.h"
/*
#include "HETHMAC/ARM_Application/Components/hal_common/netx90/Includes/hal_intphy.h"
#include "HETHMAC/ARM_Application/Components/hal_common/netx90/Includes/hw_defines.h"
*/

static uint32_t s_ulMiimFreq;
static uint32_t s_ulMiimPreamble;


/* read PHY register */
static unsigned int sys_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;
  unsigned int uiMdcFreq  = 1 & (s_ulMiimFreq >> uiPhy);
  unsigned int uiPreamble = 1 & (s_ulMiimPreamble >> uiPhy);

  NX500_MIIMU_ReadPhyReg(uiPreamble,
                         uiMdcFreq,
                         NX500_MIIMU_RTA_0_BITS,
                         uiPhy,
                         uiReg,
                         &usMiimuData,
                         0);
  return (unsigned int)usMiimuData;
}

/* write PHY register */
static void sys_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  unsigned int uiMdcFreq  = 1 & (s_ulMiimFreq >> uiPhy);
  unsigned int uiPreamble = 1 & (s_ulMiimPreamble >> uiPhy);

  NX500_MIIMU_WritePhyReg(uiPreamble,
                          uiMdcFreq,
                          uiPhy,
                          uiReg,
                          (uint16_t)uiValue,
                          0);
}

/* MII Management Interface */
static PHY_OPS_T s_tPhyOps =
{
  sys_phy_write,
  sys_phy_read
};



void hal_ethmac_phy_init(unsigned int uiEnabledPorts)
{
	NX500_PHY_CONTROL_T tPhyControlReg;


	s_ulMiimFreq = 0;
	s_ulMiimPreamble = 0xffffffff;

	/* initialize Dual-PHY */
	tPhyControlReg.bf.phy_address       = 0;
	tPhyControlReg.bf.phy0_mode         = PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
	tPhyControlReg.bf.phy0_fxmode       = 0;
	tPhyControlReg.bf.phy0_automdix     = PHY_CTRL_PHY1_AUTOMDIX;
	tPhyControlReg.bf.phy0_np_msg_code  = PHY_CTRL_PHY1_NP_MSG_CODE;
	tPhyControlReg.bf.phy0_enable       = PHY_CTRL_PHY1_ENABLE;
	tPhyControlReg.bf.phy1_mode         = PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
	tPhyControlReg.bf.phy1_fxmode       = 0;
	tPhyControlReg.bf.phy1_automdix     = PHY_CTRL_PHY2_AUTOMDIX;
	tPhyControlReg.bf.phy1_np_msg_code  = PHY_CTRL_PHY2_NP_MSG_CODE;
	tPhyControlReg.bf.phy1_enable       = PHY_CTRL_PHY2_ENABLE;
	tPhyControlReg.bf.reserved1         = 0;
	tPhyControlReg.bf.phy_sim_byp       = 0;
	tPhyControlReg.bf.phy_reset         = 0;
	NX500_INTPHY_Init( &tPhyControlReg );

	phy_init(0, 0, PHYID_NETX_INTPHY, &s_tPhyOps);
	phy_init(1, 1, PHYID_NETX_INTPHY, &s_tPhyOps);
}



void hal_ethmac_phy_deinit(unsigned int uiEnabledPorts)
{
	NX500_PHY_CONTROL_T tPhyControlReg;


	/* initialize Dual-PHY */
	tPhyControlReg.bf.phy_address       = 0;
	tPhyControlReg.bf.phy0_mode         = 0;
	tPhyControlReg.bf.phy0_fxmode       = 0;
	tPhyControlReg.bf.phy0_automdix     = 0;
	tPhyControlReg.bf.phy0_np_msg_code  = 0;
	tPhyControlReg.bf.phy0_enable       = 0;
	tPhyControlReg.bf.phy1_mode         = 0;
	tPhyControlReg.bf.phy1_fxmode       = 0;
	tPhyControlReg.bf.phy1_automdix     = 0;
	tPhyControlReg.bf.phy1_np_msg_code  = 0;
	tPhyControlReg.bf.phy1_enable       = 0;
	tPhyControlReg.bf.reserved1         = 0;
	tPhyControlReg.bf.phy_sim_byp       = 0;
	tPhyControlReg.bf.phy_reset         = 0;
	NX500_INTPHY_Init( &tPhyControlReg );

	phy_init(0, 0, PHYID_NETX_INTPHY, &s_tPhyOps);
	phy_init(1, 1, PHYID_NETX_INTPHY, &s_tPhyOps);
}


/*-------------------------------------------------------------------------*/


void hal_ethmac_pfifo_reset(void)
{
	NX500_PFIFO_Reset();
}



int hal_ethmac_xc_reset(unsigned int uiPort)
{
	int iResult;


	iResult = NX500_XC_Reset(uiPort, NULL);
	return iResult;
}
