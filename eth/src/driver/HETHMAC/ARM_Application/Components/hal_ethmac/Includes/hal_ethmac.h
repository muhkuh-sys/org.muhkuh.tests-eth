#ifndef _HAL_ETH_MAC_H
#define _HAL_ETH_MAC_H

#include "compiler_spec.h"

#define MSK_ETHMAC_IRQ_IND_HI        0x00000001U
#define MSK_ETHMAC_IRQ_IND_LO        0x00000002U
#define MSK_ETHMAC_IRQ_CON_HI        0x00000004U
#define MSK_ETHMAC_IRQ_CON_LO        0x00000008U
#define MSK_ETHMAC_IRQ_LINK_CHANGED  0x00000010U
#define MSK_ETHMAC_IRQ_COL           0x00000020U
#define MSK_ETHMAC_IRQ_EARLY_RCV     0x00000040U
#define MSK_ETHMAC_IRQ_RX_ERR        0x00000080U
#define MSK_ETHMAC_IRQ_TX_ERR        0x00000100U


/* INTRAM area structure */
typedef struct INTRAM_AREA_STRUCT
{
	unsigned long ulStart;
	unsigned long ulEnd;
} INTRAM_AREA_T;


/* MAC address structure */
typedef uint8_t ETHERNET_MAC_ADDR_T[6];

/*****************************************************************************/
/*! Ethernet Frame Structure                                                 */
/*****************************************************************************/
typedef __PACKED_PRE struct ETHERNET_FRAME_Ttag
{
  ETHERNET_MAC_ADDR_T     tDstAddr;        /**< Destination MAC address (DA)      */
  ETHERNET_MAC_ADDR_T     tSrcAddr;        /**< Source MAC address (SA)           */
  uint16_t                usType;          /**< Frame length/type (LT)            */
  uint8_t                 abData[1504];    /**< Frame data excluding DA,SA,LT,FCS */
  uint8_t                 abRes[18];       /**< reserved, shall be zero           */
  uint32_t                ulTimestampNs;   /**< receive time stamp [nanoseconds]  */
  uint32_t                ulTimestampS;    /**< receive time stamp [s]            */
} __PACKED_POST ETHERNET_FRAME_T;

/*****************************************************************************/
/*! Ethernet Counter Structure                                               */
/*****************************************************************************/
typedef struct ETHMAC_COUNTERS_Ttag
{
  uint32_t ulETHMAC_OUT_FRAMES_OKAY;                    /**< count of frames that are transmitted successfully */
  uint32_t ulETHMAC_OUT_OCTETS;                         /**< count of bytes transmitted (without Preamble, SFD and FCS) */
  uint32_t ulETHMAC_SINGLE_COLLISION_FRAMES;            /**< count of frames that are involved into a single collision */
  uint32_t ulETHMAC_MULTIPLE_COLLISION_FRAMES;          /**< count of frames that are involved into more than one collisions */
  uint32_t ulETHMAC_LATE_COLLISIONS;                    /**< count of the times that a collision has been detected later than 512 bit times into the transmitted packet */
  uint32_t ulETHMAC_LINK_DOWN_DURING_TRANSMISSION;      /**< count of the times that a frame was discarded during link down */
  uint32_t ulETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION;  /**< UTX FIFO underflow at transmission time */
  uint32_t ulETHMAC_IN_FRAMES_OKAY;                     /**< count of frames that are received without any error */
  uint32_t ulETHMAC_IN_OCTETS;                          /**< count of bytes in valid MAC frames received excluding Preamble, SFD and FCS */
  uint32_t ulETHMAC_FRAME_CHECK_SEQUENCE_ERRORS;        /**< count of frames that are an integral number of octets in length and do not pass the FCS check */
  uint32_t ulETHMAC_ALIGNMENT_ERRORS;                   /**< count of frames that are not an integral number of octets in length and do not pass the FCS check */
  uint32_t ulETHMAC_FRAME_TOO_LONG_ERRORS;              /**< count of frames that are received and exceed the maximum permitted frame size */
  uint32_t ulETHMAC_RUNT_FRAMES_RECEIVED;               /**< count of frames that have a length between 42..63 bytes and a valid CRC */
  uint32_t ulETHMAC_COLLISION_FRAGMENTS_RECEIVED;       /**< count of frames that are smaller than 64 bytes and have an invalid CRC */
  uint32_t ulETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE;    /**< no empty pointer available at indication time */
  uint32_t ulETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW;    /**< URX FIFO overflow at indication time */
  uint32_t ulETHMAC_TX_FATAL_ERROR;                     /**< counts unknown error numbers from TX xMAC, should never occur */
  uint32_t ulETHMAC_RX_FATAL_ERROR;                     /**< counts unknown error numbers from RX xMAC, should never occur */
} ETHMAC_COUNTERS_T;

/*****************************************************************************/
/*! Result Codes for Ethernet Functions                                      */
/*****************************************************************************/
typedef enum ETHERNET_RESULTtag
{
  ETH_OKAY,                               /**< Successful                                   */
  ETH_ERR_FIFO_EMPTY,                     /**< The FIFO is empty                            */
  ETH_ERR_INIT_FAILED,                    /**< Initialization has failed                    */
  ETH_ERR_INVALID_PARAMETER,              /**< Invalid parameter                            */
  ETH_ERR_TX_SUCCESSFUL_WITH_RETRIES,     /**< Transmission successful with retries         */
  ETH_ERR_TX_FAILED_LATE_COLLISION,       /**< Transmission failed due late collision       */
  ETH_ERR_TX_FAILED_LINK_DOWN_DURING_TX,  /**< Transmission failed due link down            */
  ETH_ERR_TX_FAILED_EXCESSIVE_COLLISION,  /**< Transmission failed due excessive collisions */
  ETH_ERR_TX_FAILED_UTX_UFL_DURING_TX,    /**< Transmission failed due UTX FIFO underflow   */
  ETH_ERR_TX_FAILED_FATAL_ERROR,          /**< Transmission failed due fatal error          */
  ETH_ERR_INVAL_STATE,                    /**< Invalid port state                           */
  ETH_ERR_OUT_OF_MEMORY                   /**< Not enough resources                         */
} ETHERNET_RESULT;

/*****************************************************************************/
/*! Link Status Structure                                                    */
/*****************************************************************************/
typedef struct ETHERNET_CONNECTION_STATE_Ttag
{
  unsigned int uSpeed;        /**< SPEED (100/10)                */
  unsigned int uIsLinkUp;     /**< LINK state   (!=0 -> Link UP) */
  unsigned int uIsFullDuplex; /**< DUPLEX state (!=0 -> FDX)     */
} ETHERNET_CONNECTION_STATE_T;

/*****************************************************************************/
/*! PHY LED Configuration                                                    */
/*****************************************************************************/
typedef enum ETHERNET_PHY_LED_CFGtag
{
  ETH_PHY_LED_STATIC   = 0,         /**< separate link and activity LEDs */
  ETH_PHY_LED_BLINK    = 1,         /**< separate link and activity LEDs, activity blinking when active */
  ETH_PHY_LED_SINGLE   = 2,         /**< single LED, combined link and blink on activity */
  ETH_PHY_LED_OFF      = 3          /**< PHY LEDs are disabled */
} ETHERNET_PHY_LED_CFG_E;

/*****************************************************************************/
/*! MAC addresses
* \description
*    Describes the different types of MAC addresses.                         */
/*****************************************************************************/
typedef enum
{
  ETH_MAC_ADDRESS_CHASSIS,         /**< Primary Chassis MAC address */
  ETH_MAC_ADDRESS_2ND_CHASSIS      /**< Secondary Chassis MAC address */
} ETH_MAC_ADDRESS_TYPE_E;


/******************************************************************************
 * function prototypes of Ethernet functions
 */

/* Control Service Class */
ETHERNET_RESULT EthMac_Initialize( unsigned int uiPort, ETHERNET_PHY_LED_CFG_E ePhyLedCfg, uint32_t ulActLedFlashPeriod, void* pvUser );
ETHERNET_RESULT EthMac_Start( unsigned int uiPort, void* pvUser );
ETHERNET_RESULT EthMac_SetMacAddr( unsigned int uiPort, ETH_MAC_ADDRESS_TYPE_E eType, const ETHERNET_MAC_ADDR_T tMacAddr );
ETHERNET_RESULT EthMac_SetTrafficClassArrangement( unsigned int uiPort, unsigned int uClass );
ETHERNET_RESULT EthMac_ModePromisc( unsigned int uiPort, unsigned int uEnable );
ETHERNET_RESULT EthMac_ConfirmIrq( unsigned int uiPort, uint32_t ulConfirmIrqMask );
ETHERNET_RESULT EthMac_SetIrqMask( unsigned int uiPort, uint32_t ulIrqMask );
ETHERNET_RESULT EthMac_SetLinkMode( unsigned int uiPort, bool fValid, unsigned int uiSpeed, bool fFdx );
ETHERNET_RESULT EthMac_CfgMii( unsigned int uiPort, unsigned int uiCfg );

#ifndef ETHMAC_RCV_ALL_MULTICASTS
ETHERNET_RESULT EthMac_AddGroupAddr( unsigned int uiPort, const ETHERNET_MAC_ADDR_T tMacAddr );
ETHERNET_RESULT EthMac_DeleteGroupAddr( unsigned int uiPort, const ETHERNET_MAC_ADDR_T tMacAddr );
#endif

/* Status Service Class */
ETHERNET_RESULT EthMac_GetMacAddr( unsigned int uiPort, ETH_MAC_ADDRESS_TYPE_E eType, ETHERNET_MAC_ADDR_T* ptMacAddr );
ETHERNET_RESULT EthMac_GetCounters( unsigned int uiPort, ETHMAC_COUNTERS_T* ptCounters );
ETHERNET_RESULT EthMac_GetIrqMask( unsigned int uiPort, uint32_t *pulIrqMask );
ETHERNET_RESULT EthMac_GetIrq( unsigned int uiPort, uint32_t *pulIrq );

/* Transmission Service Class */
ETHERNET_RESULT EthMac_GetFrame( unsigned int uiPort, ETHERNET_FRAME_T** pptFrame, void** phFrame );
ETHERNET_RESULT EthMac_Send( unsigned int uiPort, void* hFrame, uint32_t ulLength, unsigned int uHighPriority );
ETHERNET_RESULT EthMac_SendWithoutCnf( unsigned int uiPort, void* hFrame, uint32_t ulLength, unsigned int uHighPriority );
ETHERNET_RESULT EthMac_GetSendCnfFillLevel( unsigned int uiPort, unsigned int uHighPriority, uint32_t *pulCnfFillLevel );
ETHERNET_RESULT EthMac_GetSendCnf( unsigned int uiPort, ETHERNET_FRAME_T** pptFrame, void** phFrame, uint32_t* pulLength, unsigned int uHighPriority, ETHERNET_RESULT* peResult );

/* Reception Service Class */
ETHERNET_RESULT EthMac_GetRecvFillLevel( unsigned int uiPort, unsigned int uHighPriority, uint32_t *pulFillLevel );
ETHERNET_RESULT EthMac_Recv( unsigned int uiPort, ETHERNET_FRAME_T** pptFrame, void** phFrame, uint32_t* pulLength, unsigned int uHighPriority );
ETHERNET_RESULT EthMac_ReleaseFrame( unsigned int uiPort, void* hFrame );

#endif /* _HAL_ETH_MAC_H */
