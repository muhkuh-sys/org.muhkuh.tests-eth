#ifndef __HAL_SR_H
#define __HAL_SR_H

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

uint32_t HALDEC(SR_GetStatcfg)(unsigned int uXcNo, unsigned int uXpecNo);
void     HALDEC(SR_SetConfig)(unsigned int uXcNo, unsigned int uXpecNo, uint32_t ulValue);
uint32_t HALDEC(SR_ReadSr)(unsigned int uXcNo, unsigned int uRegNo);
void     HALDEC(SR_WriteSr)(unsigned int uXcNo, unsigned int uRegNo, uint32_t ulValue);
uint32_t HALDEC(SR_ReadStatBitsShared)(void);
void     HALDEC(SR_WriteStatBitsShared)(uint32_t ulValue);

#endif /* #ifndef __HAL_SR_H */
