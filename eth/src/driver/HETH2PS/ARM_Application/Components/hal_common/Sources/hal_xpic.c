
/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_xpic.h"
#include "hw_defines.h"
#include <string.h> /* definition of NULL */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define XPIC_SEGDEF_HEADER_MAGIC              0x43495058
#define XPIC_SEGDEF_HEADER_MSK_VERSION_MAJOR  0xffff0000
#define XPIC_SEGDEF_HEADER_MSK_VERSION_MINOR  0x0000ffff
#define XPIC_SEGDEF_HEADER_VERSION_MAJOR      0x00010000
#define XPIC_SEGDEF_HEADER_VERSION_MINOR      0x00000000

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_XPIC(s_aptXpic)
HW_PTR_XPIC_DEBUG(s_aptXpicDebug)
HW_PTR_XPIC_DRAM(s_apulXpicDram)
HW_PTR_XPIC_PRAM(s_apulXpicPram)


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Load data/code segments
* \description
*   Load dword aligned data into memory
*   Can be used for xPIC program or any other areas
* \class
*   XPIC
* \params
*   pulData [in]  Pointer to microcode
*                                                                            */
/*****************************************************************************/

static void XPIC_LoadSegments( unsigned int uiXpic, const uint32_t *pulData )
{
  const uint32_t *pulCur = &pulData[2];
  uint32_t* pulDest;
  uint32_t ulLen;

  /* check header magic */
  if( *pulCur++ != XPIC_SEGDEF_HEADER_MAGIC ) {
    /* invalid header */
    return;
  }
  /* check major version */
  if( (*pulCur++ & XPIC_SEGDEF_HEADER_MSK_VERSION_MAJOR) > XPIC_SEGDEF_HEADER_VERSION_MAJOR ) {
    /* version not supported */
    return;
  }

  pulDest = (uint32_t*)*pulCur++;
  ulLen = *pulCur++;
  while( ulLen ) {
    /* load segment */
    while( ulLen-- ) {
      *pulDest++ = *pulCur++;
    }
    /* read next segment */
    pulDest = (uint32_t*)*pulCur++;
    ulLen = *pulCur++;
  }
}

/*****************************************************************************/
/*! Load xPIC code
* \description
*   Load xPIC code,
*   Initialize trailing loads
* \class
*   XPIC
* \params
*   xpic_code [in]  Pointer to microcode
*   pvUser    [in] ignored
*                                                                            */
/*****************************************************************************/
void XPIC_Load( unsigned int uiXpic, const uint32_t *xpic_code, void* pvUser )
{
  unsigned int programm_size;
  unsigned int tl_size;
  unsigned int programm_start_adr;
  unsigned int i;
  unsigned int tl_start;
  uint32_t* pulData;

  if( ( xpic_code[0] == 0 ) && ( xpic_code[1] == 0 ) ) {
    /* xPIC GNU Toolchain - segment definitions */
    XPIC_LoadSegments(uiXpic, xpic_code);
  } else {
    /* xpicass generated program and trailing loads */
    programm_size       = xpic_code[0] / 4;  /* in Bytes / 4 */
    tl_size             = xpic_code[1] / 4;
    programm_start_adr  = xpic_code[2];
    tl_start            = programm_size + 2;

    /* program loader */
    for (i = 1; i < programm_size; i++){
      pulData = (uint32_t*) (programm_start_adr + (i - 1)*4);
      *pulData = xpic_code[i + 2];
    }

    /* trailing loader */
    for (i = 0; i < tl_size; i = i + 2){
      pulData = (uint32_t*) (xpic_code[tl_start + i]);
      *pulData = xpic_code[tl_start + i + 1];
    }
  }
}


/*****************************************************************************/
/*! Start xPIC
* \description
*   Start xPIC
* \class
*   XPIC
* \params
*   pvUser [in] ignored
*                                                                            */
/*****************************************************************************/
void XPIC_Start( unsigned int uiXpic, void* pvUser )
{
  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = 0;
}

/*****************************************************************************/
/*! Stop xPIC
* \description
*   Halt xPIC
* \class
*   XPIC
* \params
*   pvUser [in] ignored
*                                                                            */
/*****************************************************************************/
void XPIC_Stop( unsigned int uiXpic, void* pvUser )
{
  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = HW_MSK(xpic_hold_pc_hold);
}

/*****************************************************************************/
/*! Reset xPIC
* \description
*   Hold xPIC, reset xPIC state,
*   clear work registers (both banks), clear user registers
* \class
*   XPIC
* \params
*   pvUser [in] ignored
*
*                                                                            */
/*****************************************************************************/
void XPIC_Reset( unsigned int uiXpic, void* pvUser )
{
  uint32_t ulVal;

  /* hold xPIC */
  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = HW_MSK(xpic_hold_pc_hold);

  /* clear all hold reasons */
  s_aptXpicDebug[uiXpic]->ulXpic_break_irq_raw = 0x1f;

  /* hold xPIC and request reset */
  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = HW_MSK(xpic_hold_pc_hold)
                                           | HW_MSK(xpic_hold_pc_reset_xpic);

  /* wait for reset to be finished */
  do
  {
    ulVal = s_aptXpicDebug[uiXpic]->ulXpic_break_status;
  } while( (ulVal & HW_MSK(xpic_break_status_xpic_reset_status)) == 0 );

  /* release reset request, engage bank control, select bank 1 */
  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = HW_MSK(xpic_hold_pc_hold)
                                           | HW_MSK(xpic_hold_pc_bank_control)
                                           | HW_MSK(xpic_hold_pc_bank_select);

  /* reset xPIC user registers */
  for( ulVal = 0; ulVal < 5; ulVal++ )
  {
    s_aptXpic[uiXpic]->aulXpic_usr[ulVal] = 0;
  }

  /* reset xPIC work registers (bank 1) */
  for( ulVal = 0; ulVal < 8; ulVal++ )
  {
    s_aptXpic[uiXpic]->aulXpic_r[ulVal] = 0;
  }

  /* select bank 0 */
  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = HW_MSK(xpic_hold_pc_hold)
                                           | HW_MSK(xpic_hold_pc_bank_control);

  /* reset xPIC work registers (bank 0) */
  for( ulVal = 0; ulVal < 8; ulVal++ )
  {
    s_aptXpic[uiXpic]->aulXpic_r[ulVal] = 0;
  }

  /* release bank control */
  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = HW_MSK(xpic_hold_pc_hold);
}
/*****************************************************************************/
/*! Get Previous Program Counter
* \description
*  Read the xPIC last program counter register,
*  i. e. the program counter of the last executed instruction.
* \class
*   XPIC
* \params
* \return
*   raw register value                                                       */
/*****************************************************************************/
uint32_t XPIC_GetLastPc( unsigned int uiXpic )
{
  return s_aptXpicDebug[uiXpic]->ulXpic_break_last_pc;
}

/*****************************************************************************/
/*! Get xPIC Break Status
* \description
*  Read the xPIC break status register.
* \class
*   XPIC
* \params
* \return
*   raw register value                                                       */
/*****************************************************************************/
uint32_t XPIC_GetBreakStatus( unsigned int uiXpic )
{
  return s_aptXpicDebug[uiXpic]->ulXpic_break_status;
}

/*****************************************************************************/
/*! Get xPIC Raw Break Interrupt
* \description
*  Read the xPIC raw break interrupt.
* \class
*   XPIC
* \params
* \return
*   raw register value                                                       */
/*****************************************************************************/
uint32_t XPIC_GetBreakIrqRaw( unsigned int uiXpic )
{
  return s_aptXpicDebug[uiXpic]->ulXpic_break_irq_raw;
}

/*****************************************************************************/
/*! Confirm xPIC Break Interrupt
* \description
*  Clear the xPIC break interrupt.
* \class
*   XPIC
* \params
*   ulIrq [in] interrupt mask                                                */
/*****************************************************************************/
void XPIC_ConfirmBreakIrq( unsigned int uiXpic, uint32_t ulIrq )
{
  s_aptXpicDebug[uiXpic]->ulXpic_break_irq_raw = ulIrq;
}

/*****************************************************************************/
/*! Get xPIC Masked Break Interrupt
* \description
*  Read the xPIC masked break interrupt.
* \class
*   XPIC
* \params
* \return
*   raw register value                                                       */
/*****************************************************************************/
uint32_t XPIC_GetBreakIrq( unsigned int uiXpic, bool fOwn )
{
  return (fOwn) ? s_aptXpicDebug[uiXpic]->ulXpic_break_own_irq_masked : s_aptXpicDebug[uiXpic]->ulXpic_break_irq_masked;
}

/*****************************************************************************/
/*! Enable xPIC Break Interrupt
* \description
*  Set bits in the xPIC break interrupt mask register.
* \class
*   XPIC
* \params
*   ulIrq [in] interrupt enable mask                                         */
/*****************************************************************************/
void XPIC_EnableBreakIrq( unsigned int uiXpic, uint32_t ulIrq, bool fOwn )
{
  if( fOwn )
    s_aptXpicDebug[uiXpic]->ulXpic_break_own_irq_msk_set = ulIrq;
  else
    s_aptXpicDebug[uiXpic]->ulXpic_break_irq_msk_set = ulIrq;
}

/*****************************************************************************/
/*! Disable xPIC Break Interrupt
* \description
*  Clear bits in the xPIC break interrupt mask register.
* \class
*   XPIC
* \params
*   ulIrq [in] interrupt disable mask                                        */
/*****************************************************************************/
void XPIC_DisableBreakIrq( unsigned int uiXpic, uint32_t ulIrq, bool fOwn )
{
  if( fOwn )
    s_aptXpicDebug[uiXpic]->ulXpic_break_own_irq_msk_reset = ulIrq;
  else
    s_aptXpicDebug[uiXpic]->ulXpic_break_irq_msk_reset = ulIrq;
}

/*****************************************************************************/
/*! Set up xPIC Hardware Watch Point
* \description
*  Setup a watch point for the xPIC.
* \class
*   XPIC
* \params
*   uiWpNo [in] watch point number
*   ptWp   [in] watch point configuration                                    */
/*****************************************************************************/
void XPIC_SetupWatchPoint( unsigned int uiXpic, unsigned int uiWpNo, XPIC_WATCHPOINT_T* ptWp )
{
  if( ptWp == NULL )
  {
    /* disable watch point */
    s_aptXpicDebug[uiXpic]->asXpic_break[uiWpNo].ulContr = 0;
  }
  else
  {
    s_aptXpicDebug[uiXpic]->asXpic_break[uiWpNo].ulAddr       = ptWp->ulAddrValue;
    s_aptXpicDebug[uiXpic]->asXpic_break[uiWpNo].ulAddr_mask  = ptWp->ulAddrDcMsk;
    s_aptXpicDebug[uiXpic]->asXpic_break[uiWpNo].ulData       = ptWp->ulDataValue;
    s_aptXpicDebug[uiXpic]->asXpic_break[uiWpNo].ulData_mask  = ptWp->ulDataDcMsk;
    s_aptXpicDebug[uiXpic]->asXpic_break[uiWpNo].ulContr      = ptWp->ulCtrlValue;
    s_aptXpicDebug[uiXpic]->asXpic_break[uiWpNo].ulContr_mask = ptWp->ulCtrlDcMsk;
  }
}

/*****************************************************************************/
/*! Start xPIC Extended
* \description
*  Extended version of the start function.
* \class
*   XPIC
* \params
*   fSingleStep   [in] enable single stepping
*   fMonitorMode  [in] enable monitor mode (don't halt on watchpoints, just set IRQs
*   fIntDis       [in] disable xPIC interrupts (IRQs and FIQs)
*   fMisalignHold [in] halt xPIC on misaligned memory accesses
*   pvUser        [in] user defined parameter                                */
/*****************************************************************************/
void XPIC_StartEx( unsigned int uiXpic, bool fSingleStep, bool fMonitorMode, bool fIntDis, bool fMisalignHold, void* pvUser )
{
  uint32_t ulVal = 0;

  if( fSingleStep )
    ulVal |= HW_MSK(xpic_hold_pc_single_step);

  if( fMonitorMode )
    ulVal |= HW_MSK(xpic_hold_pc_monitor_mode);

  if( fIntDis )
    ulVal |= HW_MSK(xpic_hold_pc_disable_int);

  if( fMisalignHold )
    ulVal |= HW_MSK(xpic_hold_pc_misalignment_hold);

  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = ulVal;
}


/*****************************************************************************/
/*! Select xPIC Rgister Bank
* \description
*  Selects the register bank for all following register read/write accesses.
* \class
*   XPIC
* \params
*   fSel     [in] false: Use current active register bank, bank select is not controlled.
*                 true: Use register bank as defined in fFiqBank
*   fFiqBank [in] false: Use default/IRQ register bank
*                 true: Use FIQ register bank                                */
/*****************************************************************************/
void XPIC_SetBankSelect( unsigned int uiXpic, bool fSel, bool fFiqBank )
{
  uint32_t ulValue = 0;

  if( fSel )
  {
    ulValue |= HW_MSK(xpic_hold_pc_bank_control);

    if( fFiqBank )
    {
      ulValue |= HW_MSK(xpic_hold_pc_bank_select);
    }
  }

  s_aptXpicDebug[uiXpic]->ulXpic_hold_pc = ulValue;
}


/*****************************************************************************/
/*! Read xPIC Rgister
* \description
*  Reads a processor register of the xPIC.
* \class
*   XPIC
* \params
*   eReg     [in] Register to read                                           */
/*****************************************************************************/
uint32_t XPIC_GetReg( unsigned int uiXpic, XPIC_REG_E eReg )
{
  return s_aptXpic[uiXpic]->aulXpic_r[eReg];
}


/*****************************************************************************/
/*! Write xPIC Rgister
* \description
*  Writes a processor register of the xPIC.
* \class
*   XPIC
* \params
*   eReg     [in] Register to write
*   ulValue  [in] Register value                                             */
/*****************************************************************************/
void XPIC_SetReg( unsigned int uiXpic, XPIC_REG_E eReg, uint32_t ulValue )
{
  s_aptXpic[uiXpic]->aulXpic_r[eReg] = ulValue;
}


/*****************************************************************************/
/*! Get xPIC Interrupt status
* \description
*  Reads the interrupt status register of the xPIC.
* \class
*   XPIC                                                                     */
/*****************************************************************************/
uint32_t XPIC_GetIrqStatus( unsigned int uiXpic )
{
  return s_aptXpicDebug[uiXpic]->ulXpic_irq_status;
}


/*****************************************************************************/
/*! Get xPIC IRQ Return Program Counter
* \description
*  Reads the last program counter before jump into IRQ.
* \class
*   XPIC                                                                     */
/*****************************************************************************/
uint32_t XPIC_GetIrqReturnPc( unsigned int uiXpic )
{
  return s_aptXpicDebug[uiXpic]->ulXpic_break_return_irq_pc;
}


/*****************************************************************************/
/*! Get xPIC FIQ Return Program Counter
* \description
*  Reads the last program counter before jump into FIQ.
* \class
*   XPIC                                                                     */
/*****************************************************************************/
uint32_t XPIC_GetFiqReturnPc( unsigned int uiXpic )
{
  return s_aptXpicDebug[uiXpic]->ulXpic_break_return_fiq_pc;
}


/*****************************************************************************/
/*! Write xPIC DRAM DWord wise
* \description
*   Write xPIC DRAM DW-wise.
* \class
*   XPIC
* \params
*   uOffset              [in]  DRAM Offset
*   ulValue              [in]  Value                                         */
/*****************************************************************************/
void XPIC_WrDramDw( unsigned int uiXpic, unsigned int uiOffset, uint32_t ulValue )
{
  s_apulXpicDram[uiXpic][uiOffset] = ulValue;
}


/*****************************************************************************/
/*! Read xPIC DRAM DWord wise
* \description
*   Read xPIC DRAM DW-wise.
* \class
*   XPIC
* \params
*   uOffset              [in]  DRAM Offset
* \return
*   Read value                                                               */
/*****************************************************************************/
uint32_t XPIC_RdDramDw( unsigned int uiXpic, unsigned int uiOffset )
{
  return s_apulXpicDram[uiXpic][uiOffset];
}


/*****************************************************************************/
/*! Write xPIC PRAM DWord wise
* \description
*   Write xPIC PRAM DW-wise.
* \class
*   XPIC
* \params
*   uOffset              [in]  PRAM Offset
*   ulValue              [in]  Value                                         */
/*****************************************************************************/
void XPIC_WrPramDw( unsigned int uiXpic, unsigned int uiOffset, uint32_t ulValue )
{
  s_apulXpicPram[uiXpic][uiOffset] = ulValue;
}


/*****************************************************************************/
/*! Read xPIC PRAM DWord wise
* \description
*   Read xPIC PRAM DW-wise.
* \class
*   XPIC
* \params
*   uOffset              [in]  PRAM Offset
* \return
*   Read value                                                               */
/*****************************************************************************/
uint32_t XPIC_RdPramDw( unsigned int uiXpic, unsigned int uiOffset )
{
  return s_apulXpicPram[uiXpic][uiOffset];
}
