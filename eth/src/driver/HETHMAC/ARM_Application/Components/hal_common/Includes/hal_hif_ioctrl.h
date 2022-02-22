#ifndef __HAL_HIF_IOCTRL_H
#define __HAL_HIF_IOCTRL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! Configuration HIF Memory interface                                       */
/*****************************************************************************/
typedef enum HIF_IOCTRL_HIF_MI_CFG_Etag {
  HIF_IOCTRL_HIF_MI_CFG_8BIT_MI  = 0, /**< HIF IOs are used as 8 bit MI  */
  HIF_IOCTRL_HIF_MI_CFG_16BIT_MI = 1, /**< HIF IOs are used as 16 bit MI (together with serial DPM possible).  */
  HIF_IOCTRL_HIF_MI_CFG_32BIT_MI = 2, /**< HIF IOs are used as 32 bit MI (no DPM and no Ethernet possible) */
  HIF_IOCTRL_HIF_MI_CFG_NO_MI    = 3  /**< No MI usage. HIF IOs can be used as PIOs or for parallel DPM */
} HIF_IOCTRL_HIF_MI_CFG_E;

/*****************************************************************************/
/*! Configuration Data BUS for HIF and MEM interface                         */
/*****************************************************************************/
typedef enum HIF_IOCTRL_SEL_DATA_Etag {
  HIF_IOCTRL_SEL_DATA_DISABLE    = 0, /**< All HIF and MEM IOs (also address and control signals) are undriven   */
  HIF_IOCTRL_SEL_DATA_MEM_ONLY   = 1, /**< Only the MEM MI is available. 8, 16 or 32bit devices can be connected */
  HIF_IOCTRL_SEL_DATA_HIF_ONLY   = 2, /**< Only the HIF MI is available. 8, 16 or 32bit devices can be connected */
  HIF_IOCTRL_SEL_DATA_HIF_MEM    = 3  /**< Both, HIF and MEM MI available. 8 or 16bit devices  can be connected  */
} HIF_IOCTRL_SEL_DATA_E;

/*****************************************************************************/
/*! Configuration HIF Memory Interface Address Width                         */
/*****************************************************************************/
typedef enum HIF_IOCTRL_SEL_HIF_A_WIDTH_Etag {
  HIF_IOCTRL_SEL_HIF_A_WIDTH_2K  =  0,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_4K  =  1,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_8K  =  2,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_16K =  3,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_32K =  4,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_64K =  5,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_128K=  6,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_256K=  7,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_512K=  8,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_1M  =  9,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_2M  = 10,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_4M  = 11,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_8M  = 12,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_16M = 13,
  HIF_IOCTRL_SEL_HIF_A_WIDTH_32M = 14
} HIF_IOCTRL_SEL_HIF_A_WIDTH_E;

/*****************************************************************************/
/*! Configuration HIF PIOs Sampled                                           */
/*****************************************************************************/
typedef enum HIF_IOCTRL_IN_CTRL_E {
  HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_AT_POR       = 0, /**< pio_in registers show HIF IO states sampled at power-on-reset release  */
  HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_CONTINUOUSLY = 1, /**< HIF IO states are sampled continuously (each netX system clock cycle)  */
  HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_EN_IN_LO     = 2, /**< HIF IO states are sampling is done each system clock cycle when enable signal EN_IN(selected by sel_en_in bit) level is low */
  HIF_IOCTRL_IN_CTRL_PIOS_SAMPLED_EN_IN_HI     = 3  /**< HIF IO states are sampling is done each system clock cycle when enable signal EN_IN(selected by sel_en_in bit) level is high */
} HIF_IOCTRL_IN_CTRL_E;

/*****************************************************************************/
/*! Configuration HIF PIOs Sampled Enable Select                             */
/*****************************************************************************/
typedef enum HIF_IOCTRL_SEL_EN_IN_Etag {
  HIF_IOCTRL_SEL_EN_IN_HIF_D16 = 0, /**< HIF IO input sampling enable select HIF_D16  */
  HIF_IOCTRL_SEL_EN_IN_HIF_D4  = 1  /**< HIF IO input sampling enable select HIF_D4  */
} HIF_IOCTRL_SEL_EN_IN_E;

/*****************************************************************************/
/*! Configuration HIF IRQ PIO                                                */
/*****************************************************************************/
typedef enum HIF_IOCTRL_IRQ_PIO_CFG_Etag {
  HIF_IOCTRL_IRQ_PIO_CFG_LOW_LEVEL_ACTIVE    = 0, /**< low level active IRQ */
  HIF_IOCTRL_IRQ_PIO_CFG_HIGH_LEVEL_ACTIVE   = 1, /**< high level active IRQ */
  HIF_IOCTRL_IRQ_PIO_CFG_FALLING_EDGE_ACTIVE = 2, /**< falling edge active IRQ */
  HIF_IOCTRL_IRQ_PIO_CFG_RISING_EDGE_ACTIVE  = 3  /**< rising edge active IRQ  */
} HIF_IOCTRL_IRQ_PIO_CFG_E;

/*****************************************************************************/
/*! HIF_IOCTRL Parameter IDs                                                 */
/*****************************************************************************/
typedef enum
{
  HIF_IOCTRL_PRM_HIF_PIO_CFG=0,
  HIF_IOCTRL_PRM_HIF_PIO_IRQ_ARM_MASK_SET,
  HIF_IOCTRL_PRM_HIF_PIO_IRQ_ARM_MASK_RESET,
  HIF_IOCTRL_PRM_HIF_PIO_IRQ_ARM_MASKED,
  HIF_IOCTRL_PRM_HIF_PIO_IRQ_XPIC_MASK_SET,
  HIF_IOCTRL_PRM_HIF_PIO_IRQ_XPIC_MASK_RESET,
  HIF_IOCTRL_PRM_HIF_PIO_IRQ_XPIC_MASKED,
  HIF_IOCTRL_PRM_HIF_PIO_IRQ_RAW
} HIF_IOCTRL_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
#ifndef HALDEC
#define HALDEC(name) name
#endif

void     HALDEC(HIF_IOCTRL_DeInit)( void );
int      HALDEC(HIF_IOCTRL_SetIoCfg)( HIF_IOCTRL_HIF_MI_CFG_E      eDataWidth,
                                     HIF_IOCTRL_SEL_HIF_A_WIDTH_E eAddrWidth,
                                     unsigned int                        uEnHifRdyPioMi,
                                     unsigned int                        uEnHifSdram
                                    );

int      HALDEC(HIF_IOCTRL_SetIoCfgReg)( HIF_IOCTRL_HIF_MI_CFG_E      eDataWidth,
                                        HIF_IOCTRL_SEL_HIF_A_WIDTH_E eAddrWidth,
                                        unsigned int                        uEnHifRdyPioMi,
                                        unsigned int                        uEnHifSdram,
                                        HIF_IOCTRL_SEL_DATA_E        eSelData
                                      );

void     HALDEC(HIF_IOCTRL_SetIoConfig)( uint32_t ulHifIoCfg );
uint32_t HALDEC(HIF_IOCTRL_GetIoConfig)( void );
int      HALDEC(HIF_IOCTRL_SetPioCfg)( HIF_IOCTRL_IN_CTRL_E    eInCtrl,
                                      unsigned int             uSelEnIn,
                                      unsigned int             uFilterEnIn,
                                      HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio35Cfg,
                                      HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio36Cfg,
                                      HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio40Cfg,
                                      HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio47Cfg,
                                      HIF_IOCTRL_IRQ_PIO_CFG_E eIrqPio72Cfg,
                                      HIF_IOCTRL_IRQ_PIO_CFG_E eIrqD12Cfg,
                                      unsigned int             uFilterIrqs,
                                      unsigned int             uNetx50PioRegComp
                                    );
void     HALDEC(HIF_IOCTRL_SetLine)( unsigned int uOutRegNum, uint32_t ulVal );
uint32_t HALDEC(HIF_IOCTRL_GetLine)( unsigned int uOutRegNum );
void     HALDEC(HIF_IOCTRL_SetOe)( unsigned int uOeRegNum, uint32_t ulVal );
uint32_t HALDEC(HIF_IOCTRL_GetOe)( unsigned int uOeRegNum );
uint32_t HALDEC(HIF_IOCTRL_GetIn)( unsigned int uInRegNum );
void     HALDEC(HIF_IOCTRL_IrqEnable)( uint32_t ulPioNum );
void     HALDEC(HIF_IOCTRL_IrqDisable)( uint32_t ulPioNum );
int      HALDEC(HIF_IOCTRL_GetIrq)( uint32_t ulPioNum );
uint32_t HALDEC(HIF_IOCTRL_GetIrqMasked)( void );
void     HALDEC(HIF_IOCTRL_IrqReset)( uint32_t ulPioNum );
int      HALDEC(HIF_IOCTRL_SetParam)( HIF_IOCTRL_PRM_E ePrmID, uint32_t ulPrmVal );
uint32_t HALDEC(HIF_IOCTRL_GetParam)( HIF_IOCTRL_PRM_E ePrmID );


#endif /* __HAL_HIF_IOCTRL_H */
