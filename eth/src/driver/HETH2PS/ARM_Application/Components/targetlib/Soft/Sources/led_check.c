#include "led.h"
#include "io.h"
#include "asic.h"

void led_check(void)
{
  unsigned int uiIdx;
  unsigned int uiNumOutputs = io_output_cnt();

  /* set each output LED */
  for(uiIdx = 0; uiIdx < uiNumOutputs; uiIdx++)
  {
    io_output_set(1UL << uiIdx);
    sleep(50);
  }
  io_output_set(0);

  /* set each COM LED */
  for(uiIdx = 0; uiIdx < 2; ++uiIdx)
  {
    led_com_set(uiIdx, LED_COLOUR_RED);
    sleep(200);
    led_com_set(uiIdx, LED_COLOUR_GRN);
    sleep(200);
    led_com_set(uiIdx, LED_COLOUR_OFF);
    sleep(200);
  }
}
