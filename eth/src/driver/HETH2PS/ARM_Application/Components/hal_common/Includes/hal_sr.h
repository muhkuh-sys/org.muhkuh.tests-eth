#ifndef __HAL_SR_H
#define __HAL_SR_H

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

uint32_t SR_GetStatcfg(unsigned int uiXcInst, unsigned int uiXpec);
void     SR_SetConfig(unsigned int uiXcInst, unsigned int uiXpec, uint32_t ulValue);
uint32_t SR_ReadSr(unsigned int uiXcInst, unsigned int uiRegIdx);
void     SR_WriteSr(unsigned int uiXcInst, unsigned int uiRegIdx, uint32_t ulValue);
uint32_t SR_ReadStatBitsShared(void);
void     SR_WriteStatBitsShared(uint32_t ulValue);

#endif /* #ifndef __HAL_SR_H */
