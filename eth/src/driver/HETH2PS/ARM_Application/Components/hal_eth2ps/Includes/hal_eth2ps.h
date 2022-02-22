#ifndef __HAL_ETH2PS_H
#define __HAL_ETH2PS_H

#include <stdint.h>
#include <stdbool.h>


/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/* Number of supported switch ports */
#define ETH2PS_PORTS                2

/* COM IRQs */
#ifndef MSK_ETH2PS_IRQ_IND_CNF_HI                     /* If MSYC IRQ is required compile with -DMSK_ETH2PS_IRQ_IND_CNF_HI=0x2000 */
#define MSK_ETH2PS_IRQ_IND_CNF_HI           0x0001    /* High priority indication/confirmation interrupt */
#endif
#define MSK_ETH2PS_IRQ_IND_CNF_LO           0x0002    /* Low priority indication/confirmation interrupt */
#define MSK_ETH2PS_IRQ_LINK_CHANGED         0x0008    /* Link status changed */
#define MSK_ETH2PS_IRQ_CIRCULATING_FRM      0x0010    /* Received and blocked a frame from self */

/* MSYNC IRQs */
#define MSK_ETH2PS_IRQ_BCN_TIMEOUT          0x1000    /* Beacon timeout expired */

/* Synchronization unit IRQs */
#define MSK_ETH2PS_SYNC_IRQ_TRG0            0x0001    /* Trigger 0 interrupt */
#define MSK_ETH2PS_SYNC_IRQ_TRG1            0x0002    /* Trigger 1 interrupt */

/* Frame buffer settings */
#define ETH2PS_FRAME_BUF_SIZE               1560      /* size of a frame buffer                      */
#define ETH2PS_FRAME_BUF_DATA_LEN_PRIMARY   1518      /* number of payload bytes in first buffer     */
#define ETH2PS_FRAME_BUF_DATA_LEN_SECONDARY 0         /* number of payload bytes in follow-up buffer */
#define ETH2PS_FRAME_BUF_PER_FRAME          1         /* maximum number of used frame buffers per frame */

/*****************************************************************************/
/*! Functions Result Codes
* \description
*    All functions return one of the following values after returning from
*    the function call. Function return values shall always be evaluated
*    by the calling function.                                                */
/*****************************************************************************/
typedef enum
{
  ETH2PS_OKAY,               /**< Successful                           */
  ETH2PS_ERR_INVAL_PORT,     /**< Invalid switch port number           */
  ETH2PS_ERR_INVAL_PARAM,    /**< Invalid parameter in function call   */
  ETH2PS_ERR_FIFO_EMPTY,     /**< Indication FIFO is empty             */
  ETH2PS_ERR_INIT_FAILED,    /**< Error occurred during initialization */
  ETH2PS_ERR_INVAL_STATE,    /**< Invalid port state                   */
  ETH2PS_ERR_OUT_OF_MEMORY   /**< Not enough resources                 */
} ETH2PS_RESULT_E;

/*****************************************************************************/
/*! Transmit Confirmation Error Codes
 *\description
 *   The function Eth2PS_GetIndCnf() provides one of the following error codes
 *   for each transmit confirmation.                                         */
/*****************************************************************************/
typedef enum
{
  ETH2PS_CNF_ERR_CODE_SUCCESSFUL_WITHOUT_RETRIES = 0,  /**< Success on first try         */
  ETH2PS_CNF_ERR_CODE_SUCCESSFUL_WITH_RETRIES    = 1,  /**< Success after retries        */
  ETH2PS_CNF_ERR_CODE_FAILED_LATE_COLLISION      = 2,  /**< Error (late collision)       */
  ETH2PS_CNF_ERR_CODE_FAILED_EXCESSIVE_COLLISION = 4,  /**< Error (collision excess)     */
  ETH2PS_CNF_ERR_CODE_FAILED_UTX_UNDERRUN        = 5,  /**< Error (FIFO under-run error) */
  ETH2PS_CNF_ERR_CODE_TX_FAILED_ABORTED          = 6   /**< Error (Frame aborted)        */
} ETH2PS_CNF_ERROR_CODE_E;

/*****************************************************************************/
/*! Port Parameters
* \description
*    These parameters can be set for each port individually.
*    Use the function Eth2PS_SetParameter() to set one of the parameters.    */
/*****************************************************************************/
typedef enum
{
  ETH2PS_PARAM_IRQ_EN_MSK,                 /**< Interrupt enable mask, the value shall be a sum of MSK_ETH2PS_IRQ_* bits */
  ETH2PS_PARAM_AGING_TIME,                 /**< Aging time [milliseconds] */
  ETH2PS_PARAM_PORT_ENABLE,                /**< 1/0: enable/disable port, a disabled port does not transmit, receive and forward any frames */
  ETH2PS_PARAM_LINK_INPUT_ENABLE,          /**< 1/0: enable/disable usage of switch port's link input
                                                Note that if disabled user has to use PORT_ENABLE to discard transmission of frames in case of link down
                                                Not supported by netX100/500, netX50, netX5, netX51/52 and netX6.
                                                */
  ETH2PS_PARAM_BPDU_SUPPORT_ENABLE,        /**< 1/0: enable/disable special handling of BPDU (Bridge Protocol Data Unit) frames  (DA == 01:80:C2:00:xx:xx)\n
                                                if enabled, BPDU frames are always blocked and indicated regardless of other parameters */
  ETH2PS_PARAM_BPDU_ONLY,                  /**< 1: only BPDUs are received and transmitted at this port, all other frames are dropped */
  ETH2PS_PARAM_DISABLE_LEARNING,           /**< 0/1: learning of non-BPDUs enabled/disabled\n
                                                To model state STP.Disabled: PORT_ENABLE = 0, BPDU_ONLY = don't care, DISABLE_LEARNING = don't care\n
                                                To model state STP.Blocking / RSTP.Discarding: PORT_ENABLE = 1, BPDU_ONLY = 1, DISABLE_LEARNING = 1\n
                                                To model state STP.Listening / RSTP.Discarding: PORT_ENABLE = 1, BPDU_ONLY = 1, DISABLE_LEARNING = 1\n
                                                To model state STP.Learning / RSTP.Learning: PORT_ENABLE = 1, BPDU_ONLY = 1, DISABLE_LEARNING = 0\n
                                                To model state STP.Forwarding / RSTP.Forwarding: PORT_ENABLE = 1, BPDU_ONLY = 0, DISABLE_LEARNING = don't care (always 0)\n
                                                */
  ETH2PS_PARAM_DSCP_PRIORIZATION,          /**< IP based DSCP priority, disabled by default\n
                                                if enabled recommendation is 43; that means frames with DSCP < 43 / >=43 treated as low/high prior frames\n
                                                parameter must adapted to EthernetIP specific values,\n
                                                DSCP priorization overrules VLAN-Tag priority */
  /* EthernetIP/DLR (Device Level Ring) specific parameters: */
  ETH2PS_PARAM_DLR_SUPPORT_ENABLE,         /**< 1/0: enable/disable special handling of DLR frames\n
                                                if enabled, Beacon frames are parsed (DA == 01:21:6c:00:00:01),
                                                NEIGHBOR_REQ/RSP and SIGN_ON frames are blocked (DA == 01:21:6c:00:00:02) */
  ETH2PS_PARAM_DLR_BCN_IND_ENABLE,         /**< 1/0: enable/disable indication of Beacon frames,
                                                only used if ETH2PS_PARAM_DLR_SUPPORT_ENABLE is set */
  ETH2PS_PARAM_DLR_BCN_PORT_MATCH_ENABLE,  /**< 1/0: enable/disable Beacon port match, if enabled match is always inclusive,\n
                                                This parameter has no effect unless ETH2PS_PARAM_DLR_SUPPORT_ENABLE is set */
  ETH2PS_PARAM_DLR_BCN_RCV_PORT,           /**< last Beacon receive port for comparison,\n
                                                This parameter has no effect unless ETH2PS_PARAM_DLR_SUPPORT_ENABLE is set */
  ETH2PS_PARAM_DLR_BCN_PRECEDENCE,         /**< last Beacon precedence for comparison,\n
                                                This parameter has no effect unless ETH2PS_PARAM_DLR_SUPPORT_ENABLE is set */
  ETH2PS_PARAM_DLR_BCN_TIMEOUT,             /**< 0: Disable and stop Beacon timeout timer\n
                                                else: Start Beacon timeout timer with given value [10 ns]\n
                                                This parameter has no effect unless ETH2PS_PARAM_DLR_SUPPORT_ENABLE is set */
  /* IEEE1588 specific parameters: */
  ETH2PS_PARAM_INGRESS_LATENCY,             /**< All PTP event messages are time stamped on ingress.\n
                                                 The timestamp shall be the time at which the event message timestamp point passes the reference plane (boundary between PTP node and network).\n
                                                 This implementation generates event message timestamps at detection of SFD at MII. Use this parameter to correct appropriately. \n
                                                 ingressTimestamp = ingressMeasuredTimestamp - ingressLatency\n
                                                 Normally ingressLatency contains MAC sample delays + PHY receive delay + eventually PHY phase offsets */
  ETH2PS_PARAM_EGRESS_LATENCY               /**< All PTP event messages are time stamped on egress.\n
                                                 The timestamp shall be the time at which the event message timestamp point passes the reference plane (boundary between PTP node and network).\n
                                                 This implementation generates event message timestamps at detection of SFD at MII. Use this parameter to correct appropriately. \n
                                                 egressTimestamp = egressMeasuredTimestamp + egressLatency\n
                                                 Normally engressLatency contains MAC sample delays + PHY transmit delay */
} ETH2PS_PARAM_E;

/*****************************************************************************/
/*! Beacon Status
* \description
*    These values are used to determine why a received Beacon frame was
*    indicated.                                                              */
/*****************************************************************************/
typedef enum
{
  ETH2PS_BCNSTATE_NO_BEACON,           /**< This is no Beacon frame                               */
  ETH2PS_BCNSTATE_PRECEDENCE_HIGHER,   /**< Precedence of this Beacon is higher than active one   */
  ETH2PS_BCNSTATE_RX_PORT_NEQ_LAST,    /**< Beacon receive port is not equal to LastBcnRcvPort    */
  ETH2PS_BCNSTATE_RING_STATE_CHANGE    /**< Beacon with other ring state than last Beacon was received  */
} ETH2PS_BCNSTATE_E;

/*****************************************************************************/
/*! PHY LED configuration                                                    */
/*****************************************************************************/
typedef enum
{
  ETH2PS_PHYLED_STATIC = 0, /**< separate link and activity LEDs, activity statically on */
  ETH2PS_PHYLED_BLINK  = 1, /**< separate link and activity LEDs, activity blinking when active */
  ETH2PS_PHYLED_SINGLE = 2, /**< single LED, combined link and blink on activity */
  ETH2PS_PHYLED_OFF    = 3  /**< PHY LEDs are disabled */
} ETH2PS_PHYLED_CFG_E;

/*****************************************************************************/
/*! MAC addresses
* \description
*    Describes the different types of MAC addresses.                         */
/*****************************************************************************/
typedef enum
{
  ETH2PS_MAC_ADDRESS_CHASSIS,         /**< Primary Chassis MAC address valid all both ports */
  ETH2PS_MAC_ADDRESS_2ND_CHASSIS,     /**< Secondary Chassis MAC address valid all both ports */
  ETH2PS_MAC_ADDRESS_DLR_SUPERVISOR   /**< MAC address of ring supervisor */
} ETH2PS_MAC_ADDRESS_TYPE_E;


/* MAC address structure */
typedef uint8_t ETH2PS_MAC_ADDR_T[6];

/*****************************************************************************/
/*! Status Counters
* \description
*   The following structure contains the status counters of one port.        */
/*****************************************************************************/
typedef struct ETH2PS_COUNTERS_Ttag
{  
  uint32_t ulTxOutOctets;            /**< The total number of octets transmitted out of the
                                          interface, including framing characters */
  uint32_t ulTxSingleCollisions;     /**< The number of collisions during the first try of
                                          a frame transmission */
  uint32_t ulTxMultipleCollisions;   /**< The number of collisions during a retry of
                                          a frame transmission */
  uint32_t ulTxLateCollisions;       /**< The number of collisions after the first 64 octets
                                          of the packet have been transmitted */
  uint32_t ulTxUnderrun;             /**< The number of TX MAC FIFO underruns that occur when the
                                          xPEC is too slow */
  uint32_t ulTxAborted;              /**< The number of outbound packets which were aborted during
                                          a cut-through transmission because the incoming packed
                                          was erroneous */
  uint32_t ulTxDiscarded;            /**< The number of outbound packets which were chosen to be discarded
                                          because the port is disabled */
  uint32_t ulRxInOctets;             /**< The total number of octets received on the
                                          interface, including framing characters */
  uint32_t ulRxFcsErrors;            /**< Count of received frames that are an integral number of octets in length
                                          and do not pass the FCS check */
  uint32_t ulRxAlignmentErrors;      /**< Count of received frames that are not an integral number of octets in length
                                          and do not pass the FCS check */
  uint32_t ulRxFrameLengthErrors;    /**< Count of received frames that exceed the maximum permitted frame size */
  uint32_t ulRxRuntFrames;           /**< Count of received frames that have a length between 42 and 63 Bytes
                                          and pass the FCS check */
  uint32_t ulRxCollisionFragments;   /**< Count of received frames that have a length of 63 Bytes or less
                                          and do not pass the FCS check */
  uint32_t ulRxOverflow;             /**< The number of RX MAC FIFO overflows that occur when the
                                          xPEC is too slow */
  uint32_t ulRxDiscarded;            /**< Count of inbound packets which were chosen
                                          to be discarded even though no error was detected.
                                          Possible reasons are low resources or the port is disabled. */
  uint32_t ulRxCirculatingFrmBlocked;/**< Count of inbound packets which were chosen
                                          to be discarded because the source MAC address matched one of our
                                          Chassis MAC addresses */
  uint32_t ulRxUnknownErrors;        /**< Count of illegal error states reported from the RPU */
} ETH2PS_COUNTERS_T;

/*****************************************************************************/
/*! Link Status Structure
* \description
*    Describes the current state of the Ethernet PHY.                        */
/*****************************************************************************/
typedef struct ETH2PS_CONNECTION_STATE_Ttag
{
  unsigned int uSpeed;        /**< SPEED (100/10)                */
  unsigned int uIsLinkUp;     /**< LINK state   (!=0 -> Link UP) */
  unsigned int uIsFullDuplex; /**< DUPLEX state (!=0 -> FDX)     */
} ETH2PS_CONNECTION_STATE_T;

/*****************************************************************************/
/*! Ethernet Frame Buffer Header Structure
 *\description
 *   This structure defines the content at the front of each frame buffer.   */
/*****************************************************************************/
typedef struct ETH2PS_FRAME_BUF_HDR_Ttag
{
  uint32_t  ulUserData0;  /**< Not used by HAL, for application usage */
  uint32_t  ulUserData1;  /**< Not used by HAL, for application usage */
} ETH2PS_FRAME_BUF_HDR_T;

/*****************************************************************************/
/*! Frame Handle
* \description
*    This structure is used to handle a single Ethernet frame.               */
/*****************************************************************************/
typedef struct ETH2PS_FRAME_HANDLE_Ttag
{
  uint32_t                ulFifoEntry;  /**< Frame handle from Pointer FIFO            */
  uint16_t                usLength;     /**< Total size of frame data in bytes         */
  ETH2PS_FRAME_BUF_HDR_T* ptHdr;        /**< Header of frame buffer                    */
  uint8_t*                pbData;       /**< Frame data pointer, pointing to first byte of destination MAC address */
} ETH2PS_FRAME_HANDLE_T;

/*****************************************************************************/
/*! Frame Information
* \description
*    This structure holds additional information after a frame was transmitted
*    or received.
*    The information is provided by the function Eth2PS_GetIndCnf().         */
/*****************************************************************************/
typedef struct ETH2PS_FRAME_INFO_Ttag
{
  unsigned int            uPortNo;    /**< Switch port number (frame origin) */
  uint32_t                ulTimeNs;   /**< Value of systime_ns register, latched at transmission/reception of SFD (Start of Frame Delimiter) */
  uint32_t                ulTimeS;    /**< Value of systime_s register, latched at transmission/reception of SFD (Start of Frame Delimiter) */
  bool                    fCnf;       /**< Flag indicating whether this frame was a confirmation (true) or indication (false) */
  ETH2PS_CNF_ERROR_CODE_E eCnfResult; /**< Result of the transmission process,
                                           only valid if fCnf==true */
} ETH2PS_FRAME_INFO_T;


/*****************************************************************************/
/*! HAL Configuration Structure
* \description
*    Configuration for HAL initialization.                                   */
/*****************************************************************************/
typedef struct ETH2PS_CFG_Ttag
{
  ETH2PS_PHYLED_CFG_E ePhyLedCfg;          /**< PHY LED behavior */
  uint32_t            ulActLedFlashPeriod; /**< Flash frequency of activity LED [milliseconds],
                                                The blink frequency shall not be larger than 100, larger values may lead to malfunction of the LED */
} ETH2PS_CFG_T;

/*****************************************************************************/
/*! PI Controller Output
* \description
*    This structure holds information about the current PI controller output.*/
/*****************************************************************************/
typedef struct ETH2PS_PI_CONTROLLER_OUTPUT_Ttag
{
  int32_t  lPTerm;    /**< Proportional term */
  uint32_t ulITerm;   /**< Integral term     */
  uint32_t ulOutput;  /**< Output value      */
} ETH2PS_PI_CONTROLLER_OUTPUT_T;

/*****************************************************************************/
/*! Cyclic Event
* \description
*    This structure defines an event to be generated cyclically.
*    One event can raise an IRQ and/or drive a pulse signal at the sync pin. */
/*****************************************************************************/
typedef struct ETH2PS_CYCLIC_EVENT_Ttag
{
  uint32_t ulStartOffset;           /**< Time offset to ulStartTimeCyclicOp of first trigger event */
  uint32_t ulPeriod;                /**< Cycle time of trigger event                               */
  bool     fIrqEn;                  /**< true/false: IRQ generation enabled/disabled               */
  bool     fTrgEn;                  /**< true/false: trigger signal generation enabled/disabled    */
  bool     fTrgPolarity;            /**< true/false: trigger signal is High/Low active             */
  bool     fTrgOe;                  /**< true/false: trigger signal output enable enabled/disabled */
} ETH2PS_CYCLIC_EVENT_T;

/*****************************************************************************/
/*! Cyclic Configuration
* \description
*    This structure hold all parameters for the cyclic event generation.     */
/*****************************************************************************/
typedef struct ETH2PS_CYCLIC_CFG_Ttag
{
  bool     fTrgPinsControlledByHost;     /**< true: trigger 0 and 1 pins unchanged but interrupts are generated during cyclic operation */
  uint32_t ulTrgPulseLen;                /**< Pulse length of trigger signals in 10ns units */
  ETH2PS_CYCLIC_EVENT_T atTrgCfg[2];     /**< Event configuration */
} ETH2PS_CYCLIC_CFG_T;


/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/

/* Control/Status */
ETH2PS_RESULT_E Eth2PS_Initialize(ETH2PS_CFG_T* ptCfg, void* pvUser );
ETH2PS_RESULT_E Eth2PS_Start(void* pvUser);
ETH2PS_RESULT_E Eth2PS_SetMacAddr(ETH2PS_MAC_ADDRESS_TYPE_E eType, const ETH2PS_MAC_ADDR_T tMacAddr);
ETH2PS_RESULT_E Eth2PS_SetParameter(unsigned int uPortNo, ETH2PS_PARAM_E eParam, uint32_t ulValue);
uint32_t        Eth2PS_GetCnfIrq(unsigned int uPortNo, bool fHiPriority);
void            Eth2PS_FlushLearningTable(void);
ETH2PS_RESULT_E Eth2PS_FlushLearningTablePort(unsigned int uPortNo);
uint32_t        Eth2PS_GetIndCnfFillLevel(unsigned int uPriority);
uint32_t        Eth2PS_GetReqFillLevel(unsigned int uPortNo, unsigned int uPriority);
uint32_t        Eth2PS_GetEmptyFillLevel(void);
ETH2PS_RESULT_E Eth2PS_GetCounters(unsigned int uPortNo, ETH2PS_COUNTERS_T* ptCounters);
ETH2PS_RESULT_E Eth2PS_GetPhyPhaseOffset(unsigned int uPortNo, uint8_t* pbPhyPhaseOffsetNs, void* pvUser);
ETH2PS_RESULT_E Eth2PS_SetLinkMode( unsigned int uPortNo, bool fValid, unsigned int uiSpeed, bool fFdx );
ETH2PS_RESULT_E Eth2PS_CfgMii( unsigned int uPortNo, unsigned int uiCfg );
#ifndef ETH2PS_RCV_ALL_MULTICASTS
ETH2PS_RESULT_E Eth2PS_AddGroupAddr(const ETH2PS_MAC_ADDR_T tMacAddr);
ETH2PS_RESULT_E Eth2PS_DeleteGroupAddr(const ETH2PS_MAC_ADDR_T tMacAddr);
#endif

/* Communication */
ETH2PS_RESULT_E Eth2PS_GetFrame(ETH2PS_FRAME_HANDLE_T* ptFrame);
ETH2PS_RESULT_E Eth2PS_Send(unsigned int uPortInfo, ETH2PS_FRAME_HANDLE_T* ptFrame, unsigned int uPriority, bool fConfirmationEn, unsigned int* puCnfCnt);
ETH2PS_RESULT_E Eth2PS_GetIndCnf(unsigned int uPriority, ETH2PS_FRAME_HANDLE_T* ptFrame, ETH2PS_FRAME_INFO_T* ptFrameInfo);
ETH2PS_RESULT_E Eth2PS_ReleaseFrame(ETH2PS_FRAME_HANDLE_T* ptFrame);
ETH2PS_RESULT_E Eth2PS_InitFrameHandleFromFifoEntry(uint32_t ulFifoEntry, ETH2PS_FRAME_HANDLE_T* ptFrame);
ETH2PS_RESULT_E Eth2PS_SetFrameLengthFromFifoEntry(uint32_t ulFifoEntry, uint16_t usLength);

/* PTP */
void            Eth2PS_PtpResetPll(void* pvUser);
void            Eth2PS_PtpConfigPll(unsigned int ulIAmp2Pow, unsigned int ulPAmp2Pow, uint32_t ulClockSpeedVariancePpm);
void            Eth2PS_PtpControlPll(int32_t lDiff, ETH2PS_PI_CONTROLLER_OUTPUT_T* ptOutput, void* pvUser);

/* Cyclic Events */
ETH2PS_RESULT_E Eth2PS_CyclicInitialize(void* pvUser);
ETH2PS_RESULT_E Eth2PS_CyclicConfig(ETH2PS_CYCLIC_CFG_T* ptCfg);
void            Eth2PS_CyclicStart(uint32_t ulStartTimeNs);
void            Eth2PS_CyclicStop(void);
uint32_t        Eth2PS_CyclicGetCnfIrq(void);

/* EthernetIP/DLR */
ETH2PS_BCNSTATE_E Eth2PS_GetBeaconState(uint32_t ulFifoEntry);

#endif /* __HAL_ETH2PS_H */

