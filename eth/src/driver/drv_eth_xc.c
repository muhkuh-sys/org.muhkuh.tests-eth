#include "driver/drv_eth_xc.h"

#include "netx_io_areas.h"
#include "driver/ethmac_xpec_regdef.h"
#include "options.h"
#include "pad_control.h"
#include "phy_std_regs.h"
#include "systime.h"
#include "uprintf.h"


#define ACTIVITY_LED_FLASH_PERIOD 5000000 /* 50ms */

#define ETHERNET_MINIMUM_FRAMELENGTH                    60
#define ETHERNET_MAXIMUM_FRAMELENGTH                    1518

#define NUM_FIFO_CHANNELS_PER_UNIT                      16U      /**< Number of FIFO units per XC channel */
#define FIFO_ENTRIES                                    100U     /**< FIFO depth for each of the 8 FIFOs  */
#define ETH_FRAME_BUF_SIZE                              1560U    /**< size of a frame buffer     */
#define INTRAM_SEGMENT_SIZE                             0x10000U /**< size of the internal ram segments */

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
extern unsigned char aucEthernetBuffer_end[];



typedef enum PHYCTRL_LED_MODE_Etag
{
	PHYCTRL_LED_MODE_MANUAL    = 0,
	PHYCTRL_LED_MODE_STATIC    = 1,
	PHYCTRL_LED_MODE_FLASHING  = 2,
	PHYCTRL_LED_MODE_COMBINED  = 3
} PHYCTRL_LED_MODE_E;



typedef struct STRUCTURE_DRV_ETH_XC_HANDLE
{
	unsigned int uiEthPortNr;       /* The Ethernet port 0-3. */
	unsigned int uiXC;              /* The XC instance 0-1. */
	unsigned int uiExtPhyCtrlInst;
	unsigned int uiExtPhyAddress;
} DRV_ETH_XC_HANDLE_T;


static DRV_ETH_XC_HANDLE_T tHandle;



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


#include "driver/rpec_ethmac_rpec0.h"
#include "driver/rpec_ethmac_rpec1.h"
#include "driver/tpec_ethmac_tpec0.h"
#include "driver/tpec_ethmac_tpec1.h"

#include "driver/rpu_ethmac0.h"
#include "driver/rpu_ethmac1.h"

#include "driver/tpu_ethmac0.h"
#include "driver/tpu_ethmac1.h"

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


static const unsigned char aucPadCtrlRegsExtCommon[4] =
{
	PAD_REG2OFFSET(mii_mdc),
	PAD_REG2OFFSET(mii_mdio),
	PAD_REG2OFFSET(rst_out_n),
	PAD_REG2OFFSET(clk25out)
};


static const unsigned char aucPadCtrlRegsExt0[16] =
{
	PAD_REG2OFFSET(mii0_rxclk),
	PAD_AREG2OFFSET(mii0_rxd, 0),
	PAD_AREG2OFFSET(mii0_rxd, 1),
	PAD_AREG2OFFSET(mii0_rxd, 2),
	PAD_AREG2OFFSET(mii0_rxd, 3),
	PAD_REG2OFFSET(mii0_rxdv),
	PAD_REG2OFFSET(mii0_rxer),
	PAD_REG2OFFSET(mii0_txclk),
	PAD_AREG2OFFSET(mii0_txd, 0),
	PAD_AREG2OFFSET(mii0_txd, 1),
	PAD_AREG2OFFSET(mii0_txd, 2),
	PAD_AREG2OFFSET(mii0_txd, 3),
	PAD_REG2OFFSET(mii0_txen),
	PAD_REG2OFFSET(mii0_col),
	PAD_REG2OFFSET(mii0_crs),
	PAD_REG2OFFSET(phy0_led_link_in)
};


static const unsigned char aucPadCtrlRegsExt1[16] =
{
	PAD_REG2OFFSET(mii1_rxclk),
	PAD_AREG2OFFSET(mii1_rxd, 0),
	PAD_AREG2OFFSET(mii1_rxd, 1),
	PAD_AREG2OFFSET(mii1_rxd, 2),
	PAD_AREG2OFFSET(mii1_rxd, 3),
	PAD_REG2OFFSET(mii1_rxdv),
	PAD_REG2OFFSET(mii1_rxer),
	PAD_REG2OFFSET(mii1_txclk),
	PAD_AREG2OFFSET(mii1_txd, 0),
	PAD_AREG2OFFSET(mii1_txd, 1),
	PAD_AREG2OFFSET(mii1_txd, 2),
	PAD_AREG2OFFSET(mii1_txd, 3),
	PAD_REG2OFFSET(mii1_txen),
	PAD_REG2OFFSET(mii1_col),
	PAD_REG2OFFSET(mii1_crs),
	PAD_REG2OFFSET(phy1_led_link_in)
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



static int extphy_write(unsigned int uiPhyCtrlInst, unsigned int uiPhy, unsigned int uiReg, unsigned short usData)
{
	NX90_PHY_CTRL_AREA_T *ptPhyCtrl;
	int iResult;
	unsigned long ulValue;


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
		ulValue |= ((unsigned long)usData) << HOSTSRT(int_phy_ctrl_miimu_data);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_opmode);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_snrdy);
		ptPhyCtrl->ulInt_phy_ctrl_miimu = ulValue;

		/* FIXME: add a timeout here. */
		do
		{
			ulValue  = ptPhyCtrl->ulInt_phy_ctrl_miimu;
		} while( (ulValue & HOSTMSK(int_phy_ctrl_miimu_snrdy))!=0 );

		iResult = 0;
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


	/* extract ram bank and frame number */ 
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


static unsigned int drv_eth_xc_get_link_status(void *pvUser __attribute__ ((unused)))
{
	NX90_PHY_CTRL_AREA_T *ptPhyCtrl;
	unsigned long ulValue;
	int iResult;
	unsigned short usValue;
	unsigned int uiLinkStatus;


	uiLinkStatus = 0;

	if( tHandle.uiEthPortNr<2U )
	{
		/* retrieve the link status from the Ethernet port */
		ptPhyCtrl = aptPhyCtrl[tHandle.uiXC];
		ulValue  = ptPhyCtrl->ulInt_phy_ctrl_led;
		ulValue &= HOSTMSK(int_phy_ctrl_led_link_ro);
		if( ulValue!=0 )
		{
			uiLinkStatus = 1;
		}
	}
	else
	{
		iResult = extphy_read(tHandle.uiExtPhyCtrlInst, tHandle.uiExtPhyAddress, PHY_REG_STATUS, &usValue);
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



static void *drv_eth_xc_get_empty_packet(void *pvUser __attribute__ ((unused)))
{
	HOSTDEF(ptPointerFifoArea);
	unsigned long ulFifoValue;
	unsigned int uiFifoNr; 
	unsigned long ulValue;
	void *pvPacket;


	/* Expect no free packet. */
	pvPacket = NULL;

	/* Get the FIFO fill level and check if there is at least one element. */
	uiFifoNr = (tHandle.uiXC * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_EMPTY;

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



static void drv_eth_xc_release_packet(void *pvPacket, void *pvUser __attribute__ ((unused)))
{
	HOSTDEF(ptPointerFifoArea);
	unsigned int uiFifoNr; 
	unsigned long ulFifoValue;


	/* Convert the pointer to a FIFO value. */
	ulFifoValue = convert_packet_pointer_to_fifo_value(pvPacket);

	uiFifoNr = (tHandle.uiXC * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_EMPTY;
	ptPointerFifoArea->aulPfifo[uiFifoNr] = ulFifoValue;
}



static void drv_eth_xc_send_packet(void *pvPacket, unsigned int uiPacketSize, void *pvUser __attribute__ ((unused)))
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

	uiFifoNr = (tHandle.uiXC * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_REQ_LO;
	ptPointerFifoArea->aulPfifo[uiFifoNr] = ulFifoValue;
}



static void *drv_eth_xc_get_received_packet(unsigned int *puiPacketSize, void *pvUser __attribute__ ((unused)))
{
	HOSTDEF(ptPointerFifoArea);
	unsigned long ulFillLevel;
	unsigned int uiFifoNr;
	void *pvPacket;
	unsigned long ulFifoValue;


	/* Expect no packet. */
	pvPacket = NULL;

	uiFifoNr = (tHandle.uiXC * NUM_FIFO_CHANNELS_PER_UNIT) + ETHERNET_FIFO_IND_LO;
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



static void drv_eth_xc_deactivate(void *pvUser __attribute__ ((unused)))
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


static void pfifo_reset(void)
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
	ptPointerFifoArea->ulPfifo_reset = 0xffffffffU;
}



static void pfifo_init(unsigned int uPortNo)
{
	HOSTDEF(ptPointerFifoArea);

	unsigned long ulFifoPtr;
	unsigned long ulFifoStart;
	unsigned long ulFifoEnd;
	unsigned long ulFifoNum;
	unsigned long ulEmptyPtrCnt;
	unsigned long ulFrame;
	unsigned int uXcPortNo = (uPortNo)&1;
	unsigned long ulValue;


	/* Set reset bit for all pointer FIFOs. */
	ulValue  = ptPointerFifoArea->ulPfifo_reset;
	ulValue |= ((1U<<NUM_FIFO_CHANNELS_PER_UNIT)-1U) << (NUM_FIFO_CHANNELS_PER_UNIT * uXcPortNo);
	ptPointerFifoArea->ulPfifo_reset = ulValue;

	/* Get FIFO start and end number of this port number. */
	ulFifoStart = uXcPortNo * NUM_FIFO_CHANNELS_PER_UNIT;
	ulFifoEnd = ulFifoStart + NUM_FIFO_CHANNELS_PER_UNIT;

	for(ulFifoNum=ulFifoStart; ulFifoNum<ulFifoEnd; ulFifoNum++)
	{
		ptPointerFifoArea->aulPfifo_border[ulFifoNum] = (ulFifoNum * FIFO_ENTRIES) + FIFO_ENTRIES - 1;
	}

	/* clear reset bit for all pointer FIFO */
	ulValue  = ptPointerFifoArea->ulPfifo_reset;
	ulValue &= ~(((1U<<NUM_FIFO_CHANNELS_PER_UNIT)-1U) << (NUM_FIFO_CHANNELS_PER_UNIT * uXcPortNo));
	ptPointerFifoArea->ulPfifo_reset = ulValue;

	/*** fill empty pointer FIFO ***/

	/* Use the complete INTRAM segment. */
	ulEmptyPtrCnt = INTRAM_SEGMENT_SIZE / ETH_FRAME_BUF_SIZE;

	/* Fill the empty pointer FIFO. */
	for(ulFrame=1; ulFrame<=ulEmptyPtrCnt; ++ulFrame)
	{
		/* Use INTRAM3. */
		ulFifoPtr  = (0U << SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM);
		ulFifoPtr |= (ulFrame << SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM);
		ptPointerFifoArea->aulPfifo[ulFifoStart + ETHERNET_FIFO_EMPTY] = ulFifoPtr;
	}
}



static int sys_phy_write(unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned int usData)
{
	HOSTDEF(ptAsicCtrlArea);
	HOSTADEF(PHY_CTRL) *ptPhyCtrlArea;
	unsigned long ulMiimuUnitSel;
	unsigned long ulValue;


	/* internal PHYs */
	ulMiimuUnitSel = 0;

	/* exotic case: INTPHY connected with XC0.Port1 MDIO */
	ulValue   = ptAsicCtrlArea->asIo_config[1].ulConfig;
	ulValue  &= HOSTMSK(io_config1_mask_sel_xc1_mdio);
	ulValue >>= HOSTSRT(io_config1_mask_sel_xc1_mdio);
	if( ulValue==2 )
	{
		ulMiimuUnitSel = 1;
	}

	ptPhyCtrlArea = aptPhyCtrl[ulMiimuUnitSel];

	ulValue  = 1U << HOSTSRT(int_phy_ctrl_miimu_preamble);
	ulValue |= 0U << HOSTSRT(int_phy_ctrl_miimu_mdc_period);
	ulValue |= 1U << HOSTSRT(int_phy_ctrl_miimu_rta);
	ulValue |= uMiimuReqAddr << HOSTSRT(int_phy_ctrl_miimu_regaddr);
	ulValue |= uMiimuPhyAddr << HOSTSRT(int_phy_ctrl_miimu_phyaddr);
	ulValue |= usData << HOSTSRT(int_phy_ctrl_miimu_data);
	ulValue |= HOSTMSK(int_phy_ctrl_miimu_opmode);
	ulValue |= HOSTMSK(int_phy_ctrl_miimu_snrdy);
	ptPhyCtrlArea->ulInt_phy_ctrl_miimu = ulValue;

	/* Wait until the operation finished. */
	/* FIXME: add a timeout here. */
	do
	{
		ulValue  = ptPhyCtrlArea->ulInt_phy_ctrl_miimu;
		ulValue &= HOSTMSK(int_phy_ctrl_miimu_snrdy);
	} while( ulValue!=0 );

	return 0;
}

/* write PHY register in special bank */
static void sys_phy_write_bank(unsigned int uiPhy, unsigned int uiBank, unsigned int uiReg, unsigned uiValue)
{
	/* TSTMODE bit in register 20 (TSTCNTL) should be toggled from 0->1->0->1 */
	sys_phy_write(uiPhy, 20, 0x0000);
	sys_phy_write(uiPhy, 20, 0x0400);
	sys_phy_write(uiPhy, 20, 0x0000);
	sys_phy_write(uiPhy, 20, 0x0400);

	/* 1. Write the data to TSTWRITE register. */
	sys_phy_write(uiPhy, 23, uiValue);

	/* 2. Write to the TSTCNTL with the WRITE ADDRESS set to the register address and the WRITE bit set to 1. */
	sys_phy_write(uiPhy, 20, 0x4400 | (uiBank << 11) | (uiReg << 5) | uiReg); /* avoid read-back bug on register VM_DAC_CONTROL1 */

	/* exit test mode */
	sys_phy_write(uiPhy, 20, 0x0000);
}



#define INTPHY_ADDR 0

#define REG_BANK_DSP   0
#define REG_BANK_WOL   1
#define REG_BANK_BIST  3
#define REG_BANK_VMDAC 7

static void intphy_reset(void)
{
	HOSTDEF(ptIntPhyCfgComArea);
	unsigned long ulValue;
	unsigned int uiPhy;


	/* Reset the internal PHY. */
	ulValue = HOSTMSK(int_phy_cfg_phy_ctrl_phy_reset);
	ptIntPhyCfgComArea->ulInt_phy_cfg_phy_ctrl = ulValue;

	/* Wait at least 100us. */
	systime_delay_ms(2U);

	/* Set the configuration with activated reset. */
	ulValue  = g_t_romloader_options.t_ethernet.ulPhyControl;
	ulValue |= HOSTMSK(int_phy_cfg_phy_ctrl_phy_reset);
	ulValue |= HOSTMSK(int_phy_cfg_phy_ctrl_phy0_enable);
	ulValue |= HOSTMSK(int_phy_cfg_phy_ctrl_phy1_enable);
	ptIntPhyCfgComArea->ulInt_phy_cfg_phy_ctrl = ulValue;

	/* Wait 1ms for the PHY reset. */
	systime_delay_ms(2U);

	/* Set the configuration with deactivated reset. */
	ulValue  = g_t_romloader_options.t_ethernet.ulPhyControl;
	ulValue |= HOSTMSK(int_phy_cfg_phy_ctrl_phy0_enable);
	ulValue |= HOSTMSK(int_phy_cfg_phy_ctrl_phy1_enable);
	ptIntPhyCfgComArea->ulInt_phy_cfg_phy_ctrl = ulValue;

	/* Wait 1ms for the PHY reset release. */
	systime_delay_ms(2U);

	for(uiPhy=INTPHY_ADDR; uiPhy<(INTPHY_ADDR+2); ++uiPhy)
	{
		/* PLL register: This is to program the PLL divider value for reference clock of 25MHz. Default value was for reference frequency of 24MHz */
		sys_phy_write_bank(uiPhy, REG_BANK_BIST, 0x1c, 0x0000); /* FR_PLL_DIV0 */
		sys_phy_write_bank(uiPhy, REG_BANK_BIST, 0x1d, 0x0280); /* FR_PLL_DIV1 */

		systime_delay_ms(2); /* 1ms */

		/* MDIX */
		sys_phy_write(uiPhy, 17, 0x0040); /* MODE CONTROL / STATUS */
		/* IF we are programming the port for MDIX mode then the VM DAC register setting should be as follows */
		sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x02, 0xF07B); /* VM_DAC_CONTROL0 */
		sys_phy_write_bank(uiPhy, REG_BANK_VMDAC, 0x03, 0x0004); /* VM_DAC_CONTROL1 */

		/* restart AutoNeg */
		sys_phy_write(uiPhy, 0, 0x1200);
	}
}



static void initialize_standard_phy(unsigned int uiPhyCtrlInst, unsigned int uiPhyAddress)
{
	int iResult;
	unsigned short usData;


	/* Enable all capabilities for the auto negotiation process. */
	iResult = extphy_read(uiPhyCtrlInst, uiPhyAddress, PHY_REG_AUTO_NEG_ADVERTISEMENT, &usData);
	if( iResult==0 )
	{
		usData |= MSK_PHY_ANADV_10_BASE_T;
		usData |= MSK_PHY_ANADV_10_BASE_T_FDX;
		usData |= MSK_PHY_ANADV_100_BASE_TX;
		usData |= MSK_PHY_ANADV_100_BASE_TX_FDX;
		iResult = extphy_write(uiPhyCtrlInst, uiPhyAddress, PHY_REG_AUTO_NEG_ADVERTISEMENT, usData);
		if( iResult==0 )
		{
			/* Enable and restart the auto negotiation process. */
			usData  = MSK_PHY_CTRL_AUTO_NEG_RESTART;
			usData |= MSK_PHY_CTRL_AUTO_NEG_ENABLE;
			extphy_write(uiPhyCtrlInst, uiPhyAddress, PHY_REG_CONTROL, usData);
		}
	}
}



static void deactivate_standard_phy(unsigned int uiPhyCtrlInst, unsigned int uiPhyAddress)
{
	unsigned short usData;


	/* Power down. */
	usData = MSK_PHY_CTRL_POWER_DOWN;
	extphy_write(uiPhyCtrlInst, uiPhyAddress, PHY_REG_CONTROL, usData);
}



static void extphy_reset(DRV_ETH_XC_HANDLE_T *ptHandle)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;
	int iResult;
	unsigned int uiDetectedPhyCnt;
	unsigned int uiExpectedPhyIdx;
	unsigned int uiPhyCtrlInst;
	unsigned int uiPhyAddress;
	unsigned short usData;
	unsigned long ulPhyID;


	ulValue  = HOSTMSK(reset_ctrl_EN_RES_REQ_OUT);
	ulValue |= 0U << HOSTSRT(reset_ctrl_RES_REQ_OUT);
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulReset_ctrl = ulValue;

	systime_delay_ms(2); /* minimum 1ms */

	ulValue  = HOSTMSK(reset_ctrl_EN_RES_REQ_OUT);
	ulValue |= 1U << HOSTSRT(reset_ctrl_RES_REQ_OUT);
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulReset_ctrl = ulValue;

	systime_delay_ms(11); /* minimum 10ms */

	/* Scan the complete address range for devices. */
	uiExpectedPhyIdx = ptHandle->uiEthPortNr & 1U;
	uiDetectedPhyCnt = 0;
	for(uiPhyCtrlInst=0U; uiPhyCtrlInst<2U; ++uiPhyCtrlInst)
	{
		for(uiPhyAddress=0U; uiPhyAddress<32U; ++uiPhyAddress)
		{
			iResult = extphy_read(uiPhyCtrlInst, uiPhyAddress, PHY_REG_ID_1, &usData);
			if( iResult==0 )
			{
				ulPhyID  = (unsigned long)usData;
				iResult = extphy_read(uiPhyCtrlInst, uiPhyAddress, PHY_REG_ID_2, &usData);
				if( iResult==0 )
				{
					ulPhyID |= ((unsigned long)usData) << 16U;
					if( ulPhyID!=0x00000000 && ulPhyID!=0xffffffff )
					{
						if( uiDetectedPhyCnt==uiExpectedPhyIdx )
						{
							initialize_standard_phy(uiPhyCtrlInst, uiPhyAddress);
							ptHandle->uiExtPhyCtrlInst = uiPhyCtrlInst;
							ptHandle->uiExtPhyAddress = uiPhyAddress;
						}
						else
						{
							deactivate_standard_phy(uiPhyCtrlInst, uiPhyAddress);
						}
						++uiDetectedPhyCnt;
					}
				}
			}
		}
	}
}



static void configure_mode(DRV_ETH_XC_HANDLE_T *ptHandle)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned int uiPort;
	unsigned long ulValue;
	unsigned long ulXmMiiCfg;
	unsigned long ulXcMdioCfg;
	unsigned long ulPhyCtrl;


	uiPort = ptHandle->uiEthPortNr;
	if( uiPort<2U )
	{
		/* Connect to internal PHY. */
		ulXmMiiCfg = 8U;
		ulXcMdioCfg = 2U;
		ulPhyCtrl = HOSTDFLT(phy_ctrl0);
	}
	else
	{
		/* Connect to external PHY. */
		ulXmMiiCfg = 6U;
		ulXcMdioCfg = 1U;
		ulPhyCtrl  = 4U << HOSTSRT(phy_ctrl0_phy0_led_invert);
		ulPhyCtrl |= 4U << HOSTSRT(phy_ctrl0_phy1_led_invert);
	}

	/* Disable LVDS connections. */
	ulValue  = ulXmMiiCfg << HOSTSRT(io_config0_sel_xm0_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
	ulValue |= HOSTMSK(io_config0_sel_xm0_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config0_sel_xc0_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
	ulValue |= HOSTMSK(io_config0_sel_xc0_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[0].ulConfig = ulValue;

	ulValue  = ulXmMiiCfg << HOSTSRT(io_config1_sel_xm1_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
	ulValue |= HOSTMSK(io_config1_sel_xm1_mii_cfg_wm);
#endif
	ulValue |= ulXcMdioCfg << HOSTSRT(io_config1_sel_xc1_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
	ulValue |= HOSTMSK(io_config1_sel_xc1_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[1].ulConfig = ulValue;

	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->ulPhy_ctrl0 = ulPhyCtrl;

	if( uiPort>=2U )
	{
		ulValue  = ptAsicCtrlArea->asIo_config[2].ulConfig;
		ulValue |= HOSTMSK(io_config2_clk25out_oe);
		ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
		ptAsicCtrlArea->asIo_config[2].ulConfig = ulValue;
	}

	if( uiPort<2U )
	{
		intphy_reset();
	}
	else
	{
		extphy_reset(ptHandle);
	}

}



static void configure_mode_lvds(void)
{
	HOSTDEF(ptAsicCtrlArea);
	unsigned long ulValue;


	/* Enable LVDS connections. */
	ulValue  = 9U << HOSTSRT(io_config0_sel_xm0_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
	ulValue |= HOSTMSK(io_config0_sel_xm0_mii_cfg_wm);
#endif
	ulValue |= 2U << HOSTSRT(io_config0_sel_xc0_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
	ulValue |= HOSTMSK(io_config0_sel_xc0_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[0].ulConfig = ulValue;

	ulValue  = 9U << HOSTSRT(io_config1_sel_xm1_mii_cfg);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
	ulValue |= HOSTMSK(io_config1_sel_xm1_mii_cfg_wm);
#endif
	ulValue |= 2U << HOSTSRT(io_config1_sel_xc1_mdio);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
	ulValue |= HOSTMSK(io_config1_sel_xc1_mdio_wm);
#endif
	ptAsicCtrlArea->ulAsic_ctrl_access_key = ptAsicCtrlArea->ulAsic_ctrl_access_key;  /* @suppress("Assignment to itself") */
	ptAsicCtrlArea->asIo_config[1].ulConfig = ulValue;

	intphy_reset();
}



static int eth_initialize(unsigned int uiPortNr)
{
	HOSTDEF(ptXpecIrqRegistersArea);
	HOSTDEF(ptSystimeComArea);
	ETHMAC_XPEC_DPM *ptEthStdMac;
	NX90_PHY_CTRL_AREA_T *ptPhyCtrl;
	int iResult;
	unsigned int uiCnt;
	unsigned long ulValue;
	unsigned int uiXC;


	uiXC = uiPortNr & 1U;

	/* initialize pointer to xPEC DRAM */
	ptEthStdMac = aptXpecDramArea[uiXC];

	/* reset the hardware block */
	iResult = NX90_XC_Reset(uiXC);
	if( iResult==0 )
	{
		/* initialize the FIFO unit */
		pfifo_init(uiXC);

		/* load the microcode into the hardware block */
		NX90_XC_Load(paulxMacRpuCodes[uiXC]);
		NX90_XC_Load(paulxMacTpuCodes[uiXC]);
		NX90_XC_Load(paulxPecRpecCodes[uiXC]);
		NX90_XC_Load(paulxPecTpecCodes[uiXC]);

		/* Disable monitoring mode. */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_MONITORING_MODE = 0U;

		/* Set default traffic class arrangement. */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_TRAFFIC_CLASS_ARRANGEMENT = 4;

		/* Copy the current system time border to the copy inside the hardware block. */
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_SYSTIME_BORDER_COPY_PLUS1 = ptSystimeComArea->ulSystime_border + 1;

		/* Set the INTRAM buffer. */
/* FIXME: Does this work without it? */
//		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTRAM_START = (unsigned long)(aucEthernetBuffer_start);

		/* Configure the PHY LEDs. */
		ptPhyCtrl = aptPhyCtrl[uiXC];
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

		/* start XC port */
		NX90_XC_Start(uiXC);

#if 0
		/* configure MII timing */
		/* NOTE: this is only important for an external PHY. */
		if( ETH_OKAY != EthMac_CfgMii(ptEthMac->uiPort, ptConfig->uiMiiCfg) )
		{
		FATAL_ERROR("%s:%d:EthMac_CfgMii() failed", s_pszModule, __LINE__);
		}
#endif

		/* Set the MAC address. */
		ulValue  =                  g_t_romloader_options.t_ethernet.aucMac[0];
		ulValue |= ((unsigned long)(g_t_romloader_options.t_ethernet.aucMac[1])) <<  8U;
		ulValue |= ((unsigned long)(g_t_romloader_options.t_ethernet.aucMac[2])) << 16U;
		ulValue |= ((unsigned long)(g_t_romloader_options.t_ethernet.aucMac[3])) << 24U;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERFACE_MAC_ADDRESS_LO = ulValue;

		ulValue  =                  g_t_romloader_options.t_ethernet.aucMac[4];
		ulValue |= ((unsigned long)(g_t_romloader_options.t_ethernet.aucMac[5])) << 8U;
		ptEthStdMac->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERFACE_MAC_ADDRESS_HI = ulValue;
	}

	return iResult;
}



const NETWORK_IF_T *drv_eth_xc_initialize(unsigned int uiPort, void **ppvUser)
{
	HOSTDEF(ptAsicCtrlArea);
	const NETWORK_IF_T *ptIf;
	unsigned long ulMask;
	unsigned long ulEnable;
	unsigned long ulValue;


	ptIf = NULL;

	/* Check the port number. */
	if( uiPort<=3U )
	{
		/* Initialize the internal handle. */
		tHandle.uiEthPortNr = uiPort;
		tHandle.uiXC = uiPort & 1;
		tHandle.uiExtPhyCtrlInst = 0;
		tHandle.uiExtPhyAddress = 0;

		/* Check if all necessary clocks can be enabled. */
		ulMask = HOSTMSK(clock_enable0_mask_xc_misc);
		ulEnable  = HOSTMSK(clock_enable0_xc_misc);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
		ulEnable |= HOSTMSK(clock_enable0_xc_misc_wm);
#endif
		if( (uiPort&1U)==0U )
		{
			ulMask |= HOSTMSK(clock_enable0_mask_xmac0);
			ulMask |= HOSTMSK(clock_enable0_mask_tpec0);
			ulMask |= HOSTMSK(clock_enable0_mask_rpec0);

			ulEnable |= HOSTMSK(clock_enable0_xmac0);
			ulEnable |= HOSTMSK(clock_enable0_tpec0);
			ulEnable |= HOSTMSK(clock_enable0_rpec0);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
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
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
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

			/* Configure the pad_ctrl for all signals. */
			if( uiPort>=2 )
			{
				/* Configure the common pins. */
				pad_control_apply(aucPadCtrlRegsExtCommon, g_t_romloader_options.t_ethernet.aucPadCtrlExtCommon, sizeof(aucPadCtrlRegsExtCommon));
				if( uiPort==2 )
				{
					pad_control_apply(aucPadCtrlRegsExt0, g_t_romloader_options.t_ethernet.aucPadCtrlExt0, sizeof(aucPadCtrlRegsExt0));
				}
				else
				{
					pad_control_apply(aucPadCtrlRegsExt1, g_t_romloader_options.t_ethernet.aucPadCtrlExt1, sizeof(aucPadCtrlRegsExt1));
				}
			}

			/* Initialize the XC. */
			pfifo_reset();

			configure_mode(&tHandle);

			eth_initialize(uiPort);

			/* This driver needs no external handle. */
			*ppvUser = NULL;

			ptIf = &tNetworkIfXc;
		}
	}

	return ptIf;
}



const NETWORK_IF_T *drv_eth_xc_initialize_lvds(unsigned int uiPort, void **ppvUser)
{
	HOSTDEF(ptAsicCtrlArea);
	const NETWORK_IF_T *ptIf;
	unsigned long ulMask;
	unsigned long ulEnable;
	unsigned long ulValue;
#if 0
	const unsigned short *pusPortcontrol_Index;
	unsigned short *pusPortControl_Values;
	size_t sizPortControl_Values;
#endif

	ptIf = NULL;

	/* Check the port number. */
	if( uiPort<=1U )
	{
		/* Initialize the internal handle. */
		tHandle.uiEthPortNr = uiPort;
		tHandle.uiXC = uiPort & 1;
		tHandle.uiExtPhyCtrlInst = 0;
		tHandle.uiExtPhyAddress = 0;

		/* Check if all necessary clocks can be enabled. */
		ulMask = HOSTMSK(clock_enable0_mask_xc_misc);
		ulEnable  = HOSTMSK(clock_enable0_xc_misc);
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
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
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
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
#if ASIC_TYP==ASIC_TYP_NETX90_FULL
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

			configure_mode_lvds();

			eth_initialize(uiPort);
#if 0
			pusPortcontrol_Index = NULL;
			switch(uiPort)
			{
			case 0:
				pusPortcontrol_Index = ausPortcontrol_Index_Lvds0;
				break;
			case 1:
				pusPortcontrol_Index = ausPortcontrol_Index_Lvds1;
				break;
			}
			pusPortControl_Values = g_t_romloader_options.t_ethernet.ausLvdsPortControl;
			sizPortControl_Values = sizeof(g_t_romloader_options.t_ethernet.ausLvdsPortControl)/sizeof(g_t_romloader_options.t_ethernet.ausLvdsPortControl[0]);
			portcontrol_apply(pusPortcontrol_Index, pusPortControl_Values, sizPortControl_Values);
#endif
			/* This driver needs no external handle. */
			*ppvUser = NULL;

			ptIf = &tNetworkIfXcLvds;
		}
	}

	return ptIf;
}

