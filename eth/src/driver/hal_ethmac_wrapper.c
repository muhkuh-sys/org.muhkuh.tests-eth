#include "hal_ethmac_wrapper.h"

#include <string.h>

#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_miimu.h"
#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_pfifo.h"
#include "HETHMAC/ARM_Application/Components/hal_common/Includes/hal_xc.h"
#include "HETHMAC/ARM_Application/Targets/Includes/eth_mac.h"
#include "HETHMAC/ARM_Application/Components/ip_stack/Includes/phy.h"


#define ETHERNET_MINIMUM_FRAMELENGTH 60

/*-------------------------------------------------------------------------*/


#define MIIMU_READ(phy,reg,data)  MIIMU_ReadPhyReg(MIIMU_SEND_PREAMBLE, MIIMU_MDCFREQ_HIGH, MIIMU_RTA_1_BITS, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) MIIMU_WritePhyReg(MIIMU_SEND_PREAMBLE, MIIMU_MDCFREQ_HIGH, MIIMU_RTA_1_BITS, phy, reg, data, NULL)


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


void hal_ethmac_phy_init(void)
{
	phy_init(0,  0, PHYID_OMNIPHY, &s_tPhyOps);
	phy_init(1,  1, PHYID_OMNIPHY, &s_tPhyOps);
}


/*-------------------------------------------------------------------------*/


void hal_ethmac_pfifo_reset(void)
{
#if ASIC_TYP==ASIC_TYP_NETX90
	PFIFO_Reset(0);
#else
#       error "The ASIC type is not supported!"
#endif
}



int hal_ethmac_xc_reset(unsigned int uiPort)
{
	int iResult;
#if ASIC_TYP==ASIC_TYP_NETX90
	unsigned int uiXc;
	unsigned int uiXpec;
#endif


#if ASIC_TYP==ASIC_TYP_NETX90
	/* Get the XC and XPEC index from the port. */
	uiXc  = uiPort >> 1U;
	uiXpec = uiPort & 1U;
	iResult = XC_Reset(uiXc, uiXpec, NULL);
#else
#       error "The ASIC type is not supported!"
#endif

	return iResult;
}


int hal_ethmac_xc_init(unsigned int uiPort, const uint8_t *pucMAC)
{
	int iResult;
	int iHalResult;
	unsigned int uiMiiCfg;
	const unsigned long ulActivityLedFlashPeriod =  5000000; /* 50 milliseconds */


	/* Be pessimistic. */
	iResult = -1;

	/* NOTE: This should be 1 for EXTPHY. */
	uiMiiCfg = 0;

	iHalResult = EthMac_Initialize(uiPort, ETH_PHY_LED_BLINK, ulActivityLedFlashPeriod, NULL);
	if( iHalResult==ETH_OKAY )
	{
		iHalResult = EthMac_CfgMii(uiPort, uiMiiCfg);
		if( iHalResult==ETH_OKAY )
		{
			iHalResult = EthMac_SetMacAddr(uiPort, ETH_MAC_ADDRESS_CHASSIS, pucMAC);
			if( iHalResult==ETH_OKAY )
			{
				iHalResult = EthMac_SetIrqMask(uiPort, 0);
				if( iHalResult==ETH_OKAY )
				{
					iHalResult = EthMac_Start(uiPort, NULL);
					if( iHalResult==ETH_OKAY )
					{
						/* All OK. */
						iResult = 0;
					}
				}
			}
		}
	}

	return iResult;
}



int hal_ethmac_get_link_state(unsigned int uiPort, unsigned int *puiLinkState, unsigned int *puiSpeed, unsigned int *puiIsFullDuplex)
{
	int iResult;
	int iHalResult;
	ETH_MAU_TYPE_T tState;


	/* Be pessimistic. */
	iResult = -1;

	/* get current link mode */
	iHalResult = phy_get_linkstate(uiPort, &tState);
	if( iHalResult==0 )
	{
		/* Update link mode in MAC, please note that link mode must also updated in case of link down */
		EthMac_SetLinkMode(uiPort, true, tState.uSpeed, tState.uIsFullDuplex);

		if( puiLinkState!=NULL )
		{
			*puiLinkState = tState.uIsLinkUp;
		}
		if( puiSpeed!=NULL )
		{
			*puiSpeed = tState.uSpeed;
		}
		if( puiIsFullDuplex!=NULL )
		{
			*puiIsFullDuplex = tState.uIsFullDuplex;
		}

		iResult = 0;
	}

	return iResult;
}



int hal_ethmac_get_empty_packet(unsigned int uiPort, void **ppvPacket, void **pphPacket)
{
	ETHERNET_RESULT tHalResult;
	ETHERNET_FRAME_T *ptFrame;
	int iResult;


	iResult = -1;

	tHalResult = EthMac_GetFrame(uiPort, &ptFrame, pphPacket);
	if( tHalResult==ETH_OKAY )
	{
		*ppvPacket = ptFrame;
		iResult = 0;
	}
	return iResult;
}


int hal_ethmac_release_packet(unsigned int uiPort, void *pvPacket __attribute__ ((unused)), void *phPacket)
{
	int iResult;
	ETHERNET_RESULT tHalResult;


	iResult = -1;

	tHalResult = EthMac_ReleaseFrame(uiPort, phPacket);
	if( tHalResult==ETH_OKAY )
	{
		iResult = 0;
	}

	return iResult;
}


int hal_ethmac_send_packet(unsigned int uiPort, void *pvPacket __attribute__ ((unused)), void *phPacket, unsigned int uiPacketSize)
{
	int iResult;
	ETHERNET_RESULT tHalResult;


	iResult = -1;

	/* Assure a minimum packet size or the HAL will throw an error. */
	if( uiPacketSize<ETHERNET_MINIMUM_FRAMELENGTH )
	{
		uiPacketSize = ETHERNET_MINIMUM_FRAMELENGTH;
	}

	/* Send the packet. */
	tHalResult = EthMac_SendWithoutCnf(uiPort, phPacket, uiPacketSize, 0);
	if( tHalResult==ETH_OKAY )
	{
		iResult = 0;
	}

	return iResult;
}


int hal_ethmac_get_received_packet(unsigned int uiPort, void **ppvPacket, void **pphPacket, unsigned int *puiPacketSize)
{
	int iResult;
	ETHERNET_RESULT tHalResult;
	uint32_t ulFillLevel;
	ETHERNET_FRAME_T *ptFrame;
	uint32_t ulLength;


	iResult = -1;

	/* Get the number of received packets. */
	tHalResult = EthMac_GetRecvFillLevel(uiPort, 0, &ulFillLevel);
	if( tHalResult==ETH_OKAY )
	{
		/* Is at least one packet waiting? */
		if( ulFillLevel!=0 )
		{
			/* Receive the packet. */
			tHalResult = EthMac_Recv(uiPort, &ptFrame, pphPacket, &ulLength, 0);
			if( tHalResult==ETH_OKAY )
			{
				*ppvPacket = ptFrame;
				*puiPacketSize = ulLength;

				iResult = 0;
			}
		}
	}

	return iResult;
}
