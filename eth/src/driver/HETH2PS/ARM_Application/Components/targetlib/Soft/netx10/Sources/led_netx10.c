#include "led.h"
#include "common_func_netx10.h"

void led_rdyrun_set(LED_COLOUR_E eColour)
{
  static const NX10_RDYRUN_LED_E s_aeRdyRunCol[] = { NX10_RDYRUN_LED_OFF, NX10_RDYRUN_LED_RED, NX10_RDYRUN_LED_GREEN };
  NX10_RDYRUN_SetRdyRunLed(s_aeRdyRunCol[(int)eColour]);
}

void led_com_init(unsigned int uiComIdx)
{
}

void led_com_set(unsigned int uiComIdx, LED_COLOUR_E eColour)
{
}
