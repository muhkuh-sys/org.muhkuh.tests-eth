#ifndef __NETX6_XC_H
#define __NETX6_XC_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
typedef enum NX6_XC_TYPE_Etag {
  NX6_XC_TYPE_RPU  = 0,
  NX6_XC_TYPE_TPU  = 1,
  NX6_XC_TYPE_RPEC = 2,
  NX6_XC_TYPE_TPEC = 3
} NX6_XC_TYPE_E;

#define MSK_NX6_XC_TYPE_RPU  (1U << NX6_XC_TYPE_RPU)
#define MSK_NX6_XC_TYPE_TPU  (1U << NX6_XC_TYPE_TPU)
#define MSK_NX6_XC_TYPE_RPEC (1U << NX6_XC_TYPE_RPEC)
#define MSK_NX6_XC_TYPE_TPEC (1U << NX6_XC_TYPE_TPEC)
#define MSK_NX6_XC_TYPE_XMAC (MSK_NX6_XC_TYPE_RPU  | MSK_NX6_XC_TYPE_TPU)
#define MSK_NX6_XC_TYPE_XPEC (MSK_NX6_XC_TYPE_RPEC | MSK_NX6_XC_TYPE_TPEC)
#define MSK_NX6_XC_TYPE_ALL  (MSK_NX6_XC_TYPE_XMAC | MSK_NX6_XC_TYPE_XPEC)

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX6_XC_Reset(unsigned int uiPort, void* pvUser);
int NX6_XC_Load(unsigned int uiPort, NX6_XC_TYPE_E eXcType, const uint32_t* pulXcPrg, void* pvUser);
int NX6_XC_Start(unsigned int uiPort, void* pvUser);
int NX6_XC_StartUnits(unsigned int uiPort, unsigned int uiUnitVec, void* pvUser);

#endif /* #ifndef __NETX6_XC_H */
