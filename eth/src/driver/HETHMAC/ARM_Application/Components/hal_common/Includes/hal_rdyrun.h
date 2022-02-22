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
#ifndef HALDEC
#define HALDEC(name) name
#endif

void HALDEC(RDYRUN_SetRdyRunLed)( RDYRUN_LED_E tMode );

#endif /* #ifndef __HAL_RDYRUN_H */
