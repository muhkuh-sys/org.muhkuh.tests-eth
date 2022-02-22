#ifndef eth2ps_xpec_regdef_h__
#define eth2ps_xpec_regdef_h__

#include <stdint.h>

/* ===================================================================== */
/* Area ETH2PS_XPEC_PROGRAM */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_XPEC_PROGRAM  0x00000000U

/* ===================================================================== */
/* Area ETH2PS_STATUS_AREA_BASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_STATUS_AREA_BASE  0x00000004U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_OUT_OCTETS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_OUT_OCTETS  0x00000004U

/* count of bytes transmitted */
#define MSK_ETH2PS_OUT_OCTETS_CNT  0xffffffffU
#define SRT_ETH2PS_OUT_OCTETS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SINGLE_COLLISION_FRAMES */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SINGLE_COLLISION_FRAMES  0x00000008U

/* count of frames that are involved into a single collision */
#define MSK_ETH2PS_SINGLE_COLLISION_FRAMES_CNT  0xffffffffU
#define SRT_ETH2PS_SINGLE_COLLISION_FRAMES_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_MULTIPLE_COLLISION_FRAMES */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_MULTIPLE_COLLISION_FRAMES  0x0000000cU

/* count of frames that are involved into more that one collisions */
#define MSK_ETH2PS_MULTIPLE_COLLISION_FRAMES_CNT  0xffffffffU
#define SRT_ETH2PS_MULTIPLE_COLLISION_FRAMES_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LATE_COLLISIONS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LATE_COLLISIONS  0x00000010U

/* count of the times that a collision has been detected later than 512 bit times into the transmitted packet */
#define MSK_ETH2PS_LATE_COLLISIONS_CNT  0xffffffffU
#define SRT_ETH2PS_LATE_COLLISIONS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_UTX_UNDERFLOW_DURING_TRANSMISSION */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_UTX_UNDERFLOW_DURING_TRANSMISSION  0x00000014U

/* UTX fifo underflow at transmission time */
#define MSK_ETH2PS_UTX_UNDERFLOW_DURING_TRANSMISSION_CNT  0xffffffffU
#define SRT_ETH2PS_UTX_UNDERFLOW_DURING_TRANSMISSION_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_FRAMES_ABORTED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_FRAMES_ABORTED  0x00000018U

/* Cut-through frames that have been aborted due to rx FCS error */
#define MSK_ETH2PS_FRAMES_ABORTED_CNT  0xffffffffU
#define SRT_ETH2PS_FRAMES_ABORTED_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_OUT_FRAMES_DISCARDED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_OUT_FRAMES_DISCARDED  0x0000001cU

/* frame discarded due port disabled */
#define MSK_ETH2PS_OUT_FRAMES_DISCARDED_CNT  0xffffffffU
#define SRT_ETH2PS_OUT_FRAMES_DISCARDED_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_IN_OCTETS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_IN_OCTETS  0x00000020U

/* count of bytes received */
#define MSK_ETH2PS_IN_OCTETS_CNT  0xffffffffU
#define SRT_ETH2PS_IN_OCTETS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_FRAME_CHECK_SEQUENCE_ERRORS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_FRAME_CHECK_SEQUENCE_ERRORS  0x00000024U

/* count of frames that are an integral number of octets in length and do not pass the FCS check */
#define MSK_ETH2PS_FRAME_CHECK_SEQUENCE_ERRORS_CNT  0xffffffffU
#define SRT_ETH2PS_FRAME_CHECK_SEQUENCE_ERRORS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_ALIGNMENT_ERRORS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ALIGNMENT_ERRORS  0x00000028U

/* count of frames that are not an integral number of octets in length and do not pass the FCS check */
#define MSK_ETH2PS_ALIGNMENT_ERRORS_CNT  0xffffffffU
#define SRT_ETH2PS_ALIGNMENT_ERRORS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_FRAME_TOO_LONG_ERRORS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_FRAME_TOO_LONG_ERRORS  0x0000002cU

/* count of frames that are received and exceed the maximum permitted frame size */
#define MSK_ETH2PS_FRAME_TOO_LONG_ERRORS_CNT  0xffffffffU
#define SRT_ETH2PS_FRAME_TOO_LONG_ERRORS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RUNT_FRAMES_RECEIVED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RUNT_FRAMES_RECEIVED  0x00000030U

/* count of frames that have a length between 42..63 bytes and a valid CRC */
#define MSK_ETH2PS_RUNT_FRAMES_RECEIVED_CNT  0xffffffffU
#define SRT_ETH2PS_RUNT_FRAMES_RECEIVED_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_COLLISION_FRAGMENTS_RECEIVED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_COLLISION_FRAGMENTS_RECEIVED  0x00000034U

/* count of frames that are smaller that 64 bytes and have a invalid CRC */
#define MSK_ETH2PS_COLLISION_FRAGMENTS_RECEIVED_CNT  0xffffffffU
#define SRT_ETH2PS_COLLISION_FRAGMENTS_RECEIVED_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_FRAMES_DROPPED_DUE_URX_OVERFLOW */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_FRAMES_DROPPED_DUE_URX_OVERFLOW  0x00000038U

/* urx fifo overflow at indication time */
#define MSK_ETH2PS_FRAMES_DROPPED_DUE_URX_OVERFLOW_CNT  0xffffffffU
#define SRT_ETH2PS_FRAMES_DROPPED_DUE_URX_OVERFLOW_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_IN_FRAMES_DISCARDED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_IN_FRAMES_DISCARDED  0x0000003cU

/* frame discarded due no empty pointer available at indication time or port disabled */
#define MSK_ETH2PS_IN_FRAMES_DISCARDED_CNT  0xffffffffU
#define SRT_ETH2PS_IN_FRAMES_DISCARDED_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_CIRCULATING_FRAMES_BLOCKED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_CIRCULATING_FRAMES_BLOCKED  0x00000040U

/* number of circulating frames blocked from forwarding */
#define MSK_ETH2PS_CIRCULATING_FRAMES_BLOCKED_CNT  0xffffffffU
#define SRT_ETH2PS_CIRCULATING_FRAMES_BLOCKED_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_UNKNOWN_ERRORS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_UNKNOWN_ERRORS  0x00000044U

/* illegal error state was reported from rpu */
#define MSK_ETH2PS_UNKNOWN_ERRORS_CNT  0xffffffffU
#define SRT_ETH2PS_UNKNOWN_ERRORS_CNT            0


/* ===================================================================== */
/* Area ETH2PS_TX_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_TX_AREA  0x00000048U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_TX_RETRY */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_TX_RETRY  0x00000048U

/* retry counter for transmission attempts */
#define MSK_ETH2PS_TX_RETRY_CNT  0xffffffffU
#define SRT_ETH2PS_TX_RETRY_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_TX_TX_TIMESTAMP */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_TX_TX_TIMESTAMP  0x0000004cU

/* transmit timestamp ns of current transmitted frame */
#define MSK_ETH2PS_TX_TX_TIMESTAMP_VAL  0xffffffffU
#define SRT_ETH2PS_TX_TX_TIMESTAMP_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_CORR_FIELD_UPPER */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_CORR_FIELD_UPPER  0x00000050U

/* upper part of correction field ns */
#define MSK_ETH2PS_CORR_FIELD_UPPER_VAL  0xffffffffU
#define SRT_ETH2PS_CORR_FIELD_UPPER_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_CORR_FIELD_LOWER */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_CORR_FIELD_LOWER  0x00000054U

/* lower part of correction field ns */
#define MSK_ETH2PS_CORR_FIELD_LOWER_CNT  0xffffffffU
#define SRT_ETH2PS_CORR_FIELD_LOWER_CNT            0


/* ===================================================================== */
/* Area ETH2PS_RX_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_RX_AREA  0x00000058U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK  0x00000058U

/* Mask for byte within hash table */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET  0x0000005cU

/* Offset address within hash table */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET_VAL  0xffffffffU
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_SRC_MAC_ADDRESS_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_SRC_MAC_ADDRESS_LO  0x00000060U

/* MAC address bits 31..0 */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_LO_MAC_ADDR_LO  0xffffffffU
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_LO_MAC_ADDR_LO            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_SRC_MAC_ADDRESS_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_SRC_MAC_ADDRESS_HI  0x00000064U

/* MAC address bits 47..32 */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_HI_MAC_ADDR_HI  0x0000ffffU
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_HI_MAC_ADDR_HI            0

/* reserved */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_HI_RES1  0xffff0000U
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_HI_RES1           16


/* ===================================================================== */
/* Area ETH2PS_BEACON_CTRL_AREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_BEACON_CTRL_AREABASE  0x00000068U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT  0x00000068U

/* 1 */
#define MSK_ETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT_VAL  0xffffffffU
#define SRT_ETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_ACTIVE_SVR_MAC_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ACTIVE_SVR_MAC_LO  0x0000006cU

/* MAC address bits 31..0 */
#define MSK_ETH2PS_ACTIVE_SVR_MAC_LO_MAC_ADDR_LO  0xffffffffU
#define SRT_ETH2PS_ACTIVE_SVR_MAC_LO_MAC_ADDR_LO            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_ACTIVE_SVR_MAC_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ACTIVE_SVR_MAC_HI  0x00000070U

/* MAC address bits 47..32 */
#define MSK_ETH2PS_ACTIVE_SVR_MAC_HI_MAC_ADDR_HI  0x0000ffffU
#define SRT_ETH2PS_ACTIVE_SVR_MAC_HI_MAC_ADDR_HI            0

/* reserved */
#define MSK_ETH2PS_ACTIVE_SVR_MAC_HI_RES1  0xffff0000U
#define SRT_ETH2PS_ACTIVE_SVR_MAC_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_ACTIVE_SVR_PRECEDENCE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ACTIVE_SVR_PRECEDENCE  0x00000074U

/* current precedence */
#define MSK_ETH2PS_ACTIVE_SVR_PRECEDENCE_VAL  0x000000ffU
#define SRT_ETH2PS_ACTIVE_SVR_PRECEDENCE_VAL            0

/* reserved */
#define MSK_ETH2PS_ACTIVE_SVR_PRECEDENCE_RES1  0xffffff00U
#define SRT_ETH2PS_ACTIVE_SVR_PRECEDENCE_RES1            8


/* --------------------------------------------------------------------- */
/* Register ETH2PS_BEACON_TIMEOUT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BEACON_TIMEOUT  0x00000078U

/* beacon timeout in 10 ns */
#define MSK_ETH2PS_BEACON_TIMEOUT_VAL  0xffffffffU
#define SRT_ETH2PS_BEACON_TIMEOUT_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LAST_RING_STATE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LAST_RING_STATE  0x0000007cU

/* last ring state */
#define MSK_ETH2PS_LAST_RING_STATE_VAL  0x000000ffU
#define SRT_ETH2PS_LAST_RING_STATE_VAL            0

/* reserved */
#define MSK_ETH2PS_LAST_RING_STATE_RES1  0xffffff00U
#define SRT_ETH2PS_LAST_RING_STATE_RES1            8


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LAST_BEACON_RX_PORT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LAST_BEACON_RX_PORT  0x00000080U

/* LastBcnRvcPort */
#define MSK_ETH2PS_LAST_BEACON_RX_PORT_VAL  0xffffffffU
#define SRT_ETH2PS_LAST_BEACON_RX_PORT_VAL            0


/* ===================================================================== */
/* Area ETH2PS_IRQMASK_AREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_IRQMASK_AREABASE  0x00000084U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_IND_CNF_HI_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_IND_CNF_HI_IRQ_MASK  0x00000084U

/* interrupt mask */
#define MSK_ETH2PS_IND_CNF_HI_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_IND_CNF_HI_IRQ_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_IND_CNF_LO_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_IND_CNF_LO_IRQ_MASK  0x00000088U

/* interrupt mask */
#define MSK_ETH2PS_IND_CNF_LO_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_IND_CNF_LO_IRQ_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LINK_STA_CHANGED_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LINK_STA_CHANGED_IRQ_MASK  0x0000008cU

/* interrupt mask */
#define MSK_ETH2PS_LINK_STA_CHANGED_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_LINK_STA_CHANGED_IRQ_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK  0x00000090U

/* interrupt mask */
#define MSK_ETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_BCN_TIMEOUT_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BCN_TIMEOUT_IRQ_MASK  0x00000094U

/* interrupt mask */
#define MSK_ETH2PS_BCN_TIMEOUT_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_BCN_TIMEOUT_IRQ_MASK_VAL            0


/* ===================================================================== */
/* Area ETH2PS_CONFIGAREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_CONFIGAREABASE  0x00000098U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_DSCP_PRIORIZATION */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_DSCP_PRIORIZATION  0x00000098U

/* Traffic Class arrangement for DSCP frames */
#define MSK_ETH2PS_DSCP_PRIORIZATION_VAL  0xffffffffU
#define SRT_ETH2PS_DSCP_PRIORIZATION_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_INTERFACE_MAC_ADDRESS_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_INTERFACE_MAC_ADDRESS_LO  0x0000009cU

/* Interface MAC address bytes 4,3,2 and 1 */
#define MSK_ETH2PS_INTERFACE_MAC_ADDRESS_LO_BYTES_4321  0xffffffffU
#define SRT_ETH2PS_INTERFACE_MAC_ADDRESS_LO_BYTES_4321            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_INTERFACE_MAC_ADDRESS_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_INTERFACE_MAC_ADDRESS_HI  0x000000a0U

/* Interface MAC address bytes 6 and 5 */
#define MSK_ETH2PS_INTERFACE_MAC_ADDRESS_HI_BYTES_65  0x0000ffffU
#define SRT_ETH2PS_INTERFACE_MAC_ADDRESS_HI_BYTES_65            0

/* 0x0000 */
#define MSK_ETH2PS_INTERFACE_MAC_ADDRESS_HI_RES1  0xffff0000U
#define SRT_ETH2PS_INTERFACE_MAC_ADDRESS_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO  0x000000a4U

/* Interface MAC address bytes 4,3,2 and 1 */
#define MSK_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO_BYTES_4321  0xffffffffU
#define SRT_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO_BYTES_4321            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI  0x000000a8U

/* Interface MAC address bytes 6 and 5 */
#define MSK_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI_BYTES_65  0x0000ffffU
#define SRT_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI_BYTES_65            0

/* 0x0000 */
#define MSK_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI_RES1  0xffff0000U
#define SRT_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYSTIME_BORDER_COPY_PLUS1 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYSTIME_BORDER_COPY_PLUS1  0x000000acU

/* wrap-around value of systime_ns, systime_s */
#define MSK_ETH2PS_SYSTIME_BORDER_COPY_PLUS1_VAL  0xffffffffU
#define SRT_ETH2PS_SYSTIME_BORDER_COPY_PLUS1_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_TS_COR_BASE_TX */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_TS_COR_BASE_TX  0x000000b0U

/* timestamp correction value base for reception in ns */
#define MSK_ETH2PS_TS_COR_BASE_TX_VAL  0xffffffffU
#define SRT_ETH2PS_TS_COR_BASE_TX_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_TS_COR_BASE_RX */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_TS_COR_BASE_RX  0x000000b4U

/* timestamp correction value base for transmission in ns */
#define MSK_ETH2PS_TS_COR_BASE_RX_VAL  0xffffffffU
#define SRT_ETH2PS_TS_COR_BASE_RX_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_AGING_TIME_IN_MILLISECONDS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_AGING_TIME_IN_MILLISECONDS  0x000000b8U

/* aging time in milliseconds */
#define MSK_ETH2PS_AGING_TIME_IN_MILLISECONDS_VAL  0xffffffffU
#define SRT_ETH2PS_AGING_TIME_IN_MILLISECONDS_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_BRIDGE_DELAY_ADD_IN_NS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BRIDGE_DELAY_ADD_IN_NS  0x000000bcU

/* value added to bridge delay when adding bridge delay to correction field when forwarding PTP Event frames */
#define MSK_ETH2PS_BRIDGE_DELAY_ADD_IN_NS_VAL  0xffffffffU
#define SRT_ETH2PS_BRIDGE_DELAY_ADD_IN_NS_VAL            0


/* ===================================================================== */
/* Area ETH2PS_RXBUF */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_RXBUF  0x000000c0U

/* ===================================================================== */
/* Area ETH2PS_TXBUF1 */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_TXBUF1  0x000006c0U

/* ===================================================================== */
/* Area ETH2PS_TXBUF2 */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_TXBUF2  0x000006d8U

/* ===================================================================== */
/* Area ETH2PS_BRIDGECFG_AREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_BRIDGECFG_AREABASE  0x000006f0U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT  0x000006f0U

/* 0xffffff */
#define MSK_ETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT_VAL  0xffffffffU
#define SRT_ETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_BPDU_ONLY */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_BPDU_ONLY  0x000006f4U

/* 1 */
#define MSK_ETH2PS_RX_BPDU_ONLY_VAL  0xffffffffU
#define SRT_ETH2PS_RX_BPDU_ONLY_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_DIS_LEARNING */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_DIS_LEARNING  0x000006f8U

/* res */
#define MSK_ETH2PS_RX_DIS_LEARNING_RES1  0x0000000fU
#define SRT_ETH2PS_RX_DIS_LEARNING_RES1            0

/* 1 */
#define MSK_ETH2PS_RX_DIS_LEARNING_VAL  0x00000010U
#define SRT_ETH2PS_RX_DIS_LEARNING_VAL            4

/* res */
#define MSK_ETH2PS_RX_DIS_LEARNING_RES2  0xffffffe0U
#define SRT_ETH2PS_RX_DIS_LEARNING_RES2            5


/* --------------------------------------------------------------------- */
/* Register ETH2PS_BPDU_MAC */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BPDU_MAC  0x000006fcU

/* 0 */
#define MSK_ETH2PS_BPDU_MAC_VAL  0xffffffffU
#define SRT_ETH2PS_BPDU_MAC_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_DLR_MAC */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_DLR_MAC  0x00000700U

/* 0 */
#define MSK_ETH2PS_DLR_MAC_VAL  0xffffffffU
#define SRT_ETH2PS_DLR_MAC_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RCV_ENABLED_BEACON */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RCV_ENABLED_BEACON  0x00000704U

/* 0 */
#define MSK_ETH2PS_RCV_ENABLED_BEACON_VAL  0xffffffffU
#define SRT_ETH2PS_RCV_ENABLED_BEACON_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_TX_ENABLE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_TX_ENABLE  0x00000708U

/* 0 */
#define MSK_ETH2PS_TX_ENABLE_VAL  0xffffffffU
#define SRT_ETH2PS_TX_ENABLE_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_TX_LINK_CHK_EN */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_TX_LINK_CHK_EN  0x0000070cU

/* bit mask of statcfg.phy_led_link, 0 */
#define MSK_ETH2PS_TX_LINK_CHK_EN_VAL  0xffffffffU
#define SRT_ETH2PS_TX_LINK_CHK_EN_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LINK_INPUT_EN */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LINK_INPUT_EN  0x00000710U

/* 1 */
#define MSK_ETH2PS_LINK_INPUT_EN_VAL  0xffffffffU
#define SRT_ETH2PS_LINK_INPUT_EN_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_TX_BPDU_ONLY */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_TX_BPDU_ONLY  0x00000714U

/* 1 */
#define MSK_ETH2PS_TX_BPDU_ONLY_VAL  0xffffffffU
#define SRT_ETH2PS_TX_BPDU_ONLY_VAL            0


/* ===================================================================== */
/* Area ETH2PS_HASH_TABLE_LOCAL_PORT_LEARN */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_HASH_TABLE_LOCAL_PORT_LEARN  0x00000718U

/* ===================================================================== */
/* Area ETH2PS_COLLECTING_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_COLLECTING_AREA  0x00000b18U

/* ===================================================================== */
/* Area ETH2PS_FRAME_LEN_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_FRAME_LEN_AREA  0x00000b24U

/* ===================================================================== */
/* Area ETH2PS_RX_TIMESTAMP_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_RX_TIMESTAMP_AREA  0x00000c74U

/* ===================================================================== */
/* Area ETH2PS_TX_TIMESTAMP_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_TX_TIMESTAMP_AREA  0x00000dc4U

/* ===================================================================== */
/* Area ETH2PS_MULTICAST_FILTER_TABLE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_MULTICAST_FILTER_TABLE  0x00000f14U

/* ===================================================================== */
/* Area ETH2PS_TS_COR */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_TS_COR  0x00001114U

/* ===================================================================== */
/* Area ETH2PS_RESERVED */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_RESERVED  0x0000111cU

/* --------------------------------------------------------------------- */
/* Register ETH2PS_ARM2XPEC_INTERRUPTS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ARM2XPEC_INTERRUPTS  0x0000111cU

/* xpec2arm interrupts */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_UNUSABLE  0x0000ffffU
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_UNUSABLE            0

/* start the beacon timeout timer */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_START_BEACON_TIMEOUT  0x00010000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_START_BEACON_TIMEOUT           16

/* stop the beacon timeout timer */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_STOP_BEACON_TIMEOUT  0x00020000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_STOP_BEACON_TIMEOUT           17

/* flush unicast mac table */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_FLUSH_UNICAST_TABLE  0x00040000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_FLUSH_UNICAST_TABLE           18

/* res */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_RES1  0x00080000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_RES1           19

/* 1 */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ  0x00100000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ           20

/* 1 */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_VALID  0x00200000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_VALID           21

/* 1 */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_SPEED100  0x00400000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_SPEED100           22

/* 1 */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_FDX  0x00800000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_FDX           23

/* res */
#define MSK_ETH2PS_ARM2XPEC_INTERRUPTS_RES2  0xff000000U
#define SRT_ETH2PS_ARM2XPEC_INTERRUPTS_RES2           24


/* --------------------------------------------------------------------- */
/* Register ETH2PS_FIFO_ELEMENT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_FIFO_ELEMENT  0x00001120U

/* frame buffer number in INTRAM */
#define MSK_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER  0x0000007fU
#define SRT_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER            0

/* 1 */
#define MSK_ETH2PS_FIFO_ELEMENT_MULTI_REQ  0x00000080U
#define SRT_ETH2PS_FIFO_ELEMENT_MULTI_REQ            7

/* 1 */
#define MSK_ETH2PS_FIFO_ELEMENT_PTP_EVENT  0x00000100U
#define SRT_ETH2PS_FIFO_ELEMENT_PTP_EVENT            8

/* set, if a beacon was received with precedence */
#define MSK_ETH2PS_FIFO_ELEMENT_BCN_PRECEDENCE_HIGHER  0x00000200U
#define SRT_ETH2PS_FIFO_ELEMENT_BCN_PRECEDENCE_HIGHER            9

/* set, if rx port of beacon was not equal LastBcnRcvPort */
#define MSK_ETH2PS_FIFO_ELEMENT_BCN_RX_PORT_NEQ_LAST  0x00000400U
#define SRT_ETH2PS_FIFO_ELEMENT_BCN_RX_PORT_NEQ_LAST           10

/* set, if beacon ring state was changed since last Beacon */
#define MSK_ETH2PS_FIFO_ELEMENT_BCN_RING_STATE_CHANGE  0x00000800U
#define SRT_ETH2PS_FIFO_ELEMENT_BCN_RING_STATE_CHANGE           11

/* error code, must be zero in requests */
#define MSK_ETH2PS_FIFO_ELEMENT_ERROR_CODE  0x00007000U
#define SRT_ETH2PS_FIFO_ELEMENT_ERROR_CODE           12

/* 1 */
#define MSK_ETH2PS_FIFO_ELEMENT_BPDU  0x00008000U
#define SRT_ETH2PS_FIFO_ELEMENT_BPDU           15

/* Reserved for future use */
#define MSK_ETH2PS_FIFO_ELEMENT_RES1  0x00030000U
#define SRT_ETH2PS_FIFO_ELEMENT_RES1           16

/* Indication */
#define MSK_ETH2PS_FIFO_ELEMENT_PORT  0x00040000U
#define SRT_ETH2PS_FIFO_ELEMENT_PORT           18

/* 1 */
#define MSK_ETH2PS_FIFO_ELEMENT_CON_EN  0x00080000U
#define SRT_ETH2PS_FIFO_ELEMENT_CON_EN           19

/* confirmation traffic class 0 */
#define MSK_ETH2PS_FIFO_ELEMENT_CON_TC  0x00100000U
#define SRT_ETH2PS_FIFO_ELEMENT_CON_TC           20

/* Reserved for future use */
#define MSK_ETH2PS_FIFO_ELEMENT_RES2  0xffe00000U
#define SRT_ETH2PS_FIFO_ELEMENT_RES2           21


/* --------------------------------------------------------------------- */
/* Register ETH2PS_HELP */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_HELP  0x00001124U

/* 1 */
#define MSK_ETH2PS_HELP_RX_FRWD_FRAME_TO_LOCAL_PORT  0x00000001U
#define SRT_ETH2PS_HELP_RX_FRWD_FRAME_TO_LOCAL_PORT            0

/* 1 */
#define MSK_ETH2PS_HELP_RX_FRWD_FRAME_TO_OPP_PORT  0x00000002U
#define SRT_ETH2PS_HELP_RX_FRWD_FRAME_TO_OPP_PORT            1

/* 0 */
#define MSK_ETH2PS_HELP_RX_PRIO_HI  0x00000004U
#define SRT_ETH2PS_HELP_RX_PRIO_HI            2

/* 1 */
#define MSK_ETH2PS_HELP_RX_BEACON  0x00000008U
#define SRT_ETH2PS_HELP_RX_BEACON            3

/* 1 */
#define MSK_ETH2PS_HELP_RX_DIS_LEARNING  0x00000010U
#define SRT_ETH2PS_HELP_RX_DIS_LEARNING            4

/* res */
#define MSK_ETH2PS_HELP_RES1  0xffffffe0U
#define SRT_ETH2PS_HELP_RES1            5


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SR_LINK_MODE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SR_LINK_MODE  0x00001128U

/* 1 */
#define MSK_ETH2PS_SR_LINK_MODE_VALID  0x00000001U
#define SRT_ETH2PS_SR_LINK_MODE_VALID            0

/* 1 */
#define MSK_ETH2PS_SR_LINK_MODE_SPEED100  0x00000002U
#define SRT_ETH2PS_SR_LINK_MODE_SPEED100            1

/* 1 */
#define MSK_ETH2PS_SR_LINK_MODE_FDX  0x00000004U
#define SRT_ETH2PS_SR_LINK_MODE_FDX            2

/* reserved */
#define MSK_ETH2PS_SR_LINK_MODE_RES2  0x0000fff8U
#define SRT_ETH2PS_SR_LINK_MODE_RES2            3

/* unusable */
#define MSK_ETH2PS_SR_LINK_MODE_UNUSABLE  0xffff0000U
#define SRT_ETH2PS_SR_LINK_MODE_UNUSABLE           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SR_CONFIG */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SR_CONFIG  0x0000112cU

/* error code at transmission */
#define MSK_ETH2PS_SR_CONFIG_ERROR_CODE  0x00000007U
#define SRT_ETH2PS_SR_CONFIG_ERROR_CODE            0

/* reserved */
#define MSK_ETH2PS_SR_CONFIG_RES1  0x00003ff8U
#define SRT_ETH2PS_SR_CONFIG_RES1            3

/* 0- */
#define MSK_ETH2PS_SR_CONFIG_TX_TIME_VALID  0x00004000U
#define SRT_ETH2PS_SR_CONFIG_TX_TIME_VALID           14

/* 0- */
#define MSK_ETH2PS_SR_CONFIG_STOP_TRANSMIT  0x00008000U
#define SRT_ETH2PS_SR_CONFIG_STOP_TRANSMIT           15

/* unusable */
#define MSK_ETH2PS_SR_CONFIG_RES3  0xffff0000U
#define SRT_ETH2PS_SR_CONFIG_RES3           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SR_STATUS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SR_STATUS  0x00001130U

/* frame length in bytes */
#define MSK_ETH2PS_SR_STATUS_LENGTH_ERR_CODE  0x00003fffU
#define SRT_ETH2PS_SR_STATUS_LENGTH_ERR_CODE            0

/* frame is erroneous */
#define MSK_ETH2PS_SR_STATUS_ERR  0x00004000U
#define SRT_ETH2PS_SR_STATUS_ERR           14

/* reception finished */
#define MSK_ETH2PS_SR_STATUS_FRAME_FIN  0x00008000U
#define SRT_ETH2PS_SR_STATUS_FRAME_FIN           15

/* unusable */
#define MSK_ETH2PS_SR_STATUS_RES3  0xffff0000U
#define SRT_ETH2PS_SR_STATUS_RES3           16


typedef struct ETH2PS_XPEC_PROGRAM_Ttag
{
  volatile uint32_t aulDataField[1];
} ETH2PS_XPEC_PROGRAM_T;

typedef struct ETH2PS_STATUS_AREA_BASE_Ttag
{
  volatile uint32_t ulETH2PS_OUT_OCTETS;
  volatile uint32_t ulETH2PS_SINGLE_COLLISION_FRAMES;
  volatile uint32_t ulETH2PS_MULTIPLE_COLLISION_FRAMES;
  volatile uint32_t ulETH2PS_LATE_COLLISIONS;
  volatile uint32_t ulETH2PS_UTX_UNDERFLOW_DURING_TRANSMISSION;
  volatile uint32_t ulETH2PS_FRAMES_ABORTED;
  volatile uint32_t ulETH2PS_OUT_FRAMES_DISCARDED;
  volatile uint32_t ulETH2PS_IN_OCTETS;
  volatile uint32_t ulETH2PS_FRAME_CHECK_SEQUENCE_ERRORS;
  volatile uint32_t ulETH2PS_ALIGNMENT_ERRORS;
  volatile uint32_t ulETH2PS_FRAME_TOO_LONG_ERRORS;
  volatile uint32_t ulETH2PS_RUNT_FRAMES_RECEIVED;
  volatile uint32_t ulETH2PS_COLLISION_FRAGMENTS_RECEIVED;
  volatile uint32_t ulETH2PS_FRAMES_DROPPED_DUE_URX_OVERFLOW;
  volatile uint32_t ulETH2PS_IN_FRAMES_DISCARDED;
  volatile uint32_t ulETH2PS_CIRCULATING_FRAMES_BLOCKED;
  volatile uint32_t ulETH2PS_UNKNOWN_ERRORS;
} ETH2PS_STATUS_AREA_BASE_T;

typedef struct ETH2PS_TX_AREA_Ttag
{
  volatile uint32_t ulETH2PS_TX_RETRY;
  volatile uint32_t ulETH2PS_TX_TX_TIMESTAMP;
  volatile uint32_t ulETH2PS_CORR_FIELD_UPPER;
  volatile uint32_t ulETH2PS_CORR_FIELD_LOWER;
} ETH2PS_TX_AREA_T;

typedef struct ETH2PS_RX_AREA_Ttag
{
  volatile uint32_t ulETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK;
  volatile uint32_t ulETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET;
  volatile uint32_t ulETH2PS_RX_SRC_MAC_ADDRESS_LO;
  volatile uint32_t ulETH2PS_RX_SRC_MAC_ADDRESS_HI;
} ETH2PS_RX_AREA_T;

typedef struct ETH2PS_BEACON_CTRL_AREABASE_Ttag
{
  volatile uint32_t ulETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT;
  volatile uint32_t ulETH2PS_ACTIVE_SVR_MAC_LO;
  volatile uint32_t ulETH2PS_ACTIVE_SVR_MAC_HI;
  volatile uint32_t ulETH2PS_ACTIVE_SVR_PRECEDENCE;
  volatile uint32_t ulETH2PS_BEACON_TIMEOUT;
  volatile uint32_t ulETH2PS_LAST_RING_STATE;
  volatile uint32_t ulETH2PS_LAST_BEACON_RX_PORT;
} ETH2PS_BEACON_CTRL_AREABASE_T;

typedef struct ETH2PS_IRQMASK_AREABASE_Ttag
{
  volatile uint32_t ulETH2PS_IND_CNF_HI_IRQ_MASK;
  volatile uint32_t ulETH2PS_IND_CNF_LO_IRQ_MASK;
  volatile uint32_t ulETH2PS_LINK_STA_CHANGED_IRQ_MASK;
  volatile uint32_t ulETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK;
  volatile uint32_t ulETH2PS_BCN_TIMEOUT_IRQ_MASK;
} ETH2PS_IRQMASK_AREABASE_T;

typedef struct ETH2PS_CONFIGAREABASE_Ttag
{
  volatile uint32_t ulETH2PS_DSCP_PRIORIZATION;
  volatile uint32_t ulETH2PS_INTERFACE_MAC_ADDRESS_LO;
  volatile uint32_t ulETH2PS_INTERFACE_MAC_ADDRESS_HI;
  volatile uint32_t ulETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO;
  volatile uint32_t ulETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI;
  volatile uint32_t ulETH2PS_SYSTIME_BORDER_COPY_PLUS1;
  volatile uint32_t ulETH2PS_TS_COR_BASE_TX;
  volatile uint32_t ulETH2PS_TS_COR_BASE_RX;
  volatile uint32_t ulETH2PS_AGING_TIME_IN_MILLISECONDS;
  volatile uint32_t ulETH2PS_BRIDGE_DELAY_ADD_IN_NS;
} ETH2PS_CONFIGAREABASE_T;

typedef struct ETH2PS_RXBUF_Ttag
{
  volatile uint32_t aulDataField[384];
} ETH2PS_RXBUF_T;

typedef struct ETH2PS_TXBUF1_Ttag
{
  volatile uint32_t aulDataField[6];
} ETH2PS_TXBUF1_T;

typedef struct ETH2PS_TXBUF2_Ttag
{
  volatile uint32_t aulDataField[6];
} ETH2PS_TXBUF2_T;

typedef struct ETH2PS_BRIDGECFG_AREABASE_Ttag
{
  volatile uint32_t ulETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT;
  volatile uint32_t ulETH2PS_RX_BPDU_ONLY;
  volatile uint32_t ulETH2PS_RX_DIS_LEARNING;
  volatile uint32_t ulETH2PS_BPDU_MAC;
  volatile uint32_t ulETH2PS_DLR_MAC;
  volatile uint32_t ulETH2PS_RCV_ENABLED_BEACON;
  volatile uint32_t ulETH2PS_TX_ENABLE;
  volatile uint32_t ulETH2PS_TX_LINK_CHK_EN;
  volatile uint32_t ulETH2PS_LINK_INPUT_EN;
  volatile uint32_t ulETH2PS_TX_BPDU_ONLY;
} ETH2PS_BRIDGECFG_AREABASE_T;

typedef struct ETH2PS_HASH_TABLE_LOCAL_PORT_LEARN_Ttag
{
  volatile uint32_t aulDataField[256];
} ETH2PS_HASH_TABLE_LOCAL_PORT_LEARN_T;

typedef struct ETH2PS_COLLECTING_AREA_Ttag
{
  volatile uint32_t aulDataField[3];
} ETH2PS_COLLECTING_AREA_T;

typedef struct ETH2PS_FRAME_LEN_AREA_Ttag
{
  volatile uint32_t aulDataField[84];
} ETH2PS_FRAME_LEN_AREA_T;

typedef struct ETH2PS_RX_TIMESTAMP_AREA_Ttag
{
  volatile uint32_t aulDataField[84];
} ETH2PS_RX_TIMESTAMP_AREA_T;

typedef struct ETH2PS_TX_TIMESTAMP_AREA_Ttag
{
  volatile uint32_t aulDataField[84];
} ETH2PS_TX_TIMESTAMP_AREA_T;

typedef struct ETH2PS_MULTICAST_FILTER_TABLE_Ttag
{
  volatile uint32_t aulDataField[128];
} ETH2PS_MULTICAST_FILTER_TABLE_T;

typedef struct ETH2PS_TS_COR_Ttag
{
  volatile uint32_t aulDataField[2];
} ETH2PS_TS_COR_T;

typedef struct ETH2PS_XPEC_DPM_Ttag {
  ETH2PS_XPEC_PROGRAM_T tETH2PS_XPEC_PROGRAM;
  ETH2PS_STATUS_AREA_BASE_T tETH2PS_STATUS_AREA_BASE;
  ETH2PS_TX_AREA_T tETH2PS_TX_AREA;
  ETH2PS_RX_AREA_T tETH2PS_RX_AREA;
  ETH2PS_BEACON_CTRL_AREABASE_T tETH2PS_BEACON_CTRL_AREABASE;
  ETH2PS_IRQMASK_AREABASE_T tETH2PS_IRQMASK_AREABASE;
  ETH2PS_CONFIGAREABASE_T tETH2PS_CONFIGAREABASE;
  ETH2PS_RXBUF_T tETH2PS_RXBUF;
  ETH2PS_TXBUF1_T tETH2PS_TXBUF1;
  ETH2PS_TXBUF2_T tETH2PS_TXBUF2;
  ETH2PS_BRIDGECFG_AREABASE_T tETH2PS_BRIDGECFG_AREABASE;
  ETH2PS_HASH_TABLE_LOCAL_PORT_LEARN_T tETH2PS_HASH_TABLE_LOCAL_PORT_LEARN;
  ETH2PS_COLLECTING_AREA_T tETH2PS_COLLECTING_AREA;
  ETH2PS_FRAME_LEN_AREA_T tETH2PS_FRAME_LEN_AREA;
  ETH2PS_RX_TIMESTAMP_AREA_T tETH2PS_RX_TIMESTAMP_AREA;
  ETH2PS_TX_TIMESTAMP_AREA_T tETH2PS_TX_TIMESTAMP_AREA;
  ETH2PS_MULTICAST_FILTER_TABLE_T tETH2PS_MULTICAST_FILTER_TABLE;
  ETH2PS_TS_COR_T tETH2PS_TS_COR;
} ETH2PS_XPEC_DPM_T;

#endif /* eth2ps_xpec_regdef_h__ */

