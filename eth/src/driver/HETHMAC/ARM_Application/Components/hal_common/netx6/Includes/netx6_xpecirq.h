#ifndef __NETX6_XPECIRQ_H
#define __NETX6_XPECIRQ_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/
int      NX6_XPECIRQ_SetIrq ( unsigned int uXpecNum, uint32_t ulVal );
uint32_t NX6_XPECIRQ_GetIrq ( unsigned int uXpecNum );

#endif /* #ifndef __NETX6_XPECIRQ_H */
