#include "hal_ethmac_wrapper.h"

#include <stdbool.h>
#include <string.h>

#include "HETHMAC/ARM_Application/Components/ip_stack/Includes/phy.h"
#include "HETHMAC/ARM_Application/Targets/Includes/eth_mac.h"

#define ETHERNET_MINIMUM_FRAMELENGTH 60

/*-------------------------------------------------------------------------*/

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


void hal_ethmac_get_received_packet(unsigned int uiPort, void *pvNetworkDriver, PFN_HAL_HANDLE_RECEIVED_PACKET pfnReceiveHandler)
{
	ETHERNET_RESULT tHalResult;
	uint32_t ulFillLevel;
	ETHERNET_FRAME_T *ptFrame;
	void *phFrame;
	uint32_t ulLength;


	/* Get the number of received packets. */
	tHalResult = EthMac_GetRecvFillLevel(uiPort, 0, &ulFillLevel);
	if( tHalResult==ETH_OKAY )
	{
		/* Is at least one packet waiting? */
		if( ulFillLevel!=0 )
		{
			/* Receive the packet from the low priority FIFO. */
			tHalResult = EthMac_Recv(uiPort, &ptFrame, &phFrame, &ulLength, 0);
			if( tHalResult==ETH_OKAY )
			{
				pfnReceiveHandler(pvNetworkDriver, ptFrame, phFrame, ulLength);
			}
		}
	}
}
