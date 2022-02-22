#ifndef NETX51_ACCESSNX6_H_
#define NETX51_ACCESSNX6_H_

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX51_ACCESSNX6_SetupParallelDpm( unsigned int uWidth,
                                     bool fEnRdy,
                                     unsigned int uRdyPol,
                                     bool fIntramRequired );
int NX51_ACCESSNX6_SetupSerialDpm( uint16_t usClkFreq,
                                   bool fIntramRequired );

uint32_t NX51_ACCESSNX6_SerialDpmRead( unsigned int uSize, uint32_t ulDpmAddr );
void NX51_ACCESSNX6_SerialDpmWrite( unsigned int uSize, uint32_t ulDpmAddr, uint32_t ulVal );

void NX51_ACCESSNX6_SerialDpmMemRead( void* pvDst, const void *pvSrc, unsigned int uSize );
void NX51_ACCESSNX6_SerialDpmMemWrite( void* pvDst, const void *pvSrc, unsigned int uSize );

#endif /* NETX51_ACCESSNX6_H_ */
