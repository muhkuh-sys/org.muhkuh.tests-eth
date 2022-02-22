/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_ecc.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_ECC_CTRL(s_ptEccCtrl)
HW_PTR_ECC_CTRL_SHD(s_ptEccCtrlShd)

#define SRT_ECC_SHD 24

#ifdef CPU_IS_ARM_APP
#define ECC_STATUS_SBE(p) p->ulEcc_ctrl_app_status_sbe
#define ECC_STATUS_DBE(p) p->ulEcc_ctrl_app_status_dbe
#else
#define ECC_STATUS_SBE(p) p->ulEcc_ctrl_com_status_sbe
#define ECC_STATUS_DBE(p) p->ulEcc_ctrl_com_status_dbe
#endif

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! ECC Set Control
* \description
*   This function enables/disables ECC checking for a specific RAM.
*   The inversion of the syndrome bits can be used to test the ECC mechanism.
* \class
*   ECC
* \params
*   iMem          [in] The index of the memory to control
*   fEnable       [in] true/false: enable/disable ECC
*   ulSyndromeInv [in] Bit mask XORed with the Syndrome bits from the Memory
*                                                                            */
/*****************************************************************************/
void ECC_SetCtrl( int iMem, bool fEnable, uint32_t uSyndromeInv )
{
  uint32_t ulVal = fEnable ? 1UL : 0UL;

  ulVal |= (uSyndromeInv << 1);

  switch(iMem)
  {
#ifdef CPU_IS_ARM_APP
  case ECC_MEM_INTRAM6       : s_ptEccCtrl->ulEcc_ctrl_app_intram6_ctrl           = ulVal; break;
  case ECC_MEM_INTRAM7       : s_ptEccCtrl->ulEcc_ctrl_app_intram7_ctrl           = ulVal; break;
  case ECC_MEM_XPIC1_PRAM    : s_ptEccCtrl->ulEcc_ctrl_app_xpic1_pram_ctrl        = ulVal; break;
  case ECC_MEM_XPIC1_DRAM    : s_ptEccCtrl->ulEcc_ctrl_app_xpic1_dram_ctrl        = ulVal; break;
#else
  case ECC_MEM_INTRAM0       : s_ptEccCtrl->aulEcc_ctrl_com_intram_ctrl[0]        = ulVal; break;
  case ECC_MEM_INTRAM1       : s_ptEccCtrl->aulEcc_ctrl_com_intram_ctrl[1]        = ulVal; break;
  case ECC_MEM_INTRAM2       : s_ptEccCtrl->aulEcc_ctrl_com_intram_ctrl[2]        = ulVal; break;
  case ECC_MEM_INTRAM3       : s_ptEccCtrl->aulEcc_ctrl_com_intram_ctrl[3]        = ulVal; break;
  case ECC_MEM_INTRAM4       : s_ptEccCtrl->aulEcc_ctrl_com_intram_ctrl[4]        = ulVal; break;
  case ECC_MEM_INTRAM5       : s_ptEccCtrl->aulEcc_ctrl_com_intram_ctrl[5]        = ulVal; break;
  case ECC_MEM_INTRAMHS      : s_ptEccCtrl->ulEcc_ctrl_com_intramhs_ctrl          = ulVal; break;
  case ECC_MEM_XC0_RPEC0_PRAM: s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpec_pram_ctrl[0] = ulVal; break;
  case ECC_MEM_XC0_RPEC1_PRAM: s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpec_pram_ctrl[1] = ulVal; break;
  case ECC_MEM_XC0_TPEC0_PRAM: s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpec_pram_ctrl[0] = ulVal; break;
  case ECC_MEM_XC0_TPEC1_PRAM: s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpec_pram_ctrl[1] = ulVal; break;
  case ECC_MEM_XC0_XPEC0_DRAM: s_ptEccCtrl->aulEcc_ctrl_com_xc0_dpram_ctrl[0]     = ulVal; break;
  case ECC_MEM_XC0_XPEC1_DRAM: s_ptEccCtrl->aulEcc_ctrl_com_xc0_dpram_ctrl[1]     = ulVal; break;
  case ECC_MEM_XC0_RPU0_RAM  : s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpu_ram_ctrl[0]   = ulVal; break;
  case ECC_MEM_XC0_RPU1_RAM  : s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpu_ram_ctrl[1]   = ulVal; break;
  case ECC_MEM_XC0_TPU0_RAM  : s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpu_ram_ctrl[0]   = ulVal; break;
  case ECC_MEM_XC0_TPU1_RAM  : s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpu_ram_ctrl[1]   = ulVal; break;
  case ECC_MEM_XC0_PFIFO     : s_ptEccCtrl->ulEcc_ctrl_com_xc0_pfifo_ctrl         = ulVal; break;
  case ECC_MEM_XPIC0_PRAM    : s_ptEccCtrl->ulEcc_ctrl_com_xpic0_pram_ctrl        = ulVal; break;
  case ECC_MEM_XPIC0_DRAM    : s_ptEccCtrl->ulEcc_ctrl_com_xpic0_dram_ctrl        = ulVal; break;
  case ECC_MEM_IFLASH0       : s_ptEccCtrl->aulEcc_ctrl_com_iflash_ctrl[0]        = ulVal; break;
  case ECC_MEM_IFLASH1       : s_ptEccCtrl->aulEcc_ctrl_com_iflash_ctrl[1]        = ulVal; break;
#endif

  case ECC_MEM_IFLASH2       : s_ptEccCtrlShd->ulEcc_ctrl_iflash2_ctrl            = ulVal; break;

  default: return;
  }

}

/*****************************************************************************/
/*! ECC Get Error Address
* \description
*   This function gets the address of the latest detected error for a
*   specific RAM. There are two different addresses available, one for each
*   error type i.e. single bit errors and double bit errors.
* \class
*   ECC
* \params
*   eMem          [in] The index of the memory to control
*   fDbe          [in] true/false: double bit error/single bit error
*   ulSyndromeInv [in] Bit mask XORed with the Syndrome bits from the memory
*                                                                            */
/*****************************************************************************/
uint32_t ECC_GetErrAdr( int iMem, bool fDbe )
{
  uint32_t ulVal;

  switch(iMem)
  {
#ifdef CPU_IS_ARM_APP
  case ECC_MEM_INTRAM6       : ulVal = (fDbe ? s_ptEccCtrl->ulEcc_ctrl_app_intram6_addr_dbe       : s_ptEccCtrl->ulEcc_ctrl_app_intram6_addr_sbe      );
    ulVal &= HW_MSK(ecc_ctrl_app_intram6_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_INTRAM7       : ulVal = (fDbe ? s_ptEccCtrl->ulEcc_ctrl_app_intram7_addr_dbe       : s_ptEccCtrl->ulEcc_ctrl_app_intram7_addr_sbe      );
    ulVal &= HW_MSK(ecc_ctrl_app_intram7_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_XPIC1_PRAM    : ulVal = (fDbe ? s_ptEccCtrl->ulEcc_ctrl_app_xpic1_pram_addr_dbe    : s_ptEccCtrl->ulEcc_ctrl_app_xpic1_pram_addr_sbe   ) << 2; break;
  case ECC_MEM_XPIC1_DRAM    : ulVal = (fDbe ? s_ptEccCtrl->ulEcc_ctrl_app_xpic1_dram_addr_dbe    : s_ptEccCtrl->ulEcc_ctrl_app_xpic1_dram_addr_sbe   ) << 2; break;
#else
  case ECC_MEM_INTRAM0       : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_dbe[0]        : s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_sbe[0]       );
    ulVal &= HW_MSK(ecc_ctrl_com_intram0_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_INTRAM1       : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_dbe[1]        : s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_sbe[1]       );
    ulVal &= HW_MSK(ecc_ctrl_com_intram1_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_INTRAM2       : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_dbe[2]        : s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_sbe[2]       );
    ulVal &= HW_MSK(ecc_ctrl_com_intram2_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_INTRAM3       : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_dbe[3]        : s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_sbe[3]       );
    ulVal &= HW_MSK(ecc_ctrl_com_intram3_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_INTRAM4       : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_dbe[4]        : s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_sbe[4]       );
    ulVal &= HW_MSK(ecc_ctrl_com_intram4_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_INTRAM5       : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_dbe[5]        : s_ptEccCtrl->aulEcc_ctrl_com_intram_addr_sbe[5]       );
    ulVal &= HW_MSK(ecc_ctrl_com_intram5_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_INTRAMHS      : ulVal = (fDbe ? s_ptEccCtrl->ulEcc_ctrl_com_intramhs_addr_dbe          : s_ptEccCtrl->ulEcc_ctrl_com_intramhs_addr_sbe         );
    ulVal &= HW_MSK(ecc_ctrl_com_intramhs_addr_sbe_address);
    ulVal <<= 2;
    break;
  case ECC_MEM_XC0_RPEC0_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpec_pram_addr_dbe[0] : s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpec_pram_addr_sbe[0]) << 2; break;
  case ECC_MEM_XC0_RPEC1_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpec_pram_addr_dbe[1] : s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpec_pram_addr_sbe[1]) << 2; break;
  case ECC_MEM_XC0_TPEC0_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpec_pram_addr_dbe[0] : s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpec_pram_addr_sbe[0]) << 2; break;
  case ECC_MEM_XC0_TPEC1_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpec_pram_addr_dbe[1] : s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpec_pram_addr_sbe[1]) << 2; break;
  case ECC_MEM_XC0_RPU0_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpu_ram_addr_dbe[0]   : s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpu_ram_addr_sbe[0]  ) << 3; break;
  case ECC_MEM_XC0_RPU1_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpu_ram_addr_dbe[1]   : s_ptEccCtrl->aulEcc_ctrl_com_xc0_rpu_ram_addr_sbe[1]  ) << 3; break;
  case ECC_MEM_XC0_TPU0_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpu_ram_addr_dbe[0]   : s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpu_ram_addr_sbe[0]  ) << 3; break;
  case ECC_MEM_XC0_TPU1_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpu_ram_addr_dbe[1]   : s_ptEccCtrl->aulEcc_ctrl_com_xc0_tpu_ram_addr_sbe[1]  ) << 3; break;
  case ECC_MEM_XC0_PFIFO     : ulVal = 0; /* ERROR: No address for this RAM available! */ break;
  case ECC_MEM_XPIC0_PRAM    : ulVal = (fDbe ? s_ptEccCtrl->ulEcc_ctrl_com_xpic0_pram_addr_dbe        : s_ptEccCtrl->ulEcc_ctrl_com_xpic0_pram_addr_sbe       ) << 2; break;
  case ECC_MEM_XPIC0_DRAM    : ulVal = (fDbe ? s_ptEccCtrl->ulEcc_ctrl_com_xpic0_dram_addr_dbe        : s_ptEccCtrl->ulEcc_ctrl_com_xpic0_dram_addr_sbe       ) << 2; break;
  case ECC_MEM_IFLASH0       : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_iflash_addr_dbe[0]        : s_ptEccCtrl->aulEcc_ctrl_com_iflash_addr_sbe[0]       );
    ulVal &= HW_MSK(ecc_ctrl_com_iflash0_addr_sbe_address);
    ulVal <<= 4;
    break;
  case ECC_MEM_IFLASH1       : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_com_iflash_addr_dbe[1]        : s_ptEccCtrl->aulEcc_ctrl_com_iflash_addr_sbe[1]       );
    ulVal &= HW_MSK(ecc_ctrl_com_iflash1_addr_sbe_address);
    ulVal <<= 4;
    break;
#endif
  case ECC_MEM_IFLASH2       : ulVal = (fDbe ? s_ptEccCtrlShd->ulEcc_ctrl_iflash2_addr_dbe            : s_ptEccCtrlShd->ulEcc_ctrl_iflash2_addr_sbe           );
    ulVal &= HW_MSK(ecc_ctrl_iflash2_addr_sbe_address);
    ulVal <<= 4;
    break;
  default: return 0; /* ERROR: No address for this RAM available! */

  }
  return ulVal;
}

/*****************************************************************************/
/*! ECC Get Interrupt
* \description
*   This function gets the current status and interrupt vector of the ECC unit.
*   The returned value is a bit mask with each bit indicating a detected error.
*   There are two status/interrupt values available, one for each
*   error type i.e. single bit errors and double bit errors.
* \class
*   ECC
* \params
*   fDbe          [in] true/false: double bit error/single bit error
* \return
*   Bit mask indicating the detected errors and pending IRQs                 */
/*****************************************************************************/
uint32_t ECC_GetIrq( bool fDbe )
{
  return fDbe
         ? (ECC_STATUS_DBE(s_ptEccCtrl) | (s_ptEccCtrlShd->ulEcc_ctrl_status_dbe << SRT_ECC_SHD))
         : (ECC_STATUS_SBE(s_ptEccCtrl) | (s_ptEccCtrlShd->ulEcc_ctrl_status_sbe << SRT_ECC_SHD));
}

/*****************************************************************************/
/*! ECC Confirm Interrupt
* \description
*   This function resets the current status and confirms the corresponding
*   interrupts.
*   There are two status/interrupt values available, one for each
*   error type i.e. single bit errors and double bit errors.
* \class
*   ECC
* \params
*   fDbe          [in] true/false: double bit error/single bit error
*   uIrqMsk       [in] Bit mask indicating the errors to reset
* \return
*   Bit mask indicating the detected errors and pending IRQs                 */
/*****************************************************************************/
void ECC_ConfirmIrq( uint32_t uIrqMsk, bool fDbe )
{
  if( fDbe )
  {
    ECC_STATUS_DBE(s_ptEccCtrl) = uIrqMsk;
    s_ptEccCtrlShd->ulEcc_ctrl_status_dbe = (uIrqMsk >> SRT_ECC_SHD);
  }
  else
  {
    ECC_STATUS_SBE(s_ptEccCtrl) = uIrqMsk;
    s_ptEccCtrlShd->ulEcc_ctrl_status_sbe = (uIrqMsk >> SRT_ECC_SHD);
  }
}

/*****************************************************************************/
/*! ECC Get IRQ Mask
* \description
*   This function gets the mask in the status register of the given memory.
*   specific RAM. There are two different addresses available, one for each
*   error type i.e. single bit errors and double bit errors.
* \class
*   ECC
* \params
*   iMem          [in] The index of the memory to control
*   fDbe          [in] true/false: double bit error/single bit error
*                                                                            */
/*****************************************************************************/
uint32_t ECC_GetIrqMask( int iMem, bool fDbe )
{
  uint32_t ulVal;

  switch(iMem)
  {
#ifdef CPU_IS_ARM_APP
  case ECC_MEM_INTRAM6       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_app_status_dbe_intram6)           : HW_MSK(ecc_ctrl_app_status_sbe_intram6)          ); break;
  case ECC_MEM_INTRAM7       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_app_status_dbe_intram7)           : HW_MSK(ecc_ctrl_app_status_sbe_intram7)          ); break;
  case ECC_MEM_XPIC1_PRAM    : ulVal = (fDbe ? HW_MSK(ecc_ctrl_app_status_dbe_xpic1_pram)        : HW_MSK(ecc_ctrl_app_status_sbe_xpic1_pram)       ); break;
  case ECC_MEM_XPIC1_DRAM    : ulVal = (fDbe ? HW_MSK(ecc_ctrl_app_status_dbe_xpic1_dram)        : HW_MSK(ecc_ctrl_app_status_sbe_xpic1_dram)       ); break;
#else
  case ECC_MEM_INTRAM0       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_intram0)           : HW_MSK(ecc_ctrl_com_status_sbe_intram0)          ); break;
  case ECC_MEM_INTRAM1       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_intram1)           : HW_MSK(ecc_ctrl_com_status_sbe_intram1)          ); break;
  case ECC_MEM_INTRAM2       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_intram2)           : HW_MSK(ecc_ctrl_com_status_sbe_intram2)          ); break;
  case ECC_MEM_INTRAM3       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_intram3)           : HW_MSK(ecc_ctrl_com_status_sbe_intram3)          ); break;
  case ECC_MEM_INTRAM4       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_intram4)           : HW_MSK(ecc_ctrl_com_status_sbe_intram4)          ); break;
  case ECC_MEM_INTRAM5       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_intram5)           : HW_MSK(ecc_ctrl_com_status_sbe_intram5)          ); break;
  case ECC_MEM_INTRAMHS      : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_intramhs)          : HW_MSK(ecc_ctrl_com_status_sbe_intramhs)         ); break;
  case ECC_MEM_XC0_RPEC0_PRAM: ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_rpec0_pram)    : HW_MSK(ecc_ctrl_com_status_sbe_xc0_rpec0_pram)   ); break;
  case ECC_MEM_XC0_RPEC1_PRAM: ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_rpec1_pram)    : HW_MSK(ecc_ctrl_com_status_sbe_xc0_rpec1_pram)   ); break;
  case ECC_MEM_XC0_TPEC0_PRAM: ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_tpec0_pram)    : HW_MSK(ecc_ctrl_com_status_sbe_xc0_tpec0_pram)   ); break;
  case ECC_MEM_XC0_TPEC1_PRAM: ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_tpec1_pram)    : HW_MSK(ecc_ctrl_com_status_sbe_xc0_tpec1_pram)   ); break;
  case ECC_MEM_XC0_XPEC0_DRAM: ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_dpram0)        : HW_MSK(ecc_ctrl_com_status_sbe_xc0_dpram0)       ); break;
  case ECC_MEM_XC0_XPEC1_DRAM: ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_dpram1)        : HW_MSK(ecc_ctrl_com_status_sbe_xc0_dpram1)       ); break;
  case ECC_MEM_XC0_RPU0_RAM  : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_rpu0_ram)      : HW_MSK(ecc_ctrl_com_status_sbe_xc0_rpu0_ram)     ); break;
  case ECC_MEM_XC0_RPU1_RAM  : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_rpu1_ram)      : HW_MSK(ecc_ctrl_com_status_sbe_xc0_rpu1_ram)     ); break;
  case ECC_MEM_XC0_TPU0_RAM  : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_tpu0_ram)      : HW_MSK(ecc_ctrl_com_status_sbe_xc0_tpu0_ram)     ); break;
  case ECC_MEM_XC0_TPU1_RAM  : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_tpu1_ram)      : HW_MSK(ecc_ctrl_com_status_sbe_xc0_tpu1_ram)     ); break;
  case ECC_MEM_XC0_PFIFO     : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xc0_pfifo)         : HW_MSK(ecc_ctrl_com_status_sbe_xc0_pfifo)        ); break;
  case ECC_MEM_XPIC0_PRAM    : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xpic0_pram)        : HW_MSK(ecc_ctrl_com_status_sbe_xpic0_pram)       ); break;
  case ECC_MEM_XPIC0_DRAM    : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_xpic0_dram)        : HW_MSK(ecc_ctrl_com_status_sbe_xpic0_dram)       ); break;
  case ECC_MEM_IFLASH0       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_iflash0)           : HW_MSK(ecc_ctrl_com_status_sbe_iflash0)          ); break;
  case ECC_MEM_IFLASH1       : ulVal = (fDbe ? HW_MSK(ecc_ctrl_com_status_dbe_iflash1)           : HW_MSK(ecc_ctrl_com_status_sbe_iflash1)          ); break;
#endif
  case ECC_MEM_IFLASH2       : ulVal = (fDbe ? (HW_MSK(ecc_ctrl_status_dbe_iflash2) << SRT_ECC_SHD) : (HW_MSK(ecc_ctrl_status_sbe_iflash2) << SRT_ECC_SHD)); break;
  default: return 0; /* ERROR: No address for this RAM available! */

  }
  return ulVal;
}
