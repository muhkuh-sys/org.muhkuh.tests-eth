#ifndef NETX51_HIF_IOCTRL_H_
#define NETX51_HIF_IOCTRL_H_

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! Configuration HIF Memory interface                                       */
/*****************************************************************************/
typedef enum NX51_HIF_IOCTRL_HIF_MI_CFG_Etag {
  NX51_HIF_IOCTRL_HIF_MI_CFG_8BIT_MI  = 0, /**< HIF IOs are used as 8 bit MI  */
  NX51_HIF_IOCTRL_HIF_MI_CFG_16BIT_MI = 1, /**< HIF IOs are used as 16 bit MI (together with serial DPM possible).  */
  NX51_HIF_IOCTRL_HIF_MI_CFG_32BIT_MI = 2, /**< HIF IOs are used as 32 bit MI (no DPM and no Ethernet possible) */
  NX51_HIF_IOCTRL_HIF_MI_CFG_NO_MI    = 3  /**< No MI usage. HIF IOs can be used as PIOs or for parallel DPM */
} NX51_HIF_IOCTRL_HIF_MI_CFG_E;

/*****************************************************************************/
/*! Configuration HIF Memory Interface Address Width                         */
/*****************************************************************************/
typedef enum NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_Etag {
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_2K  =  0,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_4K  =  1,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_8K  =  2,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_16K =  3,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_32K =  4,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_64K =  5,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_128K=  6,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_256K=  7,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_512K=  8,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_1M  =  9,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_2M  = 10,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_4M  = 11,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_8M  = 12,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_16M = 13,
  NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_32M = 14
} NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_E;

/*****************************************************************************/
/*! Configuration HIF PIOs Sampled                                           */
/*****************************************************************************/
typedef enum NX51_HIF_IOCTRL_IN_CTRL_E {
  NX51_HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_AT_POR       = 0, /**< pio_in registers show HIF IO states sampled at power-on-reset release  */
  NX51_HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_CONTINUOUSLY = 1, /**< HIF IO states are sampled continuously (each netX system clock cycle)  */
  NX51_HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_EN_IN_LO     = 2, /**< HIF IO states are sampling is done each system clock cycle when enable signal EN_IN(selected by sel_en_in bit) level is low */
  NX51_HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_EN_IN_HI     = 3  /**< HIF IO states are sampling is done each system clock cycle when enable signal EN_IN(selected by sel_en_in bit) level is high */
} NX51_HIF_IOCTRL_IN_CTRL_E;

/*****************************************************************************/
/*! Configuration HIF IRQ PIO                                                */
/*****************************************************************************/
typedef enum NX51_HIF_IOCTRL_IRQ_PIO_CFG_Etag {
  NX51_HIF_IOCTRL_IRQ_PIO_CFG_LOW_LEVEL_ACTIVE    = 0, /**< low level active IRQ */
  NX51_HIF_IOCTRL_IRQ_PIO_CFG_HIGH_LEVEL_ACTIVE   = 1, /**< high level active IRQ */
  NX51_HIF_IOCTRL_IRQ_PIO_CFG_FALLING_EDGE_ACTIVE = 2, /**< falling edge active IRQ */
  NX51_HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE  = 3  /**< rising edge active IRQ  */
} NX51_HIF_IOCTRL_IRQ_PIO_CFG_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX51_HIF_IOCTRL_DeInit ( void );
int NX51_HIF_IOCTRL_SetIoCfg ( NX51_HIF_IOCTRL_HIF_MI_CFG_E eDataWidth,
                               NX51_HIF_IOCTRL_SEL_HIF_A_WIDTH_E eAddrWidth,
                               unsigned int uEnHifRdyPioMi,
                               unsigned int uEnHifSdram
                             );
int NX51_HIF_IOCTRL_SetPioCfg ( NX51_HIF_IOCTRL_IN_CTRL_E eInCtrl,
                                unsigned int uSelEnIn,
                                unsigned int uFilterEnIn,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio35Cfg,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio36Cfg,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio40Cfg,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio47Cfg,
                                NX51_HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio72Cfg,
                                unsigned int uFilterIrqs,
                                unsigned int uNetx50PioRegComp
                             );
void     NX51_HIF_IOCTRL_SetLine( unsigned int uOutRegNum, uint32_t ulVal );
uint32_t NX51_HIF_IOCTRL_GetLine( unsigned int uOutRegNum );
void     NX51_HIF_IOCTRL_SetOe( unsigned int uOeRegNum, uint32_t ulVal );
uint32_t NX51_HIF_IOCTRL_GetOe( unsigned int uOeRegNum );
uint32_t NX51_HIF_IOCTRL_GetIn( unsigned int uInRegNum );
void     NX51_HIF_IOCTRL_IrqEnable( uint32_t ulPioNum );
void     NX51_HIF_IOCTRL_IrqDisable( uint32_t ulPioNum );
int      NX51_HIF_IOCTRL_GetIrq( uint32_t ulPioNum );
uint32_t NX51_HIF_IOCTRL_GetIrqMasked( void );
void     NX51_HIF_IOCTRL_IrqReset( uint32_t ulPioNum );

#endif /* NETX51_HIF_IOCTRL_H_ */
