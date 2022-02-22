#include "led.h"
#include "common_func_netx500.h"

void led_rdyrun_set(LED_COLOUR_E eColour)
{
  static const NX500_RDYRUN_LED_E s_aeRdyRunCol[] = { NX500_RDYRUN_LED_OFF, NX500_RDYRUN_LED_RED, NX500_RDYRUN_LED_GREEN };
  NX500_RDYRUN_SetRdyRunLed(s_aeRdyRunCol[(int)eColour]);
}

void led_com_init(unsigned int uiComIdx)
{
  unsigned int uiPioGrn = uiComIdx << 1;
  unsigned int uiPioRed = uiPioGrn + 1;

  NX500_PIO_SetOutput(uiPioGrn, 1);
  NX500_PIO_SetOutput(uiPioRed, 1);
  NX500_PIO_SetMode(uiPioGrn, NX500_PIO_MODE_OUT);
  NX500_PIO_SetMode(uiPioRed, NX500_PIO_MODE_OUT);
}

void led_com_set(unsigned int uiComIdx, LED_COLOUR_E eColour)
{
  unsigned int uiPioGrn = uiComIdx << 1;
  unsigned int uiPioRed = uiPioGrn + 1;
  switch(eColour)
  {
  case LED_COLOUR_OFF:
    NX500_PIO_SetOutput(uiPioGrn, 1);
    NX500_PIO_SetOutput(uiPioRed, 1);
    break;
  case LED_COLOUR_RED:
    NX500_PIO_SetOutput(uiPioGrn, 1);
    NX500_PIO_SetOutput(uiPioRed, 0);
    break;
  case LED_COLOUR_GRN:
    NX500_PIO_SetOutput(uiPioGrn, 0);
    NX500_PIO_SetOutput(uiPioRed, 1);
    break;
  default:
    /* ignore */
    break;
  }
}
