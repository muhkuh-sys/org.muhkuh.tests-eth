#ifndef __NETX51_XPECIRQ_H
#define __NETX51_XPECIRQ_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/
int      NX51_XPECIRQ_SetIrq ( unsigned int uXpecNum, uint32_t ulVal );
uint32_t NX51_XPECIRQ_GetIrq ( unsigned int uXpecNum );

#endif /* #ifndef __NETX51_XPECIRQ_H */
