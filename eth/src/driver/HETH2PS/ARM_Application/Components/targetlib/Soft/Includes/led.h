#ifndef __LED_H
#define __LED_H

/****************************************************************************************
*  Definitions
****************************************************************************************/

typedef enum
{
  LED_COLOUR_OFF,
  LED_COLOUR_RED,
  LED_COLOUR_GRN
} LED_COLOUR_E;


/****************************************************************************************
*  Functions
****************************************************************************************/

void led_rdyrun_set(LED_COLOUR_E eColour);

void led_com_init(unsigned int uiComIdx);
void led_com_set(unsigned int uiComIdx, LED_COLOUR_E eColour);

void led_check(void);

#endif /* __LED_H */
