/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_ecc.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_ECC_CTRL(s_ptEccCtrl)

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
*   iMem          [in] The RAM area to control
*   fEnable       [in] true/false: enable/disable ECC
*   ulSyndromeInv [in] Bit mask XORed with the Syndrome bits from the RAM
*                                                                            */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

void     HALDEF(ECC_SetCtrl)( int iMem, bool fEnable, uint32_t uSyndromeInv )
{
  uint32_t ulVal = fEnable ? 1UL : 0UL;

  ulVal |= (uSyndromeInv << 1);

  switch(iMem)
  {
  case ECC_MEM_XC0_RPEC0_PRAM: s_ptEccCtrl->aulEcc_ctrl_xc0_rpec_pram_ctrl[0] = ulVal; break;
  case ECC_MEM_XC0_RPEC1_PRAM: s_ptEccCtrl->aulEcc_ctrl_xc0_rpec_pram_ctrl[1] = ulVal; break;
  case ECC_MEM_XC1_RPEC0_PRAM: s_ptEccCtrl->aulEcc_ctrl_xc1_rpec_pram_ctrl[0] = ulVal; break;
  case ECC_MEM_XC1_RPEC1_PRAM: s_ptEccCtrl->aulEcc_ctrl_xc1_rpec_pram_ctrl[1] = ulVal; break;
  case ECC_MEM_XC0_TPEC0_PRAM: s_ptEccCtrl->aulEcc_ctrl_xc0_tpec_pram_ctrl[0] = ulVal; break;
  case ECC_MEM_XC0_TPEC1_PRAM: s_ptEccCtrl->aulEcc_ctrl_xc0_tpec_pram_ctrl[1] = ulVal; break;
  case ECC_MEM_XC1_TPEC0_PRAM: s_ptEccCtrl->aulEcc_ctrl_xc1_tpec_pram_ctrl[0] = ulVal; break;
  case ECC_MEM_XC1_TPEC1_PRAM: s_ptEccCtrl->aulEcc_ctrl_xc1_tpec_pram_ctrl[1] = ulVal; break;
  case ECC_MEM_XC0_XPEC0_DRAM: s_ptEccCtrl->aulEcc_ctrl_xc0_dpram_ctrl[0]     = ulVal; break;
  case ECC_MEM_XC0_XPEC1_DRAM: s_ptEccCtrl->aulEcc_ctrl_xc0_dpram_ctrl[1]     = ulVal; break;
  case ECC_MEM_XC1_XPEC0_DRAM: s_ptEccCtrl->aulEcc_ctrl_xc1_dpram_ctrl[0]     = ulVal; break;
  case ECC_MEM_XC1_XPEC1_DRAM: s_ptEccCtrl->aulEcc_ctrl_xc1_dpram_ctrl[1]     = ulVal; break;
  case ECC_MEM_XC0_RPU0_RAM  : s_ptEccCtrl->aulEcc_ctrl_xc0_rpu_ram_ctrl[0]   = ulVal; break;
  case ECC_MEM_XC0_RPU1_RAM  : s_ptEccCtrl->aulEcc_ctrl_xc0_rpu_ram_ctrl[1]   = ulVal; break;
  case ECC_MEM_XC1_RPU0_RAM  : s_ptEccCtrl->aulEcc_ctrl_xc1_rpu_ram_ctrl[0]   = ulVal; break;
  case ECC_MEM_XC1_RPU1_RAM  : s_ptEccCtrl->aulEcc_ctrl_xc1_rpu_ram_ctrl[1]   = ulVal; break;
  case ECC_MEM_XC0_TPU0_RAM  : s_ptEccCtrl->aulEcc_ctrl_xc0_tpu_ram_ctrl[0]   = ulVal; break;
  case ECC_MEM_XC0_TPU1_RAM  : s_ptEccCtrl->aulEcc_ctrl_xc0_tpu_ram_ctrl[1]   = ulVal; break;
  case ECC_MEM_XC1_TPU0_RAM  : s_ptEccCtrl->aulEcc_ctrl_xc1_tpu_ram_ctrl[0]   = ulVal; break;
  case ECC_MEM_XC1_TPU1_RAM  : s_ptEccCtrl->aulEcc_ctrl_xc1_tpu_ram_ctrl[1]   = ulVal; break;
  case ECC_MEM_XC0_PFIFO     : s_ptEccCtrl->aulEcc_ctrl_xc_pfifo_ctrl[0]      = ulVal; break;
  case ECC_MEM_XC1_PFIFO     : s_ptEccCtrl->aulEcc_ctrl_xc_pfifo_ctrl[1]      = ulVal; break;
  case ECC_MEM_XPIC0_PRAM    : s_ptEccCtrl->aulEcc_ctrl_xpic_pram_ctrl[0]     = ulVal; break;
  case ECC_MEM_XPIC1_PRAM    : s_ptEccCtrl->aulEcc_ctrl_xpic_pram_ctrl[1]     = ulVal; break;
  case ECC_MEM_XPIC2_PRAM    : s_ptEccCtrl->aulEcc_ctrl_xpic_pram_ctrl[2]     = ulVal; break;
  case ECC_MEM_XPIC3_PRAM    : s_ptEccCtrl->aulEcc_ctrl_xpic_pram_ctrl[3]     = ulVal; break;
  case ECC_MEM_XPIC0_DRAM    : s_ptEccCtrl->aulEcc_ctrl_xpic_dram_ctrl[0]     = ulVal; break;
  case ECC_MEM_XPIC1_DRAM    : s_ptEccCtrl->aulEcc_ctrl_xpic_dram_ctrl[1]     = ulVal; break;
  case ECC_MEM_XPIC2_DRAM    : s_ptEccCtrl->aulEcc_ctrl_xpic_dram_ctrl[2]     = ulVal; break;
  case ECC_MEM_XPIC3_DRAM    : s_ptEccCtrl->aulEcc_ctrl_xpic_dram_ctrl[3]     = ulVal; break;
  case ECC_MEM_INTRAMETH     : s_ptEccCtrl->ulEcc_ctrl_intrameth_ctrl         = ulVal; break;
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
*   iMem          [in] The RAM area to control
*   fDbe          [in] true/false: double bit error/single bit error
*   ulSyndromeInv [in] Bit mask XORed with the Syndrome bits from the RAM
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(ECC_GetErrAdr)( int iMem, bool fDbe )
{
  uint32_t ulVal;

  switch(iMem)
  {
  case ECC_MEM_XC0_RPEC0_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc0_rpec_pram_addr_dbe[0] : s_ptEccCtrl->aulEcc_ctrl_xc0_rpec_pram_addr_sbe[0]) << 2; break;
  case ECC_MEM_XC0_RPEC1_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc0_rpec_pram_addr_dbe[1] : s_ptEccCtrl->aulEcc_ctrl_xc0_rpec_pram_addr_sbe[1]) << 2; break;
  case ECC_MEM_XC1_RPEC0_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc1_rpec_pram_addr_dbe[0] : s_ptEccCtrl->aulEcc_ctrl_xc1_rpec_pram_addr_sbe[0]) << 2; break;
  case ECC_MEM_XC1_RPEC1_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc1_rpec_pram_addr_dbe[1] : s_ptEccCtrl->aulEcc_ctrl_xc1_rpec_pram_addr_sbe[1]) << 2; break;
  case ECC_MEM_XC0_TPEC0_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc0_tpec_pram_addr_dbe[0] : s_ptEccCtrl->aulEcc_ctrl_xc0_tpec_pram_addr_sbe[0]) << 2; break;
  case ECC_MEM_XC0_TPEC1_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc0_tpec_pram_addr_dbe[1] : s_ptEccCtrl->aulEcc_ctrl_xc0_tpec_pram_addr_sbe[1]) << 2; break;
  case ECC_MEM_XC1_TPEC0_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc1_tpec_pram_addr_dbe[0] : s_ptEccCtrl->aulEcc_ctrl_xc1_tpec_pram_addr_sbe[0]) << 2; break;
  case ECC_MEM_XC1_TPEC1_PRAM: ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc1_tpec_pram_addr_dbe[1] : s_ptEccCtrl->aulEcc_ctrl_xc1_tpec_pram_addr_sbe[1]) << 2; break;
  case ECC_MEM_XC0_RPU0_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc0_rpu_ram_addr_dbe[0]   : s_ptEccCtrl->aulEcc_ctrl_xc0_rpu_ram_addr_sbe[0]  ) << 3; break;
  case ECC_MEM_XC0_RPU1_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc0_rpu_ram_addr_dbe[1]   : s_ptEccCtrl->aulEcc_ctrl_xc0_rpu_ram_addr_sbe[1]  ) << 3; break;
  case ECC_MEM_XC1_RPU0_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc1_rpu_ram_addr_dbe[0]   : s_ptEccCtrl->aulEcc_ctrl_xc1_rpu_ram_addr_sbe[0]  ) << 3; break;
  case ECC_MEM_XC1_RPU1_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc1_rpu_ram_addr_dbe[1]   : s_ptEccCtrl->aulEcc_ctrl_xc1_rpu_ram_addr_sbe[1]  ) << 3; break;
  case ECC_MEM_XC0_TPU0_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc0_tpu_ram_addr_dbe[0]   : s_ptEccCtrl->aulEcc_ctrl_xc0_tpu_ram_addr_sbe[0]  ) << 3; break;
  case ECC_MEM_XC0_TPU1_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc0_tpu_ram_addr_dbe[1]   : s_ptEccCtrl->aulEcc_ctrl_xc0_tpu_ram_addr_sbe[1]  ) << 3; break;
  case ECC_MEM_XC1_TPU0_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc1_tpu_ram_addr_dbe[0]   : s_ptEccCtrl->aulEcc_ctrl_xc1_tpu_ram_addr_sbe[0]  ) << 3; break;
  case ECC_MEM_XC1_TPU1_RAM  : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xc1_tpu_ram_addr_dbe[1]   : s_ptEccCtrl->aulEcc_ctrl_xc1_tpu_ram_addr_sbe[1]  ) << 3; break;
  case ECC_MEM_XPIC0_PRAM    : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xpic_pram_addr_dbe[0]     : s_ptEccCtrl->aulEcc_ctrl_xpic_pram_addr_sbe[0]    ) << 2; break;
  case ECC_MEM_XPIC1_PRAM    : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xpic_pram_addr_dbe[1]     : s_ptEccCtrl->aulEcc_ctrl_xpic_pram_addr_sbe[1]    ) << 2; break;
  case ECC_MEM_XPIC2_PRAM    : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xpic_pram_addr_dbe[2]     : s_ptEccCtrl->aulEcc_ctrl_xpic_pram_addr_sbe[2]    ) << 2; break;
  case ECC_MEM_XPIC3_PRAM    : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xpic_pram_addr_dbe[3]     : s_ptEccCtrl->aulEcc_ctrl_xpic_pram_addr_sbe[3]    ) << 2; break;
  case ECC_MEM_XPIC0_DRAM    : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xpic_dram_addr_dbe[0]     : s_ptEccCtrl->aulEcc_ctrl_xpic_dram_addr_sbe[0]    ) << 2; break;
  case ECC_MEM_XPIC1_DRAM    : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xpic_dram_addr_dbe[1]     : s_ptEccCtrl->aulEcc_ctrl_xpic_dram_addr_sbe[1]    ) << 2; break;
  case ECC_MEM_XPIC2_DRAM    : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xpic_dram_addr_dbe[2]     : s_ptEccCtrl->aulEcc_ctrl_xpic_dram_addr_sbe[2]    ) << 2; break;
  case ECC_MEM_XPIC3_DRAM    : ulVal = (fDbe ? s_ptEccCtrl->aulEcc_ctrl_xpic_dram_addr_dbe[3]     : s_ptEccCtrl->aulEcc_ctrl_xpic_dram_addr_sbe[3]    ) << 2; break;
  case ECC_MEM_INTRAMETH     : ulVal = (fDbe ? s_ptEccCtrl->ulEcc_ctrl_intrameth_addr_dbe         : s_ptEccCtrl->ulEcc_ctrl_intrameth_addr_sbe        );
    ulVal &= HW_MSK(ecc_ctrl_intrameth_addr_sbe_address);
    ulVal <<= 2;
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
uint32_t HALDEF(ECC_GetIrq)( bool fDbe )
{
  return fDbe ? s_ptEccCtrl->ulEcc_ctrl_status_dbe : s_ptEccCtrl->ulEcc_ctrl_status_sbe;
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
void     HALDEF(ECC_ConfirmIrq)( uint32_t uIrqMsk, bool fDbe )
{
  if( fDbe )
  {
    s_ptEccCtrl->ulEcc_ctrl_status_dbe = uIrqMsk;
  }
  else
  {
    s_ptEccCtrl->ulEcc_ctrl_status_sbe = uIrqMsk;
  }
}
