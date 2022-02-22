#ifndef __NETX6_GPIO_H
#define __NETX6_GPIO_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! Configuration GPIO Inversion                                             */
/*****************************************************************************/
typedef enum NX6_GPIO_INVERT_Etag {
  NX6_GPIO_INVERT_NOINVERT = 0x0,
  NX6_GPIO_INVERT_INVERT   = 0x10
} NX6_GPIO_INVERT_E;

/*****************************************************************************/
/*! Configuration GPIO Mode                                                  */
/*****************************************************************************/
typedef enum NX6_GPIO_MODE_Etag {
  NX6_GPIO_MODE_INPUT_READ                  = 0x0,
  NX6_GPIO_MODE_INPUT_CAPT_CONT_RISING_EDGE = 0x1,
  NX6_GPIO_MODE_INPUT_CAPT_ONCE_RISING_EDGE = 0x2,
  NX6_GPIO_MODE_INPUT_CAPT_ONCE_HIGH_LEVEL  = 0x3,
  NX6_GPIO_MODE_OUTPUT_SET_TO_0             = 0x4,
  NX6_GPIO_MODE_OUTPUT_SET_TO_1             = 0x5,
  NX6_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE     = 0x6
} NX6_GPIO_MODE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int            NX6_GPIO_GetIrq             (unsigned int uGpioNum);
int            NX6_GPIO_GetInput           (unsigned int uGpioNum);
uint32_t       NX6_GPIO_GetIn              (void);
void           NX6_GPIO_SetupMode          (unsigned int uGpioNum, NX6_GPIO_MODE_E eMode, NX6_GPIO_INVERT_E eInvert);
void           NX6_GPIO_SetOutput          (unsigned int uGpioNum, bool fEnable);
uint32_t       NX6_GPIO_GetLine            (void);
void           NX6_GPIO_SetLine            (uint32_t ulVal);
void           NX6_GPIO_IrqEnable          (unsigned int uGpioNum);
void           NX6_GPIO_IrqReset           (unsigned int uGpioNum);
void           NX6_GPIO_Sleep              (unsigned int uCounter, uint32_t ulTimeout);
void           NX6_GPIO_SetupTimer         (unsigned int uCounter, uint32_t ulTimeout, unsigned int uOnce, unsigned int uIrqEn);
bool           NX6_GPIO_IsTimerRunning     (unsigned int uCounter);
unsigned int   NX6_GPIO_GetTimerIrq        (unsigned int uCounter);
void           NX6_GPIO_EnableTimerIrq     (unsigned int uCounter);
void           NX6_GPIO_DisableTimerIrq    (unsigned int uCounter);
void           NX6_GPIO_ConfirmTimerIrq    (unsigned int uCounter);

#endif /* #ifndef __NETX6_GPIO_H */
