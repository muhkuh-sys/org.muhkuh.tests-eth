#ifndef __HAL_DPM_H
#define __HAL_DPM_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */


/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NUM_DPM_WINDOWS     4 /**< Number of DPM windows */

/*****************************************************************************/
/*! Configuration DPM access mode                                            */
/*****************************************************************************/
typedef enum
{
  DPM_MODE_SERIAL        =  0, /**< Default because not used in serial DPM mode */
  DPM_MODE_8BIT_NON_MUX  =  0, /**< 8 bit data non multiplexed mode  */
  DPM_MODE_8BIT_MUX      =  2, /**< 8 bit data multiplexed mode      */
  DPM_MODE_16BIT_NON_MUX =  4, /**< 16 bit bit data non multiplexed mode */
  DPM_MODE_16BIT_MUX     =  6, /**< 16 bit data multiplexed mode with 2 byte-enables on separated lines */
  DPM_MODE_32BIT_NON_MUX =  8, /**< 32 bit data non multiplexed mode */
  DPM_MODE_32BIT_MUX     = 10  /**< 32 bit data multiplexed mode with 4 byte-enables on separated lines */
} DPM_MODE_E;


/*****************************************************************************/
/*! Configuration DPM ENDIANESS                                              */
/*****************************************************************************/
typedef enum
{
  DPM_ENDIANESS_LITTLE     = 0, /**< Little ENDIAN      */
  DPM_ENDIANESS_16BIT_BIG  = 1, /**< 16 bit big ENDIAN  */
  DPM_ENDIANESS_32BIT_BIG  = 2  /**< 32 bit big ENDIAN  */
} DPM_ENDIANESS_E;


/*****************************************************************************/
/*! Configuration DPM address range                                          */
/*****************************************************************************/
typedef enum
{
  DPM_ADDRRANGE_RES0   =  0, /**< reserved     */
  DPM_ADDRRANGE_RES1   =  1, /**< reserved     */
  DPM_ADDRRANGE_2KB    =  2, /**< 2 kByte      */
  DPM_ADDRRANGE_4KB    =  3, /**< 4 kByte      */
  DPM_ADDRRANGE_8KB    =  4, /**< 8 kByte      */
  DPM_ADDRRANGE_16KB   =  5, /**< 16 kByte     */
  DPM_ADDRRANGE_32KB   =  6, /**< 32 kByte     */
  DPM_ADDRRANGE_64KB   =  7, /**< 64 kByte     */
  DPM_ADDRRANGE_128KB  =  8, /**< 128 kByte    */
  DPM_ADDRRANGE_256KB  =  9, /**< 256 kByte    */
  DPM_ADDRRANGE_512KB  = 10, /**< 512 kByte    */
  DPM_ADDRRANGE_1024KB = 11  /**< 1024 kByte   */
} DPM_ADDRRANGE_E;

/*****************************************************************************/
/*! Configuration DPM external address range configuration                   */
/*****************************************************************************/
typedef enum
{
  DPM_CFGWINADDRCFG_FIRST = 0, /**< Configuration window located at first 256 bytes of external DPM address range */
  DPM_CFGWINADDRCFG_LAST  = 1, /**< Configuration window located at last 256 bytes of external DPM address range  */
  DPM_CFGWINADDRCFG_RES   = 2, /**< reserved     */
  DPM_CFGWINADDRCFG_DIS   = 3  /**< Configuration window disabled      */
} DPM_CFGWINADDRCFG_E;

/*****************************************************************************/
/*! Configuration DPM RDY Polarity                                           */
/*****************************************************************************/
typedef enum
{
  DPM_RDYPOL_BUSY_WHEN_HI  = 0, /**< DPM is busy when external RDY-signal is high */
  DPM_RDYPOL_READY_WHEN_HI = 1  /**< DPM is ready when external RDY-signal is high */
} DPM_RDYPOL_E;

/*****************************************************************************/
/*! Configuration DPM RDY signal mode                                        */
/*****************************************************************************/
typedef enum
{
  DPM_RDYMODE_WAITBUSY =  0, /**< Wait/Busy */
  DPM_RDYMODE_RDYACK   =  1  /**< RDY/ACK   */
} DPM_RDYMODE_E;

/*****************************************************************************/
/*! Configuration DPM DRV mode                                               */
/*****************************************************************************/
typedef enum
{
  DPM_RDYDRV_DIS              = 0, /**< Disabled */
  DPM_RDYDRV_PUSH_PULL        = 1, /**< PushPull */
  DPM_RDYDRV_SUSTAIN_TRISTATE = 2, /**< Sustain TRI-state */
  DPM_RDYDRV_OPEN_DRAIN       = 3  /**< open-Drain */
} DPM_RDYDRV_E;

/*****************************************************************************/
/*! Configuration DPM RDY TIMEOUT                                            */
/*****************************************************************************/
typedef enum
{
  DPM_RDYTO_CFG_2048CC = 0, /**< Ready Timeout after 2048 system clock cycles (20.48µs) */
  DPM_RDYTO_CFG_256CC  = 1, /**< Ready Timeout after 256 system clock cycles (i.e. 2.56us) */
  DPM_RDYTO_CFG_RES    = 2, /**< Reserved */
  DPM_RDYTO_CFG_DIS    = 3  /**< Ready Timeout disabled */
} DPM_RDYTO_CFG_E;

/*****************************************************************************/
/*! Configuration for Window Alternative Address Mapping Configuration       */
/*****************************************************************************/
typedef enum
{
  DPM_WINMAP_ALT_DIS         = 0, /**< 00: Alternative mapping disabled                                          */
  DPM_WINMAP_ALT_EN_BUFMAN_0 = 1, /**< 01: Alternative mapping enabled: Use triple buffer manager 0 from HANDSHAKE_CTRL unit. */
  DPM_WINMAP_ALT_EN_BUFMAN_1 = 2, /**< 01: Alternative mapping enabled: Use triple buffer manager 1 from HANDSHAKE_CTRL unit. */
  DPM_WINMAP_ALT_RES         = 3  /**< 11: reserved */
} DPM_WINMAP_ALT_E;

/*****************************************************************************/
/*! Interrupt status types                                                   */
/*****************************************************************************/
typedef enum
{
  DPM_IRQ_STATUS_RAW         = 0, /**< dpm_irq_raw                     */
  DPM_IRQ_STATUS_ARM_MASKED  = 1, /**< dpm_irq_arm_masked              */
  DPM_IRQ_STATUS_XPIC_MASKED = 2, /**< dpm_irq_xpic_masked             */
  DPM_IRQ_STATUS_FIQ_MASKED  = 3, /**< dpm_irq_fiq_masked              */
  DPM_IRQ_STATUS_IRQ_MASKED  = 4, /**< dpm_irq_irq_masked              */
  DPM_IRQ_STATUS_FW_IRQ_RAW  = 5  /**< dpm_firmware_irq_raw            */
} DPM_IRQ_STATUS_E;

/*****************************************************************************/
/*! DPM access direction control                                             */
/*****************************************************************************/
typedef enum
{
  DPM_DIR_CTRL_RDn_WRn = 0, /**< Low active read -and write control signals (RDn+WRn)        */
  DPM_DIR_CTRL_nRW     = 1, /**< RDn is direction signal nRW (signal high: write, low: read) */
  DPM_DIR_CTRL_nWR     = 2  /**< RDn is direction signal nWR (signal low: write, high: read) */
} DPM_DIR_CTRL_E;

/*****************************************************************************/
/*! DPM address latch control                                                */
/*****************************************************************************/
typedef enum
{
  DPM_AEN_CTRL_DIS               = 0, /**< No additional Address controlling function        */
  DPM_AEN_CTRL_32BIT_AEN_HIF_A15 = 1, /**< netX50 compatible Address controlling: 8/16Bit: AEN = HIF_D17, 32Bit: AEN = HIF_A15 */
  DPM_AEN_CTRL_32BIT_AEN_HIF_A17 = 2  /**< Not netX50 compatible Address controlling: 8/16Bit: AEN = HIF_D17, 32Bit: AEN = HIF_A17(HIF_AHI1) */
} DPM_AEN_CTRL_E;

/*****************************************************************************/
/*! DPM Chip-Select control                                                  */
/*****************************************************************************/
typedef enum
{
  DPM_CS_CTRL_LO_1 = 0, /**< Use 1 low active Chip-Select signal(DPM_CSN)                            */
  DPM_CS_CTRL_LO_2 = 1, /**< Use 2 low active Chip-Select signals(DPM_CSN or DPM_BHE1n must be low)   */
  DPM_CS_CTRL_HI_1 = 2, /**< Use 1 high active Chip-Select signal(DPM_CSN)                            */
  DPM_CS_CTRL_HI_2 = 3, /**< Use 2 high active Chip-Select signals(DPM_CSN or DPM_BHE1n must be high) */
  DPM_CS_CTRL_NO   = 4, /**< No Chip-Select signal. Behaves like DPM_CSN is permanent active          */
  DPM_CS_CTRL_DIS  = 7  /**< Chip access is disabled                                                  */
} DPM_CS_CTRL_E;

/*****************************************************************************/
/*! DPM Address comparator                                                   */
/*****************************************************************************/
typedef enum
{
  DPM_ADDRCCMP_A11 = 0, /**< Address comparator line a11  */
  DPM_ADDRCCMP_A12 = 1, /**< Address comparator line a12  */
  DPM_ADDRCCMP_A13 = 2, /**< Address comparator line a13  */
  DPM_ADDRCCMP_A14 = 3, /**< Address comparator line a14  */
  DPM_ADDRCCMP_A15 = 4, /**< Address comparator line a15  */
  DPM_ADDRCCMP_A16 = 5, /**< Address comparator line a16  */
  DPM_ADDRCCMP_A17 = 6, /**< Address comparator line a17  */
  DPM_ADDRCCMP_A18 = 7, /**< Address comparator line a18  */
  DPM_ADDRCCMP_A19 = 8  /**< Address comparator line a19  */
} DPM_ADDRCCMP_E;

/*****************************************************************************/
/*! DPM Address comparator                                                   */
/*****************************************************************************/
typedef enum
{
  DPM_ADDRCMPCODE_DIS     = 0, /**< Address comparator disabled                                                          */
  DPM_ADDRCMPCODE_EN_NETX = 1, /**< Address comparator enabled and compare with netX input (input state of DPM_SEL_a19)  */
  DPM_ADDRCMPCODE_EN_0    = 2, /**< Address comparator enabled and compare with logic 0                                  */
  DPM_ADDRCMPCODE_EN_1    = 3  /**< Address comparator enabled and compare with logic 1                                  */
} DPM_ADDRCMPCODE_E;

/*****************************************************************************/
/*! DPM IRQ Mask Setting                                                     */
/*****************************************************************************/
typedef enum
{
  DPM_IRQ_MASK_ARM  = 0,  /**< ARM IRQ mask  */
  DPM_IRQ_MASK_XPIC = 1,  /**< xPIC IRQ mask */
  DPM_IRQ_MASK_FIQ  = 2,  /**< FIQ IRQ mask  */
  DPM_IRQ_MASK_IRQ  = 3,  /**< IRQ IRQ mask  */
  DPM_IRQ_MASK_FW   = 4   /**< Firmware IRQ mask  */
} DPM_IRQ_MASK_E;

/*****************************************************************************/
/*! DPM Parameter IDs                                                        */
/*****************************************************************************/
typedef enum
{
  DPM_PRM_DPM_IF_CFG = 0,
  DPM_PRM_DPM_ADDR_CFG,
  DPM_PRM_DPM_TIMING_CFG,
  DPM_PRM_DPM_RDY_CFG,
  DPM_PRM_DPM_STATUS_ERR_RESET,
  DPM_PRM_DPM_STATUS_ERR_ADDR,
  DPM_PRM_DPM_MISC_CFG,
  DPM_PRM_DPM_IO_CFG_MISC,
  DPM_PRM_DPM_TUNNEL_CFG,
  DPM_PRM_DPM_ITBADDR,

  DPM_PRM_DPM_WIN1_END,
  DPM_PRM_DPM_WIN1_MAP,
  DPM_PRM_DPM_WIN2_END,
  DPM_PRM_DPM_WIN2_MAP,
  DPM_PRM_DPM_WIN3_END,
  DPM_PRM_DPM_WIN3_MAP,
  DPM_PRM_DPM_WIN4_END,
  DPM_PRM_DPM_WIN4_MAP,

  DPM_PRM_DPM_IRQ_RAW,
  DPM_PRM_DPM_IRQ_RAW1,

  DPM_PRM_DPM_IRQ_ARM_MASK_SET,
  DPM_PRM_DPM_IRQ_XPIC_MASK_SET,
  DPM_PRM_DPM_IRQ_SIRQ_MASK_SET,
  DPM_PRM_DPM_IRQ_SIRQ_MASK_SET1,
  DPM_PRM_DPM_IRQ_DIRQ_MASK_SET,
  DPM_PRM_DPM_IRQ_DIRQ_MASK_SET1,

  DPM_PRM_DPM_IRQ_ARM_MASK_RESET,
  DPM_PRM_DPM_IRQ_XPIC_MASK_RESET,
  DPM_PRM_DPM_IRQ_SIRQ_MASK_RESET,
  DPM_PRM_DPM_IRQ_SIRQ_MASK_RESET1,
  DPM_PRM_DPM_IRQ_DIRQ_MASK_RESET,
  DPM_PRM_DPM_IRQ_DIRQ_MASK_RESET1,

  DPM_PRM_DPM_IRQ_ARM_MASKED,
  DPM_PRM_DPM_IRQ_XPIC_MASKED,
  DPM_PRM_DPM_IRQ_SIRQ_MASKED,
  DPM_PRM_DPM_IRQ_SIRQ_MASKED1,
  DPM_PRM_DPM_IRQ_DIRQ_MASKED,
  DPM_PRM_DPM_IRQ_DIRQ_MASKED1,

  DPM_PRM_DPM_SW_IRQ,

  DPM_PRM_DPM_FW_IRQ_RAW,
  DPM_PRM_DPM_FW_IRQ_MASK,

  DPM_PRM_DPM_CRC
} DPM_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void     DPM_CfgDpm( unsigned int        uiInst,
                             DPM_MODE_E          eMode,
                             DPM_ENDIANESS_E     eEndianess,
                             DPM_ADDRRANGE_E     eAddrRange,
                             DPM_CFGWINADDRCFG_E eCfgWinAddrCfg );
void     DPM_SetCfg0x0( unsigned int        uiInst,
                                DPM_MODE_E      eMode,
                                DPM_ENDIANESS_E eEndianess );
void     DPM_SetAddrRange( unsigned int        uiInst,
                                   DPM_ADDRRANGE_E eAddrRange );
void     DPM_SetCfgWinAddrCfg( unsigned int        uiInst,
                                       DPM_CFGWINADDRCFG_E eCfgWinAddrCfg );
void     DPM_GetCfgDpm( unsigned int        uiInst,
                                uint32_t* pulCfg0x0,
                                uint32_t* pulAddrCfg );
int      DPM_CfgIf( unsigned int   uiInst,
                            DPM_DIR_CTRL_E eDirCtrl,
                            unsigned int   uBeSel,
                            unsigned int   uBeRdDis,
                            unsigned int   uBeWrDis,
                            unsigned int   uBePol,
                            DPM_AEN_CTRL_E eAenSel,
                            unsigned int   uAenPol,
                            unsigned int   uAddrSh,
                            DPM_CS_CTRL_E  eCsCtrl,
                            unsigned int   uIsaMemCs16n );
int      DPM_CfgTiming( unsigned int uiInst,
                                unsigned int uTosa,
                                unsigned int uFilter,
                                unsigned int uTrds,
                                unsigned int uRdBurstEn );
int      DPM_CfgRdy( unsigned int     uiInst,
                             DPM_RDYPOL_E    eRdypol,
                             DPM_RDYDRV_E    eRdyDrv,
                             DPM_RDYMODE_E   eRdyMode,
                             DPM_RDYTO_CFG_E eRdyToCfg );
void     DPM_CfgPio( unsigned int     uiInst, uint32_t ulSelDPio, uint32_t ulSelMiscPio );
void     DPM_GetCfgPio( unsigned int     uiInst, uint32_t* pulDPio, uint32_t* pulMiscPio );
void     DPM_CfgMisc( unsigned int     uiInst, uint32_t ulMiscCfg, uint32_t ulIoMiscCfg );
void     DPM_GetCfgMisc( unsigned int     uiInst, uint32_t* pulMiscCfg, uint32_t* pulIoMiscCfg );
int      DPM_CfgDpmWindow( unsigned int     uiInst, unsigned int uWinNum, uint32_t uSize, uint32_t ulMapStartAddr, DPM_WINMAP_ALT_E eAltMapping );
int      DPM_SetCfgDpmWindow( unsigned int     uiInst, unsigned int uWinNum, uint32_t ulSize, uint32_t ulMapStartAddr, DPM_WINMAP_ALT_E eAltMapping );
int      DPM_GetCfgDpmWindow( unsigned int     uiInst, unsigned int uWinNum, uint32_t* pulWinEnd, uint32_t* pulWinMap );
int      DPM_SetupByteAccess( unsigned int     uiInst, unsigned int uWinNum, bool fByteAccess );
int      DPM_SetupReadAhead( unsigned int     uiInst, unsigned int uWinNum, bool fReadAhead );
int      DPM_SetupReadLatch( unsigned int     uiInst, unsigned int uWinNum, bool fReadLatch );
void     DPM_GetStatusErr( unsigned int     uiInst, uint32_t *pulStatErr, uint32_t *pulStatErrAddr );
void     DPM_ClearErr( unsigned int     uiInst, uint32_t ulClearMask );
int      DPM_GetIrq( unsigned int     uiInst, DPM_IRQ_STATUS_E eIrqType, uint32_t *pulIrq );
void     DPM_Reset( unsigned int     uiInst );
void     DPM_SetSysSta( unsigned int     uiInst, uint32_t uSysSta );
void     DPM_GetSysSta( unsigned int     uiInst, uint32_t *pulSysSta, uint32_t *pulSysStaBigEnd16 );
void     DPM_GetNetxVersion( unsigned int     uiInst, uint32_t *pulNetxVersion, uint32_t *pulNetxVersionBigEnd16 );
int      DPM_CfgTunnel( unsigned int uiInst,
                                unsigned int uWpData,
                                unsigned int uWpITBAddr,
                                unsigned int uEnable,
                                unsigned int uTunnelAll,
                                unsigned int uByteArea,
                                unsigned int uDisRdLatch,
                                unsigned int uBase );
int      DPM_ITBAddr( unsigned int     uiInst, unsigned int uMap, unsigned int uBase );
void     DPM_SetResetRequest( unsigned int     uiInst, uint8_t bResetKey );
uint8_t  DPM_GetResetRequest( unsigned int     uiInst );
void     DPM_SetFwIrqRaw( unsigned int     uiInst, uint32_t ulFwIrqRaw );
int      DPM_AddrCmp( unsigned int     uiInst, DPM_ADDRCCMP_E eAddrCmpNr, DPM_ADDRCMPCODE_E eAddrCmpCode );
bool     DPM_IrqIsPending( unsigned int     uiInst,unsigned int uIrqId);
int      DPM_SetParam( unsigned int     uiInst, DPM_PRM_E ePrmID, uint32_t ulPrmVal );
uint32_t DPM_GetParam( unsigned int     uiInst, DPM_PRM_E ePrmID );


#endif /* #ifndef __HAL_DPM_H */
