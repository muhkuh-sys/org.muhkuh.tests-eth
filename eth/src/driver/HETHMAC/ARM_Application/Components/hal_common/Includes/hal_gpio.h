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
#ifndef HALDEC
#define HALDEC(name) name
#endif

int                   HALDEC(GPIO_GetInput)          ( unsigned int uiGpioPin );
uint32_t              HALDEC(GPIO_GetIn)             ( void );
uint32_t              HALDEC(GPIO_GetTc)             ( unsigned int uiGpioPin );
void                  HALDEC(GPIO_SetTc)             ( unsigned int uiGpioPin, uint32_t ulTC );
void                  HALDEC(GPIO_SetupMode)         ( unsigned int uiGpioPin, unsigned int uiGpioCnt, GPIO_MODE_E eMode, GPIO_INVERT_E eInvert );
void                  HALDEC(GPIO_SetOutput)         ( unsigned int uiGpioPin, bool fEnable );
uint32_t              HALDEC(GPIO_GetLine)           ( void );
void                  HALDEC(GPIO_SetLine)           ( uint32_t ulVal );
void                  HALDEC(GPIO_IrqEnable)         ( uint32_t ulIrqMsk );
void                  HALDEC(GPIO_IrqDisable)        ( uint32_t ulIrqMsk );
void                  HALDEC(GPIO_IrqReset)          ( uint32_t ulIrqMsk );
uint32_t              HALDEC(GPIO_GetIrq)            ( void );
uint32_t              HALDEC(GPIO_GetIrqRaw)         ( void );
void                  HALDEC(GPIO_Sleep)             ( unsigned int uiGpioCnt, uint32_t ulTimeout );
void                  HALDEC(GPIO_ResetTimer)        ( unsigned int uiGpioCnt );
void                  HALDEC(GPIO_SetupTimer)        ( unsigned int uiGpioCnt, uint32_t ulTimeout, bool fOnce, bool fIrqEn );
void                  HALDEC(GPIO_SetupTimerCC)      ( unsigned int uiGpioCnt, uint32_t ulTimeout, bool fOnce, bool fIrqEn );
int                   HALDEC(GPIO_IsTimerRunning)    ( unsigned int uiGpioCnt );
uint32_t              HALDEC(GPIO_GetTimerCnt)       ( unsigned int uiGpioCnt );
void                  HALDEC(GPIO_TimerIrqEnable)    ( uint32_t ulIrqMsk );
void                  HALDEC(GPIO_TimerIrqDisable)   ( uint32_t ulIrqMsk );
void                  HALDEC(GPIO_TimerIrqReset)     ( uint32_t ulIrqMsk );
uint32_t              HALDEC(GPIO_GetTimerIrq)       ( void );
uint32_t              HALDEC(GPIO_GetTimerIrqRaw)    ( void );
GPIO_RESULT_E         HALDEC(GPIO_SetupBlinkMode)    ( unsigned int uiGpioPin, unsigned int uiGpioCnt, GPIO_INVERT_E eInvert, GPIO_BLINK_ONCE_E eBlinkOnce, unsigned int uiBlinkLen, uint32_t ulBlinkPattern );
void                  HALDEC(GPIO_StartBlinkMode)    ( unsigned int uiGpioCnt, uint32_t ulTimeout );
GPIO_RESULT_E         HALDEC(GPIO_SelIf)             ( unsigned int uiIf );

#endif /* #ifndef __HAL_GPIO_H */
