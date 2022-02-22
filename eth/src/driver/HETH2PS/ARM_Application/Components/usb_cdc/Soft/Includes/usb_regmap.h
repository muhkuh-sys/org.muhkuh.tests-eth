#ifndef __USB_REGMAP_H
#define __USB_REGMAP_H


#include "usb_types.h"
#include "usb_regdef.h"


#define PEEK(addr)          (*(volatile unsigned int *)(addr))
#define POKE(addr, val)     (*(volatile unsigned int *)(addr) = (val))

/***************** Register Addresses ***********************************************************/

#define    ID                 NETX_USB_ID                /* ID */
#define    USB_CTRL           NETX_USB_CTRL               /* USB Control */
#define    FRM_TIMER          NETX_USB_FRM_TMR           /* Frame Timer */

#define    MAIN_EV            NETX_USB_MAIN_EV            /* Main Event */
#define    MAIN_EM            NETX_USB_MAIN_EV_MSK            /* Main Event Mask */
#define    PIPE_EV            NETX_USB_PIPE_EV            /* Pipe Event */
#define    PIPE_EM            NETX_USB_PIPE_EV_MSK            /* Pipe Event Mask */

#define    PIPE_SEL           NETX_USB_PIPE_SEL          /* Pipe Select */

#define    PORT_STAT          NETX_USB_PORT_STAT          /* Port Status */
#define    PORT_CTRL          NETX_USB_PORT_CTRL          /* Port Control */
#define    PSC_EV             NETX_USB_PORT_STAT_CHG_EV            /* Port Status Change Event */
#define    PSC_EM             NETX_USB_PORT_STAT_CHG_EV_MSK            /* Port Status Change Event Mask */

#define    PIPE_CTRL          NETX_USB_PIPE_CTRL          /* Pipe Control */
#define    PIPE_CFG           NETX_USB_PIPE_CFG          /* Pipe Configuration */
#define    PIPE_ADDR          NETX_USB_PIPE_ADDR          /* Pipe Address */
#define    PIPE_STAT          NETX_USB_PIPE_STAT          /* Pipe Status */
#define    PIPE_DATA_PTR      NETX_USB_PIPE_DATA_PTR     /* Pipe Data Pointer */
#define    PIPE_DATA_TBYTES   NETX_USB_PIPE_DATA_TOT  /* Pipe Total Bytes */
#define    PIPE_ADATA_PTR     NETX_USB_PIPE_ALT_DATA_PTR    /* Pipe Alternative Data Pointer */
#define    PIPE_ADATA_TBYTES  NETX_USB_PIPE_ALT_DATA_TOT /* Pipe Alternative Data Total Bytes */

#define    DEBUG_CTRL         NETX_USB_DBG_CTRL        /* Debug Control */
#define    DEBUG_PID          NETX_USB_DBG_PID         /* Debug PID */
#define    DEBUG_STAT         NETX_USB_DBG_STAT        /* Debug Status */
#define    TEST               NETX_USB_TEST              /* Test */

#define    MAIN_CFG           NETX_USB_MAIN_CFG          /* Main Configuration */
#define    MODE_CFG           NETX_USB_MODE_CFG          /* Mode Configuration */


/***************** Bitfield Definitions *************************************************************/

/* bit definitions for ID */
#define    bmREV_ID       ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7) << 0)
#define    bmCORE_ID      ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4) << 8)

/* bit definitions for USB Control */
#define    bmCSUSP        bmBit0
#define    bmXSUSP        bmBit1
#define    bmHRS          bmBit2
#define    bmASE          bmBit3
#define    bmPSE          bmBit4

/* bit definitions for Frame Timer */
#define    bmFRAME_NR     ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7 | bmBit8 | bmBit9 | bmBit10) << 0)
#define    bmFTLOCK       bmBit11
#define    bmAGSOF        bmBit12

/* bit definitions for Main Event */
#define    bmFRM_EV       bmBit0
#define    bmFRM32_EV     bmBit1
#define    bmGPORT_EV     bmBit2
#define    bmGPIPE_EV     bmBit3
#define    bmHCHA_EV      bmBit4
#define    bmBWERR_EV     bmBit5

/* bit definitions for Main Event Mask */
#define    bmFRM_EM       bmBit0
#define    bmFRM32_EM     bmBit1
#define    bmGPORT_EM     bmBit2
#define    bmGPIPE_EM     bmBit3
#define    bmHCHA_EM      bmBit4
#define    bmBWERR_EM     bmBit5

/* bit definitions for Pipe Event */

/* bit definitions for Pipe Event Mask */

/* bit definitions for Pipe Select */
#define    bmPI_SEL         ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4) << 0)

/* bit definitions for Port Status */
#define    bmVBUS_VLD       bmBit0
#define    bmVA_SESS_VLD    bmBit1
#define    bmVB_SESS_VLD    bmBit2
#define    bmVB_SESS_END    bmBit3
#define    bmCONN_ID        bmBit4
#define    bmPCS            bmBit5
#define    bmDLS            bmBit6
#define    bmOCURC          bmBit7
#define    bmLINESTATE      ((bmBit0 | bmBit1) << 8)

/* bit definitions for Port Control */
#define    bmPTESTC        ((bmBit0 | bmBit1) << 0)
#define    PTESTC_DIS      ((0) << 0)
#define    PTESTC_JST      ((bmBit0) << 0)
#define    PTESTC_KST      ((bmBit1) << 0)
#define    PTESTC_SE0      ((bmBit0 | bmBit1) << 0)
#define    bmURESET        bmBit2
#define    bmFPRESU        bmBit3
#define    bmPENA          bmBit4
#define    bmPSUSP         bmBit5
#define    bmVB_ON         bmBit6
#define    bmTERM_SEL      bmBit7
#define    bmTERM_ENA      bmBit8
#define    bmDCHRG         bmBit9
#define    bmVBS_ON        bmBit10
#define    bmID_PU         bmBit12
#define    bmP_LEN         ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7) << 16)

/* bit definitions for Port Status Change Event */
#define    bmOCU_EV        bmBit0
#define    bmBERR_EV       bmBit1
#define    bmRSU_EV        bmBit2
#define    bmRSUC_EV       bmBit3
#define    bmSUSP_EV       bmBit4
#define    bmURES_EV       bmBit5
#define    bmCDC_EV        bmBit6
#define    bmPWRSC_EV      bmBit7
#define    bmP_END_EV      bmBit8

/* bit definitions for Port Status Change Event Mask */
#define    bmOCU_EM        bmBit0
#define    bmBERR_EM       bmBit1
#define    bmRSU_EM        bmBit2
#define    bmRSUC_EM       bmBit3
#define    bmSUSP_EM       bmBit4
#define    bmURES_EM       bmBit5
#define    bmCDC_EM        bmBit6
#define    bmPWRSC_EM      bmBit7
#define    bmP_END_EM      bmBit8

/* bit definitions for Pipe Control */
#define    bmTPID        ((bmBit0 | bmBit1) << 0)
#define    TPID_OUT      ((0) << 0)
#define    TPID_IN       ((bmBit0) << 0)
#define    TPID_SETUP    ((bmBit1) << 0)
#define    bmACT         bmBit2

/* bit definitions for Pipe Configuration */
#define    bmMPS        ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7 | bmBit8 | bmBit9) << 0)
#define    bmET         ((bmBit0 | bmBit1) << 10)
#define    ET_CTRL      ((0) << 10)
#define    ET_ISO       ((bmBit0) << 10)
#define    ET_BULK      ((bmBit1) << 10)
#define    ET_INT       ((bmBit0 | bmBit1) << 10)
#define    bmSTRM       bmBit12
#define    bmEPS        bmBit13
#define    bmACID       bmBit14
#define    bmSTALL      bmBit15
#define    bmPOFF       ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7) << 16)
#define    bmPI         ((bmBit0 | bmBit1 | bmBit2 | bmBit3) << 24)
#define    bmSKIPISO    bmBit28
#define    bmHIDBE      bmBit29
#define    bmIOT        bmBit30

/* bit definitions for Pipe Address */
#define    bmEPNR       ((bmBit0 | bmBit1 | bmBit2 | bmBit3) << 0)
#define    bmEPADDR     ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6) << 4)

/* bit definitions for Pipe Status */
#define    bmDBOFF      ((bmBit0 | bmBit1) << 0)
#define    bmDT         bmBit2
#define    bmDBSEL      bmBit3
#define    bmBBL        bmBit4
#define    bmHALT       bmBit5
#define    bmACTS       bmBit6
#define    bmDBERR      bmBit7
#define    bmCERR       ((bmBit0 | bmBit1) << 8)

/* bit definitions for Pipe Data Pointer */

/* bit definitions for Pipe Total Bytes */
#define    bmDBV        bmBit31

/* bit definitions for Pipe Alternative Data Pointer */

/* bit definitions for Pipe Alternative Data Total Bytes */
#define    bmADBV      bmBit31

/* bit definitions for Debug Control */
#define    bmDBSERRDET    bmBit0
#define    bmDBSTX        bmBit1
#define    bmFTXCRC5E     bmBit2
#define    bmFTXCRC16E    bmBit3
#define    bmFRXCRCE      bmBit4
#define    bmFRXCRC5G     bmBit5
#define    bmFRXCRC16G    bmBit6
#define    bmUDDPID       bmBit7
#define    bmUDHSPID      bmBit8
#define    bmUDTPID       bmBit9

/* bit definitions for Debug PID */
#define    bmDDPID        ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7) << 0)
#define    bmDTPID        ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7) << 8)
#define    bmDHSPID       ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7) << 16)

/* bit definitions for Debug Status */
#define    bmDRXPID       ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7) << 0)

/* bit definitions for Test */
#define    bmRW32         ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5 | bmBit6 | bmBit7 | bmBit8 | bmBit9 | bmBit10 | bmBit11 | bmBit12 | bmBit13 | bmBit14 | bmBit15 | bmBit16 | bmBit17 | bmBit18 | bmBit19 | bmBit20 | bmBit21 | bmBit22 | bmBit23 | bmBit24 | bmBit25 | bmBit26 | bmBit27 | bmBit28 | bmBit29 | bmBit30 | bmBit31) << 0)

/* bit definitions for Main Configuration */
#define    bmNOP_CFG      ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5) << 0)
#define    bmDW_CFG       ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5) << 6)
#define    bmRAW_CFG      ((bmBit0 | bmBit1 | bmBit2 | bmBit3 | bmBit4 | bmBit5) << 24)

/* bit definitions for Mode Configuration */
#define    bmDBG_CFG      bmBit16
#define    bmXDBG_CFG    bmBit17


#endif /* __USB_REGMAP_H */
