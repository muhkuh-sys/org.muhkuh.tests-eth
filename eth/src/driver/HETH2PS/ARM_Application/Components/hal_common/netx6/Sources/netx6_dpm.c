/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_dpm.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_DPM

#define DPM_NETX_TO_HOST_BUFFERSIZE 0x0200
#define DPM_HOST_TO_NETX_BUFFERSIZE 0x0400

#define DPM_BOOT_NETX_RECEIVED_CMD  0x01
#define DPM_BOOT_NETX_SEND_CMD      0x02

#define DPM_BOOT_HOST_SEND_CMD      0x01
#define DPM_BOOT_HOST_RECEIVED_CMD  0x02

#define SRT_HANDSHAKE_REG_ARM_DATA  16
#define SRT_HANDSHAKE_REG_PC_DATA   24

typedef struct
{
  volatile uint32_t  ulDpmBootId;
  volatile uint32_t  ulDpmByteSize;
  volatile uint32_t  ulSdramGeneralCtrl;
  volatile uint32_t  ulSdramTimingCtrl;
  volatile uint32_t  ulSdramByteSize;
  volatile uint32_t aulReserved_14[25];
  volatile uint32_t  ulNetxToHostDataSize;
  volatile uint32_t  ulHostToNetxDataSize;
  volatile uint32_t  ulHandshake;
  volatile uint32_t aulReserved_84[31];
  volatile uint32_t aulNetxToHostData[DPM_NETX_TO_HOST_BUFFERSIZE/sizeof(uint32_t)];
  volatile uint32_t aulHostToNetxData[DPM_HOST_TO_NETX_BUFFERSIZE/sizeof(uint32_t)];
} dpmMemoryLayout_t;

static const uint32_t aulCPUSleepImg[62] =
{
  0xf8beaf00, 0x00000000, 0x08008000, 0xdafd3d30,
  0x0000002e, 0x08008000, 0x5854454e, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000001, 0x00000000, 0x00000000, 0xc3eece53,
  0xe3a00000, 0xeb00000d, 0xe59f0024, 0xee010f10,
  0xe3a00000, 0xe59f101c, 0xe1e02000, 0xe5802014,
  0xe580201c, 0xee070f9a, 0xee070f90, 0xeafffffd,
  0x00000000, 0x00000f78, 0x101ff000, 0xe1a00000,
  0xe2400001, 0xe20000ff, 0xe3500001, 0x959f3048,
  0x959f2048, 0x97933100, 0x97920100, 0x93831401,
  0x91830000, 0xe59f3038, 0x83a01401, 0xe5932158,
  0x83a00000, 0xe2022402, 0xe3822401, 0xe5832158,
  0xe5832158, 0xe5832158, 0xe5831158, 0xe5831158,
  0xe5831158, 0xe5830158, 0xe12fff1e, 0x080080b0,
  0x080080a8, 0x1018c000, 0x01000000, 0x00000000,
  0x00000000, 0x02000000
};

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
*   tMode             [in] Mode
*   tEndianess        [in] ENDIANESS
*   ulIfCfg           [in] DPM interface configuration register value
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int NX6_DPM_CfgDpm( NX6_DPM_MODE_E      eMode,
                    NX6_DPM_ENDIANESS_E eEndianess,
                    uint32_t          ulIfCfg )
{
  uint32_t ulDummy;

  /* plausibility check */
  if( eMode > NX6_DPM_MODE_32BIT_MUX ) return -1;
  if( eMode & 1 ) return -1;
  if( eEndianess > NX6_DPM_ENDIANESS_32BIT_BIG ) return -1;

  NX_WRITE32(s_ptDpm->ulDpm_if_cfg, ulIfCfg);

  /* By writing DPM_CFG0x0.Mode the IF Configuration is activated;
   * Mirror configuration value into each byte of the DWORD to make sure that
   * writing the configuration via 8/16 bit interface does not clears the configuration register again;
   * Alternatively a NX_WRITE8 could be used if host interface supports byte access */
  ulDummy  = ((uint32_t)eMode<<SRT_NX51_dpm_cfg0x0_mode) | ((uint32_t)eEndianess<<SRT_NX51_dpm_cfg0x0_endian);
  ulDummy |= (((uint32_t)eMode<<SRT_NX51_dpm_cfg0x0_mode)| ((uint32_t)eEndianess<<SRT_NX51_dpm_cfg0x0_endian))<<8;
  ulDummy |= (((uint32_t)eMode<<SRT_NX51_dpm_cfg0x0_mode)| ((uint32_t)eEndianess<<SRT_NX51_dpm_cfg0x0_endian))<<16;
  ulDummy |= (((uint32_t)eMode<<SRT_NX51_dpm_cfg0x0_mode)| ((uint32_t)eEndianess<<SRT_NX51_dpm_cfg0x0_endian))<<24;
  NX_WRITE32(s_ptDpm->ulDpm_cfg0x0, ulDummy );

  return 0;
}

/*****************************************************************************/
/*! Configure DPM address range and configuration zone location
* \description
*   This function configures the DPM address range and the location of the configuration zone.
* \class
*   DPM
* \params
*   tAddrRange        [in] Address Range
*   tCfgWinAddrCfg    [in] Configuration Window location
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int NX6_DPM_CfgDpmAddr( NX6_DPM_ADDRRANGE_E     eAddrRange,
                        NX6_DPM_CFGWINADDRCFG_E eCfgWinAddrCfg )
{
  /* plausibility check */
  if( (int)eAddrRange > NX6_DPM_DPM_ADDRRANGE_1024KB ) return -1;
  if( (int)eCfgWinAddrCfg > NX6_DPM_CFGWINADDRCFG_DIS ) return -1;

  NX_WRITE32(s_ptDpm->ulDpm_addr_cfg, ((uint32_t)eAddrRange<<SRT_NX51_dpm_addr_cfg_addr_range)
                                     |((uint32_t)eCfgWinAddrCfg<<SRT_NX51_dpm_addr_cfg_cfg_win_addr_cfg)
            );

  return 0;
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
*   uDpmSerialSqiEn  [in] 1/0: DPM serial SQI enable/disable
*   uSdpmMisoEarly   [in] 1/0: Serial DPM MISO early enable/disable
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int NX6_DPM_CfgTiming( unsigned int uTosa,
                       unsigned int uFilter,
                       unsigned int uRds,
                       unsigned int uRdBurstEn,
                       unsigned int uDpmSerialSqiEn,
                       unsigned int uSdpmMisoEarly )
{
  /* plausibility check */
  if( uTosa > 3 ) return -1;
  if( uRds  > 7 ) return -1;

  NX_WRITE32(s_ptDpm->ulDpm_timing_cfg,
    ((uint32_t)uTosa<<SRT_NX51_dpm_timing_cfg_t_osa)
   |((uint32_t)uFilter<<SRT_NX51_dpm_timing_cfg_filter)
   |((uint32_t)uRds<<SRT_NX51_dpm_timing_cfg_t_rds)
   |((uint32_t)uRdBurstEn<<SRT_NX51_dpm_timing_cfg_rd_burst_en)
   |((uint32_t)uDpmSerialSqiEn<<SRT_NX51_dpm_timing_cfg_en_dpm_serial_sqi)
   |((uint32_t)uSdpmMisoEarly<<SRT_NX51_dpm_timing_cfg_sdpm_miso_early)
    );
  return 0;
}

/*****************************************************************************/
/*! Configure DPM RDY signal
* \description
*   This function configures the RDY behavior.
* \class
*   DPM
* \params
*   tRdypol         [in] RDY polarity
*   tRdyDrv         [in] RDY driver
*   tRdyMode        [in] RDY signal Mode
*   tRdyToCfg       [in] RDY timeout configuration
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int NX6_DPM_CfgRdy( NX6_DPM_RDYPOL_E    eRdyPol,
                    NX6_DPM_RDYDRV_E    eRdyDrv,
                    NX6_DPM_RDYMODE_E   eRdyMode,
                    NX6_DPM_RDYTO_CFG_E eRdyToCfg )
{
  /* plausibility check */
  if( (int)eRdyPol>NX6_DPM_RDYPOL_READY_WHEN_HI ) return -1;
  if( (int)eRdyDrv>NX6_DPM_RDYDRV_OPEN_DRAIN ) return -1;
  if( (int)eRdyMode>NX6_DPM_RDYMODE_RDYACK ) return -1;
  if( (int)eRdyToCfg>NX6_DPM_RDYTO_CFG_DIS ) return -1;

  /* disable PIO mode of RDY if RDY enabled */
  if( NX6_DPM_RDYDRV_DIS != eRdyDrv ) {
    NX_WRITE32( s_ptDpm->aulDpm_pio_cfg[1], NX_READ32(s_ptDpm->aulDpm_pio_cfg[1])
                                           &(~MSK_NX51_dpm_pio_cfg1_sel_rdy_pio)
              );
  }

  /* set RDY configuration */
  NX_WRITE32(s_ptDpm->ulDpm_rdy_cfg,
     ((uint32_t)eRdyPol<<SRT_NX51_dpm_rdy_cfg_rdy_pol)
    |((uint32_t)eRdyDrv<<SRT_NX51_dpm_rdy_cfg_rdy_drv_mode)
    |((uint32_t)eRdyMode<<SRT_NX51_dpm_rdy_cfg_rdy_sig_mode)
    |((uint32_t)eRdyToCfg<<SRT_NX51_dpm_rdy_cfg_rdy_to_cfg)
    );
  return 0;
}

/*****************************************************************************/
/*! Configure Miscellaneous
* \description
*   This function configures Miscellaneous parameters.
* \class
*   DPM
* \params
*   ulMiscCfg         [in] Miscellaneous configuration
* \return
*                                                                            */
/*****************************************************************************/
void NX6_DPM_CfgMisc( uint32_t ulMiscCfg )
{
  NX_WRITE32(s_ptDpm->ulDpm_misc_cfg, ulMiscCfg);
}

/*****************************************************************************/
/*! Configure IO Miscellaneous
* \description
*   This function configures IO Miscellaneous parameters.
* \class
*   DPM
* \params
*   ulIoMiscCfg       [in] IO Miscellaneous configuration
* \return
*                                                                            */
/*****************************************************************************/
void NX6_DPM_CfgIoMisc( uint32_t ulIoMiscCfg )
{
  NX_WRITE32(s_ptDpm->ulDpm_io_cfg_misc, ulIoMiscCfg);
}

/*****************************************************************************/
/*! Get PIO Configuration Register 1
* \description
*   Get PIO Configuration Register 1 content.
* \class
*   DPM
* \params
* \return
*   Current dpm_pio_cfg1 value                                                */
/*****************************************************************************/
uint32_t NX6_DPM_GetCfgPio1( void )
{
  return NX_READ32(s_ptDpm->aulDpm_pio_cfg[1]);
}

/*****************************************************************************/
/*! Configure PIO Configuration 1
* \description
*   This function configures the PIO Configuration Register 1
* \class
*   DPM
* \params
*   ulPioCfg1       [in] PIO Configuration 1
* \return
*                                                                            */
/*****************************************************************************/
void NX6_DPM_CfgPio1( uint32_t ulPioCfg1 )
{
  NX_WRITE32(s_ptDpm->aulDpm_pio_cfg[1], ulPioCfg1);
}

/*****************************************************************************/
/*! Configure DPM windows
* \description
*   This function configures all DPM windows.
* \class
*   DPM
* \params
*   patDpmWindowCfg     [in] Pointer to DPM Window mapping structure
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_DPM_CfgDpmWindowMapping( NX6_DPM_WINDOW_MAPPING_T* patDpmWindowCfg )
{
  uint32_t aulWinEnd[5];
  uint32_t aulWinMap[5];

  /* plausibility check */
  if( (patDpmWindowCfg[0].ulWindowSize&0x7f) != 0 ) return -1;
  if( (patDpmWindowCfg[1].ulWindowSize&0x7f) != 0 ) return -1;
  if( (patDpmWindowCfg[2].ulWindowSize&0x7f) != 0 ) return -1;
  if( (patDpmWindowCfg[3].ulWindowSize&0x7f) != 0 ) return -1;
  if( (patDpmWindowCfg[4].ulWindowSize&0x7f) != 0 ) return -1;

  if( (patDpmWindowCfg[0].ulInternalNetXAddr&0x7f) != 0 ) return -1;
  if( (patDpmWindowCfg[1].ulInternalNetXAddr&0x7f) != 0 ) return -1;
  if( (patDpmWindowCfg[2].ulInternalNetXAddr&0x7f) != 0 ) return -1;
  if( (patDpmWindowCfg[3].ulInternalNetXAddr&0x7f) != 0 ) return -1;
  if( (patDpmWindowCfg[4].ulInternalNetXAddr&0x7f) != 0 ) return -1;

  /* calculate register contents */
  aulWinEnd[0] = patDpmWindowCfg[0].ulWindowSize;
  aulWinEnd[1] = aulWinEnd[0]+patDpmWindowCfg[1].ulWindowSize;
  aulWinEnd[2] = aulWinEnd[1]+patDpmWindowCfg[2].ulWindowSize;
  aulWinEnd[3] = aulWinEnd[2]+patDpmWindowCfg[3].ulWindowSize;
  aulWinEnd[4] = aulWinEnd[3]+patDpmWindowCfg[4].ulWindowSize;

  aulWinMap[1] = (patDpmWindowCfg[1].ulInternalNetXAddr & MSK_NX51_dpm_win1_map_win_page)
                |(((patDpmWindowCfg[1].ulInternalNetXAddr & MSK_NX51_dpm_win1_map_win_map)-aulWinEnd[0])&MSK_NX51_dpm_win1_map_win_map);
  aulWinMap[2] = (patDpmWindowCfg[2].ulInternalNetXAddr & MSK_NX51_dpm_win2_map_win_page)
                |(((patDpmWindowCfg[2].ulInternalNetXAddr & MSK_NX51_dpm_win2_map_win_map)-aulWinEnd[1])&MSK_NX51_dpm_win2_map_win_map);
  aulWinMap[3] = (patDpmWindowCfg[3].ulInternalNetXAddr & MSK_NX51_dpm_win3_map_win_page)
                |(((patDpmWindowCfg[3].ulInternalNetXAddr & MSK_NX51_dpm_win3_map_win_map)-aulWinEnd[2])&MSK_NX51_dpm_win3_map_win_map);
  aulWinMap[4] = (patDpmWindowCfg[4].ulInternalNetXAddr & MSK_NX51_dpm_win4_map_win_page)
                |(((patDpmWindowCfg[4].ulInternalNetXAddr & MSK_NX51_dpm_win4_map_win_map)-aulWinEnd[3])&MSK_NX51_dpm_win4_map_win_map);

  /* configure Window 1 */
  NX_WRITE32(s_ptDpm->asDpm_win[0].ulEnd, aulWinEnd[1]);
  NX_WRITE32(s_ptDpm->asDpm_win[0].ulMap, aulWinMap[1]
                                         |((uint32_t)patDpmWindowCfg[1].uDisReadLatch<<SRT_NX51_dpm_win1_map_dis_rd_latch)
                                         |((uint32_t)patDpmWindowCfg[1].uEnReadAhead<<SRT_NX51_dpm_win1_map_read_ahead)
                                         |((uint32_t)patDpmWindowCfg[1].uEnByteAccess<<SRT_NX51_dpm_win1_map_byte_area)
            );

  /* configure Window 2 */
  NX_WRITE32(s_ptDpm->asDpm_win[1].ulEnd, aulWinEnd[2]);
  NX_WRITE32(s_ptDpm->asDpm_win[1].ulMap, aulWinMap[2]
                                         |((uint32_t)patDpmWindowCfg[2].uDisReadLatch<<SRT_NX51_dpm_win2_map_dis_rd_latch)
                                         |((uint32_t)patDpmWindowCfg[2].uEnReadAhead<<SRT_NX51_dpm_win2_map_read_ahead)
                                         |((uint32_t)patDpmWindowCfg[2].uEnByteAccess<<SRT_NX51_dpm_win2_map_byte_area)
            );

  /* configure Window 3 */
  NX_WRITE32(s_ptDpm->asDpm_win[2].ulEnd, aulWinEnd[3]);
  NX_WRITE32(s_ptDpm->asDpm_win[2].ulMap, aulWinMap[3]
                                         |((uint32_t)patDpmWindowCfg[3].uDisReadLatch<<SRT_NX51_dpm_win3_map_dis_rd_latch)
                                         |((uint32_t)patDpmWindowCfg[3].uEnReadAhead<<SRT_NX51_dpm_win3_map_read_ahead)
                                         |((uint32_t)patDpmWindowCfg[3].uEnByteAccess<<SRT_NX51_dpm_win3_map_byte_area)
            );

  /* configure Window 4 */
  NX_WRITE32(s_ptDpm->asDpm_win[3].ulEnd, aulWinEnd[4]);
  NX_WRITE32(s_ptDpm->asDpm_win[3].ulMap, aulWinMap[4]
                                         |((uint32_t)patDpmWindowCfg[4].uDisReadLatch<<SRT_NX51_dpm_win4_map_dis_rd_latch)
                                         |((uint32_t)patDpmWindowCfg[4].uEnReadAhead<<SRT_NX51_dpm_win4_map_read_ahead)
                                         |((uint32_t)patDpmWindowCfg[4].uEnByteAccess<<SRT_NX51_dpm_win4_map_byte_area)
            );
  return 0;
}


/*****************************************************************************/
/*! Setup IRQ
* \description
*   This function configures all interrupt enable masks.
* \class
*   DPM
* \params
*   ulIrqMsk    [in]  IRQ Mask
*   ulFiqMsk    [in]  FIQ Mask
*   ulFiqMsk    [in]  ARM Mask
*   ulFiqMsk    [in]  xPIC Mask
* \return
*                                                                            */
/*****************************************************************************/
void NX6_DPM_SetupIrq( uint32_t ulIrqMsk,
                       uint32_t ulFiqMsk,
                       uint32_t ulArmMsk,
                       uint32_t ulXpicMsk,
                       uint32_t ulFwIrqMask )
{
  /* clear all interrupt enable */
  NX_WRITE32(s_ptDpm->ulDpm_irq_arm_mask_reset  , 0xffffffff);
  NX_WRITE32(s_ptDpm->ulDpm_irq_xpic_mask_reset , 0xffffffff);
  NX_WRITE32(s_ptDpm->ulDpm_irq_fiq_mask_reset  , 0xffffffff);
  NX_WRITE32(s_ptDpm->ulDpm_irq_irq_mask_reset  , 0xffffffff);

  /* set dedicated interrupt enables */
  NX_WRITE32(s_ptDpm->ulDpm_irq_arm_mask_set  , ulArmMsk);
  NX_WRITE32(s_ptDpm->ulDpm_irq_xpic_mask_set , ulXpicMsk);
  NX_WRITE32(s_ptDpm->ulDpm_irq_fiq_mask_set  , ulFiqMsk);
  NX_WRITE32(s_ptDpm->ulDpm_irq_irq_mask_set  , ulIrqMsk);
  NX_WRITE32(s_ptDpm->ulDpm_firmware_irq_mask , ulFwIrqMask);
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
void NX6_DPM_GetStatusErr( uint32_t *pulStatErr,
                           uint32_t *pulStatErrAddr )
{
    *pulStatErr     = NX_READ32(s_ptDpm->ulDpm_status);
    *pulStatErrAddr = NX_READ32(s_ptDpm->ulDpm_status_err_addr);
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
void NX6_DPM_ClearErr( uint32_t ulClearMask )
{
    NX_WRITE32(s_ptDpm->ulDpm_status_err_reset, ulClearMask);
}

/*****************************************************************************/
/*! Get Interrupt
* \description
*   Get current interrupt events.
* \class
*   DPM
* \params
*   tIrqType  [in] Interrupt type
*   pulIrq    [out] Pointer of IRQ value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_DPM_GetIrq( NX6_DPM_IRQ_STATUS_E eIrqType,
                    uint32_t           *pulIrq )
{
  int iRslt = 0;

  switch( eIrqType ) {
    case NX6_DPM_IRQ_STATUS_RAW:         *pulIrq = NX_READ32(s_ptDpm->ulDpm_irq_raw); break;
    case NX6_DPM_IRQ_STATUS_ARM_MASKED:  *pulIrq = NX_READ32(s_ptDpm->ulDpm_irq_arm_masked); break;
    case NX6_DPM_IRQ_STATUS_XPIC_MASKED: *pulIrq = NX_READ32(s_ptDpm->ulDpm_irq_xpic_masked); break;
    case NX6_DPM_IRQ_STATUS_FIQ_MASKED:  *pulIrq = NX_READ32(s_ptDpm->ulDpm_irq_fiq_masked); break;
    case NX6_DPM_IRQ_STATUS_IRQ_MASKED:  *pulIrq = NX_READ32(s_ptDpm->ulDpm_irq_irq_masked); break;
    case NX6_DPM_IRQ_STATUS_FW_IRQ_RAW:  *pulIrq = NX_READ32(s_ptDpm->ulDpm_firmware_irq_raw); break;
    default: iRslt = -1; break; /* invalid instance */
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Get System Status Information
* \description
*   Get sys_sta register.
* \class
*   DPM
* \params
*   pulSysSta         [out] System Status Information
*   pulSysStaBigEnd16 [out] System Status Information Big ENDIAN 16
* \return
*                                                                            */
/*****************************************************************************/
void NX6_DPM_GetSysSta( uint32_t *pulSysSta,
                        uint32_t *pulSysStaBigEnd16 )
{
    *pulSysSta         = NX_READ32(s_ptDpm->ulDpm_sys_sta);
    *pulSysStaBigEnd16 = NX_READ32(s_ptDpm->ulDpm_sys_sta_bigend16);
}

/*****************************************************************************/
/*! Get netX Version
* \description
*   Get netX version.
* \class
*   DPM
* \params
* \return
*  current dpm_netx_version value                                            */
/*****************************************************************************/
uint32_t NX6_DPM_GetNetxVersion( void )
{
    return NX_READ32(s_ptDpm->ulDpm_netx_version);
}

/*****************************************************************************/
/*! Get Status
* \description
*   Get DPM status.
* \class
*   DPM
* \params
* \return
*   Current dpm_status value                                                  */
/*****************************************************************************/
uint32_t NX6_DPM_GetStatus( void )
{
    return NX_READ32(s_ptDpm->ulDpm_status);
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
void NX6_DPM_Reset( void )
{
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0x00);
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0x01);
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0x03);
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0x07);
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0x0f);
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0x1f);
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0x3f);
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0x7f);
    NX_WRITE32(s_ptDpm->ulDpm_reset_request , 0xff);
}


/*****************************************************************************/
/*! Mailbox Download
* \description
*   This function section is required for downloading of binary code to netX 6.
* \class
*   DPM
* \params
* \return
*                                                                            */
/*****************************************************************************/
static uint32_t mailbox_purr( dpmMemoryLayout_t *ptDpm,
                              uint32_t          ulMask )
{
  volatile uint32_t ulValue;
  uint32_t ulHostPart;
  uint32_t ulNetxPart;


  ulValue = NX_READ32(ptDpm->ulHandshake);
  ulHostPart  = ulValue >> SRT_HANDSHAKE_REG_PC_DATA;
  ulHostPart &= 3;
  ulNetxPart  = ulValue >> SRT_HANDSHAKE_REG_ARM_DATA;
  ulNetxPart &= 3;
  /* check for harmony */
  ulValue  = ulHostPart ^ ulNetxPart;
  /* mask */
  ulValue &= ulMask;

  return ulValue;
}

static void mailbox_send_chunk( dpmMemoryLayout_t *ptDpm,
                                const uint32_t    *pulChunk,
                                unsigned int      sizChunk )
{
  const uint32_t *pulSrc;
  volatile uint32_t *pulDstCnt;
  volatile uint32_t *pulDstEnd;
  uint32_t ulValue;


  /* copy the chunk to the host->netX buffer */
  pulSrc = pulChunk;
  pulDstCnt = ptDpm->aulHostToNetxData;
  pulDstEnd = ptDpm->aulHostToNetxData + sizChunk;
  while( pulDstCnt<pulDstEnd )
  {
    NX_WRITE32( *pulDstCnt, *pulSrc );
    ++pulSrc;
    ++pulDstCnt;
  }
  /* set the chunk size */
  NX_WRITE32(ptDpm->ulHostToNetxDataSize, (uint32_t)(sizChunk*sizeof(uint32_t)) );

  /* toggle handshake bit */
  ulValue = NX_READ32(ptDpm->ulHandshake);
  ulValue ^= (uint32_t)DPM_BOOT_HOST_SEND_CMD << SRT_HANDSHAKE_REG_PC_DATA;
  NX_WRITE32(ptDpm->ulHandshake, ulValue);

  /* wait until netX has processed the data */
  do
  {
    ulValue = mailbox_purr(ptDpm, DPM_BOOT_NETX_RECEIVED_CMD);
  } while( ulValue!=0 );
}

static unsigned int mailbox_get_status( dpmMemoryLayout_t *ptDpm )
{
  uint32_t ulValue;
  unsigned int uiResult;

  /* wait for data to receive */
  do
  {
    ulValue = mailbox_purr(ptDpm, DPM_BOOT_NETX_SEND_CMD);
  } while( ulValue==0 );

  /* toggle handshake bit */
  ulValue = NX_READ32(ptDpm->ulHandshake);
  ulValue ^= (uint32_t)DPM_BOOT_HOST_RECEIVED_CMD << SRT_HANDSHAKE_REG_PC_DATA;
  NX_WRITE32(ptDpm->ulHandshake, ulValue);

  uiResult = NX_READ32(ptDpm->aulNetxToHostData[0]) & 0xffU;
  return uiResult;
}

/*****************************************************************************/
/*! Mailbox Copy
* \description
* \class
*   DPM
* \params
* \return
*                                                                            */
/*****************************************************************************/

unsigned int NX6_DPM_MailboxCopy( void )
{
  dpmMemoryLayout_t *ptDpm;
  const uint32_t *pulSrc;
  unsigned int sizData;
  unsigned int sizChunk;
  unsigned int uiResult;


  /* Get the pointer to the RAM area behind the first 256 bytes in the DPM. */
  ptDpm = (dpmMemoryLayout_t*)(((uint8_t*)s_ptDpm)+0x0100U);

  /* get data pointer and length */
  sizData = sizeof(aulCPUSleepImg) / sizeof(uint32_t);
  pulSrc = aulCPUSleepImg;

  /* send header first */
  sizChunk = 64 / sizeof(uint32_t);

  do
  {
    mailbox_send_chunk(ptDpm, pulSrc, sizChunk);
    pulSrc += sizChunk;
    sizData -= sizChunk;

    uiResult = mailbox_get_status(ptDpm);
    if( uiResult!=0 )
    {
      /* Failed to transfer the chunk! */
      break;
    }

    sizChunk = sizData;
    if( sizChunk>((uint32_t)DPM_HOST_TO_NETX_BUFFERSIZE/sizeof(uint32_t)) )
    {
      sizChunk = ((uint32_t)DPM_HOST_TO_NETX_BUFFERSIZE/sizeof(uint32_t));
    }
  } while( sizData!=0 );

  return uiResult;
}



