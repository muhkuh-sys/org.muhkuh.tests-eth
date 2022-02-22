/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_rap_ecc.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_RAP_ECC_CTRL(s_aptRapEccCtrl)


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! RAP_ECC Set Control
* \description
*   This function enables/disables ECC checking for a specific RAM.
*   The inversion of the syndrome bits can be used to test the ECC mechanism.
* \class
*   RAP_ECC
* \params
*   iMemId        [in] The RAM area to control
*   fEnable       [in] true/false: enable/disable ECC
*   ulSyndromeInv [in] Bit mask XORed with the Syndrome bits from the RAM
*                                                                            */
/*****************************************************************************/
void     RAP_ECC_SetCtrl( int iMemId, bool fEnable, uint32_t ulSyndromeInv )
{
  uint32_t ulVal = fEnable ? 1UL : 0UL;

  ulVal |= (ulSyndromeInv << 1);

  switch(iMemId)
  {
  case RAP_ECC_RAM_CR7LLRAM0  : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_CTRL[0] = ulVal; break;
  case RAP_ECC_RAM_CR7LLRAM1  : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_CTRL[1] = ulVal; break;
  case RAP_ECC_RAM_CR7LLRAM2  : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_CTRL[2] = ulVal; break;
  case RAP_ECC_RAM_CR7LLRAM3  : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_CTRL[3] = ulVal; break;
  case RAP_ECC_RAM_CR7LLRAM4  : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_CTRL[4] = ulVal; break;
  case RAP_ECC_RAM_CR7LLRAM5  : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_CTRL[5] = ulVal; break;
  case RAP_ECC_RAM_CR7LLRAM6  : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_CTRL[6] = ulVal; break;
  case RAP_ECC_RAM_CR7LLRAM7  : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_CTRL[7] = ulVal; break;

  case RAP_ECC_RAM_INTRAM_RAP0: s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_CTRL[0] = ulVal; break;
  case RAP_ECC_RAM_INTRAM_RAP1: s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_CTRL[1] = ulVal; break;
  case RAP_ECC_RAM_INTRAM_RAP2: s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_CTRL[2] = ulVal; break;
  case RAP_ECC_RAM_INTRAM_RAP3: s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_CTRL[3] = ulVal; break;

  case RAP_ECC_RAM_INTRAM0    : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_CTRL[0] = ulVal; break;
  case RAP_ECC_RAM_INTRAM1    : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_CTRL[1] = ulVal; break;
  case RAP_ECC_RAM_INTRAM2    : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_CTRL[2] = ulVal; break;
  case RAP_ECC_RAM_INTRAM3    : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_CTRL[3] = ulVal; break;
  case RAP_ECC_RAM_INTRAM4    : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_CTRL[4] = ulVal; break;
  case RAP_ECC_RAM_INTRAM5    : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_CTRL[5] = ulVal; break;
  case RAP_ECC_RAM_INTRAMHS0  : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_CTRL[6] = ulVal; break;
  case RAP_ECC_RAM_INTRAMHS1  : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_CTRL[7] = ulVal; break;

  default: return;
  }

}

/*****************************************************************************/
/*! RAP_ECC Get Error Address
* \description
*   This function gets the address of the latest detected error for a
*   specific RAM. There are two different addresses available, one for each
*   error type i.e. single bit errors and double bit errors.
* \class
*   RAP_ECC
* \params
*   iMemId          [in] The RAM area to control
*   fDbe          [in] true/false: double bit error/single bit error
*   ulSyndromeInv [in] Bit mask XORed with the Syndrome bits from the RAM
*                                                                            */
/*****************************************************************************/
uint32_t RAP_ECC_GetErrAdr( int iMemId, bool fDbe )
{
  uint32_t ulVal;

  switch(iMemId)
  {
  case RAP_ECC_RAM_CR7LLRAM0:   ulVal = (fDbe ? s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[0] : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[0]); break;
  case RAP_ECC_RAM_CR7LLRAM1:   ulVal = (fDbe ? s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[1] : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[1]); break;
  case RAP_ECC_RAM_CR7LLRAM2:   ulVal = (fDbe ? s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[2] : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[2]); break;
  case RAP_ECC_RAM_CR7LLRAM3:   ulVal = (fDbe ? s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[3] : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[3]); break;
  case RAP_ECC_RAM_CR7LLRAM4:   ulVal = (fDbe ? s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[4] : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[4]); break;
  case RAP_ECC_RAM_CR7LLRAM5:   ulVal = (fDbe ? s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[5] : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[5]); break;
  case RAP_ECC_RAM_CR7LLRAM6:   ulVal = (fDbe ? s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[6] : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[6]); break;
  case RAP_ECC_RAM_CR7LLRAM7:   ulVal = (fDbe ? s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[7] : s_aptRapEccCtrl[0]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[7]); break;

  case RAP_ECC_RAM_INTRAM_RAP0: ulVal = (fDbe ? s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[0] : s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[0]); break;
  case RAP_ECC_RAM_INTRAM_RAP1: ulVal = (fDbe ? s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[1] : s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[1]); break;
  case RAP_ECC_RAM_INTRAM_RAP2: ulVal = (fDbe ? s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[2] : s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[2]); break;
  case RAP_ECC_RAM_INTRAM_RAP3: ulVal = (fDbe ? s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[3] : s_aptRapEccCtrl[1]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[3]); break;

  case RAP_ECC_RAM_INTRAM0:     ulVal = (fDbe ? s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[0] : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[0]); break;
  case RAP_ECC_RAM_INTRAM1:     ulVal = (fDbe ? s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[1] : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[1]); break;
  case RAP_ECC_RAM_INTRAM2:     ulVal = (fDbe ? s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[2] : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[2]); break;
  case RAP_ECC_RAM_INTRAM3:     ulVal = (fDbe ? s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[3] : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[3]); break;
  case RAP_ECC_RAM_INTRAM4:     ulVal = (fDbe ? s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[4] : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[4]); break;
  case RAP_ECC_RAM_INTRAM5:     ulVal = (fDbe ? s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[5] : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[5]); break;
  case RAP_ECC_RAM_INTRAMHS0:   ulVal = (fDbe ? s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[6] : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[6]); break;
  case RAP_ECC_RAM_INTRAMHS1:   ulVal = (fDbe ? s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_DBE[7] : s_aptRapEccCtrl[2]->aulRAP_ECC_CTRL_RAM_ADDR_SBE[7]); break;

  default: return 0; /* ERROR: No address for this RAM available! */

  }
  return ulVal;
}

/*****************************************************************************/
/*! RAP_ECC Get Interrupt
* \description
*   This function gets the current status and interrupt vector of the ECC unit.
*   The returned value is a bit mask with each bit indicating a detected error.
*   There are two status/interrupt values available, one for each
*   error type i.e. single bit errors and double bit errors.
* \class
*   RAP_ECC
* \params
*   fDbe          [in] true/false: double bit error/single bit error
* \return
*   Bit mask indicating the detected errors and pending IRQs                 */
/*****************************************************************************/
uint32_t RAP_ECC_GetIrq( bool fDbe )
{
  uint32_t ulIrq = 0;

  if( !fDbe )
  {
    ulIrq |= s_aptRapEccCtrl[0]->ulRAP_ECC_CTRL_RAM_STATUS_SBE;
    ulIrq |= s_aptRapEccCtrl[1]->ulRAP_ECC_CTRL_RAM_STATUS_SBE << 8;
    ulIrq |= s_aptRapEccCtrl[2]->ulRAP_ECC_CTRL_RAM_STATUS_SBE << 16;
  }
  else
  {
    ulIrq |= s_aptRapEccCtrl[0]->ulRAP_ECC_CTRL_RAM_STATUS_DBE;
    ulIrq |= s_aptRapEccCtrl[1]->ulRAP_ECC_CTRL_RAM_STATUS_DBE << 8;
    ulIrq |= s_aptRapEccCtrl[2]->ulRAP_ECC_CTRL_RAM_STATUS_DBE << 16;
  }

  return ulIrq;
}

/*****************************************************************************/
/*! RAP_ECC Confirm Interrupt
* \description
*   This function resets the current status and confirms the corresponding
*   interrupts.
*   There are two status/interrupt values available, one for each
*   error type i.e. single bit errors and double bit errors.
* \class
*   RAP_ECC
* \params
*   fDbe          [in] true/false: double bit error/single bit error
*   uIrqMsk       [in] Bit mask indicating the errors to reset
* \return
*   Bit mask indicating the detected errors and pending IRQs                 */
/*****************************************************************************/
void     RAP_ECC_ConfirmIrq( uint32_t uIrqMsk, bool fDbe )
{
  if( !fDbe )
  {
    s_aptRapEccCtrl[0]->ulRAP_ECC_CTRL_RAM_STATUS_SBE = uIrqMsk & 0xff;
    s_aptRapEccCtrl[1]->ulRAP_ECC_CTRL_RAM_STATUS_SBE = (uIrqMsk >> 8) & 0xff;
    s_aptRapEccCtrl[2]->ulRAP_ECC_CTRL_RAM_STATUS_SBE = (uIrqMsk >> 16) & 0xff;
  }
  else
  {
    s_aptRapEccCtrl[0]->ulRAP_ECC_CTRL_RAM_STATUS_DBE = uIrqMsk & 0xff;
    s_aptRapEccCtrl[1]->ulRAP_ECC_CTRL_RAM_STATUS_DBE = (uIrqMsk >> 8) & 0xff;
    s_aptRapEccCtrl[2]->ulRAP_ECC_CTRL_RAM_STATUS_DBE = (uIrqMsk >> 16) & 0xff;
  }
}
