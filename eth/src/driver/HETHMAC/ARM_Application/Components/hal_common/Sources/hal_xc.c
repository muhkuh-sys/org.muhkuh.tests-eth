#include "hal_xc.h"
#include "hw_defines.h"
#include <string.h> /* need NULL */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define NUM_XPECS_PER_XC 2
#define NUM_XMACS_PER_XC 2

#if defined(__NETX4000)
#define NUM_XPECS 4
#define NUM_XMACS 4
#elif defined(__NETX90)
#define NUM_XPECS 2
#define NUM_XMACS 2
#endif

#define XMAC_RPU_DWORD_RAMSIZE 0x0100
#define XMAC_TPU_DWORD_RAMSIZE 0x0100
#define XPEC_DWORD_RAMSIZE     0x0800

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_XC_START_STOP(s_ptXcStartStop)
HW_PTR_RPEC_REGS(s_aptRpecRegArea)
HW_PTR_TPEC_REGS(s_aptTpecRegArea)
HW_PTR_RPEC_PRAM_START(s_apulRpecPramArea)
HW_PTR_TPEC_PRAM_START(s_apulTpecPramArea)
HW_PTR_RPEC_PRAM_END(s_apulRpecPramAreaEnd)
HW_PTR_TPEC_PRAM_END(s_apulTpecPramAreaEnd)
HW_PTR_XPEC_DRAM(s_apulXpecDramArea)
HW_PTR_RPEC_DRAM(s_apulRpecDramArea)
HW_PTR_XMAC(s_aptXmacArea)
HW_PTR_RPU_PRAM_START(s_aptRpuPramArea)
HW_PTR_TPU_PRAM_START(s_aptTpuPramArea)
HW_PTR_RPU_PRAM_END(s_aptRpuPramAreaEnd)
HW_PTR_TPU_PRAM_END(s_aptTpuPramAreaEnd)
HW_PTR_XPEC_IRQ_REGS(s_ptXpecIrqRegs)
HW_PTR_XC_SYSTIME_CONFIG(s_ptXcSystimeConfig)
HW_PTR_XC_EXTERNAL_CFG(s_ptXcExtCfg)

#if defined(__NETX4000)
static const uint32_t XcCode_rpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4020000, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_rpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4020800, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_rpu_reset2[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4021000, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_rpu_reset3[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4021800, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_tpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4020400, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_tpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4020c00, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_tpu_reset2[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4021400, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_tpu_reset3[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xf4021c00, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

/* Use the same reset code binaries (placeholder for different reset codes) */
#define XcCode_xc0_rpu_reset0 XcCode_rpu_reset0
#define XcCode_xc0_rpu_reset1 XcCode_rpu_reset1

#define XcCode_xc1_rpu_reset0 XcCode_rpu_reset2
#define XcCode_xc1_rpu_reset1 XcCode_rpu_reset3

#define XcCode_xc0_tpu_reset0 XcCode_tpu_reset0
#define XcCode_xc0_tpu_reset1 XcCode_tpu_reset1

#define XcCode_xc1_tpu_reset0 XcCode_tpu_reset2
#define XcCode_xc1_tpu_reset1 XcCode_tpu_reset3

static const uint32_t* paulxMacRpuCodes[4]=
{
  XcCode_xc0_rpu_reset0,
  XcCode_xc0_rpu_reset1,
  XcCode_xc1_rpu_reset0,
  XcCode_xc1_rpu_reset1,
};

static const uint32_t* paulxMacTpuCodes[4]=
{
  XcCode_xc0_tpu_reset0,
  XcCode_xc0_tpu_reset1,
  XcCode_xc1_tpu_reset0,
  XcCode_xc1_tpu_reset1,
};

#elif defined(__NETX90)
static const uint32_t XcCode_rpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xff110000, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_rpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xff110800, /* start address */
  0x01300001, 0xe15bde81, 0x01380001, 0xe15bde82, 0x0143fffd, 0xe15bde83, 0x0147fffd, 0xe15bde84,
  0x01480001, 0xe15bde85, 0x0143fff9, 0xe15bde86, 0x014bfffd, 0xe15bde87, 0x01440001, 0xe15bde88,
  0x0143fffd, 0xe15bde89, 0x01480001, 0xe15bde8a, 0x01380005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};
static const uint32_t XcCode_tpu_reset0[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xff110400, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

static const uint32_t XcCode_tpu_reset1[27] = {
  0x00000064, /* program size */
  0x00000000, /* trailing loads size */
  0xff110c00, /* start address */
  0x014c0601, 0xe15bde81, 0x01540001, 0xe15bde82, 0x015ffffd, 0xe15bde83, 0x0163fffd, 0xe15bde84,
  0x01640001, 0xe15bde85, 0x015ffff9, 0xe15bde86, 0x0167fffd, 0xe15bde87, 0x01600001, 0xe15bde88,
  0x015ffffd, 0xe15bde89, 0x01640001, 0xe15bde8a, 0x01540005, 0xe15bde8b, 0x01080000, 0x001fde8b,
  /* trailing loads */
};

/* Use the same reset code binaries (placeholder for different reset codes) */
#define XcCode_xc0_rpu_reset0 XcCode_rpu_reset0
#define XcCode_xc0_rpu_reset1 XcCode_rpu_reset1

#define XcCode_xc0_tpu_reset0 XcCode_tpu_reset0
#define XcCode_xc0_tpu_reset1 XcCode_tpu_reset1

static const uint32_t* paulxMacRpuCodes[4]=
{
  XcCode_xc0_rpu_reset0,
  XcCode_xc0_rpu_reset1
};

static const uint32_t* paulxMacTpuCodes[4]=
{
  XcCode_xc0_tpu_reset0,
  XcCode_xc0_tpu_reset1
};

#endif

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

int HALDEF(XC_ResetDummy)( unsigned int uXcNo, unsigned int uPortNo, void* pvUser )
{
  /* XC Reset Disabled */
  return 0;
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
int HALDEF(XC_Reset)( unsigned int uXcNo, unsigned int uXcPortNo, void* pvUser )
{
  HW_TYPE(XMAC_AREA_T)* ptXmac;
  HW_TYPE(XPEC_AREA_T)* ptRpec;
  HW_TYPE(XPEC_AREA_T)* ptTpec;
  volatile uint32_t* pulRpecPram;
  volatile uint32_t* pulTpecPram;
  unsigned int uIdx;
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  if( uPortNo >= NUM_XPECS ) return -1;

  ptXmac      = s_aptXmacArea[uPortNo];
  ptRpec      = s_aptRpecRegArea[uPortNo];
  ptTpec      = s_aptTpecRegArea[uPortNo];
  pulRpecPram = s_apulRpecPramArea[uPortNo];
  pulTpecPram = s_apulTpecPramArea[uPortNo];

  /* Stop xPEC and xMAC */
  s_ptXcStartStop->ulXc_start_stop_ctrl = ( HW_MSK(xc_start_stop_ctrl_xc0_stop_rpec0) |
                                            HW_MSK(xc_start_stop_ctrl_xc0_stop_tpec0) |
                                            HW_MSK(xc_start_stop_ctrl_xc0_stop_rpu0)  |
                                            HW_MSK(xc_start_stop_ctrl_xc0_stop_tpu0)
                                           ) << (4 * uPortNo);

  /* Clear output enable of tx asap */
  ptXmac->ulXmac_config_obu  = HW_DFLT_VAL(xmac_config_obu);

  /* reset mii_txd_oe_next to avoid MII.TXEN spike (config_mii[2] = 1 resets internal OBU signal mii_txd_oe_nxt) */
  ptXmac->ulXmac_config_mii  = HW_DFLT_VAL(xmac_config_mii) | (1U << 2);

  /* Clear output enable of io0..5 asap */
  ptRpec->aulStatcfg[uXcPortNo] = 0xffff0000;

  switch( uXcPortNo )
  {
    case 0: ptRpec->ulXpec_config = HW_MSK(xpec_config_reset_urx_fifo0) | HW_MSK(xpec_config_reset_utx_fifo0);
            break;
    case 1: ptRpec->ulXpec_config = HW_MSK(xpec_config_reset_urx_fifo1) | HW_MSK(xpec_config_reset_utx_fifo1);
            break;
  }

  /* load rate multiplier reset code */
  if( HALDEF(XC_Load)( uXcNo, uXcPortNo, XC_TYPE_RPU, paulxMacRpuCodes[uPortNo], pvUser) != 0) {
    return -2;
  }
  if( HALDEF(XC_Load)( uXcNo, uXcPortNo, XC_TYPE_TPU, paulxMacTpuCodes[uPortNo], pvUser) != 0) {
    return -3;
  }

  ptXmac->ulXmac_rpu_pc      = 0; /* Reset PC to 0 */
  ptXmac->ulXmac_tpu_pc      = 0; /* Reset PC to 0 */

  s_ptXcStartStop->ulXc_start_stop_ctrl = (HW_MSK(xc_start_stop_ctrl_xc0_start_rpu0) |
                                           HW_MSK(xc_start_stop_ctrl_xc0_start_tpu0)) << (4 * uPortNo);

  /* !!!! ATTENTION: There must be enough time between starting xMAC and stopping xMAC to execute reset program */

  pulRpecPram[0] = 0xC0000FFF;                                   /* Use the command wait b000000000000,b111111111111 at Address 0*/
  pulTpecPram[0] = 0xC0000FFF;                                   /* Use the command wait b000000000000,b111111111111 at Address 0*/

  ptRpec->ulXpec_pc = 0x7ff;                                     /* Reset the Program Counter to 0x7ff */
  ptTpec->ulXpec_pc = 0x7ff;                                     /* Reset the Program Counter to 0x7ff */


  /* let the XC run for at least 10 cycles */
  for (uIdx = 0; uIdx < 10; uIdx++)
  {
    s_ptXcStartStop->ulXc_start_stop_ctrl = (HW_MSK(xc_start_stop_ctrl_xc0_start_rpec0) |
                                             HW_MSK(xc_start_stop_ctrl_xc0_start_tpec0)) << (4 * uPortNo);
  }

  s_ptXcStartStop->ulXc_start_stop_ctrl = (HW_MSK(xc_start_stop_ctrl_xc0_stop_rpec0) |
                                           HW_MSK(xc_start_stop_ctrl_xc0_stop_tpec0)) << (4 * uPortNo);
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

  /* Reset shared registers */
  /* Reset SR0-3 for XPEC0, SR4-7 for XPEC1 */
  for( uIdx = (4 * uXcPortNo); uIdx < (4 * uXcPortNo + 4); ++uIdx )
    ptRpec->aulXpec_sr[uIdx] = 0;
  /* Reset SR8-11 for XPEC0, SR12-15 for XPEC1 */
  for( uIdx = (8 + 4 * uXcPortNo); (uIdx < 12 + 4 * uXcPortNo); ++uIdx )
    ptRpec->aulXpec_sr[uIdx] = 0;

  ptRpec->ulDatach_wr_cfg = 0;
  ptTpec->ulDatach_wr_cfg = 0;
  ptRpec->ulDatach_rd_cfg = 0;
  ptTpec->ulDatach_rd_cfg = 0;
  ptRpec->ulSysch_addr    = 0;
  ptTpec->ulSysch_addr    = 0;

  /* confirm all interrupts from xPEC -> Select the specific XC instance and get IRQ */
  s_ptXpecIrqRegs->aulIrq_xpec[uPortNo] = 0x0000FFFF;

  /* hold xMAC */
  s_ptXcStartStop->ulXc_start_stop_ctrl = (HW_MSK(xc_start_stop_ctrl_xc0_stop_rpu0) |
                                           HW_MSK(xc_start_stop_ctrl_xc0_stop_tpu0)) << (4 * uPortNo);

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
  ptXmac->ulXmac_config_rx_nibble_fifo  = HW_DFLT_VAL(xmac_config_rx_nibble_fifo);
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

  ptXmac->ulXmac_config_sbu2         = HW_DFLT_VAL(xmac_config_sbu2);
  ptXmac->ulXmac_config_obu2         = HW_DFLT_VAL(xmac_config_obu2);

  ptXmac->ulXmac_rpu_pc              = 0;
  ptXmac->ulXmac_tpu_pc              = 0;

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
int HALDEF(XC_Load)(unsigned int uXcNo, unsigned int uXcPortNo, XC_TYPE_E eXcType, const uint32_t* pulXcPrg, void* pvUser)
{
  volatile uint32_t *pulDst, *pulDstCnt;
  volatile uint32_t *pulRamStart, *pulRamEnd;
  const uint32_t *pulSrcStart, *pulSrcCnt, *pulSrcEnd;
  unsigned int uiElements;
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  /* check the instance number */
  if( uPortNo >= NUM_XPECS ) return 1;

  /* get the start and end address of the ram area, get the physical address */
  switch( eXcType )
  {
    case XC_TYPE_RPEC: pulRamStart     = s_apulRpecPramArea[uPortNo];
                       pulRamEnd       = s_apulRpecPramAreaEnd[uPortNo]+1;
                       break;
    case XC_TYPE_TPEC: pulRamStart     = s_apulTpecPramArea[uPortNo];
                       pulRamEnd       = s_apulTpecPramAreaEnd[uPortNo]+1;
                       break;
    case XC_TYPE_RPU:  pulRamStart     = s_aptRpuPramArea[uPortNo];
                       pulRamEnd       = s_aptRpuPramAreaEnd[uPortNo]+1;
                       break;
    case XC_TYPE_TPU:  pulRamStart     = s_aptTpuPramArea[uPortNo];
                       pulRamEnd       = s_aptTpuPramAreaEnd[uPortNo]+1;
                       break;

    default:           return 2; /* unknown unit type */
  }

  /* get the number of code elements */
  uiElements = pulXcPrg[0] / sizeof(unsigned long) - 1;

  /* get the pointer in the xc area */
  /* ram_virtual_start + code_physical_start - ram_physical_start */
  pulDst = (volatile uint32_t*) pulXcPrg[2];

  /* the code must fit into the ram area */
  if( (pulDst<pulRamStart) || ((pulDst+uiElements)>pulRamEnd) ) {
    /* the code exceeds the program ram! */
    return 3;
  }

  /* get source start and end pointer */
  pulSrcStart = pulXcPrg + 3;
  pulSrcEnd = pulSrcStart + uiElements;

  /* copy the code to xc ram */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt<pulSrcEnd ) {
    *pulDstCnt = *pulSrcCnt;
    pulDstCnt++;
    pulSrcCnt++;
  }

#ifndef XC_LOAD_NO_COMPARE
  /* compare the code */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt<pulSrcEnd ) {
    if( *pulDstCnt != *pulSrcCnt )
    {
      return 4;
    }
    pulDstCnt++;
    pulSrcCnt++;
  }
#endif

  /* get the number of trailing loads */
  uiElements = pulXcPrg[1] / sizeof(unsigned long);

  /* get source start and end pointer */
  pulSrcCnt = pulXcPrg + 2 + pulXcPrg[0] / sizeof(unsigned long);
  pulSrcEnd = pulSrcCnt + uiElements;

  /* write all trailing loads */
  while( pulSrcCnt<pulSrcEnd ) {
    /* get the destination address ( ram_virtual_start + data_physical_start - ram_physical_start) */
    pulDst = (volatile uint32_t*) *pulSrcCnt;
    pulSrcCnt++;

    /* write the data */
    *pulDst = *pulSrcCnt;
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
int HALDEF(XC_Start)(unsigned int uXcNo, unsigned int uXcPortNo, void* pvUser)
{
  HW_TYPE(XMAC_AREA_T)* ptXmacArea;
  HW_TYPE(XPEC_AREA_T)* ptRpecArea;
  HW_TYPE(XPEC_AREA_T)* ptTpecArea;
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  if( uPortNo >= NUM_XPECS ) return -1;

  ptXmacArea = s_aptXmacArea[uPortNo];
  ptRpecArea = s_aptRpecRegArea[uPortNo];
  ptTpecArea = s_aptTpecRegArea[uPortNo];

  /* reset pc of units */
  ptXmacArea->ulXmac_rpu_pc = 0;
  ptXmacArea->ulXmac_tpu_pc = 0;
  ptRpecArea->ulXpec_pc     = 0x7ff;
  ptTpecArea->ulXpec_pc     = 0x7ff;

  /* start units */
  s_ptXcStartStop->ulXc_start_stop_ctrl = ( (HW_MSK(xc_start_stop_ctrl_xc0_start_rpec0) |
                                             HW_MSK(xc_start_stop_ctrl_xc0_start_tpec0) |
                                             HW_MSK(xc_start_stop_ctrl_xc0_start_rpu0)  |
                                             HW_MSK(xc_start_stop_ctrl_xc0_start_tpu0)
                                           ) << (4 * uPortNo)
                                          );

  return 0;
}

/*****************************************************************************/
/*! Start XC Units
* \description
*   Start XC Units.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   uUnitVec             [in]  Bit vector specifiying the units to start
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(XC_StartUnits)(unsigned int uXcNo, unsigned int uXcPortNo, unsigned int uUnitVec, void* pvUser)
{
  uint32_t ulCtrlMsk = 0;
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  /* check instance number */
  if( uPortNo >= NUM_XPECS ) return -1;

  if( 0 != (uUnitVec & MSK_XC_UNIT_XMAC) )
  {
    ulCtrlMsk |= HW_MSK(xc_start_stop_ctrl_xc0_start_tpu0);
    ulCtrlMsk |= HW_MSK(xc_start_stop_ctrl_xc0_start_rpu0);
  }

  if( 0 != (uUnitVec & MSK_XC_UNIT_XPEC) )
  {
    ulCtrlMsk |= HW_MSK(xc_start_stop_ctrl_xc0_start_tpec0);
    ulCtrlMsk |= HW_MSK(xc_start_stop_ctrl_xc0_start_rpec0);
  }

  s_ptXcStartStop->ulXc_start_stop_ctrl = (ulCtrlMsk << (4 * uPortNo));

  return 0;
}

/*****************************************************************************/
/*! Start XC Units
* \description
*   Start a set of Units for one XC port.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   uUnitVec             [in]  Bit vector defining the units
*                              bit 0: rPEC, bit 1: tPEC, bit 2: rPU, bit 3: tPU
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(XC_StartUnits2)(unsigned int uXcNo, unsigned int uXcPortNo, unsigned int uUnitVec, void* pvUser)
{
  uint32_t ulCtrlMsk = 0;
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  /* check instance number */
  if( uPortNo >= NUM_XPECS ) return -1;

  if( uUnitVec & MSK_XC_TYPE_TPU )
    ulCtrlMsk |= HW_MSK(xc_start_stop_ctrl_xc0_start_tpu0);

  if( uUnitVec & MSK_XC_TYPE_RPU )
    ulCtrlMsk |= HW_MSK(xc_start_stop_ctrl_xc0_start_rpu0);

  if( uUnitVec & MSK_XC_TYPE_TPEC )
    ulCtrlMsk |= HW_MSK(xc_start_stop_ctrl_xc0_start_tpec0);

  if( uUnitVec & MSK_XC_TYPE_RPEC )
    ulCtrlMsk |= HW_MSK(xc_start_stop_ctrl_xc0_start_rpec0);

  s_ptXcStartStop->ulXc_start_stop_ctrl = (ulCtrlMsk << (4 * uPortNo));

  return 0;
}

/*****************************************************************************/
/*! Stop XC Port
* \description
*   Stop XC port.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(XC_Stop)(unsigned int uXcNo, unsigned int uXcPortNo, void* pvUser)
{
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  if(uPortNo>= NUM_XPECS) {
    return -1;
  }

  /* stop units */
  s_ptXcStartStop->ulXc_start_stop_ctrl = ( (HW_MSK(xc_start_stop_ctrl_xc0_stop_rpec0) |
                                             HW_MSK(xc_start_stop_ctrl_xc0_stop_tpec0) |
                                             HW_MSK(xc_start_stop_ctrl_xc0_stop_rpu0)  |
                                             HW_MSK(xc_start_stop_ctrl_xc0_stop_tpu0)
                                            ) << (4 * uPortNo)
                                          );

  return 0;
}

/*****************************************************************************/
/*! Read XC Register
* \description
*  Reads a processor register of the XC.
* \params
*   uPortNo               [in]    XC Port Number
*   eXcType               [in]    XC ALU type
*   eReg                  [in]    XC Register name
*   pulValue              [out]   Register value (return value)
* \return
*   0 on success
*   -1 on erroneous                                         */
/*****************************************************************************/
int HALDEF(XC_GetReg) ( unsigned int uXcNo, unsigned int uXcPortNo, XC_TYPE_E eXcType, XC_REG_E eReg, uint32_t * pulValue )
{
  int iRslt = 0;
  HW_TYPE(XMAC_AREA_T)* ptXmac = NULL;
  HW_TYPE(XPEC_AREA_T)* ptXpec = NULL;
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  if( uPortNo >= NUM_XPECS ) return -1;

  switch( eXcType )
  {
    /* xPec */
    case XC_TYPE_RPEC:
    case XC_TYPE_TPEC:
      ptXpec = ( eXcType == XC_TYPE_RPEC ) ? s_aptRpecRegArea[uPortNo]
                                           : s_aptTpecRegArea[uPortNo];
      switch( eReg )
      {
        /* Get the correct parameter */
        case XC_XPEC_REG_R0:  *pulValue = ptXpec->aulXpec_r[0]; break;
        case XC_XPEC_REG_R1:  *pulValue = ptXpec->aulXpec_r[1]; break;
        case XC_XPEC_REG_R2:  *pulValue = ptXpec->aulXpec_r[2]; break;
        case XC_XPEC_REG_R3:  *pulValue = ptXpec->aulXpec_r[3]; break;
        case XC_XPEC_REG_R4:  *pulValue = ptXpec->aulXpec_r[4]; break;
        case XC_XPEC_REG_R5:  *pulValue = ptXpec->aulXpec_r[5]; break;
        case XC_XPEC_REG_R6:  *pulValue = ptXpec->aulXpec_r[6]; break;
        case XC_XPEC_REG_R7:  *pulValue = ptXpec->aulXpec_r[7]; break;
        case XC_XPEC_REG_PC:  *pulValue = ptXpec->ulXpec_pc; break;
        case XC_XPEC_REG_IRQ: *pulValue = ptXpec->ulIrq; break;
        default: iRslt = -1; break;
      }
      break;

    /* xMac */
    case XC_TYPE_RPU:
    case XC_TYPE_TPU:
      ptXmac = s_aptXmacArea[uPortNo];
      switch( eReg )
      {
        /* Get the correct parameter */
        case XC_XMAC_REG_CONFIG_SBU: *pulValue = ptXmac->ulXmac_config_sbu; break;
        case XC_XMAC_REG_CONFIG_MII: *pulValue = ptXmac->ulXmac_config_mii; break;
        case XC_XMAC_REG_RX_HW: *pulValue = ptXmac->ulXmac_rx_hw; break;
        case XC_XMAC_REG_TX_HW: *pulValue = ptXmac->ulXmac_tx_hw; break;
        case XC_XMAC_REG_RPU_PC: *pulValue = ptXmac->ulXmac_rpu_pc; break;
        case XC_XMAC_REG_TPU_PC: *pulValue = ptXmac->ulXmac_tpu_pc; break;
        default: iRslt = -1; break;
      }
      break;

    default:
      iRslt = -1;
      break;
  } /* End of switch */

  return iRslt;
}

/*****************************************************************************/
/*! Read XC Register
* \description
*  Reads a processor register of the XC.
* \params
*   uPortNo               [in]    XC Port Number
*   eXcType               [in]    XC ALU type
*   eReg                  [in]    XC Register name
*   ulValue               [in]    Register value (to be set)
* \return
*   0 on success
*   -1 on erroneous                                         */
/*****************************************************************************/
int HALDEF(XC_SetReg) ( unsigned int uXcNo, unsigned int uXcPortNo, XC_TYPE_E eXcType, XC_REG_E eReg, uint32_t ulValue )
{
  int iRslt = 0;
  HW_TYPE(XMAC_AREA_T)* ptXmac = NULL;
  HW_TYPE(XPEC_AREA_T)* ptXpec = NULL;
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  if( uPortNo >= NUM_XPECS ) return -1;

  switch( eXcType )
  {
    /* xPEC */
    case XC_TYPE_RPEC:
    case XC_TYPE_TPEC:
      ptXpec = ( eXcType == XC_TYPE_RPEC ) ? s_aptRpecRegArea[uPortNo]
                                           : s_aptTpecRegArea[uPortNo];
      switch( eReg )
      {
        /* Set the correct parameter */
        case XC_XPEC_REG_R0:  ptXpec->aulXpec_r[0] = ulValue; break;
        case XC_XPEC_REG_R1:  ptXpec->aulXpec_r[1] = ulValue; break;
        case XC_XPEC_REG_R2:  ptXpec->aulXpec_r[2] = ulValue; break;
        case XC_XPEC_REG_R3:  ptXpec->aulXpec_r[3] = ulValue; break;
        case XC_XPEC_REG_R4:  ptXpec->aulXpec_r[4] = ulValue; break;
        case XC_XPEC_REG_R5:  ptXpec->aulXpec_r[5] = ulValue; break;
        case XC_XPEC_REG_R6:  ptXpec->aulXpec_r[6] = ulValue; break;
        case XC_XPEC_REG_R7:  ptXpec->aulXpec_r[7] = ulValue; break;
        case XC_XPEC_REG_PC:  ptXpec->ulXpec_pc = ulValue; break;
        case XC_XPEC_REG_IRQ: ptXpec->ulIrq = ulValue; break;
        default: iRslt = -1; break;
      }
      break;

    /* xMAC */
    case XC_TYPE_RPU:
    case XC_TYPE_TPU:
      ptXmac = s_aptXmacArea[uPortNo];
      switch( eReg )
      {
        /* Set the correct parameter */
        case XC_XMAC_REG_CONFIG_SBU: ptXmac->ulXmac_config_sbu = ulValue; break;
        case XC_XMAC_REG_CONFIG_OBU: ptXmac->ulXmac_config_obu = ulValue; break;
        case XC_XMAC_REG_OBU_RATE_MUL_ADD: ptXmac->ulXmac_obu_rate_mul_add = ulValue; break;
        case XC_XMAC_REG_CONFIG_MII: ptXmac->ulXmac_config_mii = ulValue; break;
        case XC_XMAC_REG_RX_HW: ptXmac->ulXmac_rx_hw = ulValue; break;
        case XC_XMAC_REG_TX_HW: ptXmac->ulXmac_tx_hw = ulValue; break;
        case XC_XMAC_REG_RPU_PC: ptXmac->ulXmac_rpu_pc = ulValue; break;
        case XC_XMAC_REG_TPU_PC: ptXmac->ulXmac_tpu_pc = ulValue; break;
        default: iRslt = -1; break;
      }
      break;

    default:
      iRslt = -1;
      break;
  }

  return iRslt;
}

/*****************************************************************************/
/*! Write xPEC DRAM DWord wise
* \description
*   Write xPEC DRAm DW-wise.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   uOffset              [in]  DRAM Offset
*   ulVal                [in]  Value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(XC_WrXpecDramDw)( unsigned int uXcNo, unsigned int uXcPortNo, unsigned int uOffset, uint32_t ulVal )
{
  volatile uint32_t* pulXpecDram;
  unsigned int uDwOffset = uOffset/sizeof(uint32_t);
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  /* check instance number and Offset */
  if( uPortNo>=NUM_XPECS ) return -1;
  if( uDwOffset>=XPEC_DWORD_RAMSIZE ) return -1;

  pulXpecDram = s_apulXpecDramArea[uPortNo];

  /* Write value */
  pulXpecDram[uDwOffset]=ulVal;

  return 0;
}

/*****************************************************************************/
/*! Read xPEC DRAM DWord wise
* \description
*   Read xPEC DRAM DW-wise.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   uOffset              [in]  DRAM Offset
* \return
*   Read value                                                               */
/*****************************************************************************/
uint32_t HALDEF(XC_RdXpecDramDw)( unsigned int uXcNo, unsigned int uXcPortNo, unsigned int uOffset )
{
  volatile uint32_t* pulXpecDram;
  unsigned int uDwOffset = uOffset/sizeof(uint32_t);
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  /* check instance number and Offset */
  if( uPortNo >= NUM_XPECS ) return 0;
  if( uDwOffset >= XPEC_DWORD_RAMSIZE ) return 0;

  pulXpecDram = s_apulXpecDramArea[uPortNo];

  /* Read value */
  return pulXpecDram[uDwOffset];
}

/*****************************************************************************/
/*! Write rPEC DRAM DWord wise
* \description
*   Write rPEC DRAm DW-wise.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   uOffset              [in]  DRAM Offset
*   ulVal                [in]  Value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(XC_WrRpecDramDw)( unsigned int uXcNo, unsigned int uXcPortNo, unsigned int uOffset, uint32_t ulVal )
{
  volatile uint32_t* pulRpecDram;
  unsigned int uDwOffset = uOffset/sizeof(uint32_t);
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  /* check instance number and Offset */
  if( uPortNo >= NUM_XPECS ) return -1;
  if( uDwOffset >= XPEC_DWORD_RAMSIZE ) return -1;

  pulRpecDram = s_apulRpecDramArea[uPortNo];

  /* Write value */
  pulRpecDram[uDwOffset]=ulVal;

  return 0;
}

/*****************************************************************************/
/*! Read rPEC DRAM DWord wise
* \description
*   Read rPEC DRAM DW-wise.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   uOffset              [in]  DRAM Offset
* \return
*   Read value                                                               */
/*****************************************************************************/
uint32_t HALDEF(XC_RdRpecDramDw)( unsigned int uXcNo, unsigned int uXcPortNo, unsigned int uOffset )
{
  volatile uint32_t* pulRpecDram;
  unsigned int uDwOffset = uOffset/sizeof(uint32_t);
  unsigned int uPortNo = uXcNo * 2 + uXcPortNo;

  /* check instance number and Offset */
  if( uPortNo >= NUM_XPECS ) return 0;
  if( uDwOffset >= XPEC_DWORD_RAMSIZE ) return 0;

  pulRpecDram = s_apulRpecDramArea[uPortNo];

  /* Read value */
  return pulRpecDram[uDwOffset];
}

/*****************************************************************************/
/*! Set 'xc_start_stop_ctrl' Register
* \description
*   Read xPEC DRAM DW-wise.
* \class
*   XC
* \params
*   uPortNo              [in]  XC Port Number
*   ulValue              [in]  Register set mask
* \return
*   Read value                                                               */
/*****************************************************************************/
int HALDEF(XC_StartStopSet) ( uint32_t ulValue )
{
  /* Set value */
  s_ptXcStartStop->ulXc_start_stop_ctrl = ulValue;

  return ( 0 );
}

/*****************************************************************************/
/*! Get 'xc_hold_status' Register
* \description
*   Read xPEC DRAM DW-wise.
* \class
*   XC
* \params
*   pulValue              [out]   Register value (return value)
* \return
*   Read value                                                               */
/*****************************************************************************/
int HALDEF(XC_StartStopGet) ( uint32_t * pulValue )
{
  /* Get Register */
  *pulValue = s_ptXcStartStop->ulXc_hold_status;

  return ( 0 );
}

/*****************************************************************************/
/*! Set 'xc_systime_config' Register
* \description
*   Set Systime Instance for XC instance
* \class
*   XC
* \params
*   uXcInstNo              [in]  XC Instance Number
*   uSystimeInstNo         [in]  Systime Instance Number
* \return
*   int                                                                      */
/*****************************************************************************/
int HALDEF(XC_SystimeConfigSet)( unsigned int uXcInstNo, unsigned int uSystimeInstNo )
{
  int iRslt = 0;
  uint32_t ulVal = s_ptXcSystimeConfig->ulXc_systime_config;

  /* Check the instance number */
  if( uSystimeInstNo >= 4 ) return -1;

  /* Set value */
  switch( uXcInstNo )
  {
    case 0:
      ulVal &= ~HW_MSK(xc_systime_config_xc0_systime_config);
      ulVal |= (uint32_t)uSystimeInstNo << HW_SRT(xc_systime_config_xc0_systime_config);
      s_ptXcSystimeConfig->ulXc_systime_config = ulVal;
    break;

#if defined(__NETX4000)
    case 1:
      ulVal &= ~HW_MSK(xc_systime_config_xc1_systime_config);
      ulVal |= (uint32_t)uSystimeInstNo << HW_SRT(xc_systime_config_xc1_systime_config);
      s_ptXcSystimeConfig->ulXc_systime_config = ulVal;
    break;
#endif
    default:
      iRslt = -1;
    break;
  }

  return iRslt;
}


/*****************************************************************************/
/*! Load XC Channel Codes
* \description
*   Load rPEC, tPEC, RPU ind TPU of one XC port.
* \class
*   XC
* \params
*   uXcNo                [in]  XC Instance Number
*   uXcPortNo            [in]  XC Port Number
*   ptPrg                [in]  Pointer To Microcodes
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(XC_LoadChannel) ( unsigned int uXcNo, unsigned int uXcPortNo, XC_PRG_T* ptPrg, void* pvUser )
{
  int iError;

  iError = HALDEF(XC_Load)(uXcNo, uXcPortNo, XC_TYPE_RPEC, ptPrg->pulRpecPrg, pvUser);
  if( iError ) return iError;
  iError = HALDEF(XC_Load)(uXcNo, uXcPortNo, XC_TYPE_TPEC, ptPrg->pulTpecPrg, pvUser);
  if( iError ) return iError + 5;
  iError = HALDEF(XC_Load)(uXcNo, uXcPortNo, XC_TYPE_RPU, ptPrg->pulRpuPrg, pvUser);
  if( iError ) return iError + 10;
  iError = HALDEF(XC_Load)(uXcNo, uXcPortNo, XC_TYPE_TPU, ptPrg->pulTpuPrg, pvUser);
  if( iError ) return iError + 15;
  return 0;
}

/*****************************************************************************/
/*! Set FbClk
* \description
*   Configure 400MHz rate multiplier for FB clock.
* \class
*   XC
* \params
*   ulFbClkRateMulAdd  [in]  Value for Rate multiplier
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(XC_SetFbClk)(unsigned int uXc, unsigned int uXpec, uint32_t ulFbClkRateMulAdd, uint32_t ulFbClkDiv)
{
  if( uXpec<2 )
  {
#if defined(__NETX4000)
    switch( uXc )
    {
      case 0:
        s_ptXcExtCfg->asFb0[uXpec].ulClk_rate_mul_add = ulFbClkRateMulAdd;
        s_ptXcExtCfg->asFb0[uXpec].ulClk_div          = ulFbClkDiv;
      break;
      case 1:
        s_ptXcExtCfg->asFb1[uXpec].ulClk_rate_mul_add = ulFbClkRateMulAdd;
        s_ptXcExtCfg->asFb1[uXpec].ulClk_div          = ulFbClkDiv;
      break;
      default:
      break;
    }
#else
    s_ptXcExtCfg->asFb[uXpec].ulClk_rate_mul_add = ulFbClkRateMulAdd;
    s_ptXcExtCfg->asFb[uXpec].ulClk_div          = ulFbClkDiv;
#endif
  }
}
