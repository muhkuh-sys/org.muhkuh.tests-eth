#ifndef __NETX56_BUFMAN_H
#define __NETX56_BUFMAN_H

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX51_BUFMAN_CH_MAX_NUM  8

/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/
int NX51_BUFMAN_ReqWriteBuf( unsigned int uChNum );
int NX51_BUFMAN_RelWriteBuf( unsigned int uChNum );
int NX51_BUFMAN_ReqReadBuf( unsigned int uChNum );
int NX51_BUFMAN_ResetChannel( unsigned int uChNum );

#endif /* #ifndef __NETX56_BUFMAN_H */
