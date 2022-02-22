
/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_pfifo.h"
#include "hw_defines.h"
#include "compiler_spec.h"

#define PFIFO_INST_CNT (ARRAY_LEN(s_aptPFifo))

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_POINTERFIFO(s_aptPFifo)


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/


/*****************************************************************************/
/*! Reset Pointer Fifo
* \description
*   Resets pointer fifo to default values.
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
* \return
*                                                                            */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(PFIFO_Reset)( unsigned int uInstNo )
{
  unsigned int uCnt;
  
  if( uInstNo < PFIFO_INST_CNT )
  {
    /* set reset flag of all fifos */
    s_aptPFifo[uInstNo]->ulPfifo_reset = 0xffffffff;

    /* reset pointer fifo borders */
    for( uCnt = 0; uCnt < 32; uCnt++ ) {
      s_aptPFifo[uInstNo]->aulPfifo_border[uCnt] = ((uCnt+1)* 100)-1 ;
    }

    /* clear reset flag of all fifos */
    s_aptPFifo[uInstNo]->ulPfifo_reset = 0;
  }
}

/*****************************************************************************/
/*! Set Pointer Fifo Borders
* \description
*   Set pointer fifo borders to given values.
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
*   uFifoNum [in] FIFO number
* \return
*                                                                            */
/*****************************************************************************/
int HALDEF(PFIFO_SetBorders)( unsigned int uInstNo, uint32_t* auPFifoDepth)
{
  int iResult;
  unsigned int uBorderTmp;
  unsigned int uFifoNum;
  
  if( uInstNo >= PFIFO_INST_CNT )
    return -1;

  /* set reset bit for all pointer fifos */
  s_aptPFifo[uInstNo]->ulPfifo_reset = 0xffffffff;

  /* define pointer fifo borders */
  uBorderTmp = 0;
  for(uFifoNum=0; uFifoNum<32; uFifoNum++)
  {
    uBorderTmp += auPFifoDepth[uFifoNum];
    s_aptPFifo[uInstNo]->aulPfifo_border[uFifoNum] = uBorderTmp-1;
  }

  if( uBorderTmp>3200 ) {
    /* sum of all fifo elements exceeds the limit */
    iResult = -1;
  } else {
    /* ok! */
    iResult = 0;

    /* clear reset bit for all pointer fifos */
    s_aptPFifo[uInstNo]->ulPfifo_reset = 0x00000000;
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
*   uInstNo  [in] FIFO instance
*   uFifoNum [in] FIFO number
* \return
*   Current border value
*   0xffffffff on erroneous                                                  */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_GetBorder)( unsigned int uInstNo, unsigned int uFifoNum  )
{
  /* Check instance number */
  if( uInstNo >= PFIFO_INST_CNT )
    return 0xffffffff;
    
  /* set reset flag of all FIFOs */
  if( uFifoNum < 32 )
    return s_aptPFifo[uInstNo]->aulPfifo_border[uFifoNum];
  else 
    return 0xffffffff;
}

/*****************************************************************************/
/*! Get Fifo Fill Level
* \description
*   Returns the fill level of the fifo
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
* \return
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_GetFillLevel)( unsigned int uInstNo, unsigned int uFifoNum )
{
  if( (uFifoNum<32 ) && (uInstNo < PFIFO_INST_CNT) )
    return s_aptPFifo[uInstNo]->aulPfifo_fill_level[uFifoNum];
  else 
    return 0xffffffff;
}

/*****************************************************************************/
/*! Set FIFO Reset Vector
* \description
*   Sets the reset vector of pointer FIFO.
* \class 
*   PFIFO 
* \params
* \return                                                                    */
/*****************************************************************************/
int HALDEF(PFIFO_SetFifoResetVector)( unsigned int uInstNo, unsigned long ulVal )
{
  if( uInstNo < PFIFO_INST_CNT )
    s_aptPFifo[uInstNo]->ulPfifo_reset = ulVal;
  else
    return -1;
  
  return 0;
}

/*****************************************************************************/
/*! Get FIFO Reset Vector
* \description
*   Retrieves the reset vector of pointer fifo.
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
* \return
    reset vector
                                                                             */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_GetFifoResetVector)( unsigned int uInstNo )
{
  if( uInstNo < PFIFO_INST_CNT )
    return s_aptPFifo[uInstNo]->ulPfifo_reset;
  else
    return 0;
}

/*****************************************************************************/
/*! Get FIFO Full Vector
* \description
*   Retrieves the full vector of pointer fifo.
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
* \return
    full vector
                                                                             */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_GetFifoFullVector)( unsigned int uInstNo )
{
  if( uInstNo < PFIFO_INST_CNT )
    return s_aptPFifo[uInstNo]->ulPfifo_full;
  else
    return 0;
}

/*****************************************************************************/
/*! Get FIFO Empty Vector
* \description
*   Retrieves the empty vector of pointer fifo.
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
* \return
    empty vector
                                                                             */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_GetFifoEmptyVector)( unsigned int uInstNo )
{
  if( uInstNo < PFIFO_INST_CNT )
    return s_aptPFifo[uInstNo]->ulPfifo_empty;
  else
    return 0;
}

/*****************************************************************************/
/*! Get FIFO Overflow Vector
* \description
*   Retrieves the overflow vector of pointer fifo.
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
* \return
    overflow vector
                                                                             */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_GetFifoOverflowVector)( unsigned int uInstNo )
{
  if( uInstNo < PFIFO_INST_CNT )
    return s_aptPFifo[uInstNo]->ulPfifo_overflow;
  else
    return 0;
}

/*****************************************************************************/
/*! Get FIFO Underrun Vector
* \description
*   Retrieves the underrun vector of pointer fifo.
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
* \return
    underrun vector
                                                                             */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_GetFifoUnderrunVector)( unsigned int uInstNo )
{
  if( uInstNo < PFIFO_INST_CNT )
    return s_aptPFifo[uInstNo]->ulPfifo_underrun;
  else 
    return 0;
}

/*****************************************************************************/
/*! Read FIFO
* \description
*   Reads a specific PFIFO
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
*   uFifoNum [in] FIFO number
* \return
    Read value
                                                                             */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_ReadFifo)( unsigned int uInstNo, unsigned int uFifoNum )
{
  if( (uFifoNum < 32 ) && (uInstNo < PFIFO_INST_CNT) )
    /* read FIFO */
    return s_aptPFifo[uInstNo]->aulPfifo[uFifoNum];
  else 
    return 0xffffffff;
}

/*****************************************************************************/
/*! Read FIFO
* \description
*   Reads a specific PFIFO
* \class 
*   PFIFO 
* \params
*   uInstNo [in] instance number
*   uFifoNum [in] FIFO number
* \return
    Underrun vector
                                                                             */
/*****************************************************************************/
uint32_t HALDEF(PFIFO_WriteFifo)( unsigned int uInstNo, unsigned int uFifoNum, uint32_t ulVal )
{
  if( (uFifoNum < 32 ) && (uInstNo < PFIFO_INST_CNT) )
  {
    /* write FIFO */
    s_aptPFifo[uInstNo]->aulPfifo[uFifoNum] = ulVal;
    return 0;
  }
  else {
    return 0xffffffff;
  }
}
