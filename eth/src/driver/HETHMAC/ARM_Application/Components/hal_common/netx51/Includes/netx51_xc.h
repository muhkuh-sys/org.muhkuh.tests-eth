#ifndef __NETX51_XC_H
#define __NETX51_XC_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum NX51_XC_TYPE_Etag {
  NX51_XC_TYPE_RPU  = 0,
  NX51_XC_TYPE_TPU  = 1,
  NX51_XC_TYPE_RPEC = 2,
  NX51_XC_TYPE_TPEC = 3
} NX51_XC_TYPE_E;

#define MSK_NX51_XC_UNIT_RPU  (1U << NX51_XC_TYPE_RPU)
#define MSK_NX51_XC_UNIT_TPU  (1U << NX51_XC_TYPE_TPU)
#define MSK_NX51_XC_UNIT_RPEC (1U << NX51_XC_TYPE_RPEC)
#define MSK_NX51_XC_UNIT_TPEC (1U << NX51_XC_TYPE_TPEC)
#define MSK_NX51_XC_UNIT_XMAC (MSK_NX51_XC_UNIT_RPU | MSK_NX51_XC_UNIT_TPU)
#define MSK_NX51_XC_UNIT_XPEC (MSK_NX51_XC_UNIT_RPEC | MSK_NX51_XC_UNIT_TPEC)
#define MSK_NX51_XC_UNIT_ALL  (MSK_NX51_XC_UNIT_XMAC | MSK_NX51_XC_UNIT_XPEC)

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX51_XC_Reset( unsigned int uiPortNo, void* pvUser );
int NX51_XC_Load( unsigned int uiPortNo, NX51_XC_TYPE_E eXcType, const uint32_t* pulXcPrg, void* pvUser );
int NX51_XC_Start( unsigned int uiPortNo, void* pvUser );
int NX51_XC_StartUnits( unsigned int uiPortNo, unsigned int uiUnitVect, void* pvUser );

#endif /* #ifndef __NETX51_XC_H */
