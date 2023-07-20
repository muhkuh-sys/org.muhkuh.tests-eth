#include "board.h"

#include "netx_io_areas.h"
#include "pad_control.h"


typedef enum MLED_CTRL_SEL_ENUM
{
	MLED_CTRL_SEL_ALWAYS_OFF         =  0U,
	MLED_CTRL_SEL_LINE_REGISTER      =  1U,
	MLED_CTRL_SEL_MLED_CTRL_BLINK    =  2U,
	MLED_CTRL_SEL_XM0_IO0            =  3U,
	MLED_CTRL_SEL_XM0_IO1            =  4U,
	MLED_CTRL_SEL_XM0_IO2            =  5U,
	MLED_CTRL_SEL_XM0_IO3            =  6U,
	MLED_CTRL_SEL_XM0_IO4            =  7U,
	MLED_CTRL_SEL_XM0_IO5            =  8U,
	MLED_CTRL_SEL_XM1_IO0            =  9U,
	MLED_CTRL_SEL_XM1_IO1            = 10U,
	MLED_CTRL_SEL_XM1_IO2            = 11U,
	MLED_CTRL_SEL_XM1_IO3            = 12U,
	MLED_CTRL_SEL_XM1_IO4            = 13U,
	MLED_CTRL_SEL_XM1_IO5            = 14U,
	MLED_CTRL_SEL_PHY_CTRL0_LED0     = 15U,
	MLED_CTRL_SEL_PHY_CTRL0_LED1     = 16U,
	MLED_CTRL_SEL_PHY_CTRL1_LED0     = 17U,
	MLED_CTRL_SEL_PHY_CTRL1_LED1     = 18U,
	MLED_CTRL_SEL_BLINK0             = 19U,
	MLED_CTRL_SEL_BLINK1             = 20U,
	MLED_CTRL_SEL_BLINK2             = 21U,
	MLED_CTRL_SEL_BLINK3             = 22U,
	MLED_CTRL_SEL_PASSTHROUGH        = 31U
} MLED_CTRL_SEL_T;


static const unsigned char aucPadCtrlIndexBoardInit[] =
{
	PAD_AREG2OFFSET(mled, 0),
	PAD_AREG2OFFSET(mled, 1),
	PAD_AREG2OFFSET(mled, 2),
	PAD_AREG2OFFSET(mled, 3)
};

static const unsigned char aucPadCtrlValueBoardInit[] =
{
	PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),
	PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),
	PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable),
	PAD_CONFIGURATION(PAD_DRIVING_STRENGTH_High, PAD_PULL_Disable, PAD_INPUT_Disable)
};


int board_initialize(void)
{
	HOSTDEF(ptMledCtrlComArea);
	unsigned long ulValue;


	/* Stop the MLED controller. */
	ptMledCtrlComArea->ulMled_ctrl_cfg = 0U;

	/* Setup the MLED pins. */
	ptMledCtrlComArea->aulMled_ctrl_output_sel[4] = MLED_CTRL_SEL_PHY_CTRL0_LED0 << HOSTSRT(mled_ctrl_app_output_sel0_sel);
	ptMledCtrlComArea->aulMled_ctrl_output_sel[5] = MLED_CTRL_SEL_PHY_CTRL0_LED1 << HOSTSRT(mled_ctrl_app_output_sel0_sel);
	ptMledCtrlComArea->aulMled_ctrl_output_sel[6] = MLED_CTRL_SEL_PHY_CTRL1_LED0 << HOSTSRT(mled_ctrl_app_output_sel0_sel);
	ptMledCtrlComArea->aulMled_ctrl_output_sel[7] = MLED_CTRL_SEL_PHY_CTRL1_LED1 << HOSTSRT(mled_ctrl_app_output_sel0_sel);

	ptMledCtrlComArea->aulMled_ctrl_output_on_time[4] = 0xffU;
	ptMledCtrlComArea->aulMled_ctrl_output_on_time[5] = 0xffU;
	ptMledCtrlComArea->aulMled_ctrl_output_on_time[6] = 0xffU;
	ptMledCtrlComArea->aulMled_ctrl_output_on_time[7] = 0xffU;

	/* Start the MLED controller. */
	ulValue  = HOSTMSK(mled_ctrl_cfg_enable);
	ulValue |= 3U << HOSTSRT(mled_ctrl_cfg_prescale_counter_max);
	ulValue |= 0U << HOSTSRT(mled_ctrl_cfg_blink_counter_max);
	ptMledCtrlComArea->ulMled_ctrl_cfg = ulValue;

	/* Setup the padctrl. */
	pad_control_apply(aucPadCtrlIndexBoardInit, aucPadCtrlValueBoardInit, sizeof(aucPadCtrlValueBoardInit)/sizeof(aucPadCtrlValueBoardInit[0]));

	return 0;
}
