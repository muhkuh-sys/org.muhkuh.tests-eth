#ifndef __NETX6_XPIC_H
#define __NETX6_XPIC_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX6_XPIC_Reset( void* pvUser );
void NX6_XPIC_Load( const uint32_t *pulXpicPrg, void* pvUser );
void NX6_XPIC_Start( void* pvUser );
void NX6_XPIC_Stop( void* pvUser );


#endif /* #ifndef __NETX6_XPIC_H */
