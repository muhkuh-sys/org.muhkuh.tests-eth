

#include "xc.h"

#include <stddef.h>
#include <string.h>

#include "netx_io_areas.h"
#include "options.h"
#include "uprintf.h"

#include "eth_std_mac_xpec_regdef.h"
#include "xc_codes_reset.h"
#include "xc_codes_eth_std_mac.h"

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))

#define NUM_FIFO_CHANNELS_PER_UNIT                      8      /**< Number of FIFO units per XC channel */
#define ETH_FRAME_BUF_SIZE                              1560   /**< size of a frame buffer     */

#define POINTER_FIFO_ELEMENTS 3200

#define FIFO_ENTRIES_PER_CHANNEL ((unsigned long)(POINTER_FIFO_ELEMENTS/NUM_FIFO_CHANNELS_PER_UNIT))

#define ACTIVITY_LED_FLASH_PERIOD 5000000 /* 50 ms */

#define ETHERNET_MINIMUM_FRAMELENGTH                    60U
#define ETHERNET_MAXIMUM_FRAMELENGTH                    1518U

#define ETHERNET_FIFO_EMPTY                             0U /* Empty pointer FIFO               */
#define ETHERNET_FIFO_IND_HI                            1U /* High priority indication FIFO    */
#define ETHERNET_FIFO_IND_LO                            2U /* Low priority indication FIFO     */
#define ETHERNET_FIFO_REQ_HI                            3U /* High priority request FIFO       */
#define ETHERNET_FIFO_REQ_LO                            4U /* Low priority request FIFO        */
#define ETHERNET_FIFO_CON_HI                            5U /* High priority confirmation FIFO  */
#define ETHERNET_FIFO_CON_LO                            6U /* Low priority confirmation FIFO   */


static const XC_CODES_RESET_T tXcResetCodes =
{
	.tCodes =
	{
		{
			{
				XcCode_rpu_reset0,
				XcCode_tpu_reset0
			},
			{
				XcCode_rpu_reset1,
				XcCode_tpu_reset1
			},
			{
				XcCode_rpu_reset2,
				XcCode_tpu_reset2
			},
			{
				XcCode_rpu_reset3,
				XcCode_tpu_reset3
			}
		}
	}
};

static const XC_CODES_ETH_STD_MAC_T tXcCodesEthStdMac =
{
	.tCodes =
	{
		{
			{
				XcCode_rpu_eth0,
				XcCode_tpu_eth0,
				XcCode_xpec_eth_std_mac0
			},
			{
				XcCode_rpu_eth1,
				XcCode_tpu_eth1,
				XcCode_xpec_eth_std_mac1
			},
#if 0
			{
				XcCode_rpu_eth2,
				XcCode_tpu_eth2,
				XcCode_xpec_eth_std_mac2
			},
			{
				XcCode_rpu_eth3,
				XcCode_tpu_eth3,
				XcCode_xpec_eth_std_mac3
			}
#endif
		}
	}
};


//static ETHMACBASE_AREA_T * const aptXpecDramArea[2] =
//{
//	(ETHMACBASE_AREA_T * const)HOSTADR(rpec0_dram_ram_start),
//	(ETHMACBASE_AREA_T * const)HOSTADR(rpec1_dram_ram_start)
//};

static HOSTADEF(XMAC) * const aptXmacArea[4] =
{
	(HOSTADEF(XMAC) * const) HOSTADDR(xmac0),
	(HOSTADEF(XMAC) * const) HOSTADDR(xmac1),
	(HOSTADEF(XMAC) * const) HOSTADDR(xmac2),
	(HOSTADEF(XMAC) * const) HOSTADDR(xmac3)
};


static HOSTADEF(XPEC) * const aptXpecArea[4] =
{
	(HOSTADEF(XPEC) * const) HOSTADDR(xpec0),
	(HOSTADEF(XPEC) * const) HOSTADDR(xpec1),
	(HOSTADEF(XPEC) * const) HOSTADDR(xpec2),
	(HOSTADEF(XPEC) * const) HOSTADDR(xpec3)
};


static ETHMACBASE_AREA_T * const aptXpecDramArea[4] =
{
	(ETHMACBASE_AREA_T * const) HOSTADR(xpec0_ram_start),
	(ETHMACBASE_AREA_T * const) HOSTADR(xpec1_ram_start),
	(ETHMACBASE_AREA_T * const) HOSTADR(xpec2_ram_start),
	(ETHMACBASE_AREA_T * const) HOSTADR(xpec3_ram_start)
};

//static HOSTADEF(XPEC) * const aptRpecRegArea[2] =
//{
//	(HOSTADEF(XPEC) * const) HOSTADDR(rpec0_regs),
//	(HOSTADEF(XPEC) * const) HOSTADDR(rpec1_regs)
//};

//static HOSTADEF(XPEC) * const aptTpecRegArea[2] =
//{
//	(HOSTADEF(XPEC) * const) HOSTADDR(tpec0_regs),
//	(HOSTADEF(XPEC) * const) HOSTADDR(tpec1_regs)
//};

//static volatile unsigned long * const apulRpecPramArea[2] =
//{
//	(volatile unsigned long * const) HOSTADR(rpec0_pram_ram_start),
//	(volatile unsigned long * const) HOSTADR(rpec1_pram_ram_start)
//};

//static volatile unsigned long * const apulTpecPramArea[2] =
//{
//	(volatile unsigned long * const) HOSTADR(tpec0_pram_ram_start),
//	(volatile unsigned long * const) HOSTADR(tpec1_pram_ram_start)
//};


static void xc_load_code(const unsigned long *pulCode)
{
	size_t sizPrg;
	size_t sizTrl;
	volatile unsigned long *pulDst;
	const unsigned long *pulSrcCnt;
	const unsigned long *pulSrcEnd;


	/* Get the number of elements in the program code. */
	sizPrg = pulCode[0] / sizeof(unsigned long);
	/* Get the number of elements in the trailing loads. */
	sizTrl = pulCode[1] / sizeof(unsigned long);

	/* Copy the program code. */
	pulDst = (volatile unsigned long*)pulCode[2];
	pulSrcCnt = pulCode + 2 + 1;
	pulSrcEnd = pulCode + 2 + sizPrg;
	while( pulSrcCnt<pulSrcEnd )
	{
		*(pulDst++) = *(pulSrcCnt++);
	}

	/* Copy the trailing loads. */
	pulSrcCnt = pulCode + 2 + sizPrg;
	pulSrcEnd = pulCode + 2 + sizPrg + sizTrl;
	while( pulSrcCnt<pulSrcEnd )
	{
		/* Get the address. */
		pulDst = (volatile unsigned long*)(*(pulSrcCnt++));
		/* Write the value to the address. */
		*pulDst = *(pulSrcCnt++);
	}
}


static void xc_reset(unsigned int uiPortNr)
{
	HOSTDEF(ptXpecIrqRegistersArea);
	HOSTADEF(XMAC)* ptXmac;
	HOSTADEF(XPEC)* ptXpec;
	unsigned long ulDefaultStatCfg;


	ptXmac      = aptXmacArea[uiPortNr];
	ptXpec      = aptXpecArea[uiPortNr];

	/* Stop the XC units. */
	ptXpec->ulXpu_hold_pc = HOSTMSK(xpu_hold_pc_hold);
	ulDefaultStatCfg = 0;
	switch(uiPortNr)
	{
	case 0:
		ulDefaultStatCfg = HOSTDFLT(statcfg0);
		break;
	
	case 1:
		ulDefaultStatCfg = HOSTDFLT(statcfg1);
		break;
	
	case 2:
		ulDefaultStatCfg = HOSTDFLT(statcfg2);
		break;
	
	case 3:
		ulDefaultStatCfg = HOSTDFLT(statcfg3);
		break;
	}
	ptXpec->aulStatcfg[uiPortNr] = ulDefaultStatCfg;
	
	/* Just set Hold-Bit */
	ptXmac->ulXmac_tpu_hold_pc = HOSTMSK(xmac_tpu_hold_pc_tpu_hold);
	ptXmac->ulXmac_rpu_hold_pc = HOSTMSK(xmac_rpu_hold_pc_rpu_hold);
	
	ptXpec->aulStatcfg[uiPortNr] = ulDefaultStatCfg;


	/* Load the rate multiplier reset code. */
	xc_load_code(tXcResetCodes.tCodes.atUnits[uiPortNr].pulRpu);
	xc_load_code(tXcResetCodes.tCodes.atUnits[uiPortNr].pulTpu);

	/* Reset PC to 0 */
	ptXmac->ulXmac_rpu_pc      = 0;
	ptXmac->ulXmac_tpu_pc      = 0;
	/* Clear Hold-Bit */
	ptXmac->ulXmac_tpu_hold_pc = 0;
	ptXmac->ulXmac_rpu_hold_pc = 0;

	/* !!!! ATTENTION: There must be enougth time between starting xMAC and stopping xMAC to execute reset programm */

	/* clear xPEC events and stop DMA */
	ptXpec->aulRam[0] = 0xC0000FFF;       /* Use the command wait b000000000000,b111111111111 at Address 0 */
	ptXpec->ulXpec_pc = 0;                /* Reset the Program Counter to 0 on netX100/500 */
	ptXpec->ulXpec_statcfg = 0;           /* Reset Timer operation and DMA */
	ptXpec->ulXpu_hold_pc = 0;            /* Start the Program */
	ptXpec->aulRam[0x7FF] = 0x0000007F;   /* Link and Stop DMA */
	ptXpec->aulRam[0x7FE] = 0x00000000;   /* dummy access to let xpec run another cycle */
	ptXpec->ulXpu_hold_pc = 1;            /* Hold the Program Counter */
	ptXpec->ulXpec_pc = 0;                /* Reset the Program Counter to 0 on netX100/500 */

	/* reset all xPEC registers to default values */
	ptXpec->aulXpec_r[0] = 0;
	ptXpec->aulXpec_r[1] = 0;
	ptXpec->aulXpec_r[2] = 0;
	ptXpec->aulXpec_r[3] = 0;
	ptXpec->aulXpec_r[4] = 0;
	ptXpec->aulXpec_r[5] = 0;
	ptXpec->aulXpec_r[6] = 0;
	ptXpec->aulXpec_r[7] = 0;
	ptXpec->ulRange01 = 0;
	ptXpec->ulRange23 = 0;
	ptXpec->ulRange45 = 0;
	ptXpec->ulRange67 = 0;
	ptXpec->aulTimer[0] = 0;
	ptXpec->aulTimer[1] = 0;
	ptXpec->aulTimer[2] = 0;
	ptXpec->aulTimer[3] = 0;
	ptXpec->ulUrx_count = 0;
	ptXpec->ulUtx_count = 0;
	ptXpec->ulXpec_statcfg = 0;
	ptXpec->ulEc_maska = 0x0000ffff;
	ptXpec->ulEc_maskb = 0x0000ffff;
	ptXpec->aulEc_mask[0] = 0x0000ffff;
	ptXpec->aulEc_mask[1] = 0x0000ffff;
	ptXpec->aulEc_mask[2] = 0x0000ffff;
	ptXpec->aulEc_mask[3] = 0x0000ffff;
	ptXpec->aulEc_mask[4] = 0x0000ffff;
	ptXpec->aulEc_mask[5] = 0x0000ffff;
	ptXpec->aulEc_mask[6] = 0x0000ffff;
	ptXpec->aulEc_mask[7] = 0x0000ffff;
	ptXpec->aulEc_mask[8] = 0x0000ffff;
	ptXpec->aulEc_mask[9] = 0x0000ffff;
	ptXpec->ulTimer4 = 0;
	ptXpec->ulTimer5 = 0;
	ptXpec->ulIrq  = 0xffff0000;   /* confirm all ARM IRQs */
	ptXpec->ulXpec_adc = 0;

	/* reset SR of current port */
	ptXpec->aulXpec_sr[uiPortNr*4]   = 0;
	ptXpec->aulXpec_sr[uiPortNr*4+1] = 0;
	ptXpec->aulXpec_sr[uiPortNr*4+2] = 0;
	ptXpec->aulXpec_sr[uiPortNr*4+3] = 0;
	ptXpec->aulStatcfg[uiPortNr]     = ulDefaultStatCfg;

	/* Hold xMAC */
	ptXmac->ulXmac_tpu_hold_pc = HOSTMSK(xmac_tpu_hold_pc_tpu_hold);
	ptXmac->ulXmac_rpu_hold_pc = HOSTMSK(xmac_rpu_hold_pc_rpu_hold);

	/* reset urx and utx fifos */
	switch( uiPortNr )
	{
	case 0:
		ptXmac->ulXmac_config_shared0 |=  HOSTMSK(xmac_config_shared0_reset_tx_fifo);
		ptXmac->ulXmac_config_shared0 &= ~HOSTMSK(xmac_config_shared0_reset_tx_fifo);
		ptXmac->ulXmac_config_shared0 |=  HOSTMSK(xmac_config_shared0_reset_rx_fifo);
		ptXmac->ulXmac_config_shared0 &= ~HOSTMSK(xmac_config_shared0_reset_rx_fifo);
		break;
	
	case 1:
		ptXmac->ulXmac_config_shared1 |=  HOSTMSK(xmac_config_shared1_reset_tx_fifo);
		ptXmac->ulXmac_config_shared1 &= ~HOSTMSK(xmac_config_shared1_reset_tx_fifo);
		ptXmac->ulXmac_config_shared1 |=  HOSTMSK(xmac_config_shared1_reset_rx_fifo);
		ptXmac->ulXmac_config_shared1 &= ~HOSTMSK(xmac_config_shared1_reset_rx_fifo);
		break;
	
	case 2:
		ptXmac->ulXmac_config_shared2 |=  HOSTMSK(xmac_config_shared2_reset_tx_fifo);
		ptXmac->ulXmac_config_shared2 &= ~HOSTMSK(xmac_config_shared2_reset_tx_fifo);
		ptXmac->ulXmac_config_shared2 |=  HOSTMSK(xmac_config_shared2_reset_rx_fifo);
		ptXmac->ulXmac_config_shared2 &= ~HOSTMSK(xmac_config_shared2_reset_rx_fifo);
		break;
	
	case 3:
		ptXmac->ulXmac_config_shared3 |=  HOSTMSK(xmac_config_shared3_reset_tx_fifo);
		ptXmac->ulXmac_config_shared3 &= ~HOSTMSK(xmac_config_shared3_reset_tx_fifo);
		ptXmac->ulXmac_config_shared3 |=  HOSTMSK(xmac_config_shared3_reset_rx_fifo);
		ptXmac->ulXmac_config_shared3 &= ~HOSTMSK(xmac_config_shared3_reset_rx_fifo);
		break;
	}

	/* reset IRQs from ARM side */
	ptXpecIrqRegistersArea->aulIrq_xpec[uiPortNr] = 0x0000FFFF;

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
	ptXmac->ulXmac_config_nibble_fifo  = HOSTDFLT(xmac_config_nibble_fifo);
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

	/* reset encoder and PWM on ports 2 and 3 */
	switch( uiPortNr )
	{
	case 0:
	case 1:
		break;
	
	case 2:
	case 3:
		ptXmac->ulXmac_pwm_config = 0;
		ptXmac->ulXmac_pwm_status = 0;
		ptXmac->ulXmac_pwm_tp = 0;
		ptXmac->ulXmac_pwm_tu = 0;
		ptXmac->ulXmac_pwm_tv = 0;
		ptXmac->ulXmac_pwm_tw = 0;
		ptXmac->ulXmac_pwm_td = 0;
		ptXmac->ulXmac_rpwm_tp = 0;
		ptXmac->ulXmac_rpwm_tr = 0;
		ptXmac->ulXmac_pos_config_encoder = 0;
		ptXmac->ulXmac_pos_config_capture = 0;
		ptXmac->ulXmac_pos_command = 0;
		ptXmac->ulXmac_pos_status = 0;
		break;
	}
}


void xc_init_fifo_borders(void)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned long ulBorderCnt;
	unsigned long ulBorder;
	unsigned int uiCnt;


	/* Reset all FIFOs. */
	ptPointerFifoArea->ulPfifo_reset = 0xffffffffU;

	/* Setup the FIFO borders. */
	ulBorderCnt = 0;
	for( uiCnt=0; uiCnt<ARRAYSIZE(ptPointerFifoArea->aulPfifo_border); ++uiCnt)
	{
//		ulBorderCnt += FIFO_ENTRIES_PER_CHANNEL;
		ulBorderCnt += 16;
		ulBorder = ulBorderCnt - 1;
		ptPointerFifoArea->aulPfifo_border[uiCnt] = ulBorder;
	}

	/* Release the reset. */
	ptPointerFifoArea->ulPfifo_reset = 0;
}


void xc_init_empty_pointer_fifo(unsigned int uiPortNr, unsigned char *pucBufferStart, unsigned char *pucBufferEnd)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned int uiEmptyPointerFifo;
	unsigned long ulValue;
	unsigned long ulBufIdxCnt;
	unsigned int uiBuffersAdded;
	unsigned char *pucCnt;
	unsigned char *pucEnd;


	/* FIXME: the UDP checksum code uses uninitialized areas, correct this
	 * and remove this initialization!
	 */

	/* clear ethernet buffer for simulation */
	//memset(&ucEthernetBuffer_start, 0U, (size_t)(pucBufferEnd-pucBufferStart));

	/* Fill the empty pointer FIFO. */
	uiEmptyPointerFifo = uiPortNr * NUM_FIFO_CHANNELS_PER_UNIT + ETHERNET_FIFO_EMPTY;

	/* Get the start for the XC unit. */
	pucCnt = NULL;
	pucEnd = NULL;
	ulBufIdxCnt = 0;
	switch(uiPortNr)
	{
	case 0:
		pucCnt = (unsigned char*)(HOSTADDR(sram0_ahbls0));
		pucEnd = (unsigned char*)(HOSTADDR(sram0_ahbls0) + 0x00008000);
		break;

	case 1:
		pucCnt = (unsigned char*)(HOSTADDR(sram1_ahbls1));
		pucEnd = (unsigned char*)(HOSTADDR(sram1_ahbls1) + 0x00008000);
		break;

	case 2:
		pucCnt = (unsigned char*)(HOSTADDR(sram2_ahbls2));
		pucEnd = (unsigned char*)(HOSTADDR(sram2_ahbls2) + 0x00008000);
		break;

	case 3:
		pucCnt = (unsigned char*)(HOSTADDR(sram3_ahbls3));
		pucEnd = (unsigned char*)(HOSTADDR(sram3_ahbls3) + 0x00008000);
		break;
	}

	/* Loop to the start of the area. */
	uiBuffersAdded = 0;
	do
	{
		/* Is the buffer in the valid area? */
		if( pucCnt>=pucBufferStart && (pucCnt+ETH_FRAME_BUF_SIZE)<=pucBufferEnd )
		{
			/* Add this buffer to the FIFO. */
			ulValue  = ulBufIdxCnt << SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
			ulValue |= uiPortNr << SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM;
			uprintf("FIFO %d: 0x%08x -> 0x%08x\n", uiEmptyPointerFifo, (unsigned long)pucCnt, ulValue);
			ptPointerFifoArea->aulPfifo[uiEmptyPointerFifo] = ulValue;
			++uiBuffersAdded;
			if( uiBuffersAdded>=FIFO_ENTRIES_PER_CHANNEL )
			{
				break;
			}
		}

		pucCnt += ETH_FRAME_BUF_SIZE;
		++ulBufIdxCnt;
	} while( pucCnt<pucEnd );
}


static void xc_enable_multicast(unsigned int uiPortNr, unsigned long ulIp)
{
	unsigned char aucMac[6];
	unsigned int uiHash;
	unsigned int uiOffset;
	unsigned int uiBitMask;
	ETHMACBASE_AREA_T *ptEthMacBaseArea;


	/* Get the multi-cast MAC from the IP. */
	aucMac[0] = 0x01;
	aucMac[1] = 0x00;
	aucMac[2] = 0x5e;
	aucMac[3] = (unsigned char)((ulIp>>16U) & 0xffU);
	aucMac[4] = (unsigned char)((ulIp>> 8U) & 0xffU);
	aucMac[5] = (unsigned char)( ulIp       & 0xffU);

	/* XOR all bytes to get some kind of quick hash value. */
	uiHash = aucMac[0] ^ aucMac[1] ^ aucMac[2] ^ aucMac[3] ^ aucMac[4] ^ aucMac[5];

	/* Bits 5..7 are the DWORD offset in the table area. */
	uiOffset = (uiHash >> 5U) & 0x07U;
	/* Bits 0..4 are the bit position. */
	uiBitMask = 1U << (uiHash & 0x1fU);

	/* Enable the entry in the hash table. */
	ptEthMacBaseArea = aptXpecDramArea[uiPortNr];
	(&(ptEthMacBaseArea->ulAREA_ETHMAC_MULTICAST_HASH_TABLE))[uiOffset] |= uiBitMask;
}


static void xc_start(unsigned int uiPortNr)
{
	HOSTDEF(ptXpecIrqRegistersArea)
	HOSTADEF(XMAC)* ptXmac;
	HOSTADEF(XPEC)* ptXpec;


	ptXmac = aptXmacArea[uiPortNr];
	ptXpec = aptXpecArea[uiPortNr];

	/* Acknowledge all IRQs. */
	ptXpecIrqRegistersArea->aulIrq_xpec[uiPortNr] = HOSTMSK(irq_xpec_irq);

	/* Reset the PC of all units. */
	ptXmac->ulXmac_rpu_pc = 0;
	ptXmac->ulXmac_tpu_pc = 0;
	ptXpec->ulXpec_pc     = 0;

	/* start units */
	ptXmac->ulXmac_tpu_hold_pc = 0;
	ptXmac->ulXmac_rpu_hold_pc = 0;
	ptXpec->ulXpu_hold_pc      = 0;
}


static void *xc_fifo_element_to_address(unsigned int uiXcUnit, unsigned long ulFifoElement)
{
	unsigned long ulFrameNr;
	unsigned long ulFrameAddress;
	void *pvAddress;


	/* Get the frame number. */
	ulFrameNr   = ulFifoElement;
	ulFrameNr  &= MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
	ulFrameNr >>= SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;

	/* NOTE: the current eth_std_mac code directly couples the unit number
	 *       to the INTRAM segment. This means: XC unit 0 can only use
	 *       INTRAM0 as a buffer.
	 */
	pvAddress = NULL;
	switch(uiXcUnit)
	{
	case 0:
		ulFrameAddress = HOSTADDR(sram0_ahbls0);
		ulFrameAddress += ulFrameNr * ETH_FRAME_BUF_SIZE;
		pvAddress = (void*)ulFrameAddress;
		break;

	case 1:
		ulFrameAddress = HOSTADDR(sram1_ahbls1);
		ulFrameAddress += ulFrameNr * ETH_FRAME_BUF_SIZE;
		pvAddress = (void*)ulFrameAddress;
		break;
	
	case 2:
		ulFrameAddress = HOSTADDR(sram2_ahbls2);
		ulFrameAddress += ulFrameNr * ETH_FRAME_BUF_SIZE;
		pvAddress = (void*)ulFrameAddress;
		break;
	
	case 3:
		ulFrameAddress = HOSTADDR(sram3_ahbls3);
		ulFrameAddress += ulFrameNr * ETH_FRAME_BUF_SIZE;
		pvAddress = (void*)ulFrameAddress;
		break;
	}

	return pvAddress;
}


/* Convert a frame address to a FIFO pointer. */
static unsigned long xc_address_to_fifo_ptr(void *pvBuffer)
{
	unsigned long ulFrameNr;
	unsigned long ulRamSegment;
	unsigned long ulValue;


	/* Extract the frame buffer number.
	 * NOTE: the current eth_std_mac code directly couples the unit number
	 *       to the INTRAM segment. This means: XC unit 0 can only use
	 *       INTRAM0 as a buffer.
	 */
	ulFrameNr  = (unsigned long)pvBuffer;
	ulFrameNr &= 0x00007fffU;
	ulFrameNr /= ETH_FRAME_BUF_SIZE;

	ulRamSegment = (unsigned long)pvBuffer;
	ulRamSegment &= 0x00018000U;
	ulRamSegment >>=15U;

	/* Combine the ram segment and frame number. */
	ulValue  = ulFrameNr << SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
	ulValue |= ulRamSegment << SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM;

	return ulValue;
}


void xc_init(unsigned int uiPortNr, const unsigned char *aucMAC, unsigned char *pucFrameBuffer, unsigned char *pucFrameBufferEnd)
{
	HOSTDEF(ptSystimeArea);
	unsigned long ulValue;
	ETHMACBASE_AREA_T *ptEthMacBaseArea;


	xc_reset(uiPortNr);

	/* Initialize the FIFO. */
	xc_init_empty_pointer_fifo(uiPortNr, pucFrameBuffer, pucFrameBufferEnd);

	/* Load all codes. */
	xc_load_code(tXcCodesEthStdMac.tCodes.atUnits[uiPortNr].pulRpu);
	xc_load_code(tXcCodesEthStdMac.tCodes.atUnits[uiPortNr].pulTpu);
	xc_load_code(tXcCodesEthStdMac.tCodes.atUnits[uiPortNr].pulXpec);


	ptEthMacBaseArea = aptXpecDramArea[uiPortNr];

	/* Disable monitoring mode. */
	ptEthMacBaseArea->ulETHMAC_MONITORING_MODE = 0;

	/* Set default traffic class arrangement. (Whatever this might be...) */
	ptEthMacBaseArea->ulETHMAC_TRAFFIC_CLASS_ARRANGEMENT = 4;

	/* Set the default LED configuration. */
	ptEthMacBaseArea->ulETHMAC_LED_CONFIG = 3;
	/* configure flash period */
	ptEthMacBaseArea->ulETHMAC_PHY_LEDS_FLASH_PERIOD = ACTIVITY_LED_FLASH_PERIOD;

	/* Copy the current system time border to the copy inside the hardware block. */
	ptEthMacBaseArea->ulETHMAC_SYSTIME_BORDER_COPY = ptSystimeArea->ulSystime_border;

	/*
	 * Set MAC.
	 */
	ulValue  = ((unsigned long)(aucMAC[0]));
	ulValue |= ((unsigned long)(aucMAC[1])) <<  8U;
	ulValue |= ((unsigned long)(aucMAC[2])) << 16U;
	ulValue |= ((unsigned long)(aucMAC[3])) << 24U;
	ptEthMacBaseArea->ulETHMAC_LOCAL_MAC_ADDRESS_LO = ulValue;
	ulValue  = ((unsigned long)(aucMAC[4]));
	ulValue |= ((unsigned long)(aucMAC[5])) <<  8U;
	ptEthMacBaseArea->ulETHMAC_LOCAL_MAC_ADDRESS_HI = ulValue;

	/* Enable the "all systems" multi-cast address. */
	xc_enable_multicast(uiPortNr, 0x0000fbU);

	/*
	 * Disable all IRQs.
	 */
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_IND_HI = 0;
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_IND_LO = 0;
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_CON_HI = 0;
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_CON_LO = 0;
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED = 0;
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_COL = 0;
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_EARLY_RCV = 0;
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_RX_ERR = 0;
	ptEthMacBaseArea->ulETHMAC_INTERRUPTS_ENABLE_TX_ERR = 0;

	/*
	 * Start the units.
	 */
	xc_start(uiPortNr);
}


void xc_deinit(unsigned int uiPortNr)
{
	HOSTDEF(ptXpecIrqRegistersArea)
	HOSTADEF(XMAC)* ptXmac;
	HOSTADEF(XPEC)* ptXpec;


	ptXmac = aptXmacArea[uiPortNr];
	ptXpec = aptXpecArea[uiPortNr];

	/* Acknowledge all IRQs. */
	ptXpecIrqRegistersArea->aulIrq_xpec[uiPortNr] = HOSTMSK(irq_xpec_irq);

	xc_reset(uiPortNr);

	/* Stop units */
	ptXmac->ulXmac_tpu_hold_pc = HOSTMSK(xmac_tpu_hold_pc_tpu_hold);
	ptXmac->ulXmac_rpu_hold_pc = HOSTMSK(xmac_rpu_hold_pc_rpu_hold);
	ptXpec->ulXpu_hold_pc      = HOSTMSK(xpu_hold_pc_hold);
}


void *xc_get_empty_frame(unsigned int uiPortNr)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned int uiFifoNr;
	unsigned long ulFillLevel;
	unsigned long ulValue;
	void *pvEmptyPacket;


	pvEmptyPacket = NULL;

	uiFifoNr = NUM_FIFO_CHANNELS_PER_UNIT * uiPortNr + ETHERNET_FIFO_EMPTY;

	/* Keep at least one pointer for the XC level
	 * (two parties share this empty pointer FIFO).
	 */
	ulFillLevel = ptPointerFifoArea->aulPfifo_fill_level[uiFifoNr];
	if( ulFillLevel>1 )
	{
		/* Get the FIFO element. */
		ulValue = ptPointerFifoArea->aulPfifo[uiFifoNr];
		pvEmptyPacket = xc_fifo_element_to_address(uiPortNr, ulValue);
	}

	return pvEmptyPacket;
}


void xc_release_frame(unsigned int uiPortNr, void *pvFrame)
{
	HOSTDEF(ptPointerFifoArea)
	unsigned long ulFifoElement;
	unsigned int uiFifoNr;


	/* Get the FIFO pointer. */
	ulFifoElement = xc_address_to_fifo_ptr(pvFrame);

	/* Return pointer in empty FIFO. */
	uiFifoNr = uiPortNr * NUM_FIFO_CHANNELS_PER_UNIT + ETHERNET_FIFO_EMPTY;
	ptPointerFifoArea->aulPfifo[uiFifoNr] = ulFifoElement;
}


void *xc_get_rx_frame(unsigned int uiPortNr, size_t *psizFrame)
{
	HOSTDEF(ptPointerFifoArea)
	unsigned int uiFifoNr;
	size_t sizFillLevel;
	unsigned long ulValue;
	void *pvFrame;
	size_t sizFrame;


	sizFrame = 0;
	pvFrame = NULL;

	/* Get the FIFO number. */
	uiFifoNr = uiPortNr * NUM_FIFO_CHANNELS_PER_UNIT + ETHERNET_FIFO_IND_LO;

	/* Get fill level of FIFO. */
	sizFillLevel = ptPointerFifoArea->aulPfifo_fill_level[uiFifoNr];
	if( sizFillLevel>0 )
	{
		ulValue = ptPointerFifoArea->aulPfifo[uiFifoNr];
		sizFrame   = ulValue;
		sizFrame  &= MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN;
		sizFrame >>= SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN;
		pvFrame = xc_fifo_element_to_address(uiPortNr, ulValue);
	}

	*psizFrame = sizFrame;

	return pvFrame;
}


void xc_send_frame(unsigned int uiPortNr, void *pvPacket, size_t sizPacket)
{
	HOSTDEF(ptPointerFifoArea);
	unsigned long ulValue;
	unsigned int uiFifoNr;


	/* Pad too short packets. */
	if( sizPacket<ETHERNET_MINIMUM_FRAMELENGTH )
	{
		sizPacket = ETHERNET_MINIMUM_FRAMELENGTH;
	}
	/* Crop oversized packets. */
	else if( sizPacket>ETHERNET_MAXIMUM_FRAMELENGTH )
	{
		sizPacket = ETHERNET_MAXIMUM_FRAMELENGTH;
	}

	/* Get the FIFO pointer. */
	ulValue  = xc_address_to_fifo_ptr(pvPacket);
	ulValue |= MSK_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON;
	ulValue |= sizPacket << SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN;

	/* Send the packet. */
	uiFifoNr = uiPortNr * NUM_FIFO_CHANNELS_PER_UNIT + ETHERNET_FIFO_REQ_LO;
	ptPointerFifoArea->aulPfifo[uiFifoNr] = ulValue;
}

