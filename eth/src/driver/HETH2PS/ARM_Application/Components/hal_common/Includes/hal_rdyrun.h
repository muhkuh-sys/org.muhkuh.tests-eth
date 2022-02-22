#ifndef __HAL_RDYRUN_H
#define __HAL_RDYRUN_H

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum {
  RDYRUN_LED_OFF = 0,
  RDYRUN_LED_GREEN,
  RDYRUN_LED_RED,
  RDYRUN_LED_INV
} RDYRUN_LED_E;


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void RDYRUN_SetRdyRunLed( RDYRUN_LED_E eMode );

#endif /* #ifndef __HAL_RDYRUN_H */
