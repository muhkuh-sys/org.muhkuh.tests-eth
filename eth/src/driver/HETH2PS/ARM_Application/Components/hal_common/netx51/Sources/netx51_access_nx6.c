/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_access_nx6.h"
#include "common_func_netx51.h"
#include "netx6_dpm.h"
#include "regdef_netx51.h"
#include "dpm_mapping_netx6.h"

/*
 #define __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
 */
#define EXPECTED_NETX6_VERSION 0x43

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/




/*****************************************************************************/
/*! Reset netX 6
* \description
*   This static function resets the netX 6.
* \class
*   ACCESSNX6
* \params
* \return
*                                                                            */
/*****************************************************************************/
static void ResetNetX6 ( void )
{
  NX51_AsicCtrl_SetResetCtrl( (MSK_NX51_reset_ctrl_RES_REQ_OUT|MSK_NX51_reset_ctrl_EN_RES_REQ_OUT) | NX51_AsicCtrl_GetResetCtrl() );
  NX51_GPIO_Sleep( 0, 100000 ); /* Delay [CC] */
  NX51_AsicCtrl_SetResetCtrl( ~(MSK_NX51_reset_ctrl_RES_REQ_OUT) & NX51_AsicCtrl_GetResetCtrl() );
  NX51_GPIO_Sleep( 0, 100000 ); /* Delay [CC] */
}

/*****************************************************************************/
/*! Setup DPM window mapping
* \description
*   This static function configures the DPM window mapping.
* \class
*   ACCESSNX6
* \params
*   fIntramRequired  [in]  Intram required
* \return
*                                                                            */
/*****************************************************************************/
static void SetupDpmWindowMapping ( bool fIntramRequired )
{
  NX6_DPM_WINDOW_MAPPING_T atDpmWindowCfg[5];

  /* setup DPM windows */
  atDpmWindowCfg[0].ulInternalNetXAddr = 0; /* must be zero */
  atDpmWindowCfg[0].ulWindowSize       = 256; /* DPM configuration window */
  atDpmWindowCfg[0].uEnByteAccess      = 0;
  atDpmWindowCfg[0].uEnReadAhead       = 0;
  atDpmWindowCfg[0].uDisReadLatch      = 0;

  atDpmWindowCfg[1].ulInternalNetXAddr = Addr_NX51_xpic_pram;
  atDpmWindowCfg[1].ulWindowSize       = 57344;
  atDpmWindowCfg[1].uEnByteAccess      = 0;
  atDpmWindowCfg[1].uEnReadAhead       = 0;
  atDpmWindowCfg[1].uDisReadLatch      = 0;

  atDpmWindowCfg[2].ulInternalNetXAddr = Addr_NX51_xpic_dram;
  atDpmWindowCfg[2].ulWindowSize       = 40960;
  atDpmWindowCfg[2].uEnByteAccess      = 0;
  atDpmWindowCfg[2].uEnReadAhead       = 0;
  atDpmWindowCfg[2].uDisReadLatch      = 0;

  /* Note: Mapping of INTRAM 6/7/8 only done when required, if INTRAM access not required then DPM size reduced from 256 kByte to 128 kByte  */
  atDpmWindowCfg[3].ulInternalNetXAddr = (fIntramRequired) ? Addr_NX51_intram7 : 0;
  atDpmWindowCfg[3].ulWindowSize       = (fIntramRequired) ? 131072 : 0;
  atDpmWindowCfg[3].uEnByteAccess      = (fIntramRequired) ? 1 : 0;
  atDpmWindowCfg[3].uEnReadAhead       = 0;
  atDpmWindowCfg[3].uDisReadLatch      = 0;

  atDpmWindowCfg[4].ulInternalNetXAddr = (fIntramRequired) ? Addr_NX51_intram6 : 0;
  atDpmWindowCfg[4].ulWindowSize       = (fIntramRequired) ? (32768 - 256) : 0; /* Note: Last 256 byte of INTRAM6 are not accessible */
  atDpmWindowCfg[4].uEnByteAccess      = (fIntramRequired) ? 1 : 0;
  atDpmWindowCfg[4].uEnReadAhead       = 0;
  atDpmWindowCfg[4].uDisReadLatch      = 0;

  if( 0 != NX6_DPM_CfgDpmWindowMapping( &atDpmWindowCfg[0] ) ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED ); /* ERROR: failed */
    while(1) {}
  }
}

/*****************************************************************************/
/*! Setup Host Interrupt
* \description
*   This static function initializes the DPM interrupt.
* \class
*   ACCESSNX6
* \params
*   uDirqPol  [in]  Dirq polarity
* \return
*                                                                            */
/*****************************************************************************/
static void SetupDpmInterrupt ( unsigned int uDirqPol )
{
  /* configure DIRQ to low active and always driven */
  NX6_DPM_CfgIoMisc( DFLT_BF_VAL_NX51_dpm_io_cfg_misc_fiq_pol
                    |DFLT_VAL_NX51_dpm_io_cfg_misc_fiq_oec
                    |((uint32_t)0<<SRT_NX51_dpm_io_cfg_misc_irq_oec) /* DIRQ is always driven */
                    |((uint32_t)uDirqPol<<SRT_NX51_dpm_io_cfg_misc_irq_pol)
                   );

  /* disable PIO mode of DIRQ */
  NX6_DPM_CfgPio1( NX6_DPM_GetCfgPio1() & (~MSK_NX51_dpm_pio_cfg1_sel_dirq_pio) );

}

/*****************************************************************************/
/*! ACCESSNX6 Setup Parallel DPM
* \description
*   This function initializes the parallel DPM interface.
* \class
*   ACCESSNX6
* \params
*   uWidth           [in]  Data width
*   fEnRdy           [in]  Ready enable
*   uRdyPol          [in]  Ready polarity
*   fIntramRequired  [in]  Intram required
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int NX51_ACCESSNX6_SetupParallelDpm( unsigned int uWidth,
                                     bool         fEnRdy,
                                     unsigned int uRdyPol,
                                     bool         fIntramRequired )
{
  NX6_DPM_MODE_E eNx6DpmCfgDpmMode;
  NX51_HIF_IOCTRL_HIF_MI_CFG_E eNx51HifioCtrlMiCfg;
  NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_E eNx51HifAsyncMemCtrlCsAreaDwidth;

  /* plausibility checks */
  if(  (uRdyPol!=0)
     &&(uRdyPol!=1)
    ) {
      return -1;
  }

  /* set configuration parameters */
  switch( uWidth ) {
  case 8:
    eNx6DpmCfgDpmMode = NX6_DPM_MODE_8BIT_NON_MUX;
    eNx51HifioCtrlMiCfg = NX51_HIF_IOCTRL_HIF_MI_CFG_8BIT_MI;
    eNx51HifAsyncMemCtrlCsAreaDwidth = NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_8BIT;
    break;
  case 16:
    eNx6DpmCfgDpmMode = NX6_DPM_MODE_16BIT_NON_MUX;
    eNx51HifioCtrlMiCfg = NX51_HIF_IOCTRL_HIF_MI_CFG_16BIT_MI;
    eNx51HifAsyncMemCtrlCsAreaDwidth = NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_16BIT;
    break;
  case 32:
    eNx6DpmCfgDpmMode = NX6_DPM_MODE_32BIT_NON_MUX;
    eNx51HifioCtrlMiCfg = NX51_HIF_IOCTRL_HIF_MI_CFG_32BIT_MI;
    eNx51HifAsyncMemCtrlCsAreaDwidth = NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_32BIT;
    break;
  default:
    return -1;
  }

  /* reset HIF IO Control area */
  NX51_HIF_IOCTRL_DeInit();

  /* reset HIF Extension Bus */
  NX51_HIF_ASYNCMEM_CTRL_Deinit();

  /* setup netx51 extension bus globally to 8 bit data width without RDY and 2 kByte address window */
  if( 0 != NX51_HIF_IOCTRL_SetIoCfg( NX51_HIF_IOCTRL_HIF_MI_CFG_8BIT_MI,
                                     NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_2K,
                                     1 /* EnHifRdyPioMi */,
                                     0 /* Disable SDRAM */
                                   )
     )
  {
    return -1;
  }

  /* setup netx51 extension bus CS area 0 to 8 bit without RDY and worst case timing ***/
  if( 0 != NX51_HIF_ASYNCMEM_CTRL_SetupCsArea( 0,
                                               NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_8BIT,
                                               63, /* WS */
                                               3,  /* PRE-Pause */
                                               3,  /* POST-Pause */
                                               0,
                                               0,
                                               0,  /* Static CS */
                                               0   /* EnRDY */
                                              )
    )
  {
    return -1;
  }

  /* reset netX 6 */
  ResetNetX6();

  /* check that netx6 DPM configuration window is accessible */
  if( EXPECTED_NETX6_VERSION != (NX6_DPM_GetNetxVersion()&0xff) ) {
    return -1;
  }

  /* wait until netx6 DPM is unlocked */
  while( 0==(NX6_DPM_GetStatus()&MSK_NX51_dpm_status_unlocked)) { }

  /* set netX 6 CPU into sleep mode, this must be done to avoid DPM access errors */
  if( 0 != NX6_DPM_MailboxCopy() )  {
    return -1;
  }

  /* setup netx6 DPM with optimized timings and mode */
  if( 0 != NX6_DPM_CfgTiming( 3, /* T_osa */
                              0, /* Filter */
                              2, /* T_rds */
                              0, /* Rd_burst_en */
                              0, /* DpmSerialSqiEn */
                              0  /* SdpmMisoEarly */
                            )
    ) {
    return -1;
  }

  /* setup netx6 DPM RDY behavior */
  if( 0 != NX6_DPM_CfgRdy( fEnRdy ? (NX6_DPM_RDYPOL_E)uRdyPol : NX6_DPM_RDYPOL_BUSY_WHEN_HI,
                           fEnRdy ? NX6_DPM_RDYDRV_PUSH_PULL : NX6_DPM_RDYDRV_DIS, /* if RDY enabled then PushPull else Disabled */
                           NX6_DPM_RDYMODE_WAITBUSY, /* RDY is generated as Wait/Busy state signal */
                           fEnRdy ? NX6_DPM_RDYTO_CFG_256CC : NX6_DPM_RDYTO_CFG_DIS /* RDY timeout only when RDY enabled */
                         )
    ) {
    return -1;
  }

  /* setup netx6 to final data width and ENDIANESS */
  if( 0 != NX6_DPM_CfgDpm( eNx6DpmCfgDpmMode,
                           NX6_DPM_ENDIANESS_LITTLE,
                           0 /* see REGDEF for details */
                         )
    ) {
    return -1;
  }

  /* setup netx51 extension bus globally to final data width and RDY configuration, but keep address width of 2 kByte */
  if( 0 != NX51_HIF_IOCTRL_SetIoCfg( eNx51HifioCtrlMiCfg,
                                     NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_2K,
                                     fEnRdy ? 0 : 1 /* EnHifRdyPioMi */,
                                     0 /* Disable SDRAM */
                                   )
     ) {
    return -1;
  }

  /* setup netx51 extension bus RDY behavior globally */
  if( 0 != NX51_HIF_ASYNCMEM_CTRL_SetRdyCfg( fEnRdy ? uRdyPol : 0, /* RdyActLevel */
                                             NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_OFF, /* RdyFilter */
                                             fEnRdy ? 0 : 1, /* Disable RDY timeout when RDY not used */
                                             fEnRdy ? 1 : 0  /* RDY timeout IRQ enabled when RDY used */
                                           )
    ) {
    return -1;
  }

  /* setup netx51 extension bus CS area 0 to final data width and optimized timing */
  if( 0 != NX51_HIF_ASYNCMEM_CTRL_SetupCsArea( 0,
                                               eNx51HifAsyncMemCtrlCsAreaDwidth,
                                               /* Wait states are always generated independent of RDY is used,
                                                * when RDY is used make sure that after number of Wait states the RDY is busy, access is prolonged by RDY signal,
                                                * when RDY is not used make sure that after number of WS the DPM access is finished  */
                                               fEnRdy ? 10 : 40, /* Number of Wait states with/without RDY */
                                               3,  /* PRE-Pause */
                                               3,  /* POST-Pause */
                                               0,
                                               0,
                                               0,  /* Static CS */
                                               fEnRdy /* EnRDY */
                                              )
    )
  {
    return -1;
  }

  /* setup netx51 extension bus globally to final address width */
  if( 0 != NX51_HIF_IOCTRL_SetIoCfg( eNx51HifioCtrlMiCfg,
                                     fIntramRequired ? NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_256K : NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_128K,
                                     fEnRdy ? 0 : 1 /* EnHifRdyPioMi */,
                                     0 /* Disable SDRAM */
                                   )
     ) {
    return -1;
  }

  /* setup netx6 to final address width and DPM configuration zone within first 256 byte */
  if( 0 != NX6_DPM_CfgDpmAddr(  fIntramRequired ? NX6_DPM_DPM_ADDRRANGE_256KB : NX6_DPM_DPM_ADDRRANGE_128KB,
                                NX6_DPM_CFGWINADDRCFG_FIRST
                         )
    ) {
    return -1;
  }

  /* check for DPM configuration window is still accessible */
  if( EXPECTED_NETX6_VERSION != (NX6_DPM_GetNetxVersion()&0xff) ) {
    return -1;
  }

  /* Setup DPM window mapping */
  SetupDpmWindowMapping( fIntramRequired );

  /* Setup DPM Interrupt, DIRQ is active low */
  SetupDpmInterrupt( 0 );

  /* wait until netx6 DPM is unlocked */
  while( 0==(NX6_DPM_GetStatus()&MSK_NX51_dpm_status_unlocked)) { }

  return 0;
}

/*****************************************************************************/
/*! ACCESSNX6 Setup Serial DPM
* \description
*   This function initializes the serial DPM interface.
* \class
*   ACCESSNX6
* \params
*   usClkFreq        [in]  clock frequency
*   fIntramRequired  [in]  Intram required
* \return
*   0 on success
*   -1 on error                                                              */
/*****************************************************************************/
int NX51_ACCESSNX6_SetupSerialDpm( uint16_t usClkFreq,
                                   bool     fIntramRequired )
{
  /* reset HIF IO Control area */
  NX51_HIF_IOCTRL_DeInit();

  /* Reset SPI 1 interface */
  NX51_SPIMOTION_DeInit();

  /* setup netx51 SPI 1 interface as Master, set speed and mode */
  if( 0 != NX51_SPIMOTION_Init( NX51_SPIMOTION_MODE_SELECT_MASTER,
                                (NX51_SPIMOTION_SCLK_FREQ)usClkFreq,
                                NX51_SPIMOTION_SERIAL_CLK_PHASE_1ST, /* SPI mode 0 */
                                NX51_SPIMOTION_SERIAL_CLK_POL_LO,  /* SPI mode 0 */
                                8, /* byte wise */
                                NX51_SPIMOTION_CS_MODE_STATIC
                              )
    ) {
    return -1;
  }

  /* reset netX 6 */
  ResetNetX6();

  /* do two dummy accesses to enable serial DPM */
  /*lint -e(534) read value is not of interest */
  NX51_ACCESSNX6_SerialDpmRead( 4, 0 );
  /*lint -e(534) read value is not of interest */
  NX51_ACCESSNX6_SerialDpmRead( 4, 0 );

  /* check that netx6 DPM configuration window is accessible */
  if( EXPECTED_NETX6_VERSION != (NX6_DPM_GetNetxVersion()&0xff) ) {
    return -1;
  }

  /* wait until netx6 DPM is unlocked */
  while( 0==(NX6_DPM_GetStatus()&MSK_NX51_dpm_status_unlocked)) { }

  /* set netX 6 CPU into sleep mode, this must be done to avoid DPM access errors */
  if( 0 != NX6_DPM_MailboxCopy() )  {
    return -1;
  }

  /* setup netx6 to final data width and ENDIANESS */
  if( 0 != NX6_DPM_CfgDpm( NX6_DPM_MODE_SERIAL,
                           NX6_DPM_ENDIANESS_LITTLE,
                           0 /* see REGDEF for details */
                         )
    ) {
    return -1;
  }

  /* setup netx6 to final address width and DPM configuration zone within first 256 byte */
  if( 0 != NX6_DPM_CfgDpmAddr(  fIntramRequired ? NX6_DPM_DPM_ADDRRANGE_256KB : NX6_DPM_DPM_ADDRRANGE_128KB,
                                NX6_DPM_CFGWINADDRCFG_FIRST
                         )
    ) {
    return -1;
  }

  /* check for DPM configuration window is still accessible */
  if( EXPECTED_NETX6_VERSION != (NX6_DPM_GetNetxVersion()&0xff) ) {
    return -1;
  }

  /* Setup DPM window mapping */
  SetupDpmWindowMapping( fIntramRequired );

  /* Setup DPM Interrupt, DIRQ is active high */
  SetupDpmInterrupt( 1 );

  /* wait until netx6 DPM is unlocked */
  while( 0==(NX6_DPM_GetStatus()&MSK_NX51_dpm_status_unlocked)) { }

  return 0;
}

/*****************************************************************************/
/*! ACCESSNX6 Read BYTE/WORD/DWORD via Serial DPM
* \description
*   This function reads a BYTE/WORD/DWORD via serial DPM interface.
* \class
*   ACCESSNX6
* \params
*   uSize        [in]  Length in bytes
*   ulDpmAddr    [in]  DPM address
* \return
*   Read data                                                                */
/*****************************************************************************/
static uint32_t NX51_ACCESSNX6_SerialDpmReadIntram( unsigned int uSize,
                                                    uint32_t     ulDpmAddr )
{
  uint32_t ulData;
  uint8_t abTxData[8];
  uint8_t abRxBuffer[8];
  IRQSTATE irq;

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  uint32_t ulStatus;
#endif

  /* prepare transmit data */
  abTxData[0] = 0x80 | ((ulDpmAddr >> 16) & 0xf);    /* set command=READ, Address[19:16] */
  abTxData[1] = (ulDpmAddr >> 8) & 0xff;             /* set Address[15:8] */
  abTxData[2] = ulDpmAddr & 0xff;                    /* set Address[7:0] */
  abTxData[3] = uSize;                               /* length in bytes */
  abTxData[4] = 0x00;                                /* dummy data because read transfer */
  abTxData[5] = 0x00;                                /* dummy data because read transfer */
  abTxData[6] = 0x00;                                /* dummy data because read transfer */
  abTxData[7] = 0x00;                                /* dummy data because read transfer */

  /* lock IRQ */
  irq = IRQ_LockSave();

  /* assert CS */
  NX51_SPIMOTION_SelectChip( 0 );

  /* do SPI data exchange */
  NX51_SPIMOTION_ExchangeData8( 4+uSize, abTxData, abRxBuffer );

  /* DE-assert CS */
  NX51_SPIMOTION_DeSelectChip( 0 );

  /* unlock IRQ */
  IRQ_LockRestore(irq);

  /* get received data bytes */
  ulData  = (abRxBuffer[4] & 0xff);
  if( uSize>1 ) {
    ulData |= (abRxBuffer[5] & 0xff) << 8;
  }
  if( uSize>2 ) {
    ulData |= (abRxBuffer[6] & 0xff) << 16;
    ulData |= (abRxBuffer[7] & 0xff) << 24;
  }

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  ulStatus = abRxBuffer[0];

  /* check DPM status */
  if( ulStatus != (MSK_NX51_dpm_status_unlocked
                  |(MSK_NX51_dpm_status_sel_dpm_serial>>3) /* sel_dpm_serial status re-mapped in case of sDPM byte */
                  )
    ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif

  return ulData;
}

/*****************************************************************************/
/*! ACCESSNX6 Read DWORD via Serial DPM
* \description
*   This function reads a DWORD via serial DPM interface.
* \class
*   ACCESSNX6
* \params
*   ulDpmAddr    [in]  DPM address
* \return
*   Read data                                                                */
/*****************************************************************************/
static uint32_t NX51_ACCESSNX6_SerialDpmReadNonIntram( uint32_t ulDpmAddr )
{
  uint32_t ulData;
  uint8_t abTxData[12];
  uint8_t abRxBuffer[12];
  IRQSTATE irq;

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  uint32_t ulStatus;
#endif

  /* prepare transmit data */
  abTxData[0] = 0xA0 | ((ulDpmAddr >> 16) & 0xf);    /* set command=READ with dummy cycles, Address[19:16] */
  abTxData[1] = (ulDpmAddr >> 8) & 0xff;             /* set Address[15:8] */
  abTxData[2] = ulDpmAddr & 0xff;                    /* set Address[7:0] */
  abTxData[3] = 2 << 4;                              /* 2 idle start bytes (no idle inter bytes) */
  abTxData[4] = 4;                                   /* length in bytes */
  abTxData[5] = 0x00;                                /* idle start byte */
  abTxData[6] = 0x00;                                /* idle start byte */
  abTxData[7] = 0x00;                                /* dummy data because read transfer */
  abTxData[8] = 0x00;                                /* dummy data because read transfer */
  abTxData[9] = 0x00;                                /* dummy data because read transfer */
  abTxData[10] = 0x00;                               /* dummy data because read transfer */

  /* lock IRQ */
  irq = IRQ_LockSave();

  /* assert CS */
  NX51_SPIMOTION_SelectChip( 0 );

  /* do SPI data exchange */
  NX51_SPIMOTION_ExchangeData8( 11, abTxData, abRxBuffer );

  /* DE-assert CS */
  NX51_SPIMOTION_DeSelectChip( 0 );

  /* unlock IRQ */
  IRQ_LockRestore(irq);

  /* get received data bytes */
  ulData  = (abRxBuffer[7] & 0xff);
  ulData |= (abRxBuffer[8] & 0xff) << 8;
  ulData |= (abRxBuffer[9] & 0xff) << 16;
  ulData |= (abRxBuffer[10] & 0xff) << 24;

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  ulStatus = abRxBuffer[0];

  /* check DPM status */
  if( ulStatus != (MSK_NX51_dpm_status_unlocked
                  |(MSK_NX51_dpm_status_sel_dpm_serial>>3) /* sel_dpm_serial status re-mapped in case of sDPM byte */
                  )
    ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif

  return ulData;
}



/*****************************************************************************/
/*! ACCESSNX6 Read BYTE/WORD/DWORD via Serial DPM
* \description
*   This function reads a BYTE/WORD/DWORD via serial DPM interface.
* \class
*   ACCESSNX6
* \params
*   uSize        [in]  Length in bytes
*   ulDpmAddr    [in]  DPM address
* \return
*   Read data                                                                */
/*****************************************************************************/
uint32_t NX51_ACCESSNX6_SerialDpmRead( unsigned int uSize,
                                       uint32_t     ulDpmAddr )
{

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  /* check for DPM address violation */
  if( (ulDpmAddr + uSize) > NX6_DPM_OFFSET_FIN ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif

  if (ulDpmAddr >= NX6_DPM_OFFSET_INTRAM7)
  {
    return NX51_ACCESSNX6_SerialDpmReadIntram(uSize, ulDpmAddr);
  }
  else
  {
#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
    /* Only DWORD accesses are allowed */
    if( uSize != 4 ) {
      RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
      while(1);
    }
#endif
    return NX51_ACCESSNX6_SerialDpmReadNonIntram(ulDpmAddr);
  }

}

/*****************************************************************************/
/*! ACCESSNX6 Write BYTE/WORD/DWORD via Serial DPM
* \description
*   This function writes a BYTE/WORD/DWORD via serial DPM interface.
* \class
*   ACCESSNX6
* \params
*   uSize        [in]  Length in bytes
*   ulDpmAddr    [in]  Dpm address
*   ulVal        [in]  Write data
* \return
*                                                                            */
/*****************************************************************************/
void NX51_ACCESSNX6_SerialDpmWrite( unsigned int uSize,
                                    uint32_t     ulDpmAddr,
                                    uint32_t     ulVal )
{
  uint8_t abTxData[8];
  uint8_t abRxBuffer[8];
  IRQSTATE irq;

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  uint32_t ulStatus;

  /* check for DPM address violation */
  if( (ulDpmAddr + uSize) > NX6_DPM_OFFSET_FIN ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif

  /* prepare transmit data */
  abTxData[0] = (ulDpmAddr >> 16) & 0xf;            /* set command=WRITE, Address[19:16] */
  abTxData[1] = (ulDpmAddr >> 8) & 0xff;            /* set Address[15:8] */
  abTxData[2] = ulDpmAddr & 0xff;                   /* set Address[7:0] */
  abTxData[3] = ulVal&0xff;                         /* write data */
  abTxData[4] = (ulVal>>8)&0xff;                    /* write data */
  abTxData[5] = (ulVal>>16)&0xff;                   /* write data */
  abTxData[6] = (ulVal>>24)&0xff;                   /* write data */

  /* lock IRQ */
  irq = IRQ_LockSave();

  /* assert CS */
  NX51_SPIMOTION_SelectChip( 0 );

  /* do SPI data exchange */
  NX51_SPIMOTION_ExchangeData8( uSize+3, abTxData, abRxBuffer );

  /* DE-assert CS */
  NX51_SPIMOTION_DeSelectChip( 0 );

  /* unlock IRQ */
  IRQ_LockRestore(irq);

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  /* get received data bytes */
  ulStatus = abRxBuffer[0];

  /* check DPM status */
  if( ulStatus != (MSK_NX51_dpm_status_unlocked
                  |(MSK_NX51_dpm_status_sel_dpm_serial>>3) /* sel_dpm_serial status re-mapped in case of sDPM byte */
                  )
    ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif
}

/*****************************************************************************/
/*! ACCESSNX6 Byte-wise copy from netX6 memory to host memory via Serial DPM
* \description
*   This function copies byte-wise from netX6 memory to host memory via serial DPM interface.
*   Note: Byte-wise access only allowed to INTRAM of netX6.
* \class
*   ACCESSNX6
* \params
*   pvDst        [in]  Pointer to destination address
*   pvSrc        [in]  Pointer to source address
*   uSize        [in]  Length in bytes
* \return
*                                                                            */
/*****************************************************************************/
void NX51_ACCESSNX6_SerialDpmMemRead( void*        pvDst,
                                      const void   *pvSrc,
                                      unsigned int uSize )
{
  unsigned int uCnt, uBlockCnt;
  uint8_t* pbDst = (uint8_t*)pvDst;
  uint32_t ulDpmAddr = (uint32_t)pvSrc;
  uint8_t abTxData[16];
  uint8_t abRxBuffer[4];
  IRQSTATE irq;

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  uint32_t ulStatus;

  /* check for DPM address violation, only INTRAMs are byte-accessible */
  if(   (ulDpmAddr < NX6_DPM_OFFSET_INTRAM7)
      ||((ulDpmAddr + uSize) > NX6_DPM_OFFSET_FIN) ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif

  /* prepare transmit data */
  abTxData[0] = 0x80 | ((ulDpmAddr >> 16) & 0xf);    /* set command=READ, Address[19:16] */
  abTxData[1] = (ulDpmAddr >> 8) & 0xff;             /* set Address[15:8] */
  abTxData[2] = ulDpmAddr & 0xff;                    /* set Address[7:0] */
  abTxData[3] = 0;                                   /* exchange until chip DE-select */
  uCnt = 0;

  /* lock IRQ */
  irq = IRQ_LockSave();

  /* assert CS */
  NX51_SPIMOTION_SelectChip( 0 );

  /* do SPI data exchange (sDPM command header) */
  NX51_SPIMOTION_ExchangeData8( 4, abTxData, abRxBuffer );

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  /* latch sDPM status */
  ulStatus = abRxBuffer[0];
#endif

  /* do SPI data exchange (sDPM command data) */
  while( uCnt < uSize ) {
    uBlockCnt = ((uSize - uCnt) > 16 ) ? 16 : (uSize - uCnt);
    NX51_SPIMOTION_ExchangeData8( uBlockCnt, abTxData, &pbDst[uCnt] );
    uCnt += uBlockCnt;
  }

  /* DE-assert CS */
  NX51_SPIMOTION_DeSelectChip( 0 );

  /* unlock IRQ */
  IRQ_LockRestore(irq);

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  /* check sDPM status */
  if( ulStatus != (MSK_NX51_dpm_status_unlocked
                  |(MSK_NX51_dpm_status_sel_dpm_serial>>3) /* sel_dpm_serial status re-mapped in case of sDPM byte */
                  )
    ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif
}

/*****************************************************************************/
/*! ACCESSNX6 Byte-wise copy from host memory to netx6 memory via Serial DPM
* \description
*   This function copies byte-wise from host memory to netx6 memory via serial DPM interface.
*   Note: Byte-wise access only allowed to INTRAM of netX6.
* \class
*   ACCESSNX6
* \params
*   pvDst        [in]  Pointer to destination address
*   pvSrc        [in]  Pointer to source address
*   uSize        [in]  Length in bytes
* \return
*                                                                            */
/*****************************************************************************/
void NX51_ACCESSNX6_SerialDpmMemWrite( void*       pvDst,
                                       const void  *pvSrc,
                                       unsigned int uSize )
{
  unsigned int uCnt, uBlockCnt;
  uint8_t* pbSrc = (uint8_t*)pvSrc;
  uint32_t ulDpmAddr = (uint32_t)pvDst;
  uint8_t abTxData[3];
  uint8_t abRxBuffer[16];
  IRQSTATE irq;

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  uint32_t ulStatus;

  /* check for DPM address violation, only INTRAMs are byte-accessible */
  if(   (ulDpmAddr < NX6_DPM_OFFSET_INTRAM7)
      ||((ulDpmAddr + uSize) > NX6_DPM_OFFSET_FIN) ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif

  /* prepare transmit data */
  abTxData[0] = (ulDpmAddr >> 16) & 0xf;             /* set command=WRITE, Address[19:16] */
  abTxData[1] = (ulDpmAddr >> 8) & 0xff;             /* set Address[15:8] */
  abTxData[2] = ulDpmAddr & 0xff;                    /* set Address[7:0] */
  uCnt = 0;

  /* lock IRQ */
  irq = IRQ_LockSave();

  /* assert CS */
  NX51_SPIMOTION_SelectChip( 0 );

  /* do SPI data exchange (sDPM command header) */
  NX51_SPIMOTION_ExchangeData8( 3, abTxData, abRxBuffer );

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  /* latch sDPM status */
  ulStatus = abRxBuffer[0];
#endif

  /* do SPI data exchange (sDPM command data) */
  while( uCnt < uSize ) {
    uBlockCnt = ((uSize - uCnt) > 16 ) ? 16 : (uSize - uCnt);
    NX51_SPIMOTION_ExchangeData8( uBlockCnt, &pbSrc[uCnt], abRxBuffer );
    uCnt += uBlockCnt;
  }

  /* DE-assert CS */
  NX51_SPIMOTION_DeSelectChip( 0 );

  /* unlock IRQ */
  IRQ_LockRestore(irq);

#ifndef __NX51_ACCESSNX6_SDPM_DISABLE_CHECKS__
  /* check sDPM status */
  if( ulStatus != (MSK_NX51_dpm_status_unlocked
                  |(MSK_NX51_dpm_status_sel_dpm_serial>>3) /* sel_dpm_serial status re-mapped in case of sDPM byte */
                  )
    ) {
    RDYRUN_SetRdyRunLed( RDYRUN_LED_RED );
    while(1);
  }
#endif
}
