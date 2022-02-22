/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_pfifo.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_POINTERFIFO

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Reset Pointer FIFO
* \description
*   Resets pointer FIFO to default values.
* \class
*   PFIFO
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX51_PFIFO_Reset( void )
{
  unsigned int uCnt;

  /* set reset flag of all FIFOs */
  s_ptPFifo->ulPfifo_reset = 0xffffffff;

  /* reset pointer FIFO borders */
  for( uCnt = 0; uCnt < 32; uCnt++ ) {
    s_ptPFifo->aulPfifo_border[uCnt] = ((uCnt+1)* 100)-1 ;
  }

  /* clear reset flag of all FIFOs */
  s_ptPFifo->ulPfifo_reset = 0;
}

/*****************************************************************************/
/*! Set Pointer FIFO Borders
* \description
*   Set pointer FIFO borders to given values.
* \class
*   PFIFO
* \params
*   aulPFifoDepth [in] PFifo depth area
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_PFIFO_SetBorders( uint32_t *aulPFifoDepth )
{
  int iResult;
  uint32_t ulBorderTmp;
  unsigned int uFifoNum;

  /* set reset bit for all pointer FIFOs */
  s_ptPFifo->ulPfifo_reset = 0xffffffff;

  /* define pointer FIFO borders */
  ulBorderTmp = 0;
  for(uFifoNum=0; uFifoNum<32; uFifoNum++)
  {
    ulBorderTmp += aulPFifoDepth[uFifoNum];
    s_ptPFifo->aulPfifo_border[uFifoNum] = ulBorderTmp-1;
  }

  if( ulBorderTmp > 3200 )
  {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  }
  else
  {
    /* OK! */
    iResult = 0;

    /* clear reset bit for all pointer FIFOs */
    s_ptPFifo->ulPfifo_reset = 0x00000000;
  }

  return iResult;
}

/*****************************************************************************/
/*! Get FIFO Fill Level
* \description
*   Returns the fill level of the FIFO
* \class
*   PFIFO
* \params
*   uFifoNum [in] FIFO number
* \return
*   Current pfifo_fill_level value
*   0xffffffff on erroneous                                                  */
/*****************************************************************************/
uint32_t NX51_PFIFO_GetFillLevel( unsigned int uFifoNum )
{
  /* set reset flag of all FIFOs */
  if( uFifoNum<32 )
    return s_ptPFifo->aulPfifo_fill_level[uFifoNum];
  else
    return 0xffffffff;
}

/*****************************************************************************/
/*! Get FIFO Reset Vector
* \description
*   Retrieves the reset vector of pointer FIFO.
* \class
*   PFIFO
* \params
* \return
    Reset vector
                                                                             */
/*****************************************************************************/
uint32_t NX51_PFIFO_GetFifoResetVector( void )
{
  return s_ptPFifo->ulPfifo_reset;
}

/*****************************************************************************/
/*! Get FIFO Full Vector
* \description
*   Retrieves the full vector of pointer FIFO.
* \class
*   PFIFO
* \params
* \return
    Full vector
                                                                             */
/*****************************************************************************/
uint32_t NX51_PFIFO_GetFifoFullVector( void )
{
  return s_ptPFifo->ulPfifo_full;
}

/*****************************************************************************/
/*! Get FIFO Empty Vector
* \description
*   Retrieves the empty vector of pointer FIFO.
* \class
*   PFIFO
* \params
* \return
    Empty vector
                                                                             */
/*****************************************************************************/
uint32_t NX51_PFIFO_GetFifoEmptyVector( void )
{
  return s_ptPFifo->ulPfifo_empty;
}

/*****************************************************************************/
/*! Get FIFO Overflow Vector
* \description
*   Retrieves the overflow vector of pointer FIFO.
* \class
*   PFIFO
* \params
* \return
    Overflow vector
                                                                             */
/*****************************************************************************/
uint32_t NX51_PFIFO_GetFifoOverflowVector( void )
{
  return s_ptPFifo->ulPfifo_overflow;
}

/*****************************************************************************/
/*! Get FIFO Underrun Vector
* \description
*   Retrieves the underrun vector of pointer FIFO.
* \class
*   PFIFO
* \params
* \return
    Underrun vector
                                                                             */
/*****************************************************************************/
uint32_t NX51_PFIFO_GetFifoUnderrunVector( void )
{
  return s_ptPFifo->ulPfifo_underrun;
}
