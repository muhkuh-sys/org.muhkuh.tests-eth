#ifndef __NETX51_XPIC_H
#define __NETX51_XPIC_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX51_XPIC_Reset( void* pvUser );
void NX51_XPIC_Load( const uint32_t *pulXpicPrg, void* pvUser );
void NX51_XPIC_Start( void* pvUser );
void NX51_XPIC_Stop( void* pvUser );

#endif /* #ifndef __NETX51_XPIC_H */
