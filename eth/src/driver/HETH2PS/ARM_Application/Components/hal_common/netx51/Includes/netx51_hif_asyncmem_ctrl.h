#ifndef __NETX51_HIF_ASYNCMEM_CTRL_H
#define __NETX51_HIF_ASYNCMEM_CTRL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! Configuration Asynchronous Memory RDY Filter                             */
/*****************************************************************************/
typedef enum NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_Etag {
  NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_OFF             = 0,
  NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_SAMPLED_2_TIMES = 1,
  NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_SAMPLED_3_TIMES = 2,
  NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_SAMPLED_4_TIMES = 3
} NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_E;

/*****************************************************************************/
/*! Configuration Data bus width of CS area                                  */
/*****************************************************************************/
typedef enum NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_Etag {
  NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_8BIT  = 0,
  NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_16BIT = 1,
  NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_32BIT = 2,
  NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_DIS   = 3
} NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX51_HIF_ASYNCMEM_CTRL_Deinit ( void );

int NX51_HIF_ASYNCMEM_CTRL_SetRdyCfg ( unsigned int uRdyActLevel,
                                       NX51_HIF_ASYNCMEM_CTRL_RDY_FILTER_E uRdyFilter,
                                       unsigned int uDisRdyTimeout,
                                       unsigned int uEnRdyTimeOutIrq );

int NX51_HIF_ASYNCMEM_CTRL_SetupCsArea ( unsigned int uCsNum,
                                         NX51_HIF_ASYNCMEM_CTRL_CS_AREA_DWIDTH_E uDataWidth,
                                         unsigned int uNumWs,
                                         unsigned int uPrePause,
                                         unsigned int uPostPause,
                                         unsigned int uDisPrePauseSeqRd,
                                         unsigned int uDisPostPauseSeqRd,
                                         unsigned int uStaticCs,
                                         unsigned int uEnRdy );

#endif /* #ifndef __NETX51_HIF_ASYNCMEM_CTRL_H */
