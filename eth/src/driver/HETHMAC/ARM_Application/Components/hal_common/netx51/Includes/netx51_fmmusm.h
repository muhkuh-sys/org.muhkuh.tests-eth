#ifndef __NETX6_FMMUSM_H
#define __NETX6_FMMUSM_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*! Parameters ID that can be set/get by NX51_FMMUSM_SetPrm()/NX51_FMMUSM_GetPrm() */
typedef enum NX51_FMMUSM_PRM_Etag {
  NX51_FMMUSM_PRM_PHYS_ADDR_OFFSET        =  1,
  NX51_FMMUSM_PRM_PHYS_ADDR_REG_OFFSET    =  2,
  NX51_FMMUSM_PRM_PHYS_LAST_ADDR          =  3,
  NX51_FMMUSM_PRM_XPEC_NR                 =  4,
  NX51_FMMUSM_PRM_SM_BUF_STATCFG          =  5,
  NX51_FMMUSM_PRM_SM_READ_EVENT_VECTOR    =  6,
  NX51_FMMUSM_PRM_SM_WRITE_EVENT_VECTOR   =  7,
  NX51_FMMUSM_PRM_SM_FIRST_BYTE_ADDRESSED =  8,
  NX51_FMMUSM_PRM_SM_LAST_BYTE_ADDRESSED  =  9,
  NX51_FMMUSM_PRM_SM_SERVED               = 10
} NX51_FMMUSM_PRM_E;

#define NX51_SYNCMAN_CH_MAX_NUM 8
#define NX51_FMMU_CH_MAX_NUM 8

/*! FMMU channel configuration */
typedef struct NX51_FMMUSM_FMMUCFGtag
{
  uint32_t ulLogStartAddr;                   /**< Logical start address */
  uint16_t usLen;                            /**< Length */
  uint8_t  bLogStartBit;                     /**< Logical start bit */
  uint8_t  bLogStopBit;                      /**< logical stop bit */
  uint16_t usPhyStartAddr;                   /**< Physical start address */
  uint8_t  bPhyStartBit;                     /**< Physical start bit */
  uint8_t  bRdEn;                            /**< Read enable */
  uint8_t  bWrEn;                            /**< Write enable */
  bool     fEn;                              /**< Enable */
} NX51_FMMUSM_FMMUCFG;

/*! Sync Manager channel configuration */
typedef struct NX51_FMMUSM_SMCFGtag
{
  uint16_t usPhyStartAddr;                   /**< Physical start address */
  uint16_t usLen;                            /**< Length */
  uint8_t  bAdd;                             /**< Addition value for offset calculation in 3P mode, not available in netx5/50 */
  uint8_t  bMode;                            /**< Operation mode 2/0: Mailbox/Buffered */
  uint8_t  bDir;                             /**< Direction 1/0: EtherCAT writes, PDI reads / EtherCAT reads, PDI writes */
  bool     fEn;                              /**< Enable */
} NX51_FMMUSM_SMCFG;


/***********************************************************************************************/
/* Function Prototypes                                                                         */
/***********************************************************************************************/

int           NX51_FMMUSM_SetPrm( NX51_FMMUSM_PRM_E uPrmID, uint32_t ulVal );
uint32_t      NX51_FMMUSM_GetPrm( NX51_FMMUSM_PRM_E uPrmID );

int           NX51_FMMUSM_SetFmmuCfg ( unsigned int uChNum, NX51_FMMUSM_FMMUCFG* ptCfg );
int           NX51_FMMUSM_SetSmCfg ( unsigned int uChNum, NX51_FMMUSM_SMCFG* ptCfg );
int           NX51_FMMUSM_GetSmCfg ( unsigned int uChNum, NX51_FMMUSM_SMCFG* ptCfg );
int           NX51_FMMUSM_IsSmEnabled ( unsigned int uChNum );
int           NX51_FMMUSM_DisSmChByPdi ( unsigned int uChNum, bool fDis );
int           NX51_FMMUSM_IsSmDisabledByPdi ( unsigned int uChNum );

int           NX51_FMMUSM_SetWriteEvent ( unsigned int uChNum, unsigned int uVal );
int           NX51_FMMUSM_SetReadEvent ( unsigned int uChNum, unsigned int uVal );
int           NX51_FMMUSM_SetBufFull ( unsigned int uChNum, unsigned int uVal );
int           NX51_FMMUSM_GetWriteEvent ( unsigned int uChNum );
int           NX51_FMMUSM_GetReadEvent ( unsigned int uChNum );
int           NX51_FMMUSM_GetBufFull ( unsigned int uChNum );


#endif /* #ifndef __NETX51_FMMUSM_H */
