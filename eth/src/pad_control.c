
#include "pad_control.h"

#include "netx_io_areas.h"
#include "options.h"


static PAD_CTRL_VALUES_T tPadCtrlDefaults;

void pad_control_init(void)
{
	volatile unsigned long *pulPadControl;
	unsigned char *pucCnt;
	unsigned char *pucEnd;


#if ASIC_TYP==ASIC_TYP_NETX90_MPW
	pulPadControl = (volatile unsigned long*)HOSTADDR(pad_ctrl);
#elif ASIC_TYP==ASIC_TYP_NETX90_FULL
	pulPadControl = (volatile unsigned long*)HOSTADDR(pad_ctrl_com);
#else
#       error "Unsupported ASIC_TYP!"
#endif

	/* Copy all registers to the local array. */
	pucCnt = tPadCtrlDefaults.aucValues;
	pucEnd = pucCnt + NUMBER_OF_PAD_CTRL;
	while( pucCnt<pucEnd )
	{
		*(pucCnt++) = (unsigned char)(*(pulPadControl++));
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
