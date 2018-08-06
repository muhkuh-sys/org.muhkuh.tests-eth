
#include "pad_control.h"

#include "netx_io_areas.h"
#include "options.h"


static PAD_CTRL_VALUES_T tPadCtrlDefaults;

void pad_control_init(void)
{
	unsigned int sizCfgCnt;
	unsigned int sizCfgEnd;
	unsigned long ulValue;
	unsigned char ucValue;
	volatile unsigned long *pulPadControl;


#if ASIC_TYP==ASIC_TYP_NETX90_MPW
	pulPadControl = (volatile unsigned long*)HOSTADDR(pad_ctrl);
#elif ASIC_TYP==ASIC_TYP_NETX90_FULL
	pulPadControl = (volatile unsigned long*)HOSTADDR(pad_ctrl_com);
#else
#       error "Unsupported ASIC_TYP!"
#endif

	/* Copy all registers to the local array. */
	sizCfgCnt = 0;
	sizCfgEnd = sizeof(tPadCtrlDefaults.aucValues) / sizeof(tPadCtrlDefaults.aucValues[0]);
	while( sizCfgCnt<sizCfgEnd )
	{
		ulValue = pulPadControl[sizCfgCnt];
		ucValue = (unsigned char)(ulValue & 0xff);
		tPadCtrlDefaults.aucValues[sizCfgCnt] = ucValue;
		++sizCfgCnt;
	}
}



void pad_control_apply(const unsigned char *pucIndex, const unsigned char *pucConfiguration, unsigned int sizConfiguration)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned int sizCnt;
	unsigned long ulConfiguration;
	unsigned long ulOffset;
	volatile unsigned long *pulPadControl;


#if ASIC_TYP==ASIC_TYP_NETX90_MPW
	pulPadControl = (volatile unsigned long*)HOSTADDR(pad_ctrl);
#elif ASIC_TYP==ASIC_TYP_NETX90_FULL
	pulPadControl = (volatile unsigned long*)HOSTADDR(pad_ctrl_com);
#else
#       error "Unsupported ASIC_TYP!"
#endif

	for(sizCnt=0; sizCnt<sizConfiguration; ++sizCnt)
	{
		/* Get the value. */
		ulOffset = (unsigned long)(pucIndex[sizCnt]);
		ulConfiguration = (unsigned long)(pucConfiguration[sizCnt]);

		if( ulConfiguration!=PAD_CONTROL_SKIP && ulOffset!=PAD_CONTROL_SKIP )
		{
			/* Write the configuration. */
			ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
			pulPadControl[ulOffset] = ulConfiguration;
		}
	}
}



void pad_control_restore(const unsigned char *pucIndex, unsigned int sizConfiguration)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned int sizCnt;
	unsigned long ulConfiguration;
	unsigned long ulOffset;
	volatile unsigned long *pulPadControl;


#if ASIC_TYP==ASIC_TYP_NETX90_MPW
	pulPadControl = (volatile unsigned long*)HOSTADDR(pad_ctrl);
#elif ASIC_TYP==ASIC_TYP_NETX90_FULL
	pulPadControl = (volatile unsigned long*)HOSTADDR(pad_ctrl_com);
#else
#       error "Unsupported ASIC_TYP!"
#endif

	for(sizCnt=0; sizCnt<sizConfiguration; ++sizCnt)
	{
		/* Get the value. */
		ulOffset = (unsigned long)(pucIndex[sizCnt]);
		ulConfiguration = (unsigned long)(tPadCtrlDefaults.aucValues[ulOffset]);

		if( ulConfiguration!=PAD_CONTROL_SKIP && ulOffset!=PAD_CONTROL_SKIP )
		{
			/* Write the configuration. */
			ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
			pulPadControl[ulOffset] = ulConfiguration;
		}
	}
}
