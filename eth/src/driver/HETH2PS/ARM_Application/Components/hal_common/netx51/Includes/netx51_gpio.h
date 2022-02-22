#ifndef __NETX51_GPIO_H
#define __NETX51_GPIO_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! Configuration GPIO Inversion                                             */
/*****************************************************************************/
typedef enum NX51_GPIO_INVERT_Etag {
  NX51_GPIO_INVERT_NOINVERT = 0x0,
  NX51_GPIO_INVERT_INVERT   = 0x10
} NX51_GPIO_INVERT_E;

/*****************************************************************************/
/*! Configuration GPIO Mode                                                  */
/*****************************************************************************/
typedef enum NX51_GPIO_MODE_Etag {
  NX51_GPIO_MODE_INPUT_READ                  = 0x0,
  NX51_GPIO_MODE_INPUT_CAPT_CONT_RISING_EDGE = 0x1,
  NX51_GPIO_MODE_INPUT_CAPT_ONCE_RISING_EDGE = 0x2,
  NX51_GPIO_MODE_INPUT_CAPT_ONCE_HIGH_LEVEL  = 0x3,
  NX51_GPIO_MODE_OUTPUT_SET_TO_0             = 0x4,
  NX51_GPIO_MODE_OUTPUT_SET_TO_1             = 0x5,
  NX51_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE     = 0x6,
  NX51_GPIO_MODE_OUTPUT_PWM                  = 0x7,
  NX51_GPIO_MODE_OUTPUT_PWM2                 = 0xf,
} NX51_GPIO_MODE_E;


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int            NX51_GPIO_GetIrq             (unsigned int uGpioNum);
unsigned int   NX51_GPIO_GetInput           (unsigned int uGpioNum);
uint32_t       NX51_GPIO_GetTc              (unsigned int uGpioNum);
void           NX51_GPIO_SetTc              (unsigned int uGpioNum, uint32_t ulTC);
uint32_t       NX51_GPIO_GetIn              (void);
void           NX51_GPIO_SetupMode          (unsigned int uGpioNum, NX51_GPIO_MODE_E eMode, NX51_GPIO_INVERT_E eInvert);
void           NX51_GPIO_SetupMode2         (unsigned int uGpioNum, unsigned int uGpioCnt, NX51_GPIO_MODE_E eMode, NX51_GPIO_INVERT_E eInvert);
void           NX51_GPIO_SetOutput          (unsigned int uGpioNum, bool fEnable);
uint32_t       NX51_GPIO_GetLine            (void);
void           NX51_GPIO_SetLine            (uint32_t ulVal);
void           NX51_GPIO_IrqEnable          (unsigned int uGpioNum);
void           NX51_GPIO_IrqReset           (unsigned int uGpioNum);
void           NX51_GPIO_Sleep              (unsigned int uCounter, uint32_t ulTimeout);
void           NX51_GPIO_ResetTimer         (unsigned int uCounter);
void           NX51_GPIO_SetupTimer         (unsigned int uCounter, uint32_t ulTimeoutUs, unsigned int uOnce, unsigned int uIrqEn);
void           NX51_GPIO_SetupTimerCC       (unsigned int uCounter, uint32_t ulTimeoutCC, unsigned int uOnce, unsigned int uIrqEn);
bool           NX51_GPIO_IsTimerRunning     (unsigned int uCounter);
unsigned int   NX51_GPIO_GetTimerIrq        (unsigned int uCounter);
void           NX51_GPIO_EnableTimerIrq     (unsigned int uCounter);
void           NX51_GPIO_DisableTimerIrq    (unsigned int uCounter);
void           NX51_GPIO_ConfirmTimerIrq    (unsigned int uCounter);


#endif /* #ifndef __NETX51_GPIO_H */
