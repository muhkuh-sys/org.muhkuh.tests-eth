#include "board.h"
#include "netx_io_areas.h"


void board_reset(void)
{
	HOSTDEF(ptRAPSysctrlArea);


	ptRAPSysctrlArea->ulRAP_SYSCTRL_RSTMASK |= HOSTMSK(RAP_SYSCTRL_RSTMASK_SWRST_MASK);
	ptRAPSysctrlArea->ulRAP_SYSCTRL_RSTCTRL = HOSTMSK(RAP_SYSCTRL_RSTCTRL_SWRST_REQ);

	while(1) {};
}
