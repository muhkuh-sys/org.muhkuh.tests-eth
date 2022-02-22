/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "regdef_netx51.h"
#include "netx51_xc.h"

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX51_NUM_XPECS 2

#define NX51_XMAC_RPU_DWORD_RAMSIZE 0x0100
#define NX51_XMAC_TPU_DWORD_RAMSIZE 0x0100
#define NX51_XPEC_DWORD_RAMSIZE     0x0800

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

/*****************************************************************************/
#include "hal_resources_defines_netx51.h"
__USE_XC_START_STOP
__USE_RPEC_PRAM_START
__USE_TPEC_PRAM_START
__USE_RPEC_PRAM_END
__USE_TPEC_PRAM_END
__USE_RPEC_REGS
__USE_TPEC_REGS
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
*   uiPort           [in]  XC Port Number
*   pvUser           [in]  User Specific Parameters
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_XC_Reset( unsigned int uiPort,
                   void*        pvUser )
{
  NX51_XMAC_AREA_T* ptXmac;
  NX51_XPEC_AREA_T* ptRpec;
  NX51_XPEC_AREA_T* ptTpec;
  volatile uint32_t* pulRpecPram;
  volatile uint32_t* pulTpecPram;
  unsigned int uIdx;

  if( uiPort >= NX51_NUM_XPECS)
    return -1;

  ptXmac      = s_aptXmacArea[uiPort];
  ptRpec      = s_aptRpecRegArea[uiPort];
  ptTpec      = s_aptTpecRegArea[uiPort];
  pulRpecPram = s_apulRpecPramArea[uiPort];
  pulTpecPram = s_apulTpecPramArea[uiPort];

  /* Stop xPEC and xMAC */
  s_ptXcStartStop->ulXc_start_stop_ctrl = (  MSK_NX51_xc_start_stop_ctrl_stop_rpec0
                                           | MSK_NX51_xc_start_stop_ctrl_stop_tpec0
                                           | MSK_NX51_xc_start_stop_ctrl_stop_rpu0
                                           | MSK_NX51_xc_start_stop_ctrl_stop_tpu0 ) << (4*uiPort);

  /* Clear output enable of TX as soon as possible */
  ptXmac->ulXmac_config_obu  = DFLT_VAL_NX51_xmac_config_obu;
  /* Clear output enable of IO 0..5 as soon as possible */

  ptRpec->aulStatcfg[uiPort] = 0xffff0000;

  switch (uiPort)
  {
    case 0: ptRpec->ulXpec_config = MSK_NX51_xpec_config_reset_urx_fifo0 | MSK_NX51_xpec_config_reset_utx_fifo0;
            break;
    case 1: ptRpec->ulXpec_config = MSK_NX51_xpec_config_reset_urx_fifo1 | MSK_NX51_xpec_config_reset_utx_fifo1;
            break;
  }

  /* load rate multiplier reset code */
  if( NX51_XC_Load( uiPort, NX51_XC_TYPE_RPU, (uint32_t*)paulxMacRpuCodes[uiPort], pvUser)!=0 ) {
    return -1;
  }
  if( NX51_XC_Load( uiPort, NX51_XC_TYPE_TPU, (uint32_t*)paulxMacTpuCodes[uiPort], pvUser)!=0 ) {
    return -1;
  }

  ptXmac->ulXmac_rpu_pc      = 0; /* Reset PC to 0 */
  ptXmac->ulXmac_tpu_pc      = 0; /* Reset PC to 0 */

  s_ptXcStartStop->ulXc_start_stop_ctrl = (  MSK_NX51_xc_start_stop_ctrl_start_rpu0
                                           | MSK_NX51_xc_start_stop_ctrl_start_tpu0 ) << (4*uiPort);

  /* !!!! ATTENTION: There must be enough time between starting xMAC and stopping xMAC to execute reset program */

  pulRpecPram[0] = 0xC0000FFF;                                   /* Use the command wait b000000000000,b111111111111 at Address 0*/
  pulTpecPram[0] = 0xC0000FFF;                                   /* Use the command wait b000000000000,b111111111111 at Address 0*/

  ptRpec->ulXpec_pc = 0x7ff;                                     /* Reset the Program Counter to 0x7ff */
  ptTpec->ulXpec_pc = 0x7ff;                                     /* Reset the Program Counter to 0x7ff */


  /* let the XC run for at least 10 cycles */
  for (uIdx = 0; uIdx < 10; uIdx++)
  {
    s_ptXcStartStop->ulXc_start_stop_ctrl = (  MSK_NX51_xc_start_stop_ctrl_start_rpec0
                                             | MSK_NX51_xc_start_stop_ctrl_start_tpec0 ) << (4*uiPort);
  }

  s_ptXcStartStop->ulXc_start_stop_ctrl = (  MSK_NX51_xc_start_stop_ctrl_stop_rpec0
                                           | MSK_NX51_xc_start_stop_ctrl_stop_tpec0 ) << (4*uiPort);
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

  switch (uiPort)
  {
    case 0: ptRpec->ulXpec_stat_bits_shared = 0x00ff0000;
            break;
    case 1: ptRpec->ulXpec_stat_bits_shared = 0xff000000;
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

  /* Reset shared registers, reset URX/UTX FIFOs */
  /* Reset SR0-3 for XC0, SR4-7 for XC1 */
  for( uIdx = 4 * uiPort; uIdx < 4 * uiPort + 4; ++uIdx )
    ptRpec->aulXpec_sr[uIdx] = 0;
  /* Reset SR8-11 for XC0, SR12-15 for XC1 */
  for( uIdx = 8 + 4 * uiPort; uIdx < 12 + 4 * uiPort; ++uIdx )
    ptRpec->aulXpec_sr[uIdx] = 0;

  ptRpec->ulDatach_wr_cfg = 0;
  ptTpec->ulDatach_wr_cfg = 0;
  ptRpec->ulDatach_rd_cfg = 0;
  ptTpec->ulDatach_rd_cfg = 0;
  ptRpec->ulSysch_addr    = 0;
  ptTpec->ulSysch_addr    = 0;

  /* confirm all interrupts from xPEC */
  s_ptXpecIrqRegs->aulIrq_xpec[uiPort] = 0x0000FFFF;

  /* hold xMAC */
  s_ptXcStartStop->ulXc_start_stop_ctrl = (  MSK_NX51_xc_start_stop_ctrl_stop_rpu0
                                           | MSK_NX51_xc_start_stop_ctrl_stop_tpu0 ) << (4*uiPort);

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
  ptXmac->ulXmac_config_rx_nibble_fifo  = DFLT_VAL_NX51_xmac_config_rx_nibble_fifo;
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

  ptXmac->ulXmac_config_sbu2         = DFLT_VAL_NX51_xmac_config_sbu2;
  ptXmac->ulXmac_config_obu2         = DFLT_VAL_NX51_xmac_config_obu2;

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
*   uiPort               [in]  XC Port Number
*   eXcType              [in]  XC Entity To Download To (RPU, TPU, xPEC)
*   pulXcPrg             [in]  Pointer To Microcode
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_XC_Load( unsigned int    uiPort,
                  NX51_XC_TYPE_E  eXcType,
                  const uint32_t* pulXcPrg,
                  void*           pvUser )
{
  volatile uint32_t *pulDst, *pulDstCnt;
  volatile uint32_t *pulRamStart, *pulRamEnd;
  const uint32_t *pulSrcStart, *pulSrcCnt, *pulSrcEnd;
  unsigned int uiElements;

  /* check the instance number */
  if(uiPort>= NX51_NUM_XPECS) return -1;

  /* get the start and end address of the ram area, get the physical address */
  switch( eXcType )
  {
    case NX51_XC_TYPE_RPEC: pulRamStart     = s_apulRpecPramArea[uiPort];
                            pulRamEnd       = s_apulRpecPramAreaEnd[uiPort]+1;
                            break;
    case NX51_XC_TYPE_TPEC: pulRamStart     = s_apulTpecPramArea[uiPort];
                            pulRamEnd       = s_apulTpecPramAreaEnd[uiPort]+1;
                            break;
    case NX51_XC_TYPE_RPU:  pulRamStart     = s_aptRpuPramArea[uiPort];
                            pulRamEnd       = s_aptRpuPramAreaEnd[uiPort]+1;
                            break;
    case NX51_XC_TYPE_TPU:  pulRamStart     = s_aptTpuPramArea[uiPort];
                            pulRamEnd       = s_aptTpuPramAreaEnd[uiPort]+1;
                            break;

    default:           return -1; /* unknown unit type */
  }

  /* get the number of code elements */
  uiElements = pulXcPrg[0] / sizeof(uint32_t) - 1;

  /* get the pointer in the XC area */
  /* ram_virtual_start + code_physical_start - ram_physical_start */
  pulDst = (volatile uint32_t*) pulXcPrg[2];

  /* the code must fit into the ram area */
  if( (pulDst<pulRamStart) || ((pulDst+uiElements)>pulRamEnd) ) {
    /* the code exceeds the xPEC ram! */
    return -1;
  }

  /* get source start and end pointer */
  pulSrcStart = pulXcPrg + 3;
  pulSrcEnd = pulSrcStart + uiElements;

  /* copy the code to XC ram */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt<pulSrcEnd ) {
    *pulDstCnt = *pulSrcCnt;
    pulDstCnt++;
    pulSrcCnt++;
  }

  /* compare the code */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt<pulSrcEnd ) {
    if( *pulDstCnt != *pulSrcCnt )
    {
      return -1;
    }
    pulDstCnt++;
    pulSrcCnt++;
  }

  /* get the number of trailing loads */
  uiElements = pulXcPrg[1] / sizeof(uint32_t);

  /* get source start and end pointer */
  pulSrcCnt = pulXcPrg + 2 + pulXcPrg[0] / sizeof(uint32_t);
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
*   uiPort               [in]  XC Port Number
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_XC_Start( unsigned int uiPort,
                   void*        pvUser )
{
  NX51_XMAC_AREA_T* ptXmacArea;
  NX51_XPEC_AREA_T* ptRpecArea;
  NX51_XPEC_AREA_T* ptTpecArea;

  if(uiPort>= NX51_NUM_XPECS) {
    return -1;
  }
  ptXmacArea = s_aptXmacArea[uiPort];
  ptRpecArea = s_aptRpecRegArea[uiPort];
  ptTpecArea = s_aptTpecRegArea[uiPort];

  /* reset PC of units */
  ptXmacArea->ulXmac_rpu_pc = 0;
  ptXmacArea->ulXmac_tpu_pc = 0;
  ptRpecArea->ulXpec_pc     = 0x7ff;
  ptTpecArea->ulXpec_pc     = 0x7ff;

  /* start units */
  s_ptXcStartStop->ulXc_start_stop_ctrl = (( MSK_NX51_xc_start_stop_ctrl_start_rpec0
                                           | MSK_NX51_xc_start_stop_ctrl_start_tpec0
                                           | MSK_NX51_xc_start_stop_ctrl_start_rpu0
                                           | MSK_NX51_xc_start_stop_ctrl_start_tpu0 ) << (4*uiPort));

  return 0;
}

/*****************************************************************************/
/*! Start XC Units
* \description
*   Start XC Units.
* \class
*   XC
* \params
*   uiPort               [in]  XC Port Number
*   uUnitVec             [in]  Bit vector defining the units
*                              bit 0: rPU, bit 1: tPU, bit 2: rPEC, bit 3: tPEC
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_XC_StartUnits(unsigned int uiPort, unsigned int uUnitVec, void* pvUser)
{
  uint32_t ulStart = 0;

  /* check instance number */
  if( uiPort >= NX51_NUM_XPECS )
    return 1;

  if( 0 != (uUnitVec & MSK_NX51_XC_TYPE_RPU) )
  {
    ulStart |= MSK_NX51_xc_start_stop_ctrl_start_rpu0;
  }

  if( 0 != (uUnitVec & MSK_NX51_XC_TYPE_TPU) )
  {
    ulStart |= MSK_NX51_xc_start_stop_ctrl_start_tpu0;
  }

  if( 0 != (uUnitVec & MSK_NX51_XC_TYPE_RPEC) )
  {
    ulStart |= MSK_NX51_xc_start_stop_ctrl_start_rpec0;
  }

  if( 0 != (uUnitVec & MSK_NX51_XC_TYPE_TPEC) )
  {
    ulStart |= MSK_NX51_xc_start_stop_ctrl_start_tpec0;
  }

  s_ptXcStartStop->ulXc_start_stop_ctrl = ulStart << (4*uiPort);

  return 0;
}
