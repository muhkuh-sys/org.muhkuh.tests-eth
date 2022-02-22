#ifndef __HAL_IFLASH_H
#define __HAL_IFLASH_H

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

#ifndef HALDEC
#define HALDEC(name) name
#endif

typedef enum
{
  IFLASH_BLOCK_INFO,  /* information memory  */
  IFLASH_BLOCK_INFO1, /* information1 memory */
  IFLASH_BLOCK_MAIN,  /* main memory         */
} IFLASH_BLOCK_E;

#define IFLASH_CODE_ERASE_REF_CELL 0x0a
#define IFLASH_CODE_ACCESS_IFREN1  0x1b

#define IFLASH_PAGE_ALL 0x80000000

int HALDEC(IFlash_SetRedundancyPage)(unsigned int uiInst, unsigned int uiRedSel, unsigned int uiPage);
int HALDEC(IFlash_BlockSelect)(unsigned int uiInst, IFLASH_BLOCK_E eBlock);
int HALDEC(IFlash_Program)(unsigned int uiInst, uint32_t ulXAdr, uint32_t ulYAdr, const uint32_t* pulData, unsigned int uiLen);
int HALDEC(IFlash_ProgramMan)(unsigned int uiInst, uint32_t ulXAdr, uint32_t ulYAdr, const uint32_t* pulData, unsigned int uiLen);
int HALDEC(IFlash_StartErase)(unsigned int uiInst, unsigned int uiPage);
int HALDEC(IFlash_StopErase)(unsigned int uiInst);
int HALDEC(IFlash_StartEraseMan)(unsigned int uiInst, unsigned int uiPage);
int HALDEC(IFlash_StopEraseMan)(unsigned int uiInst);
int HALDEC(IFlash_EnterTestmode)(unsigned int uiInst, unsigned int uiCode);
int HALDEC(IFlash_ExitTestmode)(unsigned int uiInst);
int HALDEC(IFlash_SetProtection)(unsigned int uiInst, uint32_t ulVal);
int HALDEC(IFlash_SetGlobalTiming)(unsigned int uiMultiplier, unsigned int uiDivisor);

#endif  /* __HAL_IFLASH_H */
