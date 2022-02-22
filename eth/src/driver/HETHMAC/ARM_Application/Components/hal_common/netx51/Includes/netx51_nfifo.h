#ifndef __NETX51_NFIFO_H
#define __NETX51_NFIFO_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef struct
{
  uint32_t ulBaseAddr;  /* Base address of FIFO data memory (dw-aligned) */
  uint32_t ulConfig;
  uint32_t ulStatus;
} NX51_NFIFO_FIFO_CONFIG_T;

#define NX51_MSK_NFIFO_FIFO_ADDR_MAS                   0x00000003
#define NX51_SRT_NFIFO_FIFO_ADDR_MAS                   0
#define NX51_MSK_NFIFO_FIFO_ADDR_ADDRESS               0xfffffffc
#define NX51_SRT_NFIFO_FIFO_ADDR_ADDRESS               0

#define NX51_NFIFO_MAS_VAL_BYTE                        0
#define NX51_NFIFO_MAS_VAL_WORD                        1
#define NX51_NFIFO_MAS_VAL_DWORD                       2

#define NX51_MSK_NFIFO_FIFO_CONFIG_SIZE                0x00001fff
#define NX51_SRT_NFIFO_FIFO_CONFIG_SIZE                0
#define NX51_MSK_NFIFO_FIFO_CONFIG_RD_SWC              0x00002000
#define NX51_SRT_NFIFO_FIFO_CONFIG_RD_SWC              13
#define NX51_MSK_NFIFO_FIFO_CONFIG_RD_SEN              0x0000c000
#define NX51_SRT_NFIFO_FIFO_CONFIG_RD_SEN              14
#define NX51_MSK_NFIFO_FIFO_CONFIG_WATERMARK           0x1fff0000
#define NX51_SRT_NFIFO_FIFO_CONFIG_WATERMARK           16
#define NX51_MSK_NFIFO_FIFO_CONFIG_WR_SWC              0x20000000
#define NX51_SRT_NFIFO_FIFO_CONFIG_WR_SWC              29
#define NX51_MSK_NFIFO_FIFO_CONFIG_WR_SEN              0xc0000000
#define NX51_SRT_NFIFO_FIFO_CONFIG_WR_SEN              30

#define NX51_MSK_NFIFO_FIFO_STATUS_FILL_LEVEL          0x00001fff
#define NX51_SRT_NFIFO_FIFO_STATUS_FILL_LEVEL          0
#define NX51_MSK_NFIFO_FIFO_STATUS_FULL                0x00002000
#define NX51_SRT_NFIFO_FIFO_STATUS_FULL                13
#define NX51_MSK_NFIFO_FIFO_STATUS_FULL_WATERMARK      0x00004000
#define NX51_SRT_NFIFO_FIFO_STATUS_FULL_WATERMARK      14
#define NX51_MSK_NFIFO_FIFO_STATUS_OVERFLOW            0x00008000
#define NX51_SRT_NFIFO_FIFO_STATUS_OVERFLOW            15
#define NX51_MSK_NFIFO_FIFO_STATUS_WRITE_PTR           0x1fff0000
#define NX51_SRT_NFIFO_FIFO_STATUS_WRITE_PTR           16
#define NX51_MSK_NFIFO_FIFO_STATUS_EMPTY               0x20000000
#define NX51_SRT_NFIFO_FIFO_STATUS_EMPTY               29
#define NX51_MSK_NFIFO_FIFO_STATUS_EMPTY_WATERMARK     0x40000000
#define NX51_SRT_NFIFO_FIFO_STATUS_EMPTY_WATERMARK     30
#define NX51_MSK_NFIFO_FIFO_STATUS_UNDERRUN            0x80000000
#define NX51_SRT_NFIFO_FIFO_STATUS_UNDERRUN            31


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void NX51_NFIFO_InitHW( uint32_t ulBaseCfgAddr );
int  NX51_NFIFO_ConfigFifo( uint32_t ulFifoNum, uint32_t ulFifoDataAddr, uint32_t ulAccessSize, uint32_t ulSize, uint32_t ulWatermark );
int  NX51_NFIFO_GetFifoConfig( uint32_t ulFifoNum, uint32_t* pulFifoDataAddr, uint32_t* pulAccessSize, uint32_t* pulSize, uint32_t* pulWatermark );
int  NX51_NFIFO_WriteFifo( uint32_t ulFifoNum, uint32_t ulData );
int  NX51_NFIFO_ReadFifo( uint32_t ulFifoNum, uint32_t* pulData );
int  NX51_NFIFO_GetFifoStatus( uint32_t ulFifoNum, uint32_t* pulStatus );

#endif /* #ifndef __NETX51_NFIFO_H */
