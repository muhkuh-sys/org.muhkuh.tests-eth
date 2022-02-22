#ifndef __NETX51_SR_H
#define __NETX51_SR_H

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

uint32_t NX51_SR_GetStatcfg(unsigned int uPortNo);
void     NX51_SR_SetConfig(unsigned int uPortNo, uint32_t ulValue);
uint32_t NX51_SR_ReadSr(unsigned int uRegNo);
void     NX51_SR_WriteSr(unsigned int uRegNo, uint32_t ulValue);
uint32_t NX51_SR_ReadStatBitsShared(void);
void     NX51_SR_WriteStatBitsShared(uint32_t ulValue);

#endif /* #ifndef __NETX51_SR_H */
