/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_pfifo.h"
#include "hal_resources_defines_netx6.h"

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
void NX6_PFIFO_Reset( void )
{
  unsigned int uCnt;

  /* set reset flag of all FIFOs */
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, 0xffffffff);

  /* reset pointer FIFO borders */
  for( uCnt = 0; uCnt < 32; uCnt++ ) {
    NX_WRITE32(s_ptPFifo->aulPfifo_border[uCnt], (((uint32_t)uCnt+1)* 100)-1);
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
*   auiPFifoDepth  [in] Array of 32 Elements containing the depth of each FIFO
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_PFIFO_SetBorders(const unsigned int* auiPFifoDepth)
{
  int iResult;
  unsigned int uiBorder;
  unsigned int uiFifoNum;

  /* set reset bit for all pointer FIFOs */
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, 0xffffffff);

  /* define pointer FIFO borders */
  uiBorder = 0;
  for(uiFifoNum=0; uiFifoNum < 32; uiFifoNum++)
  {
    uiBorder += auiPFifoDepth[uiFifoNum];
    NX_WRITE32(s_ptPFifo->aulPfifo_border[uiFifoNum], uiBorder - 1);
  }

  if( uiBorder > 3200 ) {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  } else {
    /* Okay! */
    iResult = 0;

    /* clear reset bit for all pointer FIFOs */
    NX_WRITE32(s_ptPFifo->ulPfifo_reset, 0x00000000);
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
*   auiPFifoDepth  [out] Array of 32 Elements containing the depth of each FIFO
* \return
*                                                                            */
/*****************************************************************************/
int NX6_PFIFO_GetBorders(unsigned int *auiPFifoDepth)
{
  int iResult;
  unsigned int uiBorder, uiBorderPrev;
  unsigned int uiFifoNum;

  /* read pointer FIFO borders */
  uiBorderPrev = 0;
  for(uiFifoNum = 0; uiFifoNum < 32; uiFifoNum++)
  {
    uiBorder = NX_READ32(s_ptPFifo->aulPfifo_border[uiFifoNum]) + 1;
    auiPFifoDepth[uiFifoNum] = uiBorder - uiBorderPrev;
    uiBorderPrev = uiBorder;
  }

  if( uiBorder > 3200 ) {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  } else {
    /* OK! */
    iResult = 0;
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
uint32_t NX6_PFIFO_GetFillLevel( unsigned int uFifoNum )
{
  if( uFifoNum<32 )
    return NX_READ32(s_ptPFifo->aulPfifo_fill_level[uFifoNum]);
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
uint32_t NX6_PFIFO_GetFifoResetVector( void )
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
    Full vector
                                                                             */
/*****************************************************************************/
uint32_t NX6_PFIFO_GetFifoFullVector( void )
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
    Empty vector
                                                                             */
/*****************************************************************************/
uint32_t NX6_PFIFO_GetFifoEmptyVector( void )
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
    Overflow vector
                                                                             */
/*****************************************************************************/
uint32_t NX6_PFIFO_GetFifoOverflowVector( void )
{
  return NX_READ32(s_ptPFifo->ulPfifo_overflow);
}

/*****************************************************************************/
/*! Get FIFO under-run Vector
* \description
*   Retrieves the under-run vector of pointer FIFO.
* \class
*   PFIFO
* \params
* \return
    Under-run vector
                                                                             */
/*****************************************************************************/
uint32_t NX6_PFIFO_GetFifoUnderrunVector( void )
{
  return NX_READ32(s_ptPFifo->ulPfifo_underrun);
}
