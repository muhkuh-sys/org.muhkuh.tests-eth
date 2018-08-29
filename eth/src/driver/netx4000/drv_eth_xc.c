#include "driver/netx4000/drv_eth_xc.h"

#include <string.h>

#include "netx4000/cr7_global_timer.h"
#include "netx_io_areas.h"
#include "driver/netx4000/eth_std_mac_xpec_regdef.h"
#include "driver/netx4000/nec_cb12.h"
#include "options.h"
#include "uprintf.h"



#define ETHERNET_MINIMUM_FRAMELENGTH                    60
#define ETHERNET_MAXIMUM_FRAMELENGTH                    1518

#define NUM_FIFO_CHANNELS_PER_UNIT                      8      /**< Number of FIFO units per XC channel */
#define FIFO_ENTRIES                                    100    /**< FIFO depth for each of the 8 FIFOs  */
#define ETH_FRAME_BUF_SIZE                              1560   /**< size of a frame buffer     */
#define INTRAM_SEGMENT_SIZE                             0x10000 /**< size of the internal ram segments */

#define ETHERNET_FIFO_EMPTY                             0      /**< Empty pointer FIFO               */
#define ETHERNET_FIFO_IND_HI                            1      /**< High priority indication FIFO    */
#define ETHERNET_FIFO_IND_LO                            2      /**< Low priority indication FIFO     */
#define ETHERNET_FIFO_REQ_HI                            3      /**< High priority request FIFO       */
#define ETHERNET_FIFO_REQ_LO                            4      /**< Low priority request FIFO        */
#define ETHERNET_FIFO_CON_HI                            5      /**< High priority confirmation FIFO  */
#define ETHERNET_FIFO_CON_LO                            6      /**< Low priority confirmation FIFO   */



typedef enum PHYCTRL_LED_MODE_Etag
{
	PHYCTRL_LED_MODE_MANUAL    = 0,
	PHYCTRL_LED_MODE_STATIC    = 1,
	PHYCTRL_LED_MODE_FLASHING  = 2,
	PHYCTRL_LED_MODE_COMBINED  = 3
} PHYCTRL_LED_MODE_E;



static NX4000_PHY_CTRL_AREA_T * const aptPhyCtrl[4] =
{
	(NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc0_phy_ctrl0,
	(NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc0_phy_ctrl1,
	(NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc1_phy_ctrl0,
	(NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc1_phy_ctrl1
};



static NX4000_POINTER_FIFO_AREA_T * const aptPFifo[2] =
{
	(NX4000_POINTER_FIFO_AREA_T*) Addr_NX4000_xc0_pointer_fifo,
	(NX4000_POINTER_FIFO_AREA_T*) Addr_NX4000_xc1_pointer_fifo
};



static NX4000_SYSTIME_AREA_T * const aptSystime[2] =
{
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime0,
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime1
};



static const unsigned long aulIntRamStart[4] =
{
	Addr_NX4000_intram0,
	Addr_NX4000_intram1,
	Addr_NX4000_intram2,
	Addr_NX4000_intram3
};



static ETHMAC_XPEC_DPM * const aptXpecDramArea[4] =
{
	(ETHMAC_XPEC_DPM*) Adr_NX4000_xc0_tpec0_dram_ram_start,
	(ETHMAC_XPEC_DPM*) Adr_NX4000_xc0_tpec1_dram_ram_start,
	(ETHMAC_XPEC_DPM*) Adr_NX4000_xc1_tpec0_dram_ram_start,
	(ETHMAC_XPEC_DPM*) Adr_NX4000_xc1_tpec1_dram_ram_start
};



static NX4000_XMAC_AREA_T * const aptXmacArea[4] =
{
	(NX4000_XMAC_AREA_T*) Addr_NX4000_xc0_xmac0_regs,
	(NX4000_XMAC_AREA_T*) Addr_NX4000_xc0_xmac1_regs,
	(NX4000_XMAC_AREA_T*) Addr_NX4000_xc1_xmac0_regs,
	(NX4000_XMAC_AREA_T*) Addr_NX4000_xc1_xmac1_regs
};



static NX4000_XPEC_AREA_T * const aptRpecRegArea[4] =
{
	(NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_rpec0_regs,
	(NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_rpec1_regs,
	(NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_rpec0_regs,
	(NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_rpec1_regs
};



static NX4000_XPEC_AREA_T * const aptTpecRegArea[4] =
{
	(NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_tpec0_regs,
	(NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_tpec1_regs,
	(NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_tpec0_regs,
	(NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_tpec1_regs
};


static unsigned long * const apulRpecPramArea[4] =
{
	(unsigned long*) Adr_NX4000_xc0_rpec0_pram_ram_start,
	(unsigned long*) Adr_NX4000_xc0_rpec1_pram_ram_start,
	(unsigned long*) Adr_NX4000_xc1_rpec0_pram_ram_start,
	(unsigned long*) Adr_NX4000_xc1_rpec1_pram_ram_start
};



static unsigned long * const apulTpecPramArea[4] =
{
	(unsigned long*) Adr_NX4000_xc0_tpec0_pram_ram_start,
	(unsigned long*) Adr_NX4000_xc0_tpec1_pram_ram_start,
	(unsigned long*) Adr_NX4000_xc1_tpec0_pram_ram_start,
	(unsigned long*) Adr_NX4000_xc1_tpec1_pram_ram_start
};


static const unsigned long XcCode_xc0_rpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4020000, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_xc0_rpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4020800, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_xc1_rpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4021000, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_xc1_rpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4021800, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_xc0_tpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4020400, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_xc0_tpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4020c00, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_xc1_tpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4021400, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_xc1_tpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4021c00, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};


static const unsigned long* paulRpuResetCodes[4]=
{
  XcCode_xc0_rpu_reset0,
  XcCode_xc0_rpu_reset1,
  XcCode_xc1_rpu_reset0,
  XcCode_xc1_rpu_reset1,
};

static const unsigned long* paulTpuResetCodes[4]=
{
  XcCode_xc0_tpu_reset0,
  XcCode_xc0_tpu_reset1,
  XcCode_xc1_tpu_reset0,
  XcCode_xc1_tpu_reset1,
};


#include "driver/netx4000/rpu_eth0.h"
#include "driver/netx4000/rpu_eth1.h"
#include "driver/netx4000/rpu_eth2.h"
#include "driver/netx4000/rpu_eth3.h"
#include "driver/netx4000/tpu_eth0.h"
#include "driver/netx4000/tpu_eth1.h"
#include "driver/netx4000/tpu_eth2.h"
#include "driver/netx4000/tpu_eth3.h"

#include "driver/netx4000/xpec_eth_std_mac_rpec0.h"
#include "driver/netx4000/xpec_eth_std_mac_rpec1.h"
#include "driver/netx4000/xpec_eth_std_mac_rpec2.h"
#include "driver/netx4000/xpec_eth_std_mac_rpec3.h"

#include "driver/netx4000/xpec_clean_tpec0.h"
#include "driver/netx4000/xpec_clean_tpec1.h"
#include "driver/netx4000/xpec_clean_tpec2.h"
#include "driver/netx4000/xpec_clean_tpec3.h"

static const unsigned long* const paulxPecRpecCodes[4] =
{
  XcCode_xpec_eth_std_mac_rpec0,
  XcCode_xpec_eth_std_mac_rpec1,
  XcCode_xpec_eth_std_mac_rpec2,
  XcCode_xpec_eth_std_mac_rpec3
};


static const unsigned long* const paulxPecTpecCodes[4] =
{
  XcCode_xpec_clean_tpec0,
  XcCode_xpec_clean_tpec1,
  XcCode_xpec_clean_tpec2,
  XcCode_xpec_clean_tpec3
};


static const unsigned long* const paulxMacRpuCodes[4] =
{
  XcCode_rpu_eth0,
  XcCode_rpu_eth1,
  XcCode_rpu_eth2,
  XcCode_rpu_eth3
};


static const unsigned long* const paulxMacTpuCodes[4] =
{
  XcCode_tpu_eth0,
  XcCode_tpu_eth1,
  XcCode_tpu_eth2,
  XcCode_tpu_eth3
};


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/


static void NX4000_XC_Load(const unsigned long* pulXcPrg)
{
	volatile unsigned long *pulDst, *pulDstCnt;
	const unsigned long *pulSrcStart, *pulSrcCnt, *pulSrcEnd;
	unsigned int uiElements;


	/* Get the number of code elements. */
	uiElements = pulXcPrg[0] / sizeof(unsigned long) - 1;
  
	/* Get the pointer in the XC area. */
	/* ram_virtual_start + code_physical_start - ram_physical_start */
	pulDst = (volatile unsigned long*) pulXcPrg[2];
  
	/* get source start and end pointer */
	pulSrcStart = pulXcPrg + 3;
	pulSrcEnd = pulSrcStart + uiElements;
	
	/* Copy the code to XC ram */
	pulSrcCnt = pulSrcStart;
	pulDstCnt = pulDst;
	while( pulSrcCnt<pulSrcEnd )
	{
		*pulDstCnt = *pulSrcCnt;
		pulDstCnt++;
		pulSrcCnt++;
	}
  
	/* get the number of trailing loads */
	uiElements = pulXcPrg[1] / sizeof(unsigned long);
  
	/* get source start and end pointer */
	pulSrcCnt = pulXcPrg + 2 + pulXcPrg[0] / sizeof(unsigned long);
	pulSrcEnd = pulSrcCnt + uiElements;
  
	/* write all trailing loads */
	while( pulSrcCnt<pulSrcEnd )
	{
		/* get the destination address ( ram_virtual_start + data_physical_start - ram_physical_start) */
		pulDst = (volatile unsigned long*) *pulSrcCnt;
		pulSrcCnt++;
    
		/* write the data */
		*pulDst = *pulSrcCnt;
		pulSrcCnt++;
	}
}



/*****************************************************************************/
/*! Reset XC Code
* \description
*   Reset XC port.
* \class 
*   XC 
* \params
*   uPortNo          [in]  XC Port Number
*   pvUser           [in]  User Specific Paramters
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
static int NX4000_XC_Reset( unsigned int uXcNo, unsigned int uXcPortNo)
{
	HOSTDEF(ptXcStartStopArea);
	HOSTDEF(ptXpecIrqRegistersArea);
	NX4000_XMAC_AREA_T* ptXmac;
	NX4000_XPEC_AREA_T* ptRpec;
	NX4000_XPEC_AREA_T* ptTpec;
	volatile unsigned long* pulRpecPram;
	volatile unsigned long* pulTpecPram;
	unsigned int uIdx;
	unsigned int uPortNo = uXcNo * 2 + uXcPortNo;
  

	ptXmac      = aptXmacArea[uPortNo];
	ptRpec      = aptRpecRegArea[uPortNo];
	ptTpec      = aptTpecRegArea[uPortNo];
	pulRpecPram = apulRpecPramArea[uPortNo];
	pulTpecPram = apulTpecPramArea[uPortNo];

	/* Stop xPEC and xMAC */
	ptXcStartStopArea->ulXc_start_stop_ctrl = (MSK_NX4000_xc_start_stop_ctrl_xc0_stop_rpec0 | 
                                           MSK_NX4000_xc_start_stop_ctrl_xc0_stop_tpec0 | 
                                           MSK_NX4000_xc_start_stop_ctrl_xc0_stop_rpu0 | 
                                           MSK_NX4000_xc_start_stop_ctrl_xc0_stop_tpu0) << (4*uPortNo);

	/* Clear output enable of tx asap */
	ptXmac->ulXmac_config_obu  = DFLT_VAL_NX4000_xmac_config_obu;
	/* Clear output enable of io0..5 asap */

	ptRpec->aulStatcfg[uPortNo] = 0xffff0000;
            
	switch (uPortNo)
	{
	case 0:
		ptRpec->ulXpec_config = MSK_NX4000_xpec_config_reset_urx_fifo0 | MSK_NX4000_xpec_config_reset_utx_fifo0; 
		break;
	case 1:
		ptRpec->ulXpec_config = MSK_NX4000_xpec_config_reset_urx_fifo1 | MSK_NX4000_xpec_config_reset_utx_fifo1; 
		break;
	}
  
	/* load ratemul reset code */
	NX4000_XC_Load(paulRpuResetCodes[uPortNo]);
	NX4000_XC_Load(paulTpuResetCodes[uPortNo]);
  
	ptXmac->ulXmac_rpu_pc      = 0; /* Reset PC to 0 */
	ptXmac->ulXmac_tpu_pc      = 0; /* Reset PC to 0 */

	ptXcStartStopArea->ulXc_start_stop_ctrl = (MSK_NX4000_xc_start_stop_ctrl_xc0_start_rpu0 |
                                           MSK_NX4000_xc_start_stop_ctrl_xc0_start_tpu0) << (4*uPortNo);

	/* !!!! ATTENTION: There must be enougth time between starting xMAC and stopping xMAC to execute reset programm */

	pulRpecPram[0] = 0xC0000FFF;                                   /* Use the command wait b000000000000,b111111111111 at Address 0*/
	pulTpecPram[0] = 0xC0000FFF;                                   /* Use the command wait b000000000000,b111111111111 at Address 0*/

	ptRpec->ulXpec_pc = 0x7ff;                                     /* Reset the Program Counter to 0x7ff */
	ptTpec->ulXpec_pc = 0x7ff;                                     /* Reset the Program Counter to 0x7ff */


	/* let the XC run for at least 10 cycles */
	for (uIdx = 0; uIdx < 10; uIdx++)
	{
		ptXcStartStopArea->ulXc_start_stop_ctrl = (MSK_NX4000_xc_start_stop_ctrl_xc0_start_rpec0 |
                                             MSK_NX4000_xc_start_stop_ctrl_xc0_start_tpec0) << (4*uPortNo);
	}

	ptXcStartStopArea->ulXc_start_stop_ctrl = (MSK_NX4000_xc_start_stop_ctrl_xc0_stop_rpec0 |
                                           MSK_NX4000_xc_start_stop_ctrl_xc0_stop_tpec0) << (4*uPortNo);
	ptRpec->ulXpec_pc     = 0x7ff;                                 /* Reset the Program Counter to 0x7ff */
	ptTpec->ulXpec_pc     = 0x7ff;                                 /* Reset the Program Counter to 0x7ff */

	/* reset all registers */
	ptRpec->aulXpec_r[0]   = 0;
	ptTpec->aulXpec_r[0]   = 0;
	ptRpec->aulXpec_r[1]   = 0;
	ptTpec->aulXpec_r[1]   = 0;
	ptRpec->aulXpec_r[2]   = 0;
	ptTpec->aulXpec_r[2]   = 0;
	ptRpec->aulXpec_r[3]   = 0;
	ptTpec->aulXpec_r[3]   = 0;
	ptRpec->aulXpec_r[4]   = 0;
	ptTpec->aulXpec_r[4]   = 0;
	ptRpec->aulXpec_r[5]   = 0;
	ptTpec->aulXpec_r[5]   = 0;
	ptRpec->aulXpec_r[6]   = 0;
	ptTpec->aulXpec_r[6]   = 0;
	ptRpec->aulXpec_r[7]   = 0;
	ptTpec->aulXpec_r[7]   = 0;

	switch (uPortNo)
	{
	case 0:
		ptRpec->ulXpec_stat_bits_shared = 0x00ff0000;
		break;
	case 1:
		ptRpec->ulXpec_stat_bits_shared = 0xff000000;
		break;
	}

	ptRpec->ulRange_urtx_count = 0;
	ptTpec->ulRange_urtx_count = 0;
	ptRpec->ulRange45          = 0;
	ptTpec->ulRange45          = 0;
	ptRpec->ulRange67          = 0;
	ptTpec->ulRange67          = 0;
	ptRpec->ulUrx_count        = 0;
	ptTpec->ulUrx_count        = 0;
	ptRpec->ulUtx_count        = 0;
	ptTpec->ulUtx_count        = 0;
  
	/* Stop all Timers */
	ptRpec->ulTimer4       = 0;
	ptTpec->ulTimer4       = 0;
	ptRpec->ulTimer5       = 0;
	ptTpec->ulTimer5       = 0;
	ptRpec->aulTimer[0]    = 0; 
	ptTpec->aulTimer[0]    = 0; 
	ptRpec->aulTimer[1]    = 0;
	ptTpec->aulTimer[1]    = 0;
	ptRpec->aulTimer[2]    = 0;
	ptTpec->aulTimer[2]    = 0;
	ptRpec->aulTimer[3]    = 0;
	ptTpec->aulTimer[3]    = 0;

	ptRpec->ulIrq          = 0xFFFF0000; /* Clear XPEC side IRQ request lines */
	ptTpec->ulIrq          = 0xFFFF0000; /* Clear XPEC side IRQ request lines */

	/* Reset events */
	ptRpec->ulEc_maska     = 0x0000FFFF;
	ptTpec->ulEc_maska     = 0x0000FFFF;
	ptRpec->ulEc_maskb     = 0x0000FFFF;
	ptTpec->ulEc_maskb     = 0x0000FFFF;
	ptRpec->aulEc_mask[0]  = 0x0000FFFF; 
	ptTpec->aulEc_mask[0]  = 0x0000FFFF; 
	ptRpec->aulEc_mask[1]  = 0x0000FFFF;
	ptTpec->aulEc_mask[1]  = 0x0000FFFF;
	ptRpec->aulEc_mask[2]  = 0x0000FFFF;
	ptTpec->aulEc_mask[2]  = 0x0000FFFF;
	ptRpec->aulEc_mask[3]  = 0x0000FFFF;
	ptTpec->aulEc_mask[3]  = 0x0000FFFF;
	ptRpec->aulEc_mask[4]  = 0x0000FFFF;
	ptTpec->aulEc_mask[4]  = 0x0000FFFF;
	ptRpec->aulEc_mask[5]  = 0x0000FFFF;
	ptTpec->aulEc_mask[5]  = 0x0000FFFF;
	ptRpec->aulEc_mask[6]  = 0x0000FFFF;
	ptTpec->aulEc_mask[6]  = 0x0000FFFF;
	ptRpec->aulEc_mask[7]  = 0x0000FFFF;
	ptTpec->aulEc_mask[7]  = 0x0000FFFF;
	ptRpec->aulEc_mask[8]  = 0x0000FFFF;
	ptTpec->aulEc_mask[8]  = 0x0000FFFF;
	ptRpec->aulEc_mask[9]  = 0x0000FFFF;
	ptTpec->aulEc_mask[9]  = 0x0000FFFF;

	/* Reset shared registers, reset URX/UTX fifos */
	/* Reset SR0-3 for XC0, SR4-7 for XC1 */
	for( uIdx = 4 * uPortNo; uIdx < 4 * uPortNo + 4; ++uIdx )
	{
		ptRpec->aulXpec_sr[uIdx] = 0;
	}
	/* Reset SR8-11 for XC0, SR12-15 for XC1 */
	for( uIdx = 8 + 4 * uPortNo; uIdx < 12 + 4 * uPortNo; ++uIdx )
	{
		ptRpec->aulXpec_sr[uIdx] = 0;
	}
    
	ptRpec->ulDatach_wr_cfg = 0;
	ptTpec->ulDatach_wr_cfg = 0;
	ptRpec->ulDatach_rd_cfg = 0;
	ptTpec->ulDatach_rd_cfg = 0;
	ptRpec->ulSysch_addr    = 0;
	ptTpec->ulSysch_addr    = 0;

	/* confirm all interrupts from xPEC -> Select the specific XC instance and get IRQ */
	if( uXcNo==0 )
	{
		ptXpecIrqRegistersArea->aulXc0_irq_xpec[uXcPortNo] = 0x0000FFFF;
	}
	else
	{
		ptXpecIrqRegistersArea->aulXc1_irq_xpec[uXcPortNo] = 0x0000FFFF;
	}
  
	/* hold xMAC */
	ptXcStartStopArea->ulXc_start_stop_ctrl = (MSK_NX4000_xc_start_stop_ctrl_xc0_stop_rpu0 |
                                           MSK_NX4000_xc_start_stop_ctrl_xc0_stop_tpu0) << (4*uPortNo);
      
	/* reset all xMAC registers to default values */
	ptXmac->ulXmac_rx_hw               = 0;
	ptXmac->ulXmac_rx_hw_count         = 0;
	ptXmac->ulXmac_tx                  = 0;
	ptXmac->ulXmac_tx_hw               = 0;
	ptXmac->ulXmac_tx_hw_count         = 0;
	ptXmac->ulXmac_tx_sent             = 0;
	ptXmac->aulXmac_wr[0]              = 0;
	ptXmac->aulXmac_wr[1]              = 0;
	ptXmac->aulXmac_wr[2]              = 0;
	ptXmac->aulXmac_wr[3]              = 0;
	ptXmac->aulXmac_wr[4]              = 0;
	ptXmac->aulXmac_wr[5]              = 0;
	ptXmac->aulXmac_wr[6]              = 0;
	ptXmac->aulXmac_wr[7]              = 0;
	ptXmac->aulXmac_wr[8]              = 0;
	ptXmac->aulXmac_wr[9]              = 0;
	ptXmac->ulXmac_config_mii          = 0;
	ptXmac->ulXmac_config_rx_nibble_fifo  = DFLT_VAL_NX4000_xmac_config_rx_nibble_fifo;
	ptXmac->ulXmac_config_tx_nibble_fifo  = 0;
	ptXmac->ulXmac_rpu_count1          = 0;
	ptXmac->ulXmac_rpu_count2          = 0;
	ptXmac->ulXmac_tpu_count1          = 0;
	ptXmac->ulXmac_tpu_count2          = 0;
	ptXmac->ulXmac_rx_count            = 0;
	ptXmac->ulXmac_tx_count            = 0;
	ptXmac->ulXmac_rpm_mask0           = 0;
	ptXmac->ulXmac_rpm_val0            = 0;
	ptXmac->ulXmac_rpm_mask1           = 0;
	ptXmac->ulXmac_rpm_val1            = 0;
	ptXmac->ulXmac_tpm_mask0           = 0;
	ptXmac->ulXmac_tpm_val0            = 0;
	ptXmac->ulXmac_tpm_mask1           = 0;
	ptXmac->ulXmac_tpm_val1            = 0;
	
	ptXmac->ulXmac_rx_crc_polynomial_l = 0;
	ptXmac->ulXmac_rx_crc_polynomial_h = 0;
	ptXmac->ulXmac_rx_crc_l            = 0;
	ptXmac->ulXmac_rx_crc_h            = 0;
	ptXmac->ulXmac_rx_crc_cfg          = 0;
	ptXmac->ulXmac_tx_crc_polynomial_l = 0;
	ptXmac->ulXmac_tx_crc_polynomial_h = 0;
	ptXmac->ulXmac_tx_crc_l            = 0;
	ptXmac->ulXmac_tx_crc_h            = 0;
	ptXmac->ulXmac_tx_crc_cfg          = 0;
	
	ptXmac->ulXmac_rx_crc32_l          = 0;
	ptXmac->ulXmac_rx_crc32_h          = 0;
	ptXmac->ulXmac_rx_crc32_cfg        = 0;
	ptXmac->ulXmac_tx_crc32_l          = 0;
	ptXmac->ulXmac_tx_crc32_h          = 0;
	ptXmac->ulXmac_tx_crc32_cfg        = 0;
	
	ptXmac->ulXmac_config_sbu2         = DFLT_VAL_NX4000_xmac_config_sbu2;
	ptXmac->ulXmac_config_obu2         = DFLT_VAL_NX4000_xmac_config_obu2;
	
	ptXmac->ulXmac_rpu_pc              = 0;
	ptXmac->ulXmac_tpu_pc              = 0;
	
	return 0;
}



static void NX4000_XC_Start(unsigned int uXcNo, unsigned int uXcPortNo)
{
	HOSTDEF(ptXcStartStopArea);
	NX4000_XMAC_AREA_T* ptXmacArea;
	NX4000_XPEC_AREA_T* ptRpecArea;
	NX4000_XPEC_AREA_T* ptTpecArea;
	unsigned int uPortNo = uXcNo * 2 + uXcPortNo;


	ptXmacArea = aptXmacArea[uPortNo];
	ptRpecArea = aptRpecRegArea[uPortNo];
	ptTpecArea = aptTpecRegArea[uPortNo];

	/* reset pc of units */
	ptXmacArea->ulXmac_rpu_pc = 0;
	ptXmacArea->ulXmac_tpu_pc = 0;
	ptRpecArea->ulXpec_pc     = 0x7ff;  
	ptTpecArea->ulXpec_pc     = 0x7ff;  

	/* start units */
	ptXcStartStopArea->ulXc_start_stop_ctrl = ((MSK_NX4000_xc_start_stop_ctrl_xc0_start_rpec0 |
                                            MSK_NX4000_xc_start_stop_ctrl_xc0_start_tpec0 |
                                            MSK_NX4000_xc_start_stop_ctrl_xc0_start_rpu0 |
                                            MSK_NX4000_xc_start_stop_ctrl_xc0_start_tpu0) << (4*uPortNo));
}










static void *convert_fifo_value_to_packet_pointer(unsigned long ulFifoValue)
{
	unsigned long ulFrameNr;
	unsigned long ulRamSegment;
	unsigned long ulValue;
	void *pvPacket;


	/* extract ram bank and frame number */ 
	ulFrameNr   = ulFifoValue;
	ulFrameNr  &= MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
	ulFrameNr >>= SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;

	ulRamSegment   = ulFifoValue;
	ulRamSegment  &= MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM;
	ulRamSegment >>= SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM;

	ulValue  = aulIntRamStart[ulRamSegment];
	ulValue += ETH_FRAME_BUF_SIZE * ulFrameNr;

	pvPacket = (void*)ulValue;
	return pvPacket;
}



/* Convert a frame address to a FIFO pointer. */
static unsigned long convert_packet_pointer_to_fifo_value(void *pvPacket)
{       
        unsigned long ulFrameNr;
        unsigned long ulRamSegment;
        unsigned long ulValue;

        
        /* NOTE: This routine assumes that all RAM areas have a size of 0x00010000 bytes
         *       and that they are sequential one after the other.
         *       In this case the segment number is in bit 16 and 17.
         */
        ulRamSegment   = (unsigned long)pvPacket;
        ulRamSegment >>= 16U;
        ulRamSegment  &= 3U;

        /* Extract the frame buffer number. */
        ulFrameNr  = (unsigned long)pvPacket;
        ulFrameNr &= 0x0000ffffU;
        ulFrameNr /= ETH_FRAME_BUF_SIZE;
        
        /* Combine the ram segment and frame number. */
        ulValue  = ulRamSegment << SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM;
        ulValue |= ulFrameNr << SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;

        return ulValue;
}



/*-----------------------------------------------------------------------------------------------------------*/


static unsigned int drv_eth_xc_get_link_status(NETWORK_DRIVER_T *ptNetworkDriver)
{
	NX4000_PHY_CTRL_AREA_T *ptPhyCtrl;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned long ulValue;
	unsigned int uiLinkStatus;


	uiLinkStatus = 0;

	/* Retrieve the link status from the Ethernet port. */
	ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);
	ptPhyCtrl = aptPhyCtrl[ptHandle->uiEthPortNr];
	ulValue  = ptPhyCtrl->ulInt_phy_ctrl_led;
	ulValue &= HOSTMSK(int_phy_ctrl_led_link_ro);
	if( ulValue!=0 )
	{
		uiLinkStatus = 1;
	}
	
	return uiLinkStatus;
}



static void *drv_eth_xc_get_empty_packet(NETWORK_DRIVER_T *ptNetworkDriver)
{
	NX4000_POINTER_FIFO_AREA_T *ptPfifoArea;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned long ulFifoValue;
	unsigned int uiFifoNr; 
	unsigned long ulValue;
	void *pvPacket;


	/* Expect no free packet. */
	pvPacket = NULL;

	/* get FIFO fill level and check if there is at least one element */
	ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);
	uiFifoNr = (ptHandle->uiXcPort * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_EMPTY;

	/* keep at least one pointer for the XC level (two parties share this empty pointer FIFO) */
	ptPfifoArea = aptPFifo[ptHandle->uiXcUnit];
	ulValue = ptPfifoArea->aulPfifo_fill_level[uiFifoNr];
	if( ulValue>1U )
	{
		/* retrieve the FIFO element */
		ulFifoValue = ptPfifoArea->aulPfifo[uiFifoNr];
		pvPacket = convert_fifo_value_to_packet_pointer(ulFifoValue);
	}

	return pvPacket;
}



static void drv_eth_xc_release_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket)
{
	NX4000_POINTER_FIFO_AREA_T *ptPfifoArea;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned int uiFifoNr; 
	unsigned long ulFifoValue;


	/* Convert the pointer to a FIFO value. */
	ulFifoValue = convert_packet_pointer_to_fifo_value(pvPacket);

	ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);
	uiFifoNr = (ptHandle->uiXcPort * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_EMPTY;
	ptPfifoArea = aptPFifo[ptHandle->uiXcUnit];
	ptPfifoArea->aulPfifo[uiFifoNr] = ulFifoValue;
}



static void drv_eth_xc_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, unsigned int sizPacket)
{
	NX4000_POINTER_FIFO_AREA_T *ptPfifoArea;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned long ulFifoValue;
	unsigned int uiFifoNr;


	/* Pad too short packets. */
	if( sizPacket<ETHERNET_MINIMUM_FRAMELENGTH )
	{
		sizPacket = ETHERNET_MINIMUM_FRAMELENGTH;
	}
	/* Crop over-sized packets. */
	else if( sizPacket>ETHERNET_MAXIMUM_FRAMELENGTH )
	{
		sizPacket = ETHERNET_MAXIMUM_FRAMELENGTH;
	}
	
	/* Convert the pointer to a FIFO value. */
	ulFifoValue  = convert_packet_pointer_to_fifo_value(pvPacket);
	/* Do not send a confirmation. */
	ulFifoValue |= MSK_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON;
	/* Add the size information. */
	ulFifoValue |= sizPacket << SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN;

	ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);
	uiFifoNr = (ptHandle->uiXcPort * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_REQ_LO;
	ptPfifoArea = aptPFifo[ptHandle->uiXcUnit];

	ptPfifoArea->aulPfifo[uiFifoNr] = ulFifoValue;
}



static void *drv_eth_xc_get_received_packet(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int *psizPacket)
{
	NX4000_POINTER_FIFO_AREA_T *ptPfifoArea;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned long ulFillLevel;
	unsigned int uiFifoNr;
	void *pvPacket;
	unsigned long ulFifoValue;
	unsigned int sizPacket;


	/* Expect no packet. */
	pvPacket = NULL;

	ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);
	uiFifoNr = (ptHandle->uiXcPort * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_IND_LO;
	ptPfifoArea = aptPFifo[ptHandle->uiXcUnit];

	ulFillLevel = ptPfifoArea->aulPfifo_fill_level[uiFifoNr];
	if( ulFillLevel!=0 )
	{
		ulFifoValue = ptPfifoArea->aulPfifo[uiFifoNr];
		pvPacket = convert_fifo_value_to_packet_pointer(ulFifoValue);

		ulFifoValue  &= MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN;
		ulFifoValue >>= SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN;
		sizPacket = (unsigned int)ulFifoValue;
		*psizPacket = sizPacket;
	}

	return pvPacket;
}



static void drv_eth_xc_deactivate(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)))
{
}



static const NETWORK_IF_T tNetworkIfXc =
{
	.pfnGetLinkStatus = drv_eth_xc_get_link_status,
	.pfnGetEmptyPacket = drv_eth_xc_get_empty_packet,
	.pfnReleasePacket = drv_eth_xc_release_packet,
	.pfnSendPacket = drv_eth_xc_send_packet,
	.pfnGetReceivedPacket = drv_eth_xc_get_received_packet,
	.pfnDeactivate = drv_eth_xc_deactivate,
};



static const NETWORK_IF_T tNetworkIfXcLvds =
{
	.pfnGetLinkStatus = drv_eth_xc_get_link_status,
	.pfnGetEmptyPacket = drv_eth_xc_get_empty_packet,
	.pfnReleasePacket = drv_eth_xc_release_packet,
	.pfnSendPacket = drv_eth_xc_send_packet,
	.pfnGetReceivedPacket = drv_eth_xc_get_received_packet,
/* TODO: add a custom deactivate routine. At least the portcontrol is different. */
	.pfnDeactivate = drv_eth_xc_deactivate,
};


void pfifo_reset(void)
{
	HOSTDEF(ptXc0PointerFifoArea);
	HOSTDEF(ptXc1PointerFifoArea);
	unsigned int uiCnt;
	unsigned long ulBorderCnt;
	

	/* Reset all pointer FIFOs. */
	ptXc0PointerFifoArea->ulPfifo_reset = 0xffffffffU;
	ptXc1PointerFifoArea->ulPfifo_reset = 0xffffffffU;

	/* Reset the pointer FIFO borders. */
	ulBorderCnt = 99;
	for(uiCnt=0; uiCnt<(sizeof(ptXc0PointerFifoArea->aulPfifo_border)/sizeof(unsigned long)); ++uiCnt)
	{
		ptXc0PointerFifoArea->aulPfifo_border[uiCnt] = ulBorderCnt;
		ptXc1PointerFifoArea->aulPfifo_border[uiCnt] = ulBorderCnt;

		ulBorderCnt += 100U;
	}
	
	/* clear reset flag of all FIFOs */
	ptXc0PointerFifoArea->ulPfifo_reset = 0xffffffffU;
	ptXc1PointerFifoArea->ulPfifo_reset = 0xffffffffU;
}



static void pfifo_init(unsigned int uPortNo)
{
	NX4000_POINTER_FIFO_AREA_T *ptPfifoArea;

	unsigned long ulFifoPtr = 0;
	unsigned long ulFifoStart;
	unsigned long ulFifoEnd;
	unsigned long ulFifoNum;
	unsigned long ulEmptyPtrCnt;
	unsigned long ulFrame;
	unsigned int uiXcUnit;
	unsigned int uiXcPortNo = (uPortNo)&1;
	unsigned long ulValue;


	uiXcUnit = (uPortNo >> 1U) & 1U;
	uiXcPortNo = uPortNo & 1U;

	ptPfifoArea = aptPFifo[uiXcUnit];

	/* set reset bit for all pointer FIFOs */
	ulValue  = ptPfifoArea->ulPfifo_reset;
	ulValue |= ((1U<<NUM_FIFO_CHANNELS_PER_UNIT)-1U) << (NUM_FIFO_CHANNELS_PER_UNIT * uiXcPortNo);
	ptPfifoArea->ulPfifo_reset = ulValue;

	/* get FIFO start and end number of this port number */
	ulFifoStart = uiXcPortNo * NUM_FIFO_CHANNELS_PER_UNIT;
	ulFifoEnd = ulFifoStart + NUM_FIFO_CHANNELS_PER_UNIT;

	for( ulFifoNum = ulFifoStart; ulFifoNum < ulFifoEnd; ulFifoNum++ )
	{
		ptPfifoArea->aulPfifo_border[ulFifoNum] = (ulFifoNum * FIFO_ENTRIES) + FIFO_ENTRIES - 1;
	}

	/* clear reset bit for all pointer FIFO */
	ulValue  = ptPfifoArea->ulPfifo_reset;
	ulValue &= ~(((1U<<NUM_FIFO_CHANNELS_PER_UNIT)-1U) << (NUM_FIFO_CHANNELS_PER_UNIT * uiXcPortNo));
	ptPfifoArea->ulPfifo_reset = ulValue;

	/*** fill empty pointer FIFO ***/

	/* first DWORD in segment 0 is hard wired + IRQ vectors, so it cannot be used */
	ulEmptyPtrCnt = (INTRAM_SEGMENT_SIZE / ETH_FRAME_BUF_SIZE) - 1;

	/* each port has it's own internal ram bank */
	ulFifoPtr |= (uPortNo << SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM);

	/* fill the empty pointer FIFO */
	for( ulFrame = 1; ulFrame <= ulEmptyPtrCnt; ulFrame++ )
	{
		ulFifoPtr &= ~MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
		ulFifoPtr |= (ulFrame << SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM);
		ptPfifoArea->aulPfifo[ulFifoStart + ETHERNET_FIFO_EMPTY] = ulFifoPtr;
	}
}



static void reset_phy(void)
{
	HOSTDEF(ptXcExternalConfigArea);
	unsigned long ulValue;


	/* Reset the internal PHY. */
	ulValue  = g_t_romloader_options.t_ethernet.ulPhyControl;
	ulValue |= HOSTMSK(phy_control_phy_reset);
	ptXcExternalConfigArea->ulPhy_control = ulValue;

	/* Wait 100us. */
	cr7_global_timer_delay_us(100U);

	/* Remove the reset. */
	ulValue &= ~HOSTMSK(phy_control_phy_reset);
	ptXcExternalConfigArea->ulPhy_control = ulValue;
}



static void configure_mode(void)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;


	/* Disable LVDS connections. */
	ulValue = ptAsicCtrlArea->ulIo_config;
	ulValue &= ~(HOSTMSK(io_config_sel_lvds2mii0) | HOSTMSK(io_config_sel_lvds2mii1));
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulIo_config = ulValue;

	/* Use the internal PHYs. */
	ulValue = ptAsicCtrlArea->ulIo_config2;
	ulValue &= ~(HOSTMSK(io_config2_sel_xm00_mii) |
	             HOSTMSK(io_config2_sel_xm01_mii) |
	             HOSTMSK(io_config2_sel_xm10_mii) |
	             HOSTMSK(io_config2_sel_xm11_mii) |
	             HOSTMSK(io_config2_sel_eth_mii));
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulIo_config2 = ulValue;

	reset_phy();
}



static void configure_mode_lvds(NETWORK_DRIVER_T *ptNetworkDriver)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;


	/* Select the connection from XC0 to the LVDS port. */
	ulValue = ptAsicCtrlArea->ulIo_config;
	ulValue &= ~(HOSTMSK(io_config_sel_lvds2mii0) | HOSTMSK(io_config_sel_lvds2mii1));
	switch( ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiEthPortNr )
	{
	case 0:
		ulValue |= 1 << HOSTSRT(io_config_sel_lvds2mii0);
		break;
	case 1:
		ulValue |= 1 << HOSTSRT(io_config_sel_lvds2mii1);
		break;
	}
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulIo_config = ulValue;

	/* Disable external MII pins for all channels. */
	ulValue = ptAsicCtrlArea->ulIo_config2;
	ulValue &= ~(HOSTMSK(io_config2_sel_xm00_mii) |
	             HOSTMSK(io_config2_sel_xm01_mii) |
	             HOSTMSK(io_config2_sel_xm10_mii) |
	             HOSTMSK(io_config2_sel_xm11_mii) |
	             HOSTMSK(io_config2_sel_eth_mii));
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulIo_config2 = ulValue;

	reset_phy();
}



static int eth_initialize(unsigned int uiPortNo)
{
	HOSTDEF(ptXpecIrqRegistersArea);
	ETHMAC_XPEC_DPM *ptEthStdMac;
	NX4000_PHY_CTRL_AREA_T *ptPhyCtrl;
	NX4000_SYSTIME_AREA_T *ptSystime;
	int iResult;
	unsigned int uXcInst;
	unsigned int uXcPortNo;


	uXcInst = (uiPortNo>>1)&1;
	uXcPortNo = (uiPortNo)&1;

	/* initialize pointer to xPEC DRAM */
	ptEthStdMac = aptXpecDramArea[uiPortNo];

	/* reset the hardware block */
	iResult = NX4000_XC_Reset(uXcInst, uXcPortNo);
	if( iResult==0 )
	{
		/* initialize the FIFO unit */
		pfifo_init(uiPortNo);


		/* load the microcode into the hardware block */
		NX4000_XC_Load(paulxMacRpuCodes[uiPortNo]);
		NX4000_XC_Load(paulxMacTpuCodes[uiPortNo]);
		NX4000_XC_Load(paulxPecRpecCodes[uiPortNo]);
		NX4000_XC_Load(paulxPecTpecCodes[uiPortNo]);

		/* disable monitoring mode */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_MONITORING_MODE = 0U;

		/* set default traffic class arrangement */ 
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_TRAFFIC_CLASS_ARRANGEMENT = 4;

		/* copy the current system time border to the copy inside the hardware block */ 
		ptSystime = aptSystime[uXcInst];
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_SYSTIME_BORDER_COPY = ptSystime->ulSystime_border;

		/* Configure the PHY LEDs. */
		ptPhyCtrl = aptPhyCtrl[uiPortNo];
		ptPhyCtrl->ulInt_phy_ctrl_led = ((unsigned long)PHYCTRL_LED_MODE_STATIC)<<SRT_NX4000_int_phy_ctrl_led_mode;

		/* Disable all IRQs. */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_IND_HI = 0;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_IND_LO = 0;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_CON_HI = 0;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_CON_LO = 0;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED = 0;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_COL = 0;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_EARLY_RCV = 0;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_RX_ERR = 0;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_TX_ERR = 0;

		/* Confirm all pending IRQs. */
		if( uXcInst==0 )
		{
			ptXpecIrqRegistersArea->aulXc0_irq_xpec[uXcPortNo] = 0x0000ffffU;
		}
		else
		{
			ptXpecIrqRegistersArea->aulXc1_irq_xpec[uXcPortNo] = 0x0000ffffU;
		}

		/* start XC port */
		NX4000_XC_Start(uXcInst, uXcPortNo);
	}

	return iResult;
}



int drv_eth_xc_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort)
{
	HOSTDEF(ptAsicCtrlArea);
	int iResult;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned long ulMask;
	unsigned long ulValue;


	/* Be pessimistic. */
	iResult = -1;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		/* Get a shortcut to the handle. */
		ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);

		/* Initialize the internal handle. */
		ptHandle->uiEthPortNr = uiPort;
		ptHandle->uiXcUnit = (uiPort >> 1U) & 1U;
		ptHandle->uiXcPort = uiPort & 1U;

		/* Check if all necessary clocks can be enabled. */
		ulMask = HOSTMSK(clock_enable_mask_xc_misc);
		switch(uiPort)
		{
		case 0:
			ulMask |= HOSTMSK(clock_enable_mask_xmac00);
			ulMask |= HOSTMSK(clock_enable_mask_tpec00);
			ulMask |= HOSTMSK(clock_enable_mask_rpec00);
			break;
		case 1:
			ulMask |= HOSTMSK(clock_enable_mask_xmac01);
			ulMask |= HOSTMSK(clock_enable_mask_tpec01);
			ulMask |= HOSTMSK(clock_enable_mask_rpec01);
			break;
		case 2:
			ulMask |= HOSTMSK(clock_enable_mask_xmac10);
			ulMask |= HOSTMSK(clock_enable_mask_tpec10);
			ulMask |= HOSTMSK(clock_enable_mask_rpec10);
			break;
		case 3:
			ulMask |= HOSTMSK(clock_enable_mask_xmac11);
			ulMask |= HOSTMSK(clock_enable_mask_tpec11);
			ulMask |= HOSTMSK(clock_enable_mask_rpec11);
			break;
		}
		ulValue  = ptAsicCtrlArea->ulClock_enable_mask;
		ulValue &= ulMask;
		ulValue ^= ulMask;
		if( ulValue!=0U )
		{
			uprintf("ERROR: unable to activate Ethernet, the clocks are masked out.\n");
		}
		else
		{
			/* Enable the clocks. */
			ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
			ptAsicCtrlArea->ulClock_enable = ulMask;

			/* Initialize the XC. */
			configure_mode();

			eth_initialize(uiPort);

			memcpy(&(ptNetworkDriver->tNetworkIf), &tNetworkIfXc, sizeof(NETWORK_IF_T));

			iResult = 0;
		}
	}

	return iResult;
}



int drv_eth_xc_initialize_lvds(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort)
{
	HOSTDEF(ptAsicCtrlArea);
	int iResult;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned int uiXcUnit;
	unsigned int uiXcPort;
	unsigned long ulMask;
	unsigned long ulValue;


	/* Be pessimistic. */
	iResult = -1;

	/* Check the port number. */
	if( uiPort<=1U )
	{
		/* Get a shortcut to the handle. */
		ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);

		/* Initialize the internal handle. */
		uiXcUnit = (uiPort>>1U)&1;
		uiXcPort = uiPort & 1;

		ptHandle->uiEthPortNr = uiPort;
		ptHandle->uiXcUnit = uiXcUnit;
		ptHandle->uiXcPort = uiXcPort;

		/* Check if all necessary clocks can be enabled. */
		ulMask = HOSTMSK(clock_enable_mask_xc_misc);
		switch(uiPort)
		{
		case 0:
			ulMask |= HOSTMSK(clock_enable_mask_xmac00);
			ulMask |= HOSTMSK(clock_enable_mask_tpec00);
			ulMask |= HOSTMSK(clock_enable_mask_rpec00);
			break;
		case 1:
			ulMask |= HOSTMSK(clock_enable_mask_xmac01);
			ulMask |= HOSTMSK(clock_enable_mask_tpec01);
			ulMask |= HOSTMSK(clock_enable_mask_rpec01);
			break;
		}
		ulValue  = ptAsicCtrlArea->ulClock_enable_mask;
		ulValue &= ulMask;
		ulValue ^= ulMask;
		if( ulValue!=0U )
		{
			uprintf("ERROR: unable to activate Ethernet, the clocks are masked out.\n");
		}
		else
		{
			/* Enable the clocks. */
			ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
			ptAsicCtrlArea->ulClock_enable = ulMask;

			/* Initialize the XC. */
			configure_mode_lvds(ptNetworkDriver);

			eth_initialize(uiPort);

			memcpy(&(ptNetworkDriver->tNetworkIf), &tNetworkIfXcLvds, sizeof(NETWORK_IF_T));

			iResult = 0;
		}
	}

	return iResult;
}
