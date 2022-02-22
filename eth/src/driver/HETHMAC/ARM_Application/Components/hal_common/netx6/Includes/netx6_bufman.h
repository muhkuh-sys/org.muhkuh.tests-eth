#ifndef __NETX56_BUFMAN_H
#define __NETX56_BUFMAN_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX6_BUFMAN_CH_MAX_NUM  8

/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/
int NX6_BUFMAN_ReqWriteBuf( unsigned int uChNum );
int NX6_BUFMAN_RelWriteBuf( unsigned int uChNum );
int NX6_BUFMAN_ReqReadBuf( unsigned int uChNum );
int NX6_BUFMAN_ResetChannel( unsigned int uChNum );


#endif /* #ifndef __NETX56_BUFMAN_H */
