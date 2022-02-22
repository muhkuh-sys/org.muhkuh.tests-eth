
/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_rdyrun.h"
#include "hw_defines.h"


/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

#if defined(__NETX90)
HW_PTR_ASIC_CTRL_COM(s_ptAsicCtrl)
#else
HW_PTR_ASIC_CTRL(s_ptAsicCtrl)
#endif

#define RDY_RUN_CFG_DFLT ( HW_MSK(rdy_run_cfg_RDY_DRV)\
                         | HW_MSK(rdy_run_cfg_RUN_DRV)\
                         | HW_MSK(rdy_run_cfg_RDY_POL)  /* low-active signal */\
                         | HW_MSK(rdy_run_cfg_RUN_POL) )/* low-active signal */

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Set RDYRUN Led
* \description
*   Set RDY/RUN led depending of mode.
* \class
*   RDYRUN
* \params
*   tMode          [in]  Ready/Run Mode (off, green, red, inv) to set
* \return
*                                                                            */
/*****************************************************************************/
void RDYRUN_SetRdyRunLed( RDYRUN_LED_E eMode )
{
  switch( eMode )
  {
  case RDYRUN_LED_OFF:
    s_ptAsicCtrl->ulRdy_run_cfg = RDY_RUN_CFG_DFLT;
    break;

  case RDYRUN_LED_GREEN:
    s_ptAsicCtrl->ulRdy_run_cfg = RDY_RUN_CFG_DFLT | HW_MSK(rdy_run_cfg_RUN);
    break;

  case RDYRUN_LED_RED:
    s_ptAsicCtrl->ulRdy_run_cfg = RDY_RUN_CFG_DFLT | HW_MSK(rdy_run_cfg_RDY);
    break;

  case RDYRUN_LED_INV:
    s_ptAsicCtrl->ulRdy_run_cfg ^= (HW_MSK(rdy_run_cfg_RDY)
                                   |HW_MSK(rdy_run_cfg_RUN));
    break;
  }
}
