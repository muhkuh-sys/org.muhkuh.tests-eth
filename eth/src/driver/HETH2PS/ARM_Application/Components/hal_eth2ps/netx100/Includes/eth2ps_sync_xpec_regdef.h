#ifndef eth2ps_sync_xpec_regdef_h__
#define eth2ps_sync_xpec_regdef_h__

#include <stdint.h>

/* ===================================================================== */
/* Area ETH2PS_SYNC_XPEC_PROGRAM */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_SYNC_XPEC_PROGRAM  0x00000000U

/* ===================================================================== */
/* Area ETH2PS_SYNC_SYNC_AREA_BASE */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_SYNC_SYNC_AREA_BASE  0x00000004U

/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_CONFIG_CYCLIC_OP */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_CONFIG_CYCLIC_OP  0x00000004U

/*  */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG0_POLARITY  0x00000001U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG0_POLARITY            0

/*  */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG0_OE  0x00000002U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG0_OE            1

/* res */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_RES1  0x00000004U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_RES1            2

/* 1 */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG0_ACT  0x00000008U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG0_ACT            3

/* reserved, shall be set to zero */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_RES2  0x000000f0U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_RES2            4

/*  */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG1_POLARITY  0x00000100U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG1_POLARITY            8

/*  */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG1_OE  0x00000200U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG1_OE            9

/* res */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_RES3  0x00000400U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_RES3           10

/* 1 */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG1_ACT  0x00000800U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG1_ACT           11

/* reserved, shall be set to zero */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_RES4  0x00007000U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_RES4           12

/* 1 */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_SYNC_PINS_CONTROLLED_BY_HOST  0x00008000U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_SYNC_PINS_CONTROLLED_BY_HOST           15

/* Pulse length of trigger signals in 10ns units */
#define MSK_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG_PULSE_LEN  0xffff0000U
#define SRT_ETH2PS_SYNC_CONFIG_CYCLIC_OP_TRG_PULSE_LEN           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_IRQEN_TRG0 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_IRQEN_TRG0  0x00000008U

/* reserved, shall be set to zero */
#define MSK_ETH2PS_SYNC_IRQEN_TRG0_RES1  0x00000fffU
#define SRT_ETH2PS_SYNC_IRQEN_TRG0_RES1            0

/* enable trigger 0 event */
#define MSK_ETH2PS_SYNC_IRQEN_TRG0_EN  0x00001000U
#define SRT_ETH2PS_SYNC_IRQEN_TRG0_EN           12

/* reserved, shall be set to zero */
#define MSK_ETH2PS_SYNC_IRQEN_TRG0_RES2  0xffffe000U
#define SRT_ETH2PS_SYNC_IRQEN_TRG0_RES2           13


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_IRQEN_TRG1 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_IRQEN_TRG1  0x0000000cU

/* reserved, shall be set to zero */
#define MSK_ETH2PS_SYNC_IRQEN_TRG1_RES1  0x00001fffU
#define SRT_ETH2PS_SYNC_IRQEN_TRG1_RES1            0

/* enable trigger 1 event */
#define MSK_ETH2PS_SYNC_IRQEN_TRG1_EN  0x00002000U
#define SRT_ETH2PS_SYNC_IRQEN_TRG1_EN           13

/* reserved, shall be set to zero */
#define MSK_ETH2PS_SYNC_IRQEN_TRG1_RES2  0xffffc000U
#define SRT_ETH2PS_SYNC_IRQEN_TRG1_RES2           14


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_T_TRG0 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_T_TRG0  0x00000010U

/* Time between STARTTIME_CYCLIC_OP and first trigger 0 pulse in ns */
#define MSK_ETH2PS_SYNC_T_TRG0_VAL_IN_NS  0xffffffffU
#define SRT_ETH2PS_SYNC_T_TRG0_VAL_IN_NS            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_DT_TRG0 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_DT_TRG0  0x00000014U

/* Time between two consecutive trigger 0 pulses in ns, Note */
#define MSK_ETH2PS_SYNC_DT_TRG0_VAL_IN_NS  0xffffffffU
#define SRT_ETH2PS_SYNC_DT_TRG0_VAL_IN_NS            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_T_TRG1 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_T_TRG1  0x00000018U

/* Time between STARTTIME_CYCLIC_OP and first trigger 1 event in ns */
#define MSK_ETH2PS_SYNC_T_TRG1_VAL_IN_NS  0xffffffffU
#define SRT_ETH2PS_SYNC_T_TRG1_VAL_IN_NS            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_DT_TRG1 */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_DT_TRG1  0x0000001cU

/* Time between two consecutive trigger 1 pulses in ns, Note */
#define MSK_ETH2PS_SYNC_DT_TRG1_VAL_IN_NS  0xffffffffU
#define SRT_ETH2PS_SYNC_DT_TRG1_VAL_IN_NS            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_STARTTIME_CYCLIC_OP */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_STARTTIME_CYCLIC_OP  0x00000020U

/* System time ns when cyclic operation shall start */
#define MSK_ETH2PS_SYNC_STARTTIME_CYCLIC_OP_VAL_IN_NS  0xffffffffU
#define SRT_ETH2PS_SYNC_STARTTIME_CYCLIC_OP_VAL_IN_NS            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_NEXT_TRG0_EVENT_TIME */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_NEXT_TRG0_EVENT_TIME  0x00000024U

/* System time of next trigger 0 event in ns */
#define MSK_ETH2PS_SYNC_NEXT_TRG0_EVENT_TIME_VAL_IN_NS  0xffffffffU
#define SRT_ETH2PS_SYNC_NEXT_TRG0_EVENT_TIME_VAL_IN_NS            0


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_NEXT_TRG1_EVENT_TIME */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_NEXT_TRG1_EVENT_TIME  0x00000028U

/* System time of next trigger 1 event in ns */
#define MSK_ETH2PS_SYNC_NEXT_TRG1_EVENT_TIME_VAL_IN_NS  0xffffffffU
#define SRT_ETH2PS_SYNC_NEXT_TRG1_EVENT_TIME_VAL_IN_NS            0


/* ===================================================================== */
/* Area ETH2PS_SYNC_RESERVED */
/* ===================================================================== */

#define REL_Adr_AREA_ETH2PS_SYNC_RESERVED  0x0000002cU

/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_XPEC2HOST_INTERRUPTS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS  0x0000002cU

/* xpec2host com interrupts */
#define MSK_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_RES1  0x00000fffU
#define SRT_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_RES1            0

/* 1 */
#define MSK_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_TRG0_EVENT  0x00001000U
#define SRT_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_TRG0_EVENT           12

/* 1 */
#define MSK_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_TRG1_EVENT  0x00002000U
#define SRT_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_TRG1_EVENT           13

/* reserved */
#define MSK_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_RES2  0x0000c000U
#define SRT_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_RES2           14

/* host2xpec interrupts */
#define MSK_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_UNUSABLE  0xffff0000U
#define SRT_ETH2PS_SYNC_XPEC2HOST_INTERRUPTS_UNUSABLE           16


/* --------------------------------------------------------------------- */
/* Register ETH2PS_SYNC_HOST2XPEC_INTERRUPTS */
/* --------------------------------------------------------------------- */

#define REL_Adr_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS  0x00000030U

/* xpec2host interrupts */
#define MSK_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS_UNUSABLE  0x0000ffffU
#define SRT_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS_UNUSABLE            0

/* 1 */
#define MSK_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS_START_CYCLIC_OP  0x00010000U
#define SRT_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS_START_CYCLIC_OP           16

/* 1 */
#define MSK_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS_STOP_CYCLIC_OP  0x00020000U
#define SRT_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS_STOP_CYCLIC_OP           17

/* reserved */
#define MSK_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS_RES1  0xfffc0000U
#define SRT_ETH2PS_SYNC_HOST2XPEC_INTERRUPTS_RES1           18


typedef struct ETH2PS_SYNC_XPEC_PROGRAM_Ttag
{
  volatile uint32_t aulDataField[1];
} ETH2PS_SYNC_XPEC_PROGRAM_T;

typedef struct ETH2PS_SYNC_SYNC_AREA_BASE_Ttag
{
  volatile uint32_t ulETH2PS_SYNC_CONFIG_CYCLIC_OP;
  volatile uint32_t ulETH2PS_SYNC_IRQEN_TRG0;
  volatile uint32_t ulETH2PS_SYNC_IRQEN_TRG1;
  volatile uint32_t ulETH2PS_SYNC_T_TRG0;
  volatile uint32_t ulETH2PS_SYNC_DT_TRG0;
  volatile uint32_t ulETH2PS_SYNC_T_TRG1;
  volatile uint32_t ulETH2PS_SYNC_DT_TRG1;
  volatile uint32_t ulETH2PS_SYNC_STARTTIME_CYCLIC_OP;
  volatile uint32_t ulETH2PS_SYNC_NEXT_TRG0_EVENT_TIME;
  volatile uint32_t ulETH2PS_SYNC_NEXT_TRG1_EVENT_TIME;
} ETH2PS_SYNC_SYNC_AREA_BASE_T;

typedef struct ETH2PS_SYNC_XPEC_DPM_Ttag {
  ETH2PS_SYNC_XPEC_PROGRAM_T tETH2PS_SYNC_XPEC_PROGRAM;
  ETH2PS_SYNC_SYNC_AREA_BASE_T tETH2PS_SYNC_SYNC_AREA_BASE;
} ETH2PS_SYNC_XPEC_DPM_T;

#endif /* eth2ps_sync_xpec_regdef_h__ */

