#ifndef ethmac_xpec_regdef_h__
#define ethmac_xpec_regdef_h__

#include <stdint.h>

/* ===================================================================== */
/* Area ETHMAC_XPEC_PROGRAM */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_XPEC_PROGRAM  0x00000000U

/* ===================================================================== */
/* Area ETHMAC_TXAREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_TXAREABASE  0x00000004U

/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_BYTES_LEFT_FOR_UTX */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_BYTES_LEFT_FOR_UTX  0x00000004U

/* number of bytes left to transmit to utx fifo */
#define MSK_ETHMAC_TX_BYTES_LEFT_FOR_UTX_VAL  0xffffffffU
#define SRT_ETHMAC_TX_BYTES_LEFT_FOR_UTX_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_BYTES_LEFT_FOR_DMA */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_BYTES_LEFT_FOR_DMA  0x00000008U

/* number of bytes left for DMA */
#define MSK_ETHMAC_TX_BYTES_LEFT_FOR_DMA_VAL  0xffffffffU
#define SRT_ETHMAC_TX_BYTES_LEFT_FOR_DMA_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_RETRY */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_RETRY  0x0000000cU

/* retry counter for transmission attempts */
#define MSK_ETHMAC_TX_RETRY_CNT  0xffffffffU
#define SRT_ETHMAC_TX_RETRY_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_TIMESTAMP_NS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_TIMESTAMP_NS  0x00000010U

/* transmit timestamp ns of outgoing packet */
#define MSK_ETHMAC_TX_TIMESTAMP_NS_VAL  0xffffffffU
#define SRT_ETHMAC_TX_TIMESTAMP_NS_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_TIMESTAMP_S */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_TIMESTAMP_S  0x00000014U

/* transmit timestamp s of outgoing packet */
#define MSK_ETHMAC_TX_TIMESTAMP_S_VAL  0xffffffffU
#define SRT_ETHMAC_TX_TIMESTAMP_S_VAL            0


/* ===================================================================== */
/* Area ETHMAC_RXAREABASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_RXAREABASE  0x00000018U

/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_DA_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_DA_LO  0x00000018U

/* individual or group address */
#define MSK_ETHMAC_RX_DA_LO_I_G  0x00000001U
#define SRT_ETHMAC_RX_DA_LO_I_G            0

/* universally or locally administered address */
#define MSK_ETHMAC_RX_DA_LO_U_L  0x00000002U
#define SRT_ETHMAC_RX_DA_LO_U_L            1

/* MAC address bits 31..2 */
#define MSK_ETHMAC_RX_DA_LO_MAC_31_2  0xfffffffcU
#define SRT_ETHMAC_RX_DA_LO_MAC_31_2            2


/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_DA_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_DA_HI  0x0000001cU

/* MAC address bits 47..32 */
#define MSK_ETHMAC_RX_DA_HI_MAC_47_32  0x0000ffffU
#define SRT_ETHMAC_RX_DA_HI_MAC_47_32            0

/* Reserved */
#define MSK_ETHMAC_RX_DA_HI_RES1  0xffff0000U
#define SRT_ETHMAC_RX_DA_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_WORKING */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_WORKING  0x00000020U

/* receive help working variable */
#define MSK_ETHMAC_RX_WORKING_VAL  0xffffffffU
#define SRT_ETHMAC_RX_WORKING_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_TIMESTAMP_NS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_TIMESTAMP_NS  0x00000024U

/* receive timestamp ns of incoming packet */
#define MSK_ETHMAC_RX_TIMESTAMP_NS_VAL  0xffffffffU
#define SRT_ETHMAC_RX_TIMESTAMP_NS_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_TIMESTAMP_S */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_TIMESTAMP_S  0x00000028U

/* receive timestamp s of incoming packet */
#define MSK_ETHMAC_RX_TIMESTAMP_S_VAL  0xffffffffU
#define SRT_ETHMAC_RX_TIMESTAMP_S_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_DMA0_CFG */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_DMA0_CFG  0x0000002cU

/* dma enable mask for dma0 rx buf1 - */
#define MSK_ETHMAC_DMA0_CFG_VAL  0xffffffffU
#define SRT_ETHMAC_DMA0_CFG_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_DMA1_CFG */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_DMA1_CFG  0x00000030U

/* dma enable mask for dma1 rx buf2 - */
#define MSK_ETHMAC_DMA1_CFG_VAL  0xffffffffU
#define SRT_ETHMAC_DMA1_CFG_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_PHY_LEDS_TEMP */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_PHY_LEDS_TEMP  0x00000034U

/* stores last value of leds for blinking in single LED mode */
#define MSK_ETHMAC_PHY_LEDS_TEMP_VAL  0xffffffffU
#define SRT_ETHMAC_PHY_LEDS_TEMP_VAL            0


/* ===================================================================== */
/* Area ETHMAC_CONFIG_AREA_BASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_CONFIG_AREA_BASE  0x00000038U

/* --------------------------------------------------------------------- */
/* Register ETHMAC_MONITORING_MODE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_MONITORING_MODE  0x00000038U

/* 1 */
#define MSK_ETHMAC_MONITORING_MODE_EN  0x00000001U
#define SRT_ETHMAC_MONITORING_MODE_EN            0

/* reserved, shall be set to zero */
#define MSK_ETHMAC_MONITORING_MODE_RES1  0xfffffffeU
#define SRT_ETHMAC_MONITORING_MODE_RES1            1


/* --------------------------------------------------------------------- */
/* Register ETHMAC_LED_CONFIG */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_LED_CONFIG  0x0000003cU

/* 0 */
#define MSK_ETHMAC_LED_CONFIG_LED_CONFIG  0x00000003U
#define SRT_ETHMAC_LED_CONFIG_LED_CONFIG            0

/* reserved, shall be set to zero */
#define MSK_ETHMAC_LED_CONFIG_RES1  0xfffffffcU
#define SRT_ETHMAC_LED_CONFIG_RES1            2


/* --------------------------------------------------------------------- */
/* Register ETHMAC_PHY_LEDS_FLASH_PERIOD */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_PHY_LEDS_FLASH_PERIOD  0x00000040U

/* flash period of PHY LEDs in 10ns resolution, default */
#define MSK_ETHMAC_PHY_LEDS_FLASH_PERIOD_VAL  0xffffffffU
#define SRT_ETHMAC_PHY_LEDS_FLASH_PERIOD_VAL            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_TRAFFIC_CLASS_ARRANGEMENT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT  0x00000044U

/* Traffic Class arrangement */
/* 0: HI: vlan priority 7..0, LO: untagged frame */
/* 1: HI: vlan priority 7..1, LO: vlan priority 0, untagged frame */
/* 2: HI: vlan priority 7..2, LO: vlan priority 1..0, untagged frame */
/* 3: HI: vlan priority 7..3, LO: vlan priority 2..0, untagged frame */
/* 4: HI: vlan priority 7..4, LO: vlan priority 3..0, untagged frame (802.1Q Recommendation) */
/* 5: HI: vlan priority 7..5, LO: vlan priority 4..0, untagged frame */
/* 6: HI: vlan priority 7..6, LO: vlan priority 5..0, untagged frame */
/* 7: HI: vlan priority 7,    LO: vlan priority 6..0, untagged frame */
/* 8: HI: -,                  LO: vlan priority 7..0, untagged frame */
#define MSK_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT_VAL  0x0000000fU
#define SRT_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT_VAL            0

/* reserved, shall be set to zero */
#define MSK_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT_RES1  0xfffffff0U
#define SRT_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT_RES1            4


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_IND_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_IND_HI  0x00000048U

/* enable indication hi event */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_HI_VAL  0x00000001U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_HI_VAL            0

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_HI_RES1  0xfffffffeU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_HI_RES1            1


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_IND_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_IND_LO  0x0000004cU

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_LO_RES1  0x00000001U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_LO_RES1            0

/* enable indication lo event */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_LO_VAL  0x00000002U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_LO_VAL            1

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_LO_RES2  0xfffffffcU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_LO_RES2            2


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_CON_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_CON_HI  0x00000050U

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_HI_RES1  0x00000003U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_HI_RES1            0

/* enable confirmation hi event */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_HI_VAL  0x00000004U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_HI_VAL            2

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_HI_RES2  0xfffffff8U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_HI_RES2            3


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_CON_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_CON_LO  0x00000054U

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_LO_RES1  0x00000007U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_LO_RES1            0

/* enable confirmation lo event */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_LO_VAL  0x00000008U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_LO_VAL            3

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_LO_RES2  0xfffffff0U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_LO_RES2            4


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED  0x00000058U

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_RES1  0x0000000fU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_RES1            0

/* enable link status changed event */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_VAL  0x00000010U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_VAL            4

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_RES2  0xffffffe0U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_RES2            5


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_COL */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_COL  0x0000005cU

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_COL_RES1  0x0000001fU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_COL_RES1            0

/* enable collision event */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_COL_VAL  0x00000020U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_COL_VAL            5

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_COL_RES2  0xffffffc0U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_COL_RES2            6


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV  0x00000060U

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_RES1  0x0000003fU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_RES1            0

/* enable event for destination address received and empty_ptr_got and */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_VAL  0x00000040U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_VAL            6

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_RES2  0xffffff80U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_RES2            7


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_RX_ERR */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_RX_ERR  0x00000064U

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_RES1  0x0000007fU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_RES1            0

/* enable event for rx_error event is set in case of all error cases */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_VAL  0x00000080U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_VAL            7

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_RES2  0xffffff00U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_RES2            8


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_TX_ERR */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_TX_ERR  0x00000068U

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_RES1  0x000000ffU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_RES1            0

/* enable event for tx_error, event is set in case of late_collision, excessive_collision, utx_under_flow_during_tx, tx_fatal_error */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_VAL  0x00000100U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_VAL            8

/* reserved, shall be set to zero */
#define MSK_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_RES2  0xfffffe00U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_RES2            9


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERFACE_MAC_ADDRESS_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERFACE_MAC_ADDRESS_LO  0x0000006cU

/* Interface MAC address bytes 4,3,2 and 1 */
#define MSK_ETHMAC_INTERFACE_MAC_ADDRESS_LO_BYTES_4321  0xffffffffU
#define SRT_ETHMAC_INTERFACE_MAC_ADDRESS_LO_BYTES_4321            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERFACE_MAC_ADDRESS_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERFACE_MAC_ADDRESS_HI  0x00000070U

/* MAC address bytes 6 and 5 */
#define MSK_ETHMAC_INTERFACE_MAC_ADDRESS_HI_BYTES_65  0x0000ffffU
#define SRT_ETHMAC_INTERFACE_MAC_ADDRESS_HI_BYTES_65            0

/* 0x0000 */
#define MSK_ETHMAC_INTERFACE_MAC_ADDRESS_HI_RES1  0xffff0000U
#define SRT_ETHMAC_INTERFACE_MAC_ADDRESS_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO  0x00000074U

/* Interface MAC address bytes 4,3,2 and 1 */
#define MSK_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO_BYTES_4321  0xffffffffU
#define SRT_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO_BYTES_4321            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI  0x00000078U

/* MAC address bytes 6 and 5 */
#define MSK_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI_BYTES_65  0x0000ffffU
#define SRT_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI_BYTES_65            0

/* 0x0000 */
#define MSK_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI_RES1  0xffff0000U
#define SRT_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI_RES1           16


/* --------------------------------------------------------------------- */
/* Register ETHMAC_SYSTIME_BORDER_COPY_UPPER */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SYSTIME_BORDER_COPY_UPPER  0x0000007cU

/* must be zero */
#define MSK_ETHMAC_SYSTIME_BORDER_COPY_UPPER_ZERO  0x0000ffffU
#define SRT_ETHMAC_SYSTIME_BORDER_COPY_UPPER_ZERO            0

/* wrap-around value of systime_ns, systime_s */
#define MSK_ETHMAC_SYSTIME_BORDER_COPY_UPPER_VAL  0xffff0000U
#define SRT_ETHMAC_SYSTIME_BORDER_COPY_UPPER_VAL           16


/* ===================================================================== */
/* Area ETHMAC_STATUS_AREA_BASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_STATUS_AREA_BASE  0x00000080U

/* --------------------------------------------------------------------- */
/* Register ETHMAC_OUT_OCTETS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_OUT_OCTETS  0x00000080U

/* count of bytes transmitted */
#define MSK_ETHMAC_OUT_OCTETS_CNT  0xffffffffU
#define SRT_ETHMAC_OUT_OCTETS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_SINGLE_COLLISION_FRAMES */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SINGLE_COLLISION_FRAMES  0x00000084U

/* count of frames that are involved into a single collision */
#define MSK_ETHMAC_SINGLE_COLLISION_FRAMES_CNT  0xffffffffU
#define SRT_ETHMAC_SINGLE_COLLISION_FRAMES_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_MULTIPLE_COLLISION_FRAMES */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_MULTIPLE_COLLISION_FRAMES  0x00000088U

/* count of frames that are involved into more than one collisions */
#define MSK_ETHMAC_MULTIPLE_COLLISION_FRAMES_CNT  0xffffffffU
#define SRT_ETHMAC_MULTIPLE_COLLISION_FRAMES_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_LATE_COLLISIONS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_LATE_COLLISIONS  0x0000008cU

/* count of the times that a collision has been detected later than 512 bit times into the transmitted packet */
#define MSK_ETHMAC_LATE_COLLISIONS_CNT  0xffffffffU
#define SRT_ETHMAC_LATE_COLLISIONS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_LINK_DOWN_DURING_TRANSMISSION */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_LINK_DOWN_DURING_TRANSMISSION  0x00000090U

/* count of the times that a frame was discarded during link down */
#define MSK_ETHMAC_LINK_DOWN_DURING_TRANSMISSION_CNT  0xffffffffU
#define SRT_ETHMAC_LINK_DOWN_DURING_TRANSMISSION_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION  0x00000094U

/* UTX FIFO underflow at transmission time */
#define MSK_ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION_CNT  0xffffffffU
#define SRT_ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_IN_OCTETS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_IN_OCTETS  0x00000098U

/* count of bytes in valid MAC frames received excluding Preamble, SFD and FCS */
#define MSK_ETHMAC_IN_OCTETS_CNT  0xffffffffU
#define SRT_ETHMAC_IN_OCTETS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS  0x0000009cU

/* count of frames that are an integral number of octets in length and do not pass the FCS check */
#define MSK_ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS_CNT  0xffffffffU
#define SRT_ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_ALIGNMENT_ERRORS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_ALIGNMENT_ERRORS  0x000000a0U

/* count of frames that are not an integral number of octets in length and do not pass the FCS check */
#define MSK_ETHMAC_ALIGNMENT_ERRORS_CNT  0xffffffffU
#define SRT_ETHMAC_ALIGNMENT_ERRORS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_FRAME_TOO_LONG_ERRORS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FRAME_TOO_LONG_ERRORS  0x000000a4U

/* count of frames that are received and exceed the maximum permitted frame size */
#define MSK_ETHMAC_FRAME_TOO_LONG_ERRORS_CNT  0xffffffffU
#define SRT_ETHMAC_FRAME_TOO_LONG_ERRORS_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_RUNT_FRAMES_RECEIVED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RUNT_FRAMES_RECEIVED  0x000000a8U

/* count of frames that have a length between 42..63 bytes and an valid CRC */
#define MSK_ETHMAC_RUNT_FRAMES_RECEIVED_CNT  0xffffffffU
#define SRT_ETHMAC_RUNT_FRAMES_RECEIVED_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_COLLISION_FRAGMENTS_RECEIVED */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_COLLISION_FRAGMENTS_RECEIVED  0x000000acU

/* count of frames that are smaller than 64 bytes and have an invalid CRC */
#define MSK_ETHMAC_COLLISION_FRAGMENTS_RECEIVED_CNT  0xffffffffU
#define SRT_ETHMAC_COLLISION_FRAGMENTS_RECEIVED_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE  0x000000b0U

/* no empty pointer available at indication time */
#define MSK_ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE_CNT  0xffffffffU
#define SRT_ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW  0x000000b4U

/* URX FIFO overflow at indication time */
#define MSK_ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW_CNT  0xffffffffU
#define SRT_ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_FATAL_ERROR */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_FATAL_ERROR  0x000000b8U

/* counts unknown error numbers from TX xMAC, should never occur */
#define MSK_ETHMAC_TX_FATAL_ERROR_CNT  0xffffffffU
#define SRT_ETHMAC_TX_FATAL_ERROR_CNT            0


/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_FATAL_ERROR */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_FATAL_ERROR  0x000000bcU

/* counts unknown error numbers from RX xMAC, should never occur */
#define MSK_ETHMAC_RX_FATAL_ERROR_CNT  0xffffffffU
#define SRT_ETHMAC_RX_FATAL_ERROR_CNT            0


/* ===================================================================== */
/* Area ETHMAC_TXBUF1 */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_TXBUF1  0x000000c0U

/* ===================================================================== */
/* Area ETHMAC_TXBUF2 */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_TXBUF2  0x000000e0U

/* ===================================================================== */
/* Area ETHMAC_RXBUF1 */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_RXBUF1  0x00000100U

/* ===================================================================== */
/* Area ETHMAC_RXBUF2 */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_RXBUF2  0x00000120U

/* ===================================================================== */
/* Area ETHMAC_MULTICAST_FILTER_TABLE */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_MULTICAST_FILTER_TABLE  0x00000140U

/* ===================================================================== */
/* Area ETHMAC_RESERVED */
/* ===================================================================== */

#define REL_Adr_AREA_ETHMAC_RESERVED  0x00000340U

/* --------------------------------------------------------------------- */
/* Register ETHMAC_XPEC2ARM_INTERRUPTS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_XPEC2ARM_INTERRUPTS  0x00000340U

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_IND_HI  0x00000001U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_IND_HI            0

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_IND_LO  0x00000002U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_IND_LO            1

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_CON_HI  0x00000004U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_CON_HI            2

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_CON_LO  0x00000008U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_CON_LO            3

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_LINK_CHANGED  0x00000010U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_LINK_CHANGED            4

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_COL  0x00000020U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_COL            5

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_EARLY_RCV  0x00000040U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_EARLY_RCV            6

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_RX_ERR  0x00000080U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_RX_ERR            7

/* 1 */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_TX_ERR  0x00000100U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_TX_ERR            8

/* reserved */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_RES1  0x0000fe00U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_RES1            9

/* arm2xpec interrupts */
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_UNUSABLE  0xffff0000U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_UNUSABLE           16


/* --------------------------------------------------------------------- */
/* Register ETHMAC_ARM2XPEC_INTERRUPTS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_ARM2XPEC_INTERRUPTS  0x00000344U

/* xpec2arm interrupts */
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_UNUSABLE  0x0000ffffU
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_UNUSABLE            0

/* 1 */
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ  0x00010000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ           16

/* 1 */
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_VALID  0x00020000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_VALID           17

/* 1 */
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_SPEED100  0x00040000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_SPEED100           18

/* 1 */
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_FDX  0x00080000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_FDX           19

/* reserved */
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_RES1  0xfff00000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_RES1           20


/* --------------------------------------------------------------------- */
/* Register ETHMAC_FIFO_ELEMENT */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FIFO_ELEMENT  0x00000348U

/* frame length */
#define MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN  0x000007ffU
#define SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN            0

/* reserved, shall be set to zero */
#define MSK_ETHMAC_FIFO_ELEMENT_RES1  0x00000800U
#define SRT_ETHMAC_FIFO_ELEMENT_RES1           11

/* 1 */
#define MSK_ETHMAC_FIFO_ELEMENT_MULTI_REQ  0x00003000U
#define SRT_ETHMAC_FIFO_ELEMENT_MULTI_REQ           12

/* empty */
#define MSK_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON  0x00004000U
#define SRT_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON           14

/* reserved, shall be set to zero */
#define MSK_ETHMAC_FIFO_ELEMENT_RES2  0x00008000U
#define SRT_ETHMAC_FIFO_ELEMENT_RES2           15

/* frame buffer number */
#define MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM  0x003f0000U
#define SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM           16

/* internal RAM segment number */
#define MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM  0x03c00000U
#define SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM           22

/* reserved, shall be set to zero */
#define MSK_ETHMAC_FIFO_ELEMENT_RES3  0x0c000000U
#define SRT_ETHMAC_FIFO_ELEMENT_RES3           26

/* error code */
#define MSK_ETHMAC_FIFO_ELEMENT_ERROR_CODE  0xf0000000U
#define SRT_ETHMAC_FIFO_ELEMENT_ERROR_CODE           28


/* --------------------------------------------------------------------- */
/* Register ETHMAC_HELP */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_HELP  0x0000034cU

/* 1 */
#define MSK_ETHMAC_HELP_RX_BUF_ACTIVE  0x00000001U
#define SRT_ETHMAC_HELP_RX_BUF_ACTIVE            0

/* 1 */
#define MSK_ETHMAC_HELP_RX_IND_PRIO  0x00000002U
#define SRT_ETHMAC_HELP_RX_IND_PRIO            1

/* 1 */
#define MSK_ETHMAC_HELP_RX_DROP  0x00000004U
#define SRT_ETHMAC_HELP_RX_DROP            2

/* 1 */
#define MSK_ETHMAC_HELP_RX_FRWD2LOCAL  0x00000008U
#define SRT_ETHMAC_HELP_RX_FRWD2LOCAL            3

/* 1 */
#define MSK_ETHMAC_HELP_VALID  0x00000010U
#define SRT_ETHMAC_HELP_VALID            4

/* 1 */
#define MSK_ETHMAC_HELP_SPEED100  0x00000020U
#define SRT_ETHMAC_HELP_SPEED100            5

/* 1 */
#define MSK_ETHMAC_HELP_FDX  0x00000040U
#define SRT_ETHMAC_HELP_FDX            6

/* reserved */
#define MSK_ETHMAC_HELP_RES1  0x00000380U
#define SRT_ETHMAC_HELP_RES1            7

/* 1 */
#define MSK_ETHMAC_HELP_TX_BUF_FILLED_VIA_DMA  0x00000400U
#define SRT_ETHMAC_HELP_TX_BUF_FILLED_VIA_DMA           10

/* 1 */
#define MSK_ETHMAC_HELP_TX_LAST  0x00000800U
#define SRT_ETHMAC_HELP_TX_LAST           11

/* 1 */
#define MSK_ETHMAC_HELP_TX_REQ_PRIO  0x00001000U
#define SRT_ETHMAC_HELP_TX_REQ_PRIO           12

/* 1 */
#define MSK_ETHMAC_HELP_TX_DMA_YET_FIN  0x00002000U
#define SRT_ETHMAC_HELP_TX_DMA_YET_FIN           13

/* 1 */
#define MSK_ETHMAC_HELP_TX_BUF_TRANSFERS_TO_UTX  0x00004000U
#define SRT_ETHMAC_HELP_TX_BUF_TRANSFERS_TO_UTX           14

/* reserved */
#define MSK_ETHMAC_HELP_RES2  0x00008000U
#define SRT_ETHMAC_HELP_RES2           15

/* reserved */
#define MSK_ETHMAC_HELP_RES3  0xffff0000U
#define SRT_ETHMAC_HELP_RES3           16


/* --------------------------------------------------------------------- */
/* Register ETHMAC_SR_CONFIG */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SR_CONFIG  0x00000350U

/* error code at transmission */
#define MSK_ETHMAC_SR_CONFIG_ERROR_CODE  0x00000007U
#define SRT_ETHMAC_SR_CONFIG_ERROR_CODE            0

/* res */
#define MSK_ETHMAC_SR_CONFIG_RES1  0x00003ff8U
#define SRT_ETHMAC_SR_CONFIG_RES1            3

/* 1 */
#define MSK_ETHMAC_SR_CONFIG_TIMESTAMP_VALID  0x00004000U
#define SRT_ETHMAC_SR_CONFIG_TIMESTAMP_VALID           14

/* 0- */
#define MSK_ETHMAC_SR_CONFIG_START  0x00008000U
#define SRT_ETHMAC_SR_CONFIG_START           15

/* unusable */
#define MSK_ETHMAC_SR_CONFIG_UNUSABLE  0xffff0000U
#define SRT_ETHMAC_SR_CONFIG_UNUSABLE           16


/* --------------------------------------------------------------------- */
/* Register ETHMAC_SR_STATUS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SR_STATUS  0x00000354U

/* received frame length, not used in netX100 */
#define MSK_ETHMAC_SR_STATUS_RCVD_LEN  0x000007ffU
#define SRT_ETHMAC_SR_STATUS_RCVD_LEN            0

/* reserved */
#define MSK_ETHMAC_SR_STATUS_RES  0x00000800U
#define SRT_ETHMAC_SR_STATUS_RES           11

/* error code at reception */
#define MSK_ETHMAC_SR_STATUS_ERROR_CODE  0x00007000U
#define SRT_ETHMAC_SR_STATUS_ERROR_CODE           12

/* reception finished */
#define MSK_ETHMAC_SR_STATUS_FIN  0x00008000U
#define SRT_ETHMAC_SR_STATUS_FIN           15

/* unusable */
#define MSK_ETHMAC_SR_STATUS_UNUSABLE  0xffff0000U
#define SRT_ETHMAC_SR_STATUS_UNUSABLE           16


typedef struct ETHMAC_XPEC_PROGRAM_Ttag
{
  volatile uint32_t aulDataField[1];
} ETHMAC_XPEC_PROGRAM_T;

typedef struct ETHMAC_TXAREABASE_Ttag
{
  volatile uint32_t ulETHMAC_TX_BYTES_LEFT_FOR_UTX;
  volatile uint32_t ulETHMAC_TX_BYTES_LEFT_FOR_DMA;
  volatile uint32_t ulETHMAC_TX_RETRY;
  volatile uint32_t ulETHMAC_TX_TIMESTAMP_NS;
  volatile uint32_t ulETHMAC_TX_TIMESTAMP_S;
} ETHMAC_TXAREABASE_T;

typedef struct ETHMAC_RXAREABASE_Ttag
{
  volatile uint32_t ulETHMAC_RX_DA_LO;
  volatile uint32_t ulETHMAC_RX_DA_HI;
  volatile uint32_t ulETHMAC_RX_WORKING;
  volatile uint32_t ulETHMAC_RX_TIMESTAMP_NS;
  volatile uint32_t ulETHMAC_RX_TIMESTAMP_S;
  volatile uint32_t ulETHMAC_DMA0_CFG;
  volatile uint32_t ulETHMAC_DMA1_CFG;
  volatile uint32_t ulETHMAC_PHY_LEDS_TEMP;
} ETHMAC_RXAREABASE_T;

typedef struct ETHMAC_CONFIG_AREA_BASE_Ttag
{
  volatile uint32_t ulETHMAC_MONITORING_MODE;
  volatile uint32_t ulETHMAC_LED_CONFIG;
  volatile uint32_t ulETHMAC_PHY_LEDS_FLASH_PERIOD;
  volatile uint32_t ulETHMAC_TRAFFIC_CLASS_ARRANGEMENT;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_IND_HI;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_IND_LO;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_CON_HI;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_CON_LO;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_COL;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_EARLY_RCV;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_RX_ERR;
  volatile uint32_t ulETHMAC_INTERRUPTS_ENABLE_TX_ERR;
  volatile uint32_t ulETHMAC_INTERFACE_MAC_ADDRESS_LO;
  volatile uint32_t ulETHMAC_INTERFACE_MAC_ADDRESS_HI;
  volatile uint32_t ulETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO;
  volatile uint32_t ulETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI;
  volatile uint32_t ulETHMAC_SYSTIME_BORDER_COPY_UPPER;
} ETHMAC_CONFIG_AREA_BASE_T;

typedef struct ETHMAC_STATUS_AREA_BASE_Ttag
{
  volatile uint32_t ulETHMAC_OUT_OCTETS;
  volatile uint32_t ulETHMAC_SINGLE_COLLISION_FRAMES;
  volatile uint32_t ulETHMAC_MULTIPLE_COLLISION_FRAMES;
  volatile uint32_t ulETHMAC_LATE_COLLISIONS;
  volatile uint32_t ulETHMAC_LINK_DOWN_DURING_TRANSMISSION;
  volatile uint32_t ulETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION;
  volatile uint32_t ulETHMAC_IN_OCTETS;
  volatile uint32_t ulETHMAC_FRAME_CHECK_SEQUENCE_ERRORS;
  volatile uint32_t ulETHMAC_ALIGNMENT_ERRORS;
  volatile uint32_t ulETHMAC_FRAME_TOO_LONG_ERRORS;
  volatile uint32_t ulETHMAC_RUNT_FRAMES_RECEIVED;
  volatile uint32_t ulETHMAC_COLLISION_FRAGMENTS_RECEIVED;
  volatile uint32_t ulETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE;
  volatile uint32_t ulETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW;
  volatile uint32_t ulETHMAC_TX_FATAL_ERROR;
  volatile uint32_t ulETHMAC_RX_FATAL_ERROR;
} ETHMAC_STATUS_AREA_BASE_T;

typedef struct ETHMAC_TXBUF1_Ttag
{
  volatile uint32_t aulDataField[8];
} ETHMAC_TXBUF1_T;

typedef struct ETHMAC_TXBUF2_Ttag
{
  volatile uint32_t aulDataField[8];
} ETHMAC_TXBUF2_T;

typedef struct ETHMAC_RXBUF1_Ttag
{
  volatile uint32_t aulDataField[8];
} ETHMAC_RXBUF1_T;

typedef struct ETHMAC_RXBUF2_Ttag
{
  volatile uint32_t aulDataField[8];
} ETHMAC_RXBUF2_T;

typedef struct ETHMAC_MULTICAST_FILTER_TABLE_Ttag
{
  volatile uint32_t aulDataField[128];
} ETHMAC_MULTICAST_FILTER_TABLE_T;

typedef struct ETHMAC_XPEC_DPMtag {
  ETHMAC_XPEC_PROGRAM_T tETHMAC_XPEC_PROGRAM;
  ETHMAC_TXAREABASE_T tETHMAC_TXAREABASE;
  ETHMAC_RXAREABASE_T tETHMAC_RXAREABASE;
  ETHMAC_CONFIG_AREA_BASE_T tETHMAC_CONFIG_AREA_BASE;
  ETHMAC_STATUS_AREA_BASE_T tETHMAC_STATUS_AREA_BASE;
  ETHMAC_TXBUF1_T tETHMAC_TXBUF1;
  ETHMAC_TXBUF2_T tETHMAC_TXBUF2;
  ETHMAC_RXBUF1_T tETHMAC_RXBUF1;
  ETHMAC_RXBUF2_T tETHMAC_RXBUF2;
  ETHMAC_MULTICAST_FILTER_TABLE_T tETHMAC_MULTICAST_FILTER_TABLE;
} ETHMAC_XPEC_DPM;

#endif /* ethmac_xpec_regdef_h__ */

