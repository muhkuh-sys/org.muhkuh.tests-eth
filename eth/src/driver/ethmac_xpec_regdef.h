/*********************************************************************/
/* from regdef.v by regdef2h auto-generated C-header file            */
/* please see the regdef.html file for detailed register description */
/*********************************************************************/

#ifndef __ethmac_regdef_h
#define __ethmac_regdef_h

#include <stdint.h>

/* ===================================================================== */

/* Area of ETHMACBASE */

/* ===================================================================== */


/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_TXAREABASE */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_TXAREABASE        0x00000000U

/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_RETRY */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_RETRY        0x00000000U

#define MSK_ETHMAC_TX_RETRY_CNT 0xffffffffU
#define SRT_ETHMAC_TX_RETRY_CNT 0

/* all used bits of 'ETHMAC_TX_RETRY': */
#define MSK_USED_BITS_ETHMAC_TX_RETRY 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_TIMESTAMP_NS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_TIMESTAMP_NS        0x00000004U

#define MSK_ETHMAC_TX_TIMESTAMP_NS_VAL 0xffffffffU
#define SRT_ETHMAC_TX_TIMESTAMP_NS_VAL 0

/* all used bits of 'ETHMAC_TX_TIMESTAMP_NS': */
#define MSK_USED_BITS_ETHMAC_TX_TIMESTAMP_NS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_TIMESTAMP_S */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_TIMESTAMP_S        0x00000008U

#define MSK_ETHMAC_TX_TIMESTAMP_S_VAL 0xffffffffU
#define SRT_ETHMAC_TX_TIMESTAMP_S_VAL 0

/* all used bits of 'ETHMAC_TX_TIMESTAMP_S': */
#define MSK_USED_BITS_ETHMAC_TX_TIMESTAMP_S 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_RXAREABASE */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_RXAREABASE        0x0000000CU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_TIMESTAMP_NS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_TIMESTAMP_NS        0x0000000CU

#define MSK_ETHMAC_RX_TIMESTAMP_NS_VAL 0xffffffffU
#define SRT_ETHMAC_RX_TIMESTAMP_NS_VAL 0

/* all used bits of 'ETHMAC_RX_TIMESTAMP_NS': */
#define MSK_USED_BITS_ETHMAC_RX_TIMESTAMP_NS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_TIMESTAMP_S */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_TIMESTAMP_S        0x00000010U

#define MSK_ETHMAC_RX_TIMESTAMP_S_VAL 0xffffffffU
#define SRT_ETHMAC_RX_TIMESTAMP_S_VAL 0

/* all used bits of 'ETHMAC_RX_TIMESTAMP_S': */
#define MSK_USED_BITS_ETHMAC_RX_TIMESTAMP_S 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_CONFIG_AREA_BASE */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_CONFIG_AREA_BASE        0x00000014U

/* --------------------------------------------------------------------- */
/* Register ETHMAC_MONITORING_MODE */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_MONITORING_MODE        0x00000014U

#define MSK_ETHMAC_MONITORING_MODE_VAL 0xffffffffU
#define SRT_ETHMAC_MONITORING_MODE_VAL 0

/* all used bits of 'ETHMAC_MONITORING_MODE': */
#define MSK_USED_BITS_ETHMAC_MONITORING_MODE 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_TRAFFIC_CLASS_ARRANGEMENT */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT        0x00000018U

#define MSK_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT_VAL  0x0000000fU
#define SRT_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT_VAL  0
#define MSK_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT_RES1 0xfffffff0U
#define SRT_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT_RES1 4

/* all used bits of 'ETHMAC_TRAFFIC_CLASS_ARRANGEMENT': */
#define MSK_USED_BITS_ETHMAC_TRAFFIC_CLASS_ARRANGEMENT 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_IND_HI */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_IND_HI        0x0000001CU

#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_HI_VAL  0x00000001U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_HI_VAL  0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_HI_RES1 0xfffffffeU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_HI_RES1 1

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_IND_HI': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_IND_HI 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_IND_LO */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_IND_LO        0x00000020U

#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_LO_RES1 0x00000001U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_LO_RES1 0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_LO_VAL  0x00000002U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_LO_VAL  1
#define MSK_ETHMAC_INTERRUPTS_ENABLE_IND_LO_RES2 0xfffffffcU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_IND_LO_RES2 2

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_IND_LO': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_IND_LO 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_CON_HI */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_CON_HI        0x00000024U

#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_HI_RES1 0x00000003U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_HI_RES1 0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_HI_VAL  0x00000004U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_HI_VAL  2
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_HI_RES2 0xfffffff8U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_HI_RES2 3

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_CON_HI': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_CON_HI 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_CON_LO */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_CON_LO        0x00000028U

#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_LO_RES1 0x00000007U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_LO_RES1 0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_LO_VAL  0x00000008U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_LO_VAL  3
#define MSK_ETHMAC_INTERRUPTS_ENABLE_CON_LO_RES2 0xfffffff0U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_CON_LO_RES2 4

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_CON_LO': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_CON_LO 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED        0x0000002CU

#define MSK_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_RES1 0x0000000fU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_RES1 0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_VAL  0x00000010U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_VAL  4
#define MSK_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_RES2 0xffffffe0U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_RES2 5

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_COL */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_COL        0x00000030U

#define MSK_ETHMAC_INTERRUPTS_ENABLE_COL_RES1 0x0000001fU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_COL_RES1 0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_COL_VAL  0x00000020U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_COL_VAL  5
#define MSK_ETHMAC_INTERRUPTS_ENABLE_COL_RES2 0xffffffc0U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_COL_RES2 6

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_COL': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_COL 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV        0x00000034U

#define MSK_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_RES1 0x0000003fU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_RES1 0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_VAL  0x00000040U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_VAL  6
#define MSK_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_RES2 0xffffff80U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_RES2 7

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_RX_ERR */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_RX_ERR        0x00000038U

#define MSK_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_RES1 0x0000007fU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_RES1 0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_VAL  0x00000080U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_VAL  7
#define MSK_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_RES2 0xffffff00U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_RES2 8

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_RX_ERR': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_RX_ERR 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERRUPTS_ENABLE_TX_ERR */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERRUPTS_ENABLE_TX_ERR        0x0000003CU

#define MSK_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_RES1 0x000000ffU
#define SRT_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_RES1 0
#define MSK_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_VAL  0x00000100U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_VAL  8
#define MSK_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_RES2 0xfffffe00U
#define SRT_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_RES2 9

/* all used bits of 'ETHMAC_INTERRUPTS_ENABLE_TX_ERR': */
#define MSK_USED_BITS_ETHMAC_INTERRUPTS_ENABLE_TX_ERR 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERFACE_MAC_ADDRESS_LO */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERFACE_MAC_ADDRESS_LO        0x00000040U

#define MSK_ETHMAC_INTERFACE_MAC_ADDRESS_LO_BYTES_4321 0xffffffffU
#define SRT_ETHMAC_INTERFACE_MAC_ADDRESS_LO_BYTES_4321 0

/* all used bits of 'ETHMAC_INTERFACE_MAC_ADDRESS_LO': */
#define MSK_USED_BITS_ETHMAC_INTERFACE_MAC_ADDRESS_LO 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_INTERFACE_MAC_ADDRESS_HI */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_INTERFACE_MAC_ADDRESS_HI        0x00000044U

#define MSK_ETHMAC_INTERFACE_MAC_ADDRESS_HI_BYTES_65 0x0000ffffU
#define SRT_ETHMAC_INTERFACE_MAC_ADDRESS_HI_BYTES_65 0
#define MSK_ETHMAC_INTERFACE_MAC_ADDRESS_HI_RES1     0xffff0000U
#define SRT_ETHMAC_INTERFACE_MAC_ADDRESS_HI_RES1     16

/* all used bits of 'ETHMAC_INTERFACE_MAC_ADDRESS_HI': */
#define MSK_USED_BITS_ETHMAC_INTERFACE_MAC_ADDRESS_HI 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO        0x00000048U

#define MSK_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO_BYTES_4321 0xffffffffU
#define SRT_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO_BYTES_4321 0

/* all used bits of 'ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO': */
#define MSK_USED_BITS_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI        0x0000004CU

#define MSK_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI_BYTES_65 0x0000ffffU
#define SRT_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI_BYTES_65 0
#define MSK_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI_RES1     0xffff0000U
#define SRT_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI_RES1     16

/* all used bits of 'ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI': */
#define MSK_USED_BITS_ETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_SYSTIME_BORDER_COPY_PLUS1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SYSTIME_BORDER_COPY_PLUS1        0x00000050U

#define MSK_ETHMAC_SYSTIME_BORDER_COPY_PLUS1_VAL 0xffffffffU
#define SRT_ETHMAC_SYSTIME_BORDER_COPY_PLUS1_VAL 0

/* all used bits of 'ETHMAC_SYSTIME_BORDER_COPY_PLUS1': */
#define MSK_USED_BITS_ETHMAC_SYSTIME_BORDER_COPY_PLUS1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_TS_COR_BASE_TX */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TS_COR_BASE_TX        0x00000054U

#define MSK_ETHMAC_TS_COR_BASE_TX_VAL 0xffffffffU
#define SRT_ETHMAC_TS_COR_BASE_TX_VAL 0

/* all used bits of 'ETHMAC_TS_COR_BASE_TX': */
#define MSK_USED_BITS_ETHMAC_TS_COR_BASE_TX 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_TS_COR_BASE_RX */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TS_COR_BASE_RX        0x00000058U

#define MSK_ETHMAC_TS_COR_BASE_RX_VAL 0xffffffffU
#define SRT_ETHMAC_TS_COR_BASE_RX_VAL 0

/* all used bits of 'ETHMAC_TS_COR_BASE_RX': */
#define MSK_USED_BITS_ETHMAC_TS_COR_BASE_RX 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_STATUS_AREA_BASE */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_STATUS_AREA_BASE        0x0000005CU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_OUT_OCTETS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_OUT_OCTETS        0x0000005CU

#define MSK_ETHMAC_OUT_OCTETS_CNT 0xffffffffU
#define SRT_ETHMAC_OUT_OCTETS_CNT 0

/* all used bits of 'ETHMAC_OUT_OCTETS': */
#define MSK_USED_BITS_ETHMAC_OUT_OCTETS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_SINGLE_COLLISION_FRAMES */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SINGLE_COLLISION_FRAMES        0x00000060U

#define MSK_ETHMAC_SINGLE_COLLISION_FRAMES_CNT 0xffffffffU
#define SRT_ETHMAC_SINGLE_COLLISION_FRAMES_CNT 0

/* all used bits of 'ETHMAC_SINGLE_COLLISION_FRAMES': */
#define MSK_USED_BITS_ETHMAC_SINGLE_COLLISION_FRAMES 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_MULTIPLE_COLLISION_FRAMES */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_MULTIPLE_COLLISION_FRAMES        0x00000064U

#define MSK_ETHMAC_MULTIPLE_COLLISION_FRAMES_CNT 0xffffffffU
#define SRT_ETHMAC_MULTIPLE_COLLISION_FRAMES_CNT 0

/* all used bits of 'ETHMAC_MULTIPLE_COLLISION_FRAMES': */
#define MSK_USED_BITS_ETHMAC_MULTIPLE_COLLISION_FRAMES 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_LATE_COLLISIONS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_LATE_COLLISIONS        0x00000068U

#define MSK_ETHMAC_LATE_COLLISIONS_CNT 0xffffffffU
#define SRT_ETHMAC_LATE_COLLISIONS_CNT 0

/* all used bits of 'ETHMAC_LATE_COLLISIONS': */
#define MSK_USED_BITS_ETHMAC_LATE_COLLISIONS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_LINK_DOWN_DURING_TRANSMISSION */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_LINK_DOWN_DURING_TRANSMISSION        0x0000006CU

#define MSK_ETHMAC_LINK_DOWN_DURING_TRANSMISSION_CNT 0xffffffffU
#define SRT_ETHMAC_LINK_DOWN_DURING_TRANSMISSION_CNT 0

/* all used bits of 'ETHMAC_LINK_DOWN_DURING_TRANSMISSION': */
#define MSK_USED_BITS_ETHMAC_LINK_DOWN_DURING_TRANSMISSION 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION        0x00000070U

#define MSK_ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION_CNT 0xffffffffU
#define SRT_ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION_CNT 0

/* all used bits of 'ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION': */
#define MSK_USED_BITS_ETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_IN_OCTETS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_IN_OCTETS        0x00000074U

#define MSK_ETHMAC_IN_OCTETS_CNT 0xffffffffU
#define SRT_ETHMAC_IN_OCTETS_CNT 0

/* all used bits of 'ETHMAC_IN_OCTETS': */
#define MSK_USED_BITS_ETHMAC_IN_OCTETS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS        0x00000078U

#define MSK_ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS_CNT 0xffffffffU
#define SRT_ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS_CNT 0

/* all used bits of 'ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS': */
#define MSK_USED_BITS_ETHMAC_FRAME_CHECK_SEQUENCE_ERRORS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_ALIGNMENT_ERRORS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_ALIGNMENT_ERRORS        0x0000007CU

#define MSK_ETHMAC_ALIGNMENT_ERRORS_CNT 0xffffffffU
#define SRT_ETHMAC_ALIGNMENT_ERRORS_CNT 0

/* all used bits of 'ETHMAC_ALIGNMENT_ERRORS': */
#define MSK_USED_BITS_ETHMAC_ALIGNMENT_ERRORS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_FRAME_TOO_LONG_ERRORS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FRAME_TOO_LONG_ERRORS        0x00000080U

#define MSK_ETHMAC_FRAME_TOO_LONG_ERRORS_CNT 0xffffffffU
#define SRT_ETHMAC_FRAME_TOO_LONG_ERRORS_CNT 0

/* all used bits of 'ETHMAC_FRAME_TOO_LONG_ERRORS': */
#define MSK_USED_BITS_ETHMAC_FRAME_TOO_LONG_ERRORS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_RUNT_FRAMES_RECEIVED */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RUNT_FRAMES_RECEIVED        0x00000084U

#define MSK_ETHMAC_RUNT_FRAMES_RECEIVED_CNT 0xffffffffU
#define SRT_ETHMAC_RUNT_FRAMES_RECEIVED_CNT 0

/* all used bits of 'ETHMAC_RUNT_FRAMES_RECEIVED': */
#define MSK_USED_BITS_ETHMAC_RUNT_FRAMES_RECEIVED 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_COLLISION_FRAGMENTS_RECEIVED */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_COLLISION_FRAGMENTS_RECEIVED        0x00000088U

#define MSK_ETHMAC_COLLISION_FRAGMENTS_RECEIVED_CNT 0xffffffffU
#define SRT_ETHMAC_COLLISION_FRAGMENTS_RECEIVED_CNT 0

/* all used bits of 'ETHMAC_COLLISION_FRAGMENTS_RECEIVED': */
#define MSK_USED_BITS_ETHMAC_COLLISION_FRAGMENTS_RECEIVED 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE        0x0000008CU

#define MSK_ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE_CNT 0xffffffffU
#define SRT_ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE_CNT 0

/* all used bits of 'ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE': */
#define MSK_USED_BITS_ETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW        0x00000090U

#define MSK_ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW_CNT 0xffffffffU
#define SRT_ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW_CNT 0

/* all used bits of 'ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW': */
#define MSK_USED_BITS_ETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_TX_FATAL_ERROR */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_TX_FATAL_ERROR        0x00000094U

#define MSK_ETHMAC_TX_FATAL_ERROR_CNT 0xffffffffU
#define SRT_ETHMAC_TX_FATAL_ERROR_CNT 0

/* all used bits of 'ETHMAC_TX_FATAL_ERROR': */
#define MSK_USED_BITS_ETHMAC_TX_FATAL_ERROR 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_RX_FATAL_ERROR */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_RX_FATAL_ERROR        0x00000098U

#define MSK_ETHMAC_RX_FATAL_ERROR_CNT 0xffffffffU
#define SRT_ETHMAC_RX_FATAL_ERROR_CNT 0

/* all used bits of 'ETHMAC_RX_FATAL_ERROR': */
#define MSK_USED_BITS_ETHMAC_RX_FATAL_ERROR 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_TXBUF */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_TXBUF        0x0000009CU

/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_RXBUF */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_RXBUF        0x0000069CU

/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_MULTICAST_FILTER_TABLE */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_MULTICAST_FILTER_TABLE        0x00000C9CU

/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_TS_COR */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_TS_COR        0x00000E9CU

/* --------------------------------------------------------------------- */
/* Register AREA_ETHMAC_RESERVED */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_ETHMAC_RESERVED        0x00000EA4U

/* --------------------------------------------------------------------- */
/* Register ETHMAC_XPEC2ARM_INTERRUPTS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_XPEC2ARM_INTERRUPTS        0x00000EA4U

#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_IND_HI       0x00000001U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_IND_HI       0
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_IND_LO       0x00000002U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_IND_LO       1
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_CON_HI       0x00000004U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_CON_HI       2
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_CON_LO       0x00000008U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_CON_LO       3
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_LINK_CHANGED 0x00000010U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_LINK_CHANGED 4
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_COL          0x00000020U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_COL          5
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_EARLY_RCV    0x00000040U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_EARLY_RCV    6
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_RX_ERR       0x00000080U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_RX_ERR       7
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_TX_ERR       0x00000100U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_TX_ERR       8
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_RES1         0x0000fe00U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_RES1         9
#define MSK_ETHMAC_XPEC2ARM_INTERRUPTS_UNUSABLE     0xffff0000U
#define SRT_ETHMAC_XPEC2ARM_INTERRUPTS_UNUSABLE     16

/* all used bits of 'ETHMAC_XPEC2ARM_INTERRUPTS': */
#define MSK_USED_BITS_ETHMAC_XPEC2ARM_INTERRUPTS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_ARM2XPEC_INTERRUPTS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_ARM2XPEC_INTERRUPTS        0x00000EA8U

#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_UNUSABLE             0x0000ffffU
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_UNUSABLE             0
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ 0x00010000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ 16
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_VALID                0x00020000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_VALID                17
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_SPEED100             0x00040000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_SPEED100             18
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_FDX                  0x00080000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_FDX                  19
#define MSK_ETHMAC_ARM2XPEC_INTERRUPTS_RES1                 0xfff00000U
#define SRT_ETHMAC_ARM2XPEC_INTERRUPTS_RES1                 20

/* all used bits of 'ETHMAC_ARM2XPEC_INTERRUPTS': */
#define MSK_USED_BITS_ETHMAC_ARM2XPEC_INTERRUPTS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_FIFO_ELEMENT */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_FIFO_ELEMENT        0x00000EACU

#define MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN           0x000007ffU
#define SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN           0
#define MSK_ETHMAC_FIFO_ELEMENT_RES1                0x00000800U
#define SRT_ETHMAC_FIFO_ELEMENT_RES1                11
#define MSK_ETHMAC_FIFO_ELEMENT_MULTI_REQ           0x00003000U
#define SRT_ETHMAC_FIFO_ELEMENT_MULTI_REQ           12
#define MSK_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON        0x00004000U
#define SRT_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON        14
#define MSK_ETHMAC_FIFO_ELEMENT_RES2                0x00008000U
#define SRT_ETHMAC_FIFO_ELEMENT_RES2                15
#define MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM       0x003f0000U
#define SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM       16
#define MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM 0x03c00000U
#define SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM 22
#define MSK_ETHMAC_FIFO_ELEMENT_RES3                0x0c000000U
#define SRT_ETHMAC_FIFO_ELEMENT_RES3                26
#define MSK_ETHMAC_FIFO_ELEMENT_ERROR_CODE          0xf0000000U
#define SRT_ETHMAC_FIFO_ELEMENT_ERROR_CODE          28

/* all used bits of 'ETHMAC_FIFO_ELEMENT': */
#define MSK_USED_BITS_ETHMAC_FIFO_ELEMENT 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_HELP */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_HELP        0x00000EB0U

#define MSK_ETHMAC_HELP_RX_IND_PRIO   0x00000001U
#define SRT_ETHMAC_HELP_RX_IND_PRIO   0
#define MSK_ETHMAC_HELP_RX_DROP       0x00000002U
#define SRT_ETHMAC_HELP_RX_DROP       1
#define MSK_ETHMAC_HELP_RX_FRWD2LOCAL 0x00000004U
#define SRT_ETHMAC_HELP_RX_FRWD2LOCAL 2
#define MSK_ETHMAC_HELP_RES1          0x000000f8U
#define SRT_ETHMAC_HELP_RES1          3
#define MSK_ETHMAC_HELP_TX_REQ_PRIO   0x00000100U
#define SRT_ETHMAC_HELP_TX_REQ_PRIO   8
#define MSK_ETHMAC_HELP_RES2          0xfffffe00U
#define SRT_ETHMAC_HELP_RES2          9

/* all used bits of 'ETHMAC_HELP': */
#define MSK_USED_BITS_ETHMAC_HELP 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_SR_LINK_MODE */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SR_LINK_MODE        0x00000EB4U

#define MSK_ETHMAC_SR_LINK_MODE_VALID    0x00000001U
#define SRT_ETHMAC_SR_LINK_MODE_VALID    0
#define MSK_ETHMAC_SR_LINK_MODE_SPEED100 0x00000002U
#define SRT_ETHMAC_SR_LINK_MODE_SPEED100 1
#define MSK_ETHMAC_SR_LINK_MODE_FDX      0x00000004U
#define SRT_ETHMAC_SR_LINK_MODE_FDX      2
#define MSK_ETHMAC_SR_LINK_MODE_RES2     0x0000fff8U
#define SRT_ETHMAC_SR_LINK_MODE_RES2     3
#define MSK_ETHMAC_SR_LINK_MODE_UNUSABLE 0xffff0000U
#define SRT_ETHMAC_SR_LINK_MODE_UNUSABLE 16

/* all used bits of 'ETHMAC_SR_LINK_MODE': */
#define MSK_USED_BITS_ETHMAC_SR_LINK_MODE 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_SR_CONFIG */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SR_CONFIG        0x00000EB8U

#define MSK_ETHMAC_SR_CONFIG_ERROR_CODE      0x00000007U
#define SRT_ETHMAC_SR_CONFIG_ERROR_CODE      0
#define MSK_ETHMAC_SR_CONFIG_RES1            0x00003ff8U
#define SRT_ETHMAC_SR_CONFIG_RES1            3
#define MSK_ETHMAC_SR_CONFIG_TIMESTAMP_VALID 0x00004000U
#define SRT_ETHMAC_SR_CONFIG_TIMESTAMP_VALID 14
#define MSK_ETHMAC_SR_CONFIG_START           0x00008000U
#define SRT_ETHMAC_SR_CONFIG_START           15
#define MSK_ETHMAC_SR_CONFIG_UNUSABLE        0xffff0000U
#define SRT_ETHMAC_SR_CONFIG_UNUSABLE        16

/* all used bits of 'ETHMAC_SR_CONFIG': */
#define MSK_USED_BITS_ETHMAC_SR_CONFIG 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ETHMAC_SR_STATUS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETHMAC_SR_STATUS        0x00000EBCU

#define MSK_ETHMAC_SR_STATUS_RCVD_LEN   0x000007ffU
#define SRT_ETHMAC_SR_STATUS_RCVD_LEN   0
#define MSK_ETHMAC_SR_STATUS_RES        0x00000800U
#define SRT_ETHMAC_SR_STATUS_RES        11
#define MSK_ETHMAC_SR_STATUS_ERROR_CODE 0x00007000U
#define SRT_ETHMAC_SR_STATUS_ERROR_CODE 12
#define MSK_ETHMAC_SR_STATUS_FIN        0x00008000U
#define SRT_ETHMAC_SR_STATUS_FIN        15
#define MSK_ETHMAC_SR_STATUS_UNUSABLE   0xffff0000U
#define SRT_ETHMAC_SR_STATUS_UNUSABLE   16

/* all used bits of 'ETHMAC_SR_STATUS': */
#define MSK_USED_BITS_ETHMAC_SR_STATUS 0xffffffffU




/*********************************************************************/
/* area structure definitions                                        */
/*********************************************************************/


typedef struct ETHMACBASE_AREA_Ttag
{
  volatile uint32_t  ulETHMAC_TX_RETRY;
  volatile uint32_t  ulETHMAC_TX_TIMESTAMP_NS;
  volatile uint32_t  ulETHMAC_TX_TIMESTAMP_S;
  volatile uint32_t  ulETHMAC_RX_TIMESTAMP_NS;
  volatile uint32_t  ulETHMAC_RX_TIMESTAMP_S;
  volatile uint32_t  ulETHMAC_MONITORING_MODE;
  volatile uint32_t  ulETHMAC_TRAFFIC_CLASS_ARRANGEMENT;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_IND_HI;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_IND_LO;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_CON_HI;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_CON_LO;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_COL;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_EARLY_RCV;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_RX_ERR;
  volatile uint32_t  ulETHMAC_INTERRUPTS_ENABLE_TX_ERR;
  volatile uint32_t  ulETHMAC_INTERFACE_MAC_ADDRESS_LO;
  volatile uint32_t  ulETHMAC_INTERFACE_MAC_ADDRESS_HI;
  volatile uint32_t  ulETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO;
  volatile uint32_t  ulETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI;
  volatile uint32_t  ulETHMAC_SYSTIME_BORDER_COPY_PLUS1;
  volatile uint32_t  ulETHMAC_TS_COR_BASE_TX;
  volatile uint32_t  ulETHMAC_TS_COR_BASE_RX;
  volatile uint32_t  ulETHMAC_OUT_OCTETS;
  volatile uint32_t  ulETHMAC_SINGLE_COLLISION_FRAMES;
  volatile uint32_t  ulETHMAC_MULTIPLE_COLLISION_FRAMES;
  volatile uint32_t  ulETHMAC_LATE_COLLISIONS;
  volatile uint32_t  ulETHMAC_LINK_DOWN_DURING_TRANSMISSION;
  volatile uint32_t  ulETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION;
  volatile uint32_t  ulETHMAC_IN_OCTETS;
  volatile uint32_t  ulETHMAC_FRAME_CHECK_SEQUENCE_ERRORS;
  volatile uint32_t  ulETHMAC_ALIGNMENT_ERRORS;
  volatile uint32_t  ulETHMAC_FRAME_TOO_LONG_ERRORS;
  volatile uint32_t  ulETHMAC_RUNT_FRAMES_RECEIVED;
  volatile uint32_t  ulETHMAC_COLLISION_FRAGMENTS_RECEIVED;
  volatile uint32_t  ulETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE;
  volatile uint32_t  ulETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW;
  volatile uint32_t  ulETHMAC_TX_FATAL_ERROR;
  volatile uint32_t  ulETHMAC_RX_FATAL_ERROR;
  volatile uint32_t  ulAREA_ETHMAC_TXBUF;
  volatile uint32_t aulReserved0[383];
  volatile uint32_t  ulAREA_ETHMAC_RXBUF;
  volatile uint32_t aulReserved1[383];
  volatile uint32_t  ulAREA_ETHMAC_MULTICAST_FILTER_TABLE;
  volatile uint32_t aulReserved2[127];
  volatile uint32_t  ulAREA_ETHMAC_TS_COR;
  volatile uint32_t aulReserved3[1];
  volatile uint32_t  ulETHMAC_XPEC2ARM_INTERRUPTS;
  volatile uint32_t  ulETHMAC_ARM2XPEC_INTERRUPTS;
  volatile uint32_t  ulETHMAC_FIFO_ELEMENT;
  volatile uint32_t  ulETHMAC_HELP;
  volatile uint32_t  ulETHMAC_SR_LINK_MODE;
  volatile uint32_t  ulETHMAC_SR_CONFIG;
  volatile uint32_t  ulETHMAC_SR_STATUS;
} ETHMACBASE_AREA_T;


#endif

#ifndef __ethmac_struct_h
#define __ethmac_struct_h

typedef struct ETHMAC_TXAREABASE_Ttag
{
  volatile uint32_t ulETHMAC_TX_RETRY;
  volatile uint32_t ulETHMAC_TX_TIMESTAMP_NS;
  volatile uint32_t ulETHMAC_TX_TIMESTAMP_S;
} ETHMAC_TXAREABASE_T;

typedef struct ETHMAC_RXAREABASE_Ttag
{
  volatile uint32_t ulETHMAC_RX_TIMESTAMP_NS;
  volatile uint32_t ulETHMAC_RX_TIMESTAMP_S;
} ETHMAC_RXAREABASE_T;

typedef struct ETHMAC_CONFIG_AREA_BASE_Ttag
{
  volatile uint32_t ulETHMAC_MONITORING_MODE;
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
  volatile uint32_t ulETHMAC_SYSTIME_BORDER_COPY_PLUS1;
  volatile uint32_t ulETHMAC_TS_COR_BASE_TX;
  volatile uint32_t ulETHMAC_TS_COR_BASE_RX;
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

typedef struct ETHMAC_TXBUF_Ttag
{
  volatile uint32_t aulDataField[384];
} ETHMAC_TXBUF_T;

typedef struct ETHMAC_RXBUF_Ttag
{
  volatile uint32_t aulDataField[384];
} ETHMAC_RXBUF_T;

typedef struct ETHMAC_MULTICAST_FILTER_TABLE_Ttag
{
  volatile uint32_t aulDataField[128];
} ETHMAC_MULTICAST_FILTER_TABLE_T;

typedef struct ETHMAC_TS_COR_Ttag
{
  volatile uint32_t aulDataField[2];
} ETHMAC_TS_COR_T;

typedef struct ETHMAC_RESERVED_Ttag
{
  volatile uint32_t ulETHMAC_XPEC2ARM_INTERRUPTS;
  volatile uint32_t ulETHMAC_ARM2XPEC_INTERRUPTS;
  volatile uint32_t ulETHMAC_FIFO_ELEMENT;
  volatile uint32_t ulETHMAC_HELP;
  volatile uint32_t ulETHMAC_SR_LINK_MODE;
  volatile uint32_t ulETHMAC_SR_CONFIG;
  volatile uint32_t ulETHMAC_SR_STATUS;
} ETHMAC_RESERVED_T;

/* combined 9 structures */
typedef struct ETHMAC_XPEC_DPMtag {
  ETHMAC_TXAREABASE_T tETHMAC_TXAREABASE;
  ETHMAC_RXAREABASE_T tETHMAC_RXAREABASE;
  ETHMAC_CONFIG_AREA_BASE_T tETHMAC_CONFIG_AREA_BASE;
  ETHMAC_STATUS_AREA_BASE_T tETHMAC_STATUS_AREA_BASE;
  ETHMAC_TXBUF_T tETHMAC_TXBUF;
  ETHMAC_RXBUF_T tETHMAC_RXBUF;
  ETHMAC_MULTICAST_FILTER_TABLE_T tETHMAC_MULTICAST_FILTER_TABLE;
  ETHMAC_TS_COR_T tETHMAC_TS_COR;
} ETHMAC_XPEC_DPM;

#endif /* __ethmac_struct_h */

