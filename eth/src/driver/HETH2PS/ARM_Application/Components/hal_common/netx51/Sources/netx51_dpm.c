/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_dpm.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_DPM

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Configure DPM window
* \description
*   This function configures the general DPM parameters.
* \class
*   DPM
* \params
*   eMode             [in] Mode
*   eEndianess        [in] ENDIANESS
*   eAddrRange        [in] Address Range
*   eCfgWinAddrCfg    [in] Configuration Window location
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_CfgDpm( NX51_DPM_MODE_E          eMode,
                      NX51_DPM_ENDIANESS_E     eEndianess,
                      NX51_DPM_ADDRRANGE_E     eAddrRange,
                      NX51_DPM_CFGWINADDRCFG_E eCfgWinAddrCfg )
{
  s_ptDpm->ulDpm_cfg0x0   =  ((uint32_t)eMode << SRT_NX51_dpm_cfg0x0_mode)
                            |((uint32_t)eEndianess << SRT_NX51_dpm_cfg0x0_endian);

  s_ptDpm->ulDpm_addr_cfg =  ((uint32_t)eAddrRange << SRT_NX51_dpm_addr_cfg_addr_range)
                            |((uint32_t)eCfgWinAddrCfg<< SRT_NX51_dpm_addr_cfg_cfg_win_addr_cfg);
}

/*****************************************************************************/
/*! Configure DPM timing
* \description
*   This function configures the DPM timing.
* \class
*   DPM
* \params
*   uTosa            [in] Address setup time
*   uFilter          [in] 1/0: filter enable/disable
*   uRds             [in] Read data setup time
*   uRdBurstEn       [in] 1/0: Read burst enable/disable
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int NX51_DPM_CfgTiming( unsigned int uTosa,
                        unsigned int uFilter,
                        unsigned int uRds,
                        unsigned int uRdBurstEn )
{
  /* plausibility check */
  if( uTosa > 3 ) return -1;
  if( uRds  > 7 ) return -1;

  s_ptDpm->ulDpm_timing_cfg = ((uint32_t)uTosa      << SRT_NX51_dpm_timing_cfg_t_osa)
                             |((uint32_t)uFilter    << SRT_NX51_dpm_timing_cfg_filter)
                             |((uint32_t)uRds       << SRT_NX51_dpm_timing_cfg_t_rds)
                             |((uint32_t)uRdBurstEn << SRT_NX51_dpm_timing_cfg_rd_burst_en);
  return 0;
}

/*****************************************************************************/
/*! Configure DPM RDY signal
* \description
*   This function configures the RDY behavior.
* \class
*   DPM
* \params
*   uRdypol         [in] RDY polarity
*   eRdyDrv         [in] RDY driver
*   eRdyMode        [in] RDY signal Mode
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_CfgRdy( unsigned int       uRdypol,
                      NX51_DPM_RDYDRV_E  eRdyDrv,
                      NX51_DPM_RDYMODE_E eRdyMode )
{
  s_ptDpm->ulDpm_rdy_cfg = ((uint32_t)uRdypol  << SRT_NX51_dpm_rdy_cfg_rdy_pol)
                          |((uint32_t)eRdyMode << SRT_NX51_dpm_rdy_cfg_rdy_sig_mode)
                          |((uint32_t)eRdyDrv  << SRT_NX51_dpm_rdy_cfg_rdy_drv_mode);
}

/*****************************************************************************/
/*! Configure DPM PIO signals
* \description
*   This function selects the pins used as PIOs instead of DPM functionality.
* \class
*   DPM
* \params
*   ulSelDPio       [in] If bit n is set, DPM_D[n] pin is used as PIO
*   ulSelMiscPio    [in] If bit n is set, according pin is used as PIO:\
*                        Bit n   Pin\
*                        24      BHE1\
*                        25      BHE3\
*                        26      CSN\
*                        27      RDN\
*                        28      WRN\
*                        29      RDY\
*                        30      DIRQ\
*                        31      SIRQ
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_CfgPio( uint32_t ulSelDPio,
                      uint32_t ulSelMiscPio )
{
  s_ptDpm->aulDpm_pio_cfg[0] = ulSelDPio;
  s_ptDpm->aulDpm_pio_cfg[1] = ulSelMiscPio;
}

/*****************************************************************************/
/*! Configure Miscellaneous
* \description
*   This function configures miscellaneous parameters.
* \class
*   DPM
* \params
*   ulMiscCfg         [in] Miscellaneous configuration
*   ulIoMiscCfg       [in] IO Miscellaneous configuration
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_CfgMisc( uint32_t ulMiscCfg,
                       uint32_t ulIoMiscCfg )
{
  s_ptDpm->ulDpm_misc_cfg    = ulMiscCfg;
  s_ptDpm->ulDpm_io_cfg_misc = ulIoMiscCfg;
}

/*****************************************************************************/
/*! Configure DPM window
* \description
*   This function configures a DPM window.
* \class
*   DPM
* \params
*   uWinNum             [in] DPM window number 1..4
*   ulSize              [in] DPM Size
*   ulMapStartAddr      [in] netX internal start address
*   eAltMapping         [in] Alternative mapping
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_DPM_CfgDpmWindow( unsigned int          uWinNum,
                           uint32_t              ulSize,
                           uint32_t              ulMapStartAddr,
                           NX51_DPM_WINMAP_ALT_E eAltMapping )
{
  /* check 128 byte granularity */
  if( (ulSize&0x7f)!= 0 ) return -1;
  if( (ulMapStartAddr&0x7f)!= 0 ) return -1;

  switch( uWinNum )
  {
    case 1:
      s_ptDpm->asDpm_win[0].ulEnd = ulSize;
      s_ptDpm->asDpm_win[0].ulMap =  (ulMapStartAddr&0xfffff)
                                    |((ulMapStartAddr>>20)<<SRT_NX51_dpm_win1_map_win_page)
                                    |((uint32_t)eAltMapping<<SRT_NX51_dpm_win1_map_win_map_alt);
    break;

    case 2:
      s_ptDpm->asDpm_win[1].ulEnd = ulSize + s_ptDpm->asDpm_win[0].ulEnd;
      s_ptDpm->asDpm_win[1].ulMap =  ((ulMapStartAddr&0xfffff) - s_ptDpm->asDpm_win[0].ulEnd)
                                    |(((ulMapStartAddr-s_ptDpm->asDpm_win[0].ulEnd)>>20)<<SRT_NX51_dpm_win2_map_win_page)
                                    |((uint32_t)eAltMapping<<SRT_NX51_dpm_win2_map_win_map_alt);
    break;

    case 3:
      s_ptDpm->asDpm_win[2].ulEnd = ulSize + s_ptDpm->asDpm_win[1].ulEnd;
      s_ptDpm->asDpm_win[2].ulMap =  ((ulMapStartAddr&0xfffff)-s_ptDpm->asDpm_win[1].ulEnd)
                                    |(((ulMapStartAddr-s_ptDpm->asDpm_win[1].ulEnd)>>20)<<SRT_NX51_dpm_win3_map_win_page)
                                    |((uint32_t)eAltMapping<<SRT_NX51_dpm_win3_map_win_map_alt);
    break;

    case 4:
      s_ptDpm->asDpm_win[3].ulEnd = ulSize + s_ptDpm->asDpm_win[2].ulEnd;
      s_ptDpm->asDpm_win[3].ulMap =  ((ulMapStartAddr&0xfffff)-s_ptDpm->asDpm_win[2].ulEnd)
                                    |(((ulMapStartAddr-s_ptDpm->asDpm_win[2].ulEnd)>>20)<<SRT_NX51_dpm_win4_map_win_page)
                                    |((uint32_t)eAltMapping<<SRT_NX51_dpm_win4_map_win_map_alt);
    break;

    default:
      return -1; /* invalid instance */
  } /* end switch */
  return 0;
}

/*****************************************************************************/
/*! Setup Byte Access
* \description
*   Enable/Disable byte access within the selected DPM window.
* \class
*   DPM
* \params
*   uWinNum               [in]  DPM window number
*   fByteAccess           [in]  Byte Access Value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_DPM_SetupByteAccess( unsigned int uWinNum,
                              bool         fByteAccess )
{
  switch( uWinNum ) {
    case 1:
      if( fByteAccess ) s_ptDpm->asDpm_win[0].ulMap |= MSK_NX51_dpm_win1_map_byte_area;
      else s_ptDpm->asDpm_win[0].ulMap &= ~MSK_NX51_dpm_win1_map_byte_area;
    break;

    case 2:
      if( fByteAccess ) s_ptDpm->asDpm_win[1].ulMap |= MSK_NX51_dpm_win2_map_byte_area;
      else s_ptDpm->asDpm_win[1].ulMap &= ~MSK_NX51_dpm_win1_map_byte_area;
    break;

    case 3:
      if( fByteAccess ) s_ptDpm->asDpm_win[2].ulMap |= MSK_NX51_dpm_win3_map_byte_area;
      else s_ptDpm->asDpm_win[2].ulMap &= ~MSK_NX51_dpm_win1_map_byte_area;
    break;

    case 4:
      if( fByteAccess ) s_ptDpm->asDpm_win[3].ulMap |= MSK_NX51_dpm_win4_map_byte_area;
      else s_ptDpm->asDpm_win[3].ulMap &= ~MSK_NX51_dpm_win1_map_byte_area;
    break;

    default:
      return -1; /* invalid instance */
  } /* end switch */
  return 0;
}

/*****************************************************************************/
/*! Setup Read Ahead
* \description
*   Enable/Disable read ahead function within the selected DPM window.
* \class
*   DPM
* \params
*   uWinNum               [in]  Number of DPM Window
*   fReadAhead            [in]  Read Ahead value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_DPM_SetupReadAhead( unsigned int uWinNum,
                             bool         fReadAhead )
{
  switch( uWinNum ) {
    case 1:
      if( fReadAhead ) s_ptDpm->asDpm_win[0].ulMap |= MSK_NX51_dpm_win1_map_read_ahead;
      else s_ptDpm->asDpm_win[0].ulMap &= ~MSK_NX51_dpm_win1_map_read_ahead;
    break;

    case 2:
      if( fReadAhead ) s_ptDpm->asDpm_win[1].ulMap |= MSK_NX51_dpm_win2_map_read_ahead;
      else s_ptDpm->asDpm_win[1].ulMap &= ~MSK_NX51_dpm_win2_map_read_ahead;
    break;

    case 3:
      if( fReadAhead ) s_ptDpm->asDpm_win[2].ulMap |= MSK_NX51_dpm_win3_map_read_ahead;
      else s_ptDpm->asDpm_win[2].ulMap &= ~MSK_NX51_dpm_win3_map_read_ahead;
    break;

    case 4:
      if( fReadAhead ) s_ptDpm->asDpm_win[3].ulMap |= MSK_NX51_dpm_win4_map_read_ahead;
      else s_ptDpm->asDpm_win[3].ulMap &= ~MSK_NX51_dpm_win4_map_read_ahead;
    break;

    default:
      return -1; /* invalid instance */
  } /* end switch */
  return 0;
}

/*****************************************************************************/
/*! Setup Read Latch
* \description
*   Enable/Disable read latch function within the selected DPM window.
* \class
*   DPM
* \params
*   uWinNum               [in]  Number of DPM Window
*   fReadAhead            [in]  Read Latch value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_DPM_SetupReadLatch( unsigned int uWinNum,
                             bool         fReadLatch )
{
  switch( uWinNum ) {
    case 1:
      if( fReadLatch ) s_ptDpm->asDpm_win[0].ulMap &= ~MSK_NX51_dpm_win1_map_dis_rd_latch;
      else s_ptDpm->asDpm_win[0].ulMap |= MSK_NX51_dpm_win1_map_dis_rd_latch;
    break;

    case 2:
      if( fReadLatch ) s_ptDpm->asDpm_win[1].ulMap &= ~MSK_NX51_dpm_win1_map_dis_rd_latch;
      else s_ptDpm->asDpm_win[1].ulMap |= MSK_NX51_dpm_win1_map_dis_rd_latch;
    break;

    case 3:
      if( fReadLatch ) s_ptDpm->asDpm_win[2].ulMap &= ~MSK_NX51_dpm_win1_map_dis_rd_latch;
      else s_ptDpm->asDpm_win[2].ulMap |= MSK_NX51_dpm_win1_map_dis_rd_latch;
    break;

    case 4:
      if( fReadLatch ) s_ptDpm->asDpm_win[3].ulMap &= ~MSK_NX51_dpm_win1_map_dis_rd_latch;
      else s_ptDpm->asDpm_win[3].ulMap |= MSK_NX51_dpm_win1_map_dis_rd_latch;
    break;

    default:
      return -1; /* invalid instance */
  } /* end switch */
  return 0;
}

/*****************************************************************************/
/*! Setup IRQ
* \description
*   This function configures all interrupt enable masks.
* \class
*   DPM
* \params
*   ulIrqMsk      [in]  IRQ Mask
*   ulFiqMsk      [in]  FIQ Mask
*   ulArmMsk      [in]  ARM Mask
*   ulXpicMsk     [in]  xPIC Mask
*   ulFwIrqMask   [in]  Firmware irq Mask
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_SetupIrq( uint32_t ulIrqMsk,
                        uint32_t ulFiqMsk,
                        uint32_t ulArmMsk,
                        uint32_t ulXpicMsk,
                        uint32_t ulFwIrqMask )
{
  /* clear all interrupt enable */
  s_ptDpm->ulDpm_irq_arm_mask_reset  = 0xffffffff;
  s_ptDpm->ulDpm_irq_xpic_mask_reset = 0xffffffff;
  s_ptDpm->ulDpm_irq_fiq_mask_reset  = 0xffffffff;
  s_ptDpm->ulDpm_irq_irq_mask_reset  = 0xffffffff;

  /* set dedicated interrupt enables */
  s_ptDpm->ulDpm_irq_arm_mask_set  = ulArmMsk;
  s_ptDpm->ulDpm_irq_xpic_mask_set = ulXpicMsk;
  s_ptDpm->ulDpm_irq_fiq_mask_set  = ulFiqMsk;
  s_ptDpm->ulDpm_irq_irq_mask_set  = ulIrqMsk;
  s_ptDpm->ulDpm_firmware_irq_mask = ulFwIrqMask;
}

/*****************************************************************************/
/*! Get Status/Error
* \description
*   Get current error events.
* \class
*   DPM
* \params
*   pulStatErr        [out] Pointer of DPM status
*   pulStatErrAddr    [out] Pointer of DPM status error address
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_GetStatusErr( uint32_t *pulStatErr,
                            uint32_t *pulStatErrAddr )
{
  *pulStatErr     = s_ptDpm->ulDpm_status;
  *pulStatErrAddr = s_ptDpm->ulDpm_status_err_addr;
}

/*****************************************************************************/
/*! Clear Error
* \description
*   Clear error events
* \class
*   DPM
* \params
*   ulClearMask  [in] Reset mask
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_ClearErr( uint32_t ulClearMask )
{
  s_ptDpm->ulDpm_status_err_reset = ulClearMask;
}

/*****************************************************************************/
/*! Get Interrupt
* \description
*   Get current interrupt events.
* \class
*   DPM
* \params
*   eIrqType  [in] Interrupt type
*   pulIrq    [out] Pointer of IRQ value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_DPM_GetIrq( NX51_DPM_IRQ_STATUS_E eIrqType,
                     uint32_t              *pulIrq )
{
  switch( eIrqType ) {
    case NX51_DPM_IRQ_STATUS_RAW:         *pulIrq = s_ptDpm->ulDpm_irq_raw; break;
    case NX51_DPM_IRQ_STATUS_ARM_MASKED:  *pulIrq = s_ptDpm->ulDpm_irq_arm_masked; break;
    case NX51_DPM_IRQ_STATUS_XPIC_MASKED: *pulIrq = s_ptDpm->ulDpm_irq_xpic_masked; break;
    case NX51_DPM_IRQ_STATUS_FIQ_MASKED:  *pulIrq = s_ptDpm->ulDpm_irq_fiq_masked; break;
    case NX51_DPM_IRQ_STATUS_IRQ_MASKED:  *pulIrq = s_ptDpm->ulDpm_irq_irq_masked; break;
    case NX51_DPM_IRQ_STATUS_FW_IRQ_RAW:  *pulIrq = s_ptDpm->ulDpm_firmware_irq_raw; break;
    default: return -1; /* invalid instance */
  } /* end switch */

  return 0;
}

/*****************************************************************************/
/*! Reset
* \description
*   This function resets the DPM interface.
*   Note: This function may not be interrupted by other processes.
* \class
*   DPM
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_Reset( void )
{
  s_ptDpm->ulDpm_reset_request = 0x00;
  s_ptDpm->ulDpm_reset_request = 0x01;
  s_ptDpm->ulDpm_reset_request = 0x03;
  s_ptDpm->ulDpm_reset_request = 0x07;
  s_ptDpm->ulDpm_reset_request = 0x0f;
  s_ptDpm->ulDpm_reset_request = 0x1f;
  s_ptDpm->ulDpm_reset_request = 0x3f;
  s_ptDpm->ulDpm_reset_request = 0x7f;
  s_ptDpm->ulDpm_reset_request = 0xff;
}

/*****************************************************************************/
/*! Get System Status Information
* \description
*   Get sys_sta register.
* \class
*   DPM
* \params
*   pulSysSta         [out] System Status Information
*   pulSysStaBigEnd16 [out] System Status Information Big Endian 16
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_GetSysSta( uint32_t *pulSysSta,
                         uint32_t *pulSysStaBigEnd16 )
{
  *pulSysSta         = s_ptDpm->ulDpm_sys_sta;
  *pulSysStaBigEnd16 = s_ptDpm->ulDpm_sys_sta_bigend16;
}

/*****************************************************************************/
/*! Get netX Version
* \description
*   Get netX version.
* \class
*   DPM
* \params
*   pulNetxVersion         [out] NetX Version
*   pulNetxVersionBigEnd16 [out] NetX Version Big Endian 16
* \return
*                                                                            */
/*****************************************************************************/
void NX51_DPM_GetNetxVersion( uint32_t *pulNetxVersion,
                              uint32_t *pulNetxVersionBigEnd16 )
{
  *pulNetxVersion         = s_ptDpm->ulDpm_netx_version;
  *pulNetxVersionBigEnd16 = s_ptDpm->ulDpm_netx_version_bigend16;
}
