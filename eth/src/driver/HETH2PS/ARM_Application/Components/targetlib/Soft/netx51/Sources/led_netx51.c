#include "led.h"
#include "netx51_pio.h"

void led_com_init(unsigned int uiComIdx)
{
  unsigned int uiPioGrn = uiComIdx << 1;
  unsigned int uiPioRed = uiPioGrn + 1;

  NX51_PIO_SetOutput(uiPioGrn, 1);
  NX51_PIO_SetOutput(uiPioRed, 1);
  NX51_PIO_SetMode(uiPioGrn, NX51_PIO_MODE_OUT);
  NX51_PIO_SetMode(uiPioRed, NX51_PIO_MODE_OUT);
}

void led_com_set(unsigned int uiComIdx, LED_COLOUR_E eColour)
{
  unsigned int uiPioGrn = uiComIdx << 1;
  unsigned int uiPioRed = uiPioGrn + 1;
  switch(eColour)
  {
  case LED_COLOUR_OFF:
    NX51_PIO_SetOutput(uiPioGrn, 1);
    NX51_PIO_SetOutput(uiPioRed, 1);
    break;
  case LED_COLOUR_RED:
    NX51_PIO_SetOutput(uiPioGrn, 1);
    NX51_PIO_SetOutput(uiPioRed, 0);
    break;
  case LED_COLOUR_GRN:
    NX51_PIO_SetOutput(uiPioGrn, 0);
    NX51_PIO_SetOutput(uiPioRed, 1);
    break;
  default:
    /* ignore */
    break;
  }
}
