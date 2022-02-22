
/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_rdyrun.h"
#include "hw_defines.h"


/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_ASIC_CTRL_COM(s_ptRdyRun)


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
#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(RDYRUN_SetRdyRunLed)( RDYRUN_LED_E tMode )
{
  switch( tMode ) {
  case RDYRUN_LED_OFF:
    s_ptRdyRun->ulRdy_run_cfg =   HW_MSK(rdy_run_cfg_RDY_DRV)
                                  | HW_MSK(rdy_run_cfg_RUN_DRV)
                                  | HW_MSK(rdy_run_cfg_RDY_POL) // low-active signal
                                  | HW_MSK(rdy_run_cfg_RUN_POL); // low-active signal
    break;
  case RDYRUN_LED_GREEN:
    s_ptRdyRun->ulRdy_run_cfg =   HW_MSK(rdy_run_cfg_RDY_DRV)
                                  | HW_MSK(rdy_run_cfg_RUN_DRV)
                                  | HW_MSK(rdy_run_cfg_RDY_POL) // low-active signal
                                  | HW_MSK(rdy_run_cfg_RUN_POL) // low-active signal
                                  | HW_MSK(rdy_run_cfg_RUN);
    break;
  case RDYRUN_LED_RED:
    s_ptRdyRun->ulRdy_run_cfg =   HW_MSK(rdy_run_cfg_RDY_DRV)
                                  | HW_MSK(rdy_run_cfg_RUN_DRV)
                                  | HW_MSK(rdy_run_cfg_RDY_POL) // low-active signal
                                  | HW_MSK(rdy_run_cfg_RUN_POL) // low-active signal
                                  | HW_MSK(rdy_run_cfg_RDY);
    break;
  case RDYRUN_LED_INV:
    s_ptRdyRun->ulRdy_run_cfg =   HW_MSK(rdy_run_cfg_RDY_DRV)
                                  | HW_MSK(rdy_run_cfg_RUN_DRV)
                                  | HW_MSK(rdy_run_cfg_RDY_POL) // low-active signal
                                  | HW_MSK(rdy_run_cfg_RUN_POL) // low-active signal
                                  | (s_ptRdyRun->ulRdy_run_cfg ^ HW_MSK(rdy_run_cfg_RDY))
                                  | (s_ptRdyRun->ulRdy_run_cfg ^ HW_MSK(rdy_run_cfg_RUN));
    break;
  }
}
