/*****************************************************************************/
#include "hal_resources_defines_netx5.h"
__USE_XPEC_REGS
__USE_XPEC_PRAM
__USE_XPEC_DRAM
__USE_XPEC_IRQS
__USE_XMAC
__USE_MIIMU
__USE_SYSTIME
__USE_POINTERFIFO
__USE_ASICCTRL
__USE_PIO
__USE_DPM
__USE_ZPU
__USE_INTRAM
/*****************************************************************************/


/*
  ____    ___    ___  
 |  _ \  |_ _|  / _ \ 
 | |_) |  | |  | | | |
 |  __/   | |  | |_| |
 |_|     |___|  \___/ 
                      
*/

/*****************************************************************************/
/*! Set PIO Mode
* \description
*   Set the mode for the selected PIO.
* \class 
*   PIO
* \params
*   ulPioNum  [in] Selected PIO
*   ulMode    [in] PIO_MODE_IN/PIO_MODE_OUT  = Set the PIO to input/output mode
* \return
*                                                                            */
/*****************************************************************************/
void NX5_PIO_SetMode( unsigned long ulPioNum, unsigned long ulMode )
{
  unsigned long  ulTmp;
  if( NX5_PIO_MODE_OUT == ulMode ) {
    ulTmp = NX_READ32(s_ptPio->ulPio_oe);
    ulTmp |= 1U << ulPioNum;
    NX_WRITE32(s_ptPio->ulPio_oe, ulTmp);
  } else {
    ulTmp = NX_READ32(s_ptPio->ulPio_oe);
    ulTmp &= ~(1U << ulPioNum);
    NX_WRITE32(s_ptPio->ulPio_oe, ulTmp);
  }
}

/*****************************************************************************/
/*! PIO Get Output
* \description
*   Read current value of selected PIO within PIO out register.
* \class 
*   PIO 
* \params
*   ulPioNum          [in]  Selected PIO
* \return
*    0
*    1                                                                       */
/*****************************************************************************/
int NX5_PIO_GetOutput( unsigned long ulPioNum )
{
  return ( NX_READ32(s_ptPio->ulPio_out) & (1U << ulPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! PIO Set Output
* \description
*   Enable/Disable the output for the selected PIO.
* \class 
*   PIO
* \params
*   ulPioNum  [in]   Select the PIO
*   fEnable   [in]   0 = Disable the Output, 1 = Enable the Output  
* \return
*                                                                            */
/*****************************************************************************/
void NX5_PIO_SetOutput( unsigned long ulPioNum, int fEnable )
{
  unsigned long  ulTmp;
  if( fEnable ) {
    ulTmp = NX_READ32(s_ptPio->ulPio_out);
    ulTmp |= 1U << ulPioNum;
    NX_WRITE32(s_ptPio->ulPio_out, ulTmp);
  } else {
    ulTmp = NX_READ32(s_ptPio->ulPio_out);
    ulTmp &= ~(1U << ulPioNum);
    NX_WRITE32(s_ptPio->ulPio_out, ulTmp);
  }
}

/*****************************************************************************/
/*! PIO Toggle Output
* \description
*   Toggle the output for the selected PIO.
* \class 
*   PIO
* \params
*   ulPioNum  [in]   Select the PIO
* \return
*                                                                            */
/*****************************************************************************/
void NX5_PIO_ToggleOutput( unsigned long ulPioNum )
{
  unsigned long  ulTmp;
  ulTmp = NX_READ32(s_ptPio->ulPio_out);
  ulTmp ^= 1U << ulPioNum;
  NX_WRITE32(s_ptPio->ulPio_out, ulTmp);
}

/*****************************************************************************/
/*! Get PIO Input
* \description
*   Read the current value of the selected PIO.
* \class 
*   PIO
* \params
*   ulPioNum  [in]   Select the PIO
* \return
*                                                                            */
/*****************************************************************************/
int NX5_PIO_GetInput( unsigned long ulPioNum )
{
  return ( NX_READ32(s_ptPio->ulPio_in) & (1U << ulPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! Get PIO In
* \description
*   Read the current value of the PIO In register.
* \class 
*   PIO
* \params
* \return
*   current PIO In value                                                     */
/*****************************************************************************/
int NX5_PIO_GetIn( void )
{
  return ( NX_READ32(s_ptPio->ulPio_in) & 0xffff );
}

/*
  ____    ____    __  __ 
 |  _ \  |  _ \  |  \/  |
 | | | | | |_) | | |\/| |
 | |_| | |  __/  | |  | |
 |____/  |_|     |_|  |_|
                         
*/

#ifdef NX5_PARALLEL_DPM

/*****************************************************************************/
/*! Inilize DPM
* \description
*   Initialize the DPM.
* \class 
*   DPM 
* \params
*   iCnt                [out] Counter
*   tSize               [in]  DPM Size
*   uRdAddrSetupTime    [in]  Address Setup Time
*   fReadyEn            [in]  Ready enabled (1) or disabled (0)
* \return
*   0 on success
*   -1 on erroneous 
*                                                                            */
/*****************************************************************************/
int NX5_DPM_InitDpm( int iCnt, NX5_DPM_SIZE_E tSize, unsigned int uRdAddrSetupTime, int fReadyEn )
{
  unsigned long ulValue;
  unsigned char ucCfg0x0 = 0;
  volatile unsigned char* pbBytePtr;
  unsigned char bRdyCfg;
  int iRslt = 0;
  
  switch( tSize ) {
    case NX5_DPM_SIZE_08:
      ucCfg0x0 = 0x00;
      break;
    case NX5_DPM_SIZE_16:
      ucCfg0x0 = 0x05;
      break;
    case NX5_DPM_SIZE_32:
      ucCfg0x0 = 0x08;
      break;
    default:
      /* invalid DPM size */
      iRslt = -1;
      break;
  }

  if( iRslt != 0 )
    return iRslt;

  bRdyCfg = (fReadyEn) ? 0x03 : 0x00;  /* if enabled RDY=PushPull, Polarity=1 (High), RDY is generated as Wait/Busy state signal */

  /* get byte pointer to DPM base */
  pbBytePtr = (unsigned char*) s_ptDpm;
  
  /* try to configure DPM several times */
  do { 
    /* write configuration (DPM = 8 bit) */
    pbBytePtr[0x18] = bRdyCfg; 
    pbBytePtr[0xa0] = 0x00; /* enable RDY pin, leave SIRQ and DIRQ as IRQ pins */
    pbBytePtr[0x10] = 0x08; /* address area=128kB */
    
    /* read back configuration */
    ulValue  = (pbBytePtr[0x18] ^ bRdyCfg);
    ulValue |= (pbBytePtr[0xa0] ^ 0x00);
    ulValue |= (pbBytePtr[0x10] ^ 0x08);
    
    /* check for configuration successful */
    if( ulValue != 0 ) {
      --iCnt;
      if( iCnt == 0 ) break;
    }
  } while( ulValue!=0 );
    
  /* Configuration successful? */
  if( iCnt==0 ) { 
    /* DPM access failed */
    return -1;
  }

  /* DPM access successful */

  /* configure netX5 DPM with selected size */
  pbBytePtr[0x00] = ucCfg0x0;

  /* configure t_osa */
  if (fReadyEn) { pbBytePtr[0x14] = 0x10 | uRdAddrSetupTime; }
  else { pbBytePtr[0x14] = 0x30 | uRdAddrSetupTime; }
    
  /* check for configuration successful */
  if( pbBytePtr[0x00]!=ucCfg0x0 ) {
    return -1;
  }

  /* All okay */
  return iRslt;
}
#endif /* NX5_PARALLEL_DPM */


#ifdef NX5_SERIAL_DPM

const unsigned char _binary_zpu_code_bin_start[] = {
  0x8c, 0x08, 0x02, 0x8c, 0x0c, 0xaf, 0xff, 0x8c, 0x0b, 0x08, 0x80, 0x0b, 0x2f, 0x81, 0x2b, 0xaf,
  0xff, 0x90, 0x0b, 0x08, 0x80, 0x0b, 0x2f, 0x07, 0x83, 0x32, 0x9a, 0x2b, 0xaf, 0xff, 0xb4, 0x0b,
  0x0c, 0xa1, 0x0b, 0x04, 0x0b, 0x0b, 0x0b, 0x0b, 0x8c, 0x0c, 0x04
};
const size_t _binary_zpu_code_bin_size = 43;

#define ZPU_PRG_OFFSET     0x001c
#define ZPU_XOR_OFFSET     0x0034
#define ZPU_STACK_OFFSET   0x0180

/*****************************************************************************/
/*! Initialize Serial DPM
* \description
*   Initialize the serial DPM.
* \class 
*   DPM 
* \params
*   fIrqPol     [in]    IRQ polarity (0: low active: 1: high active)
* \return
*   0 on success
*   -1 on erroneous 
*                                                                            */
/*****************************************************************************/
int NX5_DPM_InitSerDpm( int fIrqPol )
{
  const unsigned char* pucSrcCnt;
  const unsigned char* pucSrcEnd;
  unsigned long  ulAddrDstCnt;
  unsigned long  ulValIn, ulValOut;

  /* set IRQ pins to PIOs */
  NX_WRITE8_SR(REL_Adr_NX5_dpm_io_cfg_misc,
                                MSK_NX5_dpm_io_cfg_misc_sel_rdy_pio |
                                MSK_NX5_dpm_io_cfg_misc_sel_dirq_pio |
                                MSK_NX5_dpm_io_cfg_misc_sel_sirq_pio);

  /* select 128 kB address range, from now on all accesses must be done 
     with methods for full address range! */
  NX_WRITE8_SR(REL_Adr_NX5_dpm_addr_cfg, 0x08);

  if (fIrqPol == 0)
  {
    /* bring IRQ PIOs to idle polarity high */
    NX_WRITE32(REL_Adr_NX5_dpm_pio_out1,
                    MSK_NX5_dpm_pio_out1_dpm_sirq |
                    MSK_NX5_dpm_pio_out1_dpm_dirq);
  }
  else
  {
    /* bring IRQ PIOs to idle polarity low */
    NX_WRITE32(REL_Adr_NX5_dpm_pio_out1, 0);
  }
                  
  NX_WRITE32(REL_Adr_NX5_dpm_pio_oe1,
                  MSK_NX5_dpm_pio_oe1_dpm_sirq |
                  MSK_NX5_dpm_pio_oe1_dpm_dirq);

  /* load ZPU code for IRQ generation, program is placed at beginning of intram0 */
  pucSrcCnt = _binary_zpu_code_bin_start;
  pucSrcEnd = _binary_zpu_code_bin_start + _binary_zpu_code_bin_size;
  ulAddrDstCnt = NX5_DPM_OFFSET_INTRAM0+ZPU_PRG_OFFSET;

  /* copy code */
  while( pucSrcCnt<pucSrcEnd )
  {
    ulValOut  = (*(pucSrcCnt++)) << 24;
    ulValOut |= (*(pucSrcCnt++)) << 16;
    ulValOut |= (*(pucSrcCnt++)) <<  8;
    ulValOut |=  *(pucSrcCnt++);
    NX_WRITE32(ulAddrDstCnt, ulValOut);
    ulAddrDstCnt += 4;
  }
  
  /* compare code */
  pucSrcCnt = _binary_zpu_code_bin_start;
  ulAddrDstCnt = NX5_DPM_OFFSET_INTRAM0+ZPU_PRG_OFFSET;
  while( pucSrcCnt<pucSrcEnd )
  {
    ulValIn = NX_READ32(ulAddrDstCnt);
    ulValOut  = (*(pucSrcCnt++)) << 24;
    ulValOut |= (*(pucSrcCnt++)) << 16;
    ulValOut |= (*(pucSrcCnt++)) <<  8;
    ulValOut |=  *(pucSrcCnt++);
    if (ulValOut != ulValIn)
    {
      return -1;
    }
    ulAddrDstCnt += 4;
  }
  
  /* modify polarity of output signal (patch ZPU code) */
  if (fIrqPol == 0)
  {
    NX_WRITE32(NX5_DPM_OFFSET_INTRAM0+ZPU_XOR_OFFSET, 0x83329a2b);
  }
  else
  {
    NX_WRITE32(NX5_DPM_OFFSET_INTRAM0+ZPU_XOR_OFFSET, 0x80329a2b);
  }

  /* start ZPU */
  NX_WRITE32(NX5_DPM_OFFSET_INTRAM1 + 0x7ff8, ZPU_STACK_OFFSET);  /* SP */
  NX_WRITE32(NX5_DPM_OFFSET_INTRAM1 + 0x7ffc, ZPU_PRG_OFFSET);  /* PC */

  /* enable the counter irq */
  NX_WRITE32(NX5_DPM_OFFSET_ZPU + REL_Adr_NX5_zpu_irq_mask_set, MSK_NX5_zpu_irq_mask_set_zpu_counter);
  NX_WRITE32(NX5_DPM_OFFSET_ZPU + REL_Adr_NX5_zpu_counter, 1);

  return 0;
}
#endif /* NX5_SERIAL_DPM */



/*****************************************************************************/
/*! Check Netx DPM
* \description
*   Check netx version and whether DPM is detected.\n
* \class 
*   DPM 
* \params
*
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_DPM_ChkDpm( void )
{
  /* check for netx version (mask out the license bits) */
  if( (NX_READ32(s_ptDpm->ulDpm_netx_version) & 0x000000ff) != 0x00000041 ) return -1;

  /* check dpm_win5_end register, it is fixed to 0x0001ff00 */
  if( NX_READ32(s_ptDpm->ulDpm_win5_end) != 0x0001ff00 ) return -1;
  
  /* ok, netx5 DPM detected! */
  return 0;
}

/*****************************************************************************/
/*! Setup Byte Access
* \description
*   Enable/Disable byte access within the selected DPM window.
* \class 
*   DPM 
* \params
*   iWindow               [in]  Number of DPM Window
*   fByteAccess           [in]  Byte Access Value
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_SetupByteAccess( int iWindow, int fByteAccess )
{
  unsigned long ulTmp;
  switch( iWindow ) {
    case 1:
      ulTmp = NX_READ32(s_ptDpm->ulDpm_win1_map);
      if( fByteAccess ) ulTmp |= MSK_NX5_dpm_win1_map_byte_area;
      else ulTmp &= ~MSK_NX5_dpm_win1_map_byte_area;
      NX_WRITE32(s_ptDpm->ulDpm_win1_map, ulTmp);
    break;
  
    case 2:
      ulTmp = NX_READ32(s_ptDpm->ulDpm_win2_map);
      if( fByteAccess ) ulTmp |= MSK_NX5_dpm_win2_map_byte_area;
      else ulTmp &= ~MSK_NX5_dpm_win2_map_byte_area;
      NX_WRITE32(s_ptDpm->ulDpm_win2_map, ulTmp);
    break;

    case 3:
      ulTmp = NX_READ32(s_ptDpm->ulDpm_win3_map);
      if( fByteAccess ) ulTmp |= MSK_NX5_dpm_win3_map_byte_area;
      else ulTmp &= ~MSK_NX5_dpm_win3_map_byte_area;
      NX_WRITE32(s_ptDpm->ulDpm_win3_map, ulTmp);
    break;

    case 4:
      ulTmp = NX_READ32(s_ptDpm->ulDpm_win4_map);
      if( fByteAccess ) ulTmp |= MSK_NX5_dpm_win4_map_byte_area;
      else ulTmp &= ~MSK_NX5_dpm_win4_map_byte_area;
      NX_WRITE32(s_ptDpm->ulDpm_win4_map, ulTmp);
    break;

    case 5:
      ulTmp = NX_READ32(s_ptDpm->ulDpm_win5_map);
      if( fByteAccess ) ulTmp |= MSK_NX5_dpm_win5_map_byte_area;
      else ulTmp &= ~MSK_NX5_dpm_win5_map_byte_area;
      NX_WRITE32(s_ptDpm->ulDpm_win5_map, ulTmp);
    break;

    default: /* do nothing */
    break;
  }
}

/*****************************************************************************/
/*! Setup IRQ
* \description
*   Set Pad Control and FIQ/IRQ mask.
* \class 
*   DPM 
* \params
*   ulIrqMsk             [in]  IRQ Mask
*   ulFiqMsk             [in]  FIQ Mask
*   ulPadCtrl            [in]  Pad Control
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_SetupIrq( unsigned long ulIrqMsk, unsigned long ulFiqMsk, unsigned long ulPadCtrl )
{
  /* set FIQ mask */
  NX_WRITE32(s_ptDpm->ulDpm_irq_fiq_mask, ulFiqMsk);
  
  /* set IRQ mask */
  NX_WRITE32(s_ptDpm->ulDpm_irq_irq_mask, ulIrqMsk);
  
  /* set PAD control */
  NX_WRITE32(s_ptDpm->ulDpm_fiq_irq_pad_ctrl, ulPadCtrl);
}

/*****************************************************************************/
/*! Get FIQ
* \description
*   Get  FIQ events.
* \class 
*   DPM 
* \params
*   pulFiq    [out] Pointer of FIQ value
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_GetFiq( unsigned long *pulFiq )
{
  /* get fiq events */
  *pulFiq = NX_READ32(s_ptDpm->ulDpm_irq_fiq_masked);
}

/*****************************************************************************/
/*! Get IRQ
* \description
*   Get  IRQ events.
* \class 
*   DPM 
* \params
*   pulIrq    [out] Pointer of IRQ value
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_GetIrq( unsigned long *pulIrq )
{
  /* get irq events */
  *pulIrq = NX_READ32(s_ptDpm->ulDpm_irq_irq_masked);
}

/*****************************************************************************/
/*! Get Error
* \description
*   Get DPM Error and Error Address.
* \class 
*   DPM 
* \params
*   pulErr        [out] Pointer to Error vector
*   pulErrAddr    [out] Pointer to Error Address
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_GetErr( unsigned long *pulErr, unsigned long *pulErrAddr )
{
  /* get status and error address status */
  *pulErrAddr = NX_READ32(s_ptDpm->ulDpm_status_err_addr);
  *pulErr     =  (   MSK_NX5_dpm_status_rd_err 
                   | MSK_NX5_dpm_status_wr_err
                   | MSK_NX5_dpm_status_rdy_to_err
                   | MSK_NX5_dpm_status_bus_conflict_err
                 ) & NX_READ32(s_ptDpm->ulDpm_status);
}

/*****************************************************************************/
/*! Get Error
* \description
*   Get DPM Error and Error Address.
* \class 
*   DPM 
* \params
*   pulErr        [out] Pointer to Error vector
*   pulErrAddr    [out] Pointer to Error Address
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_ClrErr( void )
{
  /* clear DPM error due reading status_err_auto_reset register */
  NX_READ32(s_ptDpm->ulDpm_status_err_auto_reset);
}

/*****************************************************************************/
/*! Get and Clear Error
* \description
*   Get DPM Error and Error Address and clear DPM Error afterwards.
* \class 
*   DPM 
* \params
*   pulErr        [out] Pointer to Error vector
*   pulErrAddr    [out] Pointer to Error Address
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_GetAndClrErr( unsigned long *pulErr, unsigned long *pulErrAddr )
{
  /* get status and error address status */
  *pulErrAddr = NX_READ32(s_ptDpm->ulDpm_status_err_addr);
  *pulErr     =  (   MSK_NX5_dpm_status_rd_err 
                   | MSK_NX5_dpm_status_wr_err
                   | MSK_NX5_dpm_status_rdy_to_err
                   | MSK_NX5_dpm_status_bus_conflict_err
                 ) & NX_READ32(s_ptDpm->ulDpm_status_err_auto_reset);
}

/*****************************************************************************/
/*! Set PIO 31..28 Mode 
* \description
*   Set the mode of the DPM PIOs 31..28
* \class 
*   DPM
* \params
*   ulPioOe31_28  [in] output enable value for PIOs 31..28
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_SetPioOe31_28( unsigned long ulPioOe31_28 )
{
  unsigned long ulVal;
  
  ulVal = NX_READ32(s_ptDpm->aulDpm_pio_oe[1]) & (~0x000f0000);
  NX_WRITE32(s_ptDpm->aulDpm_pio_oe[1], ulVal | ((ulPioOe31_28&0xf)<<16));
}

/*****************************************************************************/
/*! Set PIO 31..28 Output
* \description
*   Set the output of the DPM PIOs 31..28
* \class 
*   DPM
* \params
*   ulPioOut31_28  [in] output enable value for PIOs 31..28
* \return
*                                                                            */
/*****************************************************************************/
void NX5_DPM_SetPioOut31_28( unsigned long ulPioOut31_28 )
{
  unsigned long ulVal;
  
  ulVal = NX_READ32(s_ptDpm->aulDpm_pio_out[1]) & (~0x000f0000);
  NX_WRITE32(s_ptDpm->aulDpm_pio_out[1], ulVal | ((ulPioOut31_28&0xf)<<16));
}

/*****************************************************************************/
/*! Wait DPM Unlocked
* \description
*   Waits until DPM is unlocked and stops all AHB masters of netX 5.
* \class 
*   DPM 
* \params
*
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
unsigned int NX5_DPM_WaitUnlocked( void )
{
  unsigned int uWaitTime;
  unsigned long ulValue;
  
  uWaitTime=0;
  while( (NX_READ32(s_ptDpm->ulDpm_status) & MSK_NX5_dpm_status_unlocked) == 0 ) {
    uWaitTime++;
  }

  // stop DMAC of xPEC 0
  ulValue = NX_READ32(s_ptXpecStart[0]->ulXpec_statcfg);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  NX_WRITE32(s_ptXpecStart[0]->ulXpec_statcfg, 0);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  ulValue = NX_READ32(s_ptXpecStart[0]->ulXpec_statcfg);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM

  // stop DMAC of xPEC 1
  ulValue = NX_READ32(s_ptXpecStart[1]->ulXpec_statcfg);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  NX_WRITE32(s_ptXpecStart[1]->ulXpec_statcfg, 0);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  ulValue = NX_READ32(s_ptXpecStart[1]->ulXpec_statcfg);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM

  // stop ZPU
  ulValue = NX_READ32(s_ptZpu->ulZpu_ctrl);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  NX_WRITE32(s_ptZpu->ulZpu_ctrl, 0x2);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  ulValue = NX_READ32(s_ptZpu->ulZpu_ctrl);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM

  // stop xPEC 0
  ulValue = NX_READ32(s_ptXpecStart[0]->ulXpu_hold_pc);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  NX_WRITE32(s_ptXpecStart[0]->ulXpu_hold_pc, MSK_NX5_xpu_hold_pc_hold);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  ulValue = NX_READ32(s_ptXpecStart[0]->ulXpu_hold_pc);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM

  // stop xPEC 1
  ulValue = NX_READ32(s_ptXpecStart[1]->ulXpu_hold_pc);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  NX_WRITE32(s_ptXpecStart[1]->ulXpu_hold_pc, MSK_NX5_xpu_hold_pc_hold);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM
  ulValue = NX_READ32(s_ptXpecStart[1]->ulXpu_hold_pc);
  ulValue = NX_READ32(s_pulIntRamStart[0][0]); // read from INTRAM

  // clear xPEC 0 DRAM
  ulValue=0x800;
  while( ulValue!=0 ) {
    ulValue--;
    NX_WRITE32_XPEC(s_pulXpecDramStart[0][ulValue], 0x0); // clear DMA address
    ulValue--;
    NX_WRITE32_XPEC(s_pulXpecDramStart[0][ulValue], 0x73); // link and stop DMA
  }

  // clear xPEC 1 DRAM
  ulValue=0x800;
  while( ulValue!=0 ) {
    ulValue--;
    NX_WRITE32_XPEC(s_pulXpecDramStart[1][ulValue], 0x0); // clear DMA address
    ulValue--;
    NX_WRITE32_XPEC(s_pulXpecDramStart[1][ulValue], 0x73); // link and stop DMA
  }

  return uWaitTime;
}

/*
  _____          _                                   _           ____    _   _  __   __
 | ____| __  __ | |_    ___   _ __   _ __     __ _  | |         |  _ \  | | | | \ \ / /
 |  _|   \ \/ / | __|  / _ \ | '__| | '_ \   / _` | | |         | |_) | | |_| |  \ V / 
 | |___   >  <  | |_  |  __/ | |    | | | | | (_| | | |         |  __/  |  _  |   | |  
 |_____| /_/\_\  \__|  \___| |_|    |_| |_|  \__,_| |_|  _____  |_|     |_| |_|   |_|  
                                                        |_____|                        
*/

/*****************************************************************************/
/*! Enable PHY Clock
* \description
*   Enables PHY clock for external PHY(s).
* \class 
*   EXTPHY 
* \params
*   fPhyClkOutOe               [in]  PHY Clock Outenable
*   fDerivePhyClkOutFromClk    [in]  Derive PHY Clock Out From Clock
* \return
*                                                                            */
/*****************************************************************************/
void NX5_EXTPHY_EnablePhyClk( int fPhyClkOutOe, int fDerivePhyClkOutFromClk )
{
  unsigned long ulVal;

  /* get current value */
  ulVal = NX_READ32(s_ptAsicCtrl->ulIo_config);
  
  if( fPhyClkOutOe ) {
    ulVal |= MSK_NX5_io_config_phy_clk_out_oe;
  } else {
    ulVal &= ~MSK_NX5_io_config_phy_clk_out_oe;
  }
  
  if( fDerivePhyClkOutFromClk ) {
    ulVal |= MSK_NX5_io_config_sel_phy_clk_digital;
  } else {
    ulVal &= ~MSK_NX5_io_config_sel_phy_clk_digital;
  }
  
  /* get access key */
  NX_WRITE32(s_ptAsicCtrl->ulAsic_ctrl_access_key,
                NX_READ32(s_ptAsicCtrl->ulAsic_ctrl_access_key));
  
  /* write new value */
  NX_WRITE32(s_ptAsicCtrl->ulIo_config, ulVal);

}

/*****************************************************************************/
/*! Reset PHY
* \description
*   Assert/deassert phy_reset.
* \class 
*   EXTPHY 
* \params
*   fEnable               [in]  Enable PHY Reset
* \return
*                                                                            */
/*****************************************************************************/
void NX5_EXTPHY_SetPhyReset( int fEnable )
{
  if( fEnable ) {
    /* assert phy_reset */
    NX_WRITE32(s_ptDpm->aulDpm_pio_out[0], 0x0);
    NX_WRITE32(s_ptDpm->aulDpm_pio_oe[0], 0x80000);
  } else {
    /* deassert phy_reset */
    NX_WRITE32(s_ptDpm->aulDpm_pio_out[0], 0x80000);
    NX_WRITE32(s_ptDpm->aulDpm_pio_oe[0], 0x80000);
  }
}


/*****************************************************************************/
/*! Initialize External PHY
* \description
*   Initialize an over MIIMU connected PHY.
* \class 
*   EXTPHY 
* \params
*   uPhyAddr               [in] PHY Address
*   ePhyMode               [in] PHY Mode
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_EXTPHY_Init( unsigned int uPhyAddr, NX5_PHY_MODE_E ePhyMode )
{
  unsigned short usMiiData;
  int fAutoMdixEn = 0;
  int iRslt = 0;

  if( uPhyAddr > 31 ) return -1; /* invalid phy register number */

  /* MDIO bug: 1st MDIO read fails */
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_MULTIPLE, &usMiiData, NULL);

  /* get PHY out of standby power mode */
  if( PHY_MODE_POWER_DOWN_MODE != ePhyMode ) { 
    /* read Standby Power Mode, switch to shadow register area */
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_BCM_TEST, &usMiiData, NULL);
    usMiiData |= BCM5241_BCM_TEST_SHADOW_REG_EN;
    NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_BCM_TEST, usMiiData, NULL);
    
    /* support auto-MDIX when auto-negotiation is disabled */
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_SHADOW_MISC_CTRL, &usMiiData, NULL);
    usMiiData |= BCM5241_SHADOW_MISC_CTRL_FORCED_AUTO_MDIX_EN;
    NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_SHADOW_MISC_CTRL, usMiiData, NULL);

    /* clear standby power flag */
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_SHADOW_MODE_4, &usMiiData, NULL);
    usMiiData &= ~BCM5241_SHADOW_MODE_4_STANDBY_PWR;
    NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_SHADOW_MODE_4, usMiiData, NULL);
    
    /* switch back to standard register area */
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_BCM_TEST, &usMiiData, NULL);
    usMiiData &= ~BCM5241_BCM_TEST_SHADOW_REG_EN;
    NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_BCM_TEST, usMiiData, NULL);

    /* disable forcing of ACT and LNK LEDs */
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_MODE, &usMiiData, NULL);
    usMiiData &= ~(BCM5241_AUX_MODE_FORCE_ACT_LED_OFF | BCM5241_AUX_MODE_FORCE_LNK_LED_OFF);
    NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_AUXILIARY_MODE, usMiiData, NULL);
  }

  /* get PHY out of super isolate mode */
  if( PHY_MODE_SUPER_ISOLATE != ePhyMode) { 
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_MULTIPLE, &usMiiData, NULL);
    usMiiData &= ~BCM5241_AUX_MUL_SUPER_ISOLATE;
    NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_AUXILIARY_MULTIPLE, usMiiData, NULL);
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_MULTIPLE, &usMiiData, NULL);
  }
  
  switch( ePhyMode ) {
  
    case PHY_MODE_SUPER_ISOLATE:
      /* enable super isolate mode */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_MULTIPLE, &usMiiData, NULL);
      usMiiData |= BCM5241_AUX_MUL_SUPER_ISOLATE;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_AUXILIARY_MULTIPLE, usMiiData, NULL);
    break;

    case PHY_MODE_10BASE_T_HD_NOAUTONEG_AUTOMDIXDIS:
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_CONTROL, usMiiData, NULL);
    break;
    
    case PHY_MODE_10BASE_T_FD_NOAUTONEG_AUTOMDIXDIS:
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      usMiiData |= BCM5241_CONTROL_FULL_DUPLEX;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_CONTROL, usMiiData, NULL);
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
    break;
    
    case PHY_MODE_100BASE_TX_HD_NOAUTONEG_AUTOMDIXDIS:
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      usMiiData |= BCM5241_CONTROL_SPEED_SELECT_100;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_CONTROL, usMiiData, NULL);
    break;
    
    case PHY_MODE_100BASE_TX_FD_NOAUTONEG_AUTOMDIXDIS:
      /* set PHY mode */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      usMiiData |= BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_FULL_DUPLEX;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_CONTROL, usMiiData, NULL);
    break;
    
    case PHY_MODE_POWER_DOWN_MODE:
      /* read Standby Power Mode, switch to shadow register area */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_BCM_TEST, &usMiiData, NULL);
      usMiiData |= BCM5241_BCM_TEST_SHADOW_REG_EN;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_BCM_TEST, usMiiData, NULL);
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_BCM_TEST, &usMiiData, NULL);
      
      /* set standby power flag */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_SHADOW_MODE_4, &usMiiData, NULL);
      usMiiData |= BCM5241_SHADOW_MODE_4_STANDBY_PWR;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_SHADOW_MODE_4, usMiiData, NULL);
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_SHADOW_MODE_4, &usMiiData, NULL);
      
      /* switch back to standard register area */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_BCM_TEST, &usMiiData, NULL);
      usMiiData &= ~BCM5241_BCM_TEST_SHADOW_REG_EN;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_BCM_TEST, usMiiData, NULL);
    
      /* force LINK and ACTIVITY TO OFF */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_MODE, &usMiiData, NULL);
      usMiiData |= (BCM5241_AUX_MODE_FORCE_ACT_LED_OFF | BCM5241_AUX_MODE_FORCE_LNK_LED_OFF);
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_AUXILIARY_MODE, usMiiData, NULL);

    break;
    
    case PHY_MODE_AUTONEG_AUTOMDIXDIS:
      /* set PHY mode */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      usMiiData |= BCM5241_CONTROL_AUTO_NEG_ENABLE;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_CONTROL, usMiiData, NULL);
    break;
    
    case PHY_MODE_AUTONEG_AUTOMDIXEN:
      /* enable AUTOMDIX */
      fAutoMdixEn = 1;
      
      /* set PHY mode */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      usMiiData |= BCM5241_CONTROL_AUTO_NEG_ENABLE;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_CONTROL, usMiiData, NULL);
    break;

    case PHY_MODE_AUTONEG_AUTOMDIXEN_100BASE_TX_FD_ONLY:
      /* enable AUTOMDIX */
      fAutoMdixEn = 1;

      /* set PHY capability to 100BASE-TX FD only */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUTO_NEG_ADVERTISEMENT, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_ADV_100_BASE_TX | BCM5241_ADV_10_BASE_T_FDX | BCM5241_ADV_10_BASE_T);
      usMiiData |= BCM5241_ADV_100_BASE_TX_FDX;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_AUTO_NEG_ADVERTISEMENT, usMiiData, NULL);

      /* set PHY mode */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      usMiiData |= BCM5241_CONTROL_AUTO_NEG_ENABLE;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_CONTROL, usMiiData, NULL);
    break;

    case PHY_MODE_AUTONEG_AUTOMDIXEN_100BASE_TX_HD_ONLY:
      /* enable AUTOMDIX */
      fAutoMdixEn = 1;

      /* set PHY capability to 100BASE-TX HD only */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUTO_NEG_ADVERTISEMENT, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_ADV_100_BASE_TX_FDX | BCM5241_ADV_10_BASE_T_FDX | BCM5241_ADV_10_BASE_T);
      usMiiData |= BCM5241_ADV_100_BASE_TX;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_AUTO_NEG_ADVERTISEMENT, usMiiData, NULL);

      /* set PHY mode */
      NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);
      usMiiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      usMiiData |= BCM5241_CONTROL_AUTO_NEG_ENABLE;
      NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_CONTROL, usMiiData, NULL);
    break;
    
    default:
      iRslt = -1;
    break;
  }

  if(fAutoMdixEn)
  {
    /* enable AUTOMDIX */
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_ERR_GEN_STATUS, &usMiiData, NULL);
    usMiiData &= ~BCM5241_AUX_ERR_HP_AUTOMDIXDIS;
    NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_AUXILIARY_ERR_GEN_STATUS, usMiiData, NULL);
  }
  else
  {
    /* disable HP-AUTOMDIX */
    NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_ERR_GEN_STATUS, &usMiiData, NULL);
    usMiiData |= BCM5241_AUX_ERR_HP_AUTOMDIXDIS;
    NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_AUXILIARY_ERR_GEN_STATUS, usMiiData, NULL);
  }

  return iRslt;
}

/*****************************************************************************/
/*! Get Connection State
* \description
*   Retrieve connection information from PHY.
* \class 
*   EXTPHY 
* \params
*   uPhyAddr              [in]  PHY Address
*   ptPhyState            [out] Pointer to connection status block
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_EXTPHY_GetStatus( unsigned int uPhyAddr, NX5_PHY_STATUS_T* ptPhyState )
{
  unsigned short usMiiData;

  if( uPhyAddr > 31 ) return -1; /* invalid phy register number */

  /* MDIO bug: 1st MDIO read fails */
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_MULTIPLE, &usMiiData, NULL);
  
  /* read out super isolate mode */
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_MULTIPLE, &usMiiData, NULL);
  ptPhyState->fIsSuperIsolate = (usMiiData & BCM5241_AUX_MUL_SUPER_ISOLATE) ? 1 : 0;

  /* read HP-Auto MDIX status */ 
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_ERR_GEN_STATUS, &usMiiData, NULL);
  ptPhyState->fIsAutoMdixEn   = (usMiiData & BCM5241_AUX_ERR_HP_AUTOMDIXDIS) ? 0 : 1;
  ptPhyState->fMdixInUse      = (usMiiData & BCM5241_AUX_ERR_MDIX_STATUS)    ? 1 : 0;

  /* read out PHY status */ 
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUXILIARY_STATUS_SUM, &usMiiData, NULL);
  ptPhyState->fIsLinkUp       = (usMiiData & BCM5241_AUX_STAT_SUM_LINK_STATUS) ? 1   : 0;
  ptPhyState->uSpeed          = (usMiiData & BCM5241_AUX_STAT_SUM_SPEED_IND)   ? 100 : 10;
  ptPhyState->fIsFullDuplex   = (usMiiData & BCM5241_AUX_STAT_SUM_FDX_IND)     ? 1   : 0;
  ptPhyState->fIsAutoNegEn    = (usMiiData & BCM5241_AUX_STAT_SUM_AUTONEG_IND) ? 1   : 0;


  /* read Standby Power Mode, 1. switch to shadow register area, 2. read standby power flag, 3. switch back to standard register area */ 
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_BCM_TEST, &usMiiData, NULL);
  usMiiData |= BCM5241_BCM_TEST_SHADOW_REG_EN;
  NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_BCM_TEST, usMiiData, NULL);

  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_SHADOW_MODE_4, &usMiiData, NULL);
  ptPhyState->fIsPowerDown    = (usMiiData & BCM5241_SHADOW_MODE_4_STANDBY_PWR) ? 1 : 0;
  
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_BCM_TEST, &usMiiData, NULL);
  usMiiData &= ~BCM5241_BCM_TEST_SHADOW_REG_EN;
  NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, uPhyAddr, BCM5241_BCM_TEST, usMiiData, NULL);

  /* read Local Capabilities */ 
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUTO_NEG_ADVERTISEMENT, &usMiiData, NULL);
  ptPhyState->fAdv100BaseX_Fdx   = (usMiiData & BCM5241_ADV_100_BASE_TX_FDX) ? 1 : 0;
  ptPhyState->fAdv100BaseX_Hdx   = (usMiiData & BCM5241_ADV_100_BASE_TX)     ? 1 : 0;
  ptPhyState->fAdv10BaseT_Fdx    = (usMiiData & BCM5241_ADV_10_BASE_T_FDX)   ? 1 : 0;
  ptPhyState->fAdv10BaseT_Hdx    = (usMiiData & BCM5241_ADV_10_BASE_T)       ? 1 : 0;

  /* read Link Partner Link Capabilities */ 
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_AUTO_NEG_LINK_PARTNER_ABIL, &usMiiData, NULL);
  ptPhyState->fLpAdv100BaseX_Fdx   = (usMiiData & BCM5241_ADV_100_BASE_TX_FDX) ? 1 : 0;
  ptPhyState->fLpAdv100BaseX_Hdx   = (usMiiData & BCM5241_ADV_100_BASE_TX)     ? 1 : 0;
  ptPhyState->fLpAdv10BaseT_Fdx    = (usMiiData & BCM5241_ADV_10_BASE_T_FDX)   ? 1 : 0;
  ptPhyState->fLpAdv10BaseT_Hdx    = (usMiiData & BCM5241_ADV_10_BASE_T)       ? 1 : 0;

  /* dummy read */
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPhyAddr, BCM5241_CONTROL, &usMiiData, NULL);

  return 0;
}

/*****************************************************************************/
/*! Get Link State
* \description
*   Retrieve connection information from PHY.
* \class
*   EXTPHY
* \params
*   uPhyAddr              [in]  PHY Address
*   ptPhyState            [out] Pointer to connection status block
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_EXTPHY_GetLinkState( unsigned int uPortNo, unsigned int* puLink, unsigned int* puMbps, unsigned int* puFullDuplex, void* pvUser )
{
  unsigned short usMiiData;

  if( uPortNo >= 2 ) return -1; /* invalid phy register number */

  /* read out PHY status */
  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, uPortNo + NX5_INT_PHY1_ADDR, BCM5241_AUXILIARY_STATUS_SUM, &usMiiData, NULL);
  *puLink       = (usMiiData & BCM5241_AUX_STAT_SUM_LINK_STATUS) ? 1   : 0;
  *puMbps       = (usMiiData & BCM5241_AUX_STAT_SUM_SPEED_IND)   ? 100 : 10;
  *puFullDuplex = (usMiiData & BCM5241_AUX_STAT_SUM_FDX_IND)     ? 1   : 0;

  return 0;
}

/*
  ____            _           _                           _____   _    __         
 |  _ \    ___   (_)  _ __   | |_    ___   _ __          |  ___| (_)  / _|   ___  
 | |_) |  / _ \  | | | '_ \  | __|  / _ \ | '__|         | |_    | | | |_   / _ \ 
 |  __/  | (_) | | | | | | | | |_  |  __/ | |            |  _|   | | |  _| | (_) |
 |_|      \___/  |_| |_| |_|  \__|  \___| |_|     _____  |_|     |_| |_|    \___/ 
                                                 |_____|                          
*/

/*****************************************************************************/
/*! Reset Pointer FIFO
* \description
*   Reset the Pointer FIFO into default state.
* \class 
*   PFIFO 
* \params
*
* \return
                                                                             */
/*****************************************************************************/
void NX5_PFIFO_Reset( void )
{
  unsigned int uCnt;
  
  /* set reset flag of all FIFOs */
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, 0xffffffff);

  /* reset pointer FIFO borders to first 16 FIFOs of size 64 and last 16 FIFOs of size 0 */
  /* Note: 8 FIFOs must have contain exact 512 entries */
  for( uCnt = 0; uCnt < 16; uCnt++ ) {
    NX_WRITE32(s_ptPFifo->aulPfifo_border[uCnt], ((uCnt+1)* 64)-1);
  }
  for( uCnt = 16; uCnt < 32; uCnt++ ) {
    NX_WRITE32(s_ptPFifo->aulPfifo_border[uCnt], ((15+1)* 64)-1);
  }

  /* clear reset flag of all FIFOs */
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, 0);
}

/*****************************************************************************/
/*! Set Pointer FIFO Borders
* \description
*   Set pointer FIFO borders to given values.
* \class 
*   PFIFO 
* \params
* \return
*                                                                            */
/*****************************************************************************/
int NX5_PFIFO_SetBorders( unsigned int *auPFifoDepth )
{
  int iResult;
  unsigned int uBorderTmp;
  unsigned int uFifoNum;

  /* set reset bit for all pointer FIFOs */
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, 0xffffffff);

  /* define pointer FIFO borders */
  uBorderTmp = 0;
  for(uFifoNum=0; uFifoNum<32; uFifoNum++)
  {
    uBorderTmp += auPFifoDepth[uFifoNum];
    NX_WRITE32(s_ptPFifo->aulPfifo_border[uFifoNum], uBorderTmp-1);
  }

  if( uBorderTmp>1024 ) {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  } else {
    /* ok! */
    iResult = 0;

    /* clear reset bit for all pointer FIFOs */
    NX_WRITE32(s_ptPFifo->ulPfifo_reset, 0x00000000);
  }

  return iResult;
}

/*****************************************************************************/
/*! Get FIFO Fill Level
* \description
*   Retrieves the fill level of a single FIFO.
* \class 
*   PFIFO 
* \params
*   uFifoNum  [in]  FIFO number between 0..31
* \return
    >=0 Fill Level
    -1  on Error                                                                */
/*****************************************************************************/
int NX5_PFIFO_GetFillLevel( unsigned int uFifoNum )
{
  /* get fill level of fifo */
  if( uFifoNum<32 ) {
    return NX_READ32(s_ptPFifo->aulPfifo_fill_level[uFifoNum]);
  } else {
    return -1;
  }
}

/*****************************************************************************/
/*! Get FIFO Reset Vector
* \description
*   Retrieves the reset vector of pointer FIFO.
* \class 
*   PFIFO 
* \params
* \return
    reset vector
                                                                             */
/*****************************************************************************/
unsigned long NX5_PFIFO_GetFifoResetVector( void )
{
  return NX_READ32(s_ptPFifo->ulPfifo_reset);
}

/*****************************************************************************/
/*! Get FIFO Full Vector
* \description
*   Retrieves the full vector of pointer FIFO.
* \class 
*   PFIFO 
* \params
* \return
    full vector
                                                                             */
/*****************************************************************************/
unsigned long NX5_PFIFO_GetFifoFullVector( void )
{
  return NX_READ32(s_ptPFifo->ulPfifo_full);
}

/*****************************************************************************/
/*! Get FIFO Empty Vector
* \description
*   Retrieves the empty vector of pointer FIFO.
* \class 
*   PFIFO 
* \params
* \return
    empty vector
                                                                             */
/*****************************************************************************/
unsigned long NX5_PFIFO_GetFifoEmptyVector( void )
{
  return NX_READ32(s_ptPFifo->ulPfifo_empty);
}

/*****************************************************************************/
/*! Get FIFO Overflow Vector
* \description
*   Retrieves the overflow vector of pointer FIFO.
* \class 
*   PFIFO 
* \params
* \return
    overflow vector
                                                                             */
/*****************************************************************************/
unsigned long NX5_PFIFO_GetFifoOverflowVector( void )
{
  return NX_READ32(s_ptPFifo->ulPfifo_overflow);
}

/*****************************************************************************/
/*! Get FIFO Underrun Vector
* \description
*   Retrieves the underrun vector of pointer FIFO.
* \class 
*   PFIFO 
* \params
* \return
    underrun vector
                                                                             */
/*****************************************************************************/
unsigned long NX5_PFIFO_GetFifoUnderrunVector( void )
{
  return NX_READ32(s_ptPFifo->ulPfifo_underrun);
}


/*                                                                  
 ######  #    #  #    #   ####   #####  #   ####   #    #   ####  
 #       #    #  ##   #  #    #    #    #  #    #  ##   #  #      
 #####   #    #  # #  #  #         #    #  #    #  # #  #   ####  
 #       #    #  #  # #  #         #    #  #    #  #  # #       # 
 #       #    #  #   ##  #    #    #    #  #    #  #   ##  #    # 
 #        ####   #    #   ####     #    #   ####   #    #   ####  
                                                                  
                                                        #     #     #     #       
 #    #   ####   ######  #####       #####   #   #      #     #    # #    #       
 #    #  #       #       #    #      #    #   # #       #     #   #   #   #       
 #    #   ####   #####   #    #      #####     #        #######  #     #  #       
 #    #       #  #       #    #      #    #    #        #     #  #######  #       
 #    #  #    #  #       #    #      #    #    #        #     #  #     #  #       
  ####    ####   ######  #####       #####     #        #     #  #     #  ####### 
*/                                                                                  

/*
 __  __   ____           _                           _               
 \ \/ /  / ___|         | |       ___     __ _    __| |   ___   _ __ 
  \  /  | |             | |      / _ \   / _` |  / _` |  / _ \ | '__|
  /  \  | |___          | |___  | (_) | | (_| | | (_| | |  __/ | |   
 /_/\_\  \____|  _____  |_____|  \___/   \__,_|  \__,_|  \___| |_|   
                |_____|                                              

*/

/*****************************************************************************/
/* constants and defines                                                     */
/*****************************************************************************/

static const unsigned long XcCode_rpu_reset0[27] = {
  0x00000064,
  0x00000000,
  0x018e0000, 0x01040001, 0xa15fdc81, 0x01080001, 0xa15fdc82, 0x0113fffd, 0xa15fdc83, 0x0117fffd, 
  0xa15fdc84, 0x01180001, 0xa15fdc85, 0x0113fff9, 0xa15fdc86, 0x011bfffd, 0xa15fdc87, 0x01140001, 
  0xa15fdc88, 0x0113fffd, 0xa15fdc89, 0x01180001, 0xa15fdc8a, 0x01080005, 0xa15fdc8b, 0x00e40000, 
  0x001fdc8b, 
};

static const unsigned long XcCode_rpu_reset1[27] = {
  0x00000064,
  0x00000000,
  0x018e1000, 0x01040001, 0xa15fdc81, 0x01080001, 0xa15fdc82, 0x0113fffd, 0xa15fdc83, 0x0117fffd, 
  0xa15fdc84, 0x01180001, 0xa15fdc85, 0x0113fff9, 0xa15fdc86, 0x011bfffd, 0xa15fdc87, 0x01140001, 
  0xa15fdc88, 0x0113fffd, 0xa15fdc89, 0x01180001, 0xa15fdc8a, 0x01080005, 0xa15fdc8b, 0x00e40000, 
  0x001fdc8b, 
};

static const unsigned long XcCode_tpu_reset0[27] = {
  0x00000064,
  0x00000000,
  0x018e0400, 0x011c0601, 0xa15fdc81, 0x01200001, 0xa15fdc82, 0x012bfffd, 0xa15fdc83, 0x012ffffd, 
  0xa15fdc84, 0x01300001, 0xa15fdc85, 0x012bfff9, 0xa15fdc86, 0x0133fffd, 0xa15fdc87, 0x012c0001, 
  0xa15fdc88, 0x012bfffd, 0xa15fdc89, 0x01300001, 0xa15fdc8a, 0x01200005, 0xa15fdc8b, 0x00e40000, 
  0x001fdc8b, 
};

static const unsigned long XcCode_tpu_reset1[27] = {
  0x00000064, 
  0x00000000, 
  0x018e1400, 0x011c0601, 0xa15fdc81, 0x01200001, 0xa15fdc82, 0x012bfffd, 0xa15fdc83, 0x012ffffd, 
  0xa15fdc84, 0x01300001, 0xa15fdc85, 0x012bfff9, 0xa15fdc86, 0x0133fffd, 0xa15fdc87, 0x012c0001, 
  0xa15fdc88, 0x012bfffd, 0xa15fdc89, 0x01300001, 0xa15fdc8a, 0x01200005, 0xa15fdc8b, 0x00e40000, 
  0x001fdc8b, 
};


/* netx5 address space: start address of the xPEC Register area */
static const unsigned long aulNetx5XpecReg[2] = { Addr_NX5_xpec0, Addr_NX5_xpec1 };

/* netx5 address space: start address of the xPEC PRAM area */
static const unsigned long aulNetx5XpecPram[2] = { Adr_NX5_xpec0_pram_start, Adr_NX5_xpec1_pram_start };

/* netx5 address space: start address of the xPEC DRAM */
static const unsigned long aulNetx5XpecDram[2] = { Adr_NX5_xpec0_dram_start, Adr_NX5_xpec1_dram_start };

/* netx5 address space: start address of the xmac area */
static const unsigned long aulNetx5Xmac[2] = { Addr_NX5_xmac0, Addr_NX5_xmac1 };

static const unsigned long* paulxMacRpuCodes[4]=
{
        XcCode_rpu_reset0,
        XcCode_rpu_reset1
};

static const unsigned long* paulxMacTpuCodes[4]=
{
        XcCode_tpu_reset0,
        XcCode_tpu_reset1
};

#define NUM_XPECS 2

/*****************************************************************************/
/*! Reset Routine for netX5
* \description
*   Resets the XC port.
* \class 
*   XC 
* \params
*   uPortNo              [in]  XC Port Number
*   pvUser               [in] User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_XC_Reset( unsigned int uPortNo, void* pvUser )
{
  NX5_XMAC_AREA_T* ptXmac;
  NX5_XPEC_AREA_T* ptXpec;
  volatile uint32_t* pulPram;
  volatile uint32_t* pulDram;
  
  if( uPortNo >= NUM_XPECS)
    return -1;

  ptXmac  = s_ptXmacStart[uPortNo];
  ptXpec  = s_ptXpecStart[uPortNo];
  pulPram = s_pulXpecPramStart[uPortNo];
  pulDram = s_pulXpecDramStart[uPortNo];
  
  /* Note: 
     On netX5 Evaboard NXEB5 Rev2 the pins XC0/1_IO0 is directly connected to 
     pulled-up Link-LED. This causes bus errors after netX5 reset 
     until XC port is initialized and started with dedicated 
     fieldbus that uses XC0/1_IO0 as driver enable. (PROFIBUS, MVB, CC-Link, RS232, ... ).
   */
  
  /* Stop xPEC */
  NX_WRITE32(ptXpec->ulXpu_hold_pc, 1);
  /* Stop xMAC */
  NX_WRITE32(ptXmac->ulXmac_tpu_hold_pc, MSK_NX5_xmac_tpu_hold_pc_tpu_hold);
  NX_WRITE32(ptXmac->ulXmac_rpu_hold_pc, MSK_NX5_xmac_rpu_hold_pc_rpu_hold);
  /* Clear output enable of tx asap */
  NX_WRITE32(ptXmac->ulXmac_config_obu, 0x00000180);
  /* Clear output enable of io0..5 asap */
  NX_WRITE32(ptXpec->aulStatcfg[uPortNo], 0);

  /* load ratemul reset code */
  if( NX5_XC_Load(uPortNo, NX5_XC_TYPE_RPU, (unsigned long*)paulxMacRpuCodes[uPortNo], pvUser)!=0 ) {
    return -1;
  }
  if( NX5_XC_Load(uPortNo, NX5_XC_TYPE_TPU, (unsigned long*)paulxMacTpuCodes[uPortNo], pvUser)!=0 ) {
    return -1;
  }
  
  NX_WRITE32(ptXmac->ulXmac_rpu_pc, 0);
  NX_WRITE32(ptXmac->ulXmac_tpu_pc, 0);
  NX_WRITE32(ptXmac->ulXmac_rpu_hold_pc, 0);              /* Clear Hold-Bit */
  NX_WRITE32(ptXmac->ulXmac_tpu_hold_pc, 0);              /* Clear Hold-Bit */

  /* !!!! ATTENTION: There must be enougth time between starting xMAC and stopping xMAC to 
          execute reset programm */

  /* clear xPEC events and stop DMA */
  NX_WRITE32(pulPram[0], 0xC0000FFF);           /* Use the command wait b000000000000,b111111111111 at Address 0*/
  NX_WRITE32(ptXpec->ulXpec_pc, 0x7ff);           /* Reset the Program Counter to 0x7ff on netX5 */
  NX_WRITE32(ptXpec->ulXpec_statcfg, 0);          /* Reset Timer operation and DMA */

  NX_WRITE32(ptXpec->ulXpu_hold_pc, 0);           /* Start the Program */
  NX_WRITE32_XPEC(pulDram[0x7FF], 0x7F);        /* Link and Stop DMA */
  NX_WRITE32_XPEC(pulDram[0x7FE], 0x00);        /* dummy access to let xpec run another cycle */
          
  NX_WRITE32(ptXpec->ulXpu_hold_pc, 1);           /* Hold the Program Counter */
  NX_WRITE32(ptXpec->ulXpec_pc, 0x7ff);           /* Reset the Program Counter to 0x7ff on netX5 */

  /* reset all registers */
  NX_WRITE32(ptXpec->aulXpec_r[0], 0);
  NX_WRITE32(ptXpec->aulXpec_r[1], 0);
  NX_WRITE32(ptXpec->aulXpec_r[2], 0);
  NX_WRITE32(ptXpec->aulXpec_r[3], 0);
  NX_WRITE32(ptXpec->aulXpec_r[4], 0);
  NX_WRITE32(ptXpec->aulXpec_r[5], 0);
  NX_WRITE32(ptXpec->aulXpec_r[6], 0);
  NX_WRITE32(ptXpec->aulXpec_r[7], 0);
  NX_WRITE32(ptXpec->aulUsr[0], 0);
  NX_WRITE32(ptXpec->aulUsr[1], 0);
  NX_WRITE32(ptXpec->ulRange45, 0);
  NX_WRITE32(ptXpec->ulRange67, 0);
  NX_WRITE32(ptXpec->ulUrx_count, 0);
  NX_WRITE32(ptXpec->ulUtx_count, 0);
  NX_WRITE32(ptXpec->ulXpec_statcfg, 0);
  NX_WRITE32(ptXpec->aulTimer[0], 0); /* Stop all Timers */
  NX_WRITE32(ptXpec->aulTimer[1], 0);
  NX_WRITE32(ptXpec->aulTimer[2], 0);
  NX_WRITE32(ptXpec->aulTimer[3], 0);
  NX_WRITE32(ptXpec->ulTimer4, 0);
  NX_WRITE32(ptXpec->ulTimer5, 0);

  NX_WRITE32(ptXpec->ulIrq, 0xFFFF0000); /* Clear XPEC side IRQ request lines */

  NX_WRITE32(ptXpec->aulEc_mask[0], 0x0000FFFF); /* Reset events */
  NX_WRITE32(ptXpec->aulEc_mask[1], 0x0000FFFF);
  NX_WRITE32(ptXpec->aulEc_mask[2], 0x0000FFFF);
  NX_WRITE32(ptXpec->aulEc_mask[3], 0x0000FFFF);
  NX_WRITE32(ptXpec->aulEc_mask[4], 0x0000FFFF);
  NX_WRITE32(ptXpec->aulEc_mask[5], 0x0000FFFF);
  NX_WRITE32(ptXpec->aulEc_mask[6], 0x0000FFFF);
  NX_WRITE32(ptXpec->aulEc_mask[7], 0x0000FFFF);
  NX_WRITE32(ptXpec->aulEc_mask[8], 0x0000FFFF);
  NX_WRITE32(ptXpec->aulEc_mask[9], 0x0000FFFF);
  NX_WRITE32(ptXpec->ulEc_maska, 0x0000FFFF);
  NX_WRITE32(ptXpec->ulEc_maskb, 0x0000FFFF);

  NX_WRITE32(ptXpec->ulXpec_adc, 0);

  /* Reset shared registers, reset URX/UTX fifos */
  switch( uPortNo ) {
  case 0:
    NX_WRITE32(ptXpec->aulXpec_sr[0], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[1], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[2], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[3], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[8], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[9], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[10], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[11], 0);
    NX_WRITE32(ptXpec->aulStatcfg[0], (MSK_NX5_statcfg0_reset_rx_fifo | MSK_NX5_statcfg0_reset_tx_fifo));
    NX_WRITE32(ptXpec->aulStatcfg[0], 0x00000000);
    NX_WRITE32(s_ptXpecIrqs->aulIrq_xpec[0], 0xffff); /* confirm all interrupts from xPEC */
    break;
  case 1:
    /* Reset shared registers, reset URX/UTX fifos and ARM side IRQ request lines */
    NX_WRITE32(ptXpec->aulXpec_sr[4], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[5], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[6], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[7], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[12], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[13], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[14], 0);
    NX_WRITE32(ptXpec->aulXpec_sr[15], 0);
    NX_WRITE32(ptXpec->aulStatcfg[1], (MSK_NX5_statcfg1_reset_rx_fifo | MSK_NX5_statcfg1_reset_tx_fifo));
    NX_WRITE32(ptXpec->aulStatcfg[1], 0x00000000);
    NX_WRITE32(s_ptXpecIrqs->aulIrq_xpec[1], 0xffff); /* confirm all interrupts from xPEC */
    break;
  }        
      
  /* Stop xMAC */
  NX_WRITE32(ptXmac->ulXmac_tpu_hold_pc, MSK_NX5_xmac_tpu_hold_pc_tpu_hold);
  NX_WRITE32(ptXmac->ulXmac_rpu_hold_pc, MSK_NX5_xmac_rpu_hold_pc_rpu_hold);
      
  /* reset all xMAC registers to default values */
  NX_WRITE32(ptXmac->ulXmac_rx_hw, 0);
  NX_WRITE32(ptXmac->ulXmac_rx_hw_count, 0);
  NX_WRITE32(ptXmac->ulXmac_tx, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_hw, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_hw_count, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_sent, 0);
  NX_WRITE32(ptXmac->aulXmac_wr[0], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[1], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[2], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[3], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[4], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[5], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[6], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[7], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[8], 0);
  NX_WRITE32(ptXmac->aulXmac_wr[9], 0);
  NX_WRITE32(ptXmac->ulXmac_config_mii, 0);
  NX_WRITE32(ptXmac->ulXmac_config_rx_nibble_fifo, 0x00001000);
  NX_WRITE32(ptXmac->ulXmac_config_tx_nibble_fifo, 0);
  NX_WRITE32(ptXmac->ulXmac_rpu_count1, 0);
  NX_WRITE32(ptXmac->ulXmac_rpu_count1, 0);
  NX_WRITE32(ptXmac->ulXmac_tpu_count1, 0);
  NX_WRITE32(ptXmac->ulXmac_tpu_count2, 0);
  NX_WRITE32(ptXmac->ulXmac_rx_count, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_count, 0);
  NX_WRITE32(ptXmac->ulXmac_rpm_mask0, 0);
  NX_WRITE32(ptXmac->ulXmac_rpm_val0, 0);
  NX_WRITE32(ptXmac->ulXmac_rpm_mask1, 0);
  NX_WRITE32(ptXmac->ulXmac_rpm_val1, 0);
  NX_WRITE32(ptXmac->ulXmac_tpm_mask0, 0);
  NX_WRITE32(ptXmac->ulXmac_tpm_val0, 0);
  NX_WRITE32(ptXmac->ulXmac_tpm_mask1, 0);
  NX_WRITE32(ptXmac->ulXmac_tpm_val1, 0);
  
  NX_WRITE32(ptXmac->ulXmac_rx_crc_polynomial_l, 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc_polynomial_h, 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc_l, 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc_h, 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc_cfg, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_polynomial_l, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_polynomial_h, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_l, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_h, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_cfg, 0);

  NX_WRITE32(ptXmac->ulXmac_rx_tx_nof_bits, 0x00000088);
  NX_WRITE32(ptXmac->ulXmac_rx_crc32_l, 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc32_h, 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc32_cfg, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc32_l, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc32_h, 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc32_cfg, 0);

  NX_WRITE32(ptXmac->ulXmac_rpu_pc, 0);
  NX_WRITE32(ptXmac->ulXmac_tpu_pc, 0);

  return 0;

}

/*****************************************************************************/
/*! Load Microcode to XC Entity
* \description
*   Load XC port.
* \class 
*   XC 
* \params
*   uPortNo              [in]  XC Port Number
*   eXcType              [in]  XC Entity To Download To (RPU, TPU, xPEC)
*   pulXcPrg             [in]  Pointer To Microcode
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_XC_Load( unsigned int uPortNo, NX5_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser )
{
  volatile uint32_t *pulDstCnt, *pulDst;
  volatile uint32_t *pulPRamStart, *pulPRamEnd;
  volatile uint32_t *pulSrcStart, *pulSrcCnt, *pulSrcEnd;
  unsigned int uiElements;
  volatile uint32_t *pulRange0Dpm, *pulRange1Dpm, *pulRange2Dpm;
  unsigned long ulRange0Start, ulRange1Start, ulRange2Start;
  unsigned long ulRange0Size, ulRange1Size, ulRange2Size;
  unsigned long ulAddress, ulOffset;
  
  /* check the instance number */
  if(uPortNo >= NUM_XPECS)
    return -1;

  /* PRAM Area */
  pulRange1Dpm    = s_pulXpecPramStart[uPortNo];
  ulRange1Start   = aulNetx5XpecPram[uPortNo];
  ulRange1Size    = NX5_XPEC_DWORD_RAMSIZE * sizeof(unsigned long);

  /* DRAM Area */
  pulRange2Dpm    = s_pulXpecDramStart[uPortNo];
  ulRange2Start   = aulNetx5XpecDram[uPortNo];
  ulRange2Size    = NX5_XPEC_DWORD_RAMSIZE * sizeof(unsigned long);

  /* get the start and end address of the ram area, get the physical address */
  switch( eXcType ) {
                         
    case NX5_XC_TYPE_XPEC: 
      /* Pointer to PRAM area */
      pulPRamStart    = s_pulXpecPramStart[uPortNo];
      pulPRamEnd      = pulPRamStart + NX5_XPEC_DWORD_RAMSIZE;
      
      /* Register Area */
      pulRange0Dpm    = (volatile uint32_t*)s_ptXpecStart[uPortNo];
      ulRange0Start   = aulNetx5XpecReg[uPortNo];
      ulRange0Size    = 0x0104;
      break;

    case NX5_XC_TYPE_RPU: 
      /* Pointer to PRAM area */
      pulPRamStart    = s_ptXmacStart[uPortNo]->aulXmac_rpu_program;
      pulPRamEnd      = pulPRamStart + NX5_XMAC_RPU_DWORD_RAMSIZE;
      
      /* PRAM Area */
      pulRange0Dpm    = (volatile uint32_t*)s_ptXmacStart[uPortNo];
      ulRange0Start   = aulNetx5Xmac[uPortNo];
      ulRange0Size    = 0x0a08;
      break;
    
    case NX5_XC_TYPE_TPU: 
      /* Pointer to PRAM area */
      pulPRamStart    = s_ptXmacStart[uPortNo]->aulXmac_tpu_program;
      pulPRamEnd      = pulPRamStart + NX5_XMAC_TPU_DWORD_RAMSIZE;
      
      /* PRAM Area */
      pulRange0Dpm    = (volatile uint32_t*)s_ptXmacStart[uPortNo];
      ulRange0Start   = aulNetx5Xmac[uPortNo];
      ulRange0Size    = 0x0a08;
      break;
             
    default: 
      /* unknown unit type */
      return -1;
  }
  
  /* get the number of code elements (first element in array is PRAM start address) */
  uiElements = (pulXcPrg[0] / sizeof(unsigned long)) - 1;

  /* get the pointer in the xc area */
  /* ram_virtual_start + code_physical_start - ram_physical_start */
  pulDst = pulPRamStart;

  /* the code must fit into the PRAM area */
  if( (pulDst + uiElements) > pulPRamEnd ) {
    /* code exceeds PRAM -> exit */
    return -1;
  }

  /* get source start and end pointer */
  pulSrcStart = pulXcPrg + 3;
  pulSrcEnd   = pulSrcStart + uiElements;
  
  /* copy the code to XC */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt < pulSrcEnd )
  {
    NX_WRITE32(pulDstCnt[0], *pulSrcCnt);
    pulDstCnt++;
    pulSrcCnt++;
  }
  
  /* compare the code */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt < pulSrcEnd )
  {
    if( NX_READ32(pulDstCnt[0]) != *pulSrcCnt )
    {
      return -1;
    }
    pulDstCnt++;
    pulSrcCnt++;
  }
  
  /* get the number of trailing loads */
  uiElements = pulXcPrg[1] / sizeof(unsigned long);
  
  /* get source start and end pointer */
  pulSrcCnt = pulXcPrg + 2 + ( pulXcPrg[0] / sizeof(unsigned long) );
  pulSrcEnd = pulSrcCnt + uiElements;
  
  /* write all trailing loads */
  while( pulSrcCnt < pulSrcEnd )
  {
    /* get the address */
    ulAddress = *pulSrcCnt;
    pulSrcCnt++;
    
    /* test if the address is in one of the 3 ranges */
    if( (ulAddress >= ulRange0Start) && (ulAddress < (ulRange0Start + ulRange0Size)) )
    {
      /* address is in range 0 */
      ulOffset = ulAddress - ulRange0Start;
      pulDst = pulRange0Dpm + ulOffset/sizeof(uint32_t);
    }
    else if( (ulAddress >= ulRange1Start) && (ulAddress < (ulRange1Start + ulRange1Size)) )
    {
      /* address is in range 1 */
      ulOffset = ulAddress - ulRange1Start;
      pulDst = pulRange1Dpm + ulOffset/sizeof(uint32_t);
    }
    else if( (ulAddress >= ulRange2Start) && (ulAddress < (ulRange2Start + ulRange2Size)) )
    {
      /* address is in range 2 */
      ulOffset = ulAddress - ulRange2Start;
      pulDst = pulRange2Dpm + ulOffset/sizeof(uint32_t);
    }
    else
    {
      /* address is outside all ranges, cancel! */
      pulDst = NULL;
    }

    /* address error -> exit */
    if( pulDst==NULL )
    {
      return -1;
    }

    /* write the data */
    NX_WRITE32(pulDst[0], *pulSrcCnt);
    pulSrcCnt++;
  }

  return 0;
}

/*****************************************************************************/
/*! Start XC Port
* \description
*   Start XC port.
* \class 
*   XC 
* \params
*   uPortNo              [in]  XC Port Number
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_XC_Start( unsigned int uPortNo, void* pvUser )
{

  if(uPortNo>= NUM_XPECS) {
    return -1;
  }

  /* reset pc of units */
  NX_WRITE32(s_ptXmacStart[uPortNo]->ulXmac_rpu_pc, 0);
  NX_WRITE32(s_ptXmacStart[uPortNo]->ulXmac_tpu_pc, 0);
  NX_WRITE32(s_ptXpecStart[uPortNo]->ulXpec_pc, 0x7ff);
  
  /* start units */
  NX_WRITE32(s_ptXmacStart[uPortNo]->ulXmac_tpu_hold_pc, 0);
  NX_WRITE32(s_ptXpecStart[uPortNo]->ulXpu_hold_pc, 0);
  NX_WRITE32(s_ptXmacStart[uPortNo]->ulXmac_rpu_hold_pc, 0);

  return 0;
}


/*****************************************************************************/
/*! Start XC Units
* \description
*   Start XC Units.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   uUnitVec             [in]  Bit vector defining the units
*                              bit 0: rPU, bit 1: tPU, bit 2: xPEC
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_XC_StartUnits(unsigned int uPortNo, unsigned int uUnitVec, void* pvUser)
{
  NX5_XMAC_AREA_T* ptXmac;
  NX5_XPEC_AREA_T* ptXpec;
  /* check instance number */
  if( uPortNo>=NUM_XPECS ) return -1;

  ptXmac  = s_ptXmacStart[uPortNo];
  ptXpec  = s_ptXpecStart[uPortNo];

  if( 0 != (uUnitVec & MSK_NX5_XC_UNIT_RPU) )
  {
    NX_WRITE32(ptXmac->ulXmac_rpu_pc, 0);
    NX_WRITE32(ptXmac->ulXmac_rpu_hold_pc, 0);
  }

  if( 0 != (uUnitVec & MSK_NX5_XC_UNIT_TPU) )
  {
    NX_WRITE32(ptXmac->ulXmac_tpu_pc, 0);
    NX_WRITE32(ptXmac->ulXmac_tpu_hold_pc, 0);
  }

  if( 0 != (uUnitVec & MSK_NX5_XC_UNIT_XPEC) )
  {
    NX_WRITE32(ptXpec->ulXpec_pc, 0x7ff);
    NX_WRITE32(ptXpec->ulXpu_hold_pc, 0);
  }

  return 0;
}


/*
  __  __   ___   ___   __  __   _   _ 
 |  \/  | |_ _| |_ _| |  \/  | | | | |
 | |\/| |  | |   | |  | |\/| | | | | |
 | |  | |  | |   | |  | |  | | | |_| |
 |_|  |_| |___| |___| |_|  |_|  \___/ 

*/                                      

/*****************************************************************************/
/*! Read PHY Register
* \description
*   Reads a PHY register over MDIO.
* \class 
*   MIIMU 
* \params
*   uMiimuPreamble              [in]  Miimu Preamble
*   uMiimuMdcFreq               [in]  Miimu Mdc Frequence
*   uMiimuRtaField              [in]  Miimu Rta Field
*   uMiimuPhyAddr               [in]  Miimu PHY Address
*   uMiimuReqAddr               [in]  Miimu Register Address
*   pusData                     [out] Miimu Data
*   pvUser                      [in]  User specific parameter 
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_MIIMU_ReadPhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser)
{
  NX5_MIIMU_REG_T tMiiMuReg = {0};

  if( (uMiimuPhyAddr>31) || (uMiimuReqAddr>31) ) return -1; /* invalid phy register number */
  else {
    tMiiMuReg.bf.miimu_snrdy      = 1;
    tMiiMuReg.bf.phy_nres         = 1;
    tMiiMuReg.bf.miimu_preamble   = uMiimuPreamble;
    tMiiMuReg.bf.miimu_mdc_period = uMiimuMdcFreq;
    tMiiMuReg.bf.miimu_rta        = uMiimuRtaField;
    tMiiMuReg.bf.miimu_regaddr    = uMiimuReqAddr;
    tMiiMuReg.bf.miimu_phyaddr    = uMiimuPhyAddr;
  
    /* write command to PHY */
    NX_WRITE32(s_ptMiimu->ulMiimu_reg, tMiiMuReg.val);
  
    /* wait until ready */
    do {
      tMiiMuReg.val = NX_READ32(s_ptMiimu->ulMiimu_reg);
    } while( tMiiMuReg.bf.miimu_snrdy );
  
    *pusData = (unsigned short) tMiiMuReg.bf.miimu_data;
  }

  return 0;
}
/*****************************************************************************/
/*! Write PHY Register
* \description
*   Write PHY register over MDIO.
* \class 
*   MIIMU 
* \params
*   uMiimuPreamble              [in]  Miimu Preamble
*   uMiimuMdcFreq               [in]  Miimu Mdc Frequence
*   uMiimuPhyAddr               [in]  Miimu PHY Address
*   uMiimuReqAddr               [in]  Miimu Register Address
*   usData                      [in]  Miimu Data
*   pvUser                      [in]  User specific parameter 
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX5_MIIMU_WritePhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser)
{
  NX5_MIIMU_REG_T tMiiMuReg = {0};
  
  if( (uMiimuPhyAddr>31) || (uMiimuReqAddr>31) ) return -1; /* invalid phy register number */
  else {
    tMiiMuReg.bf.miimu_snrdy        = 1;
    tMiiMuReg.bf.phy_nres           = 1;
    tMiiMuReg.bf.miimu_opmode       = 1;
    tMiiMuReg.bf.miimu_preamble     = uMiimuPreamble;
    tMiiMuReg.bf.miimu_mdc_period   = uMiimuMdcFreq;
    tMiiMuReg.bf.miimu_regaddr      = uMiimuReqAddr;
    tMiiMuReg.bf.miimu_phyaddr      = uMiimuPhyAddr;
    tMiiMuReg.bf.miimu_data         = usData;
  
    /* write command to PHY */ 
    NX_WRITE32(s_ptMiimu->ulMiimu_reg, tMiiMuReg.val);
  
    /* wait until ready */
    do {
      tMiiMuReg.val = NX_READ32(s_ptMiimu->ulMiimu_reg);
    } while( tMiiMuReg.bf.miimu_snrdy );
  }

  return 0;
}

/*
  ____                  _     _                    
 / ___|   _   _   ___  | |_  (_)  _ __ ___     ___ 
 \___ \  | | | | / __| | __| | | | '_ ` _ \   / _ \
  ___) | | |_| | \__ \ | |_  | | | | | | | | |  __/
 |____/   \__, | |___/  \__| |_| |_| |_| |_|  \___|
          |___/                                    

*/

/*****************************************************************************/
/*! Set Systime Border
* \description
*   Set the systime border.
* \class 
*   SYSTIME 
* \params
*   ulBorder              [in]  Systime Border Value
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX5_SYSTIME_SetBorder( unsigned long ulBorder, void* pvUser )
{
  NX_WRITE32(s_ptSystime->ulSystime_border, ulBorder);
}

/*****************************************************************************/
/*! Set Systime Speed
* \description
*   Set the systime speed.
* \class 
*   SYSTIME 
* \params
*   ulSpeed               [in]  Systime Speed Value
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX5_SYSTIME_SetSpeed( unsigned long ulSpeed, void* pvUser )
{
  NX_WRITE32(s_ptSystime->ulSystime_count_value, ulSpeed);
}

/*****************************************************************************/
/*! Set Systime
* \description
*   Sets the systime.
* \class 
*   SYSTIME 
* \params
*   ulSystime_s           [in]  Systime Value in Seconds
*   ulSystime_ns          [in]  Systime Value in Nanoseconds
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX5_SYSTIME_SetSystime( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser )
{
  NX_WRITE32(s_ptSystime->ulSystime_s, ulSystime_s);
  NX_WRITE32(s_ptSystime->ulSystime_ns, ulSystime_ns);
}

/*****************************************************************************/
/*! Get Systime
* \description
*   Read the systime.
* \class 
*   SYSTIME 
* \params
*   pulSystime_s          [in]  Pointer of Systime Value in Seconds
*   pulSystime_ns         [in]  Pointer of Systime Value in Nanoseconds
*   pvUser                [in]  User specific parameter 
* \return 
*                                                                            */
/*****************************************************************************/
void NX5_SYSTIME_GetSystime( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser )
{
  *pulSystime_s  = NX_READ32(s_ptSystime->ulSystime_s);
  *pulSystime_ns = NX_READ32(s_ptSystime->ulSystime_ns);
}
