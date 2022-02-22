/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_gpio.h"
#include "hal_resources_defines_netx6.h"

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
*   tMode             [in]   GPIO_MODE_OUTPUT      = Set the selected GPIO to Output mode
*                            GPIO_MODE_INPUT_READ  = Set the selected GPIO to Input mode
*   tInvert           [in]   GPIO_NOINVERT         = Does not invert the selected GPIO
*                            GPIO_INVERT           = Inverts the selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX6_GPIO_SetupMode( unsigned int      uGpioNum,
                         NX6_GPIO_MODE_E   eMode,
                         NX6_GPIO_INVERT_E eInvert)
{
  NX_WRITE32(s_ptGpio->aulGpio_cfg[uGpioNum], ( (uint32_t)eMode
                                               |(uint32_t)eInvert)
            );
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
void NX6_GPIO_SetOutput( unsigned int uGpioNum,
                         bool         fEnable )
{
  uint32_t ulVal = NX_READ32(s_ptGpio->ulGpio_line);
  if( fEnable )
    ulVal |= (uint32_t)1U << uGpioNum;
  else
    ulVal &= ~((uint32_t)1U << uGpioNum);
  NX_WRITE32(s_ptGpio->ulGpio_line, ulVal);
}

/*****************************************************************************/
/*! GPIO Get Line
* \description
*  Read the GPIO line register.
* \class 
*   GPIO 
* \params
* \return
*   Current Gpio_line value                                                  */
/*****************************************************************************/
uint32_t NX6_GPIO_GetLine( void )
{
  return NX_READ32(s_ptGpio->ulGpio_line);
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
void NX6_GPIO_SetLine( uint32_t ulVal )
{
  NX_WRITE32(s_ptGpio->ulGpio_line, ulVal);
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
*   Current gpio_in value of selected GPIO                                   */
/*****************************************************************************/
int NX6_GPIO_GetInput( unsigned int uGpioNum )
{
  return ( NX_READ32(s_ptGpio->ulGpio_in) & ((uint32_t)1<<uGpioNum) ) ? 1 : 0;
}

/*****************************************************************************/
/*! GPIO Get In Register
* \description
*  Read the GPIO IN register.
* \class 
*   GPIO 
* \params
* \return
*   Current Gpio_in value                                                    */
/*****************************************************************************/
uint32_t NX6_GPIO_GetIn( void )
{
  return NX_READ32(s_ptGpio->ulGpio_in);
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
void NX6_GPIO_IrqEnable( unsigned int uGpioNum )
{
  uint32_t ulVal = NX_READ32(s_ptGpio->ulGpio_irq_mask_set);
  NX_WRITE32(s_ptGpio->ulGpio_irq_mask_set, ulVal | ((uint32_t)1<<uGpioNum));
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
*   Current gpio_irq_masked value of selected GPIO                           */
/*****************************************************************************/
int NX6_GPIO_GetIrq( unsigned int uGpioNum )
{
  return (NX_READ32(s_ptGpio->ulGpio_irq_masked) & ((uint32_t)1<<uGpioNum)) ? 1 : 0;
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
void NX6_GPIO_IrqReset( unsigned int uGpioNum )
{
  NX_WRITE32(s_ptGpio->ulGpio_irq_raw, (uint32_t)1<<uGpioNum);
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
void NX6_GPIO_Sleep( unsigned int uCounter,
                     uint32_t     ulTimeout )
{
  uint32_t ulVal;

  NX_WRITE32(s_ptGpio->aulGpio_counter_ctrl[uCounter], 0);          /* Clear the timer register         */
  NX_WRITE32(s_ptGpio->aulGpio_counter_cnt[uCounter] , 0);          /* Clear the current counter value  */
  NX_WRITE32(s_ptGpio->aulGpio_counter_max[uCounter] , ulTimeout * (NX6_DEV_FREQUENCY/1000000));  /* Set the counter value converted from microseconds to nanoseconds */
  NX_WRITE32(s_ptGpio->aulGpio_counter_ctrl[uCounter], (MSK_NX51_gpio_counter0_ctrl_run | MSK_NX51_gpio_counter0_ctrl_once) ); /* Enable the timer to one shot */
                                              
  /* poll timer Control for 'run' bit */
  do {
    ulVal  = NX_READ32(s_ptGpio->aulGpio_counter_ctrl[uCounter]);
  } while ( (ulVal & MSK_NX51_gpio_counter0_ctrl_run) != 0 );
}

/*****************************************************************************/
/*! GPIO Setup Timer
* \description
*   Setup a GPIO timer.
* \class 
*   GPIO 
* \params
*   uCounter          [in] Selected counter
*   ulTimeout         [in] The Time to wait in microseconds
*   uOnce             [in] timer once configuration
*   uIrqEn            [in] Timer Irq enable
* \return
*                                                                            */
/*****************************************************************************/
void NX6_GPIO_SetupTimer( unsigned int uCounter,
                          uint32_t     ulTimeout,
                          unsigned int uOnce,
                          unsigned int uIrqEn )
{
  NX_WRITE32(s_ptGpio->aulGpio_counter_ctrl[uCounter], 0);          /* Clear the timer register         */
  NX_WRITE32(s_ptGpio->aulGpio_counter_cnt[uCounter] , 0);          /* Clear the current counter value  */
  NX_WRITE32(s_ptGpio->aulGpio_counter_max[uCounter] , ulTimeout * (NX6_DEV_FREQUENCY/1000000));  /* Set the counter value converted form microseconds to nanoseconds */
  NX_WRITE32(s_ptGpio->aulGpio_counter_ctrl[uCounter],
     (MSK_NX51_gpio_counter0_ctrl_run
    |((uint32_t)uOnce<<SRT_NX51_gpio_counter0_ctrl_once)
    |((uint32_t)uIrqEn<<SRT_NX51_gpio_counter0_ctrl_irq_en))); /* Enable the timer to one shot */

}

/*****************************************************************************/
/*! GPIO IsTimerRunning
* \description
*   Checks for GPIO timer expired.
* \class 
*   GPIO 
* \params
*   uCounter          [in] Selected counter
* \return
*   Current timer_ctrl_run bit value                                         */
/*****************************************************************************/
bool NX6_GPIO_IsTimerRunning( unsigned int uCounter )
{
  /* gets timer Control 'run' bit */
  if( (NX_READ32(s_ptGpio->aulGpio_counter_ctrl[uCounter]) & MSK_NX51_gpio_counter0_ctrl_run) == 0 )
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
*   ulCounter          [in]   Selected Counter
* \return
*   Current counter_irq_masked value                                          */
/*****************************************************************************/
unsigned int NX6_GPIO_GetTimerIrq( unsigned int uCounter )
{
  return (NX_READ32(s_ptGpio->ulCnt_irq_masked) & ((uint32_t)1<<uCounter)) ? 1 : 0;
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
void NX6_GPIO_EnableTimerIrq( unsigned int uCounter )
{
  NX_WRITE32(s_ptGpio->ulCnt_irq_mask_set, (uint32_t)1<<uCounter);
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
void NX6_GPIO_DisableTimerIrq( unsigned int uCounter )
{
  NX_WRITE32(s_ptGpio->ulCnt_irq_mask_rst, (uint32_t)1<<uCounter);
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
void NX6_GPIO_ConfirmTimerIrq( unsigned int uCounter )
{
  NX_WRITE32(s_ptGpio->ulCnt_irq_raw, (uint32_t)1<<uCounter);
}
