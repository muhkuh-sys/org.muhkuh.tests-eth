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

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int NX6_XC_Reset(unsigned int uPortNo, void* pvUser);
int NX6_XC_Load(unsigned int uPortNo, NX6_XC_TYPE_E eXcType, const uint32_t* pulXcPrg, void* pvUser);
int NX6_XC_Start(unsigned int uPortNo, void* pvUser);

#endif /* #ifndef __NETX6_XC_H */
