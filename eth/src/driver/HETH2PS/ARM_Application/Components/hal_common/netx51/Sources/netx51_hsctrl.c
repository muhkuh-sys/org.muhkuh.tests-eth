/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_hsctrl.h"
#include "netx51_dpm.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_HANDSHAKE_CTRL
__USE_DPM

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Configure Handshake cells base address
* \description
*   This function configures the handshake base address within INTRAM 5.
* \class
*   HSCTRL
* \params
*   ulHsBaseAddr     [in] Base address
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_CfgHsBaseAddr( uint32_t ulHsBaseAddr )
{
  /* check 128 byte granularity */
  if( (ulHsBaseAddr&MSK_NX51_handshake_base_addr_zero_ro)!= 0 ) return -1;

  s_ptHsCtrl->ulHandshake_base_addr = ulHsBaseAddr | MSK_NX51_handshake_base_addr_enable;

  return 0;
}

/*****************************************************************************/
/*! Enable Interrupt(s)
* \description
*   This function enables a set of interrupts.
* \class
*   HSCTRL
* \params
*   eSrc     [in] Source
*   ulIrqEn  [in] interrupt enable mask
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_EnIrq( NX51_HSCTRL_IRQ_SRC_E eSrc,
                       uint32_t              ulIrqEn )
{
  int iRslt = 0;

  switch( eSrc ) {
    case NX51_HSCTRL_IRQ_SRC_DPM:  s_ptHsCtrl->ulHandshake_dpm_irq_msk_set  = ulIrqEn; break;
    case NX51_HSCTRL_IRQ_SRC_ARM:  s_ptHsCtrl->ulHandshake_arm_irq_msk_set  = ulIrqEn; break;
    case NX51_HSCTRL_IRQ_SRC_XPIC: s_ptHsCtrl->ulHandshake_xpic_irq_msk_set = ulIrqEn; break;
    default: iRslt = -1; break; /* invalid source */
  }

  return iRslt;
}

/*****************************************************************************/
/*! Disable Interrupt(s)
* \description
*   This function disables a set of interrupts.
* \class
*   HSCTRL
* \params
*   eSrc      [in] Source
*   ulIrqDis  [in] interrupt disable mask
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_DisIrq( NX51_HSCTRL_IRQ_SRC_E eSrc,
                        uint32_t              ulIrqDis )
{
  int iRslt = 0;

  switch( eSrc ) {
    case NX51_HSCTRL_IRQ_SRC_DPM:  s_ptHsCtrl->ulHandshake_dpm_irq_msk_reset  = ulIrqDis; break;
    case NX51_HSCTRL_IRQ_SRC_ARM:  s_ptHsCtrl->ulHandshake_arm_irq_msk_reset  = ulIrqDis; break;
    case NX51_HSCTRL_IRQ_SRC_XPIC: s_ptHsCtrl->ulHandshake_xpic_irq_msk_reset = ulIrqDis; break;
    default: iRslt = -1; break; /* invalid source */
  }

  return iRslt;
}

/*****************************************************************************/
/*! Get Interrupt(s)
* \description
*   This function retrieves the current interrupt masked requests.
* \class
*   HSCTRL
* \params
*   eSrc      [in]  Source
*   pulIrq    [out] Pointer to interrupt events
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_GetIrq( NX51_HSCTRL_IRQ_SRC_E eSrc,
                        uint32_t              *pulIrq )
{
  int iRslt = 0;

  switch( eSrc ) {
    case NX51_HSCTRL_IRQ_SRC_DPM:  *pulIrq = s_ptHsCtrl->ulHandshake_dpm_irq_masked; break;
    case NX51_HSCTRL_IRQ_SRC_ARM:  *pulIrq = s_ptHsCtrl->ulHandshake_arm_irq_masked; break;
    case NX51_HSCTRL_IRQ_SRC_XPIC: *pulIrq = s_ptHsCtrl->ulHandshake_xpic_irq_masked; break;
    default: iRslt = -1; break; /* invalid source */
  }

  return iRslt;
}

/*****************************************************************************/
/*! Get Raw Interrupt(s)
* \description
*   This function retrieves the current interrupt raw requests.
* \class
*   HSCTRL
* \params
*   eSrc      [in]  Source
*   pulIrq    [out] Pointer to interrupt events
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_GetIrqRaw( NX51_HSCTRL_IRQ_SRC_E eSrc,
                           uint32_t              *pulIrq )
{
  int iRslt = 0;

  switch( eSrc ) {
    case NX51_HSCTRL_IRQ_SRC_DPM:  *pulIrq = s_ptHsCtrl->ulHandshake_dpm_irq_raw_clear; break;
    case NX51_HSCTRL_IRQ_SRC_ARM:  *pulIrq = s_ptHsCtrl->ulHandshake_arm_irq_raw_clear; break;
    case NX51_HSCTRL_IRQ_SRC_XPIC: *pulIrq = s_ptHsCtrl->ulHandshake_xpic_irq_raw_clear; break;
    default: iRslt = -1; break; /* invalid source */
  }

  return iRslt;
}

/*****************************************************************************/
/*! Confirm Interrupt(s)
* \description
*   This function confirms a set of interrupts.
* \class
*   HSCTRL
* \params
*  eSrc             [in] Source
*  ulConfirmIrqMask [in] Mask to confirm interrupt events
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_ConfirmIrq( NX51_HSCTRL_IRQ_SRC_E eSrc,
                            uint32_t              ulConfirmIrqMask )
{
  int iRslt = 0;

  switch( eSrc ) {
    case NX51_HSCTRL_IRQ_SRC_DPM:  s_ptHsCtrl->ulHandshake_dpm_irq_raw_clear  = ulConfirmIrqMask; break;
    case NX51_HSCTRL_IRQ_SRC_ARM:  s_ptHsCtrl->ulHandshake_arm_irq_raw_clear  = ulConfirmIrqMask; break;
    case NX51_HSCTRL_IRQ_SRC_XPIC: s_ptHsCtrl->ulHandshake_xpic_irq_raw_clear = ulConfirmIrqMask; break;
    default: iRslt = -1; break; /* invalid source */
  }

  return iRslt;
}

/*****************************************************************************/
/*! Configure Handshake cell mode
* \description
*   This function configures the mode of a handshake cell.
* \class
*   HSCTRL
* \params
*   uInst            [in] Handshake cell number
*   eHsCellmode      [in] Handshake cell mode
*   eHsCellWidth     [in] Handshake cell width
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_CfgHsCell( unsigned int               uInst,
                           NX51_HSCTRL_CELL_MODE_E    eHsCellmode,
                           NX51_HSCTRL_HSCELL_WIDTH_E eHsCellWidth )
{
  /* check number of instances */
  if( uInst >= NX51_NUM_HS_CELL )
    return -1;

  s_ptHsCtrl->aulHandshake_hsc_ctrl[uInst] =  ((uint32_t)eHsCellmode  << SRT_NX51_handshake_hsc0_ctrl_mode)
                                             |((uint32_t)eHsCellWidth << SRT_NX51_handshake_hsc0_ctrl_width);

  return 0;
}

/*****************************************************************************/
/*! Configure Handshake BufMan0
* \description
*   This function configures the PdOut buffer manager of the handshake control unit.
* \class
*   HSCTRL
* \params
*  eAddrmapCfg      [in] Address mapping configuration
*  uAutoMode        [in] 1/0: Automatic mode enabled/disabled
*  ulWinMapBuf1     [in] Alternate mapping buffer 1
*  ulWinMapBuf2     [in] Alternate mapping buffer 2
* \return
*                                                                            */
/*****************************************************************************/
void NX51_HSCTRL_CfgBufmanPdOut( NX51_HSCTRL_BUFMAN_ADDRMAPCFG_E eAddrmapCfg,
                                 unsigned int                    uAutoMode,
                                 uint32_t                        ulWinMapBuf1,
                                 uint32_t                        ulWinMapBuf2 )
{
  /* configure alternative window mappings */
  s_ptHsCtrl->asHandshake_buf_man[0].ulWin_map =  ( (((ulWinMapBuf1 & 0xfffff)- s_ptDpm->asDpm_win[0].ulEnd) >> 7) << SRT_NX51_handshake_buf_man0_win_map_win_map_buf1)
                                                 |( (((ulWinMapBuf2 & 0xfffff)- s_ptDpm->asDpm_win[0].ulEnd) >> 7) << SRT_NX51_handshake_buf_man0_win_map_win_map_buf2);

  /* configure AutoMode */
  s_ptHsCtrl->asHandshake_buf_man[0].ulCtrl    =  (((uint32_t)uAutoMode   << SRT_NX51_handshake_buf_man0_ctrl_hsc2_auto_PD_OUT)&MSK_NX51_handshake_buf_man0_ctrl_hsc2_auto_PD_OUT)
                                                  |((uint32_t)eAddrmapCfg << SRT_NX51_handshake_buf_man0_ctrl_buf_dam_cfg);

  /* Reset BufMan (self-clearing) */
  s_ptHsCtrl->asHandshake_buf_man[0].ulCtrl   |=  MSK_NX51_handshake_buf_man0_ctrl_reset;

  /* supply first write buffer for host if BufMan enabled */
  if( eAddrmapCfg == NX51_HSCTRL_BUFMAN_ADDRMAPCFG_CURBUF ) {
    /* request write buffer to get first buffer */
    s_ptHsCtrl->asHandshake_buf_man[0].ulStatus_ctrl_host = (uint32_t)NX51_HSCTRL_BUFMAN_CMD_REQ_WRITE_BUF << SRT_NX51_handshake_buf_man0_status_ctrl_host_cmd;
    /* request write buffer to release first buffer and to get first buffer for cyclic writing */
    s_ptHsCtrl->asHandshake_buf_man[0].ulStatus_ctrl_host = (uint32_t)NX51_HSCTRL_BUFMAN_CMD_REQ_WRITE_BUF << SRT_NX51_handshake_buf_man0_status_ctrl_host_cmd;
  }
}

/*****************************************************************************/
/*! Configure Handshake BufMan1
* \description
*   This function configures the PdIn buffer manager of the handshake control unit.
* \class
*   HSCTRL
* \params
*  eAddrmapCfg      [in] Address mapping configuration
*  uAutoMode        [in] 1/0: Automatic mode enabled/disabled
*  ulWinMapBuf1     [in] Alternate mapping buffer 1
*  ulWinMapBuf2     [in] Alternate mapping buffer 2
* \return
*                                                                            */
/*****************************************************************************/
void NX51_HSCTRL_CfgBufmanPdIn( NX51_HSCTRL_BUFMAN_ADDRMAPCFG_E eAddrmapCfg,
                                unsigned int                    uAutoMode,
                                uint32_t                        ulWinMapBuf1,
                                uint32_t                        ulWinMapBuf2 )
{
  /* configure alternative window mappings */
  s_ptHsCtrl->asHandshake_buf_man[1].ulWin_map =  ( (((ulWinMapBuf1&0xfffff)- s_ptDpm->asDpm_win[1].ulEnd) >> 7) << SRT_NX51_handshake_buf_man1_win_map_win_map_buf1)
                                                 |( (((ulWinMapBuf2&0xfffff)- s_ptDpm->asDpm_win[1].ulEnd) >> 7) << SRT_NX51_handshake_buf_man1_win_map_win_map_buf2);
  /* configure AutoMode */
  s_ptHsCtrl->asHandshake_buf_man[1].ulCtrl    = (((uint32_t)uAutoMode   << SRT_NX51_handshake_buf_man1_ctrl_hsc2_auto_PD_IN) & MSK_NX51_handshake_buf_man1_ctrl_hsc2_auto_PD_IN)
                                                 |((uint32_t)eAddrmapCfg << SRT_NX51_handshake_buf_man1_ctrl_buf_dam_cfg);

  /* Reset BufMan (self-clearing) */
  s_ptHsCtrl->asHandshake_buf_man[1].ulCtrl   |=  MSK_NX51_handshake_buf_man1_ctrl_reset;

  /* supply first buffer for netX if BufMan enabled */
  if( eAddrmapCfg == NX51_HSCTRL_BUFMAN_ADDRMAPCFG_CURBUF ) {
    /* request write buffer to get first buffer */
    s_ptHsCtrl->asHandshake_buf_man[1].ulStatus_ctrl_netx = (uint32_t)NX51_HSCTRL_BUFMAN_CMD_REQ_WRITE_BUF << SRT_NX51_handshake_buf_man1_status_ctrl_netx_cmd;
    /* request write buffer to release first buffer and to get first buffer for cyclic writing */
    s_ptHsCtrl->asHandshake_buf_man[1].ulStatus_ctrl_netx = (uint32_t)NX51_HSCTRL_BUFMAN_CMD_REQ_WRITE_BUF << SRT_NX51_handshake_buf_man1_status_ctrl_netx_cmd;
  }
}

/*****************************************************************************/
/*! Reset bufMan
* \description
*   This function resets a BufMan unit.
* \class
*   HSCTRL
* \params
*  uInst            [in] 0/1: BufMan PdOut/PdIn
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_ResetBufMan( unsigned int uInst )
{
  /* check number of instances */
  if( uInst >= NX51_NUM_HS_BUFMAN )
    return -1;

  /* Reset BufMan (self-clearing) */
  s_ptHsCtrl->asHandshake_buf_man[uInst].ulCtrl |= MSK_NX51_handshake_buf_man1_ctrl_reset;

  return 0;
}

/*****************************************************************************/
/*! Set BufMan Command
* \description
*   This function setups a command to a buffer manager.
* \class
*   HSCTRL
* \params
*  uInst          [in] Buffer manager number
*  tCmd           [in] Command
*  tCmdMaster     [in] Command initiated by Host/netX
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_SetBufManCmd( unsigned int              uInst,
                              NX51_HSCTRL_BUFMAN_CMDM_E eCmdMaster,
                              NX51_HSCTRL_BUFMAN_CMD_E  eCmd )
{
  int iRslt = 0;

  /* check number of instances */
  if( uInst >= NX51_NUM_HS_BUFMAN )
    return -1;

  switch( eCmdMaster ) {
    case NX51_HSCTRL_BUFMAN_CMDM_NETX:
      s_ptHsCtrl->asHandshake_buf_man[uInst].ulStatus_ctrl_netx = (uint32_t)eCmd << SRT_NX51_handshake_buf_man0_status_ctrl_netx_cmd;
    break;

    case NX51_HSCTRL_BUFMAN_CMDM_HOST:
      s_ptHsCtrl->asHandshake_buf_man[uInst].ulStatus_ctrl_host = (uint32_t)eCmd << SRT_NX51_handshake_buf_man0_status_ctrl_host_cmd;
    break;

    default:
      /* invalid command master */
      iRslt = -1;
    break;
  } /* end switch */
  return iRslt;
}

/*****************************************************************************/
/*! Get BufMan Status
* \description
*   This function retrieves the current buffer manager status.
* \class
*   HSCTRL
* \params
*  uInst             [in]  Buffer manager instance
*  ptBufManStatNetx  [out] Pointer to netX status
*  ptBufManStatHost  [out] Pointer to host status
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_GetBufManStat( unsigned int               uInst,
                               NX51_HSCTRL_BUFMAN_STAT_E* ptBufManStatNetx,
                               NX51_HSCTRL_BUFMAN_STAT_E* ptBufManStatHost )
{
  /* check number of instances */
  if( uInst >= NX51_NUM_HS_BUFMAN )
    return -1;

  *ptBufManStatNetx = (NX51_HSCTRL_BUFMAN_STAT_E)(s_ptHsCtrl->asHandshake_buf_man[uInst].ulStatus_ctrl_netx & MSK_NX51_handshake_buf_man0_status_ctrl_netx_buf_ro);
  *ptBufManStatHost = (NX51_HSCTRL_BUFMAN_STAT_E)(s_ptHsCtrl->asHandshake_buf_man[uInst].ulStatus_ctrl_host & MSK_NX51_handshake_buf_man0_status_ctrl_host_buf_ro);

  return 0;
}

/*****************************************************************************/
/*! Read Handshake Cell
* \description
*   This function retrieves the current handshake cell value.
* \class
*   HSCTRL
* \params
*  uInst             [in]  Handshake cell instance
*  pulHandshakeVal    [out] Pointer to handshake cell value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_ReadHsCell( unsigned int uInst,
                            uint32_t*    pulHandshakeVal )
{
  uint32_t* pulHsCellAddr;

  /* check number of instances */
  if( uInst >= NX51_NUM_HS_CELL ) return -1;

  /* calculate address of handshake cell */
  pulHsCellAddr = (uint32_t*) ( Addr_NX51_intramhs_arm_mirror+NX51_OFFSET_CH_HS+(uInst*sizeof(uint32_t)) );

  /* read value */
  *pulHandshakeVal = *pulHsCellAddr;

  return 0;
}

/*****************************************************************************/
/*! Write Handshake Cell
* \description
*   This function write the a handshake cell value.
* \class
*   HSCTRL
* \params
*  uInst             [in]  Handshake cell instance
*  ulHandshakeVal    [in]  Handshake cell value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_WriteHsCell( unsigned int uInst,
                             uint32_t     ulHandshakeVal )
{
  uint32_t* pulHsCellAddr;

  /* check number of instances */
  if( uInst >= NX51_NUM_HS_CELL ) return -1;

  /* calculate address of handshake cell */
  pulHsCellAddr = (uint32_t*) ( Addr_NX51_intramhs_arm_mirror+NX51_OFFSET_CH_HS+(uInst*sizeof(uint32_t)) );

  /* write value */
  *pulHsCellAddr = ulHandshakeVal;

  return 0;
}

/*****************************************************************************/
/*! Write Handshake Cell
* \description
*   This function write the a handshake cell value.
* \class
*   HSCTRL
* \params
*  uInst             [in]  Handshake cell instance
*  ulHandshakeVal    [in]  Handshake cell value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_HSCTRL_ToggleHsCellFlag( unsigned int uInst,
                                  uint32_t     ulHsCellBitNum )
{
  uint32_t* pulHsCellAddr;

  /* check number of instances */
  if( uInst >= NX51_NUM_HS_CELL ) return -1;

  /* calculate address of handshake cell */
  pulHsCellAddr = (uint32_t*) ( Addr_NX51_intramhs_arm_mirror+NX51_OFFSET_CH_HS+(uInst*sizeof(uint32_t)) );

  /* toggle value */
  *pulHsCellAddr ^= ((uint32_t)1<<ulHsCellBitNum);

  return 0;
}

/*****************************************************************************/
/*! Check for handshake flags equal
* \description
*   This function checks for equality of two handshake flags.
* \class
*   HSCTRL
* \params
*  uInst             [in]  Handshake cell instance
*  ulHsFlagNum1      [in]  Handshake cell flag 1
*  ulHsFlagNum2      [in]  Handshake cell flag 2
* \return
*   Equality value                                                           */
/*****************************************************************************/
int NX51_HSCTRL_AreHsFlagsEqual( unsigned int uInst,
                                 uint32_t     ulHsFlagNum1,
                                 uint32_t     ulHsFlagNum2 )
{
  volatile uint32_t* pulHsCellAddr;
  uint32_t ulHsVal;

  /* calculate address of handshake cell */
  pulHsCellAddr = (volatile uint32_t*) ( Addr_NX51_intramhs_arm_mirror + NX51_OFFSET_CH_HS + (uInst * sizeof(uint32_t)) );

  ulHsVal = *pulHsCellAddr;

  /* return equality value */
  return ( ((ulHsVal >> ulHsFlagNum1) & 1) == ((ulHsVal >> ulHsFlagNum2) & 1) ) ? 1 : 0;
}
