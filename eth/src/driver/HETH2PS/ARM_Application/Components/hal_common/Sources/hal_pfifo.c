#include "hal_pfifo.h"
#include "hw_defines.h"
#include "compiler_spec.h"

#define PFIFO_INST_CNT (ARRAY_LEN(s_aptPFifo))
#define PFIFO_FIFO_CNT 32

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_POINTERFIFO(s_aptPFifo)


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/


/*****************************************************************************/
/*! Reset Pointer FIFO
* \description
*   Resets Pointer FIFO to default values.
* \class
*   PFIFO
* \params
*   uiInst [in] PFIFO instance
* \return
*                                                                            */
/*****************************************************************************/
void PFIFO_Reset( unsigned int uiInst )
{
  unsigned int uiCnt;

  if( uiInst >= PFIFO_INST_CNT )
    return;

  /* set reset flag of all FIFOs */
  s_aptPFifo[uiInst]->ulPfifo_reset = 0xffffffff;

  /* reset pointer FIFO borders */
  for( uiCnt = 0; uiCnt < PFIFO_FIFO_CNT; uiCnt++ )
  {
    s_aptPFifo[uiInst]->aulPfifo_border[uiCnt] = ((uiCnt + 1) * 100) - 1;
  }

  /* clear reset flag of all FIFOs */
  s_aptPFifo[uiInst]->ulPfifo_reset = 0;
}

/*****************************************************************************/
/*! Set Pointer FIFO Borders
* \description
*   Set pointer FIFO borders to given values.
* \class
*   PFIFO
* \params
*   uiInst         [in]  PFIFO instance
*   auiPFifoDepth  [in]  Array of 32 Elements containing the depth of each FIFO
* \return
*                                                                            */
/*****************************************************************************/
int PFIFO_SetBorders(unsigned int uiInst, const unsigned int* auiPFifoDepth)
{
  int iResult = 0;
  unsigned int uiBorder;
  unsigned int uiFifoNum;

  if( uiInst >= PFIFO_INST_CNT )
    return -1;

  /* set reset bit for all pointer FIFOs */
  s_aptPFifo[uiInst]->ulPfifo_reset = 0xffffffff;

  /* define pointer FIFO borders */
  uiBorder = 0;
  for(uiFifoNum=0; uiFifoNum < PFIFO_FIFO_CNT; uiFifoNum++)
  {
    uiBorder += auiPFifoDepth[uiFifoNum];
    s_aptPFifo[uiInst]->aulPfifo_border[uiFifoNum] = uiBorder-1;
  }

  if( uiBorder > 3200 )
  {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  }
  else
  {
    /* clear reset bit for all pointer FIFOs */
    s_aptPFifo[uiInst]->ulPfifo_reset = 0x00000000;
  }

  return iResult;
}


/*****************************************************************************/
/*! Get Pointer FIFO Borders
* \description
*   Get pointer FIFO depth.
* \class
*   PFIFO
* \params
*   uiInst         [in]  PFIFO instance
*   auiPFifoDepth  [out] Array of 32 Elements containing the depth of each FIFO
* \return
*                                                                            */
/*****************************************************************************/
int PFIFO_GetBorders(unsigned int uiInst, unsigned int *auiPFifoDepth)
{
  int iResult = 0;
  unsigned int uiBorder, uiBorderPrev;
  unsigned int uiFifoNum;

  /* read pointer FIFO borders */
  uiBorderPrev = 0;
  for(uiFifoNum = 0; uiFifoNum < PFIFO_FIFO_CNT; uiFifoNum++)
  {
    uiBorder = s_aptPFifo[uiInst]->aulPfifo_border[uiFifoNum] + 1;
    auiPFifoDepth[uiFifoNum] = uiBorder - uiBorderPrev;
    uiBorderPrev = uiBorder;
  }

  if( uiBorder > 3200 )
  {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  }

  return iResult;
}

/*****************************************************************************/
/*! Get FIFO Border
* \description
*   Returns the border of the FIFO
* \class
*   PFIFO
* \params
*   uiInst    [in] PFIFO instance
*   uiFifoNum [in] FIFO number
* \return
*   Current border value
*   0xffffffff on erroneous                                                  */
/*****************************************************************************/
uint32_t PFIFO_GetBorder( unsigned int uiInst, unsigned int uiFifoNum  )
{
  if( (uiFifoNum >= PFIFO_FIFO_CNT ) || (uiInst >= PFIFO_INST_CNT) )
  {
    return 0xffffffff;
  }

  return s_aptPFifo[uiInst]->aulPfifo_border[uiFifoNum];
}

/*****************************************************************************/
/*! Get FIFO Fill Level
* \description
*   Returns the fill level of the FIFO
* \class
*   PFIFO
* \params
*   uiInst    [in] PFIFO instance
*   uiFifoNum [in] FIFO number
* \return
*                                                                            */
/*****************************************************************************/
uint32_t PFIFO_GetFillLevel( unsigned int uiInst, unsigned int uiFifoNum )
{
  if( (uiFifoNum >= PFIFO_FIFO_CNT ) || (uiInst >= PFIFO_INST_CNT) )
  {
    return 0xffffffff;
  }

  return s_aptPFifo[uiInst]->aulPfifo_fill_level[uiFifoNum];
}

/*****************************************************************************/
/*! Set FIFO Reset Vector
* \description
*   Sets the reset vector of pointer FIFO.
* \class
*   PFIFO
* \params
*   uiInst    [in] PFIFO instance
*   ulVal     [in] Bit field defining which FIFOs to reset
* \return                                                                    */
/*****************************************************************************/
int PFIFO_SetFifoResetVector( unsigned int uiInst, unsigned long ulVal )
{
  if( uiInst >= PFIFO_INST_CNT )
  {
    return -1;
  }

  s_aptPFifo[uiInst]->ulPfifo_reset = ulVal;

  return 0;
}

/*****************************************************************************/
/*! Get FIFO Reset Vector
* \description
*   Retrieves the reset vector of pointer FIFO.
* \class
*   PFIFO
* \params
*   uiInst  [in]  PFIFO instance
* \return
    reset vector
                                                                             */
/*****************************************************************************/
uint32_t PFIFO_GetFifoResetVector( unsigned int uiInst )
{
  if( uiInst >= PFIFO_INST_CNT )
  {
    return 0;
  }

  return s_aptPFifo[uiInst]->ulPfifo_reset;
}

/*****************************************************************************/
/*! Get FIFO Full Vector
* \description
*   Retrieves the full vector of pointer FIFO.
* \class
*   PFIFO
* \params
*   uiInst  [in]  PFIFO instance
* \return
    full vector
                                                                             */
/*****************************************************************************/
uint32_t PFIFO_GetFifoFullVector( unsigned int uiInst )
{
  if( uiInst >= PFIFO_INST_CNT )
  {
    return 0;
  }

  return s_aptPFifo[uiInst]->ulPfifo_full;
}

/*****************************************************************************/
/*! Get FIFO Empty Vector
* \description
*   Retrieves the empty vector of pointer FIFO.
* \class
*   PFIFO
* \params
*   uiInst  [in]  PFIFO instance
* \return
    empty vector
                                                                             */
/*****************************************************************************/
uint32_t PFIFO_GetFifoEmptyVector( unsigned int uiInst )
{
  if( uiInst >= PFIFO_INST_CNT )
  {
    return 0;
  }

  return s_aptPFifo[uiInst]->ulPfifo_empty;
}

/*****************************************************************************/
/*! Get FIFO Overflow Vector
* \description
*   Retrieves the overflow vector of pointer FIFO.
* \class
*   PFIFO
* \params
*   uiInst  [in]  PFIFO instance
* \return
    overflow vector
                                                                             */
/*****************************************************************************/
uint32_t PFIFO_GetFifoOverflowVector( unsigned int uiInst )
{
  if( uiInst >= PFIFO_INST_CNT )
  {
    return 0;
  }

  return s_aptPFifo[uiInst]->ulPfifo_overflow;
}

/*****************************************************************************/
/*! Get FIFO Underrun Vector
* \description
*   Retrieves the underrun vector of pointer FIFO.
* \class
*   PFIFO
* \params
*   uiInst  [in]  PFIFO instance
* \return
    underrun vector
                                                                             */
/*****************************************************************************/
uint32_t PFIFO_GetFifoUnderrunVector( unsigned int uiInst )
{
  if( uiInst >= PFIFO_INST_CNT )
  {
    return 0;
  }

  return s_aptPFifo[uiInst]->ulPfifo_underrun;
}

/*****************************************************************************/
/*! Read FIFO
* \description
*   Reads a specific PFIFO
* \class
*   PFIFO
* \params
*   uiInst     [in]  PFIFO instance
*   uiFifoNum  [in]  FIFO number
* \return
    Read value
                                                                             */
/*****************************************************************************/
uint32_t PFIFO_ReadFifo( unsigned int uiInst, unsigned int uiFifoNum )
{
  if( (uiFifoNum >= PFIFO_FIFO_CNT ) || (uiInst >= PFIFO_INST_CNT) )
  {
    return 0xffffffff;
  }

  /* read FIFO */
  return s_aptPFifo[uiInst]->aulPfifo[uiFifoNum];
}

/*****************************************************************************/
/*! Read FIFO
* \description
*   Reads a specific PFIFO
* \class
*   PFIFO
* \params
*   uiInst    [in]  PFIFO instance
*   uFifoNum  [in]  FIFO number
* \return
    Underrun vector
                                                                             */
/*****************************************************************************/
uint32_t PFIFO_WriteFifo( unsigned int uiInst, unsigned int uiFifoNum, uint32_t ulVal )
{
  if( (uiFifoNum >= PFIFO_FIFO_CNT ) || (uiInst >= PFIFO_INST_CNT) )
  {
    return 0xffffffff;
  }

  /* write FIFO */
  s_aptPFifo[uiInst]->aulPfifo[uiFifoNum] = ulVal;

  return 0;
}
