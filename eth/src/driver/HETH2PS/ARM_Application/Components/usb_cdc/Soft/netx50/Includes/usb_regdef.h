#ifndef __USB_REGDEF_H
#define __USB_REGDEF_H

/* ===================================================================== */
/* Area of usb_core */
/* ===================================================================== */

#define NETX_USB_CORE_AREA	0x1C020000U

/* --------------------------------------------------------------------- */
/* Register ID */
/* =>  ID Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_ID 0x00000000U
#define NETX_USB_ID     0x1C020000U

/* --------------------------------------------------------------------- */
/* Register USB_CTRL */
/* =>  USB Control Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_CTRL 0x00000004U
#define NETX_USB_CTRL     0x1C020004U

/* --------------------------------------------------------------------- */
/* Register FRM_TIMER */
/* =>  Frame Timer Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_FRM_TMR 0x00000008U
#define NETX_USB_FRM_TMR     0x1C020008U

/* --------------------------------------------------------------------- */
/* Register MAIN_EV */
/* =>  Main Event Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_MAIN_EV 0x0000000CU
#define NETX_USB_MAIN_EV     0x1C02000CU

/* --------------------------------------------------------------------- */
/* Register MAIN_EM */
/* =>  Main Event Mask Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_MAIN_EV_MSK 0x00000010U
#define NETX_USB_MAIN_EV_MSK     0x1C020010U

/* --------------------------------------------------------------------- */
/* Register PIPE_EV */
/* =>  Pipe Event Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_EV 0x00000014U
#define NETX_USB_PIPE_EV     0x1C020014U

/* --------------------------------------------------------------------- */
/* Register PIPE_EM */
/* =>  Pipe Event Mask Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_EV_MSK 0x00000018U
#define NETX_USB_PIPE_EV_MSK     0x1C020018U

/* --------------------------------------------------------------------- */
/* Register PIPE_SEL */
/* =>  Pipe Select Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_SEL 0x00000024U
#define NETX_USB_PIPE_SEL     0x1C020024U

/* --------------------------------------------------------------------- */
/* Register PORT_STAT */
/* =>  Port Statuus Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PORT_STAT 0x0000002CU
#define NETX_USB_PORT_STAT     0x1C02002CU

/* --------------------------------------------------------------------- */
/* Register PORT_CTRL */
/* =>  Port Control Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PORT_CTRL 0x00000030U
#define NETX_USB_PORT_CTRL     0x1C020030U

/* --------------------------------------------------------------------- */
/* Register PSC_EV */
/* =>  Port Status Change Event Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PORT_STAT_CHG_EV 0x00000034U
#define NETX_USB_PORT_STAT_CHG_EV     0x1C020034U

/* --------------------------------------------------------------------- */
/* Register PSC_EM */
/* =>  Port Status Change Event Mask Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PORT_STAT_CHG_EV_MSK 0x00000038U
#define NETX_USB_PORT_STAT_CHG_EV_MSK     0x1C020038U

/* --------------------------------------------------------------------- */
/* Register PIPE_CTRL */
/* =>  Pipe Control Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_CTRL 0x00000040U
#define NETX_USB_PIPE_CTRL     0x1C020040U

/* --------------------------------------------------------------------- */
/* Register PIPE_CFG */
/* =>  Pipe Configuration Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_CFG 0x00000044U
#define NETX_USB_PIPE_CFG     0x1C020044U

/* --------------------------------------------------------------------- */
/* Register PIPE_ADDR */
/* =>  Pipe Address Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_ADDR 0x00000048U
#define NETX_USB_PIPE_ADDR     0x1C020048U

/* --------------------------------------------------------------------- */
/* Register PIPE_STAT */
/* =>  Pipe Status Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_STAT 0x0000004CU
#define NETX_USB_PIPE_STAT     0x1C02004CU

/* --------------------------------------------------------------------- */
/* Register PIPE_DATA_PTR */
/* =>  Pipe Data Pointer Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_DATA_PTR 0x00000050U
#define NETX_USB_PIPE_DATA_PTR     0x1C020050U

/* --------------------------------------------------------------------- */
/* Register PIPE_DATA_TBYTES */
/* =>  Pipe Total Bytes Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_DATA_TOT 0x00000054U
#define NETX_USB_PIPE_DATA_TOT     0x1C020054U

/* --------------------------------------------------------------------- */
/* Register PIPE_ADATA_PTR */
/* =>  Pipe Alternative Data Pointer Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_ALT_DATA_PTR 0x00000058U
#define NETX_USB_PIPE_ALT_DATA_PTR     0x1C020058U

/* --------------------------------------------------------------------- */
/* Register PIPE_ADATA_TBYTES */
/* =>  Pipe Altenative Data Total Bytes Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_PIPE_ALT_DATA_TOT 0x0000005CU
#define NETX_USB_PIPE_ALT_DATA_TOT     0x1C02005CU

/* --------------------------------------------------------------------- */
/* Register DEBUG_CTRL */
/* =>  Debug Control Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_DBG_CTRL 0x00000060U
#define NETX_USB_DBG_CTRL     0x1C020060U

/* --------------------------------------------------------------------- */
/* Register DEBUG_PID */
/* =>  Debug PID Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_DBG_PID 0x00000064U
#define NETX_USB_DBG_PID     0x1C020064U

/* --------------------------------------------------------------------- */
/* Register DEBUG_STAT */
/* =>  Debug Status Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_DBG_STAT 0x00000068U
#define NETX_USB_DBG_STAT     0x1C020068U

/* --------------------------------------------------------------------- */
/* Register TEST */
/* =>  Test Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_TEST 0x0000006CU
#define NETX_USB_TEST     0x1C02006CU

/* --------------------------------------------------------------------- */
/* Register MAIN_CFG */
/* =>  Main Configuration Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_MAIN_CFG 0x00000080U
#define NETX_USB_MAIN_CFG     0x1C020080U

/* --------------------------------------------------------------------- */
/* Register MODE_CFG */
/* =>  Mode Configuration Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_MODE_CFG 0x00000084U
#define NETX_USB_MODE_CFG     0x1C020084U

/* --------------------------------------------------------------------- */
/* Register usb_core_ctrl */
/* =>  USB core control and status register */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_CORE_CTRL 0x00000088U
#define NETX_USB_CORE_CTRL     0x1C020088U

#define MSK_NETX_USB_CORE_CTRL_reset              0x00000001U /* [0] */
#define SRT_NETX_USB_CORE_CTRL_reset              0
#define MSK_NETX_USB_CORE_CTRL_dbg_support        0x00000002U /* [1] */
#define SRT_NETX_USB_CORE_CTRL_dbg_support        1
#define MSK_NETX_USB_CORE_CTRL_xtd_dbg_support    0x00000004U /* [2] */
#define SRT_NETX_USB_CORE_CTRL_xtd_dbg_support    2
#define MSK_NETX_USB_CORE_CTRL_soft_id_dig        0x00000008U /* [3] */
#define SRT_NETX_USB_CORE_CTRL_soft_id_dig        3
#define MSK_NETX_USB_CORE_CTRL_alt_buffer_support 0x00000ff0U /* [11:4] */
#define SRT_NETX_USB_CORE_CTRL_alt_buffer_support 4
#define MSK_NETX_USB_CORE_CTRL_core_suspend_n     0x00001000U /* [12] */
#define SRT_NETX_USB_CORE_CTRL_core_suspend_n     12
#define MSK_NETX_USB_CORE_CTRL_xcvr_suspend_n     0x00002000U /* [13] */
#define SRT_NETX_USB_CORE_CTRL_xcvr_suspend_n     13
#define MSK_NETX_USB_CORE_CTRL_over_current       0x00004000U /* [14] */
#define SRT_NETX_USB_CORE_CTRL_over_current       14
#define MSK_NETX_USB_CORE_CTRL_va_sess_vld        0x00008000U /* [15] */
#define SRT_NETX_USB_CORE_CTRL_va_sess_vld        15
#define MSK_NETX_USB_CORE_CTRL_vb_sess_end        0x00010000U /* [16] */
#define SRT_NETX_USB_CORE_CTRL_vb_sess_end        16
#define MSK_NETX_USB_CORE_CTRL_vb_sess_vld        0x00020000U /* [17] */
#define SRT_NETX_USB_CORE_CTRL_vb_sess_vld        17
#define MSK_NETX_USB_CORE_CTRL_vbus_vld           0x00040000U /* [18] */
#define SRT_NETX_USB_CORE_CTRL_vbus_vld           18
#define MSK_NETX_USB_CORE_CTRL_chrg_vbus          0x00080000U /* [19] */
#define SRT_NETX_USB_CORE_CTRL_chrg_vbus          19
#define MSK_NETX_USB_CORE_CTRL_dlp_active         0x00100000U /* [20] */
#define SRT_NETX_USB_CORE_CTRL_dlp_active         20
#define MSK_NETX_USB_CORE_CTRL_vb_on              0x00200000U /* [21] */
#define SRT_NETX_USB_CORE_CTRL_vb_on              21
#define MSK_NETX_USB_CORE_CTRL_dischrg_vbus       0x00400000U /* [22] */
#define SRT_NETX_USB_CORE_CTRL_dischrg_vbus       22
#define MSK_NETX_USB_CORE_CTRL_ucif_rdy           0x00800000U /* [23] */
#define SRT_NETX_USB_CORE_CTRL_ucif_rdy           23
#define MSK_NETX_USB_CORE_CTRL_usb_irq            0x01000000U /* [24] */
#define SRT_NETX_USB_CORE_CTRL_usb_irq            24

enum {
	BFW_NETX_USB_CORE_CTRL_reset              = 1, /* [0] */
	BFW_NETX_USB_CORE_CTRL_dbg_support        = 1, /* [1] */
	BFW_NETX_USB_CORE_CTRL_xtd_dbg_support    = 1, /* [2] */
	BFW_NETX_USB_CORE_CTRL_soft_id_dig        = 1, /* [3] */
	BFW_NETX_USB_CORE_CTRL_alt_buffer_support = 8, /* [11:4] */
	BFW_NETX_USB_CORE_CTRL_core_suspend_n     = 1, /* [12] */
	BFW_NETX_USB_CORE_CTRL_xcvr_suspend_n     = 1, /* [13] */
	BFW_NETX_USB_CORE_CTRL_over_current       = 1, /* [14] */
	BFW_NETX_USB_CORE_CTRL_va_sess_vld        = 1, /* [15] */
	BFW_NETX_USB_CORE_CTRL_vb_sess_end        = 1, /* [16] */
	BFW_NETX_USB_CORE_CTRL_vb_sess_vld        = 1, /* [17] */
	BFW_NETX_USB_CORE_CTRL_vbus_vld           = 1, /* [18] */
	BFW_NETX_USB_CORE_CTRL_chrg_vbus          = 1, /* [19] */
	BFW_NETX_USB_CORE_CTRL_dlp_active         = 1, /* [20] */
	BFW_NETX_USB_CORE_CTRL_vb_on              = 1, /* [21] */
	BFW_NETX_USB_CORE_CTRL_dischrg_vbus       = 1, /* [22] */
	BFW_NETX_USB_CORE_CTRL_ucif_rdy           = 1, /* [23] */
	BFW_NETX_USB_CORE_CTRL_usb_irq            = 1, /* [24] */
	BFW_NETX_USB_CORE_CTRL_reserved1          = 7  /* [31:25] */
};

typedef struct USB_CORE_CTRL_BIT_Ttag {
	unsigned int reset              : BFW_NETX_USB_CORE_CTRL_reset;              /* software reset (r/w) */
	unsigned int dbg_support        : BFW_NETX_USB_CORE_CTRL_dbg_support;        /* enables debug support (r/w) */
	unsigned int xtd_dbg_support    : BFW_NETX_USB_CORE_CTRL_xtd_dbg_support;    /* enables extended debug support (r/w) */
	unsigned int soft_id_dig        : BFW_NETX_USB_CORE_CTRL_soft_id_dig;        /* to set id_dig via software (r/w) */
	unsigned int alt_buffer_support : BFW_NETX_USB_CORE_CTRL_alt_buffer_support; /* alt buffer support (r/w) */
	unsigned int core_suspend_n     : BFW_NETX_USB_CORE_CTRL_core_suspend_n;     /* reflects core_suspend_n (ro) */
	unsigned int xcvr_suspend_n     : BFW_NETX_USB_CORE_CTRL_xcvr_suspend_n;     /* reflects xcvr_suspend_n (ro) */
	unsigned int over_current       : BFW_NETX_USB_CORE_CTRL_over_current;       /* reflects over_current (ro) */
	unsigned int va_sess_vld        : BFW_NETX_USB_CORE_CTRL_va_sess_vld;        /* reflects va_sess_vld (ro) */
	unsigned int vb_sess_end        : BFW_NETX_USB_CORE_CTRL_vb_sess_end;        /* reflects vb_sess_end (ro) */
	unsigned int vb_sess_vld        : BFW_NETX_USB_CORE_CTRL_vb_sess_vld;        /* reflects vb_sess_vld (ro) */
	unsigned int vbus_vld           : BFW_NETX_USB_CORE_CTRL_vbus_vld;           /* reflects vbus_vld (ro) */
	unsigned int chrg_vbus          : BFW_NETX_USB_CORE_CTRL_chrg_vbus;          /* reflects chrg_vbus (ro) */
	unsigned int dlp_active         : BFW_NETX_USB_CORE_CTRL_dlp_active;         /* reflects dlp_active (ro) */
	unsigned int vb_on              : BFW_NETX_USB_CORE_CTRL_vb_on;              /* reflects vb_on (ro) */
	unsigned int dischrg_vbus       : BFW_NETX_USB_CORE_CTRL_dischrg_vbus;       /* reflects dischrg_vbus (ro) */
	unsigned int ucif_rdy           : BFW_NETX_USB_CORE_CTRL_ucif_rdy;           /* reflects ucif_rdy (ro) */
	unsigned int usb_irq            : BFW_NETX_USB_CORE_CTRL_usb_irq;            /* reflects usb_irq (ro) */
	unsigned int reserved1          : BFW_NETX_USB_CORE_CTRL_reserved1;          /* reserved */
} USB_CORE_CTRL_BIT_T;

typedef union {
	unsigned int        val;
	USB_CORE_CTRL_BIT_T bf;
} USB_CORE_CTRL_T;


/* ===================================================================== */
/* Area of usb_fifo */
/* ===================================================================== */

#define NETX_USB_FIFO_AREA	0x1C030000U

/* --------------------------------------------------------------------- */
/* Register USB_FIFO_BASE */
/* =>  USB End Point 0 (IN) Base Address */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_NETX_USB_FIFO 0x00000000U
#define NETX_USB_FIFO     0x1C030000U

#endif /* __USB_REGDEF_H */
