#include "hal_ptimer.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Definitions                                                              */
/*****************************************************************************/

#define MSK_PTIMER_CTRL_100MHZ      (HW_MSK(ptimer_ctrl_ptimer_en) | HW_MSK(ptimer_ctrl_irq_en) | ((PERIPHCLK_PER_10NS - 1) << HW_SRT(ptimer_ctrl_prescaler)))
#define MSK_PTIMER_CTRL_SINGLE_SHOT (MSK_PTIMER_CTRL_100MHZ)
#define MSK_PTIMER_CTRL_WATCHDOG    (MSK_PTIMER_CTRL_100MHZ | HW_MSK(wdg_ctrl_wdg_mode_en))
#define MSK_PTIMER_CTRL_AUTO_RELOAD (MSK_PTIMER_CTRL_100MHZ | HW_MSK(ptimer_ctrl_auto_reload_en))

typedef struct
{
  volatile uint32_t ulLoad;
  volatile uint32_t ulCount;
  volatile uint32_t ulCtrl;
  volatile uint32_t ulInt;
  volatile uint32_t aulReserved[4];
} HAL_PTIMER_T;


/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_PRIVATE_TIMER(s_ptPTimer)


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! PTIMER Start
* \description
*   This function starts an ARM timer..
* \class 
*   PTIMER
* \params
*   eMode      [in]  Timer mode (auto reload/single shot)
*   ulVal      [in]  Timer value                                             */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(PTimer_Start)( unsigned int uiTimer, PTIMER_CFG_MODE_E eMode, uint32_t ulVal )
{
  HAL_PTIMER_T* ptTimer = ((HAL_PTIMER_T*)s_ptPTimer) + uiTimer;
  ptTimer->ulCtrl = 0;
  ptTimer->ulLoad = ulVal;
  ptTimer->ulCtrl = (eMode == PTIMER_CFG_MODE_PRELOAD_AT_0) ?
      MSK_PTIMER_CTRL_AUTO_RELOAD : MSK_PTIMER_CTRL_SINGLE_SHOT;
}

/*****************************************************************************/
/*! PTIMER Stop
* \description
*   This function stops and resets an ARM timer..
* \class 
*   PTIMER
* \params                                                                    */
/*****************************************************************************/
void HALDEF(PTimer_Stop)( unsigned int uiTimer )
{
  HAL_PTIMER_T* ptTimer = ((HAL_PTIMER_T*)s_ptPTimer) + uiTimer;
  ptTimer->ulCtrl = 0;
}

/*****************************************************************************/
/*! PTIMER Sleep 10 ns
* \description
*   Sleep a while using an ARM timer.
* \class 
*   PTIMER
* \params
*   ulTimeout10Ns      [in]  The Time to wait in 10 ns
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(PTimer_Sleep)( unsigned int uiTimer, uint32_t ulTimeout10Ns )
{
  HAL_PTIMER_T* ptTimer = ((HAL_PTIMER_T*)s_ptPTimer) + uiTimer;
  ptTimer->ulCtrl = 0;
  ptTimer->ulLoad = ulTimeout10Ns;
  ptTimer->ulCtrl = MSK_PTIMER_CTRL_SINGLE_SHOT;
                                                
  /* poll timer finished */
  while ( ptTimer->ulCount != 0 );
}

/*****************************************************************************/
/*! PTIMER Get Timer
* \description
*   This function reads the current timer value.
* \class
*   PTIMER
* \return
*   timer value                                                              */
/*****************************************************************************/
uint32_t HALDEF(PTimer_GetTimer)( unsigned int uiTimer )
{
  HAL_PTIMER_T* ptTimer = ((HAL_PTIMER_T*)s_ptPTimer) + uiTimer;
  return ptTimer->ulCount;
}

/*****************************************************************************/
/*! PTIMER Get IRQ Raw
* \description
*   This function reads the raw interrupt status.
* \class
*   PTIMER
* \return
*   Interrupt bit mask                                                       */
/*****************************************************************************/
uint32_t HALDEF(PTimer_GetIrqRaw)( unsigned int uiTimer )
{
  HAL_PTIMER_T* ptTimer = ((HAL_PTIMER_T*)s_ptPTimer) + uiTimer;
  return ptTimer->ulInt;
}

/*****************************************************************************/
/*! PTIMER Confirm IRQ
* \description
*   This function resets the interrupt status.
* \class
*   PTIMER
* \params
*   ulIrq      [in]  Interrupt bit mask                                      */
/*****************************************************************************/
void HALDEF(PTimer_CnfIrq)( unsigned int uiTimer, uint32_t ulIrq )
{
  HAL_PTIMER_T* ptTimer = ((HAL_PTIMER_T*)s_ptPTimer) + uiTimer;
  ptTimer->ulInt = ulIrq;
}

/*****************************************************************************/
/*! Start
* \description
*   This function starts an ARM timer..
* \class
*   PWDG
* \params                                                                    */
/*****************************************************************************/
void HALDEF(PWdg_Start)( uint32_t ulVal )
{
  s_ptPTimer->ulWdg_ctrl = 0;
  s_ptPTimer->ulWdg_load = ulVal;
  s_ptPTimer->ulWdg_ctrl = MSK_PTIMER_CTRL_WATCHDOG;
}

/*****************************************************************************/
/*! Trigger
* \description
*   This function retriggers the watchdog
* \class
*   PWDG
* \params                                                                    */
/*****************************************************************************/
void HALDEF(PWdg_Trigger)( void )
{
  s_ptPTimer->ulWdg_load = s_ptPTimer->ulWdg_load;
}

/*****************************************************************************/
/*! Stop
* \description
*   This function stops and resets an ARM timer..
* \class
*   PWDG
* \params                                                                    */
/*****************************************************************************/
void HALDEF(PWdg_Stop)( void )
{
  s_ptPTimer->ulWdg_disable = 0x12345678;
  s_ptPTimer->ulWdg_disable = 0x87654321;
}
