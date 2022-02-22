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


/* ===================================================================== */
/* Area ETH2PS_RX_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_RX_AREA  0x00000050U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK  0x00000050U

/* Mask for byte within hash table */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET  0x00000054U

/* Offset address within hash table */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET_VAL  0xffffffffU
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_HASH_OFFSET_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_SRC_MAC_ADDRESS_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_SRC_MAC_ADDRESS_LO  0x00000058U

/* MAC address bits 31..0 */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_LO_MAC_ADDR_LO  0xffffffffU
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_LO_MAC_ADDR_LO            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_SRC_MAC_ADDRESS_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_SRC_MAC_ADDRESS_HI  0x0000005cU

/* MAC address bits 47..32 */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_HI_MAC_ADDR_HI  0x0000ffffU
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_HI_MAC_ADDR_HI            0

/* reserved */
#define MSK_ETH2PS_RX_SRC_MAC_ADDRESS_HI_RES1  0xffff0000U
#define SRT_ETH2PS_RX_SRC_MAC_ADDRESS_HI_RES1           16


/* ===================================================================== */
/* Area ETH2PS_BEACON_CTRL_AREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_BEACON_CTRL_AREABASE  0x00000060U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT  0x00000060U

/* 1 */
#define MSK_ETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT_VAL  0xffffffffU
#define SRT_ETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_ACTIVE_SVR_MAC_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ACTIVE_SVR_MAC_LO  0x00000064U

/* MAC address bits 31..0 */
#define MSK_ETH2PS_ACTIVE_SVR_MAC_LO_MAC_ADDR_LO  0xffffffffU
#define SRT_ETH2PS_ACTIVE_SVR_MAC_LO_MAC_ADDR_LO            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_ACTIVE_SVR_MAC_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ACTIVE_SVR_MAC_HI  0x00000068U

/* MAC address bits 47..32 */
#define MSK_ETH2PS_ACTIVE_SVR_MAC_HI_MAC_ADDR_HI  0x0000ffffU
#define SRT_ETH2PS_ACTIVE_SVR_MAC_HI_MAC_ADDR_HI            0

/* reserved */
#define MSK_ETH2PS_ACTIVE_SVR_MAC_HI_RES1  0xffff0000U
#define SRT_ETH2PS_ACTIVE_SVR_MAC_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_ACTIVE_SVR_PRECEDENCE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ACTIVE_SVR_PRECEDENCE  0x0000006cU

/* current precedence */
#define MSK_ETH2PS_ACTIVE_SVR_PRECEDENCE_VAL  0x000000ffU
#define SRT_ETH2PS_ACTIVE_SVR_PRECEDENCE_VAL            0

/* reserved */
#define MSK_ETH2PS_ACTIVE_SVR_PRECEDENCE_RES1  0xffffff00U
#define SRT_ETH2PS_ACTIVE_SVR_PRECEDENCE_RES1            8


/* --------------------------------------------------------------------- */
/* Register ETH2PS_BEACON_TIMEOUT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BEACON_TIMEOUT  0x00000070U

/* beacon timeout in 10 ns */
#define MSK_ETH2PS_BEACON_TIMEOUT_VAL  0xffffffffU
#define SRT_ETH2PS_BEACON_TIMEOUT_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LAST_RING_STATE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LAST_RING_STATE  0x00000074U

/* last ring state */
#define MSK_ETH2PS_LAST_RING_STATE_VAL  0x000000ffU
#define SRT_ETH2PS_LAST_RING_STATE_VAL            0

/* reserved */
#define MSK_ETH2PS_LAST_RING_STATE_RES1  0xffffff00U
#define SRT_ETH2PS_LAST_RING_STATE_RES1            8


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LAST_BEACON_RX_PORT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LAST_BEACON_RX_PORT  0x00000078U

/* LastBcnRvcPort */
#define MSK_ETH2PS_LAST_BEACON_RX_PORT_VAL  0xffffffffU
#define SRT_ETH2PS_LAST_BEACON_RX_PORT_VAL            0


/* ===================================================================== */
/* Area ETH2PS_IRQMASK_AREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_IRQMASK_AREABASE  0x0000007cU

/* --------------------------------------------------------------------- */
/* Register ETH2PS_IND_CNF_HI_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_IND_CNF_HI_IRQ_MASK  0x0000007cU

/* interrupt mask */
#define MSK_ETH2PS_IND_CNF_HI_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_IND_CNF_HI_IRQ_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_IND_CNF_LO_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_IND_CNF_LO_IRQ_MASK  0x00000080U

/* interrupt mask */
#define MSK_ETH2PS_IND_CNF_LO_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_IND_CNF_LO_IRQ_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LINK_STA_CHANGED_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LINK_STA_CHANGED_IRQ_MASK  0x00000084U

/* interrupt mask */
#define MSK_ETH2PS_LINK_STA_CHANGED_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_LINK_STA_CHANGED_IRQ_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK  0x00000088U

/* interrupt mask */
#define MSK_ETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_BCN_TIMEOUT_IRQ_MASK */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BCN_TIMEOUT_IRQ_MASK  0x0000008cU

/* interrupt mask */
#define MSK_ETH2PS_BCN_TIMEOUT_IRQ_MASK_VAL  0xffffffffU
#define SRT_ETH2PS_BCN_TIMEOUT_IRQ_MASK_VAL            0


/* ===================================================================== */
/* Area ETH2PS_CONFIGAREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_CONFIGAREABASE  0x00000090U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_DSCP_PRIORIZATION */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_DSCP_PRIORIZATION  0x00000090U

/* Traffic Class arrangement for DSCP frames */
#define MSK_ETH2PS_DSCP_PRIORIZATION_VAL  0xffffffffU
#define SRT_ETH2PS_DSCP_PRIORIZATION_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_INTERFACE_MAC_ADDRESS_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_INTERFACE_MAC_ADDRESS_LO  0x00000094U

/* Interface MAC address bytes 4,3,2 and 1 */
#define MSK_ETH2PS_INTERFACE_MAC_ADDRESS_LO_BYTES_4321  0xffffffffU
#define SRT_ETH2PS_INTERFACE_MAC_ADDRESS_LO_BYTES_4321            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_INTERFACE_MAC_ADDRESS_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_INTERFACE_MAC_ADDRESS_HI  0x00000098U

/* Interface MAC address bytes 6 and 5 */
#define MSK_ETH2PS_INTERFACE_MAC_ADDRESS_HI_BYTES_65  0x0000ffffU
#define SRT_ETH2PS_INTERFACE_MAC_ADDRESS_HI_BYTES_65            0

/* 0x0000 */
#define MSK_ETH2PS_INTERFACE_MAC_ADDRESS_HI_RES1  0xffff0000U
#define SRT_ETH2PS_INTERFACE_MAC_ADDRESS_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO  0x0000009cU

/* Interface MAC address bytes 4,3,2 and 1 */
#define MSK_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO_BYTES_4321  0xffffffffU
#define SRT_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO_BYTES_4321            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI  0x000000a0U

/* Interface MAC address bytes 6 and 5 */
#define MSK_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI_BYTES_65  0x0000ffffU
#define SRT_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI_BYTES_65            0

/* 0x0000 */
#define MSK_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI_RES1  0xffff0000U
#define SRT_ETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_PHY_LEDS_BLINK_FREQ_MS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_PHY_LEDS_BLINK_FREQ_MS  0x000000a4U

/* blink frequency of PHY LEDs in milliseconds */
#define MSK_ETH2PS_PHY_LEDS_BLINK_FREQ_MS_VAL  0xffffffffU
#define SRT_ETH2PS_PHY_LEDS_BLINK_FREQ_MS_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYSTIME_BORDER_COPY_PLUS1 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYSTIME_BORDER_COPY_PLUS1  0x000000a8U

/* wrap-around value of systime_ns, systime_s */
#define MSK_ETH2PS_SYSTIME_BORDER_COPY_PLUS1_VAL  0xffffffffU
#define SRT_ETH2PS_SYSTIME_BORDER_COPY_PLUS1_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFF0000 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFF0000  0x000000acU

/* systime border masked with 0xffff0000, required for receive timestamp calculation */
#define MSK_ETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFF0000_VAL  0xffffffffU
#define SRT_ETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFF0000_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFFC000 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFFC000  0x000000b0U

/* systime border masked with 0xffff0000, required for transmit timestamp calculation */
#define MSK_ETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFFC000_VAL  0xffffffffU
#define SRT_ETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFFC000_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_AGING_TIME_IN_MILLISECONDS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_AGING_TIME_IN_MILLISECONDS  0x000000b4U

/* aging time in milliseconds */
#define MSK_ETH2PS_AGING_TIME_IN_MILLISECONDS_VAL  0xffffffffU
#define SRT_ETH2PS_AGING_TIME_IN_MILLISECONDS_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_BRIDGE_DELAY_ADD_IN_NS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_BRIDGE_DELAY_ADD_IN_NS  0x000000b8U

/* value added to bridge delay when adding bridge delay to correction field when forwarding PTP Event frames */
#define MSK_ETH2PS_BRIDGE_DELAY_ADD_IN_NS_VAL  0xffffffffU
#define SRT_ETH2PS_BRIDGE_DELAY_ADD_IN_NS_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LED_CONFIG */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LED_CONFIG  0x000000bcU

/* 0 */
#define MSK_ETH2PS_LED_CONFIG_LED_CONFIG  0x00000003U
#define SRT_ETH2PS_LED_CONFIG_LED_CONFIG            0

/* reserved, shall be set to zero */
#define MSK_ETH2PS_LED_CONFIG_RES1  0x0000fffcU
#define SRT_ETH2PS_LED_CONFIG_RES1            2

/* jump label, set by xPEC */
#define MSK_ETH2PS_LED_CONFIG_LABEL  0xffff0000U
#define SRT_ETH2PS_LED_CONFIG_LABEL           16


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
/* Area ETH2PS_STATUSAREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_STATUSAREABASE  0x000006f0U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_PHY_LEDS_TEMP */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_PHY_LEDS_TEMP  0x000006f0U

/* stores last value of leds for blinking in single LED mode */
#define MSK_ETH2PS_PHY_LEDS_TEMP_VAL  0xffffffffU
#define SRT_ETH2PS_PHY_LEDS_TEMP_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_LEDBLINK_CURRENT_TIMER */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_LEDBLINK_CURRENT_TIMER  0x000006f4U

/* current led blink timer in milliseconds */
#define MSK_ETH2PS_LEDBLINK_CURRENT_TIMER_VAL  0xffffffffU
#define SRT_ETH2PS_LEDBLINK_CURRENT_TIMER_VAL            0


/* ===================================================================== */
/* Area ETH2PS_BRIDGECFG_AREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_BRIDGECFG_AREABASE  0x000006f8U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT  0x000006f8U

/* 0xffffff */
#define MSK_ETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT_VAL  0xffffffffU
#define SRT_ETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_BPDU_ONLY */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_BPDU_ONLY  0x000006fcU

/* 1 */
#define MSK_ETH2PS_RX_BPDU_ONLY_VAL  0xffffffffU
#define SRT_ETH2PS_RX_BPDU_ONLY_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_DIS_LEARNING */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_DIS_LEARNING  0x00000700U

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

#define REL_Adr_ETH2PS_BPDU_MAC  0x00000704U

/* 0 */
#define MSK_ETH2PS_BPDU_MAC_VAL  0xffffffffU
#define SRT_ETH2PS_BPDU_MAC_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_DLR_MAC */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_DLR_MAC  0x00000708U

/* 0 */
#define MSK_ETH2PS_DLR_MAC_VAL  0xffffffffU
#define SRT_ETH2PS_DLR_MAC_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RCV_ENABLED_BEACON */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RCV_ENABLED_BEACON  0x0000070cU

/* 0 */
#define MSK_ETH2PS_RCV_ENABLED_BEACON_VAL  0xffffffffU
#define SRT_ETH2PS_RCV_ENABLED_BEACON_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_TX_ENABLE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_TX_ENABLE  0x00000710U

/* 0 */
#define MSK_ETH2PS_TX_ENABLE_VAL  0xffffffffU
#define SRT_ETH2PS_TX_ENABLE_VAL            0


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
/* Area ETH2PS_RX_STATUS_SAVE_BASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_RX_STATUS_SAVE_BASE  0x00000b20U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_STATUS_SAVE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_STATUS_SAVE  0x00000b20U

/* stores last rpu state */
#define MSK_ETH2PS_RX_STATUS_SAVE_VAL  0xffffffffU
#define SRT_ETH2PS_RX_STATUS_SAVE_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_RX_LAST_DMA_ADDR */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_RX_LAST_DMA_ADDR  0x00000b24U

/* stores last DMA address at rx frame finished */
#define MSK_ETH2PS_RX_LAST_DMA_ADDR_VAL  0xffffffffU
#define SRT_ETH2PS_RX_LAST_DMA_ADDR_VAL            0


/* ===================================================================== */
/* Area ETH2PS_FRAME_LEN_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_FRAME_LEN_AREA  0x00000b28U

/* ===================================================================== */
/* Area ETH2PS_RX_TIMESTAMP_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_RX_TIMESTAMP_AREA  0x00000bd0U

/* ===================================================================== */
/* Area ETH2PS_TX_TIMESTAMP_AREA */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_TX_TIMESTAMP_AREA  0x00000c78U

/* ===================================================================== */
/* Area ETH2PS_MULTICAST_FILTER_TABLE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_MULTICAST_FILTER_TABLE  0x00000d20U

/* ===================================================================== */
/* Area ETH2PS_RESERVED */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_RESERVED  0x00000f20U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_ARM2XPEC_INTERRUPTS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_ARM2XPEC_INTERRUPTS  0x00000f20U

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

#define REL_Adr_ETH2PS_FIFO_ELEMENT  0x00000f24U

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

#define REL_Adr_ETH2PS_HELP  0x00000f28U

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

/* 1 */
#define MSK_ETH2PS_HELP_VALID  0x00000020U
#define SRT_ETH2PS_HELP_VALID            5

/* 1 */
#define MSK_ETH2PS_HELP_SPEED100  0x00000040U
#define SRT_ETH2PS_HELP_SPEED100            6

/* 1 */
#define MSK_ETH2PS_HELP_FDX  0x00000080U
#define SRT_ETH2PS_HELP_FDX            7

/* 1 */
#define MSK_ETH2PS_HELP_TX_BUF_FILLED_VIA_DMA  0x00000100U
#define SRT_ETH2PS_HELP_TX_BUF_FILLED_VIA_DMA            8

/* 1 */
#define MSK_ETH2PS_HELP_TX_LAST  0x00000200U
#define SRT_ETH2PS_HELP_TX_LAST            9

/* 1 */
#define MSK_ETH2PS_HELP_TX_PTP_CORRECTION_FIELD  0x00000400U
#define SRT_ETH2PS_HELP_TX_PTP_CORRECTION_FIELD           10

/* set if frame has vlan tag */
#define MSK_ETH2PS_HELP_TX_HAS_VLAN  0x00000800U
#define SRT_ETH2PS_HELP_TX_HAS_VLAN           11

/* res */
#define MSK_ETH2PS_HELP_RES2  0x00003000U
#define SRT_ETH2PS_HELP_RES2           12

/* 1 */
#define MSK_ETH2PS_HELP_TX_DMA_FIN  0x00004000U
#define SRT_ETH2PS_HELP_TX_DMA_FIN           14

/* 1 */
#define MSK_ETH2PS_HELP_TX_BUF_TRANSFERS_TO_UTX  0x00008000U
#define SRT_ETH2PS_HELP_TX_BUF_TRANSFERS_TO_UTX           15

/* this is a 16 bit shared-register, these bits are not available */
#define MSK_ETH2PS_HELP_NOT_AVAILABLE  0xffff0000U
#define SRT_ETH2PS_HELP_NOT_AVAILABLE           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SR_CONFIG */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SR_CONFIG  0x00000f2cU

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

#define REL_Adr_ETH2PS_SR_STATUS  0x00000f30U

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
  volatile uint32_t ulETH2PS_PHY_LEDS_BLINK_FREQ_MS;
  volatile uint32_t ulETH2PS_SYSTIME_BORDER_COPY_PLUS1;
  volatile uint32_t ulETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFF0000;
  volatile uint32_t ulETH2PS_SYSTIME_BORDER_COPY_MASKED_FFFFC000;
  volatile uint32_t ulETH2PS_AGING_TIME_IN_MILLISECONDS;
  volatile uint32_t ulETH2PS_BRIDGE_DELAY_ADD_IN_NS;
  volatile uint32_t ulETH2PS_LED_CONFIG;
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

typedef struct ETH2PS_STATUSAREABASE_Ttag
{
  volatile uint32_t ulETH2PS_PHY_LEDS_TEMP;
  volatile uint32_t ulETH2PS_LEDBLINK_CURRENT_TIMER;
} ETH2PS_STATUSAREABASE_T;

typedef struct ETH2PS_BRIDGECFG_AREABASE_Ttag
{
  volatile uint32_t ulETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT;
  volatile uint32_t ulETH2PS_RX_BPDU_ONLY;
  volatile uint32_t ulETH2PS_RX_DIS_LEARNING;
  volatile uint32_t ulETH2PS_BPDU_MAC;
  volatile uint32_t ulETH2PS_DLR_MAC;
  volatile uint32_t ulETH2PS_RCV_ENABLED_BEACON;
  volatile uint32_t ulETH2PS_TX_ENABLE;
  volatile uint32_t ulETH2PS_TX_BPDU_ONLY;
} ETH2PS_BRIDGECFG_AREABASE_T;

typedef struct ETH2PS_HASH_TABLE_LOCAL_PORT_LEARN_Ttag
{
  volatile uint32_t aulDataField[256];
} ETH2PS_HASH_TABLE_LOCAL_PORT_LEARN_T;

typedef struct ETH2PS_COLLECTING_AREA_Ttag
{
  volatile uint32_t aulDataField[2];
} ETH2PS_COLLECTING_AREA_T;

typedef struct ETH2PS_RX_STATUS_SAVE_BASE_Ttag
{
  volatile uint32_t ulETH2PS_RX_STATUS_SAVE;
  volatile uint32_t ulETH2PS_RX_LAST_DMA_ADDR;
} ETH2PS_RX_STATUS_SAVE_BASE_T;

typedef struct ETH2PS_FRAME_LEN_AREA_Ttag
{
  volatile uint32_t aulDataField[42];
} ETH2PS_FRAME_LEN_AREA_T;

typedef struct ETH2PS_RX_TIMESTAMP_AREA_Ttag
{
  volatile uint32_t aulDataField[42];
} ETH2PS_RX_TIMESTAMP_AREA_T;

typedef struct ETH2PS_TX_TIMESTAMP_AREA_Ttag
{
  volatile uint32_t aulDataField[42];
} ETH2PS_TX_TIMESTAMP_AREA_T;

typedef struct ETH2PS_MULTICAST_FILTER_TABLE_Ttag
{
  volatile uint32_t aulDataField[128];
} ETH2PS_MULTICAST_FILTER_TABLE_T;

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
  ETH2PS_STATUSAREABASE_T tETH2PS_STATUSAREABASE;
  ETH2PS_BRIDGECFG_AREABASE_T tETH2PS_BRIDGECFG_AREABASE;
  ETH2PS_HASH_TABLE_LOCAL_PORT_LEARN_T tETH2PS_HASH_TABLE_LOCAL_PORT_LEARN;
  ETH2PS_COLLECTING_AREA_T tETH2PS_COLLECTING_AREA;
  ETH2PS_RX_STATUS_SAVE_BASE_T tETH2PS_RX_STATUS_SAVE_BASE;
  ETH2PS_FRAME_LEN_AREA_T tETH2PS_FRAME_LEN_AREA;
  ETH2PS_RX_TIMESTAMP_AREA_T tETH2PS_RX_TIMESTAMP_AREA;
  ETH2PS_TX_TIMESTAMP_AREA_T tETH2PS_TX_TIMESTAMP_AREA;
  ETH2PS_MULTICAST_FILTER_TABLE_T tETH2PS_MULTICAST_FILTER_TABLE;
} ETH2PS_XPEC_DPM_T;

#endif /* eth2ps_xpec_regdef_h__ */

