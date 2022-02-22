#ifndef __HAL_RAP_ECC_H
#define __HAL_RAP_ECC_H

#include <stdbool.h>
#include <stdint.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum
{
  RAP_ECC_RAM_CR7LLRAM0 = 0, /* LLRAM 0x00000..0x1ffff */
  RAP_ECC_RAM_CR7LLRAM1 = 1, /* LLRAM 0x20000..0x3ffff */
  RAP_ECC_RAM_CR7LLRAM2 = 2, /* LLRAM 0x40000..0x5ffff */
  RAP_ECC_RAM_CR7LLRAM3 = 3, /* LLRAM 0x60000..0x7ffff */
  RAP_ECC_RAM_CR7LLRAM4 = 4, /* LLRAM 0x80000..0x9ffff */
  RAP_ECC_RAM_CR7LLRAM5 = 5, /* LLRAM 0xa0000..0xbffff */
  RAP_ECC_RAM_CR7LLRAM6 = 6, /* LLRAM 0xc0000..0xdffff */
  RAP_ECC_RAM_CR7LLRAM7 = 7, /* LLRAM 0xe0000..0xfffff */
  RAP_ECC_RAM_INTRAM_RAP0 = 8,
  RAP_ECC_RAM_INTRAM_RAP1 = 9,
  RAP_ECC_RAM_INTRAM_RAP2 = 10,
  RAP_ECC_RAM_INTRAM_RAP3 = 11,
  RAP_ECC_RAM_INTRAM0 = 16,
  RAP_ECC_RAM_INTRAM1 = 17,
  RAP_ECC_RAM_INTRAM2 = 18,
  RAP_ECC_RAM_INTRAM3 = 19,
  RAP_ECC_RAM_INTRAM4 = 20,
  RAP_ECC_RAM_INTRAM5 = 21,
  RAP_ECC_RAM_INTRAMHS0 = 22,
  RAP_ECC_RAM_INTRAMHS1 = 23
} RAP_ECC_RAM_E;

/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

void     HALDEC(RAP_ECC_SetCtrl)( int iMemId, bool fEnable, uint32_t ulSyndromeInv );
uint32_t HALDEC(RAP_ECC_GetErrAdr)( int iMemId, bool fDbe );
uint32_t HALDEC(RAP_ECC_GetIrq)( bool fDbe );
void     HALDEC(RAP_ECC_ConfirmIrq)( uint32_t ulIrqMsk, bool fDbe );

#endif /* #ifndef __HAL_RAP_ECC_H */
