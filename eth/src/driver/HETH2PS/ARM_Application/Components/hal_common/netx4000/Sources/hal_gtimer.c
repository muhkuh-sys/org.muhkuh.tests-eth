#include "hal_gtimer.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Definitions                                                              */
/*****************************************************************************/

#define MSK_GTIMER_CTRL_100MHZ      (HW_MSK(gtimer_ctrl_gtimer_en) | HW_MSK(gtimer_ctrl_irq_en) | ((PERIPHCLK_PER_10NS - 1) << HW_SRT(gtimer_ctrl_prescaler)))

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_GLOBAL_TIMER(s_ptGTimer)


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! GTIMER Start
* \description
*   This function starts an ARM timer..
* \class
*   GTIMER
* \params
*   eMode      [in]  Timer mode (auto reload/single shot)
*   ulVal      [in]  Timer value                                             */
/*****************************************************************************/
void GTimer_Start( uint64_t ulStartVal )
{
  uint32_t ulCtrl = s_ptGTimer->ulGtimer_ctrl & ~(HW_MSK(gtimer_ctrl_gtimer_en) | HW_MSK(gtimer_ctrl_prescaler));
  s_ptGTimer->ulGtimer_ctrl = ulCtrl;
  s_ptGTimer->aulGtimer_count[0] = 0;
  s_ptGTimer->aulGtimer_count[1] = 0;
  s_ptGTimer->ulGtimer_ctrl = ulCtrl | MSK_GTIMER_CTRL_100MHZ;
}

/*****************************************************************************/
/*! GTIMER Stop
* \description
*   This function stops and resets an ARM timer..
* \class
*   GTIMER
* \params                                                                    */
/*****************************************************************************/
void GTimer_Stop( void )
{
  s_ptGTimer->ulGtimer_ctrl &= ~HW_MSK(gtimer_ctrl_gtimer_en);
}

/*****************************************************************************/
/*! GTIMER Get Timer
* \description
*   This function reads the current timer value.
* \class
*   GTIMER
* \return
*   timer value                                                              */
/*****************************************************************************/
uint64_t GTimer_GetTimer( void )
{
  uint32_t ulValHi;
  uint32_t ulValLo;
  uint64_t ullVal;

  ullVal  = s_ptGTimer->aulGtimer_count[1];
  ulValLo = s_ptGTimer->aulGtimer_count[0];
  ulValHi = s_ptGTimer->aulGtimer_count[1];
  if( ullVal != ulValHi )
  {
    ullVal  = ulValHi;
    ulValLo = s_ptGTimer->aulGtimer_count[0];
  }

  return (ullVal << 32) + ulValLo;
}

void GTimer_StartCmp ( uint64_t ullCmpVal, uint32_t ulAutoInc )
{
  uint32_t ulCtrl = s_ptGTimer->ulGtimer_ctrl & ~HW_MSK(gtimer_ctrl_auto_inc);

  if( 0 != (ulCtrl & HW_MSK(gtimer_ctrl_comp_en)) )
  {
    s_ptGTimer->ulGtimer_ctrl = ulCtrl & ~HW_MSK(gtimer_ctrl_comp_en);
  }

  s_ptGTimer->ulGtimer_auto_inc = ulAutoInc;
  s_ptGTimer->aulGtimer_comp[0] = (uint32_t)ullCmpVal;
  s_ptGTimer->aulGtimer_comp[1] = (uint32_t)(ullCmpVal >> 32);

  if( ulAutoInc != 0 )
  {
    ulCtrl |= HW_MSK(gtimer_ctrl_auto_inc);
  }

  s_ptGTimer->ulGtimer_ctrl = ulCtrl | HW_MSK(gtimer_ctrl_comp_en);
}

void GTimer_StopCmp ( void )
{
  s_ptGTimer->ulGtimer_ctrl &= ~HW_MSK(gtimer_ctrl_comp_en);
}

/*****************************************************************************/
/*! GTIMER Get IRQ Raw
* \description
*   This function reads the raw interrupt status.
* \class
*   GTIMER
* \return
*   Interrupt bit mask                                                       */
/*****************************************************************************/
uint32_t GTimer_GetIrqRaw( void )
{
  return s_ptGTimer->ulGtimer_int;
}

/*****************************************************************************/
/*! GTIMER Confirm IRQ
* \description
*   This function resets the interrupt status.
* \class
*   GTIMER
* \params
*   ulIrq      [in]  Interrupt bit mask                                      */
/*****************************************************************************/
void GTimer_CnfIrq( uint32_t ulIrq )
{
  s_ptGTimer->ulGtimer_int = ulIrq;
}
