/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_gpio.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_GPIO

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! GPIO Setup Mode
* \description
*   Configure mode for the selected GPIO.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
*   eMode             [in]   GPIO_MODE_OUTPUT      = Set the selected GPIO to Output mode
*                            GPIO_MODE_INPUT_READ  = Set the selected GPIO to Input mode
*   eInvert           [in]   GPIO_NOINVERT         = Does not invert the selected GPIO
*                            GPIO_INVERT           = Inverts the selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_SetupMode( unsigned int       uGpioNum,
                          NX51_GPIO_MODE_E   eMode,
                          NX51_GPIO_INVERT_E eInvert )
{
  s_ptGpio->aulGpio_cfg[uGpioNum] = (uint32_t) (eMode | eInvert);
}

/*****************************************************************************/
/*! GPIO Setup Mode
* \description
*   Configure mode for the selected GPIO.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
*   eMode             [in]   GPIO_MODE_OUTPUT      = Set the selected GPIO to Output mode
*                            GPIO_MODE_INPUT_READ  = Set the selected GPIO to Input mode
*   eInvert           [in]   GPIO_NOINVERT         = Does not invert the selected GPIO
*                            GPIO_INVERT           = Inverts the selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_SetupMode2( unsigned int       uGpioNum,
                           unsigned int       uGpioCnt,
                           NX51_GPIO_MODE_E   eMode,
                           NX51_GPIO_INVERT_E eInvert )
{
  s_ptGpio->aulGpio_cfg[uGpioNum] = (uint32_t) (eMode | eInvert | (uGpioCnt << SRT_NX51_gpio_cfg0_count_ref));
}

/*****************************************************************************/
/*! GPIO Set Output
* \description
*   Enables/disable output for the selected GPIO.
* \class
*   GPIO
* \params
*   uGpioNum           [in]   Selected GPIO
*   fEnable            [in]   0 = Disable the Output
*                             1 = Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_SetOutput( unsigned int uGpioNum,
                          bool         fEnable )
{
  if( fEnable )
    s_ptGpio->ulGpio_line |= (uint32_t)1 << uGpioNum;
  else
    s_ptGpio->ulGpio_line &= ~((uint32_t)1 << uGpioNum);
}

/*****************************************************************************/
/*! GPIO Get Line
* \description
*  Read the GPIO line register.
* \class
*   GPIO
* \params
* \return
*   Gpio_line value                                                          */
/*****************************************************************************/
uint32_t NX51_GPIO_GetLine( void )
{
  return ( s_ptGpio->ulGpio_line );
}

/*****************************************************************************/
/*! GPIO Set Line
* \description
*   Set GPIO line.
* \class
*   GPIO
* \params
*   ulVal          [in]   Value of GPIO Line
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_SetLine( uint32_t ulVal )
{
  s_ptGpio->ulGpio_line = ulVal;
}

/*****************************************************************************/
/*! GPIO Get Input
* \description
*   Read selected GPIO.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
* \return
*   Gpio_in value of selected GPIO                                           */
/*****************************************************************************/
unsigned int NX51_GPIO_GetInput( unsigned int uGpioNum )
{
  return ( s_ptGpio->ulGpio_in & ((uint32_t)1<<uGpioNum) ) ? 1 : 0;
}

/*****************************************************************************/
/*! GPIO Get TC
* \description
*   Read selected GPIO TC register.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
uint32_t NX51_GPIO_GetTc( unsigned int uGpioNum )
{
  return ( s_ptGpio->aulGpio_tc[uGpioNum] );
}

/*****************************************************************************/
/*! GPIO Set TC
* \description
*   Read selected GPIO TC register.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
*   uTc               [in]   TC value
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_SetTc( unsigned int uGpioNum, uint32_t ulTC )
{
  s_ptGpio->aulGpio_tc[uGpioNum] = ulTC;
}

/*****************************************************************************/
/*! GPIO Get In Register
* \description
*  Read the GPIO IN register.
* \class
*   GPIO
* \params
* \return
*   Gpio_in value                                                            */
/*****************************************************************************/
uint32_t NX51_GPIO_GetIn( void )
{
  return ( s_ptGpio->ulGpio_in );
}

/*****************************************************************************/
/*! GPIO Interrupt Enable
* \description
*   Enable interrupt request for the selected GPIO.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_IrqEnable( unsigned int uGpioNum )
{
  s_ptGpio->ulGpio_irq_mask_set |= (uint32_t)1 << uGpioNum;
}

/*****************************************************************************/
/*! GPIO Interrupt Disable
* \description
*   Disable interrupt request for the selected GPIO.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_IrqDisable( unsigned int uGpioNum )
{
  s_ptGpio->ulGpio_irq_mask_rst |= (uint32_t)1 << uGpioNum;
}

/*****************************************************************************/
/*! GPIO Get Interrupt
* \description
*   Read interrupt status of the selected GPIO.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
* \return
*  Current irq_masked value of selected GPIO                                 */
/*****************************************************************************/
int NX51_GPIO_GetIrq( unsigned int uGpioNum )
{
  return (s_ptGpio->ulGpio_irq_masked & ((uint32_t)1 << uGpioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! GPIO Interrupt Reset
* \description
*   Writes 1 to reset the interrupt of the selected GPIO.
* \class
*   GPIO
* \params
*   uGpioNum          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_IrqReset( unsigned int uGpioNum )
{
  s_ptGpio->ulGpio_irq_raw = (uint32_t)1 << uGpioNum;
}

/*****************************************************************************/
/*! GPIO Sleep
* \description
*   Sleep a while using a GPIO timer.
* \class
*   GPIO
* \params
*   uCounter           [in]  Selected GPIO
*   ulTimeout          [in]  The Time to wait in microseconds
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_Sleep( unsigned int uCounter,
                      uint32_t     ulTimeout )
{
  uint32_t ulVal;

  s_ptGpio->aulGpio_counter_ctrl[uCounter]  = 0;          /* Clear the timer register         */
  s_ptGpio->aulGpio_counter_cnt[uCounter]   = 0;          /* Clear the current counter value  */
  s_ptGpio->aulGpio_counter_max[uCounter]   = ulTimeout * (NX51_DEV_FREQUENCY/1000000);  /* Set the counter value converted from microseconds to nanoseconds  */
  s_ptGpio->aulGpio_counter_ctrl[uCounter] |= (MSK_NX51_gpio_counter0_ctrl_run | MSK_NX51_gpio_counter0_ctrl_once  ); /* Enable the timer to one shot */

  /* poll timer Control for 'run' bit */
  do {
    ulVal  = s_ptGpio->aulGpio_counter_ctrl[uCounter];
    ulVal &= MSK_NX51_gpio_counter0_ctrl_run;
  } while ( ulVal!=0 );
}

/*****************************************************************************/
/*! GPIO Reset Timer
* \description
*   Reset a GPIO timer.
* \class
*   GPIO
* \params
*   uCounter          [in]  Selected GPIO timer
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_ResetTimer( unsigned int uCounter )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter] = 0;
  s_ptGpio->aulGpio_counter_cnt[uCounter]  = 0;
  s_ptGpio->aulGpio_counter_max[uCounter]  = 0;
  s_ptGpio->ulCnt_irq_mask_rst             = 1 << uCounter;
  s_ptGpio->ulCnt_irq_raw                  = 1 << uCounter;
}


/*****************************************************************************/
/*! GPIO Setup Timer
* \description
*   Setup a GPIO timer.
* \class
*   GPIO
* \params
*   uCounter          [in]  Selected GPIO
*   ulTimeout         [in]  The Time to wait in microseconds
*   uOnce             [in]  Timer Once configuration
*   uIrqEn            [in]  Timer IRQ enable
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_SetupTimer( unsigned int uCounter,
                           uint32_t     ulTimeout,
                           unsigned int uOnce,
                           unsigned int uIrqEn )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  = 0;          /* Clear the timer register         */
  s_ptGpio->aulGpio_counter_cnt[uCounter]   = 0;          /* Clear the current counter value  */
  s_ptGpio->aulGpio_counter_max[uCounter]   = ulTimeout * (NX51_DEV_FREQUENCY/1000000);  /* Set the counter value converted from microseconds to nanoseconds  */
  s_ptGpio->aulGpio_counter_ctrl[uCounter] |= (  MSK_NX51_gpio_counter0_ctrl_run
                                                |((uint32_t)uOnce  << SRT_NX51_gpio_counter0_ctrl_once)
                                                |((uint32_t)uIrqEn << SRT_NX51_gpio_counter0_ctrl_irq_en)
                                               );
}

/*****************************************************************************/
/*! GPIO Setup Timer in Clock cycles
* \description
*   Setup a GPIO timer.
* \class
*   GPIO
* \params
*   uCounter          [in]  Selected GPIO
*   ulTimeout         [in]  The Time to wait in microseconds
*   uOnce             [in]  Timer Once configuration
*   uIrqEn            [in]  Timer IRQ enable
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_SetupTimerCC( unsigned int uCounter,
                             uint32_t     ulTimeout,
                             unsigned int uOnce,
                             unsigned int uIrqEn )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  = 0;          /* Clear the timer register         */
  s_ptGpio->aulGpio_counter_cnt[uCounter]   = 0;          /* Clear the current counter value  */
  s_ptGpio->aulGpio_counter_max[uCounter]   = ulTimeout;  /* Set the counter value            */
  s_ptGpio->aulGpio_counter_ctrl[uCounter] |= (  MSK_NX51_gpio_counter0_ctrl_run
                                                |((uint32_t)uOnce  << SRT_NX51_gpio_counter0_ctrl_once)
                                                |((uint32_t)uIrqEn << SRT_NX51_gpio_counter0_ctrl_irq_en)
                                               );
}

/*****************************************************************************/
/*! GPIO IsTimerRunning
* \description
*   Checks for GPIO timer expired.
* \class
*   GPIO
* \params
*    uCounter          [in] Selected counter
* \return
*    false
*    true                                                                       */
/*****************************************************************************/
bool NX51_GPIO_IsTimerRunning( unsigned int uCounter )
{
  /* gets timer Control 'run' bit */
  if( (s_ptGpio->aulGpio_counter_ctrl[uCounter] & MSK_NX51_gpio_counter0_ctrl_run) == 0 )
    return false;
  else
    return true;
}

/*****************************************************************************/
/*! GPIO Get Timer Interrupt
* \description
*   Read interrupt status of the selected GPIO Timer.
* \class
*   GPIO
* \params
*   uCounter          [in]   Selected Counter
* \return
*   Current counter_irq_masked value                                         */
/*****************************************************************************/
unsigned int NX51_GPIO_GetTimerIrq( unsigned int uCounter )
{
  return (s_ptGpio->ulCnt_irq_masked & ((uint32_t)1 << uCounter)) ? 1 : 0;
}


/*****************************************************************************/
/*! GPIO Timer Interrupt Enable
* \description
*   Enable interrupt request for the selected Counter.
* \class
*   GPIO
* \params
*   uCounter          [in]   Selected Counter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_EnableTimerIrq( unsigned int uCounter )
{
  s_ptGpio->ulCnt_irq_mask_set = (uint32_t)1 << uCounter;
}

/*****************************************************************************/
/*! GPIO Timer Irq Disable
* \description
*   Disable interrupt request for the selected Counter..
* \class
*   GPIO
* \params
*   uCounter          [in]   Selected Counter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_DisableTimerIrq( unsigned int uCounter )
{
  s_ptGpio->ulCnt_irq_mask_rst = (uint32_t)1 << uCounter;
}

/*****************************************************************************/
/*! GPIO Timer Interrupt Reset
* \description
*   Writes 1 to reset the interrupt of the selected Counter.
* \class
*   GPIO
* \params
*   uCounter          [in]   Selected Counter
* \return
*                                                                            */
/*****************************************************************************/
void NX51_GPIO_ConfirmTimerIrq( unsigned int uCounter )
{
  s_ptGpio->ulCnt_irq_raw = (uint32_t)1 << uCounter;
}
