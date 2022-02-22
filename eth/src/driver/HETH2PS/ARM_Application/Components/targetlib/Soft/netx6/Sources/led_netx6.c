#include "led.h"
#include "netx6_pio.h"


void led_com_init(unsigned int uiComIdx)
{
  unsigned int uiPioGrn = uiComIdx << 1;
  unsigned int uiPioRed = uiPioGrn + 1;

  NX6_PIO_SetOutput(uiPioGrn, 1);
  NX6_PIO_SetOutput(uiPioRed, 1);
  NX6_PIO_SetMode(uiPioGrn, NX6_PIO_MODE_OUT);
  NX6_PIO_SetMode(uiPioRed, NX6_PIO_MODE_OUT);
}

void led_com_set(unsigned int uiComIdx, LED_COLOUR_E eColour)
{
  unsigned int uiPioGrn = uiComIdx << 1;
  unsigned int uiPioRed = uiPioGrn + 1;
  switch(eColour)
  {
  case LED_COLOUR_OFF:
    NX6_PIO_SetOutput(uiPioGrn, 1);
    NX6_PIO_SetOutput(uiPioRed, 1);
    break;
  case LED_COLOUR_RED:
    NX6_PIO_SetOutput(uiPioGrn, 1);
    NX6_PIO_SetOutput(uiPioRed, 0);
    break;
  case LED_COLOUR_GRN:
    NX6_PIO_SetOutput(uiPioGrn, 0);
    NX6_PIO_SetOutput(uiPioRed, 1);
    break;
  default:
    /* ignore */
    break;
  }
}
