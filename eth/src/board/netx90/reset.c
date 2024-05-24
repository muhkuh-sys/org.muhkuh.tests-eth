#include "board.h"
#include "netx_io_areas.h"


void board_reset(void)
{
	HOSTDEF(ptAsicCtrlArea);
	HOSTDEF(ptAsicCtrlComArea);
	unsigned long ulValue;


	ulValue  = ptAsicCtrlComArea->ulReset_ctrl;
	ulValue |= HOSTMSK(reset_ctrl_RES_REQ_FIRMWARE);
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;
	ptAsicCtrlComArea->ulReset_ctrl = ulValue;

	while(1) {};
}
