#ifndef __NETX51_HSCTRL_H
#define __NETX51_HSCTRL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX51_NUM_HS_CELL    16 /**< Number of Handshake cells */
#define NX51_NUM_HS_BUFMAN   2 /**< Number of Handshake Buffer Managers */

/* netX communication flags 16-bit mode */
#define SRT_NX51_NCF_PD0_OUT_16BIT             6
#define SRT_NX51_NCF_PD0_IN_16BIT              7
#define MSK_NX51_NCF_PD0_OUT_16BIT             1<<SRT_NX51_NCF_PD0_OUT_16BIT
#define MSK_NX51_NCF_PD0_IN_16BIT              1<<SRT_NX51_NCF_PD0_IN_16BIT
/* host communication flags 16-bit mode */
#define SRT_NX51_HCF_PD0_OUT_16BIT            22
#define SRT_NX51_HCF_PD0_IN_16BIT             23
#define MSK_NX51_HCF_PD0_OUT_16BIT             1<<SRT_NX51_HCF_PD0_OUT_16BIT
#define MSK_NX51_HCF_PD0_IN_16BIT              1<<SRT_NX51_HCF_PD0_IN_16BIT
/* netX communication flags 8-bit mode */
#define SRT_NX51_NCF_PD0_OUT_8BIT             22
#define SRT_NX51_NCF_PD0_IN_8BIT              23
#define MSK_NX51_NCF_PD0_OUT_8BIT              1<<SRT_NX51_NCF_PD0_OUT_8BIT
#define MSK_NX51_NCF_PD0_IN_8BIT               1<<SRT_NX51_NCF_PD0_IN_8BIT
/* host communication flags 8-bit mode */
#define SRT_NX51_HCF_PD0_OUT_8BIT             30
#define SRT_NX51_HCF_PD0_IN_8BIT              31
#define MSK_NX51_HCF_PD0_OUT_8BIT              1<<SRT_NX51_HCF_PD0_OUT_8BIT
#define MSK_NX51_HCF_PD0_IN_8BIT               1<<SRT_NX51_HCF_PD0_IN_8BIT

/*****************************************************************************/
/*! Configuration Handshake width                                            */
/*****************************************************************************/
typedef enum NX51_HSCTRL_HSCELL_WIDTH_Etag {
  NX51_HSCTRL_HSCELL_WIDTH_8BIT   = 0, /**< 8 bit   */
  NX51_HSCTRL_HSCELL_WIDTH_16BIT  = 1  /**< 16 bit  */
} NX51_HSCTRL_HSCELL_WIDTH_E;

/*****************************************************************************/
/*! Interrupt sources                                                        */
/*****************************************************************************/
typedef enum NX51_HSCTRL_IRQ_SRC_Etag {
  NX51_HSCTRL_IRQ_SRC_DPM         = 1, /**< Interrupt source DPM   */
  NX51_HSCTRL_IRQ_SRC_ARM         = 2, /**< Interrupt source ARM   */
  NX51_HSCTRL_IRQ_SRC_XPIC        = 3  /**< Interrupt source xPIC  */
} NX51_HSCTRL_IRQ_SRC_E;

/*****************************************************************************/
/*! Handshake cell mode                                                      */
/*****************************************************************************/
typedef enum NX51_HSCTRL_CELL_MODE_Etag {
  NX51_HSCTRL_CELL_MODE_DIS       = 0, /**< Cell disabled                    */
  NX51_HSCTRL_CELL_MODE_DPM_ARM   = 1, /**< Cell for handshaking DPM<->ARM . */
  NX51_HSCTRL_CELL_MODE_DPM_XPIC  = 2, /**< Cell for handshaking DPM<->XPIC. */
  NX51_HSCTRL_CELL_MODE_ARM_XPIC  = 3  /**< Cell for handshaking ARM<->XPIC. */
} NX51_HSCTRL_CELL_MODE_E;

/*****************************************************************************/
/*! Handshake BufMan Address Mapping Configuration                           */
/*****************************************************************************/
typedef enum NX51_HSCTRL_BUFMAN_ADDRMAPCFG_Etag {
  NX51_HSCTRL_BUFMAN_ADDRMAPCFG_DPMWIN    =  0, /**< Use mapping from DPM window config    */
  NX51_HSCTRL_BUFMAN_ADDRMAPCFG_ALTWIN1   =  1, /**< Use alternate mapping 1               */
  NX51_HSCTRL_BUFMAN_ADDRMAPCFG_ALTWIN2   =  2, /**< Use alternate mapping 2               */
  NX51_HSCTRL_BUFMAN_ADDRMAPCFG_CURBUF    =  3  /**< Mapping controlled by BufMan          */
} NX51_HSCTRL_BUFMAN_ADDRMAPCFG_E;

/*****************************************************************************/
/*! Handshake buffer manager command master                                  */
/*****************************************************************************/
typedef enum NX51_HSCTRL_BUFMAN_CMDM_Etag {
  NX51_HSCTRL_BUFMAN_CMDM_NETX    = 0, /**< netX    */
  NX51_HSCTRL_BUFMAN_CMDM_HOST    = 1  /**< host    */
} NX51_HSCTRL_BUFMAN_CMDM_E;

/*****************************************************************************/
/*! Handshake buffer manager command                                         */
/*****************************************************************************/
typedef enum NX51_HSCTRL_BUFMAN_CMD_Etag {
  NX51_HSCTRL_BUFMAN_CMD_NOP             = 0, /**< Nop/Idle               */
  NX51_HSCTRL_BUFMAN_CMD_REQ_READ_BUF    = 1, /**< Request read buffer    */
  NX51_HSCTRL_BUFMAN_CMD_REQ_WRITE_BUF   = 2, /**< Request write buffer   */
  NX51_HSCTRL_BUFMAN_CMD_RELEASE_CUR_BUF = 3  /**< Release current buffer */
} NX51_HSCTRL_BUFMAN_CMD_E;

/*****************************************************************************/
/*! Handshake buffer manager status                                          */
/*****************************************************************************/
typedef enum NX51_HSCTRL_BUFMAN_STAT_Etag {
  NX51_HSCTRL_BUFMAN_STAT_BUF0_VALID   = 0, /**< Buffer 0 valid     */
  NX51_HSCTRL_BUFMAN_STAT_BUF1_VALID   = 1, /**< Buffer 1 valid     */
  NX51_HSCTRL_BUFMAN_STAT_BUF2_VALID   = 2, /**< Buffer 2 valid     */
  NX51_HSCTRL_BUFMAN_STAT_NO_BUF_VALID = 3  /**< No buffer valid  . */
} NX51_HSCTRL_BUFMAN_STAT_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int  NX51_HSCTRL_CfgHsBaseAddr( uint32_t ulHsBaseAddr );
int  NX51_HSCTRL_EnIrq( NX51_HSCTRL_IRQ_SRC_E tSrc, uint32_t ulIrqEn );
int  NX51_HSCTRL_DisIrq( NX51_HSCTRL_IRQ_SRC_E tSrc, uint32_t ulIrqDis );
int  NX51_HSCTRL_GetIrq( NX51_HSCTRL_IRQ_SRC_E tSrc, uint32_t *pulIrq );
int  NX51_HSCTRL_GetIrqRaw( NX51_HSCTRL_IRQ_SRC_E tSrc, uint32_t *pulIrq );
int  NX51_HSCTRL_ConfirmIrq( NX51_HSCTRL_IRQ_SRC_E tSrc, uint32_t ulConfirmIrqMask );
int  NX51_HSCTRL_CfgHsCell( unsigned int uInst, NX51_HSCTRL_CELL_MODE_E eHsCellmode, NX51_HSCTRL_HSCELL_WIDTH_E eHsCellWidth );
void NX51_HSCTRL_CfgBufmanPdOut( NX51_HSCTRL_BUFMAN_ADDRMAPCFG_E eAddrmapCfg, unsigned int uAutoMode, uint32_t ulWinMapBuf1, uint32_t ulWinMapBuf2 );
void NX51_HSCTRL_CfgBufmanPdIn( NX51_HSCTRL_BUFMAN_ADDRMAPCFG_E eAddrmapCfg, unsigned int uAutoMode, uint32_t ulWinMapBuf1, uint32_t ulWinMapBuf2 );
int  NX51_HSCTRL_ResetBufMan( unsigned int uInst );
int  NX51_HSCTRL_SetBufManCmd( unsigned int uInst, NX51_HSCTRL_BUFMAN_CMDM_E eCmdMaster, NX51_HSCTRL_BUFMAN_CMD_E eCmd );
int  NX51_HSCTRL_GetBufManStat( unsigned int uInst, NX51_HSCTRL_BUFMAN_STAT_E* peBufManStatNetx, NX51_HSCTRL_BUFMAN_STAT_E* peBufManStatHost );
int  NX51_HSCTRL_ReadHsCell( unsigned int uInst, uint32_t* pulHandshakeVal );
int  NX51_HSCTRL_WriteHsCell( unsigned int uInst, uint32_t ulHandshakeVal );
int  NX51_HSCTRL_ToggleHsCellFlag( unsigned int uInst, uint32_t ulHsCellBitNum );
int  NX51_HSCTRL_AreHsFlagsEqual( unsigned int uInst, uint32_t ulHsFlagNum1, uint32_t ulHsFlagNum2 );


#endif /* #ifndef __NETX51_HSCTRL_H */
