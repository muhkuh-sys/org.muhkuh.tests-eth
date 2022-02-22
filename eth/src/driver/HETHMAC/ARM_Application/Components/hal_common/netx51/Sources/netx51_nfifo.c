/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_nfifo.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_NFIFO

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Initialize NFIFO HW
* \description
*   Sets up the NFIFO hardware.
* \class
*   NFIFO
* \params
*   ulBaseCfgAddr  [in]  Address of NFIFO base Configuration
* \return
*                                                                            */
/*****************************************************************************/
void NX51_NFIFO_InitHW( uint32_t ulBaseCfgAddr )
{
  s_ptNFifo->ulNfifo_config = ulBaseCfgAddr;
}

/*****************************************************************************/
/*! Configure NFIFO Channel
* \description
*   Sets up one specific FIFO of the NFIFO block.
* \class
*   NFIFO
* \params
*   ulFifoNum       [in]  FIFO number (0..1013)
*   ulFifoDataAddr  [in]  Base address of FIFO data
*   ulAccessSize    [in]  Access size of FIFO (0: byte, 1: word, 2: DWORD)
*   ulSize          [in]  Size of FIFO (1..8192)
*   ulWatermark     [in]  Water mark level (1..8192)
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_NFIFO_ConfigFifo( uint32_t ulFifoNum,
                           uint32_t ulFifoDataAddr,
                           uint32_t ulAccessSize,
                           uint32_t ulSize,
                           uint32_t ulWatermark )
{
  NX51_NFIFO_FIFO_CONFIG_T* ptCurFifoConfig;

  /* check parameters */
  if (ulFifoNum > 1013) return -1;
  if ((ulSize < 1) || (ulSize > 8192)) return -1;
  if (ulWatermark > 8192) return -1;
  if (ulAccessSize > 2) return -1;

  /* get pointer to current FIFO configuration */
  ptCurFifoConfig = (NX51_NFIFO_FIFO_CONFIG_T*) s_ptNFifo->ulNfifo_config;
  ptCurFifoConfig += ulFifoNum;

  ptCurFifoConfig->ulBaseAddr = (ulFifoDataAddr & NX51_MSK_NFIFO_FIFO_ADDR_ADDRESS) | (ulAccessSize << NX51_SRT_NFIFO_FIFO_ADDR_MAS);
  ptCurFifoConfig->ulConfig = ((ulSize-1) << NX51_SRT_NFIFO_FIFO_CONFIG_SIZE) | ((ulWatermark-1) << NX51_SRT_NFIFO_FIFO_CONFIG_WATERMARK);
  ptCurFifoConfig->ulStatus = NX51_MSK_NFIFO_FIFO_STATUS_EMPTY_WATERMARK | NX51_MSK_NFIFO_FIFO_STATUS_EMPTY;

  return 0;
}

/*****************************************************************************/
/*! Get NFIFO Channel Configuration
* \description
*   Gets the configuration of one specific FIFO of the NFIFO block.
* \class
*   NFIFO
* \params
*   ulFifoNum        [in]   FIFO number (0..1013)
*   pulFifoDataAddr  [out]  Base address of FIFO data
*   pulAccessSize    [out]  Access size of FIFO (0: byte, 1: word, 2: DWORD)
*   pulSize          [out]  Size of FIFO (1..8192)
*   pulWatermark     [out]  Water mark level (1..8192)
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_NFIFO_GetFifoConfig( uint32_t  ulFifoNum,
                              uint32_t* pulFifoDataAddr,
                              uint32_t* pulAccessSize,
                              uint32_t* pulSize,
                              uint32_t* pulWatermark )
{
  NX51_NFIFO_FIFO_CONFIG_T* ptCurFifoConfig;

  /* check parameters */
  if( ulFifoNum > 1013 ) return -1;

  /* get pointer to current FIFO configuration */
  ptCurFifoConfig = (NX51_NFIFO_FIFO_CONFIG_T*) s_ptNFifo->ulNfifo_config;
  ptCurFifoConfig += ulFifoNum;

  *pulFifoDataAddr = ptCurFifoConfig->ulBaseAddr & NX51_MSK_NFIFO_FIFO_ADDR_ADDRESS;
  *pulAccessSize   = (ptCurFifoConfig->ulBaseAddr & NX51_MSK_NFIFO_FIFO_ADDR_MAS) >> NX51_SRT_NFIFO_FIFO_ADDR_MAS;
  *pulSize         = (ptCurFifoConfig->ulConfig & NX51_MSK_NFIFO_FIFO_CONFIG_SIZE)      >> NX51_SRT_NFIFO_FIFO_CONFIG_SIZE;
  *pulWatermark    = (ptCurFifoConfig->ulConfig & NX51_MSK_NFIFO_FIFO_CONFIG_WATERMARK) >> NX51_SRT_NFIFO_FIFO_CONFIG_WATERMARK;

  return 0;
}

/*****************************************************************************/
/*! Write Data to NFIFO Channel
* \description
*   Writes an entry to a NFIFO channel
* \class
*   NFIFO
* \params
*   ulFifoNum       [in]  FIFO number (0..1013)
*   ulData          [in]  Data to write to FIFO
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_NFIFO_WriteFifo( uint32_t ulFifoNum,
                          uint32_t ulData )
{
  /* check parameters */
  if( ulFifoNum > 1013 ) return -1;

  s_ptNFifo->aulNfifo_fifo[ulFifoNum] = ulData;

  return 0;
}

/*****************************************************************************/
/*! Read Data from NFIFO Channel
* \description
*   Reads an entry from a NFIFO channel
* \class
*   NFIFO
* \params
*   ulFifoNum       [in]   FIFO number (0..1013)
*   pulData         [out]  Data from FIFO
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_NFIFO_ReadFifo( uint32_t  ulFifoNum,
                         uint32_t* pulData )
{
  /* check parameters */
  if (ulFifoNum > 1013) return -1;

  *pulData = s_ptNFifo->aulNfifo_fifo[ulFifoNum];

  return 0;
}


/*****************************************************************************/
/*! Get NFIFO Channel Status
* \description
*   Gets the status of one specific FIFO of the NFIFO block.
* \class
*   NFIFO
* \params
*   ulFifoNum        [in]   FIFO number (0..1013)
*   pulStatus        [out]  FIFO status vector
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_NFIFO_GetFifoStatus( uint32_t  ulFifoNum,
                              uint32_t* pulStatus )
{
  NX51_NFIFO_FIFO_CONFIG_T* ptCurFifoConfig;

  /* check parameters */
  if (ulFifoNum > 1013) return -1;

  /* get pointer to current FIFO configuration */
  ptCurFifoConfig = (NX51_NFIFO_FIFO_CONFIG_T*) s_ptNFifo->ulNfifo_config;
  ptCurFifoConfig += ulFifoNum;

  *pulStatus = ptCurFifoConfig->ulStatus;

  return 0;
}
