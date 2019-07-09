#include "driver/netx90/drv_eth_xc.h"

#include <string.h>

#include "netx_io_areas.h"
#include "driver/netx90/ethmac_xpec_regdef.h"
#include "options.h"
#include "phy.h"
#include "phy_std_regs.h"
#include "systime.h"
#include "uprintf.h"


#define ACTIVITY_LED_FLASH_PERIOD 5000000 /* 50ms */

#define ETHERNET_MINIMUM_FRAMELENGTH                    60
#define ETHERNET_MAXIMUM_FRAMELENGTH                    1518

#define NUM_FIFO_CHANNELS_PER_UNIT                      16U      /**< Number of FIFO units per XC channel */
#define FIFO_ENTRIES                                    100U     /**< FIFO depth for each of the 8 FIFOs  */
#define ETH_FRAME_BUF_SIZE                              1560U    /**< size of a frame buffer     */

#define ETHERNET_FIFO_EMPTY                             0      /**< Empty pointer FIFO               */
#define ETHERNET_FIFO_IND_HI                            1      /**< High priority indication FIFO    */
#define ETHERNET_FIFO_IND_LO                            2      /**< Low priority indication FIFO     */
#define ETHERNET_FIFO_REQ_HI                            3      /**< High priority request FIFO       */
#define ETHERNET_FIFO_REQ_LO                            4      /**< Low priority request FIFO        */
#define ETHERNET_FIFO_CON_HI                            5      /**< High priority confirmation FIFO  */
#define ETHERNET_FIFO_CON_LO                            6      /**< Low priority confirmation FIFO   */

/* confirmation error codes */
#define CONF_ERRCODE_TX_SUCCESSFUL_WITHOUT_RETRIES      0x0    /**< Confirmation: Success on first try   */
#define CONF_ERRCODE_TX_SUCCESSFUL_WITH_RETRIES         0x1    /**< Confirmation: Success after some retries */
#define CONF_ERRCODE_TX_FAILED_LATE_COLLISION           0x8    /**< Confirmation: Error late collision */
#define CONF_ERRCODE_TX_FAILED_LINK_DOWN_DURING_TX      0x9    /**< Confirmation: Error link down      */
#define CONF_ERRCODE_TX_FAILED_EXCESSIVE_COLLISION      0xa    /**< Confirmation: Error collision      */
#define CONF_ERRCODE_TX_FAILED_UTX_UFL_DURING_TX        0xb    /**< Confirmation: Error FIFO overflow  */
#define CONF_ERRCODE_TX_FAILED_FATAL_ERROR              0xc    /**< Confirmation: Error Fatal          */


/* This is the Ethernet buffer from the linker description file. */
extern unsigned char aucEthernetBuffer_start[];
extern unsigned char aucEthernetBuffer0_start[];
extern unsigned char aucEthernetBuffer0_end[];
extern unsigned char aucEthernetBuffer1_start[];
extern unsigned char aucEthernetBuffer1_end[];



typedef enum PHYCTRL_LED_MODE_Etag
{
	PHYCTRL_LED_MODE_MANUAL    = 0,
	PHYCTRL_LED_MODE_STATIC    = 1,
	PHYCTRL_LED_MODE_FLASHING  = 2,
	PHYCTRL_LED_MODE_COMBINED  = 3
} PHYCTRL_LED_MODE_E;



static NX90_PHY_CTRL_AREA_T * const aptPhyCtrl[2] =
{
	(NX90_PHY_CTRL_AREA_T*) Addr_NX90_xc0_phy_ctrl0,
	(NX90_PHY_CTRL_AREA_T*) Addr_NX90_xc0_phy_ctrl1,
};



static ETHMAC_XPEC_DPM * const aptXpecDramArea[2] =
{
	(ETHMAC_XPEC_DPM*) Adr_NX90_xc0_tpec0_dram_ram_start,
	(ETHMAC_XPEC_DPM*) Adr_NX90_xc0_tpec1_dram_ram_start
};



static NX90_XMAC_AREA_T * const aptXmacArea[2] =
{
	(NX90_XMAC_AREA_T*) Addr_NX90_xc0_xmac0_regs,
	(NX90_XMAC_AREA_T*) Addr_NX90_xc0_xmac1_regs
};



static NX90_XPEC_AREA_T * const aptRpecRegArea[2] =
{
	(NX90_XPEC_AREA_T*) Addr_NX90_xc0_rpec0_regs,
	(NX90_XPEC_AREA_T*) Addr_NX90_xc0_rpec1_regs
};



static NX90_XPEC_AREA_T * const aptTpecRegArea[2] =
{
	(NX90_XPEC_AREA_T*) Addr_NX90_xc0_tpec0_regs,
	(NX90_XPEC_AREA_T*) Addr_NX90_xc0_tpec1_regs
};


static unsigned long * const apulRpecPramArea[2] =
{
	(unsigned long*) Adr_NX90_xc0_rpec0_pram_ram_start,
	(unsigned long*) Adr_NX90_xc0_rpec1_pram_ram_start
};



static unsigned long * const apulTpecPramArea[2] =
{
	(unsigned long*) Adr_NX90_xc0_tpec0_pram_ram_start,
	(unsigned long*) Adr_NX90_xc0_tpec1_pram_ram_start
};


static const unsigned long XcCode_rpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xff110000, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_rpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xff110800, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};
static const unsigned long XcCode_tpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xff110400, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const unsigned long XcCode_tpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xff110c00, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};


static const unsigned long * const paulRpuResetCodes[2]=
{
	XcCode_rpu_reset0,
	XcCode_rpu_reset1
};

static const unsigned long * const paulTpuResetCodes[2]=
{
	XcCode_tpu_reset0,
	XcCode_tpu_reset1
};


#include "driver/netx90/rpec_ethmac_rpec0.h"
#include "driver/netx90/rpec_ethmac_rpec1.h"
#include "driver/netx90/tpec_ethmac_tpec0.h"
#include "driver/netx90/tpec_ethmac_tpec1.h"

#include "driver/netx90/rpu_ethmac0.h"
#include "driver/netx90/rpu_ethmac1.h"

#include "driver/netx90/tpu_ethmac0.h"
#include "driver/netx90/tpu_ethmac1.h"

static const unsigned long *const paulxPecRpecCodes[2] =
{
	XcCode_rpec_ethmac_rpec0,
	XcCode_rpec_ethmac_rpec1
};


static const unsigned long *const paulxPecTpecCodes[4] =
{
	XcCode_tpec_ethmac_tpec0,
	XcCode_tpec_ethmac_tpec1
};


static const unsigned long* const paulxMacRpuCodes[2] =
{
	XcCode_rpu_ethmac0,
	XcCode_rpu_ethmac1
};


static const unsigned long* const paulxMacTpuCodes[2] =
{
	XcCode_tpu_ethmac0,
	XcCode_tpu_ethmac1
};


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/


static int extphy_read(unsigned int uiPhyCtrlInst, unsigned int uiPhy, unsigned int uiReg, unsigned short *pusData)
{
	NX90_PHY_CTRL_AREA_T *ptPhyCtrl;
	int iResult;
	unsigned long ulValue;
	unsigned short usData;


	/* Be pessimistic. */
	iResult = -1;

	if( uiPhyCtrlInst<2U && uiPhy<32U && uiReg<32U )
	{
		/* Get the pointer to the PHY control. */
		ptPhyCtrl = aptPhyCtrl[uiPhyCtrlInst];

		ulValue  = HOSTMSK(int_phy_ctrl_miimu_preamble);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_mdc_period);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_rta);
		ulValue |= uiPhy << HOSTSRT(int_phy_ctrl_miimu_phyaddr);
		ulValue |= uiReg << HOSTSRT(int_phy_ctrl_miimu_regaddr);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_snrdy);
		ptPhyCtrl->ulInt_phy_ctrl_miimu = ulValue;

		/* FIXME: add a timeout here. */
		do
		{
			ulValue  = ptPhyCtrl->ulInt_phy_ctrl_miimu;
		} while( (ulValue & HOSTMSK(int_phy_ctrl_miimu_snrdy))!=0 );

		iResult = 0;
		usData = (unsigned short)((ulValue & HOSTMSK(int_phy_ctrl_miimu_data)) >> HOSTSRT(int_phy_ctrl_miimu_data));
		if( pusData!=NULL )
		{
			*pusData = usData;
		}
	}

	return iResult;
}



static void NX90_XC_Load(const unsigned long* pulXcPrg)
{
	volatile unsigned long *pulDst, *pulDstCnt;
	const unsigned long *pulSrcStart, *pulSrcCnt, *pulSrcEnd;
	unsigned int uiElements;


	/* Get the number of code elements. */
	uiElements = pulXcPrg[0] / sizeof(unsigned long) - 1;
  
	/* Get the pointer in the XC area. */
	pulDst = (volatile unsigned long*) pulXcPrg[2];
  
	/* Get source start and end pointer. */
	pulSrcStart = pulXcPrg + 3;
	pulSrcEnd = pulSrcStart + uiElements;
	
	/* Copy the code to the XC RAM. */
	pulSrcCnt = pulSrcStart;
	pulDstCnt = pulDst;
	while( pulSrcCnt<pulSrcEnd )
	{
		*pulDstCnt = *pulSrcCnt;
		pulDstCnt++;
		pulSrcCnt++;
	}
  
	/* Get the number of trailing loads. */
	uiElements = pulXcPrg[1] / sizeof(unsigned long);
  
	/* Get source start and end pointer. */
	pulSrcCnt = pulXcPrg + 2 + pulXcPrg[0] / sizeof(unsigned long);
	pulSrcEnd = pulSrcCnt + uiElements;
  
	/* Write all trailing loads. */
	while( pulSrcCnt<pulSrcEnd )
	{
		/* Get the destination address. */
		pulDst = (volatile unsigned long*) *pulSrcCnt;
		pulSrcCnt++;
    
		/* write the data. */
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
static int NX90_XC_Reset(unsigned int uiPortNr)
{
	HOSTDEF(ptXcStartStopArea);
	HOSTDEF(ptXpecIrqRegistersArea);
	NX90_XMAC_AREA_T* ptXmac;
	NX90_XPEC_AREA_T* ptRpec;
	NX90_XPEC_AREA_T* ptTpec;
	volatile unsigned long* pulRpecPram;
	volatile unsigned long* pulTpecPram;
	unsigned int uIdx;
	unsigned long ulValue;
  

	ptXmac      = aptXmacArea[uiPortNr];
	ptRpec      = aptRpecRegArea[uiPortNr];
	ptTpec      = aptTpecRegArea[uiPortNr];
	pulRpecPram = apulRpecPramArea[uiPortNr];
	pulTpecPram = apulTpecPramArea[uiPortNr];

	/* Stop xPEC and xMAC */
	if( uiPortNr==0 )
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_stop_rpec0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_tpec0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_rpu0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_tpu0);
	}
	else
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_stop_rpec1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_tpec1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_rpu1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_tpu1);
	}
	ptXcStartStopArea->ulXc_start_stop_ctrl = ulValue;

	/* Clear output enable of TX as soon as possible. */
	ptXmac->ulXmac_config_obu  = HOSTDFLT(xmac_config_obu);
	/* Clear output enable of io0..5 as soon as possible. */
	ptRpec->aulStatcfg[uiPortNr] = 0xffff0000;

	if( uiPortNr==0 )
	{
		ulValue  = HOSTMSK(xpec_config_reset_urx_fifo0);
		ulValue |= HOSTMSK(xpec_config_reset_utx_fifo0);
	}
	else
	{
		ulValue  = HOSTMSK(xpec_config_reset_urx_fifo1);
		ulValue |= HOSTMSK(xpec_config_reset_utx_fifo1);
	}
	ptRpec->ulXpec_config = ulValue;
  
	/* Load the rate multiplier reset code. */
	NX90_XC_Load(paulRpuResetCodes[uiPortNr]);
	NX90_XC_Load(paulTpuResetCodes[uiPortNr]);
  
	/* Reset the PC of the RPU and TPU unit to 0 */
	ptXmac->ulXmac_rpu_pc = 0;
	ptXmac->ulXmac_tpu_pc = 0;

	if( uiPortNr==0 )
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_start_rpu0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_tpu0);
	}
	else
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_start_rpu1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_tpu1);
	}
	ptXcStartStopArea->ulXc_start_stop_ctrl = ulValue;

	/* !!!! ATTENTION: There must be enough time between starting xMAC and stopping xMAC to execute reset program */

	pulRpecPram[0] = 0xC0000FFF;                                   /* Use the command wait b000000000000,b111111111111 at Address 0*/
	pulTpecPram[0] = 0xC0000FFF;                                   /* Use the command wait b000000000000,b111111111111 at Address 0*/

	ptRpec->ulXpec_pc = 0x7ff;                                     /* Reset the Program Counter to 0x7ff */
	ptTpec->ulXpec_pc = 0x7ff;                                     /* Reset the Program Counter to 0x7ff */


	/* let the XC run for at least 10 cycles */
	if( uiPortNr==0 )
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_start_rpec0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_tpec0);
	}
	else
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_start_rpec1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_tpec1);
	}
	for(uIdx=0; uIdx<10; ++uIdx)
	{
		ptXcStartStopArea->ulXc_start_stop_ctrl = ulValue;
	}

	if( uiPortNr==0 )
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_stop_rpec0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_tpec0);
	}
	else
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_stop_rpec1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_tpec1);
	}
	ptXcStartStopArea->ulXc_start_stop_ctrl = ulValue;
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

	/* Note regarding stat_bits_shared: this register is used for XC instance crossover protocols, protocol has to clear this register when initializing */

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

	/* Reset shared registers. */
	if( uiPortNr==0 )
	{
		/* Reset SR0-3. */
		ptRpec->aulXpec_sr[0] = 0;
		ptRpec->aulXpec_sr[1] = 0;
		ptRpec->aulXpec_sr[2] = 0;
		ptRpec->aulXpec_sr[3] = 0;

		/* Reset SR8-11. */
		ptRpec->aulXpec_sr[8] = 0;
		ptRpec->aulXpec_sr[9] = 0;
		ptRpec->aulXpec_sr[10] = 0;
		ptRpec->aulXpec_sr[11] = 0;
	}
	else
	{
		/* Reset SR4-7. */
		ptRpec->aulXpec_sr[4] = 0;
		ptRpec->aulXpec_sr[5] = 0;
		ptRpec->aulXpec_sr[6] = 0;
		ptRpec->aulXpec_sr[7] = 0;

		/* Reset SR12-15. */
		ptRpec->aulXpec_sr[12] = 0;
		ptRpec->aulXpec_sr[13] = 0;
		ptRpec->aulXpec_sr[14] = 0;
		ptRpec->aulXpec_sr[15] = 0;
	}
    
	ptRpec->ulDatach_wr_cfg = 0;
	ptTpec->ulDatach_wr_cfg = 0;
	ptRpec->ulDatach_rd_cfg = 0;
	ptTpec->ulDatach_rd_cfg = 0;
	ptRpec->ulSysch_addr    = 0;
	ptTpec->ulSysch_addr    = 0;

	/* confirm all interrupts from xPEC -> Select the specific XC instance and get IRQ */
	ptXpecIrqRegistersArea->aulXc0_irq_xpec[uiPortNr] = 0x0000FFFF;
  
	/* hold xMAC */
	if( uiPortNr==0 )
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_stop_rpu0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_tpu0);
	}
	else
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_stop_rpu1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_stop_tpu1);
	}
	ptXcStartStopArea->ulXc_start_stop_ctrl = ulValue;
      
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
	ptXmac->ulXmac_config_rx_nibble_fifo  = HOSTDFLT(xmac_config_rx_nibble_fifo);
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
	
	ptXmac->ulXmac_config_sbu2         = HOSTDFLT(xmac_config_sbu2);
	ptXmac->ulXmac_config_obu2         = HOSTDFLT(xmac_config_obu2);
	
	ptXmac->ulXmac_rpu_pc              = 0;
	ptXmac->ulXmac_tpu_pc              = 0;
	
	return 0;
}



static void NX90_XC_Start(unsigned int uiPortNr)
{
	HOSTDEF(ptXcStartStopArea);
	NX90_XMAC_AREA_T* ptXmacArea;
	NX90_XPEC_AREA_T* ptRpecArea;
	NX90_XPEC_AREA_T* ptTpecArea;
	unsigned long ulValue;


	ptXmacArea = aptXmacArea[uiPortNr];
	ptRpecArea = aptRpecRegArea[uiPortNr];
	ptTpecArea = aptTpecRegArea[uiPortNr];

	/* Reset the PC of all units. */
	ptXmacArea->ulXmac_rpu_pc = 0;
	ptXmacArea->ulXmac_tpu_pc = 0;
	ptRpecArea->ulXpec_pc     = 0x7ff;  
	ptTpecArea->ulXpec_pc     = 0x7ff;  

	/* Start the units. */
	if( uiPortNr==0 )
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_start_rpec0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_tpec0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_rpu0);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_tpu0);
	}
	else
	{
		ulValue  = HOSTMSK(xc_start_stop_ctrl_xc0_start_rpec1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_tpec1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_rpu1);
		ulValue |= HOSTMSK(xc_start_stop_ctrl_xc0_start_tpu1);
	}
	ptXcStartStopArea->ulXc_start_stop_ctrl = ulValue;
}










static void *convert_fifo_value_to_packet_pointer(unsigned long ulFifoValue)
{
	unsigned long ulFrameNr;
	unsigned char *pucAddress;
	void *pvPacket;


	/* Extract the RAM bank and frame number. */
	ulFrameNr   = ulFifoValue;
	ulFrameNr  &= MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
	ulFrameNr >>= SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;

	pucAddress  = aucEthernetBuffer_start;
	pucAddress += ulFrameNr * ETH_FRAME_BUF_SIZE;
	pvPacket = (void*)pucAddress;

	return pvPacket;
}



/* Convert a frame address to a FIFO pointer. */
static unsigned long convert_packet_pointer_to_fifo_value(void *pvPacket)
{       
        unsigned long ulFrameNr;
        unsigned long ulValue;

        
	/* Extract the frame buffer number. */
	ulFrameNr  = (unsigned long)pvPacket;
	ulFrameNr &= 0xffffU;
	ulFrameNr /= ETH_FRAME_BUF_SIZE;

	ulValue = ulFrameNr << SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;

        return ulValue;
}



/*-----------------------------------------------------------------------------------------------------------*/


static unsigned int drv_eth_xc_get_link_status(NETWORK_DRIVER_T *ptNetworkDriver)
{
	NX90_PHY_CTRL_AREA_T *ptPhyCtrl;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned long ulValue;
	int iResult;
	unsigned short usValue;
	unsigned int uiLinkStatus;
	unsigned int uiPort;


	uiLinkStatus = 0;

	ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);
	uiPort = ptHandle->uiEthPortNr;
	if( uiPort<2U )
	{
		/* retrieve the link status from the Ethernet port */
		ptPhyCtrl = aptPhyCtrl[ptHandle->uiXcPort];
		ulValue  = ptPhyCtrl->ulInt_phy_ctrl_led;
		ulValue &= HOSTMSK(int_phy_ctrl_led_link_ro);
		if( ulValue!=0 )
		{
			uiLinkStatus = 1;
		}
	}
	else if( uiPort<4U )
	{
		iResult = extphy_read(ptHandle->auiExtPhyCtrlInst[uiPort-2U], ptHandle->auiExtPhyAddress[uiPort-2U], PHY_REG_STATUS, &usValue);
		if( iResult==0 )
		{
			usValue &= MSK_PHY_STATUS_LINK_UP;
			if( usValue!=0 )
			{
				uiLinkStatus = 1;
			}
		}
	}
	
	return uiLinkStatus;
}



static void *drv_eth_xc_get_empty_packet(NETWORK_DRIVER_T *ptNetworkDriver)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned long ulFifoValue;
	unsigned int uiFifoNr; 
	unsigned long ulValue;
	void *pvPacket;


	/* Expect no free packet. */
	pvPacket = NULL;

	/* Get the FIFO fill level and check if there is at least one element. */
	uiFifoNr = (ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_EMPTY;

	/* Keep at least one pointer for the XC level (two parties share this empty pointer FIFO). */
	ulValue = ptPointerFifoArea->aulPfifo_fill_level[uiFifoNr];
	if( ulValue>1U )
	{
		/* Get the FIFO element. */
		ulFifoValue = ptPointerFifoArea->aulPfifo[uiFifoNr];
		pvPacket = convert_fifo_value_to_packet_pointer(ulFifoValue);
	}

	return pvPacket;
}



static void drv_eth_xc_release_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned int uiFifoNr; 
	unsigned long ulFifoValue;


	/* Convert the pointer to a FIFO value. */
	ulFifoValue = convert_packet_pointer_to_fifo_value(pvPacket);

	uiFifoNr = (ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_EMPTY;
	ptPointerFifoArea->aulPfifo[uiFifoNr] = ulFifoValue;
}



static void drv_eth_xc_send_packet(NETWORK_DRIVER_T *ptNetworkDriver, void *pvPacket, unsigned int uiPacketSize)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned long ulFifoValue;
	unsigned int uiFifoNr;


	/* Pad too short packets. */
	if( uiPacketSize<ETHERNET_MINIMUM_FRAMELENGTH )
	{
		uiPacketSize = ETHERNET_MINIMUM_FRAMELENGTH;
	}
	/* Crop over-sized packets. */
	else if( uiPacketSize>ETHERNET_MAXIMUM_FRAMELENGTH )
	{
		uiPacketSize = ETHERNET_MAXIMUM_FRAMELENGTH;
	}
	
	/* Convert the pointer to a FIFO value. */
	ulFifoValue  = convert_packet_pointer_to_fifo_value(pvPacket);
	/* Do not send a confirmation. */
	ulFifoValue |= MSK_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON;
	/* Add the size information. */
	ulFifoValue |= uiPacketSize << SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN;

	uiFifoNr = (ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_REQ_LO;
	ptPointerFifoArea->aulPfifo[uiFifoNr] = ulFifoValue;
}



static void *drv_eth_xc_get_received_packet(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int *puiPacketSize)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned long ulFillLevel;
	unsigned int uiFifoNr;
	void *pvPacket;
	unsigned long ulFifoValue;


	/* Expect no packet. */
	pvPacket = NULL;

	uiFifoNr = (ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle.uiXcPort * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_IND_LO;
	ulFillLevel = ptPointerFifoArea->aulPfifo_fill_level[uiFifoNr];
	if( ulFillLevel!=0 )
	{
		ulFifoValue = ptPointerFifoArea->aulPfifo[uiFifoNr];
		pvPacket = convert_fifo_value_to_packet_pointer(ulFifoValue);

		ulFifoValue  &= MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN;
		ulFifoValue >>= SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN;
		*puiPacketSize = (unsigned int)ulFifoValue;
	}

	return pvPacket;
}



static void drv_eth_xc_deactivate(NETWORK_DRIVER_T *ptNetworkDriver __attribute__((unused)))
{
	/* TODO: deactivate all. */
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
	HOSTDEF(ptPointerFifoArea);
	unsigned int uiCnt;
	unsigned long ulBorderCnt;
	

	/* Reset all pointer FIFOs. */
	ptPointerFifoArea->ulPfifo_reset = 0xffffffffU;

	/* Reset the pointer FIFO borders. */
	ulBorderCnt = FIFO_ENTRIES - 1U;
	for(uiCnt=0; uiCnt<(sizeof(ptPointerFifoArea->aulPfifo_border)/sizeof(unsigned long)); ++uiCnt)
	{
		ptPointerFifoArea->aulPfifo_border[uiCnt] = ulBorderCnt;

		ulBorderCnt += FIFO_ENTRIES;
	}
	
	/* Clear the reset flag of all FIFOs. */
	ptPointerFifoArea->ulPfifo_reset = 0;
}



static void pfifo_init(unsigned int uiPortNo)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned int uiXcPort;
	unsigned long ulFifoStart;
	unsigned char *pucStart;
	unsigned char *pucEnd;
	unsigned char *pucCnt;
	unsigned long ulFrameNumber;
	unsigned long ulFifoPtr;


	uiXcPort = uiPortNo & 1U;
	ulFifoStart = NUM_FIFO_CHANNELS_PER_UNIT * uiXcPort;

	/*** fill empty pointer FIFO ***/
	if( uiXcPort==0 )
	{
		pucStart = aucEthernetBuffer0_start;
		pucEnd = aucEthernetBuffer0_end;
	}
	else
	{
		pucStart = aucEthernetBuffer1_start;
		pucEnd = aucEthernetBuffer1_end;
	}

	/* Get the first frame number. */
	ulFrameNumber = 0U;
	pucCnt = aucEthernetBuffer_start;
	while( pucCnt<pucStart )
	{
		pucCnt += ETH_FRAME_BUF_SIZE;
		++ulFrameNumber;
	}

	while( pucCnt<pucEnd )
	{
		/* Is the pointer still in the defined area? */
		if( (pucCnt+ETH_FRAME_BUF_SIZE)<=pucEnd )
		{
			/* Use INTRAM3. */
			ulFifoPtr  = (0U << SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM);
			ulFifoPtr |= (ulFrameNumber << SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM);
			ptPointerFifoArea->aulPfifo[ulFifoStart + ETHERNET_FIFO_EMPTY] = ulFifoPtr;
		}
		pucCnt += ETH_FRAME_BUF_SIZE;
		++ulFrameNumber;
	}
}



static int intphy_reset(void)
{
	int iResult;
	const unsigned char *pucMacro;


	/* Get a pointer to the macro data. */
	pucMacro = g_t_romloader_options.t_ethernet.tPhyMacroIntern.aucMacro;

	/* There is no default for the initialization of the internal PHY.
	 * An empty macro is an error.
	 */
	if( *pucMacro==0x00U )
	{
		iResult = -1;
	}
	else
	{
		iResult = phy_setup_execute_sequence(pucMacro, sizeof(PHY_MACRO_T));
	}

	return iResult;
}



static int extphy_reset(void)
{
	const unsigned char *pucMacro;
	int iResult;


	/* Get a pointer to the macro data. */
	pucMacro = g_t_romloader_options.t_ethernet.tPhyMacroExtern.aucMacro;

	/* Use the standard setup if the macro is empty.
	 * A macro starting with a "0" is empty.
	 */
	if( *pucMacro==0x00U )
	{
		iResult = -1;
	}
	else
	{
		iResult = phy_setup_execute_sequence(pucMacro, sizeof(PHY_MACRO_T));
	}

	return iResult;
}



void setup_phy_internal(void)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;
	unsigned long ulXmMiiCfg;
	unsigned long ulXcMdioCfg;
	unsigned long ulPhyCtrl;


	/* Connect to internal PHY. */
	ulXmMiiCfg = 8U;
	ulXcMdioCfg = 2U;
	ulPhyCtrl = HOSTDFLT(phy_ctrl0);

	/* Disable LVDS connections. */
	ulValue  = ulXmMiiCfg << HOSTSRT(io_config0_sel_xm0_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config0_sel_xc0_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xc0_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[0].ulConfig = ulValue;

	ulValue  = ulXmMiiCfg << HOSTSRT(io_config1_sel_xm1_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config1_sel_xc1_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xc1_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[1].ulConfig = ulValue;

	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulPhy_ctrl0 = ulPhyCtrl;

	intphy_reset();
}



void setup_phy_external(void)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;
	unsigned long ulXmMiiCfg;
	unsigned long ulXcMdioCfg;
	unsigned long ulPhyCtrl;


	/* Connect to external PHY. */
	ulXmMiiCfg = 6U;
	ulXcMdioCfg = 1U;
	ulPhyCtrl  = 4U << HOSTSRT(phy_ctrl0_phy0_led_invert);
	ulPhyCtrl |= 4U << HOSTSRT(phy_ctrl0_phy1_led_invert);

	/* Disable LVDS connections. */
	ulValue  = ulXmMiiCfg << HOSTSRT(io_config0_sel_xm0_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config0_sel_xc0_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xc0_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[0].ulConfig = ulValue;

	ulValue  = ulXmMiiCfg << HOSTSRT(io_config1_sel_xm1_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config1_sel_xc1_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xc1_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[1].ulConfig = ulValue;

	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulPhy_ctrl0 = ulPhyCtrl;

	ulValue  = ptAsicCtrlArea->asIo_config[2].ulConfig;
	ulValue |= HOSTMSK(io_config2_clk25out_oe);
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[2].ulConfig = ulValue;

	extphy_reset();
}



void setup_phy_lvds(void)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;


	/* Enable LVDS connections. */
	ulValue  = 9U << HOSTSRT(io_config0_sel_xm0_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xm0_mii_cfg_wm);
#endif
	ulValue |= 2U << HOSTSRT(io_config0_sel_xc0_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config0_sel_xc0_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[0].ulConfig = ulValue;

	ulValue  = 9U << HOSTSRT(io_config1_sel_xm1_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xm1_mii_cfg_wm);
#endif
	ulValue |= 2U << HOSTSRT(io_config1_sel_xc1_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90
	ulValue |= HOSTMSK(io_config1_sel_xc1_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[1].ulConfig = ulValue;

	intphy_reset();
}



static int eth_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPortNr)
{
	HOSTDEF(ptXpecIrqRegistersArea);
	HOSTDEF(ptSystimeComArea);
	ETHMAC_XPEC_DPM *ptEthStdMac;
	NX90_PHY_CTRL_AREA_T *ptPhyCtrl;
	NX90_XMAC_AREA_T *ptXmac;
	int iResult;
	unsigned int uiCnt;
	unsigned long ulValue;
	unsigned int uiXcPort;
	unsigned char *pucMAC;


	/* Set the systime. */
	ptSystimeComArea->ulSystime_border = 1000000000U - 1U;
	ptSystimeComArea->ulSystime_count_value = 0xa0000000U;

	uiXcPort = uiPortNr & 1U;

	/* initialize pointer to xPEC DRAM */
	ptEthStdMac = aptXpecDramArea[uiXcPort];

	/* reset the hardware block */
	iResult = NX90_XC_Reset(uiXcPort);
	if( iResult==0 )
	{
		/* initialize the FIFO unit */
		pfifo_init(uiXcPort);

		/* load the microcode into the hardware block */
		NX90_XC_Load(paulxMacRpuCodes[uiXcPort]);
		NX90_XC_Load(paulxMacTpuCodes[uiXcPort]);
		NX90_XC_Load(paulxPecRpecCodes[uiXcPort]);
		NX90_XC_Load(paulxPecTpecCodes[uiXcPort]);

		/* Disable monitoring mode. */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_MONITORING_MODE = 0U;

		/* Set default traffic class arrangement. */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_TRAFFIC_CLASS_ARRANGEMENT = 4;

		/* Copy the current system time border to the copy inside the hardware block. */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_SYSTIME_BORDER_COPY_PLUS1 = ptSystimeComArea->ulSystime_border + 1;

		/* Configure the PHY LEDs. */
		ptPhyCtrl = aptPhyCtrl[uiXcPort];
		ptPhyCtrl->ulInt_phy_ctrl_led = ((unsigned long)PHYCTRL_LED_MODE_STATIC)<<HOSTSRT(int_phy_ctrl_led_mode);

		/* Enable all Multicast addresses */
		for(uiCnt=0; uiCnt<(sizeof(ptEthStdMac->tETHMAC_MULTICAST_FILTER_TABLE.aulDataField)/sizeof(unsigned long)); ++uiCnt)
		{
			ptEthStdMac->tETHMAC_MULTICAST_FILTER_TABLE.aulDataField[uiCnt] = 0xffffffffU;
		}

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
		ptXpecIrqRegistersArea->aulXc0_irq_xpec[uiPortNr] = 0x0000ffffU;

		/* configure MII timing */
		ptXmac = aptXmacArea[uiPortNr];
		ulValue  = ptXmac->ulXmac_config_tx_nibble_fifo;
		ulValue &= ~HOSTMSK(xmac_config_tx_nibble_fifo_output_phase);
		ulValue |= 0 << HOSTSRT(xmac_config_tx_nibble_fifo_output_phase);
		ptXmac->ulXmac_config_tx_nibble_fifo = ulValue;

		/* update correction base values */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_TS_COR_BASE_TX = 15;

		/* Set the MAC address. */
		pucMAC = ptNetworkDriver->tEthernetPortCfg.aucMac;
		ulValue  =                  pucMAC[0];
		ulValue |= ((unsigned long)(pucMAC[1])) <<  8U;
		ulValue |= ((unsigned long)(pucMAC[2])) << 16U;
		ulValue |= ((unsigned long)(pucMAC[3])) << 24U;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERFACE_MAC_ADDRESS_LO = ulValue;

		ulValue  =                  pucMAC[4];
		ulValue |= ((unsigned long)(pucMAC[5])) << 8U;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERFACE_MAC_ADDRESS_HI = ulValue;

		/* start XC port */
		NX90_XC_Start(uiXcPort);
	}

	return iResult;
}



int drv_eth_xc_initialize(NETWORK_DRIVER_T *ptNetworkDriver, unsigned int uiPort)
{
	HOSTDEF(ptAsicCtrlArea);
	int iResult;
	DRV_ETH_XC_HANDLE_T *ptHandle;
	unsigned long ulMask;
	unsigned long ulEnable;
	unsigned long ulValue;
	unsigned int uiXcPort;


	/* Be pessimistic. */
	iResult = -1;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		/* Get a shortcut to the handle. */
		ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);

		uiXcPort = uiPort & 1;

		/* Initialize the internal handle. */
		ptHandle->uiEthPortNr = uiPort;
		ptHandle->uiXcPort = uiXcPort;
		ptHandle->auiExtPhyCtrlInst[0] = 0;
		ptHandle->auiExtPhyCtrlInst[1] = 0;
		ptHandle->auiExtPhyAddress[0] = 0;
		ptHandle->auiExtPhyAddress[1] = 0;

		/* Check if all necessary clocks can be enabled. */
		ulMask = HOSTMSK(clock_enable0_mask_xc_misc);
		ulEnable  = HOSTMSK(clock_enable0_xc_misc);
#if ASIC_TYP==ASIC_TYP_NETX90
		ulEnable |= HOSTMSK(clock_enable0_xc_misc_wm);
#endif
		if( uiXcPort==0U )
		{
			ulMask |= HOSTMSK(clock_enable0_mask_xmac0);
			ulMask |= HOSTMSK(clock_enable0_mask_tpec0);
			ulMask |= HOSTMSK(clock_enable0_mask_rpec0);

			ulEnable |= HOSTMSK(clock_enable0_xmac0);
			ulEnable |= HOSTMSK(clock_enable0_tpec0);
			ulEnable |= HOSTMSK(clock_enable0_rpec0);
#if ASIC_TYP==ASIC_TYP_NETX90
			ulEnable |= HOSTMSK(clock_enable0_xmac0_wm);
			ulEnable |= HOSTMSK(clock_enable0_tpec0_wm);
			ulEnable |= HOSTMSK(clock_enable0_rpec0_wm);
#endif
		}
		else
		{
			ulMask |= HOSTMSK(clock_enable0_mask_xmac1);
			ulMask |= HOSTMSK(clock_enable0_mask_tpec1);
			ulMask |= HOSTMSK(clock_enable0_mask_rpec1);

			ulEnable |= HOSTMSK(clock_enable0_xmac1);
			ulEnable |= HOSTMSK(clock_enable0_tpec1);
			ulEnable |= HOSTMSK(clock_enable0_rpec1);
#if ASIC_TYP==ASIC_TYP_NETX90
			ulEnable |= HOSTMSK(clock_enable0_xmac1_wm);
			ulEnable |= HOSTMSK(clock_enable0_tpec1_wm);
			ulEnable |= HOSTMSK(clock_enable0_rpec1_wm);
#endif
		}
		ulValue  = ptAsicCtrlArea->asClock_enable[0].ulMask;
		ulValue &= ulMask;
		ulValue ^= ulMask;
		if( ulValue!=0U )
		{
			uprintf("The Ethernet clocks are masked out.\n");
		}
		else
		{
			/* Enable the clocks. */
			ulValue  = ptAsicCtrlArea->asClock_enable[0].ulEnable;
			ulValue |= ulEnable;
			ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
			ptAsicCtrlArea->asClock_enable[0].ulEnable = ulValue;

//			setup_phy_internal();

			eth_initialize(ptNetworkDriver, uiPort);

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
	unsigned long ulMask;
	unsigned long ulEnable;
	unsigned long ulValue;
	unsigned int uiXcPort;
#if 0
	const unsigned short *pusPortcontrol_Index;
	unsigned short *pusPortControl_Values;
	size_t sizPortControl_Values;
#endif

	/* Be pessimistic. */
	iResult = -1;

	/* Check the port number. */
	if( uiPort<=1U )
	{
		/* Get a shortcut to the handle. */
		ptHandle = &(ptNetworkDriver->tNetworkDriverData.tDrvEthXcHandle);

		uiXcPort = uiPort & 1;

		/* Initialize the internal handle. */
		ptHandle->uiEthPortNr = uiPort;
		ptHandle->uiXcPort = uiXcPort;
		ptHandle->auiExtPhyCtrlInst[0] = 0;
		ptHandle->auiExtPhyCtrlInst[1] = 0;
		ptHandle->auiExtPhyAddress[0] = 0;
		ptHandle->auiExtPhyAddress[1] = 0;

		/* Check if all necessary clocks can be enabled. */
		ulMask = HOSTMSK(clock_enable0_mask_xc_misc);
		ulEnable  = HOSTMSK(clock_enable0_xc_misc);
#if ASIC_TYP==ASIC_TYP_NETX90
		ulEnable |= HOSTMSK(clock_enable0_xc_misc_wm);
#endif
		switch(uiPort)
		{
		case 0:
			ulMask |= HOSTMSK(clock_enable0_mask_xmac0);
			ulMask |= HOSTMSK(clock_enable0_mask_tpec0);
			ulMask |= HOSTMSK(clock_enable0_mask_rpec0);

			ulEnable |= HOSTMSK(clock_enable0_xmac0);
			ulEnable |= HOSTMSK(clock_enable0_tpec0);
			ulEnable |= HOSTMSK(clock_enable0_rpec0);
#if ASIC_TYP==ASIC_TYP_NETX90
			ulEnable |= HOSTMSK(clock_enable0_xmac0_wm);
			ulEnable |= HOSTMSK(clock_enable0_tpec0_wm);
			ulEnable |= HOSTMSK(clock_enable0_rpec0_wm);
#endif
			break;
		case 1:
			ulMask |= HOSTMSK(clock_enable0_mask_xmac1);
			ulMask |= HOSTMSK(clock_enable0_mask_tpec1);
			ulMask |= HOSTMSK(clock_enable0_mask_rpec1);

			ulEnable |= HOSTMSK(clock_enable0_xmac1);
			ulEnable |= HOSTMSK(clock_enable0_tpec1);
			ulEnable |= HOSTMSK(clock_enable0_rpec1);
#if ASIC_TYP==ASIC_TYP_NETX90
			ulEnable |= HOSTMSK(clock_enable0_xmac1_wm);
			ulEnable |= HOSTMSK(clock_enable0_tpec1_wm);
			ulEnable |= HOSTMSK(clock_enable0_rpec1_wm);
#endif
			break;
		}
		ulValue  = ptAsicCtrlArea->asClock_enable[0].ulMask;
		ulValue &= ulMask;
		ulValue ^= ulMask;
		if( ulValue!=0U )
		{
			uprintf("The Ethernet clocks are masked out.\n");
		}
		else
		{
			/* Enable the clocks. */
			ulValue  = ptAsicCtrlArea->asClock_enable[0].ulEnable;
			ulValue |= ulEnable;
			ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
			ptAsicCtrlArea->asClock_enable[0].ulEnable = ulValue;

			/* Initialize the XC. */
			pfifo_reset();

			eth_initialize(ptNetworkDriver, uiPort);

			memcpy(&(ptNetworkDriver->tNetworkIf), &tNetworkIfXcLvds, sizeof(NETWORK_IF_T));

			iResult = 0;
		}
	}

	return iResult;
}


