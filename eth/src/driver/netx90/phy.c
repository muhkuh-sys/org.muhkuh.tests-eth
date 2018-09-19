#include "phy.h"

#include "netx_io_areas.h"
#include "phy_std_regs.h"
#include "systime.h"
#include "tools.h"


typedef int (*PFN_PHY_SETUP_FUNCTION)(unsigned long ulR0, unsigned long ulR1, unsigned long ulR2, unsigned long ulR3);



typedef struct PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_STRUCT
{
	unsigned char  ucOffset;
	unsigned long  ulValue;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T;



typedef struct PHY_SETUP_COMMAND_DATA_WRITE_ABS_STRUCT
{
	unsigned long  ulAddress;
	unsigned long  ulValue;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_WRITE_ABS_T;



typedef struct PHY_SETUP_COMMAND_DATA_POLL_OFFSET_STRUCT
{
	unsigned char  ucOffset;
	unsigned long  ulMask;
	unsigned long  ulValue;
	unsigned long  ulTimeoutMs;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T;



typedef struct PHY_SETUP_COMMAND_DATA_POLL_ABS_STRUCT
{
	unsigned long  ulAddress;
	unsigned long  ulMask;
	unsigned long  ulValue;
	unsigned long  ulTimeoutMs;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_POLL_ABS_T;



typedef struct PHY_SETUP_COMMAND_DATA_MII_STRUCT
{
	unsigned long  ulValue;
	unsigned long  ulTimeoutMs;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_MII_T;



typedef struct PHY_SETUP_COMMAND_DATA_DELAY_STRUCT
{
	unsigned long  ulTimeoutMs;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_DELAY_T;



typedef struct PHY_SETUP_COMMAND_DATA_EXECUTE_STRUCT
{
	unsigned long  ulAddress;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_EXECUTE_T;



typedef struct PHY_SETUP_COMMAND_DATA_EXECUTE01_STRUCT
{
	unsigned long  ulAddress;
	unsigned long  ulR0;
	unsigned long  ulR1;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_EXECUTE01_T;



typedef struct PHY_SETUP_COMMAND_DATA_EXECUTE0123_STRUCT
{
	unsigned long  ulAddress;
	unsigned long  ulR0;
	unsigned long  ulR1;
	unsigned long  ulR2;
	unsigned long  ulR3;
} __attribute__((packed)) PHY_SETUP_COMMAND_DATA_EXECUTE0123_T;



typedef union PHY_SETUP_COMMAND_DATA_UNION
{
	const unsigned char *puc;
	PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T *ptWriteOffset;
	PHY_SETUP_COMMAND_DATA_WRITE_ABS_T *ptWriteAbs;
	PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T *ptPollOffset;
	PHY_SETUP_COMMAND_DATA_POLL_ABS_T *ptPollAbs;
	PHY_SETUP_COMMAND_DATA_MII_T *ptMii;
	PHY_SETUP_COMMAND_DATA_DELAY_T *ptDelay;
	PHY_SETUP_COMMAND_DATA_EXECUTE_T *ptExecute;
	PHY_SETUP_COMMAND_DATA_EXECUTE01_T *ptExecute01;
	PHY_SETUP_COMMAND_DATA_EXECUTE0123_T *ptExecute0123;
} PHY_SETUP_COMMAND_DATA_T;



static void phy_setup_command_write_register(unsigned long ulAddress, unsigned long ulValue)
{
	HOSTDEF(ptAsicCtrlArea);
	VADR_T tVAdr;


	if( (ulAddress&0x00000001U)!=0U )
	{
		ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	}

	tVAdr.ul = (ulAddress & 0xfffffffcU);
	*(tVAdr.pul) = ulValue;
}



static int phy_setup_command_poll_register(unsigned long ulAddress, unsigned long ulMask, unsigned long ulExpectedValue, unsigned long ulTimeoutMs)
{
	VADR_T tVAdr;
	unsigned long ulValue;
	int iIsElapsed;
	TIMER_HANDLE_T tTimer;


	tVAdr.ul = ulAddress;
	iIsElapsed = 0;
	systime_handle_start_ms(&tTimer, ulTimeoutMs);
	do
	{
		ulValue  = *(tVAdr.pul);
		ulValue &= ulMask;
		if( ulValue==ulExpectedValue )
		{
			break;
		}
		iIsElapsed = systime_handle_is_elapsed(&tTimer);
	} while( iIsElapsed==0 );

	return iIsElapsed;
}



static int phy_setup_command_mii_eth_system(unsigned long ulMiiValue, unsigned long ulTimeoutMs, unsigned long *pulResult)
{
#if ASIC_TYP==ASIC_TYP_NETX90_MPW
	HOSTDEF(ptMiimuArea);
	unsigned long ulValue;
	int iIsElapsed;
	TIMER_HANDLE_T tTimer;


	ptMiimuArea->ulMiimu = ulMiiValue;
	iIsElapsed = 0;
	systime_handle_start_ms(&tTimer, ulTimeoutMs);
	do
	{
		ulValue = ptMiimuArea->ulMiimu;
		if( (ulValue&HOSTMSK(miimu_snrdy))==0 )
		{
			if( pulResult!=NULL )
			{
				*pulResult = (ulValue&HOSTMSK(miimu_data))>>HOSTSRT(miimu_data);
			}
			break;
		}
		iIsElapsed = systime_handle_is_elapsed(&tTimer);
	} while( iIsElapsed==0 );

	return iIsElapsed;
#elif ASIC_TYP==ASIC_TYP_NETX90
	HOSTDEF(ptEthArea);
	unsigned long ulValue;
	int iIsElapsed;
	TIMER_HANDLE_T tTimer;


	ptEthArea->ulEth_miimu = ulMiiValue;
	iIsElapsed = 0;
	systime_handle_start_ms(&tTimer, ulTimeoutMs);
	do
	{
		ulValue = ptEthArea->ulEth_miimu;
		if( (ulValue&HOSTMSK(eth_miimu_snrdy))==0 )
		{
			if( pulResult!=NULL )
			{
				*pulResult = (ulValue&HOSTMSK(eth_miimu_data))>>HOSTSRT(eth_miimu_data);
			}
			break;
		}
		iIsElapsed = systime_handle_is_elapsed(&tTimer);
	} while( iIsElapsed==0 );

	return iIsElapsed;
#else
#       error "Unsupported ASIC_TYP!"
#endif
}



static int phy_setup_command_mii_xc0_phy_ctrl0(unsigned long ulMiiValue, unsigned long ulTimeoutMs, unsigned long *pulResult)
{
	HOSTDEF(ptXc0PhyCtrl0Area);
	unsigned long ulValue;
	int iIsElapsed;
	TIMER_HANDLE_T tTimer;


	ptXc0PhyCtrl0Area->ulInt_phy_ctrl_miimu = ulMiiValue;
	iIsElapsed = 0;
	systime_handle_start_ms(&tTimer, ulTimeoutMs);
	do
	{
		ulValue = ptXc0PhyCtrl0Area->ulInt_phy_ctrl_miimu;
		if( (ulValue&HOSTMSK(int_phy_ctrl_miimu_snrdy))==0 )
		{
			if( pulResult!=NULL )
			{
				*pulResult = (ulValue&HOSTMSK(int_phy_ctrl_miimu_data))>>HOSTSRT(int_phy_ctrl_miimu_data);
			}
			break;
		}
		iIsElapsed = systime_handle_is_elapsed(&tTimer);
	} while( iIsElapsed==0 );

	return iIsElapsed;
}



static int phy_setup_command_mii_xc0_phy_ctrl1(unsigned long ulMiiValue, unsigned long ulTimeoutMs, unsigned long *pulResult)
{
	HOSTDEF(ptXc0PhyCtrl1Area);
	unsigned long ulValue;
	int iIsElapsed;
	TIMER_HANDLE_T tTimer;


	ptXc0PhyCtrl1Area->ulInt_phy_ctrl_miimu = ulMiiValue;
	iIsElapsed = 0;
	systime_handle_start_ms(&tTimer, ulTimeoutMs);
	do
	{
		ulValue = ptXc0PhyCtrl1Area->ulInt_phy_ctrl_miimu;
		if( (ulValue&HOSTMSK(int_phy_ctrl_miimu_snrdy))==0 )
		{
			if( pulResult!=NULL )
			{
				*pulResult = (ulValue&HOSTMSK(int_phy_ctrl_miimu_data))>>HOSTSRT(int_phy_ctrl_miimu_data);
			}
			break;
		}
		iIsElapsed = systime_handle_is_elapsed(&tTimer);
	} while( iIsElapsed==0 );

	return iIsElapsed;
}



int phy_setup_execute_sequence(const unsigned char *pucSequence, unsigned int sizSequence)
{
	int iResult;
	const unsigned char *pucCnt;
	const unsigned char *pucEnd;
	PHY_SETUP_COMMAND_T tCmd;
	unsigned int uiDataLeft;
	PHY_SETUP_COMMAND_DATA_T tCmdData;
	unsigned long ulAddress;
	PFN_PHY_SETUP_FUNCTION pfnSetup;


	/* An empty script is no error. */
	iResult = 0;

	pucCnt = pucSequence;
	pucEnd = pucSequence + sizSequence;
	while( pucCnt<pucEnd )
	{
		/* Get the next command. */
		tCmd = (PHY_SETUP_COMMAND_T)*(pucCnt++);

		/* Get the number of remaining data bytes. */
		uiDataLeft = (unsigned int)(pucEnd-pucCnt);

		/* Set the pointer of the data structure. */
		tCmdData.puc = pucCnt;

		iResult = -1;
		switch(tCmd)
		{
		case PHY_SETUP_COMMAND_End:
			/* Set the program counter to the end. */
			pucCnt = pucEnd;

			iResult = 0;
			break;

		case PHY_SETUP_COMMAND_WriteIntPhyCfg:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T) )
			{
				/* Add the offset to the module address. */
				ulAddress  = HOSTADDR(int_phy_cfg_com);
				ulAddress += tCmdData.ptWriteOffset->ucOffset * sizeof(unsigned long);
				phy_setup_command_write_register(ulAddress, tCmdData.ptWriteOffset->ulValue);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T);

				iResult = 0;
			}
			break;

		case PHY_SETUP_COMMAND_WriteXc0PhyCtrl0:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T) )
			{
				/* Add the offset to the module address. */
				ulAddress  = HOSTADDR(xc0_phy_ctrl0);
				ulAddress += tCmdData.ptWriteOffset->ucOffset * sizeof(unsigned long);
				phy_setup_command_write_register(ulAddress, tCmdData.ptWriteOffset->ulValue);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T);

				iResult = 0;
			}
			break;

		case PHY_SETUP_COMMAND_WriteXc0PhyCtrl1:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T) )
			{
				/* Add the offset to the module address. */
				ulAddress  = HOSTADDR(xc0_phy_ctrl1);
				ulAddress += tCmdData.ptWriteOffset->ucOffset * sizeof(unsigned long);
				phy_setup_command_write_register(ulAddress, tCmdData.ptWriteOffset->ulValue);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T);

				iResult = 0;
			}
			break;

		case PHY_SETUP_COMMAND_WriteEthSystem:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T) )
			{
				/* Add the offset to the module address. */
				ulAddress  = HOSTADDR(eth_system);
				ulAddress += tCmdData.ptWriteOffset->ucOffset * sizeof(unsigned long);
				phy_setup_command_write_register(ulAddress, tCmdData.ptWriteOffset->ulValue);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_WRITE_OFFSET_T);

				iResult = 0;
			}
			break;

		case PHY_SETUP_COMMAND_WriteAbs:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_WRITE_ABS_T) )
			{
				/* Add the offset to the module address. */
				phy_setup_command_write_register(tCmdData.ptWriteAbs->ulAddress, tCmdData.ptWriteAbs->ulValue);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_WRITE_ABS_T);

				iResult = 0;
			}
			break;

		case PHY_SETUP_COMMAND_PollIntPhyCfg:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T) )
			{
				/* Add the offset to the module address. */
				ulAddress  = HOSTADDR(int_phy_cfg_com);
				ulAddress += tCmdData.ptPollOffset->ucOffset * sizeof(unsigned long);
				iResult = phy_setup_command_poll_register(ulAddress, tCmdData.ptPollOffset->ulMask, tCmdData.ptPollOffset->ulValue, tCmdData.ptPollOffset->ulTimeoutMs);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T);
			}
			break;

		case PHY_SETUP_COMMAND_PollXc0PhyCtrl0:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T) )
			{
				/* Add the offset to the module address. */
				ulAddress  = HOSTADDR(xc0_phy_ctrl0);
				ulAddress += tCmdData.ptPollOffset->ucOffset * sizeof(unsigned long);
				iResult = phy_setup_command_poll_register(ulAddress, tCmdData.ptPollOffset->ulMask, tCmdData.ptPollOffset->ulValue, tCmdData.ptPollOffset->ulTimeoutMs);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T);
			}
			break;

		case PHY_SETUP_COMMAND_PollXc0PhyCtrl1:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T) )
			{
				/* Add the offset to the module address. */
				ulAddress  = HOSTADDR(xc0_phy_ctrl1);
				ulAddress += tCmdData.ptPollOffset->ucOffset * sizeof(unsigned long);
				iResult = phy_setup_command_poll_register(ulAddress, tCmdData.ptPollOffset->ulMask, tCmdData.ptPollOffset->ulValue, tCmdData.ptPollOffset->ulTimeoutMs);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T);
			}
			break;

		case PHY_SETUP_COMMAND_PollEthSystem:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T) )
			{
				/* Add the offset to the module address. */
				ulAddress  = HOSTADDR(eth_system);
				ulAddress += tCmdData.ptPollOffset->ucOffset * sizeof(unsigned long);
				iResult = phy_setup_command_poll_register(ulAddress, tCmdData.ptPollOffset->ulMask, tCmdData.ptPollOffset->ulValue, tCmdData.ptPollOffset->ulTimeoutMs);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_POLL_OFFSET_T);
			}
			break;

		case PHY_SETUP_COMMAND_PollAbs:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_POLL_ABS_T) )
			{
				iResult = phy_setup_command_poll_register(tCmdData.ptPollAbs->ulAddress, tCmdData.ptPollAbs->ulMask, tCmdData.ptPollAbs->ulValue, tCmdData.ptPollAbs->ulTimeoutMs);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_POLL_ABS_T);
			}
			break;

		case PHY_SETUP_COMMAND_MiiEthSystem:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_MII_T) )
			{
				iResult = phy_setup_command_mii_eth_system(tCmdData.ptMii->ulValue, tCmdData.ptMii->ulTimeoutMs, NULL);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_MII_T);
			}
			break;

		case PHY_SETUP_COMMAND_MiiXc0PhyCtrl0:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_MII_T) )
			{
				iResult = phy_setup_command_mii_xc0_phy_ctrl0(tCmdData.ptMii->ulValue, tCmdData.ptMii->ulTimeoutMs, NULL);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_MII_T);
			}
			break;

		case PHY_SETUP_COMMAND_MiiXc0PhyCtrl1:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_MII_T) )
			{
				iResult = phy_setup_command_mii_xc0_phy_ctrl1(tCmdData.ptMii->ulValue, tCmdData.ptMii->ulTimeoutMs, NULL);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_MII_T);
			}
			break;

		case PHY_SETUP_COMMAND_Delay:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_DELAY_T) )
			{
				systime_delay_ms(tCmdData.ptDelay->ulTimeoutMs);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_DELAY_T);

				iResult = 0;
			}
			break;

		case PHY_SETUP_COMMAND_Execute:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_EXECUTE_T) )
			{
				pfnSetup = (PFN_PHY_SETUP_FUNCTION)tCmdData.ptExecute->ulAddress;
				iResult = pfnSetup(0, 0, 0, 0);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_EXECUTE_T);
			}
			break;

		case PHY_SETUP_COMMAND_Execute01:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_EXECUTE01_T) )
			{
				pfnSetup = (PFN_PHY_SETUP_FUNCTION)tCmdData.ptExecute01->ulAddress;
				iResult = pfnSetup(tCmdData.ptExecute01->ulR0, tCmdData.ptExecute01->ulR1, 0, 0);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_EXECUTE01_T);
			}
			break;

		case PHY_SETUP_COMMAND_Execute0123:
			if( uiDataLeft>=sizeof(PHY_SETUP_COMMAND_DATA_EXECUTE0123_T) )
			{
				pfnSetup = (PFN_PHY_SETUP_FUNCTION)tCmdData.ptExecute0123->ulAddress;
				iResult = pfnSetup(tCmdData.ptExecute0123->ulR0, tCmdData.ptExecute0123->ulR1, tCmdData.ptExecute0123->ulR2, tCmdData.ptExecute0123->ulR3);

				pucCnt += sizeof(PHY_SETUP_COMMAND_DATA_EXECUTE0123_T);
			}
			break;
		}

		/* Do not process the rest of the macro if an error occurred. */
		if( iResult!=0 )
		{
			break;
		}
	}

	return iResult;
}
