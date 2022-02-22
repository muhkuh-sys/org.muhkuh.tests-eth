/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_rdyrun.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_ASIC_CTRL

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
*   eMode          [in]  Ready/Run Mode (off, green, red, inv) to set
* \return
*                                                                            */
/*****************************************************************************/
void NX6_RDYRUN_SetRdyRunLed( NX6_RDYRUN_LED_E eMode )
{
  uint32_t ulVal;
  switch( eMode ) {
  case NX6_RDYRUN_LED_OFF:
    NX_WRITE32(s_ptAsicCtrl->ulRdy_run_cfg, MSK_NX51_rdy_run_cfg_RDY_DRV
                                          | MSK_NX51_rdy_run_cfg_RUN_DRV
                                          | MSK_NX51_rdy_run_cfg_RDY_POL   /* low-active signal */
                                          | MSK_NX51_rdy_run_cfg_RUN_POL); /* low-active signal */
    break;
  case NX6_RDYRUN_LED_GREEN:
    NX_WRITE32(s_ptAsicCtrl->ulRdy_run_cfg, MSK_NX51_rdy_run_cfg_RDY_DRV
                                          | MSK_NX51_rdy_run_cfg_RUN_DRV
                                          | MSK_NX51_rdy_run_cfg_RDY_POL /* low-active signal */
                                          | MSK_NX51_rdy_run_cfg_RUN_POL /* low-active signal */
                                          | MSK_NX51_rdy_run_cfg_RUN);
    break;
  case NX6_RDYRUN_LED_RED:
    NX_WRITE32(s_ptAsicCtrl->ulRdy_run_cfg, MSK_NX51_rdy_run_cfg_RDY_DRV
                                          | MSK_NX51_rdy_run_cfg_RUN_DRV
                                          | MSK_NX51_rdy_run_cfg_RDY_POL /* low-active signal */
                                          | MSK_NX51_rdy_run_cfg_RUN_POL /* low-active signal */
                                          | MSK_NX51_rdy_run_cfg_RDY);
    break;
  case NX6_RDYRUN_LED_INV:
    ulVal = NX_READ32(s_ptAsicCtrl->ulRdy_run_cfg);
    NX_WRITE32(s_ptAsicCtrl->ulRdy_run_cfg, MSK_NX51_rdy_run_cfg_RDY_DRV
                                          | MSK_NX51_rdy_run_cfg_RUN_DRV
                                          | MSK_NX51_rdy_run_cfg_RDY_POL /* low-active signal */
                                          | MSK_NX51_rdy_run_cfg_RUN_POL /* low-active signal */
                                          | (ulVal ^ (MSK_NX51_rdy_run_cfg_RDY
                                                     |MSK_NX51_rdy_run_cfg_RUN)));
    break;
  }
}
