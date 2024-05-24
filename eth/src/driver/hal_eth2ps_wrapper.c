#include "hal_eth2ps_wrapper.h"

#include <stdbool.h>
#include <string.h>

#include "HETH2PS/ARM_Application/Components/hal_adinphy/Includes/hal_adinphy.h"
#include "HETH2PS/ARM_Application/Components/hal_common/netx90/Includes/hal_intphy.h"
#include "HETH2PS/ARM_Application/Components/hal_common/netx90/Includes/hw_defines.h"
#include "HETH2PS/ARM_Application/Components/hal_common/Includes/hal_asicctrl.h"
#include "HETH2PS/ARM_Application/Components/hal_common/Includes/hal_miimu.h"
#include "HETH2PS/ARM_Application/Components/hal_common/Includes/hal_pfifo.h"
#include "HETH2PS/ARM_Application/Components/hal_common/Includes/hal_xc.h"
#include "HETH2PS/ARM_Application/Components/targetlib/Soft/Includes/phy.h"
#include "HETH2PS/ARM_Application/Targets/Includes/eth_2ps.h"
#include "HETH2PS/ARM_Application/Targets/Includes/sys.h"

#define EXTPHY_MDIO_ADDR_PORT0 0x0  /* MDIO hardware address of the external PHY on port 0 */
#define EXTPHY_MDIO_ADDR_PORT1 0x3  /* MDIO hardware address of the external PHY on port 1, if connected */

#define INGRESS_LATENCY (200 + 20) /* default is internal OmniPhy EPHY (latency_red=0) + half clock period due timestamp reference point at rising edge of MII.RxClk */
#define EGRESS_LATENCY  116 /* default is internal OmniPhy PHY */

#define ETHERNET_MINIMUM_FRAMELENGTH 60






#include "HETH2PS/ARM_Application/Components/hal_common/netx90/Includes/hal_mled_ctrl.h"
#include "HETH2PS/ARM_Application/Components/hal_common/netx90/Includes/hal_padctrl.h"


static uint32_t const s_aulPadCtrlTable[] =
{
  /*          PORT                     IE DS              PE */

  /* set driver strength to 8mA for brighter LEDs */
  PADCTRL_CFG(PAD_CTRL_PORT_MLED0,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED1,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED2,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED3,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),

  /* enable inputs and outputs */
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO0,     1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO1,     1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO2,     1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO3,     1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),

  PADCTRL_CFG(PAD_CTRL_PORT_MMIO4,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO5,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO6,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO7,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),

  /* Enable inputs at MII1.RXD */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD0, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD1, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD2, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD3, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXDV, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXCLK,1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_COM_IO0,   1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),  /* MII1.RX_ER muxed to COM_IO0 with sel_extphy */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXCLK,1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),

  /* Disable input of COL/CRS pins as they assert FCS and alignment errors
   * at the MAC (even in full-duplex mode) */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_RDN,   0, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),  /* MII1.COL muxed to HIF_RDN with sel_extphy */
  PADCTRL_CFG(PAD_CTRL_PORT_COM_IO1,   0, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),  /* MII1.CRS muxed to COM_IO1 with sel_extphy */

  /* Use high output driver strength of 8mA to achieve small rise and fall times.
   * Errors have been observed with 4mA drivers! */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD0, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD1, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD2, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD3, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXEN, 1, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* Activate IE of MII1.TX_EN to feed this signal
                                                                                 back to the MLED blink unit for activity LED */

  /* Added for software MDIO */
  PADCTRL_CFG(PAD_CTRL_PORT_MII_MDIO,  1, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII_MDC,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),

  /* Added for interrupt on MMIO17 (INT_N of extPHY) */
  // HIF_DIRQ input enable is enabled by default, as well as pull up enable
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_DIRQ,  1, PADCTRL_DS_4MA, PADCTRL_PE_PU50K),
};


void hal_ethps2_prepare(void)
{
  /* Set PAD_CTRL with the configuration table above */
  if( PADCTRL_RESULT_OK != PadCtrl_SetCfgTable(s_aulPadCtrlTable) )
  {
    while(1);
  }

  /* Map MLED2 to internal PHY LEDs of port 0 */
  MLEDCTRL_ConfigureOutput(0, 4, MLED_CTRL_SEL_PHY_CTRL0_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 5, MLED_CTRL_SEL_PHY_CTRL0_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);

  /* Map MLED3 to PHY LEDs of port 1, but only led1 (activity). LED0 (link status) is controlled
   * via software, as the pins PHY_LED_LINK_INx (link status of external PHY) are occupied with
   * SD-RAM */
  /* Activity LED is generated with external MII's TX_EN and RX_DV signals in MLED blink unit */
  MLEDCTRL_ConfigureOutput(0, 6, MLED_CTRL_SEL_PHY_CTRL1_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 7, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);

  /* Select line-mode for MLED[1:0] */
  MLEDCTRL_ConfigureOutput(0, 0, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 1, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 2, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 3, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);

  MLEDCTRL_SetLine(0, 0x0);
  MLEDCTRL_Start(0, 3, 0);

  /* Connect XC to external PHY, no TX_ER */
  AsicCtrl_SetIoConfig(2, 5U << SRT_NX90_io_config1_sel_xm1_mii_cfg       /* Select full MII1 with COL / CRS  */
                        | 1U << SRT_NX90_io_config1_sel_xc1_mdio);     /* Connect MDIO of xC1 to external MII_MDIO  */


  AsicCtrl_SetIoConfig(9, 1U << SRT_NX90_io_config8_sel_extphy);        /* Select *_extphy pins in pinning table
                                                                     to enable the use of SD-RAM with an
                                                                     external PHY */

  /* Uncomment this line only if the crystal of the external PHY is not populated on the board
   * and the netX 90 should be used as a clock source for the external PHY  */
  // AsicCtrl_SetIoConfig(3, BF(io_config2_clk25out_oe, 0x1));

  /* Apply reset to external PHYs */
  AsicCtrl_SetResetOut(true, true);

  /* Wait 1ms (creates the minimum length for a reset pulse) */
  ArmTimer_Sleep(2, 100000);
}






SYS_CFG_T g_tSysCfg;
extern ETH_2PS_T g_tEth2PS;

static void intphy_write(unsigned int uiPhy, unsigned int uiAddr, unsigned int uiVal)
{
  unsigned int uiBank = uiAddr >> 5;

  if( uiBank == 0 )
  {
    INTPHY_Write(uiPhy, uiAddr, uiVal);
  }
  else
  {
    --uiBank;
    INTPHY_WriteBank(uiPhy, uiBank, uiAddr & 0x1f, uiVal);
  }
}

static unsigned int intphy_read(unsigned int uiPhy, unsigned int uiAddr)
{
  unsigned int uiBank = uiAddr >> 5;
  unsigned int uiVal;

  if( uiBank == 0 )
  {
    uiVal = INTPHY_Read(uiPhy, uiAddr);
  }
  else
  {
    --uiBank;
    uiVal = INTPHY_ReadBank(uiPhy, uiBank, uiAddr & 0x1f);
  }

  return uiVal;
}


HAL_ETH2PS_RESULT_T hal_eth2ps_phy_init(unsigned int uiNumberOfSpePorts)
{
	HAL_ETH2PS_RESULT_T tResult;
	uint32_t ulValue;
	int iHalResult;


	/* Be optimistic. */
	tResult = HAL_ETH2PS_RESULT_Ok;

	if( uiNumberOfSpePorts==0 )
	{
		/* Not supported yet. */
		tResult = HAL_ETH2PS_RESULT_0SPEPortsAreNotSupportedYet;
	}
	else if( uiNumberOfSpePorts==1 )
	{
		/* Connect XC to internal PHY */
		AsicCtrl_SetIoConfig(1, 8U << SRT_NX90_io_config0_sel_xm0_mii_cfg
		                      | 2U << SRT_NX90_io_config0_mask_sel_xc0_mdio);

		/* PHY LEDs from internal PHY are all low-active */
		ulValue = MSK_NX90_phy_ctrl0_phy0_led_invert;
		AsicCtrl_SetPhyCtrl0(ulValue);

		INTPHY_Init();

		INTPHY_SetupDualPhy();

		/* start AutoNeg with all capable modes advertised on port 0 */
		INTPHY_SetModeAutoNeg(0, MSK_INTPHY_AUTONEG_ADV_ALL_CAPABLE);

		iHalResult = phy_add(0, 0, intphy_write, intphy_read);
		if( iHalResult!=0 )
		{
			tResult = HAL_ETH2PS_RESULT_FailedToAddRTEPhy0;
		}
		else
		{
			/* Initialize external SPE PHY on xC channel 1 */
			iHalResult = adinphy_initialize(1, EXTPHY_MDIO_ADDR_PORT1, &(g_tEth2PS.tEth));
			if( iHalResult!=0 )
			{
				tResult = HAL_ETH2PS_RESULT_FailedToInitializeSPEPhy1;
			}
			else
			{
				/* Add PHY to HAL driver */
				iHalResult = phy_add(1, EXTPHY_MDIO_ADDR_PORT1, adinphy_write_cl45, adinphy_read_cl45);
				if( iHalResult!=0 )
				{
					tResult = HAL_ETH2PS_RESULT_FailedToAddSPEPhy1;
				}
				else
				{
					/* Register link state API for external PHYs */
					iHalResult = phy_register_ext_link_api(1, adinphy_get_linkstate);
					if( iHalResult!=0 )
					{
						tResult = HAL_ETH2PS_RESULT_FailedToRegisterExtLinkApiSPEPhy1;
					}
					else
					{
						/* Configure the port with the external PHY on it */
						g_tSysCfg.atPortCfg[1].fNoPhyLinkInput = true;        /* Disable link check during transmission for xC channel 1 as well
													as the link state change interrupt from xMAC1 to ARM. */
						g_tSysCfg.atPortCfg[1].fDisablePort = true;           /* Disable transmit on this port initially. When a link comes up,
													the link state change handler will enable the port again. */

						/* Disable software power-down and enable auto-negotiation */
						iHalResult = adinphy_release(1);
						if( iHalResult!=0 )
						{
							tResult = HAL_ETH2PS_RESULT_FailedToReleaseSPEPhy1;
						}
						else
						{
							/* Fill out PHY latencies MDI<->MII */
							g_tSysCfg.uiSystime = 0;
							g_tSysCfg.atPortCfg[0].ulIngressLatency = INGRESS_LATENCY;
							g_tSysCfg.atPortCfg[0].ulEgressLatency = EGRESS_LATENCY;
							g_tSysCfg.atPortCfg[1].ulIngressLatency = INGRESS_LATENCY;
							g_tSysCfg.atPortCfg[1].ulEgressLatency = EGRESS_LATENCY;

							/* set AUTO_NEG_ADVERTISEMENT, this has no effect on PHYs that use clause 45 MDIO only */
							phy_write(0, 4, 1 | PHY_ADV_ALL);             /* advertise all modes */
							phy_write(0, 0, PHY_CONTROL_AUTO_NEG_ENABLE | PHY_CONTROL_AUTO_NEG_RESTART); /* restart auto-neg */

							/* set AUTO_NEG_ADVERTISEMENT, this has no effect on PHYs that use clause 45 MDIO only */
							phy_write(1, 4, 1 | PHY_ADV_ALL);             /* advertise all modes */
							phy_write(1, 0, PHY_CONTROL_AUTO_NEG_ENABLE | PHY_CONTROL_AUTO_NEG_RESTART); /* restart auto-neg */

							tResult = HAL_ETH2PS_RESULT_Ok;
						}
					}
				}
			}
		}
	}
	else if( uiNumberOfSpePorts==2 )
	{
		/* Not supported yet. */
		tResult = HAL_ETH2PS_RESULT_2SPEPortsAreNotSupportedYet;
	}
	else
	{
		/* Unknown setup. */
		tResult = HAL_ETH2PS_RESULT_UnknownSetup;
	}

	return tResult;
}



HAL_ETH2PS_RESULT_T hal_eth2ps_phy_deinit(unsigned int uiNumberOfSpePorts)
{
	HAL_ETH2PS_RESULT_T tResult;
	int iHalResult;


	/* Be optimistic. */
	tResult = HAL_ETH2PS_RESULT_Ok;

	if( uiNumberOfSpePorts==0 )
	{
		/* Not supported yet. */
		tResult = HAL_ETH2PS_RESULT_0SPEPortsAreNotSupportedYet;
	}
	else if( uiNumberOfSpePorts==1 )
	{
		/* Disconnect XC from internal PHY */
		AsicCtrl_SetIoConfig(1, DFLT_VAL_NX90_io_config0_sel_xm0_mii_cfg
		                      | DFLT_VAL_NX90_io_config0_mask_sel_xc0_mdio);

		/* This sets both ports into power down mode. */
		INTPHY_SetupDualPhy();

		/* This sets the external PHY to power down mode. */
		iHalResult = adinphy_initialize(1, EXTPHY_MDIO_ADDR_PORT1, &(g_tEth2PS.tEth));
		if( iHalResult!=0 )
		{
			tResult = HAL_ETH2PS_RESULT_FailedToInitializeSPEPhy1;
		}
		else
		{
			tResult = HAL_ETH2PS_RESULT_Ok;
		}
	}
	else if( uiNumberOfSpePorts==2 )
	{
		/* Not supported yet. */
		tResult = HAL_ETH2PS_RESULT_2SPEPortsAreNotSupportedYet;
	}
	else
	{
		/* Unknown setup. */
		tResult = HAL_ETH2PS_RESULT_UnknownSetup;
	}

	return tResult;
}



void hal_eth2ps_pfifo_reset(void)
{
	/* reset pointer FIFO borders */
	PFIFO_Reset(0);
}



int hal_eth2ps_init(uint8_t *pucMAC0, uint8_t *pucMAC1)
{
	ETH_2PS_CONFIG_T tEthCfg;
	unsigned int uiPort;
	ETH2PS_CFG_T tCfg;
	ETH2PS_RESULT_E tHalResult;
	uint32_t ulIrqEn;
	int iResult;


	ulIrqEn  = MSK_ETH2PS_IRQ_IND_CNF_LO;
	ulIrqEn |= MSK_ETH2PS_IRQ_IND_CNF_HI;
	ulIrqEn |= MSK_ETH2PS_IRQ_LINK_CHANGED;
	ulIrqEn |= MSK_ETH2PS_IRQ_CIRCULATING_FRM;

	/* initialize driver for Ethernet 2-Port Switch */
	tEthCfg.ptAddrChassis0 = (ETH_MAC_ADDR_T*)pucMAC0;
	tEthCfg.ptAddrChassis1 = (ETH_MAC_ADDR_T*)pucMAC1;
	tEthCfg.ulAgingTimeMs  = 10000; /* 10 seconds */

	/* Copy target specific port configuration */
	for(uiPort = 0; uiPort < 2; ++uiPort)
	{
		tEthCfg.atPortCfg[uiPort].ulIngressLatency = g_tSysCfg.atPortCfg[uiPort].ulIngressLatency;
		tEthCfg.atPortCfg[uiPort].ulEgressLatency  = g_tSysCfg.atPortCfg[uiPort].ulEgressLatency;
		tEthCfg.atPortCfg[uiPort].fSetExtMii = g_tSysCfg.atPortCfg[uiPort].fSetExtMii;
		tEthCfg.atPortCfg[uiPort].fNoPhyLinkInput = g_tSysCfg.atPortCfg[uiPort].fNoPhyLinkInput;
		tEthCfg.atPortCfg[uiPort].fDisablePort = g_tSysCfg.atPortCfg[uiPort].fDisablePort;
	}

	memset(&tCfg, 0, sizeof(tCfg));
	memset(&g_tEth2PS, 0, sizeof(g_tEth2PS));

	memcpy(g_tEth2PS.tAddrChassis0, pucMAC0, 6);
	memcpy(g_tEth2PS.tAddrChassis1, pucMAC1, 6);

	tCfg.ePhyLedCfg          = ETH2PS_PHYLED_BLINK;
	tCfg.ulActLedFlashPeriod = 50;

	tHalResult = Eth2PS_Initialize(&tCfg, NULL);
	if( tHalResult==ETH2PS_OKAY )
	{
		tHalResult = Eth2PS_SetMacAddr(ETH2PS_MAC_ADDRESS_CHASSIS, pucMAC0);
		if( tHalResult==ETH2PS_OKAY )
		{
			tHalResult = Eth2PS_SetMacAddr(ETH2PS_MAC_ADDRESS_2ND_CHASSIS, pucMAC1);
			if( tHalResult==ETH2PS_OKAY )
			{
				for(uiPort = 0; uiPort < 2; ++uiPort)
				{
					tHalResult = Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_AGING_TIME, tEthCfg.ulAgingTimeMs);
					if( tHalResult!=ETH2PS_OKAY )
					{
						break;
					}
					tHalResult = Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_INGRESS_LATENCY, tEthCfg.atPortCfg[uiPort].ulIngressLatency );
					if( tHalResult!=ETH2PS_OKAY )
					{
						break;
					}
					tHalResult = Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_EGRESS_LATENCY, tEthCfg.atPortCfg[uiPort].ulEgressLatency );
					if( tHalResult!=ETH2PS_OKAY )
					{
						break;
					}
					tHalResult = Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_IRQ_EN_MSK, ulIrqEn);
					if( tHalResult!=ETH2PS_OKAY )
					{
						break;
					}
					if( tEthCfg.atPortCfg[uiPort].fSetExtMii )
					{
						tHalResult = Eth2PS_CfgMii(uiPort, 1);
						if( tHalResult!=ETH2PS_OKAY )
						{
							break;
						}
					}

					/* Disable Tx link check if needed */
					if( tEthCfg.atPortCfg[uiPort].fNoPhyLinkInput )
					{
						tHalResult = Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_LINK_INPUT_ENABLE, 0);
						if( tHalResult!=ETH2PS_OKAY )
						{
							break;
						}
					}

					/* Set initial state of PORT_ENABLE */
					if( tEthCfg.atPortCfg[uiPort].fDisablePort )
					{
						tHalResult = Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_PORT_ENABLE, 0);
						if( tHalResult!=ETH2PS_OKAY )
						{
							break;
						}
					}

					/* initialize Ingress and Egress latencies in driver */
					g_tEth2PS.atIf[uiPort].ulPortRxDelayLocal = tEthCfg.atPortCfg[uiPort].ulIngressLatency;
					g_tEth2PS.atIf[uiPort].ulPortTxDelayLocal = tEthCfg.atPortCfg[uiPort].ulEgressLatency;
				}
			}
		}
	}

	if( tHalResult==ETH2PS_OKAY )
	{
		/* enable driver */
		tHalResult = Eth2PS_Start(0);
	}

	/* ??? */
//	sync_init();


	iResult = -1;
	if( tHalResult==ETH2PS_OKAY )
	{
		iResult = 0;
	}

	return iResult;
}



int hal_eth2ps_deinit(void)
{
	int iResult;


	/* reset the hardware block */
	iResult = XC_Reset(0, 0, NULL);
	if( iResult==0 )
	{
		iResult = XC_Reset(0, 1, NULL);
	}

	return iResult;
}


int hal_eth2ps_get_link_state(unsigned int uiPort, unsigned int *puiLinkState, unsigned int *puiSpeed, unsigned int *puiIsFullDuplex)
{
	int iResult;
	int iHalResult;
	unsigned int uiLink;
	unsigned int uiSpeed;
	unsigned int uiDuplex;


	/* Be pessimistic. */
	iResult = -1;

	/* get current link mode */
	iHalResult = phy_get_linkstate(uiPort, &uiLink, &uiSpeed, &uiDuplex);
	if( iHalResult==0 )
	{
		Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_PORT_ENABLE, uiLink);
		MLEDCTRL_SetOutput(0, uiPort ? 7 : 5, uiLink);

		/* Indicate a link state change at the corresponding ETH_2PS port */
//		ETH_T* eth_instance;
//		eth_2ps_mau_type_change_ind(eth_instance, uiPort);
		/* Update link mode in MAC, please note that link mode must also updated in case of link down */
		Eth2PS_SetLinkMode(uiPort, true, uiSpeed, uiDuplex);

		if( puiLinkState!=NULL )
		{
			*puiLinkState = uiLink;
		}
		if( puiSpeed!=NULL )
		{
			*puiSpeed = uiSpeed;
		}
		if( puiIsFullDuplex!=NULL )
		{
			*puiIsFullDuplex = uiDuplex;
		}

		iResult = 0;
	}

	return iResult;
}



int hal_eth2ps_get_empty_packet(unsigned int uiPort __attribute__ ((unused)), void **ppvPacket, void **pphPacket)
{
	int iResult;
	ETH2PS_RESULT_E tHalResult;
	ETH2PS_FRAME_HANDLE_T hFrame;


	iResult = -1;

	tHalResult = Eth2PS_GetFrame(&hFrame);
	if( tHalResult==ETH2PS_OKAY )
	{
		*ppvPacket = hFrame.pbData;
		*pphPacket = (void*)(hFrame.ulFifoEntry);
		iResult = 0;
	}
	return iResult;
}



int hal_eth2ps_release_packet(unsigned int uiPort __attribute__ ((unused)), void *pvPacket, void *phPacket)
{
	int iResult;
	ETH2PS_RESULT_E tHalResult;
	ETH2PS_FRAME_HANDLE_T hFrame;


	iResult = -1;

	/* Prepare a frame handle for the Eth2PS_ReleaseFrame function.
	 * Only "ulFifoEntry" is used by the function.
	 */
	hFrame.ulFifoEntry = (uint32_t)phPacket;
	hFrame.usLength = 0U;
	hFrame.ptHdr = NULL;
	hFrame.pbData = pvPacket;
	tHalResult = Eth2PS_ReleaseFrame(&hFrame);
	if( tHalResult==ETH2PS_OKAY )
	{
		iResult = 0;
	}

	return iResult;
}



int hal_eth2ps_send_packet(unsigned int uiPort, void *pvPacket, void *phPacket, unsigned int uiPacketSize)
{
	int iResult;
	ETH2PS_RESULT_E tHalResult;
	ETH2PS_FRAME_HANDLE_T hFrame;
	unsigned int uiCnfCnt;
	unsigned short usPacketSize;


	iResult = -1;

	/* Assure a minimum packet size or the HAL will throw an error. */
	if( uiPacketSize<ETHERNET_MINIMUM_FRAMELENGTH )
	{
		usPacketSize = ETHERNET_MINIMUM_FRAMELENGTH;
	}
	else
	{
		usPacketSize = (unsigned short)uiPacketSize;
	}


	hFrame.ulFifoEntry = (uint32_t)phPacket;
	hFrame.usLength = usPacketSize;
	hFrame.ptHdr = NULL;
	hFrame.pbData = pvPacket;

	/* Send the packet to the switch. */
	tHalResult = Eth2PS_Send(uiPort, &hFrame, 0, false, &uiCnfCnt);
	if( tHalResult==ETH2PS_OKAY )
	{
		iResult = 0;
	}

	return iResult;

}



void hal_eth2ps_get_received_packet(void *pvNetworkDriver0, PFN_HAL_HANDLE_RECEIVED_PACKET pfnPort0, void *pvNetworkDriver1, PFN_HAL_HANDLE_RECEIVED_PACKET pfnPort1)
{
	ETH2PS_RESULT_E tHalResult;
	ETH2PS_FRAME_HANDLE_T hFrame;
	ETH2PS_FRAME_INFO_T tFrameInfo;


	/* Receive a packet from to low priority queue. */
	tHalResult = Eth2PS_GetIndCnf(0, &hFrame, &tFrameInfo);
	if( tHalResult==ETH2PS_ERR_FIFO_EMPTY )
	{
		/* No packet is waiting. */
	}
	else if( tHalResult==ETH2PS_OKAY )
	{
		/* Is this an indication or confirmation? */
		if( tFrameInfo.fCnf==true )
		{
			/* This is a confirmation.
			 * Release the frame.
			 */
			Eth2PS_ReleaseFrame(&hFrame);
		}
		/* Was this packet received on port 0? */
		else if( tFrameInfo.uPortNo==0 )
		{
			/* Does a callback handler exist? */
			if( pfnPort0!=NULL )
			{
				pfnPort0(pvNetworkDriver0, hFrame.pbData, (void*)hFrame.ulFifoEntry, hFrame.usLength);
			}
			else
			{
				/* No handler -> release the frame. */
				Eth2PS_ReleaseFrame(&hFrame);
			}
		}
		/* Was this packet received on port 1? */
		else if( tFrameInfo.uPortNo==1 )
		{
			/* Does a callback handler exist? */
			if( pfnPort1!=NULL )
			{
				pfnPort1(pvNetworkDriver1, hFrame.pbData, (void*)hFrame.ulFifoEntry, hFrame.usLength);
			}
			else
			{
				/* No handler -> release the frame. */
				Eth2PS_ReleaseFrame(&hFrame);
			}
		}
		else
		{
			/* Unknown port -> release the frame. */
			Eth2PS_ReleaseFrame(&hFrame);
		}
	}
}


int hal_eth2ps_get_statistics(unsigned int uiPort, void *pvBuffer, unsigned int sizBuffer)
{
	int iResult;
	ETH2PS_RESULT_E tResult;
	unsigned int sizChunk;
	ETH2PS_COUNTERS_T tCounters;


	iResult = -1;

	tResult = Eth2PS_GetCounters(uiPort, &tCounters);
	if( tResult==ETH2PS_OKAY )
	{
		/* Does the destination buffer exist? */
		if( pvBuffer!=NULL && sizBuffer>0 )
		{
			/* Limit the size of the data to copy to the available counter data. */
			sizChunk = sizBuffer;
			if( sizChunk>sizeof(ETH2PS_COUNTERS_T) )
			{
				sizChunk = sizeof(ETH2PS_COUNTERS_T);
			}
			memcpy(pvBuffer, &tCounters, sizChunk);
		}
		iResult = 0;
	}

	return iResult;
}
