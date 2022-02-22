/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "regdef_netx51.h"
#include "netx6_xc.h"
#include <string.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX6_NUM_XPECS 2

#define NX6_XMAC_RPU_DWORD_RAMSIZE 0x0100
#define NX6_XMAC_TPU_DWORD_RAMSIZE 0x0100
#define NX6_XPEC_DWORD_RAMSIZE     0x0800

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

/*****************************************************************************/
#include "hal_resources_defines_netx6.h"
__USE_XC_START_STOP
__USE_RPEC_PRAM_START
__USE_TPEC_PRAM_START
__USE_RPEC_PRAM_END
__USE_TPEC_PRAM_END
__USE_RPEC_REGS
__USE_TPEC_REGS
__USE_XPEC_DRAM
__USE_XMAC
__USE_RPU_PRAM_START
__USE_TPU_PRAM_START
__USE_RPU_PRAM_END
__USE_TPU_PRAM_END
__USE_XPEC_IRQ_REGS
/*****************************************************************************/

static const uint32_t XcCode_rpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0x1018a000, 0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 
  0xe15bde84, 0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 
  0xe15bde88, 0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 
  0x001fde8b, 
  /* trailing loads */
  
};

static const uint32_t XcCode_rpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0x1018a800, 0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 
  0xe15bde84, 0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 
  0xe15bde88, 0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 
  0x001fde8b, 
  /* trailing loads */
  
};

static const uint32_t XcCode_tpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0x1018a400, 0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 
  0xe15bde84, 0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 
  0xe15bde88, 0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 
  0x001fde8b, 
  /* trailing loads */
  
};

static const uint32_t XcCode_tpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0x1018ac00, 0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 
  0xe15bde84, 0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 
  0xe15bde88, 0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 
  0x001fde8b, 
  /* trailing loads */
  
};

static const uint32_t* paulxMacRpuCodes[2]=
{
  XcCode_rpu_reset0,
  XcCode_rpu_reset1,
};

static const uint32_t* paulxMacTpuCodes[2]=
{
  XcCode_tpu_reset0,
  XcCode_tpu_reset1,
};

/* netx6 address space: start address of the xPEC Register area */
static const uint32_t s_aulNX6RpecRegs[2] = { Addr_NX51_rpec0_regs, Addr_NX51_rpec1_regs };
static const uint32_t s_aulNX6TpecRegs[2] = { Addr_NX51_tpec0_regs, Addr_NX51_tpec1_regs };

/* netx6 address space: start address of the xPEC PRAM area */
static const uint32_t s_aulNX6RpecPram[2] = { Adr_NX51_rpec0_pram_ram_start, Adr_NX51_rpec1_pram_ram_start };
static const uint32_t s_aulNX6TpecPram[2] = { Adr_NX51_tpec0_pram_ram_start, Adr_NX51_tpec1_pram_ram_start };

/* netx6 address space: start address of the xPEC DRAM */
static const uint32_t s_aulNX6XpecDram[2] = { Adr_NX51_rpec0_dram_ram_start, Adr_NX51_rpec1_dram_ram_start };

/* netx6 address space: start address of the xMAC area */
static const uint32_t s_aulNX6XmacRegs[2] = { Addr_NX51_xmac0_regs, Addr_NX51_xmac1_regs };

/* netx6 address space: start address of the xMAC ram */
static const uint32_t s_aulNX6RpuPram[2]  = { Adr_NX51_rpu0_ram_xmac_ram_start, Adr_NX51_rpu1_ram_xmac_ram_start };
static const uint32_t s_aulNX6TpuPram[2]  = { Adr_NX51_tpu0_ram_xmac_ram_start, Adr_NX51_tpu1_ram_xmac_ram_start };

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Reset XC Code
* \description
*   Reset XC port.
* \class 
*   XC 
* \params
*   uPortNo          [in]  XC Port Number
*   pvUser           [in]  User specific parameters
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_XC_Reset( unsigned int uPortNo,
                  void*        pvUser )
{
  NX51_XMAC_AREA_T* ptXmac;
  NX51_XPEC_AREA_T* ptRpec;
  NX51_XPEC_AREA_T* ptTpec;
  volatile uint32_t* pulRpecPram;
  volatile uint32_t* pulTpecPram;
  unsigned int uIdx;
  
  if( uPortNo >= NX6_NUM_XPECS)
    return -1;

  ptXmac      = s_aptXmacArea[uPortNo];
  ptRpec      = s_aptRpecRegArea[uPortNo];
  ptTpec      = s_aptTpecRegArea[uPortNo];
  pulRpecPram = s_apulRpecPramArea[uPortNo];
  pulTpecPram = s_apulTpecPramArea[uPortNo];

  /* Stop xPEC and xMAC */
  NX_WRITE32(s_ptXcStartStop->ulXc_start_stop_ctrl,( MSK_NX51_xc_start_stop_ctrl_stop_rpec0
                                                    |MSK_NX51_xc_start_stop_ctrl_stop_tpec0
                                                    |MSK_NX51_xc_start_stop_ctrl_stop_rpu0
                                                    |MSK_NX51_xc_start_stop_ctrl_stop_tpu0
                                                   ) << (4*uPortNo)
            );

  /* Clear output enable of TX as soon as possible */
  NX_WRITE32(ptXmac->ulXmac_config_obu, DFLT_VAL_NX51_xmac_config_obu);
  /* Clear output enable of IO 0..5 as soon as possible */

  NX_WRITE32(ptRpec->aulStatcfg[uPortNo], 0xffff0000);

  switch (uPortNo)
  {
    case 0: NX_WRITE32(ptRpec->ulXpec_config, MSK_NX51_xpec_config_reset_urx_fifo0 | MSK_NX51_xpec_config_reset_utx_fifo0);
            break;
    case 1: NX_WRITE32(ptRpec->ulXpec_config, MSK_NX51_xpec_config_reset_urx_fifo1 | MSK_NX51_xpec_config_reset_utx_fifo1);
            break;
  }
  
  /* load rate multiplier reset code */
  if( NX6_XC_Load( uPortNo, NX6_XC_TYPE_RPU, (uint32_t*)paulxMacRpuCodes[uPortNo], pvUser)!=0 ) {
    return -1;
  }
  if( NX6_XC_Load( uPortNo, NX6_XC_TYPE_TPU, (uint32_t*)paulxMacTpuCodes[uPortNo], pvUser)!=0 ) {
    return -1;
  }
  
  NX_WRITE32(ptXmac->ulXmac_rpu_pc, 0); /* Reset PC to 0 */
  NX_WRITE32(ptXmac->ulXmac_tpu_pc, 0); /* Reset PC to 0 */

  NX_WRITE32(s_ptXcStartStop->ulXc_start_stop_ctrl, ( MSK_NX51_xc_start_stop_ctrl_start_rpu0
                                                     |MSK_NX51_xc_start_stop_ctrl_start_tpu0
                                                    ) << (4*uPortNo)
            );

  /* !!!! ATTENTION: There must be enough time between starting xMAC and stopping xMAC to execute reset program */

  NX_WRITE32(pulRpecPram[0], 0xC0000FFF);
  NX_WRITE32(pulTpecPram[0], 0xC0000FFF);

  NX_WRITE32(ptRpec->ulXpec_pc, 0x7ff); /* Reset the Program Counter to 0x7ff */
  NX_WRITE32(ptTpec->ulXpec_pc, 0x7ff); /* Reset the Program Counter to 0x7ff */


  /* let the XC run for at least 10 cycles */
  for(uIdx = 0; uIdx < 10; uIdx++)
  {
    NX_WRITE32(s_ptXcStartStop->ulXc_start_stop_ctrl, ( MSK_NX51_xc_start_stop_ctrl_start_rpec0
                                                       |MSK_NX51_xc_start_stop_ctrl_start_tpec0
                                                      ) << (4*uPortNo)
              );
  }

  NX_WRITE32(s_ptXcStartStop->ulXc_start_stop_ctrl,  ( MSK_NX51_xc_start_stop_ctrl_stop_rpec0
                                                      |MSK_NX51_xc_start_stop_ctrl_stop_tpec0
                                                     ) << (4*uPortNo)
            );
  NX_WRITE32(ptRpec->ulXpec_pc, 0x7ff); /* Reset the Program Counter to 0x7ff */
  NX_WRITE32(ptTpec->ulXpec_pc, 0x7ff); /* Reset the Program Counter to 0x7ff */

  /* reset all registers */
  NX_WRITE32(ptRpec->aulXpec_r[0], 0);
  NX_WRITE32(ptTpec->aulXpec_r[0], 0);
  NX_WRITE32(ptRpec->aulXpec_r[1], 0);
  NX_WRITE32(ptTpec->aulXpec_r[1], 0);
  NX_WRITE32(ptRpec->aulXpec_r[2], 0);
  NX_WRITE32(ptTpec->aulXpec_r[2], 0);
  NX_WRITE32(ptRpec->aulXpec_r[3], 0);
  NX_WRITE32(ptTpec->aulXpec_r[3], 0);
  NX_WRITE32(ptRpec->aulXpec_r[4], 0);
  NX_WRITE32(ptTpec->aulXpec_r[4], 0);
  NX_WRITE32(ptRpec->aulXpec_r[5], 0);
  NX_WRITE32(ptTpec->aulXpec_r[5], 0);
  NX_WRITE32(ptRpec->aulXpec_r[6], 0);
  NX_WRITE32(ptTpec->aulXpec_r[6], 0);
  NX_WRITE32(ptRpec->aulXpec_r[7], 0);
  NX_WRITE32(ptTpec->aulXpec_r[7], 0);

  switch (uPortNo)
  {
    case 0: NX_WRITE32(ptRpec->ulXpec_stat_bits_shared, 0x00ff0000);
            break;
    case 1: NX_WRITE32(ptRpec->ulXpec_stat_bits_shared, 0xff000000);
            break;
  }

  NX_WRITE32(ptRpec->ulRange_urtx_count, 0);
  NX_WRITE32(ptTpec->ulRange_urtx_count, 0);
  NX_WRITE32(ptRpec->ulRange45         , 0);
  NX_WRITE32(ptTpec->ulRange45         , 0);
  NX_WRITE32(ptRpec->ulRange67         , 0);
  NX_WRITE32(ptTpec->ulRange67         , 0);
  NX_WRITE32(ptRpec->ulUrx_count       , 0);
  NX_WRITE32(ptTpec->ulUrx_count       , 0);
  NX_WRITE32(ptRpec->ulUtx_count       , 0);
  NX_WRITE32(ptTpec->ulUtx_count       , 0);
  
  /* Stop all Timers */
  NX_WRITE32(ptRpec->ulTimer4   , 0);
  NX_WRITE32(ptTpec->ulTimer4   , 0);
  NX_WRITE32(ptRpec->ulTimer5   , 0);
  NX_WRITE32(ptTpec->ulTimer5   , 0);
  NX_WRITE32(ptRpec->aulTimer[0], 0);
  NX_WRITE32(ptTpec->aulTimer[0], 0);
  NX_WRITE32(ptRpec->aulTimer[1], 0);
  NX_WRITE32(ptTpec->aulTimer[1], 0);
  NX_WRITE32(ptRpec->aulTimer[2], 0);
  NX_WRITE32(ptTpec->aulTimer[2], 0);
  NX_WRITE32(ptRpec->aulTimer[3], 0);
  NX_WRITE32(ptTpec->aulTimer[3], 0);

  NX_WRITE32(ptRpec->ulIrq, 0xFFFF0000); /* Clear XPEC side IRQ request lines */
  NX_WRITE32(ptTpec->ulIrq, 0xFFFF0000); /* Clear XPEC side IRQ request lines */

  /* Reset events */
  NX_WRITE32(ptRpec->ulEc_maska   , 0x0000FFFF);
  NX_WRITE32(ptTpec->ulEc_maska   , 0x0000FFFF);
  NX_WRITE32(ptRpec->ulEc_maskb   , 0x0000FFFF);
  NX_WRITE32(ptTpec->ulEc_maskb   , 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[0], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[0], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[1], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[1], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[2], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[2], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[3], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[3], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[4], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[4], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[5], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[5], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[6], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[6], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[7], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[7], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[8], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[8], 0x0000FFFF);
  NX_WRITE32(ptRpec->aulEc_mask[9], 0x0000FFFF);
  NX_WRITE32(ptTpec->aulEc_mask[9], 0x0000FFFF);

  /* Reset shared registers, reset URX/UTX FIFOs */
  /* Reset SR0-3 for XC0, SR4-7 for XC1 */
  for( uIdx = 4 * uPortNo; uIdx < 4 * uPortNo + 4; ++uIdx )
    NX_WRITE32(ptRpec->aulXpec_sr[uIdx], 0);
  /* Reset SR8-11 for XC0, SR12-15 for XC1 */
  for( uIdx = 8 + 4 * uPortNo; uIdx < 12 + 4 * uPortNo; ++uIdx )
    NX_WRITE32(ptRpec->aulXpec_sr[uIdx], 0);
    
  NX_WRITE32(ptRpec->ulDatach_wr_cfg, 0);
  NX_WRITE32(ptTpec->ulDatach_wr_cfg, 0);
  NX_WRITE32(ptRpec->ulDatach_rd_cfg, 0);
  NX_WRITE32(ptTpec->ulDatach_rd_cfg, 0);
  NX_WRITE32(ptRpec->ulSysch_addr   , 0);
  NX_WRITE32(ptTpec->ulSysch_addr   , 0);

  /* confirm all interrupts from xPEC */
  NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uPortNo], 0x0000FFFF);

  /* hold xMAC */
  NX_WRITE32(s_ptXcStartStop->ulXc_start_stop_ctrl, ( MSK_NX51_xc_start_stop_ctrl_stop_rpu0
                                                     |MSK_NX51_xc_start_stop_ctrl_stop_tpu0
                                                    ) << (4*uPortNo)
            );
      
  /* reset all xMAC registers to default values */
  NX_WRITE32(ptXmac->ulXmac_rx_hw               , 0);
  NX_WRITE32(ptXmac->ulXmac_rx_hw_count         , 0);
  NX_WRITE32(ptXmac->ulXmac_tx                  , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_hw               , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_hw_count         , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_sent             , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[0]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[1]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[2]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[3]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[4]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[5]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[6]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[7]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[8]              , 0);
  NX_WRITE32(ptXmac->aulXmac_wr[9]              , 0);
  NX_WRITE32(ptXmac->ulXmac_config_mii          , 0);
  NX_WRITE32(ptXmac->ulXmac_config_rx_nibble_fifo, DFLT_VAL_NX51_xmac_config_rx_nibble_fifo);
  NX_WRITE32(ptXmac->ulXmac_config_tx_nibble_fifo, 0);
  NX_WRITE32(ptXmac->ulXmac_rpu_count1          , 0);
  NX_WRITE32(ptXmac->ulXmac_rpu_count2          , 0);
  NX_WRITE32(ptXmac->ulXmac_tpu_count1          , 0);
  NX_WRITE32(ptXmac->ulXmac_tpu_count2          , 0);
  NX_WRITE32(ptXmac->ulXmac_rx_count            , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_count            , 0);
  NX_WRITE32(ptXmac->ulXmac_rpm_mask0           , 0);
  NX_WRITE32(ptXmac->ulXmac_rpm_val0            , 0);
  NX_WRITE32(ptXmac->ulXmac_rpm_mask1           , 0);
  NX_WRITE32(ptXmac->ulXmac_rpm_val1            , 0);
  NX_WRITE32(ptXmac->ulXmac_tpm_mask0           , 0);
  NX_WRITE32(ptXmac->ulXmac_tpm_val0            , 0);
  NX_WRITE32(ptXmac->ulXmac_tpm_mask1           , 0);
  NX_WRITE32(ptXmac->ulXmac_tpm_val1            , 0);
  
  NX_WRITE32(ptXmac->ulXmac_rx_crc_polynomial_l , 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc_polynomial_h , 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc_l            , 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc_h            , 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc_cfg          , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_polynomial_l , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_polynomial_h , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_l            , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_h            , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc_cfg          , 0);

  NX_WRITE32(ptXmac->ulXmac_rx_crc32_l          , 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc32_h          , 0);
  NX_WRITE32(ptXmac->ulXmac_rx_crc32_cfg        , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc32_l          , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc32_h          , 0);
  NX_WRITE32(ptXmac->ulXmac_tx_crc32_cfg        , 0);

  NX_WRITE32(ptXmac->ulXmac_config_sbu2         , DFLT_VAL_NX51_xmac_config_sbu2);
  NX_WRITE32(ptXmac->ulXmac_config_obu2         , DFLT_VAL_NX51_xmac_config_obu2);

  NX_WRITE32(ptXmac->ulXmac_rpu_pc              , 0);
  NX_WRITE32(ptXmac->ulXmac_tpu_pc              , 0);

  return 0;
}

/*****************************************************************************/
/*! Load XC Code
* \description
*   Load XC port.
* \class 
*   XC 
* \params
*   uPortNo              [in]  XC Port Number
*   eXcType              [in]  XC Entity To Download To (RPU, TPU, xPEC)
*   pulXcPrg             [in]  Pointer To Microcode
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_XC_Load( unsigned int    uPortNo,
                 NX6_XC_TYPE_E   eXcType,
                 const uint32_t* pulXcPrg,
                 void*           pvUser )
{
  volatile uint32_t *pulDst, *pulDstCnt;
  volatile uint32_t *pulRamStart, *pulRamEnd;
  const uint32_t *pulSrcStart, *pulSrcCnt, *pulSrcEnd;
  unsigned int uiElements;
  volatile uint32_t *pulRange0Dpm, *pulRange1Dpm, *pulRange2Dpm;
  uint32_t ulRange0Start, ulRange1Start, ulRange2Start;
  uint32_t ulRange0Size, ulRange1Size, ulRange2Size;
  uint32_t ulAddress, ulOffset;
  
  /* check the instance number */
  if( uPortNo >= NX6_NUM_XPECS )
    return -1;
  
  /* get the start and end address of the ram area, get the physical address */
  switch( eXcType )
  {
    case NX6_XC_TYPE_RPEC:
      pulRamStart     = s_apulRpecPramArea[uPortNo];
      pulRamEnd       = s_apulRpecPramAreaEnd[uPortNo]+1;

      /* Register Area */
      pulRange0Dpm    = (volatile uint32_t*)s_aptRpecRegArea[uPortNo];
      ulRange0Start   = s_aulNX6RpecRegs[uPortNo];
      ulRange0Size    = sizeof(NX51_XPEC_AREA_T);

      /* PRAM Area */
      pulRange1Dpm    = s_apulRpecPramArea[uPortNo];
      ulRange1Start   = s_aulNX6RpecPram[uPortNo];
      ulRange1Size    = NX6_XPEC_DWORD_RAMSIZE * sizeof(uint32_t);

      /* DRAM Area */
      pulRange2Dpm    = s_apulXpecDramArea[uPortNo];
      ulRange2Start   = s_aulNX6XpecDram[uPortNo];
      ulRange2Size    = NX6_XPEC_DWORD_RAMSIZE * sizeof(uint32_t);
      break;

    case NX6_XC_TYPE_TPEC:
      pulRamStart     = s_apulTpecPramArea[uPortNo];
      pulRamEnd       = s_apulTpecPramAreaEnd[uPortNo]+1;

      /* Register Area */
      pulRange0Dpm    = (volatile uint32_t*)s_aptTpecRegArea[uPortNo];
      ulRange0Start   = s_aulNX6TpecRegs[uPortNo];
      ulRange0Size    = sizeof(NX51_XPEC_AREA_T);

      /* PRAM Area */
      pulRange1Dpm    = s_apulTpecPramArea[uPortNo];
      ulRange1Start   = s_aulNX6TpecPram[uPortNo];
      ulRange1Size    = NX6_XPEC_DWORD_RAMSIZE * sizeof(uint32_t);

      /* DRAM Area */
      pulRange2Dpm    = s_apulXpecDramArea[uPortNo];
      ulRange2Start   = s_aulNX6XpecDram[uPortNo] + NX6_XPEC_DWORD_RAMSIZE * sizeof(uint32_t);
      ulRange2Size    = NX6_XPEC_DWORD_RAMSIZE * sizeof(uint32_t);
      break;

    case NX6_XC_TYPE_RPU:
      pulRamStart     = s_aptRpuPramArea[uPortNo];
      pulRamEnd       = s_aptRpuPramAreaEnd[uPortNo]+1;

      /* Register Area */
      pulRange0Dpm    = (volatile uint32_t*)s_aptXmacArea[uPortNo];
      ulRange0Start   = s_aulNX6XmacRegs[uPortNo];
      ulRange0Size    = sizeof(NX51_XMAC_AREA_T);

      /* PRAM Area */
      pulRange1Dpm    = s_aptRpuPramArea[uPortNo];
      ulRange1Start   = s_aulNX6RpuPram[uPortNo];
      ulRange1Size    = NX6_XMAC_RPU_DWORD_RAMSIZE * sizeof(uint32_t);

      pulRange2Dpm    = pulRange1Dpm;
      ulRange2Start   = ulRange1Start;
      ulRange2Size    = ulRange1Size;
      break;

    case NX6_XC_TYPE_TPU:
      pulRamStart     = s_aptTpuPramArea[uPortNo];
      pulRamEnd       = s_aptTpuPramAreaEnd[uPortNo]+1;

      /* Register Area */
      pulRange0Dpm    = (volatile uint32_t*)s_aptXmacArea[uPortNo];
      ulRange0Start   = s_aulNX6XmacRegs[uPortNo];
      ulRange0Size    = sizeof(NX51_XMAC_AREA_T);

      /* PRAM Area */
      pulRange1Dpm    = s_aptTpuPramArea[uPortNo];
      ulRange1Start   = s_aulNX6TpuPram[uPortNo];
      ulRange1Size    = NX6_XMAC_TPU_DWORD_RAMSIZE * sizeof(uint32_t);

      pulRange2Dpm    = pulRange1Dpm;
      ulRange2Start   = ulRange1Start;
      ulRange2Size    = ulRange1Size;
      break;
             
    default:           return -1; /* unknown unit type */
  }
  
  /* get the number of code elements */
  uiElements = pulXcPrg[0] / sizeof(uint32_t) - 1;
  
  /* get the pointer in the XC area */
  /* ram_virtual_start + code_physical_start - ram_physical_start */
  pulDst = pulRamStart;
  
  /* the code must fit into the ram area */
  if( (pulDst + uiElements) > pulRamEnd ) {
    /* the code exceeds the xPEC ram! */
    return -1;
  }
  
  /* get source start and end pointer */
  pulSrcStart = pulXcPrg + 3;
  pulSrcEnd = pulSrcStart + uiElements;
  
  /* copy the code to xc ram */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt < pulSrcEnd )
  {
    NX_WRITE32(*pulDstCnt, *pulSrcCnt);
    pulDstCnt++;
    pulSrcCnt++;
  }
  
  /* compare the code */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt < pulSrcEnd )
  {
    if( NX_READ32(*pulDstCnt) != *pulSrcCnt )
      return -1;
    pulDstCnt++;
    pulSrcCnt++;
  }
 
  /* get the number of trailing loads */
  uiElements = pulXcPrg[1] / sizeof(uint32_t);
  
  /* get source start and end pointer */
  pulSrcCnt = pulXcPrg + 2 + pulXcPrg[0] / sizeof(uint32_t);
  pulSrcEnd = pulSrcCnt + uiElements;
  
  /* write all trailing loads */
  while( pulSrcCnt < pulSrcEnd )
  {
    /* get the destination address */
    ulAddress = *pulSrcCnt++;

    /* test if the address is in one of the 3 ranges */
    if( ulAddress >= ulRange0Start && (ulOffset=ulAddress-ulRange0Start)<ulRange0Size ) {
      /* address is in range 0 */
      pulDst = pulRange0Dpm + ulOffset/sizeof(uint32_t);
    }
    else if( ulAddress >= ulRange1Start && (ulOffset=ulAddress-ulRange1Start)<ulRange1Size ) {
      /* address is in range 1 */
      pulDst = pulRange1Dpm + ulOffset/sizeof(uint32_t);
    }
    else if( ulAddress >= ulRange2Start && (ulOffset=ulAddress-ulRange2Start)<ulRange2Size ) {
      /* address is in range 2 */
      pulDst = pulRange2Dpm + ulOffset/sizeof(uint32_t);
    }
    else {
      /* address is outside all ranges, cancel! */
      pulDst = NULL;
    }

    /* address error -> exit */
    if( pulDst == NULL ) {
      return -1;
    }

    /* write the data */
    NX_WRITE32(*pulDst, *pulSrcCnt);
    pulSrcCnt++;
  }
  
  return 0;
}

/*****************************************************************************/
/*! Start XC Port
* \description
*   Start XC port.
* \class 
*   XC 
* \params
*   uPortNo              [in]  XC Port Number
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_XC_Start( unsigned int uPortNo,
                  void*        pvUser )
{
  NX51_XMAC_AREA_T* ptXmacArea;
  NX51_XPEC_AREA_T* ptRpecArea;
  NX51_XPEC_AREA_T* ptTpecArea;

  if(uPortNo>= NX6_NUM_XPECS) {
    return -1;
  }
  ptXmacArea = s_aptXmacArea[uPortNo];
  ptRpecArea = s_aptRpecRegArea[uPortNo];
  ptTpecArea = s_aptTpecRegArea[uPortNo];

  /* reset PC of units */
  NX_WRITE32(ptXmacArea->ulXmac_rpu_pc, 0);
  NX_WRITE32(ptXmacArea->ulXmac_tpu_pc, 0);
  NX_WRITE32(ptRpecArea->ulXpec_pc, 0x7ff);
  NX_WRITE32(ptTpecArea->ulXpec_pc, 0x7ff);

  /* start units */
  NX_WRITE32(s_ptXcStartStop->ulXc_start_stop_ctrl, ( ( MSK_NX51_xc_start_stop_ctrl_start_rpec0
                                                       |MSK_NX51_xc_start_stop_ctrl_start_tpec0
                                                       |MSK_NX51_xc_start_stop_ctrl_start_rpu0
                                                       |MSK_NX51_xc_start_stop_ctrl_start_tpu0
                                                       ) << (4*uPortNo))
            );
  return 0;
}
