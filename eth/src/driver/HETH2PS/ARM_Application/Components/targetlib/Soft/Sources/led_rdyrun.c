#include "led.h"
#include "hal_rdyrun.h"

void led_rdyrun_set(LED_COLOUR_E eColor)
{
  static const RDYRUN_LED_E s_aeRdyRunCol[] = { RDYRUN_LED_OFF, RDYRUN_LED_RED, RDYRUN_LED_GREEN };
  RDYRUN_SetRdyRunLed(s_aeRdyRunCol[eColor]);
}
