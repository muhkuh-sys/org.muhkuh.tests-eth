#ifndef _SYS_H
#define _SYS_H

/****************************************************************************************
*  Definitions
****************************************************************************************/

typedef enum
{
  LED_COLOUR_OFF,
  LED_COLOUR_RED,
  LED_COLOUR_GREEN
} LED_COLOUR_E;

/* IEEE 1588 time format */
#define SYSTIME_BORDER 1000000000 /* 1 second */
#define SYSTIME_SPEED  0xa0000000 /* 1ns resolution (10ns/clock cycle) */
extern unsigned int g_auiMiiCfg[];

/****************************************************************************************
*  Functions
****************************************************************************************/

void     sys_led_rdyrun_set(LED_COLOUR_E eColour);
void     sys_time_get(uint32_t* pulTimeS, uint32_t* pulTimeNs);
void     sys_poll(void);
void     sys_sleep(unsigned int uiTimeMs);

void     irq_init(void);
int      main_task(unsigned int uiPortCnt);

#endif
