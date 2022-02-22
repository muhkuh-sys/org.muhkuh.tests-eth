/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_fmmusm.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_FMMUSM

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! FMMUSM Set parameter
* \description
*   This function sets a FMMUSM parameter.
* \class 
*   FMMUSM Service Class
* \params
*   ePrmID           [in]  Parameter ID
*   ulVal            [in]  Value to set
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int NX6_FMMUSM_SetPrm( NX6_FMMUSM_PRM_E ePrmID,
                       uint32_t         ulVal )
{
  uint32_t ulReg;
  int iRslt = 0;

  switch( ePrmID )
  { 
    case NX6_FMMUSM_PRM_PHYS_ADDR_OFFSET:
      ulReg = NX_READ32(s_ptFmmuSm->ulPhys_addr_offset) & ~MSK_NX51_phys_addr_offset_phys_addr_offset;
      NX_WRITE32(s_ptFmmuSm->ulPhys_addr_offset, ulReg | ((ulVal&0xffff)<<SRT_NX51_phys_addr_offset_phys_addr_offset));
    break;
    case NX6_FMMUSM_PRM_PHYS_ADDR_REG_OFFSET:
      ulReg = NX_READ32(s_ptFmmuSm->ulPhys_addr_offset) & ~MSK_NX51_phys_addr_offset_phys_addr_offset_register_area;
      NX_WRITE32(s_ptFmmuSm->ulPhys_addr_offset, ulReg | ((ulVal&0xffff)<<SRT_NX51_phys_addr_offset_phys_addr_offset_register_area));
    break;
    case NX6_FMMUSM_PRM_PHYS_LAST_ADDR: 
      NX_WRITE32(s_ptFmmuSm->ulPhys_last_addr, ulVal);
    break;
    case NX6_FMMUSM_PRM_XPEC_NR:
      NX_WRITE32(s_ptFmmuSm->ulFmmusm_xpec_nr, ulVal);
    break;
    case NX6_FMMUSM_PRM_SM_BUF_STATCFG:
      NX_WRITE32(s_ptFmmuSm->ulSm_buf_statcfg, ulVal);
    break;
    case NX6_FMMUSM_PRM_SM_READ_EVENT_VECTOR:
      NX_WRITE32(s_ptFmmuSm->ulSm_read_event, ulVal);
    break;
    case NX6_FMMUSM_PRM_SM_WRITE_EVENT_VECTOR:
      NX_WRITE32(s_ptFmmuSm->ulSm_write_event, ulVal);
    break;
    case NX6_FMMUSM_PRM_SM_FIRST_BYTE_ADDRESSED:
      NX_WRITE32(s_ptFmmuSm->ulSm_first_byte_addressed, ulVal);
    break;
    case NX6_FMMUSM_PRM_SM_LAST_BYTE_ADDRESSED:
      NX_WRITE32(s_ptFmmuSm->ulSm_last_byte_addressed, ulVal);
    break;
    case NX6_FMMUSM_PRM_SM_SERVED:
      NX_WRITE32(s_ptFmmuSm->ulSm_served, ulVal);
    break;
    default: iRslt = -1; break;
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! FMMUSM Get parameter
* \description
*   This function gets a FMMUSM parameter.
* \class 
*   FMMUSM Service Class
* \params
*   uPrmID          [in]  Parameter ID
* \return
*   FMMUSM parameter                                                         */
/*****************************************************************************/
uint32_t NX6_FMMUSM_GetPrm( NX6_FMMUSM_PRM_E ePrmID )
{
  uint32_t ulRetVal;
  
  switch( ePrmID )
  { 
    case NX6_FMMUSM_PRM_PHYS_ADDR_OFFSET:
      ulRetVal = (NX_READ32(s_ptFmmuSm->ulPhys_addr_offset) & MSK_NX51_phys_addr_offset_phys_addr_offset)>>SRT_NX51_phys_addr_offset_phys_addr_offset;
    break;
    case NX6_FMMUSM_PRM_PHYS_ADDR_REG_OFFSET:
      ulRetVal = (NX_READ32(s_ptFmmuSm->ulPhys_addr_offset) & MSK_NX51_phys_addr_offset_phys_addr_offset_register_area)>>SRT_NX51_phys_addr_offset_phys_addr_offset_register_area;
    break;
    case NX6_FMMUSM_PRM_PHYS_LAST_ADDR:
      ulRetVal = NX_READ32(s_ptFmmuSm->ulPhys_last_addr);
    break;
    case NX6_FMMUSM_PRM_XPEC_NR:
      ulRetVal = NX_READ32(s_ptFmmuSm->ulFmmusm_xpec_nr);
    break;
    case NX6_FMMUSM_PRM_SM_BUF_STATCFG:
      ulRetVal = NX_READ32(s_ptFmmuSm->ulSm_buf_statcfg);
    break;
    case NX6_FMMUSM_PRM_SM_READ_EVENT_VECTOR:
      ulRetVal = NX_READ32(s_ptFmmuSm->ulSm_read_event);
    break;
    case NX6_FMMUSM_PRM_SM_WRITE_EVENT_VECTOR:
      ulRetVal = NX_READ32(s_ptFmmuSm->ulSm_write_event);
    break;
    case NX6_FMMUSM_PRM_SM_FIRST_BYTE_ADDRESSED:
      ulRetVal = NX_READ32(s_ptFmmuSm->ulSm_first_byte_addressed);
    break;
    case NX6_FMMUSM_PRM_SM_LAST_BYTE_ADDRESSED:
      ulRetVal = NX_READ32(s_ptFmmuSm->ulSm_last_byte_addressed);
    break;
    case NX6_FMMUSM_PRM_SM_SERVED:
      ulRetVal = NX_READ32(s_ptFmmuSm->ulSm_served);
    break;
    default: ulRetVal = 0; break;
  } /* end switch */

  return ulRetVal;
}

/*****************************************************************************/
/*! FMMUSM Configure FMMU channel
* \description
*   This function configures a FMMU channel.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
*   ptCfg    [in] Pointer to FMMU configuration
* \return
*   0 ok
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_SetFmmuCfg ( unsigned int        uChNum,
                            NX6_FMMUSM_FMMUCFG* ptCfg )
{
  uint32_t ulVal;

  if( uChNum >= NX6_FMMU_CH_MAX_NUM )
    return -1;

  /* disable channel */
  NX_WRITE32(s_ptFmmuSm->asFmmu[uChNum].ulCfg_enable, 0);

  /* configure channel */
  NX_WRITE32(s_ptFmmuSm->asFmmu[uChNum].ulCfg_log_startaddr, ptCfg->ulLogStartAddr<<SRT_NX51_fmmu0_cfg_log_startaddr_log_startaddr);
  
  NX_WRITE32(s_ptFmmuSm->asFmmu[uChNum].ulCfg_length,  (uint32_t)ptCfg->usLen<<SRT_NX51_fmmu0_cfg_length_length
                                                      |(uint32_t)ptCfg->bLogStartBit<<SRT_NX51_fmmu0_cfg_length_log_start_bit
                                                      |(uint32_t)ptCfg->bLogStopBit<<SRT_NX51_fmmu0_cfg_length_log_stop_bit
            );

  NX_WRITE32(s_ptFmmuSm->asFmmu[uChNum].ulCfg_phys_startaddr, (uint32_t)ptCfg->usPhyStartAddr<<SRT_NX51_fmmu0_cfg_phys_startaddr_phys_startaddr
                                                             |(uint32_t)ptCfg->bPhyStartBit<<SRT_NX51_fmmu0_cfg_phys_startaddr_phys_start_bit
                                                             |(uint32_t)ptCfg->bRdEn<<SRT_NX51_fmmu0_cfg_phys_startaddr_proc_rd_enable
                                                             |(uint32_t)ptCfg->bWrEn<<SRT_NX51_fmmu0_cfg_phys_startaddr_proc_wr_enable
            );

  /* enable channel if requested */
  ulVal = NX_READ32(s_ptFmmuSm->asFmmu[uChNum].ulCfg_enable);
  if(ptCfg->fEn) {
    ulVal |= MSK_NX51_fmmu0_cfg_enable_proc_enable;
  } else {
    ulVal &= ~MSK_NX51_fmmu0_cfg_enable_proc_enable;
  }
  NX_WRITE32(s_ptFmmuSm->asFmmu[uChNum].ulCfg_enable, ulVal);

  return 0;
}

/*****************************************************************************/
/*! FMMUSM Configure SM channel
* \description
*   This function configures a Sync Manager channel.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
*   ptCfg    [in] Pointer to SM configuration structure
* \return
*   0 ok
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_SetSmCfg ( unsigned int      uChNum,
                          NX6_FMMUSM_SMCFG* ptCfg )
{
  uint32_t ulVal;

  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  /* disable channel */
  NX_WRITE32(s_ptFmmuSm->asSm[uChNum].ulCfg_mode, 0);

  /* configure channel */
  NX_WRITE32(s_ptFmmuSm->asSm[uChNum].ulCfg_adr_len, (uint32_t)ptCfg->usPhyStartAddr<<SRT_NX51_sm0_cfg_adr_len_startaddr
                                                    |(uint32_t)ptCfg->usLen<<SRT_NX51_sm0_cfg_adr_len_length
            );

  NX_WRITE32(s_ptFmmuSm->asSm[uChNum].ulCfg_mode, (uint32_t)ptCfg->bMode<<SRT_NX51_sm0_cfg_mode_buf_method
                                                 |(uint32_t)ptCfg->bDir<<SRT_NX51_sm0_cfg_mode_write_read
            );

  /* set Add to align SyncMan buffer to DWORD addresses */
  ulVal = NX_READ32(s_ptFmmuSm->ulPhys_last_addr) & ~(MSK_NX51_phys_last_addr_sm0_cfg_add<<(2*uChNum));
  NX_WRITE32(s_ptFmmuSm->ulPhys_last_addr, ulVal
                                           |(uint32_t)ptCfg->bAdd<<(SRT_NX51_phys_last_addr_sm0_cfg_add+2*uChNum)
            );

  /* enable channel if requested */
  ulVal = NX_READ32(s_ptFmmuSm->asFmmu[uChNum].ulCfg_enable);
  if(ptCfg->fEn) {
    ulVal |= MSK_NX51_fmmu0_cfg_enable_proc_enable;
  } else {
    ulVal &= ~MSK_NX51_fmmu0_cfg_enable_proc_enable;
  }
  NX_WRITE32(s_ptFmmuSm->asFmmu[uChNum].ulCfg_enable, ulVal);
  
  return 0;
}

/*****************************************************************************/
/*! FMMUSM Return a SM channel configuration
* \description
*   This function gets the previously configured Sync Manager channel.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in]  Channel number
*   ptCfg    [out] Pointer to SM configuration structure
* \return
*   0 ok
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_GetSmCfg ( unsigned int      uChNum,
                          NX6_FMMUSM_SMCFG* ptCfg )
{
  uint32_t ulVal;

  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  /* get channel configuration */
  ulVal = NX_READ32( s_ptFmmuSm->asSm[uChNum].ulCfg_adr_len );
  ptCfg->usPhyStartAddr = (ulVal & MSK_NX51_sm0_cfg_adr_len_startaddr)>>SRT_NX51_sm0_cfg_adr_len_startaddr;
  ptCfg->usLen          = (ulVal & MSK_NX51_sm0_cfg_adr_len_length)>>SRT_NX51_sm0_cfg_adr_len_length;
  ulVal = NX_READ32( s_ptFmmuSm->asSm[uChNum].ulCfg_mode );
  ptCfg->bMode          = (ulVal & MSK_NX51_sm0_cfg_mode_buf_method)>>SRT_NX51_sm0_cfg_mode_buf_method;
  ptCfg->bDir           = (ulVal & MSK_NX51_sm0_cfg_mode_write_read)>>SRT_NX51_sm0_cfg_mode_write_read;

  if( 0!=(ulVal & MSK_NX51_sm0_cfg_mode_proc_enable) ) {
    ptCfg->fEn=true;
  } else {
    ptCfg->fEn=false;
  }

  return 0;
}

/*****************************************************************************/
/*! FMMUSM Return SM enabled status
* \description
*   This function returns the SM enabled status.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
* \return
*   0/1 enabled/disabled
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_IsSmEnabled ( unsigned int uChNum )
{
  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  return (NX_READ32(s_ptFmmuSm->asSm[uChNum].ulCfg_mode) & MSK_NX51_sm0_cfg_mode_proc_enable)>>SRT_NX51_sm0_cfg_mode_proc_enable;
}

/*****************************************************************************/
/*! FMMUSM Disable SM channel via PDI
* \description
*   This function disables/enables a SM channel via PDI.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
*   fDis    [in] 1/0: disable/enable
* \return
*   0 ok
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_DisSmChByPdi ( unsigned int uChNum,
                              bool         fDis )
{
  uint32_t ulVal;

  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  ulVal = NX_READ32(s_ptFmmuSm->asSm[uChNum].ulCfg_mode);
  if( fDis ) {
    ulVal |= MSK_NX51_sm0_cfg_mode_dis_by_arm;
  } else {
    ulVal &= ~MSK_NX51_sm0_cfg_mode_dis_by_arm;
  }
  NX_WRITE32(s_ptFmmuSm->asSm[uChNum].ulCfg_mode, ulVal);
  
  return 0;
}

/*****************************************************************************/
/*! FMMUSM Return SM disabled by PDI status
* \description
*   This function returns the SM disabled by PDI status.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
* \return
*   0/1 enabled/disabled
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_IsSmDisabledByPdi ( unsigned int uChNum )
{
  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  return (NX_READ32(s_ptFmmuSm->asSm[uChNum].ulCfg_mode) & MSK_NX51_sm0_cfg_mode_dis_by_arm)>>SRT_NX51_sm0_cfg_mode_dis_by_arm;
}

/*****************************************************************************/
/*! FMMUSM Set Write event
* \description
*   This function sets a sync manager write event.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
*   uVal    [in] 0/1: value
* \return
*   0 ok
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_SetWriteEvent ( unsigned int uChNum,
                               unsigned int uVal )
{
  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  /* set event */
  NX_WRITE32(s_ptFmmuSm->ulSm_write_event, ((uint32_t)uVal<<uChNum)
                                          |((uint32_t)1<<(uChNum+SRT_NX51_sm_write_event_write_event_mask0))
            );
  
  return 0;
}

/*****************************************************************************/
/*! FMMUSM Get Write event
* \description
*   This function gets a sync manager write event.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] channel number
* \return
*   0/1 event value
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_GetWriteEvent ( unsigned int uChNum )
{
  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  /* get event */
  return (NX_READ32(s_ptFmmuSm->ulSm_write_event) & (1<<uChNum))>>(uChNum+SRT_NX51_sm_write_event_write_event_0);
}

/*****************************************************************************/
/*! FMMUSM Set Read event
* \description
*   This function sets a sync manager read event.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
*   uVal    [in] 0/1: value
* \return
*   0 ok
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_SetReadEvent ( unsigned int uChNum,
                              unsigned int uVal )
{
  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  /* set event */
  NX_WRITE32(s_ptFmmuSm->ulSm_read_event, ((uint32_t)uVal<<uChNum)
                                         |((uint32_t)1<<(uChNum+SRT_NX51_sm_read_event_read_event_mask0))
            );

  return 0;
}

/*****************************************************************************/
/*! FMMUSM Get Read event
* \description
*   This function gets a sync manager read event.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
* \return
*   0/1 event value
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_GetReadEvent ( unsigned int uChNum )
{
  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  /* get event */
  return (NX_READ32(s_ptFmmuSm->ulSm_read_event) & (1U<<uChNum))>>(uChNum+SRT_NX51_sm_read_event_read_event_0);
}

/*****************************************************************************/
/*! FMMUSM Set Buffer full status
* \description
*   This function sets a sync manager buffer full status.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
*   uVal    [in] 0/1: value
* \return
*   0 ok
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_SetBufFull ( unsigned int uChNum,
                            unsigned int uVal )
{
  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  /* set status */
  NX_WRITE32(s_ptFmmuSm->ulSm_buf_statcfg, ((uint32_t)uVal<<uChNum)
                                          |((uint32_t)1<<(uChNum+SRT_NX51_sm_buf_statcfg_buf_mask0))
            );
  
  return 0;
}

/*****************************************************************************/
/*! FMMUSM Get Buffer full status
* \description
*   This function gets a sync manager buffer full status.
* \class 
*   FMMUSM Service Class
* \params
*   uChNum  [in] Channel number
* \return
*   0/1 status
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX6_FMMUSM_GetBufFull ( unsigned int uChNum )
{
  if( uChNum >= NX6_SYNCMAN_CH_MAX_NUM )
    return -1;

  /* set status */
  return( (NX_READ32(s_ptFmmuSm->ulSm_buf_statcfg) & (1<<uChNum))>>(uChNum+SRT_NX51_sm_buf_statcfg_buf_full_0) );

}
