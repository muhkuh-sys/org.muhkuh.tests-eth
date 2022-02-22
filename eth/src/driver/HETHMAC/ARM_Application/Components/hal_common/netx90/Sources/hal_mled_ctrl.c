/* Includes */
#include "hal_mled_ctrl.h"
#include "hw_defines.h"

#if defined(__NETX90)
#define HAL_MLED_CTRL_OUT_CNT 8
#endif // defined(__NETX90)

/*  Variables */
HW_PTR_MLED_CTRL(s_ptMledCtrl)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

/*****************************************************************************/
/*! MLEDCTRL Configure Channel
* \description
*   This function configures a MLEDCTRL channel.
* \class
*   MLEDCTRL
* \params
*   uiInst       [in] instance
*   uiOutput     [in] LED number
*   eOutputSel   [in] signal selector
*   fInvertInput [in] invert output signal
*   uiBrightness [in] brightness 0 (off) .. 255 (maximum)
* \return
*   0
*  -1                                                                        */
/*****************************************************************************/
int HALDEF(MLEDCTRL_ConfigureOutput)(unsigned int uiInst,
                                     unsigned int uiOutput,
                                     MLED_CTRL_SEL_E eOutputSel,
                                     bool fInvertInput,
                                     unsigned int uiBrightness)
{
  uint32_t ulVal;

  if(uiOutput >= HAL_MLED_CTRL_OUT_CNT)
    return -1;

  /* set output select */
  ulVal = (MLED_CTRL_SEL_PASSTHROUGH == eOutputSel) ?  HW_MSK(mled_ctrl_output_sel0_sel) : (uint32_t)eOutputSel << HW_SRT(mled_ctrl_app_output_sel0_sel);

  /* include Invert flag */
  if(fInvertInput) ulVal |= HW_MSK(mled_ctrl_output_sel0_inv);

  s_ptMledCtrl->aulMled_ctrl_output_sel[uiOutput] = ulVal;
  s_ptMledCtrl->aulMled_ctrl_output_on_time[uiOutput] = uiBrightness;

  return 0;
}

/*****************************************************************************/
/*! MLEDCTRL Start
* \description
*   This function starts/stops MLEDCTRL channels.
* \class 
*   MLEDCTRL
* \params
*   uiInst                [in] instance
*   uiPrescaleCounterMax [in] PreScaler counter max
*   uiBlinkCounterMax    [in] blink counter max
* \return                                                                    */
/*****************************************************************************/
void HALDEF(MLEDCTRL_Start) (unsigned int uiInst, unsigned int uiPrescaleCounterMax, unsigned int uiBlinkCounterMax)
{
  uint32_t ulVal =  ((uint32_t)uiPrescaleCounterMax << HW_SRT(mled_ctrl_cfg_prescale_counter_max))
                   |((uint32_t)uiBlinkCounterMax << HW_SRT(mled_ctrl_cfg_blink_counter_max))
                   |HW_MSK(mled_ctrl_cfg_enable);

  s_ptMledCtrl->ulMled_ctrl_cfg = ulVal;
}

/*****************************************************************************/
/*! MLEDCTRL Stop
* \description
*   This function stops MLEDCTRL unit.
* \class
*   MLEDCTRL
* \params
*   uiInst      [in] instance
* \return                                                                    */
/*****************************************************************************/
void HALDEF(MLEDCTRL_Stop) (unsigned int uiInst)
{
  s_ptMledCtrl->ulMled_ctrl_cfg = 0;
}

/*****************************************************************************/
/*! MLEDCTRL Set line
* \description
*   This function set the line register.
* \class
*   MLEDCTRL
* \params
*   uiInst      [in] instance
*   ulVal      [in] value to set
* \return                                                                    */
/*****************************************************************************/
void HALDEF(MLEDCTRL_SetLine) (unsigned int uiInst, uint32_t ulVal)
{
  s_ptMledCtrl->ulMled_ctrl_line0 = ulVal;
}

