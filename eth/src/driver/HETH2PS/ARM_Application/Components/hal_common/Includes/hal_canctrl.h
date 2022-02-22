#ifndef __HAL_CANCTRL_H
#define __HAL_CANCTRL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define CANCTRL_BAUDRATE_1000K   1000
#define CANCTRL_BAUDRATE_800K     800
#define CANCTRL_BAUDRATE_500K     500
#define CANCTRL_BAUDRATE_250K     250
#define CANCTRL_BAUDRATE_125K     125
#define CANCTRL_BAUDRATE_50K       50
#define CANCTRL_BAUDRATE_20K       20
#define CANCTRL_BAUDRATE_10K       10

#define CANCTRL_OVERSAMPLING_EN    1
#define CANCTRL_OVERSAMPLING_DIS   0

/* Interrupt Enable register */
#define MSK_CANCTRL_IRQ_EN_RX_IRQ_EN          0x00000001U
#define MSK_CANCTRL_IRQ_EN_TX_IRQ_EN          0x00000002U
#define MSK_CANCTRL_IRQ_EN_WARNING_IRQ_EN     0x00000004U
#define MSK_CANCTRL_IRQ_EN_OVERRUN_IRQ_EN     0x00000008U
#define MSK_CANCTRL_IRQ_EN_ERR_PASSIVE_IRQ_EN 0x00000020U
#define MSK_CANCTRL_IRQ_EN_ARB_LOST_IRQ_EN    0x00000040U
#define MSK_CANCTRL_IRQ_EN_BUS_ERROR_IRQ_EN   0x00000080U

/* Interrupt register */
#define MSK_CANCTRL_IRQ_RX_IRQ          0x00000001U
#define MSK_CANCTRL_IRQ_TX_IRQ          0x00000002U
#define MSK_CANCTRL_IRQ_WARNING_IRQ     0x00000004U
#define MSK_CANCTRL_IRQ_OVERRUN_IRQ     0x00000008U
#define MSK_CANCTRL_IRQ_ERR_PASSIVE_IRQ 0x00000020U
#define MSK_CANCTRL_IRQ_ARB_LOST_IRQ    0x00000040U
#define MSK_CANCTRL_IRQ_BUS_ERROR_IRQ   0x00000080U

/* Mode register */
#define MSK_CANCTRL_MODE_RESET_MODE      0x00000001U
#define MSK_CANCTRL_MODE_LISTEN_MODE     0x00000002U
#define MSK_CANCTRL_MODE_SELFTEST        0x00000004U
#define MSK_CANCTRL_MODE_ACCEPTANCE_MODE 0x00000008U

/* Status register */
#define MSK_CANCTRL_STATUS_RX_BUF_STATUS 0x00000001U
#define MSK_CANCTRL_STATUS_OVERRUN       0x00000002U
#define MSK_CANCTRL_STATUS_TX_BUF_STATUS 0x00000004U
#define MSK_CANCTRL_STATUS_TX_COMPLETE   0x00000008U
#define MSK_CANCTRL_STATUS_RX_STATUS     0x00000010U
#define MSK_CANCTRL_STATUS_TX_STATUS     0x00000020U
#define MSK_CANCTRL_STATUS_ERROR_STATUS  0x00000040U
#define MSK_CANCTRL_STATUS_BUS_STATUS    0x00000080U

/*****************************************************************************/
/*! CAN Single Filter Structure                                              */
/*****************************************************************************/
typedef struct CANCTRL_SINGLEFILTER_Ttag
{
  uint32_t ulAcceptanceCode; /**< Acceptance Code */
  uint32_t ulAcceptanceMask; /**< Acceptance Mask */
} CANCTRL_SINGLEFILTER_T;

/*****************************************************************************/
/*! CAN Dual Filter Structure                                                */
/*****************************************************************************/
typedef struct CANCTRL_DUALFILTER_Ttag
{
  uint16_t usAcceptanceCode0; /**< Acceptance Code 0 */
  uint16_t usAcceptanceMask0; /**< Acceptance Mask 0 */
  uint16_t usAcceptanceCode1; /**< Acceptance Code 1 */
  uint16_t usAcceptanceMask1; /**< Acceptance Mask 1 */
} CANCTRL_DUALFILTER_T;

/*****************************************************************************/
/*! CAN Message Structure                                                    */
/*****************************************************************************/
typedef struct CANCTRL_MESSAGE_Ttag
{
  unsigned int uFrameFormat;   /**< 1/0: Extended/Standard identifier */
  unsigned int uRtr;           /**< 1/0: Remote / Data Frame          */
  unsigned int uDlc;           /**< Data Length Code                  */
  uint32_t     ulIdentifier;   /**< 11 bits for standard frame, 29 bits for extended frame, LSB aligned */
  uint8_t      abData[8];      /**< Frame Data                        */
} CANCTRL_MESSAGE_T;

/*****************************************************************************/
/*! Result Codes for CAN Controller Functions                                */
/*****************************************************************************/
typedef enum CANCTRL_RESULTtag
{
  CANCTRL_RESULT_OKAY = 0,             /**< Successful                                      */
  CANCTRL_RESULT_INVAL_PARAM,          /**< Invalid parameter                               */
  CANCTRL_RESULT_NO_MESSAGE_AVAILABLE, /**< No message in receive buffer                    */
  CANCTRL_RESULT_MESSAGE_DROPPED,      /**< Message dropped due low receive buffer          */
  CANCTRL_RESULT_TX_BUFFER_NOT_FREE,   /**< No free transmission buffer for new message     */
  CANCTRL_RESULT_WRONG_MODE            /**< Operation only permitted in reset mode but core is in operating mode or vice versa  */
} CANCTRL_RESULT;

/*****************************************************************************/
/*! Parameters IDs                                                           */
/*****************************************************************************/
typedef enum CANCTRL_PRM_Etag {
  CANCTRL_PRM_IRQEN = 0,
  CANCTRL_PRM_BT0,
  CANCTRL_PRM_BT1,
  CANCTRL_PRM_ERR_WARNING_LIMIT,
  CANCTRL_PRM_RXERR_CNT,
  CANCTRL_PRM_TXERR_CNT,
  CANCTRL_PRM_ACCEPTANCE_CODE0,
  CANCTRL_PRM_ACCEPTANCE_CODE1,
  CANCTRL_PRM_ACCEPTANCE_CODE2,
  CANCTRL_PRM_ACCEPTANCE_CODE3,
  CANCTRL_PRM_ACCEPTANCE_MASK0,
  CANCTRL_PRM_ACCEPTANCE_MASK1,
  CANCTRL_PRM_ACCEPTANCE_MASK2,
  CANCTRL_PRM_ACCEPTANCE_MASK3
} CANCTRL_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

CANCTRL_RESULT        CanCtrl_EnterResetMode( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_LeaveResetMode( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_EnterListenMode( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_LeaveListenMode( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_EnterSelftestMode( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_LeaveSelftestMode( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_SetBaudrate( unsigned int uiInst, uint32_t ulBaudrate, unsigned int uOversampling );
CANCTRL_RESULT        CanCtrl_SetSingleAcceptanceFilter( unsigned int uiInst, CANCTRL_SINGLEFILTER_T* ptFilter );
CANCTRL_RESULT        CanCtrl_GetSingleAcceptanceFilter( unsigned int uiInst, CANCTRL_SINGLEFILTER_T* ptFilter );
CANCTRL_RESULT        CanCtrl_SetDualAcceptanceFilter( unsigned int uiInst, CANCTRL_DUALFILTER_T* ptFilter );
CANCTRL_RESULT        CanCtrl_GetDualAcceptanceFilter( unsigned int uiInst, CANCTRL_DUALFILTER_T* ptFilter );
CANCTRL_RESULT        CanCtrl_SendMessage( unsigned int uiInst, CANCTRL_MESSAGE_T* ptMessage, bool fSingleShot, bool fSelfRx );
CANCTRL_RESULT        CanCtrl_AbortMessage( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_ReceiveMessage( unsigned int uiInst, CANCTRL_MESSAGE_T* ptMessage );
uint32_t              CanCtrl_GetRxMessageCnt( unsigned int uiInst );
uint32_t              CanCtrl_GetMode( unsigned int uiInst );
uint32_t              CanCtrl_GetStatus( unsigned int uiInst );
uint32_t              CanCtrl_GetAndClearIrqStatus( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_SetIrqMask( unsigned int uiInst, uint32_t ulIrqMask );
uint32_t              CanCtrl_GetIrqMask( unsigned int uiInst );
uint32_t              CanCtrl_GetAndClearArblostStatus( unsigned int uiInst );
uint32_t              CanCtrl_GetAndClearErrorCode( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_SetWarningLimit( unsigned int uiInst, uint32_t ulLimit );
uint32_t              CanCtrl_GetWarningLimit( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_SetRxErrorCnt( unsigned int uiInst, uint32_t ulCnt );
uint32_t              CanCtrl_GetRxErrorCnt( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_SetTxErrorCnt( unsigned int uiInst, uint32_t ulCnt );
uint32_t              CanCtrl_GetTxErrorCnt( unsigned int uiInst );
CANCTRL_RESULT        CanCtrl_SetPrm( unsigned int uiInst, CANCTRL_PRM_E ePrmID, uint32_t ulPrmVal );
CANCTRL_RESULT        CanCtrl_GetPrm( unsigned int uiInst, CANCTRL_PRM_E ePrmID, uint32_t* pulPrmVal );

#endif /* #ifndef __HAL_CANCTRL_H */
