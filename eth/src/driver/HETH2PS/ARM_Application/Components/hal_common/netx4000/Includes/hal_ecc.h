#ifndef __HAL_ECC_H
#define __HAL_ECC_H

#include <stdbool.h>
#include <stdint.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum
{
  ECC_MEM_XC0_RPEC0_PRAM = 0,
  ECC_MEM_XC0_RPEC1_PRAM,
  ECC_MEM_XC1_RPEC0_PRAM,
  ECC_MEM_XC1_RPEC1_PRAM,
  ECC_MEM_XC0_TPEC0_PRAM,
  ECC_MEM_XC0_TPEC1_PRAM,
  ECC_MEM_XC1_TPEC0_PRAM,
  ECC_MEM_XC1_TPEC1_PRAM,
  ECC_MEM_XC0_XPEC0_DRAM,
  ECC_MEM_XC0_XPEC1_DRAM,
  ECC_MEM_XC1_XPEC0_DRAM,
  ECC_MEM_XC1_XPEC1_DRAM,
  ECC_MEM_XC0_RPU0_RAM,
  ECC_MEM_XC0_RPU1_RAM,
  ECC_MEM_XC1_RPU0_RAM,
  ECC_MEM_XC1_RPU1_RAM,
  ECC_MEM_XC0_TPU0_RAM,
  ECC_MEM_XC0_TPU1_RAM,
  ECC_MEM_XC1_TPU0_RAM,
  ECC_MEM_XC1_TPU1_RAM,
  ECC_MEM_XC0_PFIFO,
  ECC_MEM_XC1_PFIFO,
  ECC_MEM_XPIC0_PRAM,
  ECC_MEM_XPIC1_PRAM,
  ECC_MEM_XPIC2_PRAM,
  ECC_MEM_XPIC3_PRAM,
  ECC_MEM_XPIC0_DRAM,
  ECC_MEM_XPIC1_DRAM,
  ECC_MEM_XPIC2_DRAM,
  ECC_MEM_XPIC3_DRAM,
  ECC_MEM_INTRAMETH
} ECC_MEM_E;

/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/

void     ECC_SetCtrl( int iMem, bool fEnable, uint32_t ulSyndromeInv );
uint32_t ECC_GetErrAdr( int iMem, bool fDbe );
uint32_t ECC_GetIrq( bool fDbe );
void     ECC_ConfirmIrq( uint32_t ulIrqMsk, bool fDbe );

#endif /* #ifndef __HAL_ECC_H */
