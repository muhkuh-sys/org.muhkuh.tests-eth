#ifndef __HAL_GPIO_H
#define __HAL_GPIO_H

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/*! Result codes for functions */
/*****************************************************************************/
typedef enum
{
  GPIO_OKAY            = 0, /**< Successful          */
  GPIO_ERR_INVAL_PARAM = 1  /**< Invalid parameter   */
} GPIO_RESULT_E;

/*****************************************************************************/
/*! Configuration GPIO Inversion                                             */
/*****************************************************************************/
typedef enum
{
  GPIO_INVERT_NOINVERT = 0x0,
  GPIO_INVERT_INVERT   = 0x10
} GPIO_INVERT_E;

/*****************************************************************************/
/*! Configuration GPIO Mode                                                  */
/*****************************************************************************/
typedef enum
{
  GPIO_MODE_INPUT_READ                  = 0x0,
  GPIO_MODE_INPUT_CAPT_CONT_RISING_EDGE = 0x1,
  GPIO_MODE_INPUT_CAPT_ONCE_RISING_EDGE = 0x2,
  GPIO_MODE_INPUT_CAPT_ONCE_HIGH_LEVEL  = 0x3,
  GPIO_MODE_OUTPUT_SET_TO_0             = 0x4,
  GPIO_MODE_OUTPUT_SET_TO_1             = 0x5,
  GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE     = 0x6,
  GPIO_MODE_OUTPUT_PWM                  = 0x7,
  GPIO_MODE_OUTPUT_BLINK_MODE           = 0x8,
  GPIO_MODE_OUTPUT_PWM2                 = 0xf
} GPIO_MODE_E;

/*****************************************************************************/
/*! Configuration GPIO COUNT_REF                                             */
/*****************************************************************************/
typedef enum
{
  GPIO_MODE_COUNT_REF0    = 0x0,
  GPIO_MODE_COUNT_REF1    = 0x1,
  GPIO_MODE_COUNT_REF2    = 0x2,
  GPIO_MODE_COUNT_REF3    = 0x3,
  GPIO_MODE_COUNT_REF4    = 0x4,
  GPIO_MODE_COUNT_SYSTIME = 0x7
} GPIO_COUNT_REF_E;

/*****************************************************************************/
/*! Configuration Blink Mode Once                                            */
/*****************************************************************************/
typedef enum
{
  GPIO_BLINK_ONCE_CYCLIC = 0x0,
  GPIO_BLINK_ONCE_ONCE   = 0x1
} GPIO_BLINK_ONCE_E;



/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

int           GPIO_GetInput        ( unsigned int uiGpioPin );
uint32_t      GPIO_GetIn           ( void );
uint32_t      GPIO_GetTc           ( unsigned int uiGpioPin );
void          GPIO_SetTc           ( unsigned int uiGpioPin, uint32_t ulTC );
void          GPIO_SetupMode       ( unsigned int uiGpioPin, unsigned int uiGpioCnt, GPIO_MODE_E eMode, GPIO_INVERT_E eInvert );
void          GPIO_SetOutput       ( unsigned int uiGpioPin, bool fEnable );
uint32_t      GPIO_GetLine         ( void );
void          GPIO_SetLine         ( uint32_t ulVal );
void          GPIO_IrqEnable       ( uint32_t ulIrqMsk );
void          GPIO_IrqDisable      ( uint32_t ulIrqMsk );
void          GPIO_IrqReset        ( uint32_t ulIrqMsk );
uint32_t      GPIO_GetIrq          ( void );
uint32_t      GPIO_GetIrqRaw       ( void );
void          GPIO_Sleep           ( unsigned int uiGpioCnt, uint32_t ulTimeout );
void          GPIO_ResetTimer      ( unsigned int uiGpioCnt );
void          GPIO_SetupTimer      ( unsigned int uiGpioCnt, uint32_t ulTimeout, bool fOnce, bool fIrqEn );
void          GPIO_SetupTimerCC    ( unsigned int uiGpioCnt, uint32_t ulTimeout, bool fOnce, bool fIrqEn );
int           GPIO_IsTimerRunning  ( unsigned int uiGpioCnt );
uint32_t      GPIO_GetTimerCnt     ( unsigned int uiGpioCnt );
void          GPIO_TimerIrqEnable  ( uint32_t ulIrqMsk );
void          GPIO_TimerIrqDisable ( uint32_t ulIrqMsk );
void          GPIO_TimerIrqReset   ( uint32_t ulIrqMsk );
uint32_t      GPIO_GetTimerIrq     ( void );
uint32_t      GPIO_GetTimerIrqRaw  ( void );
GPIO_RESULT_E GPIO_SetupBlinkMode  ( unsigned int uiGpioPin, unsigned int uiGpioCnt, GPIO_INVERT_E eInvert, GPIO_BLINK_ONCE_E eBlinkOnce, unsigned int uiBlinkLen, uint32_t ulBlinkPattern );
void          GPIO_StartBlinkMode  ( unsigned int uiGpioCnt, uint32_t ulTimeout );
GPIO_RESULT_E GPIO_SelIf           ( unsigned int uiIf );

#endif /* #ifndef __HAL_GPIO_H */
