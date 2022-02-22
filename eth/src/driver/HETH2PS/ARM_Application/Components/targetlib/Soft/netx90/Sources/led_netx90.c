#include "led.h"
#include "hal_mled_ctrl.h"

#define MSK_MLED_COM0_RED 0x01 /* MLED0 high side LED */
#define MSK_MLED_COM0_GRN 0x02 /* MLED0 low side LED */
#define MSK_MLED_COM1_RED 0x04 /* MLED1 high side LED */
#define MSK_MLED_COM1_GRN 0x08 /* MLED1 low side LED */

#define MSK_MLED_COM0 (MSK_MLED_COM0_GRN | MSK_MLED_COM0_RED)
#define MSK_MLED_COM1 (MSK_MLED_COM1_GRN | MSK_MLED_COM1_RED)


static uint32_t s_ulMled;

void led_com_init(unsigned int uiComIdx)
{
  unsigned int uiPioRed = uiComIdx << 1;
  unsigned int uiPioGrn = uiPioRed + 1;

  s_ulMled = 0;

  /* Map MLED to COM Red/Green, set to Manual Mode (control LEDs via line register) */
  MLEDCTRL_ConfigureOutput(0, uiPioRed, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, uiPioGrn, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);
}

void led_com_set(unsigned int uiComIdx, LED_COLOUR_E eColour)
{
  unsigned int uiSrt = (uiComIdx * 2);

  s_ulMled &= ~(MSK_MLED_COM0 << uiSrt);

  switch(eColour)
  {
  case LED_COLOUR_OFF:
    break;
  case LED_COLOUR_RED:
    s_ulMled |= (MSK_MLED_COM0_RED << uiSrt);
    break;
  case LED_COLOUR_GRN:
    s_ulMled |= (MSK_MLED_COM0_GRN << uiSrt);
    break;
  default:
    /* ignore */
    break;
  }

  MLEDCTRL_SetLine(0, s_ulMled);
}
