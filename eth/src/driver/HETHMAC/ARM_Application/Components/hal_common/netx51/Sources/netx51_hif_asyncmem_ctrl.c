/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_hif_asyncmem_ctrl.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_HIF_ASYNCMEM_CTRL

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! HIF_ASYNCMEM_CTRL DeInitialize
* \description
*   This function resets the HIF ASYNC MEMORY Control Area to default values.
* \class
*   HIF_ASYNCMEM_CTRL Service Class
* \params
* \return                                                                    */
/*****************************************************************************/
void NX51_HIF_ASYNCMEM_CTRL_Deinit ( void )
{
  s_ptHifAsyncMemCtrl->aulExtsram_ctrl[0] = DFLT_VAL_NX51_extsram0_ctrl;
  s_ptHifAsyncMemCtrl->aulExtsram_ctrl[1] = DFLT_VAL_NX51_extsram0_ctrl;
  s_ptHifAsyncMemCtrl->aulExtsram_ctrl[2] = DFLT_VAL_NX51_extsram0_ctrl;
  s_ptHifAsyncMemCtrl->ulExt_cs0_apm_ctrl = DFLT_VAL_NX51_ext_cs0_apm_ctrl;
  s_ptHifAsyncMemCtrl->ulExt_rdy_cfg      = DFLT_VAL_NX51_ext_rdy_cfg;
}

/*****************************************************************************/
/*! HIF_ASYNCMEM_CTRL Setup global
* \description
*   This function configures the RDY behavior.
* \class
*   HIF_ASYNCMEM_CTRL Service Class
* \params
*   uRdyActLevel     [in] 1/0: RDY is active high/low
*   eRdyFilter       [in] Number of times the RDY signal is sampled
*   uDisRdyTimeout   [in] 1: Disable RDY timeout
*   uEnRdyTimeOutIrq [in] 1: Enable RDY Timeout IRQ
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HIF_ASYNCMEM_CTRL_SetRdyCfg ( unsigned int                        uRdyActLevel,
                                       NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_E eRdyFilter,
                                       unsigned int                        uDisRdyTimeout,
                                       unsigned int                        uEnRdyTimeOutIrq )
{
  if( (int)eRdyFilter > 0x3 )
    return -1;

  /* configure RDY signal */
  s_ptHifAsyncMemCtrl->ulExt_rdy_cfg =  ((uint32_t)uRdyActLevel     << SRT_NX51_ext_rdy_cfg_rdy_act_level)
                                       |((uint32_t)eRdyFilter       << SRT_NX51_ext_rdy_cfg_rdy_filter)
                                       |((uint32_t)uDisRdyTimeout   << SRT_NX51_ext_rdy_cfg_rdy_to_dis)
                                       |((uint32_t)uEnRdyTimeOutIrq << SRT_NX51_ext_rdy_cfg_rdy_to_irq_en);
  return 0;
}

/*****************************************************************************/
/*! HIF_ASYNCMEM_CTRL Configure CS area
* \description
*   This function configures a Chip-Select Area of the bus interface.
* \class
*   HIF_ASYNCMEM_CTRL Service Class
* \params
*   uCsNum             [in] Chip-Select area
*   eDataWidth         [in] 0/1/2/3: 8 bit/16 bit/32 bit/disabled interface
*   uNumWs             [in] number of wait states
*   uPrePause          [in] Pre-Pause (0 - 3 cycles)
*   uPostPause         [in] Post-Pause (0 - 3 cycles)
*   uDisPrePauseSeqRd  [in] 1: No Pre-Pause insertion between sequential reads
*   uDisPostPauseSeqRd [in] 1: No Post-Pause insertion between sequential reads
*   uStaticCs          [in] 1: Static chip-select signal generation enabled (e.g. for i80 displays)
*   uEnRdy             [in] 1/0: Use external ready input to stretch Wait-State phase / Access timing is only controlled by Wait-State and Pre/Post-Pause configuration above
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HIF_ASYNCMEM_CTRL_SetupCsArea ( unsigned int uCsNum,
                                         NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_E eDataWidth,
                                         unsigned int uNumWs,
                                         unsigned int uPrePause,
                                         unsigned int uPostPause,
                                         unsigned int uDisPrePauseSeqRd,
                                         unsigned int uDisPostPauseSeqRd,
                                         unsigned int uStaticCs,
                                         unsigned int uEnRdy )
{
  /* Plausibility check */
  if( uCsNum > 0x3 )
    return -1;
  if( (int)eDataWidth > 0x3 )
    return -1;
  if( uNumWs > 63 )
    return -1;
  if( uPrePause > 3 )
    return -1;
  if( uPostPause > 3 )
    return -1;

  /* configure CS area */
  s_ptHifAsyncMemCtrl->aulExtsram_ctrl[uCsNum] =  ((uint32_t)eDataWidth         << SRT_NX51_extsram0_ctrl_dwidth)
                                                 |((uint32_t)uNumWs             << SRT_NX51_extsram0_ctrl_ws)
                                                 |((uint32_t)uPrePause          << SRT_NX51_extsram0_ctrl_p_pre)
                                                 |((uint32_t)uPostPause         << SRT_NX51_extsram0_ctrl_p_post)
                                                 |((uint32_t)uDisPrePauseSeqRd  << SRT_NX51_extsram0_ctrl_no_p_pre_seq_rd)
                                                 |((uint32_t)uDisPostPauseSeqRd << SRT_NX51_extsram0_ctrl_no_p_post_seq_rd)
                                                 |((uint32_t)uStaticCs          << SRT_NX51_extsram0_ctrl_static_cs)
                                                 |((uint32_t)uEnRdy             << SRT_NX51_extsram0_ctrl_ready_en);

  return 0;
}


