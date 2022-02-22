/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_spi_motion.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_SPI_MOTION

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! SPIMOTION Initialize
* \description
*   This function initializes the SPI Motion interface.
* \class
*   SPIMOTION Service Class
* \params
*   eModeSelect  [in] 1/0: Slave/Master mode
*   eSckMulAdd  [in] Speed f_spi_sck = (sck_muladd * 100)/4096 [MHz].
*   eSph        [in] Serial clock phase
*   eSpo        [in] Serial clock polarity
*   uDataSize   [in] Data size in bits
*   eCsMode     [in] 1/0: CS is toggled manually/automatically
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_SPIMOTION_Init ( NX51_SPIMOTION_MODE_SELECT      eModeSelect,
                          NX51_SPIMOTION_SCLK_FREQ        eSckMulAdd,
                          NX51_SPIMOTION_SERIAL_CLK_PHASE eSph,
                          NX51_SPIMOTION_SERIAL_CLK_POL   eSpo,
                          unsigned int                    uDataSize,
                          NX51_SPIMOTION_CS_MODE          eCsMode )
{
  /* Plausibility check */
  if( eSckMulAdd > 0x800 )
    return -1;   /* speed > 50 MHz is not supported */

  uDataSize--;
  if( ( uDataSize < 0x3 ) || ( uDataSize > 0xf ) )
    return -1;

  /* configure SPI interface */
  s_ptSpiMotion->aulSpi_cr[0] =  ((uint32_t)eSckMulAdd << SRT_NX51_spi_cr0_sck_muladd)
                                |((uint32_t)uDataSize  << SRT_NX51_spi_cr0_datasize)
                                |((uint32_t)eSph       << SRT_NX51_spi_cr0_SPH)
                                |((uint32_t)eSpo       << SRT_NX51_spi_cr0_SPO);

  /* enable interface, clear receive and transmit FIFOs and configure to default FIFO water mark generation */
  s_ptSpiMotion->aulSpi_cr[1] = MSK_NX51_spi_cr1_rx_fifo_clr
                               |((uint32_t)8 << SRT_NX51_spi_cr1_rx_fifo_wm)
                               | MSK_NX51_spi_cr1_tx_fifo_clr
                               |((uint32_t)8 << SRT_NX51_spi_cr1_tx_fifo_wm)
                               |(eCsMode << SRT_NX51_spi_cr1_fss_static)
                               |((uint32_t)0 << SRT_NX51_spi_cr1_fss)
                               |((uint32_t)eModeSelect << SRT_NX51_spi_cr1_MS)
                               |MSK_NX51_spi_cr1_SSE;

  s_ptSpiMotion->aulSpi_cr[1] = ((uint32_t)8<<SRT_NX51_spi_cr1_rx_fifo_wm)
                               |((uint32_t)8<<SRT_NX51_spi_cr1_tx_fifo_wm)
                               |((uint32_t)eCsMode<<SRT_NX51_spi_cr1_fss_static)
                               |((uint32_t)0<<SRT_NX51_spi_cr1_fss)
                               |((uint32_t)eModeSelect<<SRT_NX51_spi_cr1_MS)
                               |MSK_NX51_spi_cr1_SSE;

  return 0;
}

/*****************************************************************************/
/*! SPIMOTION DeInitialize
* \description
*   This function DE-initializes the SPI Motion interface.
* \class
*   SPIMOTION Service Class
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SPIMOTION_DeInit ( void )
{
  /* disable interface, clear receive and transmit FIFOs and configure to default FIFO water mark generation */
  s_ptSpiMotion->aulSpi_cr[1] = MSK_NX51_spi_cr1_rx_fifo_clr
                               |((uint32_t)8<<SRT_NX51_spi_cr1_rx_fifo_wm)
                               |MSK_NX51_spi_cr1_tx_fifo_clr
                               |((uint32_t)8<<SRT_NX51_spi_cr1_tx_fifo_wm)
                               |((uint32_t)0<<SRT_NX51_spi_cr1_fss_static)
                               |((uint32_t)0<<SRT_NX51_spi_cr1_fss)
                               |((uint32_t)0<<SRT_NX51_spi_cr1_MS);

  s_ptSpiMotion->aulSpi_cr[1] = ((uint32_t)8<<SRT_NX51_spi_cr1_rx_fifo_wm)
                               |((uint32_t)8<<SRT_NX51_spi_cr1_tx_fifo_wm)
                               |((uint32_t)0<<SRT_NX51_spi_cr1_fss_static)
                               |((uint32_t)0<<SRT_NX51_spi_cr1_fss)
                               |((uint32_t)0<<SRT_NX51_spi_cr1_MS);

  s_ptSpiMotion->aulSpi_cr[0] =  ((uint32_t)0x800<<SRT_NX51_spi_cr0_sck_muladd)
                                |((uint32_t)7<<SRT_NX51_spi_cr0_datasize)
                                |((uint32_t)0<<SRT_NX51_spi_cr0_SPH)
                                |((uint32_t)0<<SRT_NX51_spi_cr0_SPO);

}

/*****************************************************************************/
/*! SPIMOTION Assert Chip Select
* \description
*   This function asserts a CS.
* \class
*   SPIMOTION Service Class
* \params
*   uChipNum [in] 1/0: Chip number
* \return                                                                    */
/*****************************************************************************/
void NX51_SPIMOTION_SelectChip( unsigned int uChipNum )
{
  /* assert chip select */
  if( uChipNum<NUM_CS ) {
    s_ptSpiMotion->aulSpi_cr[1] |= ((uint32_t)1 << (SRT_NX51_spi_cr1_fss+uChipNum));
  }
}

/*****************************************************************************/
/*! SPIMOTION DE-assert Chip Select
* \description
*   This function DE-asserts a CS.
* \class
*   SPIMOTION Service Class
* \params
*   uChipNum [in] 1/0: Chip number
* \return                                                                    */
/*****************************************************************************/
void NX51_SPIMOTION_DeSelectChip( unsigned int uChipNum )
{
  /* DE-assert chip select */
  if( uChipNum<NUM_CS ) {
    s_ptSpiMotion->aulSpi_cr[1] &= ~((uint32_t)1 << (SRT_NX51_spi_cr1_fss+uChipNum));
  }
}


/*****************************************************************************/
/*! SPIMOTION Exchange Data 16 bit wise
* \description
*   This function exchanges data via SPI.
* \class
*   SPIMOTION Service Class
* \params
*   uLen         [in] Data length
*   pausTxData   [in] Pointer to area of tx data
*   pausRxBuffer [in] Pointer to area of rx buffer
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SPIMOTION_ExchangeData16( unsigned int    uLen,
                                    const uint16_t* pausTxData,
                                    uint16_t*       pausRxBuffer )
{
  unsigned int uLoop;

  /* transmit data */
  for( uLoop=0; uLoop < uLen; uLoop++ ) {
    s_ptSpiMotion->ulSpi_dr = (uint32_t)pausTxData[uLoop];
  }

  /* wait for transfer completed */
  while ( s_ptSpiMotion->ulSpi_sr & MSK_NX51_spi_sr_BSY );

  /* read received data */
  for( uLoop=0; uLoop < uLen; uLoop++ ) {
    pausRxBuffer[uLoop] = (uint16_t)s_ptSpiMotion->ulSpi_dr;
  }
}

/*****************************************************************************/
/*! SPIMOTION Exchange Data 8 bit wise
* \description
*   This function exchanges data via SPI.
* \class
*   SPIMOTION Service Class
* \params
*   uLen         [in] Data length
*   pabTxData    [in] Pointer to area of tx data
*   pabRxBuffer  [in] Pointer to area of rx buffer
* \return
*                                                                            */
/*****************************************************************************/
void NX51_SPIMOTION_ExchangeData8( unsigned int   uLen,
                                   const uint8_t* pabTxData,
                                   uint8_t*       pabRxBuffer )
{
  unsigned int uLoop;

  /* transmit data */
  for( uLoop=0; uLoop < uLen; uLoop++ ) {
    s_ptSpiMotion->ulSpi_dr = (uint32_t)pabTxData[uLoop];
  }

  /* wait for transfer completed */
  while ( s_ptSpiMotion->ulSpi_sr & MSK_NX51_spi_sr_BSY );

  /* read received data */
  for( uLoop=0; uLoop < uLen; uLoop++ ) {
    pabRxBuffer[uLoop] = (uint8_t)s_ptSpiMotion->ulSpi_dr;
  }
}
