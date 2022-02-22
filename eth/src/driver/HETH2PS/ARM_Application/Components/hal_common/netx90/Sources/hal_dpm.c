/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_dpm.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_DPM(s_aptDpm)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Configure DPM
* \description
*   This function configures the general DPM parameters.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   eMode             [in] Mode
*   eEndianess        [in] ENDIANESS
*   eAddrRange        [in] Address Range
*   eCfgWinAddrCfg    [in] Configuration Window location
* \return
*                                                                            */
/*****************************************************************************/
void DPM_CfgDpm( unsigned int        uiInst,
                         DPM_MODE_E          eMode,
                         DPM_ENDIANESS_E     eEndianess,
                         DPM_ADDRRANGE_E     eAddrRange,
                         DPM_CFGWINADDRCFG_E eCfgWinAddrCfg )
{
  s_aptDpm[uiInst]->ulDpm_cfg0x0   =  ((uint32_t)eMode << HW_SRT(dpm_cfg0x0_mode))
                            |((uint32_t)eEndianess << HW_SRT(dpm_cfg0x0_endian));

  s_aptDpm[uiInst]->ulDpm_addr_cfg =  ((uint32_t)eAddrRange << HW_SRT(dpm_addr_cfg_addr_range))
                            |((uint32_t)eCfgWinAddrCfg<< HW_SRT(dpm_addr_cfg_cfg_win_addr_cfg));
}

/*****************************************************************************/
/*! Configure DPM_Cfg0x0
* \description
*   This function sets register DPM_SetCfg0x0.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   eMode             [in] Mode
*   eEndianess        [in] ENDIANESS
* \return
*                                                                            */
/*****************************************************************************/
void DPM_SetCfg0x0( unsigned int    uiInst,
                            DPM_MODE_E      eMode,
                            DPM_ENDIANESS_E eEndianess )
{
  s_aptDpm[uiInst]->ulDpm_cfg0x0 =  ((uint32_t)eMode << HW_SRT(dpm_cfg0x0_mode))
                          |((uint32_t)eEndianess << HW_SRT(dpm_cfg0x0_endian));
}

/*****************************************************************************/
/*! Configure DPM Address Range
* \description
*   This function configures the general DPM parameters.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   eAddrRange        [in] Address Range
* \return
*                                                                            */
/*****************************************************************************/
void DPM_SetAddrRange( unsigned int    uiInst,
                               DPM_ADDRRANGE_E eAddrRange )
{
  s_aptDpm[uiInst]->ulDpm_addr_cfg =  (s_aptDpm[uiInst]->ulDpm_addr_cfg & ~HW_MSK(dpm_addr_cfg_addr_range))
                            |((uint32_t)eAddrRange << HW_SRT(dpm_addr_cfg_addr_range));
}

/*****************************************************************************/
/*! Configure DPM Address Range
* \description
*   This function configures the general DPM parameters.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   eAddrRange        [in] Address Range
* \return
*                                                                            */
/****************************************************************************/
void DPM_SetCfgWinAddrCfg( unsigned int uiInst,
    DPM_CFGWINADDRCFG_E eCfgWinAddrCfg )
{
  s_aptDpm[uiInst]->ulDpm_addr_cfg =  (s_aptDpm[uiInst]->ulDpm_addr_cfg & ~HW_MSK(dpm_addr_cfg_cfg_win_addr_cfg))
                            |((uint32_t)eCfgWinAddrCfg << HW_SRT(dpm_addr_cfg_cfg_win_addr_cfg));
}

/*****************************************************************************/
/*! Configure DPM Address Comparator
* \description
*   This function configures the DPM address comparator.
*   Note: By writing DPM_CFG0x0.Mode the address comparator configuration is activated!
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   eAddrCmpNr        [in] Address comparator line number
*   eAddrCmpCode      [in] Coding of address comparator
* \return
*                                                                            */
/*****************************************************************************/
int DPM_AddrCmp( unsigned int      uiInst,
                        DPM_ADDRCCMP_E     eAddrCmpNr,
                        DPM_ADDRCMPCODE_E  eAddrCmpCode )
{
  /* No address comparators available */
  return -1;
}

/*****************************************************************************/
/*! Get DPM configuration
* \description
*   This function gets the DPM configuration values.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   pulCfg0x0         [out] DPM configuration value
*   pulAddrCfg        [out] DPM address configuration
* \return
*                                                                            */
/*****************************************************************************/
void DPM_GetCfgDpm( unsigned int uiInst,
                            uint32_t*    pulCfg0x0,
                            uint32_t*    pulAddrCfg )
{
  *pulCfg0x0  = s_aptDpm[uiInst]->ulDpm_cfg0x0;
  *pulAddrCfg = s_aptDpm[uiInst]->ulDpm_addr_cfg;
}

/*****************************************************************************/
/*! Configure DPM interface
* \description
*   This function configures the DPM interface.
*   Note: By writing DPM_CFG0x0.Mode the IF Configuration is activated!
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   eDirCtrl          [in] DPM access direction control
*   uBeSel            [in] DPM access byte enable signal selection
*   uBeRdDis          [in] 1/0: Read access byte enables will be ignored/used
*   uBeWrDis          [in] 1/0: Write access byte enables will be ignored/used
*   uBePol            [in] Byte enable polarity
*   eAenSel           [in] Address controlling
*   uAenPol           [in] 1/0: ALE-signal High/Low
*   uAddrSh           [in] 1/0: Address is shifted/byte address
*   eCsCtrl           [in] Chip select controlling
*   uIsaMemCs16n      [in] 1/0: used as ISA_CS/Byte_en

* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/

int DPM_CfgIf( unsigned int   uiInst,
                       DPM_DIR_CTRL_E eDirCtrl,
                       unsigned int   uBeSel,
                       unsigned int   uBeRdDis,
                       unsigned int   uBeWrDis,
                       unsigned int   uBePol,
                       DPM_AEN_CTRL_E eAenSel,
                       unsigned int   uAenPol,
                       unsigned int   uAddrSh,
                       DPM_CS_CTRL_E  eCsCtrl,
                       unsigned int   uIsaMemCs16n )
{
  /* plausibility check */
  if( eDirCtrl > 2 ) return -1;
  if( uBePol  > 15 ) return -1;
  if( eAenSel > 2 ) return -1;
  if( (eCsCtrl > 4) && (eCsCtrl != 7) ) return -1;

  s_aptDpm[uiInst]->ulDpm_if_cfg =  ((uint32_t)eDirCtrl   << HW_SRT(dpm_if_cfg_dir_ctrl))
                          |((uint32_t)uBeSel     << HW_SRT(dpm_if_cfg_be_sel))
                          |((uint32_t)uBeRdDis   << HW_SRT(dpm_if_cfg_be_rd_dis))
                          |((uint32_t)uBeWrDis   << HW_SRT(dpm_if_cfg_be_wr_dis))
                          |((uint32_t)uBePol     << HW_SRT(dpm_if_cfg_be_pol))
                          |((uint32_t)eAenSel    << HW_SRT(dpm_if_cfg_aen_sel))
                          |((uint32_t)uAenPol    << HW_SRT(dpm_if_cfg_aen_pol))
                          |((uint32_t)uAddrSh    << HW_SRT(dpm_if_cfg_addr_sh))
                          |((uint32_t)eCsCtrl    << HW_SRT(dpm_if_cfg_cs_ctrl));
  return 0;
}

/*****************************************************************************/
/*! Configure DPM timing
* \description
*   This function configures the DPM timing.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uTosa             [in] Address setup time
*   uFilter           [in] 1/0: filter enable/disable
*   uTrds             [in] Read data setup time
*   uRdBurstEn        [in] 1/0: Read burst enable/disable
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int DPM_CfgTiming( unsigned int uiInst,
                           unsigned int uTosa,
                           unsigned int uFilter,
                           unsigned int uTrds,
                           unsigned int uRdBurstEn )
{
  /* plausibility check */
  if( uTosa > 3 ) return -1;
  if( uTrds > 7 ) return -1;

  s_aptDpm[uiInst]->ulDpm_timing_cfg = ((uint32_t)uTosa      << HW_SRT(dpm_timing_cfg_t_osa))
                             |((uint32_t)uFilter    << HW_SRT(dpm_timing_cfg_filter))
                             |((uint32_t)uTrds      << HW_SRT(dpm_timing_cfg_t_rds))
                             |((uint32_t)uRdBurstEn << HW_SRT(dpm_timing_cfg_rd_burst_en));
  return 0;
}

/*****************************************************************************/
/*! Set DPM RDY configuration
* \description
*   This function sets the DPM RDY configuration.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   eRdypol           [in] RDY polarity
*   eRdyDrv           [in] RDY driver
*   eRdyMode          [in] RDY signal Mode
*   eRdyToCfg         [in] RDY Timeout Configuration
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int DPM_CfgRdy( unsigned int    uiInst,
                        DPM_RDYPOL_E    eRdypol,
                        DPM_RDYDRV_E    eRdyDrv,
                        DPM_RDYMODE_E   eRdyMode,
                        DPM_RDYTO_CFG_E eRdyToCfg )
{
  /* plausibility check */
  if( (int)eRdypol > DPM_RDYPOL_READY_WHEN_HI ) return -1;
  if( (int)eRdyDrv > DPM_RDYDRV_OPEN_DRAIN ) return -1;
  if( (int)eRdyMode > DPM_RDYMODE_RDYACK ) return -1;
  if( (int)eRdyToCfg > DPM_RDYTO_CFG_DIS ) return -1;

  /* disable PIO mode of RDY if RDY enabled */
  if( DPM_RDYDRV_DIS != eRdyDrv )
  {
    s_aptDpm[uiInst]->aulDpm_pio_cfg[1] = s_aptDpm[uiInst]->aulDpm_pio_cfg[1] & (~HW_MSK(dpm_pio_cfg1_sel_rdy_pio));
  }

  s_aptDpm[uiInst]->ulDpm_rdy_cfg = ((uint32_t)eRdypol    << HW_SRT(dpm_rdy_cfg_rdy_pol))
                          |((uint32_t)eRdyMode   << HW_SRT(dpm_rdy_cfg_rdy_sig_mode))
                          |((uint32_t)eRdyDrv    << HW_SRT(dpm_rdy_cfg_rdy_drv_mode))
                          |((uint32_t)eRdyToCfg  << HW_SRT(dpm_rdy_cfg_rdy_to_cfg));

  return 0;
}

/*****************************************************************************/
/*! Configure DPM PIO signals
* \description
*   This function selects the pins used as PIOs instead of DPM functionality.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   ulSelDPio         [in] If bit n is set, DPM_D[n] pin is used as PIO
*   ulSelMiscPio      [in] If bit n is set, according pin is used as PIO:\
*                          Bit n   Pin\
*                          24      BHE1\
*                          25      BHE3\
*                          26      CSN\
*                          27      RDN\
*                          28      WRN\
*                          29      RDY\
*                          30      DIRQ\
*                          31      SIRQ
* \return
*                                                                            */
/*****************************************************************************/
void DPM_CfgPio( unsigned int uiInst,
                         uint32_t     ulSelDPio,
                         uint32_t     ulSelMiscPio )
{
  s_aptDpm[uiInst]->aulDpm_pio_cfg[0] = ulSelDPio;
  s_aptDpm[uiInst]->aulDpm_pio_cfg[1] = ulSelMiscPio;
}

/*****************************************************************************/
/*! Get DPM PIO configuration
* \description
*   This function gets the DPM PIO configuration values.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   pulDPio           [out] DPM configuration value
*   pulMiscPio        [out] DPM address configuration
* \return
*                                                                            */
/*****************************************************************************/
void DPM_GetCfgPio( unsigned int uiInst,
                            uint32_t*    pulDPio,
                            uint32_t*    pulMiscPio )
{
  *pulDPio    = s_aptDpm[uiInst]->aulDpm_pio_cfg[0];
  *pulMiscPio = s_aptDpm[uiInst]->aulDpm_pio_cfg[1];
}

/*****************************************************************************/
/*! Configure Miscellaneous
* \description
*   This function configures miscellaneous parameters.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   ulMiscCfg         [in] Miscellaneous configuration
*   ulIoMiscCfg       [in] IO Miscellaneous configuration
* \return
*                                                                            */
/*****************************************************************************/
void DPM_CfgMisc( unsigned int uiInst,
                          uint32_t     ulMiscCfg,
                          uint32_t     ulIoMiscCfg )
{
  s_aptDpm[uiInst]->ulDpm_misc_cfg    = ulMiscCfg;
  s_aptDpm[uiInst]->ulDpm_io_cfg_misc = ulIoMiscCfg;
}

/*****************************************************************************/
/*! Get DPM Miscellaneous configuration
* \description
*   This function gets the DPM miscellaneous configuration values.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   pulDPio           [out] DPM configuration value
*   pulMiscPio        [out] DPM address configuration
* \return
*                                                                            */
/*****************************************************************************/
void DPM_GetCfgMisc(unsigned int uiInst,
                            uint32_t*    pulMiscCfg,
                            uint32_t*    pulIoMiscCfg )
{
  *pulMiscCfg   = s_aptDpm[uiInst]->ulDpm_misc_cfg;
  *pulIoMiscCfg = s_aptDpm[uiInst]->ulDpm_io_cfg_misc;
}

/*****************************************************************************/
/*! Configure DPM window
* \description
*   This function configures a DPM window.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uWinNum           [in] DPM window number 1..4
*   ulSize            [in] DPM Size
*   ulMapStartAddr    [in] netX internal start address
*   eAltMapping       [in] Alternative mapping
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int DPM_CfgDpmWindow( unsigned int uiInst,
                              unsigned int uWinNum,
                              uint32_t     ulSize,
                              uint32_t     ulMapStartAddr,
                              DPM_WINMAP_ALT_E eAltMapping )
{
  int iRslt = 0;

  /* check 128 byte granularity */
  if( (ulSize & 0x7f)!= 0 ) return -1;
  if( (ulMapStartAddr & 0x7f)!= 0 ) return -1;

  switch( uWinNum )
  {
    case 1:
      s_aptDpm[uiInst]->ulDpm_win1_end = ulSize;
      s_aptDpm[uiInst]->ulDpm_win1_map =  (ulMapStartAddr & 0xfffff)
                                |(ulMapStartAddr & HW_MSK(dpm_win1_map_win_page))
                                |((uint32_t)eAltMapping << HW_SRT(dpm_win1_map_win_map_alt));
    break;

    case 2:
      s_aptDpm[uiInst]->ulDpm_win2_end = ulSize + s_aptDpm[uiInst]->ulDpm_win1_end;
      s_aptDpm[uiInst]->ulDpm_win2_map =  ((ulMapStartAddr - s_aptDpm[uiInst]->ulDpm_win1_end) & 0xfffff)
                                |(ulMapStartAddr & HW_MSK(dpm_win2_map_win_page))
                                |((uint32_t)eAltMapping << HW_SRT(dpm_win2_map_win_map_alt));
    break;

    case 3:
      s_aptDpm[uiInst]->ulDpm_win3_end = ulSize + s_aptDpm[uiInst]->ulDpm_win2_end;
      s_aptDpm[uiInst]->ulDpm_win3_map =  ((ulMapStartAddr - s_aptDpm[uiInst]->ulDpm_win2_end) & 0xfffff)
                                |(ulMapStartAddr & HW_MSK(dpm_win3_map_win_page))
                                |((uint32_t)eAltMapping << HW_SRT(dpm_win3_map_win_map_alt));
    break;

    case 4:
      s_aptDpm[uiInst]->ulDpm_win4_end = ulSize + s_aptDpm[uiInst]->ulDpm_win3_end;
      s_aptDpm[uiInst]->ulDpm_win4_map =  ((ulMapStartAddr - s_aptDpm[uiInst]->ulDpm_win3_end) & 0xfffff)
                                |(ulMapStartAddr & HW_MSK(dpm_win4_map_win_page))
                                |((uint32_t)eAltMapping << HW_SRT(dpm_win4_map_win_map_alt));
    break;

    default:
      iRslt = -1; /* invalid instance */
    break;
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Set DPM window configuration
* \description
*   This function sets a DPM window configuration.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uWinNum           [in] DPM window number 1..4
*   ulSize            [in] DPM Size
*   ulMapStartAddr    [in] netX internal start address
*   eAltMapping       [in] Alternative mapping
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int DPM_SetCfgDpmWindow( unsigned int     uiInst,
                                 unsigned int     uWinNum,
                                 uint32_t         ulSize,
                                 uint32_t         ulMapStartAddr,
                                 DPM_WINMAP_ALT_E eAltMapping )
{
  int iRslt = 0;

  /* check 128 byte granularity */
  if( (ulSize & 0x7f)!= 0 ) return -1;
  if( (ulMapStartAddr & 0x7f)!= 0 ) return -1;

  switch( uWinNum )
  {
    case 1:
      s_aptDpm[uiInst]->ulDpm_win1_end = ulSize;
      s_aptDpm[uiInst]->ulDpm_win1_map =  (ulMapStartAddr & 0xfffff)
                                |(ulMapStartAddr & HW_MSK(dpm_win1_map_win_page))
                                |((uint32_t)eAltMapping << HW_SRT(dpm_win1_map_win_map_alt));
    break;

    case 2:
      s_aptDpm[uiInst]->ulDpm_win2_end = ulSize;
      s_aptDpm[uiInst]->ulDpm_win2_map =  (ulMapStartAddr & 0xfffff)
                                |(ulMapStartAddr & HW_MSK(dpm_win2_map_win_page))
                                |((uint32_t)eAltMapping << HW_SRT(dpm_win2_map_win_map_alt));
    break;

    case 3:
      s_aptDpm[uiInst]->ulDpm_win3_end = ulSize;
      s_aptDpm[uiInst]->ulDpm_win3_map =  (ulMapStartAddr & 0xfffff)
                                |(ulMapStartAddr & HW_MSK(dpm_win3_map_win_page))
                                |((uint32_t)eAltMapping << HW_SRT(dpm_win3_map_win_map_alt));
    break;

    case 4:
      s_aptDpm[uiInst]->ulDpm_win4_end = ulSize;
      s_aptDpm[uiInst]->ulDpm_win4_map =  (ulMapStartAddr & 0xfffff)
                                |(ulMapStartAddr & HW_MSK(dpm_win4_map_win_page))
                                |((uint32_t)eAltMapping << HW_SRT(dpm_win4_map_win_map_alt));
    break;

    default:
      iRslt = -1; /* invalid instance */
    break;
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Get DPM window configuration
* \description
*   This function gets a DPM window configuration.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uWinNum           [in]  DPM window number 1..4
*   pulWinEnd         [out] DPM window end value
*   pulWinMap         [out] DPM window end value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int DPM_GetCfgDpmWindow( unsigned int uiInst,
                                 unsigned int uWinNum,
                                 uint32_t*    pulWinEnd,
                                 uint32_t*    pulWinMap )
{
  int iRslt = 0;

  switch( uWinNum )
  {
    case 1:
      *pulWinEnd = s_aptDpm[uiInst]->ulDpm_win1_end;
      *pulWinMap = s_aptDpm[uiInst]->ulDpm_win1_map;
    break;

    case 2:
      *pulWinEnd = s_aptDpm[uiInst]->ulDpm_win2_end;
      *pulWinMap = s_aptDpm[uiInst]->ulDpm_win2_map;
    break;

    case 3:
      *pulWinEnd = s_aptDpm[uiInst]->ulDpm_win3_end;
      *pulWinMap = s_aptDpm[uiInst]->ulDpm_win3_map;
    break;

    case 4:
      *pulWinEnd = s_aptDpm[uiInst]->ulDpm_win4_end;
      *pulWinMap = s_aptDpm[uiInst]->ulDpm_win4_map;
    break;

    default:
      iRslt = -1; /* invalid instance */
    break;
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Setup Byte Access
* \description
*   Enable/Disable byte access within the selected DPM window.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uWinNum           [in]  DPM window number
*   fByteAccess       [in]  Byte Access Value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int DPM_SetupByteAccess( unsigned int uiInst,
                                 unsigned int uWinNum,
                                 bool         fByteAccess )
{
  int iRslt = 0;

  switch( uWinNum ) {
    case 1:
      if( fByteAccess ) s_aptDpm[uiInst]->ulDpm_win1_map |= HW_MSK(dpm_win1_map_byte_area);
      else s_aptDpm[uiInst]->ulDpm_win1_map &= ~HW_MSK(dpm_win1_map_byte_area);
    break;

    case 2:
      if( fByteAccess ) s_aptDpm[uiInst]->ulDpm_win2_map |= HW_MSK(dpm_win2_map_byte_area);
      else s_aptDpm[uiInst]->ulDpm_win2_map &= ~HW_MSK(dpm_win1_map_byte_area);
    break;

    case 3:
      if( fByteAccess ) s_aptDpm[uiInst]->ulDpm_win3_map |= HW_MSK(dpm_win3_map_byte_area);
      else s_aptDpm[uiInst]->ulDpm_win3_map &= ~HW_MSK(dpm_win1_map_byte_area);
    break;

    case 4:
      if( fByteAccess ) s_aptDpm[uiInst]->ulDpm_win4_map |= HW_MSK(dpm_win4_map_byte_area);
      else s_aptDpm[uiInst]->ulDpm_win4_map &= ~HW_MSK(dpm_win1_map_byte_area);
    break;

    default:
      iRslt = -1; /* invalid instance */
    break;
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Setup Read Ahead
* \description
*   Enable/Disable read ahead function within the selected DPM window.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uWinNum           [in]  Number of DPM Window
*   fReadAhead        [in]  Read Ahead value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int DPM_SetupReadAhead( unsigned int uiInst,
                                unsigned int uWinNum,
                                bool         fReadAhead )
{
  int iRslt = 0;

  switch( uWinNum ) {
    case 1:
      if( fReadAhead ) s_aptDpm[uiInst]->ulDpm_win1_map |= HW_MSK(dpm_win1_map_read_ahead);
      else s_aptDpm[uiInst]->ulDpm_win1_map &= ~HW_MSK(dpm_win1_map_read_ahead);
    break;

    case 2:
      if( fReadAhead ) s_aptDpm[uiInst]->ulDpm_win2_map |= HW_MSK(dpm_win2_map_read_ahead);
      else s_aptDpm[uiInst]->ulDpm_win2_map &= ~HW_MSK(dpm_win2_map_read_ahead);
    break;

    case 3:
      if( fReadAhead ) s_aptDpm[uiInst]->ulDpm_win3_map |= HW_MSK(dpm_win3_map_read_ahead);
      else s_aptDpm[uiInst]->ulDpm_win3_map &= ~HW_MSK(dpm_win3_map_read_ahead);
    break;

    case 4:
      if( fReadAhead ) s_aptDpm[uiInst]->ulDpm_win4_map |= HW_MSK(dpm_win4_map_read_ahead);
      else s_aptDpm[uiInst]->ulDpm_win4_map &= ~HW_MSK(dpm_win4_map_read_ahead);
    break;

    default:
      iRslt = -1; /* invalid instance */
    break;
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Setup Read Latch
* \description
*   Enable/Disable read latch function within the selected DPM window.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uWinNum           [in] Number of DPM Window
*   fReadAhead        [in] Read Latch value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int DPM_SetupReadLatch( unsigned int uiInst,
                                unsigned int uWinNum,
                                bool         fReadLatch )
{
  int iRslt = 0;

  switch( uWinNum ) {
    case 1:
      if( fReadLatch ) s_aptDpm[uiInst]->ulDpm_win1_map &= ~HW_MSK(dpm_win1_map_dis_rd_latch);
      else s_aptDpm[uiInst]->ulDpm_win1_map |= HW_MSK(dpm_win1_map_dis_rd_latch);
    break;

    case 2:
      if( fReadLatch ) s_aptDpm[uiInst]->ulDpm_win2_map &= ~HW_MSK(dpm_win1_map_dis_rd_latch);
      else s_aptDpm[uiInst]->ulDpm_win2_map |= HW_MSK(dpm_win1_map_dis_rd_latch);
    break;

    case 3:
      if( fReadLatch ) s_aptDpm[uiInst]->ulDpm_win3_map &= ~HW_MSK(dpm_win1_map_dis_rd_latch);
      else s_aptDpm[uiInst]->ulDpm_win3_map |= HW_MSK(dpm_win1_map_dis_rd_latch);
    break;

    case 4:
      if( fReadLatch ) s_aptDpm[uiInst]->ulDpm_win4_map &= ~HW_MSK(dpm_win1_map_dis_rd_latch);
      else s_aptDpm[uiInst]->ulDpm_win4_map |= HW_MSK(dpm_win1_map_dis_rd_latch);
    break;

    default:
      iRslt = -1; /* invalid instance */
    break;
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Get Status/Error
* \description
*   This function gets current error events.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   pulStatErr        [out] Pointer of DPM status
*   pulStatErrAddr    [out] Pointer of DPM status error address
* \return
*                                                                            */
/*****************************************************************************/
void DPM_GetStatusErr( unsigned int uiInst,
                               uint32_t*    pulStatErr,
                               uint32_t*    pulStatErrAddr )
{
  *pulStatErr     = s_aptDpm[uiInst]->ulDpm_status;
  *pulStatErrAddr = s_aptDpm[uiInst]->ulDpm_status_err_addr;
}

/*****************************************************************************/
/*! Clear Error
* \description
*   This function clears error events
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   ulClearMask       [in] Reset mask
* \return
*                                                                            */
/*****************************************************************************/
void DPM_ClearErr( unsigned int uiInst,
                           uint32_t     ulClearMask )
{
  s_aptDpm[uiInst]->ulDpm_status_err_reset = ulClearMask;
}

/*****************************************************************************/
/*! Get Interrupt
* \description
*   This function gets the current interrupt events.
*   Note: This is a legacy function for backward compatibility.
*   So old naming and only first registers are supported.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   eIrqType          [in] Interrupt type
*   pulIrq            [out] Pointer of IRQ value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int DPM_GetIrq( unsigned int     uiInst,
                        DPM_IRQ_STATUS_E eIrqType,
                        uint32_t*        pulIrq )
{
  int iRslt = 0;

  switch( eIrqType ) {
    case DPM_IRQ_STATUS_RAW:         *pulIrq = s_aptDpm[uiInst]->aulDpm_irq_raw[0]; break;
    case DPM_IRQ_STATUS_FIQ_MASKED:  *pulIrq = s_aptDpm[uiInst]->aulDpm_irq_host_sirq_masked[0]; break;
    case DPM_IRQ_STATUS_IRQ_MASKED:  *pulIrq = s_aptDpm[uiInst]->aulDpm_irq_host_dirq_masked[0]; break;
    case DPM_IRQ_STATUS_FW_IRQ_RAW:  *pulIrq = s_aptDpm[uiInst]->ulDpm_firmware_irq_raw; break;
    default: iRslt = -1; break; /* invalid instance */
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Set Firmware IRQ Raw
* \description
*   This function sets the DPM firmware_irq_raw register
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   ulFwIrqRaw        [in] Firmware IRQ RAW value
* \return
*                                                                            */
/*****************************************************************************/
void DPM_SetFwIrqRaw( unsigned int uiInst,
                              uint32_t     ulFwIrqRaw )
{
  s_aptDpm[uiInst]->ulDpm_firmware_irq_raw = ulFwIrqRaw;
}

/*****************************************************************************/
/*! Reset
* \description
*   This function resets the DPM interface.
*   Note: This function may not be interrupted by other processes.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
* \return
*                                                                            */
/*****************************************************************************/
void DPM_Reset( unsigned int uiInst )
{
  s_aptDpm[uiInst]->ulDpm_reset_request = 0x00;
  s_aptDpm[uiInst]->ulDpm_reset_request = 0x01;
  s_aptDpm[uiInst]->ulDpm_reset_request = 0x03;
  s_aptDpm[uiInst]->ulDpm_reset_request = 0x07;
  s_aptDpm[uiInst]->ulDpm_reset_request = 0x0f;
  s_aptDpm[uiInst]->ulDpm_reset_request = 0x1f;
  s_aptDpm[uiInst]->ulDpm_reset_request = 0x3f;
  s_aptDpm[uiInst]->ulDpm_reset_request = 0x7f;
  s_aptDpm[uiInst]->ulDpm_reset_request = 0xff;
}

/*****************************************************************************/
/*! Set DPM Reset Request
* \description
*   This function sets the DPM reset request register.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   bResetKey         [in] Reset key sequence value
* \return
*                                                                            */
/*****************************************************************************/
void DPM_SetResetRequest( unsigned int uiInst,
                                  uint8_t      bResetKey )
{
  s_aptDpm[uiInst]->ulDpm_reset_request = (uint32_t) bResetKey;
}

/*****************************************************************************/
/*! Get DPM Reset Request
* \description
*   This function gets the DPM reset request register.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
* \return
*   DPM Reset_Request value                                                  */
/*****************************************************************************/
uint8_t DPM_GetResetRequest( unsigned int uiInst )
{
  return (s_aptDpm[uiInst]->ulDpm_reset_request & HW_MSK(dpm_reset_request_reset_key));
}

/*****************************************************************************/
/*! Set System Status
* \description
*   This function sets the sys_sta register.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   ulSysSta          [in] System Status
* \return
*                                                                            */
/*****************************************************************************/
void DPM_SetSysSta( unsigned int uiInst,
                            uint32_t     ulSysSta )
{
  s_aptDpm[uiInst]->ulDpm_sys_sta = ulSysSta;
}

/*****************************************************************************/
/*! Get System Status Information
* \description
*   This function gets the sys_sta register.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   pulSysSta         [out] System Status Information
*   pulSysStaBigEnd16 [out] System Status Information Big Endian 16
* \return
*                                                                            */
/*****************************************************************************/
void DPM_GetSysSta( unsigned int uiInst,
                            uint32_t*    pulSysSta,
                            uint32_t*    pulSysStaBigEnd16 )
{
  *pulSysSta         = s_aptDpm[uiInst]->ulDpm_sys_sta;
  *pulSysStaBigEnd16 = s_aptDpm[uiInst]->ulDpm_sys_sta_bigend16;
}

/*****************************************************************************/
/*! Get netX Version
* \description
*   Get netX version.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   pulNetxVersion         [out] NetX Version
*   pulNetxVersionBigEnd16 [out] NetX Version BigEndian16
* \return
*                                                                            */
/*****************************************************************************/
void DPM_GetNetxVersion( unsigned int uiInst,
                                 uint32_t *pulNetxVersion,
                                 uint32_t *pulNetxVersionBigEnd16 )
{
  *pulNetxVersion         = s_aptDpm[uiInst]->ulDpm_netx_version;
  *pulNetxVersionBigEnd16 = s_aptDpm[uiInst]->ulDpm_netx_version_bigend16;
}

/*****************************************************************************/
/*! Configure DPM Tunnel
* \description
*   This function configures the DPM Access Tunnel.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uWpData           [in] Access Tunnel function is data write protected
*   uWpITBAddr        [in] ITBAddr is write protected
*   uEnable           [in] Enable/disable Access Tunnel function
*   uTunnelAll        [in] Enable/disable external access to Internal Target Base Address
*   uByteArea         [in] Tunnel is byte area or not
*   uDisRdLatch       [in] Disabled read data latch for tunnel
*   uBase             [in] DPM Access Tunnel Base Address divided by 64

* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/

int DPM_CfgTunnel( unsigned int uiInst,
                           unsigned int uWpData,
                           unsigned int uWpITBAddr,
                           unsigned int uEnable,
                           unsigned int uTunnelAll,
                           unsigned int uByteArea,
                           unsigned int uDisRdLatch,
                           unsigned int uBase )
{

  s_aptDpm[uiInst]->ulDpm_tunnel_cfg =  ((uint32_t)uWpData     << HW_SRT(dpm_tunnel_cfg_wp_data))
                              |((uint32_t)uWpITBAddr  << HW_SRT(dpm_tunnel_cfg_wp_itbaddr))
                              |((uint32_t)uEnable     << HW_SRT(dpm_tunnel_cfg_enable))
                              |((uint32_t)uTunnelAll  << HW_SRT(dpm_tunnel_cfg_tunnel_all))
                              |((uint32_t)uByteArea   << HW_SRT(dpm_tunnel_cfg_byte_area))
                              |((uint32_t)uDisRdLatch << HW_SRT(dpm_tunnel_cfg_dis_rd_latch))
                              |((uint32_t)uBase       << HW_SRT(dpm_tunnel_cfg_base));
  return 0;
}

/*****************************************************************************/
/*! Configure DPM ITBAddr
* \description
*   This function configures the DPM Access Tunnel netX Internal Target Base Address.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   uMap              [in] Mapping part of ITBAddr
*   uBase             [in] Internal netX Tunnel Target Base Address divided by 64

* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/

int DPM_ITBAddr( unsigned int uiInst,
                         unsigned int uMap,
                         unsigned int uBase )
{
  s_aptDpm[uiInst]->ulDpm_itbaddr =  ((uint32_t)uMap  << HW_SRT(dpm_itbaddr_map))
                                    |((uint32_t)uBase << HW_SRT(dpm_itbaddr_base));
  return 0;
}

/*****************************************************************************/
/*! Is IRQ pending
* \description
*  Read the RAW interrupt register and checks if the given interrupt request
*  is active.
* \class
*   DPM
* \params
*   uiInst           [in] DPM instance number
*   uIrqId           [in] Interrupt ID
* \return
*   true: IRQ is active
*   false: IRQ is inactive                                                   */
/*****************************************************************************/
bool DPM_IrqIsPending( unsigned int uiInst,
                               unsigned int uIrqId )
{
  if(uIrqId < 32) {
    return 0 != (s_aptDpm[uiInst]->aulDpm_irq_raw[0] & (1UL << uIrqId));
  } else {
    return 0 != (s_aptDpm[uiInst]->aulDpm_irq_raw[1] & (1UL << (uIrqId - 32)));
  }
}

/*****************************************************************************/
/*! DPM Set Parameter
* \description
*   This function writes parameter.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   ePrmID            [in] Parameter ID
*   ulPrmVal          [in] Parameter value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int DPM_SetParam( unsigned int uiInst,
                          DPM_PRM_E    ePrmID,
                          uint32_t     ulPrmVal )
{
  int iRslt = 0;

  switch( ePrmID )
  {
    case DPM_PRM_DPM_IF_CFG:              s_aptDpm[uiInst]->ulDpm_if_cfg              = ulPrmVal; break;
    case DPM_PRM_DPM_ADDR_CFG:            s_aptDpm[uiInst]->ulDpm_addr_cfg            = ulPrmVal; break;
    case DPM_PRM_DPM_TIMING_CFG:          s_aptDpm[uiInst]->ulDpm_timing_cfg          = ulPrmVal; break;
    case DPM_PRM_DPM_MISC_CFG:            s_aptDpm[uiInst]->ulDpm_misc_cfg            = ulPrmVal; break;
    case DPM_PRM_DPM_IO_CFG_MISC:         s_aptDpm[uiInst]->ulDpm_io_cfg_misc         = ulPrmVal; break;
    case DPM_PRM_DPM_TUNNEL_CFG:          s_aptDpm[uiInst]->ulDpm_tunnel_cfg          = ulPrmVal; break;
    case DPM_PRM_DPM_ITBADDR:             s_aptDpm[uiInst]->ulDpm_itbaddr             = ulPrmVal; break;

    case DPM_PRM_DPM_WIN1_END:            s_aptDpm[uiInst]->ulDpm_win1_end            = ulPrmVal; break;
    case DPM_PRM_DPM_WIN1_MAP:            s_aptDpm[uiInst]->ulDpm_win1_map            = ulPrmVal; break;
    case DPM_PRM_DPM_WIN2_END:            s_aptDpm[uiInst]->ulDpm_win2_end            = ulPrmVal; break;
    case DPM_PRM_DPM_WIN2_MAP:            s_aptDpm[uiInst]->ulDpm_win2_map            = ulPrmVal; break;
    case DPM_PRM_DPM_WIN3_END:            s_aptDpm[uiInst]->ulDpm_win3_end            = ulPrmVal; break;
    case DPM_PRM_DPM_WIN3_MAP:            s_aptDpm[uiInst]->ulDpm_win3_map            = ulPrmVal; break;
    case DPM_PRM_DPM_WIN4_END:            s_aptDpm[uiInst]->ulDpm_win4_end            = ulPrmVal; break;
    case DPM_PRM_DPM_WIN4_MAP:            s_aptDpm[uiInst]->ulDpm_win4_map            = ulPrmVal; break;

    case DPM_PRM_DPM_IRQ_SIRQ_MASK_SET:   s_aptDpm[uiInst]->aulDpm_irq_host_sirq_mask_set[0] = ulPrmVal; break;
    case DPM_PRM_DPM_IRQ_SIRQ_MASK_SET1:  s_aptDpm[uiInst]->aulDpm_irq_host_sirq_mask_set[1] = ulPrmVal; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASK_SET:   s_aptDpm[uiInst]->aulDpm_irq_host_dirq_mask_set[0] = ulPrmVal; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASK_SET1:  s_aptDpm[uiInst]->aulDpm_irq_host_dirq_mask_set[1] = ulPrmVal; break;

    case DPM_PRM_DPM_IRQ_SIRQ_MASK_RESET: s_aptDpm[uiInst]->aulDpm_irq_host_sirq_mask_reset[0] = ulPrmVal; break;
    case DPM_PRM_DPM_IRQ_SIRQ_MASK_RESET1:s_aptDpm[uiInst]->aulDpm_irq_host_sirq_mask_reset[1] = ulPrmVal; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASK_RESET: s_aptDpm[uiInst]->aulDpm_irq_host_dirq_mask_reset[0] = ulPrmVal; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASK_RESET1:s_aptDpm[uiInst]->aulDpm_irq_host_dirq_mask_reset[1] = ulPrmVal; break;

    case DPM_PRM_DPM_FW_IRQ_MASK:         s_aptDpm[uiInst]->ulDpm_firmware_irq_mask   = ulPrmVal; break;
    case DPM_PRM_DPM_SW_IRQ:              s_aptDpm[uiInst]->ulDpm_sw_irq              = ulPrmVal; break;
    case DPM_PRM_DPM_CRC:                 s_aptDpm[uiInst]->ulDpm_crc                 = ulPrmVal; break;

    default:
      iRslt = -1;
      break;
  }

  return iRslt;
}

/*****************************************************************************/
/*! DPM Get Parameter
* \description
*   This function reads parameter and returns it.
* \class
*   DPM
* \params
*   uiInst            [in] DPM instance number
*   ePrmID            [in] Parameter ID
* \return
*   Read Parameter Value                                                     */
/*****************************************************************************/
uint32_t DPM_GetParam( unsigned int uiInst,
                               DPM_PRM_E    ePrmID )
{
  uint32_t ulRetVal = 0;

  switch( ePrmID )
  {
    case DPM_PRM_DPM_IF_CFG:              ulRetVal = s_aptDpm[uiInst]->ulDpm_if_cfg;              break;
    case DPM_PRM_DPM_ADDR_CFG:            ulRetVal = s_aptDpm[uiInst]->ulDpm_addr_cfg;            break;
    case DPM_PRM_DPM_TIMING_CFG:          ulRetVal = s_aptDpm[uiInst]->ulDpm_timing_cfg;          break;
    case DPM_PRM_DPM_RDY_CFG:             ulRetVal = s_aptDpm[uiInst]->ulDpm_rdy_cfg;             break;
    case DPM_PRM_DPM_STATUS_ERR_RESET:    ulRetVal = s_aptDpm[uiInst]->ulDpm_status_err_reset;    break;
    case DPM_PRM_DPM_STATUS_ERR_ADDR:     ulRetVal = s_aptDpm[uiInst]->ulDpm_status_err_addr;     break;
    case DPM_PRM_DPM_MISC_CFG:            ulRetVal = s_aptDpm[uiInst]->ulDpm_misc_cfg;            break;
    case DPM_PRM_DPM_IO_CFG_MISC:         ulRetVal = s_aptDpm[uiInst]->ulDpm_io_cfg_misc;         break;
    case DPM_PRM_DPM_TUNNEL_CFG:          ulRetVal = s_aptDpm[uiInst]->ulDpm_tunnel_cfg;          break;
    case DPM_PRM_DPM_ITBADDR:             ulRetVal = s_aptDpm[uiInst]->ulDpm_itbaddr;             break;

    case DPM_PRM_DPM_WIN1_END:            ulRetVal = s_aptDpm[uiInst]->ulDpm_win1_end;            break;
    case DPM_PRM_DPM_WIN1_MAP:            ulRetVal = s_aptDpm[uiInst]->ulDpm_win1_map;            break;
    case DPM_PRM_DPM_WIN2_END:            ulRetVal = s_aptDpm[uiInst]->ulDpm_win2_end;            break;
    case DPM_PRM_DPM_WIN2_MAP:            ulRetVal = s_aptDpm[uiInst]->ulDpm_win2_map;            break;
    case DPM_PRM_DPM_WIN3_END:            ulRetVal = s_aptDpm[uiInst]->ulDpm_win3_end;            break;
    case DPM_PRM_DPM_WIN3_MAP:            ulRetVal = s_aptDpm[uiInst]->ulDpm_win3_map;            break;
    case DPM_PRM_DPM_WIN4_END:            ulRetVal = s_aptDpm[uiInst]->ulDpm_win4_end;            break;
    case DPM_PRM_DPM_WIN4_MAP:            ulRetVal = s_aptDpm[uiInst]->ulDpm_win4_map;            break;

    case DPM_PRM_DPM_IRQ_RAW:             ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_raw[0];         break;
    case DPM_PRM_DPM_IRQ_RAW1:            ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_raw[1];         break;

    case DPM_PRM_DPM_IRQ_SIRQ_MASK_SET:   ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_sirq_mask_set[0]; break;
    case DPM_PRM_DPM_IRQ_SIRQ_MASK_SET1:  ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_sirq_mask_set[1]; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASK_SET:   ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_dirq_mask_set[0]; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASK_SET1:  ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_dirq_mask_set[1]; break;

    case DPM_PRM_DPM_IRQ_SIRQ_MASK_RESET: ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_sirq_mask_reset[0]; break;
    case DPM_PRM_DPM_IRQ_SIRQ_MASK_RESET1:ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_sirq_mask_reset[1]; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASK_RESET: ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_dirq_mask_reset[0]; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASK_RESET1:ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_dirq_mask_reset[1]; break;

    case DPM_PRM_DPM_IRQ_SIRQ_MASKED:     ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_sirq_masked[0]; break;
    case DPM_PRM_DPM_IRQ_SIRQ_MASKED1:    ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_sirq_masked[1]; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASKED:     ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_dirq_masked[0]; break;
    case DPM_PRM_DPM_IRQ_DIRQ_MASKED1:    ulRetVal = s_aptDpm[uiInst]->aulDpm_irq_host_dirq_masked[1]; break;

    case DPM_PRM_DPM_FW_IRQ_RAW:          ulRetVal = s_aptDpm[uiInst]->ulDpm_firmware_irq_raw;   break;
    case DPM_PRM_DPM_FW_IRQ_MASK:         ulRetVal = s_aptDpm[uiInst]->ulDpm_firmware_irq_mask;   break;
    case DPM_PRM_DPM_SW_IRQ:              ulRetVal = s_aptDpm[uiInst]->ulDpm_sw_irq;              break;
    case DPM_PRM_DPM_CRC:                 ulRetVal = s_aptDpm[uiInst]->ulDpm_crc;                 break;

    default: ulRetVal = 0;
  }

  return ulRetVal;
}
