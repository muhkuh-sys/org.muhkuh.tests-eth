/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_gpio.h"
#include "hw_defines.h"

#if defined(__NETX4000)
#define HAL_GPIO_PIN_CNT 16
#define HAL_GPIO_CNT_CNT 4
#define HAL_GPIO_IF_CNT  5
#define ulGpio_cnt_irq_raw      ulCnt_irq_raw
#define ulGpio_cnt_irq_masked   ulCnt_irq_masked
#define ulGpio_cnt_irq_mask_set ulCnt_irq_mask_set
#define ulGpio_cnt_irq_mask_rst ulCnt_irq_mask_rst
#endif

#if defined(__NETX90)

#if defined(CPU_IS_ARM_APP)
#define HAL_GPIO_PIN_CNT 8
#define HAL_GPIO_CNT_CNT 3
#define HAL_GPIO_IF_CNT  2
#define aulGpio_cfg aulGpio_app_cfg
#define ulGpio_line ulGpio_app_line
#define ulGpio_in ulGpio_app_in
#define aulGpio_tc aulGpio_app_tc
#define ulGpio_irq_mask_set ulGpio_app_irq_mask_set
#define ulGpio_irq_mask_rst ulGpio_app_irq_mask_rst
#define ulGpio_irq_masked ulGpio_app_irq_masked
#define ulGpio_irq_raw ulGpio_app_irq_raw
#define aulGpio_counter_ctrl aulGpio_app_counter_ctrl
#define aulGpio_counter_cnt aulGpio_app_counter_cnt
#define aulGpio_counter_max aulGpio_app_counter_max
#define ulGpio_cnt_irq_mask_set ulGpio_app_cnt_irq_mask_set
#define ulGpio_cnt_irq_mask_rst ulGpio_app_cnt_irq_mask_rst
#define ulGpio_cnt_irq_raw ulGpio_app_cnt_irq_raw
#define ulGpio_cnt_irq_masked ulGpio_app_cnt_irq_masked

#else /* CPU_IS_ARM */
#define HAL_GPIO_PIN_CNT 4
#define HAL_GPIO_CNT_CNT 1
#define HAL_GPIO_IF_CNT  1

#endif /* defined(CPU_IS_ARM_APP) */
#endif /* defined(__NETX90) */

#if HAL_GPIO_CNT_CNT > 1
#define HAL_GPIO_COUNTER_CTRL(idx) aulGpio_counter_ctrl[idx]
#define HAL_GPIO_COUNTER_MAX(idx)  aulGpio_counter_max[idx]
#define HAL_GPIO_COUNTER_CNT(idx)  aulGpio_counter_cnt[idx]
#else
#define HAL_GPIO_COUNTER_CTRL(idx) ulGpio_counter0_ctrl
#define HAL_GPIO_COUNTER_MAX(idx)  ulGpio_counter0_max
#define HAL_GPIO_COUNTER_CNT(idx)  ulGpio_counter0_cnt
#endif

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_GPIO(s_aptGpio)

/* Interface selector */
static unsigned int s_uiIf = 0;

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
*   uiGpioPin         [in]   Selected GPIO
*   eMode             [in]   GPIO_MODE_OUTPUT      = Set the selected GPIO to Output mode
*                            GPIO_MODE_INPUT_READ  = Set the selected GPIO to Input mode
*   eInvert           [in]   GPIO_NOINVERT         = Does not invert the selected GPIO
*                            GPIO_INVERT           = Inverts the selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(GPIO_SetupMode)( unsigned int  uiGpioPin,
                             unsigned int  uiGpioCnt,
                             GPIO_MODE_E   eMode,
                             GPIO_INVERT_E eInvert )
{
#if defined(__NETX90)
  if(uiGpioCnt ==  GPIO_MODE_COUNT_SYSTIME) uiGpioCnt = 0x1; /* remap counter reference */
#endif // defined(__NETX90)
  s_aptGpio[s_uiIf]->aulGpio_cfg[uiGpioPin] = (
                                        ( (uint32_t) (eMode | eInvert) )
                                        | (uiGpioCnt << HW_SRT(gpio_cfg0_count_ref))
                                    );
}

/*****************************************************************************/
/*! GPIO Set Output
* \description
*   Enables/disable output for the selected GPIO.
* \class 
*   GPIO 
* \params
*   uiGpioPin          [in]   Selected GPIO
*   fEnable           [in]   0 = Disable the Output
*                            1 = Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_SetOutput)( unsigned int uiGpioPin, bool fEnable )
{
  if( fEnable )
    s_aptGpio[s_uiIf]->ulGpio_line |= (uint32_t)1 << uiGpioPin;
  else
    s_aptGpio[s_uiIf]->ulGpio_line &= ~((uint32_t)1 << uiGpioPin);
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
uint32_t HALDEF(GPIO_GetLine)( void )
{
  return ( s_aptGpio[s_uiIf]->ulGpio_line );
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
void HALDEF(GPIO_SetLine)( uint32_t ulVal )
{
  s_aptGpio[s_uiIf]->ulGpio_line = ulVal;
}

/*****************************************************************************/
/*! GPIO Get Input
* \description
*   Read selected GPIO.
* \class 
*   GPIO 
* \params
*   uiGpioPin          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
int HALDEF(GPIO_GetInput)( unsigned int uiGpioPin )
{
  return ( s_aptGpio[s_uiIf]->ulGpio_in & ((uint32_t)1 << uiGpioPin) ) ? 1 : 0;
}

/*****************************************************************************/
/*! GPIO Get TC
* \description
*   Read selected GPIO TC register.
* \class
*   GPIO
* \params
*   uiGpioPin          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(GPIO_GetTc)( unsigned int uiGpioPin )
{
  return ( s_aptGpio[s_uiIf]->aulGpio_tc[uiGpioPin] );
}

/*****************************************************************************/
/*! GPIO Set TC
* \description
*   Read selected GPIO TC register.
* \class
*   GPIO
* \params
*   uiGpioPin          [in]   Selected GPIO
*   uTc               [in]   TC value
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_SetTc)( unsigned int uiGpioPin, uint32_t ulTC )
{
  s_aptGpio[s_uiIf]->aulGpio_tc[uiGpioPin] = ulTC;
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
uint32_t HALDEF(GPIO_GetIn)( void )
{
  return ( s_aptGpio[s_uiIf]->ulGpio_in );
}

/*****************************************************************************/
/*! GPIO IRQ Enable
* \description
*   Enable interrupt request for the selected GPIOs.
* \class 
*   GPIO 
* \params
*   ulIrqMsk          [in]   IRQ Bit vector
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_IrqEnable)( uint32_t ulIrqMsk )
{
  s_aptGpio[s_uiIf]->ulGpio_irq_mask_set = ulIrqMsk;
}

/*****************************************************************************/
/*! GPIO IRQ Disable
* \description
*   Disable interrupt request for the selected GPIOs.
* \class 
*   GPIO 
* \params
*   ulIrqMsk          [in]   IRQ Bit vector
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_IrqDisable)( uint32_t ulIrqMsk )
{
  s_aptGpio[s_uiIf]->ulGpio_irq_mask_rst = ulIrqMsk;
}

/*****************************************************************************/
/*! GPIO Get IRQ
* \description
*   Read interrupt status of the selected GPIO.
* \class
*   GPIO                                                                     */
/*****************************************************************************/
uint32_t HALDEF(GPIO_GetIrq)( void )
{
  return s_aptGpio[s_uiIf]->ulGpio_irq_masked;
}

/*****************************************************************************/
/*! GPIO Get Raw IRQ
* \description
*   Read interrupt status of the selected GPIO.
* \class
*   GPIO                                                                     */
/*****************************************************************************/
uint32_t HALDEF(GPIO_GetIrqRaw)( void )
{
  return s_aptGpio[s_uiIf]->ulGpio_irq_raw;
}

/*****************************************************************************/
/*! GPIO IRQ Reset
* \description
*   Writes 1 to reset the IRQ of the selected GPIO.
* \class 
*   GPIO 
* \params
*   ulIrqMsk          [in]   IRQ Bit vector
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_IrqReset)( uint32_t ulIrqMsk )
{
  s_aptGpio[s_uiIf]->ulGpio_irq_raw = ulIrqMsk;
}

/*****************************************************************************/
/*! GPIO Sleep
* \description
*   Sleep a while using a GPIO timer.
* \class 
*   GPIO 
* \params
*   uiGpioPin          [in]   Selected GPIO
*   ulTimeout         [out]  The Time to wait in µs
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_Sleep)( unsigned int uiGpioCnt, uint32_t ulTimeout )
{
  uint32_t ulVal;

  /* Convert counter value from µs to ns */
  ulTimeout = ulTimeout * (HW_DEV_FREQUENCY/1000000);
  
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt)  = 0;          /* Clear the timer register         */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CNT(uiGpioCnt)   = 0;          /* Clear the current counter value  */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_MAX(uiGpioCnt)   = ulTimeout;  /* Set the counter value            */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt) |= ( HW_MSK(gpio_counter0_ctrl_run)
                                               |HW_MSK(gpio_counter0_ctrl_once) /* Enable the timer to one shot */
                                              );
                                              
  /* poll timer CTRL for 'run' bit */
  do {
    ulVal  = s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt);
    ulVal &= HW_MSK(gpio_counter0_ctrl_run);
  } while ( ulVal != 0 );
}

/*****************************************************************************/
/*! GPIO Reset Timer
* \description
*   Resets a GPIO timer.
* \class
*   GPIO
* \params
*   uiGpioCnt          [in]  Selected Counter
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_ResetTimer)( unsigned int uiGpioCnt )
{
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt)  = 0;          /* Clear the timer register         */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CNT(uiGpioCnt)   = 1;          /* avoid generation of Signal "counter_zero" */
}

/*****************************************************************************/
/*! GPIO Setup Timer
* \description
*   Setup a GPIO timer.
* \class 
*   GPIO 
* \params
*   uiGpioCnt          [in]  Selected Counter
*   ulTimeout          [in]  The Time to wait in MicroSeconds
*   fOnce              [in]  Run once or continuously
*   fIrqEn             [in]  Enable/disable interrupt  
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_SetupTimer)( unsigned int uiGpioCnt, uint32_t ulTimeout, bool fOnce, bool fIrqEn )
{
  /* Convert counter value from us to ns */
  ulTimeout = ulTimeout * (HW_DEV_FREQUENCY/1000000);
  
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt)  = 0;          /* Clear the timer register         */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CNT(uiGpioCnt)   = 0;          /* Clear the current counter value  */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_MAX(uiGpioCnt)   = ulTimeout;  /* Set the counter value            */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt) = ( HW_MSK(gpio_counter0_ctrl_run)
                                                       |((uint32_t)fOnce  << HW_SRT(gpio_counter0_ctrl_once)) /* Enable the timer to one shot */
                                                       |((uint32_t)fIrqEn << HW_SRT(gpio_counter0_ctrl_irq_en))
                                                       );
}

/*****************************************************************************/
/*! GPIO Setup Timer in Clock Cycles
* \description
*   Setup a GPIO timer.
* \class
*   GPIO
* \params
*   uiGpioCnt          [in]  Selected Counter
*   ulTimeout          [in]  The Time to wait in MicroSeconds
*   fOnce              [in]  Run once or continuously
*   fIrqEn             [in]  Enable/disable interrupt
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_SetupTimerCC)( unsigned int uiGpioCnt, uint32_t ulTimeout, bool fOnce, bool fIrqEn )
{
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_MAX(uiGpioCnt)   = ulTimeout;  /* Set the counter value            */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt) = ( HW_MSK(gpio_counter0_ctrl_run)
                                                       |((uint32_t)fOnce  << HW_SRT(gpio_counter0_ctrl_once)) /* Enable the timer to one shot */
                                                       |((uint32_t)fIrqEn << HW_SRT(gpio_counter0_ctrl_irq_en))
                                                       );
}

/*****************************************************************************/
/*! GPIO IsTimerRunning
* \description
*   Checks for GPIO timer expired.
* \class 
*   GPIO 
* \params
*   uiGpioPin          [in]  Selected GPIO
* \return
*    0
*    1                                                                       */
/*****************************************************************************/
int HALDEF(GPIO_IsTimerRunning)( unsigned int uiGpioCnt )
{
  /* gets timer CTRL 'run' bit */
  if( (s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt) & HW_MSK(gpio_counter0_ctrl_run)) == 0 )
    return 0;
  else
    return 1;
}

/*****************************************************************************/
/*! GPIO Timer Count Value
* \description
*   Returns the current counter value of the selected Counter.
* \class
*   GPIO
* \params
*   uiGpioPin          [in]   Selected Counter
* \return
*   Counter value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(GPIO_GetTimerCnt)( unsigned int uiGpioCnt )
{
  return s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CNT(uiGpioCnt);
}

/*****************************************************************************/
/*! GPIO Timer IRQ Enable
* \description
*   Enable interrupt request for the selected Counter.
* \class 
*   GPIO 
* \params
*   ulIrqMsk          [in]   IRQ Bit vector
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_TimerIrqEnable)( uint32_t ulIrqMsk )
{
  s_aptGpio[s_uiIf]->ulGpio_cnt_irq_mask_set = ulIrqMsk;
}

/*****************************************************************************/
/*! GPIO Timer Irq Disable
* \description
*   Disable interrupt request for the selected Counter..
* \class 
*   GPIO 
* \params
*   ulIrqMsk          [in]   IRQ Bit vector
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_TimerIrqDisable)( uint32_t ulIrqMsk )
{
  s_aptGpio[s_uiIf]->ulGpio_cnt_irq_mask_rst = ulIrqMsk;
}

/*****************************************************************************/
/*! GPIO Timer IRQ Reset
* \description
*   Writes 1 to reset the IRQ of the selected Counter.
* \class 
*   GPIO 
* \params
*   ulIrqMsk          [in]   IRQ Bit vector
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_TimerIrqReset)( uint32_t ulIrqMsk )
{
  s_aptGpio[s_uiIf]->ulGpio_cnt_irq_raw = ulIrqMsk;
}

/*****************************************************************************/
/*! GPIO Get Timer IRQ
* \description
*   Read interrupt status of the GPIO timers.
* \class
*   GPIO                                                                     */
/*****************************************************************************/
uint32_t HALDEF(GPIO_GetTimerIrq)( void )
{
  return s_aptGpio[s_uiIf]->ulGpio_cnt_irq_masked;
}

/*****************************************************************************/
/*! GPIO Get Raw Timer IRQ
* \description
*   Read interrupt status of the GPIO timers.
* \class
*   GPIO                                                                     */
/*****************************************************************************/
uint32_t HALDEF(GPIO_GetTimerIrqRaw)( void )
{
  return s_aptGpio[s_uiIf]->ulGpio_cnt_irq_raw;
}

/*****************************************************************************/
/*! GPIO Setup Blink Mode
* \description
*   Configure blink mode for the selected GPIO.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
*   ulMode             [in]   GPIO_MODE_OUTPUT      = Set the selected GPIO to Output mode
*                             GPIO_MODE_INPUT_READ  = Set the selected GPIO to Input mode
*   ulInvert           [in]   GPIO_NOINVERT         = Does not invert the selected GPIO
*                             GPIO_INVERT           = Inverts the selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
GPIO_RESULT_E HALDEF(GPIO_SetupBlinkMode)( unsigned int      uiGpioPin,
                                           unsigned int      uiGpioCnt,
                                           GPIO_INVERT_E     eInvert,
                                           GPIO_BLINK_ONCE_E eBlinkOnce,
                                           unsigned int      uBlinkLen,
                                           uint32_t          ulBlinkPattern )
{

  if( uBlinkLen > 32 ) return GPIO_ERR_INVAL_PARAM;
  if( uBlinkLen == 0 ) return GPIO_ERR_INVAL_PARAM;
  if( uiGpioPin >= HAL_GPIO_PIN_CNT ) return GPIO_ERR_INVAL_PARAM;
  if( uiGpioCnt >= HAL_GPIO_CNT_CNT ) return GPIO_ERR_INVAL_PARAM;

  s_aptGpio[s_uiIf]->aulGpio_cfg[uiGpioPin] = ((uint32_t) (GPIO_MODE_OUTPUT_SET_TO_0 |eInvert));

  /* set pattern */
  s_aptGpio[s_uiIf]->aulGpio_tc[uiGpioPin] = ulBlinkPattern;

  /* configured GPIO in blink mode */
  s_aptGpio[s_uiIf]->aulGpio_cfg[uiGpioPin] = (
                                         (uint32_t) (GPIO_MODE_OUTPUT_BLINK_MODE |eInvert)
                                        |(uint32_t) (uiGpioCnt    << HW_SRT(gpio_cfg0_count_ref))
                                        |(uint32_t) (eBlinkOnce   << HW_SRT(gpio_cfg0_blink_once))
                                        |(uint32_t) ((uBlinkLen-1)<< HW_SRT(gpio_cfg0_blink_len))
                                    );


  return GPIO_OKAY;
}

/*****************************************************************************/
/*! GPIO Start Blink Mode
* \description
*   Starts a GPIO counter that drives GPIOs configured in Blink Mode.
* \class
*   GPIO
* \params
*   uiGpioCnt          [in]  uiGpioCnt
*   ulTimeout         [in]  Time quantum in microseconds
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(GPIO_StartBlinkMode)( unsigned int uiGpioCnt, uint32_t ulTimeout )
{
  /* Convert counter value from µs to ns */
  ulTimeout = ulTimeout * (HW_DEV_FREQUENCY/1000000);

  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_MAX(uiGpioCnt)   = ulTimeout-1;  /* Set the counter value            */
  s_aptGpio[s_uiIf]->HAL_GPIO_COUNTER_CTRL(uiGpioCnt)  = HW_MSK(gpio_counter0_ctrl_run); /* Start timer */
}

/*****************************************************************************/
/*! GPIO Select Interface
* \description
*   Selects the hardware interface to be used in all subsequent operations.
* \class
*   GPIO
* \params
*   uiIf          [in]  interface
* \return
*                                                                            */
/*****************************************************************************/
GPIO_RESULT_E HALDEF(GPIO_SelIf)( unsigned int uiIf )
{
  if( uiIf > HAL_GPIO_IF_CNT )
    return GPIO_ERR_INVAL_PARAM;

  s_uiIf = uiIf;

  return GPIO_OKAY;
}
