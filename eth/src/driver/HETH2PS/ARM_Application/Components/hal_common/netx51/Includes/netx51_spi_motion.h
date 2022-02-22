#ifndef __NETX51_SPI_MOTION_H
#define __NETX51_SPI_MOTION_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NUM_CS 3

/*****************************************************************************/
/*! Mode Select                                                              */
/*****************************************************************************/
typedef enum
{
  NX51_SPIMOTION_MODE_SELECT_MASTER = 0,
  NX51_SPIMOTION_MODE_SELECT_SLAVE  = 1
} NX51_SPIMOTION_MODE_SELECT;

/*****************************************************************************/
/*! Speed                                                                    */
/*****************************************************************************/
typedef enum
{
  NX51_SPIMOTION_SCLK_FREQ_50MHZ    = 2048,
  NX51_SPIMOTION_SCLK_FREQ_25MHZ    = 1024,
  NX51_SPIMOTION_SCLK_FREQ_20MHZ    =  816,
  NX51_SPIMOTION_SCLK_FREQ_12_5MHZ  =  512,
  NX51_SPIMOTION_SCLK_FREQ_10MHZ    =  408
} NX51_SPIMOTION_SCLK_FREQ;

/*****************************************************************************/
/*! Chip Select Mode                                                         */
/*****************************************************************************/
typedef enum
{
  NX51_SPIMOTION_CS_MODE_AUTO    = 0,
  NX51_SPIMOTION_CS_MODE_STATIC  = 1
} NX51_SPIMOTION_CS_MODE;

/*****************************************************************************/
/*! Serial Clock Phase                                                       */
/*****************************************************************************/
typedef enum
{
  NX51_SPIMOTION_SERIAL_CLK_PHASE_1ST = 0,
  NX51_SPIMOTION_SERIAL_CLK_PHASE_2ND = 1
} NX51_SPIMOTION_SERIAL_CLK_PHASE;

/*****************************************************************************/
/*! Serial Clock Polarity                                                    */
/*****************************************************************************/
typedef enum
{
  NX51_SPIMOTION_SERIAL_CLK_POL_LO = 0,
  NX51_SPIMOTION_SERIAL_CLK_POL_HI = 1
} NX51_SPIMOTION_SERIAL_CLK_POL;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX51_SPIMOTION_Init ( NX51_SPIMOTION_MODE_SELECT eModeSelect,
                          NX51_SPIMOTION_SCLK_FREQ eSckMulAdd,
                          NX51_SPIMOTION_SERIAL_CLK_PHASE eSph,
                          NX51_SPIMOTION_SERIAL_CLK_POL eSpo,
                          unsigned int uDataSize,
                          NX51_SPIMOTION_CS_MODE eCsMode );
void NX51_SPIMOTION_DeInit ( void );
void NX51_SPIMOTION_SelectChip( unsigned int uChipNum );
void NX51_SPIMOTION_DeSelectChip( unsigned int uChipNum );
void NX51_SPIMOTION_ExchangeData8( unsigned int uLen, const uint8_t* pabTxData, uint8_t* pabRxBuffer );
void NX51_SPIMOTION_ExchangeData16( unsigned int uLen, const uint16_t* pausTxData, uint16_t* pausRxBuffer );

#endif /* #ifndef __NETX51_SPI_MOTION_H */
