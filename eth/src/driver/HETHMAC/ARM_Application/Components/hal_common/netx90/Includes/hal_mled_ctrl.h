#ifndef __HAL_MLEDCTRL_H
#define __HAL_MLEDCTRL_H

#include <stdint.h>
#include <stdbool.h>

/* Definitions */

/*! Select Input for MLED_CTRL units */
typedef enum
{
  MLED_CTRL_SEL_ALWAYS_OFF  = 0,   /**<   0 */
  MLED_CTRL_SEL_LINE_REGISTER,     /**<   1 */
  MLED_CTRL_SEL_MLED_CTRL_BLINK,   /**<   2 */
  MLED_CTRL_SEL_XM0_IO0,           /**<   3, (COM only) */
  MLED_CTRL_SEL_XM0_IO1,           /**<   4, (COM only) */
  MLED_CTRL_SEL_XM0_IO2,           /**<   5, (COM only) */
  MLED_CTRL_SEL_XM0_IO3,           /**<   6, (COM only) */
  MLED_CTRL_SEL_XM0_IO4,           /**<   7, (COM only) */
  MLED_CTRL_SEL_XM0_IO5,           /**<   8, (COM only) */
  MLED_CTRL_SEL_XM1_IO0,           /**<   9, (COM only) */
  MLED_CTRL_SEL_XM1_IO1,           /**<  10, (COM only) */
  MLED_CTRL_SEL_XM1_IO2,           /**<  11, (COM only) */
  MLED_CTRL_SEL_XM1_IO3,           /**<  12, (COM only) */
  MLED_CTRL_SEL_XM1_IO4,           /**<  13, (COM only) */
  MLED_CTRL_SEL_XM1_IO5,           /**<  14, (COM only) */
  MLED_CTRL_SEL_PHY_CTRL0_LED0,    /**<  15, (COM only) */
  MLED_CTRL_SEL_PHY_CTRL0_LED1,    /**<  16, (COM only) */
  MLED_CTRL_SEL_PHY_CTRL1_LED0,    /**<  17, (COM only) */
  MLED_CTRL_SEL_PHY_CTRL1_LED1,    /**<  18, (COM only) */
  MLED_CTRL_SEL_BLINK0,            /**<  19, (COM only) */
  MLED_CTRL_SEL_BLINK1,            /**<  20, (COM only) */
  MLED_CTRL_SEL_BLINK2,            /**<  21, (COM only) */
  MLED_CTRL_SEL_BLINK3,            /**<  22, (COM only) */
  MLED_CTRL_SEL_PASSTHROUGH = 31   /**<  31 */
} MLED_CTRL_SEL_E;

#define MLED_CTRL_BRIGHTNESS_MAX  0xffU

/* Function prototypes */
#ifndef HALDEC
#define HALDEC(name) name
#endif

int  HALDEC(MLEDCTRL_ConfigureOutput)(unsigned int uiInst,
                                      unsigned int uiOutput,
                                      MLED_CTRL_SEL_E eOutputSel,
                                      bool fInvertInput,
                                      unsigned int uiBrightness);
void HALDEC(MLEDCTRL_Start)          (unsigned int uInst, unsigned int uiPrescaleCounterMax, unsigned int uiBlinkCounterMax);
void HALDEC(MLEDCTRL_Stop)           (unsigned int uInst);
void HALDEC(MLEDCTRL_SetLine)        (unsigned int uInst, uint32_t ulVal);

#endif /* #ifndef __HAL_MLEDCTRL_H */
