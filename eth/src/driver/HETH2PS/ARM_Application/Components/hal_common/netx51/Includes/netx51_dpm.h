#ifndef __NETX51_DPM_H
#define __NETX51_DPM_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX51_NUM_DPM_WINDOWS     4 /**< Number of DPM windows */

/* DPM parameters (Hilscher standard layout as host sight) */
#define NX51_NETX_DPM_START_ADDR      Addr_NX51_intramhs_dpm_mirror
#define NX51_OFFSET_CH_SYSTEM         0
#define NX51_SIZE_CH_SYSTEM           0x0200
#define NX51_OFFSET_CH_HS             NX51_OFFSET_CH_SYSTEM + NX51_SIZE_CH_SYSTEM
#define NX51_SIZE_CH_HS               0x0100
#define NX51_OFFSET_CH_COM0           NX51_OFFSET_CH_HS + NX51_SIZE_CH_HS
#define NX51_SIZE_CH_COM0             0x1000
#define NX51_OFFSET_AREA_OUT0         NX51_OFFSET_CH_COM0 + NX51_SIZE_CH_COM0
#define NX51_SIZE_AREA_OUT0           0x1680
#define NX51_OFFSET_AREA_IN0          NX51_OFFSET_AREA_OUT0 + NX51_SIZE_AREA_OUT0
#define NX51_SIZE_AREA_IN0            0x1680

/* sizes for DPM areas as netX sight within INTRAM 5 */
#define NX51_SIZE_CH_SYSTEM_NETX      0x0200
#define NX51_SIZE_CH_HS_NETX          0x0100
#define NX51_SIZE_CH_COM0_NETX        0x1000
#define NX51_SIZE_AREA_OUT0_NETX      0x0600
#define NX51_SIZE_AREA_IN0_NETX       0x0600

/* DPM structure (Hilscher standard layout) */
typedef struct NX51_DPM_STD_LAYOUT_Ttag
{
  volatile uint8_t abSystemCh[NX51_SIZE_CH_SYSTEM_NETX];
  volatile uint8_t abHandshakeCh[NX51_SIZE_CH_HS_NETX];
  volatile uint8_t abCom0Ch[NX51_SIZE_CH_COM0_NETX];
  volatile uint8_t abOutDataArea0[NX51_SIZE_AREA_OUT0_NETX];
  volatile uint8_t abInDataArea0[NX51_SIZE_AREA_IN0_NETX];
} NX51_DPM_STD_LAYOUT_T;

/*****************************************************************************/
/*! Configuration DPM access mode                                            */
/*****************************************************************************/
typedef enum NX51_DPM_MODE_Etag {
  NX51_DPM_MODE_SERIAL        =  0, /**< Default because not used in serial DPM mode */
  NX51_DPM_MODE_8BIT_NON_MUX  =  0, /**< 8 bit data non multiplexed mode  */
  NX51_DPM_MODE_8BIT_MUX      =  2, /**< 8 bit data multiplexed mode      */
  NX51_DPM_MODE_16BIT_NON_MUX =  4, /**< 16 bit bit data non multiplexed mode */
  NX51_DPM_MODE_16BIT_MUX     =  6, /**< 16 bit data multiplexed mode with 2 byte-enables on separated lines */
  NX51_DPM_MODE_32BIT_NON_MUX =  8, /**< 32 bit data non multiplexed mode */
  NX51_DPM_MODE_32BIT_MUX     = 10  /**< 32 bit data multiplexed mode with 4 byte-enables on separated lines */
} NX51_DPM_MODE_E;

/*****************************************************************************/
/*! Configuration DPM ENDIANESS                                              */
/*****************************************************************************/
typedef enum NX51_DPM_ENDIANESS_Etag {
  NX51_DPM_ENDIANESS_LITTLE     =  0, /**< Little ENDIAN      */
  NX51_DPM_ENDIANESS_16BIT_BIG  =  1, /**< 16 bit big ENDIAN  */
  NX51_DPM_ENDIANESS_32BIT_BIG  =  2  /**< 32 bit big ENDIAN  */
} NX51_DPM_ENDIANESS_E;

/*****************************************************************************/
/*! Configuration DPM address range                                          */
/*****************************************************************************/
typedef enum NX51_DPM_ADDRRANGE_Etag {
  NX51_DPM_DPM_ADDRRANGE_RES0   =  0, /**< reserved     */
  NX51_DPM_DPM_ADDRRANGE_RES1   =  1, /**< reserved     */
  NX51_DPM_DPM_ADDRRANGE_2KB    =  2, /**< 2 kByte      */
  NX51_DPM_DPM_ADDRRANGE_4KB    =  3, /**< 4 kByte      */
  NX51_DPM_DPM_ADDRRANGE_8KB    =  4, /**< 8 kByte      */
  NX51_DPM_DPM_ADDRRANGE_16KB   =  5, /**< 16 kByte     */
  NX51_DPM_DPM_ADDRRANGE_32KB   =  6, /**< 32 kByte     */
  NX51_DPM_DPM_ADDRRANGE_64KB   =  7, /**< 64 kByte     */
  NX51_DPM_DPM_ADDRRANGE_128KB  =  8, /**< 128 kByte    */
  NX51_DPM_DPM_ADDRRANGE_256KB  =  9, /**< 256 kByte    */
  NX51_DPM_DPM_ADDRRANGE_512KB  = 10, /**< 512 kByte    */
  NX51_DPM_DPM_ADDRRANGE_1024KB = 11  /**< 1024 kByte   */
} NX51_DPM_ADDRRANGE_E;

/*****************************************************************************/
/*! Configuration DPM external address range configuration                   */
/*****************************************************************************/
typedef enum NX51_DPM_CFGWINADDRCFG_Etag {
  NX51_DPM_CFGWINADDRCFG_FIRST  =  0, /**< Configuration window located at first 256 bytes of external DPM address range */
  NX51_DPM_CFGWINADDRCFG_LAST   =  1, /**< Configuration window located at last 256 bytes of external DPM address range  */
  NX51_DPM_CFGWINADDRCFG_RES    =  2, /**< reserved     */
  NX51_DPM_CFGWINADDRCFG_DIS    =  3  /**< Configuration window disabled      */
} NX51_DPM_CFGWINADDRCFG_E;

/*****************************************************************************/
/*! Configuration DPM RDY Polarity                                           */
/*****************************************************************************/
typedef enum NX51_DPM_RDYPOL_Etag {
  NX51_DPM_RDYPOL_BUSY_WHEN_HI  =  0, /**< DPM is busy when external RDY-signal is high */
  NX51_DPM_RDYPOL_READY_WHEN_HI =  1  /**< DPM is ready when external RDY-signal is high */
} NX51_DPM_RDYPOL_E;

/*****************************************************************************/
/*! Configuration DPM RDY signal mode                                        */
/*****************************************************************************/
typedef enum NX51_DPM_RDYMODE_Etag {
  NX51_DPM_RDYMODE_WAITBUSY     =  0, /**< Wait/Busy */
  NX51_DPM_RDYMODE_RDYACK       =  1  /**< RDY/ACK   */
} NX51_DPM_RDYMODE_E;

/*****************************************************************************/
/*! Configuration DPM DRV mode                                               */
/*****************************************************************************/
typedef enum NX51_DPM_RDYDRV_Etag {
  NX51_DPM_RDYDRV_DIS              = 0, /**< Disabled */
  NX51_DPM_RDYDRV_PUSH_PULL        = 1, /**< PushPull */
  NX51_DPM_RDYDRV_SUSTAIN_TRISTATE = 2, /**< Sustain TRI-state */
  NX51_DPM_RDYDRV_OPEN_DRAIN       = 3  /**< open-Drain */
} NX51_DPM_RDYDRV_E;

/*****************************************************************************/
/*! Configuration DPM RDY TIMEOUT                                            */
/*****************************************************************************/
typedef enum NX51_DPM_RDYTO_CFG_Etag {
  NX51_DPM_RDYTO_CFG_2048CC        = 0, /**< Ready Timeout after 2048 system clock cycles (20.48µs) */
  NX51_DPM_RDYTO_CFG_256CC         = 1, /**< Ready Timeout after 256 system clock cycles (i.e. 2.56us) */
  NX51_DPM_RDYTO_CFG_RES           = 2, /**< Reserved */
  NX51_DPM_RDYTO_CFG_DIS           = 3  /**< Ready Timeout disabled */
} NX51_DPM_RDYTO_CFG_E;

/*****************************************************************************/
/*! Configuration for Window Alternative Address Mapping Configuration       */
/*****************************************************************************/
typedef enum NX51_DPM_WINMAP_ALT_Etag {
  NX51_DPM_WINMAP_ALT_DIS          = 0, /**< 00: Alternative mapping disabled                                          */
  NX51_DPM_WINMAP_ALT_EN_BUFMAN_0  = 1, /**< 01: Alternative mapping enabled: Use triple buffer manager 0 from HANDSHAKE_CTRL unit. */
  NX51_DPM_WINMAP_ALT_EN_BUFMAN_1  = 2, /**< 01: Alternative mapping enabled: Use triple buffer manager 1 from HANDSHAKE_CTRL unit. */
  NX51_DPM_WINMAP_ALT_RES          = 3  /**< 11: reserved */
} NX51_DPM_WINMAP_ALT_E;

/*****************************************************************************/
/*! Interrupt status types                                                   */
/*****************************************************************************/
typedef enum NX51_DPM_IRQ_STATUS_Etag {
  NX51_DPM_IRQ_STATUS_RAW          = 0, /**< dpm_irq_raw                     */
  NX51_DPM_IRQ_STATUS_ARM_MASKED      , /**< dpm_irq_arm_masked            . */
  NX51_DPM_IRQ_STATUS_XPIC_MASKED     , /**< dpm_irq_xpic_masked           . */
  NX51_DPM_IRQ_STATUS_FIQ_MASKED      , /**< dpm_irq_fiq_masked              */
  NX51_DPM_IRQ_STATUS_IRQ_MASKED      , /**< dpm_irq_irq_masked              */
  NX51_DPM_IRQ_STATUS_FW_IRQ_RAW        /**< dpm_firmware_irq_raw            */
} NX51_DPM_IRQ_STATUS_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX51_DPM_CfgDpm( NX51_DPM_MODE_E eMode, NX51_DPM_ENDIANESS_E eEndianess, NX51_DPM_ADDRRANGE_E eAddrRange, NX51_DPM_CFGWINADDRCFG_E eCfgWinAddrCfg );
int  NX51_DPM_CfgTiming( unsigned int uTosa, unsigned int uFilter, unsigned int uRds, unsigned int uRdBurstEn );
void NX51_DPM_CfgRdy( unsigned int uRdypol, NX51_DPM_RDYDRV_E eRdyDrv, NX51_DPM_RDYMODE_E eRdyMode );
void NX51_DPM_CfgPio( uint32_t ulSelDPio, uint32_t ulSelMiscPio );
void NX51_DPM_CfgMisc( uint32_t ulMiscCfg, uint32_t ulIoMiscCfg );
int  NX51_DPM_CfgDpmWindow( unsigned int uWinNum, uint32_t uSize, uint32_t ulMapStartAddr, NX51_DPM_WINMAP_ALT_E eAltMapping );
int  NX51_DPM_SetupByteAccess( unsigned int uWinNum, bool fByteAccess );
int  NX51_DPM_SetupReadAhead( unsigned int uWinNum, bool fReadAhead );
int  NX51_DPM_SetupReadLatch( unsigned int uWinNum, bool fReadLatch );
void NX51_DPM_SetupIrq( uint32_t ulIrqMsk, uint32_t ulFiqMsk, uint32_t ulArmMsk, uint32_t ulXpicMsk, uint32_t ulFwIrqMask );
void NX51_DPM_GetStatusErr( uint32_t *pulStatErr, uint32_t *pulStatErrAddr );
void NX51_DPM_ClearErr( uint32_t ulClearMask );
int  NX51_DPM_GetIrq( NX51_DPM_IRQ_STATUS_E eIrqType, uint32_t *pulIrq );
void NX51_DPM_Reset( void );
void NX51_DPM_GetSysSta( uint32_t *pulSysSta, uint32_t *pulSysStaBigEnd16 );
void NX51_DPM_GetNetxVersion( uint32_t *pulNetxVersion, uint32_t *pulNetxVersionBigEnd16 );

#endif /* #ifndef __NETX51_DPM_H */
