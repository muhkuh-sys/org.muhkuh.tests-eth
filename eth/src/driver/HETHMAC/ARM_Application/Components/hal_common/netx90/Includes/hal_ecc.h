#ifndef __HAL_ECC_H
#define __HAL_ECC_H

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum
{
  ECC_MEM_INTRAM0        = 0,
  ECC_MEM_INTRAM1        = 1,
  ECC_MEM_INTRAM2        = 2,
  ECC_MEM_INTRAM3        = 3,
  ECC_MEM_INTRAM4        = 4,
  ECC_MEM_INTRAM5        = 5,
  ECC_MEM_INTRAM6        = 6,
  ECC_MEM_INTRAM7        = 7,
  ECC_MEM_INTRAMHS       = 8,
  ECC_MEM_XC0_RPEC0_PRAM = 9,
  ECC_MEM_XC0_RPEC1_PRAM = 10,
  ECC_MEM_XC0_TPEC0_PRAM = 11,
  ECC_MEM_XC0_TPEC1_PRAM = 12,
  ECC_MEM_XC0_XPEC0_DRAM = 13,
  ECC_MEM_XC0_XPEC1_DRAM = 14,
  ECC_MEM_XC0_RPU0_RAM   = 15,
  ECC_MEM_XC0_RPU1_RAM   = 16,
  ECC_MEM_XC0_TPU0_RAM   = 17,
  ECC_MEM_XC0_TPU1_RAM   = 18,
  ECC_MEM_XC0_PFIFO      = 19,
  ECC_MEM_XPIC0_PRAM     = 20,
  ECC_MEM_XPIC1_PRAM     = 21,
  ECC_MEM_XPIC0_DRAM     = 22,
  ECC_MEM_XPIC1_DRAM     = 23,
  ECC_MEM_IFLASH0        = 24,
  ECC_MEM_IFLASH1        = 25,
  ECC_MEM_IFLASH2        = 26,
} ECC_MEM_E;

/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

void     HALDEC(ECC_SetCtrl)( int iMem, bool fEnable, uint32_t ulSyndromeInv );
uint32_t HALDEC(ECC_GetErrAdr)( int iMem, bool fDbe );
uint32_t HALDEC(ECC_GetIrq)( bool fDbe );
void     HALDEC(ECC_ConfirmIrq)( uint32_t uIrqMsk, bool fDbe );
uint32_t HALDEC(ECC_GetIrqMask)( int iMem, bool fDbe );

#endif /* #ifndef __HAL_ECC_H */
