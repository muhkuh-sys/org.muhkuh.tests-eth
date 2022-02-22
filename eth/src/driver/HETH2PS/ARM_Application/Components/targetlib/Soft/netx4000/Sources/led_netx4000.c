#include "led.h"
#include "hal_pio.h"

void led_com_init(unsigned int uiComIdx)
{
  unsigned int uiPioGrn = uiComIdx << 1;
  unsigned int uiPioRed = uiPioGrn + 1;

  PIO_SetOutput(uiPioGrn, 1);
  PIO_SetOutput(uiPioRed, 1);
  PIO_SetMode(uiPioGrn, PIO_MODE_OUT);
  PIO_SetMode(uiPioRed, PIO_MODE_OUT);
}

void led_com_set(unsigned int uiComIdx, LED_COLOUR_E eColour)
{
  unsigned int uiPioGrn = uiComIdx << 1;
  unsigned int uiPioRed = uiPioGrn + 1;
  switch(eColour)
  {
  case LED_COLOUR_OFF:
    PIO_SetOutput(uiPioGrn, 1);
    PIO_SetOutput(uiPioRed, 1);
    break;
  case LED_COLOUR_RED:
    PIO_SetOutput(uiPioGrn, 1);
    PIO_SetOutput(uiPioRed, 0);
    break;
  case LED_COLOUR_GRN:
    PIO_SetOutput(uiPioGrn, 0);
    PIO_SetOutput(uiPioRed, 1);
    break;
  default:
    /* ignore */
    break;
  }
}
