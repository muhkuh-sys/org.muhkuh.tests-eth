#include "hal_ethmac_wrapper.h"

#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_armtimer.h"
#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_asicctrl.h"
#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_miimu.h"
#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_xc.h"
#include "HETHMAC/ARM_Application/Components/hal_common/netx90/Includes/hal_intphy.h"
#include "HETHMAC/ARM_Application/Components/hal_common/netx90/Includes/hw_defines.h"
#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_pfifo.h"
#include "HETHMAC/ARM_Application/Components/ip_stack/Includes/phy.h"
#include "HETHMAC/ARM_Application/Targets/Includes/eth_mac.h"


#define INTPHY_PORT_CNT 2

#define MIIMU_READ(phy,reg,data)  MIIMU_ReadPhyReg(MIIMU_SEND_PREAMBLE, MIIMU_MDCFREQ_HIGH, MIIMU_RTA_1_BITS, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) MIIMU_WritePhyReg(MIIMU_SEND_PREAMBLE, MIIMU_MDCFREQ_HIGH, MIIMU_RTA_1_BITS, phy, reg, data, NULL)


static void sys_sleep(unsigned int uiTimeMs)
{
  ArmTimer_Sleep(0, uiTimeMs * 1000000);
}


/* read PHY register */
static unsigned int sys_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
	uint16_t usMiimuData = 0;

	MIIMU_READ(uiPhy, uiReg,  &usMiimuData);

	return (unsigned int)usMiimuData;
}

/* write PHY register */
static void sys_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
	MIIMU_WRITE(uiPhy, uiReg, (uint16_t)uiValue);
}

/* MII Management Interface */
static const PHY_OPS_T s_tPhyOps =
{
  sys_phy_write,
  sys_phy_read
};


/* Reset internal Dual-PHY */
static void int_phy_reset(unsigned int uiEnabledPorts, bool fFxMode)
{
  INTPHY_CTRL_T tPhyCtrl;
  unsigned int uiPort;

  INTPHY_Init();

  tPhyCtrl.uiPhyMode = fFxMode ? INTPHY_MODE_100BASE_TXFX_FD_NOAUTONEG_CRSRX
                               : INTPHY_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;

  tPhyCtrl.fPhySimBypass = false;
  tPhyCtrl.fPhyReset = false;
  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort) {
    if( uiPort<uiEnabledPorts)
    {
      tPhyCtrl.afPhyFxMode[uiPort]   = fFxMode;
      tPhyCtrl.afPhyAutoMdix[uiPort] = !fFxMode;
      tPhyCtrl.afPhyEnable[uiPort]   = true;
    }
    else
    {
      tPhyCtrl.afPhyFxMode[uiPort]   = false;
      tPhyCtrl.afPhyAutoMdix[uiPort] = false;
      tPhyCtrl.afPhyEnable[uiPort]   = false;
    }
  }
  INTPHY_Reset(&tPhyCtrl);

  /* Wait 1ms for PHY reset */
  sys_sleep(1);
}


void hal_ethmac_phy_init(unsigned int uiEnabledPorts)
{
	uint32_t ulValue;


	/* Connect XC to internal PHY */
	AsicCtrl_SetIoConfig(1, 8U << SRT_NX90_io_config0_sel_xm0_mii_cfg
	                      | 2U << SRT_NX90_io_config0_mask_sel_xc0_mdio);
	if( uiEnabledPorts>1 )
	{
		AsicCtrl_SetIoConfig(2, 8U << SRT_NX90_io_config1_sel_xm1_mii_cfg);
	}

	/* PHY LEDs from internal PHY are all low-active */
	ulValue = MSK_NX90_phy_ctrl0_phy0_led_invert;
	if( uiEnabledPorts>1 )
	{
		ulValue |= MSK_NX90_phy_ctrl0_phy1_led_invert;
	}
	AsicCtrl_SetPhyCtrl0(ulValue);

	int_phy_reset(uiEnabledPorts, 0);
	phy_init(0,  0, PHYID_OMNIPHY, &s_tPhyOps);
	if( uiEnabledPorts>1 )
	{
		phy_init(1,  1, PHYID_OMNIPHY, &s_tPhyOps);
	}
}



void hal_ethmac_phy_deinit(unsigned int uiEnabledPorts)
{
	/* Disconnect XC from internal PHY */
	AsicCtrl_SetIoConfig(1, DFLT_VAL_NX90_io_config0_sel_xm0_mii_cfg
	                      | DFLT_VAL_NX90_io_config0_mask_sel_xc0_mdio);
	if( uiEnabledPorts>1 )
	{
		AsicCtrl_SetIoConfig(2, DFLT_VAL_NX90_io_config1_sel_xm1_mii_cfg);
	}

	int_phy_reset(0, 0);
}


/*-------------------------------------------------------------------------*/


void hal_ethmac_pfifo_reset(void)
{
	PFIFO_Reset(0);
}



int hal_ethmac_xc_reset(unsigned int uiPort)
{
	int iResult;
	unsigned int uiXc;
	unsigned int uiXpec;


	/* Get the XC and XPEC index from the port. */
	uiXc  = uiPort >> 1U;
	uiXpec = uiPort & 1U;
	iResult = XC_Reset(uiXc, uiXpec, NULL);

	return iResult;
}
