#include "hal_eth2ps.h"

#define ETH2PS_CONCAT(a,b) a ## b

/* netX4000 specific settings */
#if defined(HAL_ETH2PS_TARGET_NX4000)
#include "hal_resources_defines_netx4000.h"
#define NXT(id)                  ETH2PS_CONCAT(NX4000_,id)
#define MSK_NXT(id)              ETH2PS_CONCAT(MSK_NX4000_,id)
#define SRT_NXT(id)              ETH2PS_CONCAT(SRT_NX4000_,id)
#define RPU_TIMESTAMP_OFFSET     0 /* in nanoseconds, 0 means correction done within xPEC */
#define TPU_TIMESTAMP_OFFSET     0 /* in nanoseconds, 0 means correction done within xPEC */
#define MSK_XPEC_STATCFG_PHY_LED_LINK ( MSK_NXT(xmac_status_shared0_gpio2_in_phy_led0) << SRT_NXT(sr_statcfg0_xmac0_status_shared0) )

/* netX90 specific settings */
#elif defined(HAL_ETH2PS_TARGET_NX90)
#include "hal_resources_defines_netx90.h"
#define NXT(id)                  ETH2PS_CONCAT(NX90_,id)
#define MSK_NXT(id)              ETH2PS_CONCAT(MSK_NX90_,id)
#define SRT_NXT(id)              ETH2PS_CONCAT(SRT_NX90_,id)
#define RPU_TIMESTAMP_OFFSET     0 /* in nanoseconds, 0 means correction done within xPEC */
#define TPU_TIMESTAMP_OFFSET     0 /* in nanoseconds, 0 means correction done within xPEC */
#define MSK_XPEC_STATCFG_PHY_LED_LINK ( MSK_NXT(xmac_status_shared0_gpio2_in_phy_led0) << SRT_NXT(sr_statcfg0_xmac0_status_shared0) )

#else
#error "NO ETH2PS HAL target defined"
#endif

#include "eth2ps_xpec_regdef.h"

#ifndef XC_INST
#define XC_INST 0
#endif

#if XC_INST == 0
#include "rpu_eth2ps0.h"
#include "rpu_eth2ps1.h"
#include "tpu_eth2ps0.h"
#include "tpu_eth2ps1.h"
#include "rpec_eth2ps_rpec0.h"
#include "rpec_eth2ps_rpec1.h"
#include "tpec_eth2ps_tpec0.h"
#include "tpec_eth2ps_tpec1.h"
#elif XC_INST == 1
#include "rpu_eth2ps2.h"
#include "rpu_eth2ps3.h"
#include "tpu_eth2ps2.h"
#include "tpu_eth2ps3.h"
#include "rpec_eth2ps_rpec2.h"
#include "rpec_eth2ps_rpec3.h"
#include "tpec_eth2ps_tpec2.h"
#include "tpec_eth2ps_tpec3.h"
#endif

#define ETH2PS_XPEC_DPM_PTR      s_apulXpecDramArea
#define ETH2PS_XC_INTRAM_PTR     s_apulIntRamStart
#define ETH2PS_PFIFO_DEPTH       100
#define ETH2PS_FRAME_BUF_CNT     84
__USE_PHY_CTRL
__USE_XC_LOADER_FUNC_RESET
__USE_XC_LOADER_FUNC_LOAD
__USE_XC_LOADER_FUNC_START
__USE_XPEC_DRAM
__USE_XPEC_IRQ_REGS
__USE_SYSTIME
__USE_POINTERFIFO
__USE_INTRAM
__USE_TRIGGER_SAMPLE
__USE_XMAC
#if defined(HAL_ETH2PS_TARGET_NX90)
__USE_INT_PHY_CFG
#endif

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/* Frame buffer properties */
#define ETH_FRAME_BUF_SIZE          1560   /* size of a frame buffer     */
#define ETH_FRAME_BUF_SIZE_DW       (ETH_FRAME_BUF_SIZE/4)

#define ETH2PS_SYSTIME_BORDER       0x3b9aca00UL /* 1 s in ns */
#define ETH2PS_SYSTIME_SPEED        0xa0000000

#define ETH2PS_PFIFO_MAX            32
#define ETH2PS_FRAME_BUF_NUM_MIN    1

#define ETH2PS_BPDU_MAC             0x00c28001
#define ETH2PS_DLR_MAC              0x006c2101
#define ETH2PS_BROADCAST_MAC        0xffffffffffffULL

#define MSK_ETH2PS_PFIFO_RESET      (0xffffffffUL >> (32 - ETH2PS_PFIFO_MAX))

/* Minimum DSCP value of high priority frames */
#ifndef ETH2PS_DLR_HI_PRIO_DSCP_MIN
#define ETH2PS_DLR_HI_PRIO_DSCP_MIN 43
#endif

/* Maximum number of entries in the group address table */
#ifndef ETH2PS_GROUP_ADDR_NUM_MAX
#define ETH2PS_GROUP_ADDR_NUM_MAX   32
#endif

/* Minimum period of cyclic events */
#define ETH2PS_SYNC_PERIOD_MIN      5000

/* Pointer FIFO Mapping */
typedef enum
{
  ETH2PS_PFIFO_REQ_HI_P0  = 0,
  ETH2PS_PFIFO_REQ_LO_P0  = 1,
  ETH2PS_PFIFO_REQ_HI_P1  = 4,
  ETH2PS_PFIFO_REQ_LO_P1  = 5,
  ETH2PS_PFIFO_EMPTY      = 8,
  ETH2PS_PFIFO_COLLECT    = 9,
  ETH2PS_PFIFO_IND_CNF_HI = 10,
  ETH2PS_PFIFO_IND_CNF_LO = 11
} ETH2PS_PFIFO_E;

/* PTP PLL */
typedef struct ETH2PS_PTP_PLL_Ttag
{
  unsigned int uPAmp2Pow;
  unsigned int uIAmp2Pow;
  uint32_t  ulClkBorderHi;
  uint32_t  ulClkBorderLo;
  uint32_t  ulIAccum;
} ETH2PS_PTP_PLL_T;

typedef struct ETH2PS_PORT_Ttag
{
  /* INGRESS and EGRESS Latencies (see IEEE1588v2) */
  uint32_t ulIngressLatency;
  uint32_t ulEgressLatency;

  bool fExtMii;
} ETH2PS_PORT_T;

typedef struct ETH2PS_Ttag
{
  ETH2PS_PORT_T atPort[ETH2PS_PORTS];

#if !defined(HAL_ETH2PS_TARGET_NX500)
  uint32_t ulTriggerActivate;
  uint32_t aulTriggerOffset[2];
#endif

#ifndef ETH2PS_RCV_ALL_MULTICASTS
  uint64_t aullGroupAddr[ETH2PS_GROUP_ADDR_NUM_MAX + 1]; /* +1 for Broadcast address */
#endif

  ETH2PS_PTP_PLL_T tPtpPll;
} ETH2PS_T;

/*****************************************************************************/
/* Variables                                                                 */
/*****************************************************************************/

/* Pointer to XPEC DRAMs */
static ETH2PS_XPEC_DPM_T* s_aptXpecRam[ETH2PS_PORTS];

/* Pointer to XC microcodes */
#if XC_INST == 0
static const uint32_t* const paulRPecCodes[]    = { XcCode_rpec_eth2ps_rpec0, XcCode_rpec_eth2ps_rpec1 };
static const uint32_t* const paulTPecCodes[]    = { XcCode_tpec_eth2ps_tpec0, XcCode_tpec_eth2ps_tpec1 };
static const uint32_t* const paulxMacRpuCodes[] = { XcCode_rpu_eth2ps0,       XcCode_rpu_eth2ps1       };
static const uint32_t* const paulxMacTpuCodes[] = { XcCode_tpu_eth2ps0,       XcCode_tpu_eth2ps1       };
#elif XC_INST == 1
static const uint32_t* const paulRPecCodes[]    = { XcCode_rpec_eth2ps_rpec2, XcCode_rpec_eth2ps_rpec3 };
static const uint32_t* const paulTPecCodes[]    = { XcCode_tpec_eth2ps_tpec2, XcCode_tpec_eth2ps_tpec3 };
static const uint32_t* const paulxMacRpuCodes[] = { XcCode_rpu_eth2ps2,       XcCode_rpu_eth2ps3       };
static const uint32_t* const paulxMacTpuCodes[] = { XcCode_tpu_eth2ps2,       XcCode_tpu_eth2ps3       };
#endif

static NXT(POINTER_FIFO_AREA_T)* s_ptPFifo;
static NXT(SYSTIME_AREA_T)* s_ptSystime;
static NXT(TRIGGER_SAMPLE_UNIT_AREA_T)* s_ptTriggerSample;

static const uint8_t s_abRstpBpduMac[] = { 0x01, 0x80, 0xC2, 0x00 };

static ETH2PS_T s_t2Ps;

/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/

/* Initialize the FIFO units to optimized depth and insert empty pointers */
static void initFifoUnit(void)
{
  int      iIdx;
  uint32_t ulBorder, ulResetVal;

  /* set reset bit for all pointer FIFOs */
  ulResetVal = NX_READ32(s_ptPFifo->ulPfifo_reset);
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, ulResetVal | MSK_ETH2PS_PFIFO_RESET );

  /* initialize FIFO sizes */
  ulBorder = 0;
  for(iIdx = 0; iIdx < ETH2PS_PFIFO_MAX; iIdx++)
  {
    ulBorder += ETH2PS_PFIFO_DEPTH;
    NX_WRITE32(s_ptPFifo->aulPfifo_border[iIdx], ulBorder - 1);
  }

  /* clear reset bit for all pointer FIFOs */
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, ulResetVal & ~MSK_ETH2PS_PFIFO_RESET);

  /* fill the empty pointer FIFO */
  for(iIdx = ETH2PS_FRAME_BUF_NUM_MIN; iIdx < ETH2PS_FRAME_BUF_CNT; ++iIdx)
  {
    NX_WRITE32(s_ptPFifo->aulPfifo[ETH2PS_PFIFO_EMPTY], (uint32_t)iIdx << SRT_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER);
  }
}


/*****************************************************************************/
/*! Initialize 2-Port Switch
* \description
*   Initializes the switch and configures it with the default parameter settings.
* \class
*   Control Service Class
* \params
*   ptCfg             [in] HAL configuration
*   pvUser            [in] User specific parameter
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INIT_FAILED                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_Initialize(ETH2PS_CFG_T* ptCfg,
                                  void*         pvUser )
{
  unsigned int uiXc, uiPort, uiIdx;

  uiXc = XC_INST;

  s_aptXpecRam[0]   = (ETH2PS_XPEC_DPM_T*) ETH2PS_XPEC_DPM_PTR[uiXc * 2 + 0];
  s_aptXpecRam[1]   = (ETH2PS_XPEC_DPM_T*) ETH2PS_XPEC_DPM_PTR[uiXc * 2 + 1];
  s_ptPFifo         = s_aptPFifo[uiXc];
  s_ptSystime       = s_aptSystime[uiXc];
  s_ptTriggerSample = s_aptTriggerSample[uiXc];

  /* reset the hardware block */
  if( (0 != pfnXcReset(uiXc, 0, pvUser))
    ||(0 != pfnXcReset(uiXc, 1, pvUser))
    )
    return ETH2PS_ERR_INIT_FAILED;

  /* initialize the FIFO unit */
  initFifoUnit();

  /* set system time border and system time speed to IEEE 1588 format */
  NX_WRITE32(s_ptSystime->ulSystime_border, ETH2PS_SYSTIME_BORDER - 1);

  Eth2PS_PtpResetPll(pvUser);

  /* default setting for the clock controller */
  Eth2PS_PtpConfigPll(1, 3, 200);

  /* load the microcode into the hardware block */
  if(   (0 != pfnXcLoad(uiXc, 0, NX_XC_TYPE_RPU,  paulxMacRpuCodes[0], pvUser))
      ||(0 != pfnXcLoad(uiXc, 0, NX_XC_TYPE_TPU,  paulxMacTpuCodes[0], pvUser))
      ||(0 != pfnXcLoad(uiXc, 0, NX_XC_TYPE_RPEC, paulRPecCodes[0],    pvUser))
      ||(0 != pfnXcLoad(uiXc, 0, NX_XC_TYPE_TPEC, paulTPecCodes[0],    pvUser))
      ||(0 != pfnXcLoad(uiXc, 1, NX_XC_TYPE_RPU,  paulxMacRpuCodes[1], pvUser))
      ||(0 != pfnXcLoad(uiXc, 1, NX_XC_TYPE_TPU,  paulxMacTpuCodes[1], pvUser))
      ||(0 != pfnXcLoad(uiXc, 1, NX_XC_TYPE_RPEC, paulRPecCodes[1],    pvUser))
      ||(0 != pfnXcLoad(uiXc, 1, NX_XC_TYPE_TPEC, paulTPecCodes[1],    pvUser))
    ) return ETH2PS_ERR_INIT_FAILED;

  for(uiPort = 0; uiPort < ETH2PS_PORTS; ++uiPort)
  {
    /* configure PHY LEDs */
    switch( ptCfg->ePhyLedCfg ) {
      case ETH2PS_PHYLED_STATIC:
        NX_WRITE32(s_aptPhyCtrl[uiXc * 2 + uiPort]->ulInt_phy_ctrl_led,
            (NX_INT_PHYCTRL_LED_MODE_STATIC << SRT_NXT(int_phy_ctrl_led_mode)));
        break;

      case ETH2PS_PHYLED_BLINK:
        NX_WRITE32(s_aptPhyCtrl[uiXc * 2 + uiPort]->ulInt_phy_ctrl_led,
            (NX_INT_PHYCTRL_LED_MODE_FLASHING << SRT_NXT(int_phy_ctrl_led_mode))
          |((ptCfg->ulActLedFlashPeriod/10-1) << SRT_NXT(int_phy_ctrl_led_interval)));
        break;

      case ETH2PS_PHYLED_SINGLE:
        NX_WRITE32(s_aptPhyCtrl[uiXc * 2 + uiPort]->ulInt_phy_ctrl_led,
            (NX_INT_PHYCTRL_LED_MODE_COMBINED << SRT_NXT(int_phy_ctrl_led_mode))
          |((ptCfg->ulActLedFlashPeriod/10-1) << SRT_NXT(int_phy_ctrl_led_interval)));
        break;

      case ETH2PS_PHYLED_OFF:
        NX_WRITE32(s_aptPhyCtrl[uiXc * 2 + uiPort]->ulInt_phy_ctrl_led, 0);
         break;

      default:
        break;
    }

    /* copy the current SYSTIME border plus 1 value to the copy inside the hardware block */
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_CONFIGAREABASE.ulETH2PS_SYSTIME_BORDER_COPY_PLUS1, ETH2PS_SYSTIME_BORDER);

    /* initialize INGRESS and EGRESS Latencies */
    s_t2Ps.atPort[uiPort].ulIngressLatency = 0;
    s_t2Ps.atPort[uiPort].ulEgressLatency = 0;
    s_t2Ps.atPort[uiPort].fExtMii = false; /* we are running internal MII per default */

#if XC_INST == 1
    /* XC instance 1 runs on external MII per default */
    if( ETH2PS_OKAY != Eth2PS_CfgMii( uiPort, 1 /* External MII */ ) ) {
      return ETH2PS_ERR_INIT_FAILED;
    }
#endif
  }

#ifndef ETH2PS_RCV_ALL_MULTICASTS
  /* initialize group address table, broadcast address is first entry, all others are free */
  for(uiIdx = 0; uiIdx <= ETH2PS_GROUP_ADDR_NUM_MAX; ++uiIdx)
    s_t2Ps.aullGroupAddr[uiIdx] = ( uiIdx == 0 ) ? ETH2PS_BROADCAST_MAC : 0;
#else
  /* enable all Multicast addresses */
  for(uiIdx = 0; uiIdx < sizeof(s_aptXpecRam[0]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField)/4; ++uiIdx)
  {
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiIdx], 0xffffffff);
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiIdx], 0xffffffff);
  }
#endif

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Start 2-Port Switch
* \description
*   Confirms all pending interrupts and starts the 2-port switch.
* \class
*   Control Service Class
* \params
*   pvUser                 [in] User specific parameter
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INIT_FAILED                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_Start(void* pvUser)
{
  unsigned int uiXc = XC_INST;
  unsigned int uiXpec = 2 * uiXc;

  /* confirm the set of IRQs */
  NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec + 0], 0xffff);
  NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec + 1], 0xffff);

  /* start XC port */
  if( (pfnXcStart(uiXc, 0, pvUser) != 0 )
    ||(pfnXcStart(uiXc, 1, pvUser) != 0 )
    )
    return ETH2PS_ERR_INIT_FAILED;

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Set MAC Address
* \description
*   Sets a MAC address.
*   Note: The Chassis MAC addresses shall be set before the switch is started.
*   Note: The DLR supervisor address is used for Beacon handling, but only if
*         DLR support is enabled (see parameter ETH2PS_PARAM_DLR_SUPPORT_ENABLE).
* \class
*   Control Service Class
* \params
*   eType        [in] Defines which MAC address shall be configured
*   tMacAddr     [in] MAC address value
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_SetMacAddr(ETH2PS_MAC_ADDRESS_TYPE_E eType, const ETH2PS_MAC_ADDR_T tMacAddr)
{
  uint32_t ulMacHi, ulMacLo;

  ulMacHi = (uint32_t)(tMacAddr[5] << 8)  | (uint32_t)tMacAddr[4];
  ulMacLo = (uint32_t)(tMacAddr[3] << 24) | (uint32_t)(tMacAddr[2]  << 16) | (uint32_t)(tMacAddr[1]  << 8) | (uint32_t)tMacAddr[0];

  switch(eType)
  {
  case ETH2PS_MAC_ADDRESS_CHASSIS:
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_CONFIGAREABASE.ulETH2PS_INTERFACE_MAC_ADDRESS_HI,  ulMacHi);
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_CONFIGAREABASE.ulETH2PS_INTERFACE_MAC_ADDRESS_LO,  ulMacLo);
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_CONFIGAREABASE.ulETH2PS_INTERFACE_MAC_ADDRESS_HI,  ulMacHi);
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_CONFIGAREABASE.ulETH2PS_INTERFACE_MAC_ADDRESS_LO,  ulMacLo);
    break;

  case ETH2PS_MAC_ADDRESS_2ND_CHASSIS:
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_CONFIGAREABASE.ulETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI,  ulMacHi);
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_CONFIGAREABASE.ulETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO,  ulMacLo);
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_CONFIGAREABASE.ulETH2PS_2ND_INTERFACE_MAC_ADDRESS_HI,  ulMacHi);
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_CONFIGAREABASE.ulETH2PS_2ND_INTERFACE_MAC_ADDRESS_LO,  ulMacLo);
    break;

  case ETH2PS_MAC_ADDRESS_DLR_SUPERVISOR:
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_BEACON_CTRL_AREABASE.ulETH2PS_ACTIVE_SVR_MAC_HI,  ulMacHi);
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_BEACON_CTRL_AREABASE.ulETH2PS_ACTIVE_SVR_MAC_LO,  ulMacLo);
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_BEACON_CTRL_AREABASE.ulETH2PS_ACTIVE_SVR_MAC_HI,  ulMacHi);
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_BEACON_CTRL_AREABASE.ulETH2PS_ACTIVE_SVR_MAC_LO,  ulMacLo);
    break;

  default:
    return ETH2PS_ERR_INVAL_PARAM;
  }

  return ETH2PS_OKAY;
}


#ifndef ETH2PS_RCV_ALL_MULTICASTS
/*****************************************************************************/
/*! Add Group Address
* \description
*   Add the given Multicast group address to the address recognition filter.
* \class
*   Control Service Class
* \params
*   tMacAddr     [in] Multicast MAC address value
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_OUT_OF_MEMORY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_AddGroupAddr(const ETH2PS_MAC_ADDR_T tMacAddr)
{
  uint64_t ullEntry = 0;
  unsigned int uiIdx;
  bool fAdded = false;
  unsigned int uiOfs;
  uint32_t ulMsk;

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check if Multicast address is given */
  if( (tMacAddr[0] & 1) == 0 )
    return ETH2PS_ERR_INVAL_PARAM;
#endif

  /* get group address table entry */
  for(uiIdx = 0; uiIdx < 6; ++uiIdx)
  {
    ullEntry = (ullEntry << 8) | tMacAddr[uiIdx];
  }

  /* get offset and mask for XC hash table */
  uiOfs = tMacAddr[5] >> 1;
  uiIdx = (((tMacAddr[5] & 1) << 4) | (tMacAddr[4] & 0xf));
  ulMsk = 1UL << uiIdx;

  /* iterate over all entries */
  for(uiIdx = 0; uiIdx <= ETH2PS_GROUP_ADDR_NUM_MAX; ++uiIdx)
  {
    if( !fAdded )
    {
      if( s_t2Ps.aullGroupAddr[uiIdx] == ullEntry )
        fAdded = true; /* entry already exists */
      else if( s_t2Ps.aullGroupAddr[uiIdx] == 0 )
      {
        /* free slot, add entry */
        s_t2Ps.aullGroupAddr[uiIdx] = ullEntry;
        NX_WRITE32(s_aptXpecRam[0]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiOfs], NX_READ32(s_aptXpecRam[0]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiOfs]) | ulMsk);
        NX_WRITE32(s_aptXpecRam[1]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiOfs], NX_READ32(s_aptXpecRam[1]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiOfs]) | ulMsk);
        fAdded = true;
      }
    }
    else
    {
      if( s_t2Ps.aullGroupAddr[uiIdx] == ullEntry )
        s_t2Ps.aullGroupAddr[uiIdx] = 0; /* remove duplicate entries */
    }
  }

  return fAdded ? ETH2PS_OKAY : ETH2PS_ERR_OUT_OF_MEMORY;
}


/*****************************************************************************/
/*! Delete Group Address
* \description
*   Delete the given Multicast group address from the address recognition.
* \class
*   Control Service Class
* \params
*   tMacAddr     [in] Multicast MAC address value
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM
*   ETH2PS_ERR_INVAL_STATE                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_DeleteGroupAddr(const ETH2PS_MAC_ADDR_T tMacAddr)
{
  uint64_t ullEntry = 0;
  unsigned int uiIdx;
  bool fClear = true;
  unsigned int uiOfs;
  uint32_t ulMsk;
  ETH2PS_RESULT_E eRslt = ETH2PS_ERR_INVAL_PARAM;

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check if Multicast filtering is enabled */
  if( s_t2Ps.aullGroupAddr[0] == 0 )
    return ETH2PS_ERR_INVAL_STATE;
#endif

  /* get group address table entry */
  for(uiIdx = 0; uiIdx < 6; ++uiIdx)
  {
    ullEntry = (ullEntry << 8) | tMacAddr[uiIdx];
  }

  /* get offset and mask for XC hash table */
  uiOfs = tMacAddr[5] >> 1;
  uiIdx = (((tMacAddr[5] & 1) << 4) | (tMacAddr[4] & 0xf));
  ulMsk = 1UL << uiIdx;

  /* iterate over all entries */
  for(uiIdx = 0; uiIdx <= ETH2PS_GROUP_ADDR_NUM_MAX; ++uiIdx)
  {
    if( s_t2Ps.aullGroupAddr[uiIdx] == ullEntry )
    {
      /* entry found, remove it */
      s_t2Ps.aullGroupAddr[uiIdx] = 0;
      eRslt = ETH2PS_OKAY;
    }
    else if( (s_t2Ps.aullGroupAddr[uiIdx] & 0xfff) == (ullEntry & 0xfff) )
    {
      /* entry with same hash value present: do not clear bit in XC table */
      fClear = false;
    }
  }

  if( fClear )
  {
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiOfs], NX_READ32(s_aptXpecRam[0]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiOfs]) & ~ulMsk);
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiOfs], NX_READ32(s_aptXpecRam[1]->tETH2PS_MULTICAST_FILTER_TABLE.aulDataField[uiOfs]) & ~ulMsk);
  }

  return eRslt;
}
#endif


/*****************************************************************************/
/*! Get Empty Ethernet Frame Block
* \description
*   Gets an element from the empty pointer FIFO.
* \class
*   Communication Service Class
* \params
*   ptFrame         [out] Frame handle structure filled out by the function on success
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_FIFO_EMPTY                                                    */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_GetFrame(ETH2PS_FRAME_HANDLE_T* ptFrame)
{
  /* three parties access the pointer FIFO, so keep at least 3 entries */
  if( NX_READ32(s_ptPFifo->aulPfifo_fill_level[ETH2PS_PFIFO_EMPTY]) < 3)
    return ETH2PS_ERR_FIFO_EMPTY;

  /* retrieve the FIFO element and fill out the handle */
  ptFrame->ulFifoEntry = NX_READ32(s_ptPFifo->aulPfifo[ETH2PS_PFIFO_EMPTY]) & MSK_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER;
  ptFrame->ptHdr       = (ETH2PS_FRAME_BUF_HDR_T*)(ETH2PS_XC_INTRAM_PTR[XC_INST * 2] + ptFrame->ulFifoEntry * ETH_FRAME_BUF_SIZE_DW);
  ptFrame->pbData      = (uint8_t*)(ptFrame->ptHdr + 1);
  ptFrame->usLength    = 0;

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Get Indication/Confirmation
* \description
*   Retrieves a received frame (indication) or a transmitted frame (confirmation)
*   from the combined indication/confirmation FIFO.
* \class
*   Communication Service Class
* \params
*   uPriority        [in]      0: low priority, 1: high priority
*   ptFrame          [out]     Frame handle filled by the function
*   ptFrameInfo      [out]     Additional frame information
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_FIFO_EMPTY                                                    */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_GetIndCnf(unsigned int             uPriority,
                                 ETH2PS_FRAME_HANDLE_T*   ptFrame,
                                 ETH2PS_FRAME_INFO_T*     ptFrameInfo)
{
  uint32_t      ulFifoEntry, ulFrameBufNo;
  uint32_t      ulSystimeNs, ulSystimeS;
  unsigned int  uiPort;

  /* get time stamp, correct wrap at SYSTIME border */
  ulSystimeS  = NX_READ32(s_ptSystime->ulSystime_s);
  ulSystimeNs = NX_READ32(s_ptSystime->ulSystime_ns);

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check fill level */
  if( 0 == Eth2PS_GetIndCnfFillLevel(uPriority) )
    return ETH2PS_ERR_FIFO_EMPTY;
#endif

  /* retrieve the stored FIFO element */
  ulFifoEntry = NX_READ32(s_ptPFifo->aulPfifo[uPriority ? ETH2PS_PFIFO_IND_CNF_HI : ETH2PS_PFIFO_IND_CNF_LO]);

  ulFrameBufNo         = (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER) >> SRT_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER;
  uiPort               = (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_PORT)             >> SRT_ETH2PS_FIFO_ELEMENT_PORT;
  ptFrameInfo->uPortNo = uiPort;

  /* fill out frame handle */
  ptFrame->ulFifoEntry = ulFifoEntry;
  ptFrame->ptHdr       = (ETH2PS_FRAME_BUF_HDR_T*)(ETH2PS_XC_INTRAM_PTR[XC_INST * 2] + ulFrameBufNo * ETH_FRAME_BUF_SIZE_DW);
  ptFrame->pbData      = (uint8_t*)(ptFrame->ptHdr + 1);

  /* indication or confirmation? */
  if( 0 != (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_CON_EN) )
  {
    /* confirmation */
    unsigned int uiXpecNo    = uiPort ^ 1;
    uint32_t ulTms;

    ptFrameInfo->fCnf       = true;
    ptFrameInfo->eCnfResult = (ETH2PS_CNF_ERROR_CODE_E)((ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_ERROR_CODE) >> SRT_ETH2PS_FIFO_ELEMENT_ERROR_CODE);

    /* get measured transmit time stamp and correct it to time stamp reference plane regarding IEEE1588v2-2008 */
    ulTms  = NX_READ32(s_aptXpecRam[uiXpecNo]->tETH2PS_TX_TIMESTAMP_AREA.aulDataField[ulFrameBufNo]);
    ulTms += s_t2Ps.atPort[uiPort].ulEgressLatency;

    if( ulTms >= ETH2PS_SYSTIME_BORDER )
    {
      /* correct overflow due addition of EGRESS latency */
      ulTms -= ETH2PS_SYSTIME_BORDER;
    }
    ptFrameInfo->ulTimeNs   = ulTms;
    ptFrame->usLength       = (uint16_t)NX_READ32(s_aptXpecRam[uiXpecNo]->tETH2PS_FRAME_LEN_AREA.aulDataField[ulFrameBufNo]);
  }
  else
  {
    /* indication */
    unsigned int uiXpecNo  = uiPort;
    uint32_t ulTms;

    ptFrameInfo->fCnf     = false;

    /* get measured receive time stamp and correct it to time stamp reference plane regarding IEEE1588v2-2008 */
    ulTms  = NX_READ32(s_aptXpecRam[uiXpecNo]->tETH2PS_RX_TIMESTAMP_AREA.aulDataField[ulFrameBufNo]);
    if( ulTms < s_t2Ps.atPort[uiPort].ulIngressLatency )
    {
      /* correct overflow in case of subtraction of INGRESS latency */
      ulTms += ETH2PS_SYSTIME_BORDER;
    }
    ulTms -= s_t2Ps.atPort[uiPort].ulIngressLatency;
    ptFrameInfo->ulTimeNs = ulTms;
    ptFrame->usLength     = (uint16_t)NX_READ32(s_aptXpecRam[uiXpecNo]->tETH2PS_FRAME_LEN_AREA.aulDataField[ulFrameBufNo]);
  }

  if( ulSystimeNs < ptFrameInfo->ulTimeNs )
    --ulSystimeS;

  ptFrameInfo->ulTimeS = ulSystimeS;

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Get Indication/Confirmation FIFO Fill Level
* \description
*   Gets the fill level of the indication/confirmation FIFO.
* \class
*   Status Service Class
* \params
*   uPriority   [in] 0: low priority, 1: high priority
* \return
*   FIFO fill level                                                          */
/*****************************************************************************/
uint32_t Eth2PS_GetIndCnfFillLevel(unsigned int uPriority)
{
  return NX_READ32(s_ptPFifo->aulPfifo_fill_level[uPriority ? ETH2PS_PFIFO_IND_CNF_HI : ETH2PS_PFIFO_IND_CNF_LO]);
}


/*****************************************************************************/
/*! Get Empty Pointer FIFO Fill Level
* \description
*   Returns the current fill level of the empty pointer FIFO.
* \class
*   Status Service Class
* \return
*   FIFO fill level                                                          */
/*****************************************************************************/
uint32_t Eth2PS_GetEmptyFillLevel(void)
{
  /* retrieve the current fill level from the selected FIFO */
  return NX_READ32(s_ptPFifo->aulPfifo_fill_level[ETH2PS_PFIFO_EMPTY]);
}


/*****************************************************************************/
/*! Get Request FIFO Fill Level
* \description
*   Returns the current fill level of the transmit request FIFO.
* \class
*   Status Service Class
* \params
*   uiPort          [in]      Switch port number
*   uPriority        [in]      0: low priority, 1: high priority
* \return
*   FIFO fill level                                                          */
/*****************************************************************************/
uint32_t Eth2PS_GetReqFillLevel(unsigned int uiPort, unsigned int uPriority)
{
  /* retrieve the current fill level from the selected FIFO */
  if( uiPort == 0 )
    return NX_READ32(s_ptPFifo->aulPfifo_fill_level[uPriority ? ETH2PS_PFIFO_REQ_HI_P0 : ETH2PS_PFIFO_REQ_LO_P0]);
  else
    return NX_READ32(s_ptPFifo->aulPfifo_fill_level[uPriority ? ETH2PS_PFIFO_REQ_HI_P1 : ETH2PS_PFIFO_REQ_LO_P1]);
}


/*****************************************************************************/
/*! Send Ethernet Frame
* \description
*   Initiates a transmission request.
* \class
*   Communication Service Class
* \params
*   uiPortInfo         [in]  0: Transmit on port 0 only\n
*                           1: Transmit on port 1 only\n
*                           2: Transmit on both ports\n
*                           3: Determine transmit port automatically by checking the MAC learning tables
*   ptFrame           [in]  Frame handle of the frame to be transmitted
*   uPriority         [in]  0: low priority, 1: high priority
*   fConfirmationEn   [in]  false: Frame is released automatically after transmission\n
*                           true: Frame is returned into the IND_CON_FIFO after transmission.\n
*                                 In this case the application has to release the frame manually.
*   puCnfCnt          [out] Number of confirmations produced by the transmission.
* \return
*   ETH2PS_OKAY                                                              */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_Send(unsigned int uiPortInfo, ETH2PS_FRAME_HANDLE_T* ptFrame, unsigned int uPriority, bool fConfirmationEn, unsigned int* puCnfCnt)
{
  bool fSendP0, fSendP1;
  uint8_t* pbData = ptFrame->pbData;
  uint32_t ulFrameBufNo = (ptFrame->ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER) >> SRT_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER;
  unsigned int uiTransmitCnt = 1;

  /* if a fixed send port is configured overwrite learning table value */
  switch( uiPortInfo )
  {
  case 0:
    fSendP0 = true;
    fSendP1 = false;
    break;

  case 1:
    fSendP0 = false;
    fSendP1 = true;
    break;

  case 2:
    fSendP0 = true;
    fSendP1 = true;
    break;

  default:
    if( NX_READ8(pbData[0]) & 0x01 )
    {
      /* Multicast, send to both ports */
      fSendP0 = true;
      fSendP1 = true;
    }
    else
    {
      uint32_t ulMsk;
      unsigned int uiIdx, uiOfs;

      uiOfs = NX_READ8(pbData[3]) ^ NX_READ8(pbData[5]);
      uiIdx = (0xf & (NX_READ8(pbData[2]) ^ NX_READ8(pbData[4])));
      ulMsk = 1UL << uiIdx;

      fSendP0 = ( 0 != (NX_READ32(s_aptXpecRam[0]->tETH2PS_HASH_TABLE_LOCAL_PORT_LEARN.aulDataField[uiOfs]) & ulMsk) );
      fSendP1 = ( 0 != (NX_READ32(s_aptXpecRam[1]->tETH2PS_HASH_TABLE_LOCAL_PORT_LEARN.aulDataField[uiOfs]) & ulMsk) );

      if( !(fSendP0 || fSendP1) )
      {
        /* address unknown, send to both ports */
        fSendP0 = true;
        fSendP1 = true;
      }
    }
    break;
  }

  if( 0 == memcmp(pbData, s_abRstpBpduMac, sizeof(s_abRstpBpduMac)) )
  {
    ptFrame->ulFifoEntry |= MSK_ETH2PS_FIFO_ELEMENT_BPDU;
  }
  /* set multi-request info */
  if( fSendP0 && fSendP1 )
  {
    uiTransmitCnt = 2;
    ptFrame->ulFifoEntry |= MSK_ETH2PS_FIFO_ELEMENT_MULTI_REQ;
  }

  /* set confirmation info */
  if( fConfirmationEn )
  {
    *puCnfCnt = uiTransmitCnt;
    ptFrame->ulFifoEntry |= MSK_ETH2PS_FIFO_ELEMENT_CON_EN;
    if( uPriority )
      ptFrame->ulFifoEntry |= MSK_ETH2PS_FIFO_ELEMENT_CON_TC;
  }
  else
  {
    *puCnfCnt = 0;
  }

  /* put FIFO element into request FIFO */
  if( fSendP0 )
  {
    /* set frame length */
    NX_WRITE32(s_aptXpecRam[1]->tETH2PS_FRAME_LEN_AREA.aulDataField[ulFrameBufNo], ptFrame->usLength);

    /* request transmission */
    NX_WRITE32(s_ptPFifo->aulPfifo[uPriority ? ETH2PS_PFIFO_REQ_HI_P0 : ETH2PS_PFIFO_REQ_LO_P0], ptFrame->ulFifoEntry);
  }

  if( fSendP1 )
  {
    /* set frame length */
    NX_WRITE32(s_aptXpecRam[0]->tETH2PS_FRAME_LEN_AREA.aulDataField[ulFrameBufNo],  ptFrame->usLength);

    /* request transmission */
    NX_WRITE32(s_ptPFifo->aulPfifo[uPriority ? ETH2PS_PFIFO_REQ_HI_P1 : ETH2PS_PFIFO_REQ_LO_P1], ptFrame->ulFifoEntry | MSK_ETH2PS_FIFO_ELEMENT_PORT);
  }

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Release Ethernet Frame Buffer
* \description
*   Puts an Ethernet frame buffer back into the empty pointer FIFO.
* \class
*   Communication Service Class
* \params
*   ptFrame    [in]  Handle of Ethernet frame
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_ReleaseFrame(ETH2PS_FRAME_HANDLE_T* ptFrame)
{
  uint32_t ulFrameBufNo = (ptFrame->ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER) >> SRT_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER;

#ifndef __ETH2PS_DISABLE_CHECKS__
  if( ulFrameBufNo >= ETH2PS_FRAME_BUF_CNT )
    return ETH2PS_ERR_INVAL_PARAM;
#endif

  /* return pointer in empty/collect FIFO */
  if( 0 != (ptFrame->ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_MULTI_REQ) )
  {
    NX_WRITE32(s_ptPFifo->aulPfifo[ETH2PS_PFIFO_COLLECT], ulFrameBufNo);
  }
  else
  {
    NX_WRITE32(s_ptPFifo->aulPfifo[ETH2PS_PFIFO_EMPTY], ulFrameBufNo);
  }

  /* invalidate handle */
  ptFrame->ulFifoEntry = 0;
  ptFrame->usLength    = 0;
  ptFrame->ptHdr       = NULL;
  ptFrame->pbData      = NULL;

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Get Status Counters
* \description
*   Gets the status counters of the according switch port.
* \class
*   Status Service Class
* \params
*   uiPort           [in]  Switch port number
*   ptCounters        [out] Counter structure filled out by the function
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PORT                                                    */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_GetCounters(unsigned int uiPort, ETH2PS_COUNTERS_T* ptCounters)
{
  unsigned int uiTpec = uiPort ^ 1; /* xPEC 0 transmits on port 1 and vice versa */

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETH2PS_PORTS )
    return ETH2PS_ERR_INVAL_PORT;
#endif

  /* get transmit counters */
  ptCounters->ulTxOutOctets             = NX_READ32(s_aptXpecRam[uiTpec]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_OUT_OCTETS);
  ptCounters->ulTxSingleCollisions      = NX_READ32(s_aptXpecRam[uiTpec]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_SINGLE_COLLISION_FRAMES);
  ptCounters->ulTxMultipleCollisions    = NX_READ32(s_aptXpecRam[uiTpec]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_MULTIPLE_COLLISION_FRAMES);
  ptCounters->ulTxLateCollisions        = NX_READ32(s_aptXpecRam[uiTpec]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_LATE_COLLISIONS);
  ptCounters->ulTxUnderrun              = NX_READ32(s_aptXpecRam[uiTpec]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_UTX_UNDERFLOW_DURING_TRANSMISSION);
  ptCounters->ulTxAborted               = NX_READ32(s_aptXpecRam[uiTpec]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_FRAMES_ABORTED);
  ptCounters->ulTxDiscarded             = NX_READ32(s_aptXpecRam[uiTpec]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_OUT_FRAMES_DISCARDED);

  /* get receive counters */
  ptCounters->ulRxInOctets              = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_IN_OCTETS);
  ptCounters->ulRxFcsErrors             = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_FRAME_CHECK_SEQUENCE_ERRORS);
  ptCounters->ulRxAlignmentErrors       = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_ALIGNMENT_ERRORS);
  ptCounters->ulRxFrameLengthErrors     = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_FRAME_TOO_LONG_ERRORS);
  ptCounters->ulRxRuntFrames            = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_RUNT_FRAMES_RECEIVED);
  ptCounters->ulRxCollisionFragments    = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_COLLISION_FRAGMENTS_RECEIVED);
  ptCounters->ulRxOverflow              = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_FRAMES_DROPPED_DUE_URX_OVERFLOW);
  ptCounters->ulRxDiscarded             = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_IN_FRAMES_DISCARDED);
  ptCounters->ulRxCirculatingFrmBlocked = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_CIRCULATING_FRAMES_BLOCKED);
  ptCounters->ulRxUnknownErrors         = NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_STATUS_AREA_BASE.ulETH2PS_UNKNOWN_ERRORS);

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Get and Confirm Communication Interrupts
* \description
*   Retrieves and confirms the current interrupt requests
*   from the according communication port.
* \class
*   Status Service Class
* \params
*   uiPort           [in]  Switch port number
*   fHiPriority       [in]  IRQ priority selector (0: COM IRQs, 1: MSYNC IRQs)
* \return
*   Bit mask of pending interrupts                                           */
/*****************************************************************************/
uint32_t Eth2PS_GetCnfIrq(unsigned int uiPort, bool fHiPriority)
{
  uint32_t ulIrq;
  unsigned int uiXc = XC_INST;
  unsigned int uiXpec;

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETH2PS_PORTS )
    return 0;
#endif

  uiXpec = 2 * uiXc + uiPort;

  /* get IRQ events */
  ulIrq = (fHiPriority ? 0xf000 : 0x0fff) & NX_READ32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec]);

  /* confirm the set of IRQs */
  NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec], ulIrq);

  return ulIrq;

}


/*****************************************************************************/
/*! Flush Learning Table
* \description
*   Deletes all entries in the MAC address learning table.
*   The switch "forgets" all received source MAC addresses.
* \class
*   Control Service Class                                                    */
/*****************************************************************************/
void Eth2PS_FlushLearningTable(void)
{
  /*lint -e(534) cannot react to return value */
  Eth2PS_FlushLearningTablePort(0);
  /*lint -e(534) cannot react to return value */
  Eth2PS_FlushLearningTablePort(1);
}

/*****************************************************************************/
/*! Flush Learning Table at port
* \description
*   Deletes all entries in the MAC address learning table of a port.
*   The switch "forgets" all received source MAC addresses.
* \class
*   Control Service Class
* \params
*   uiPort               [in]  Switch port number
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PORT                                                    */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_FlushLearningTablePort(unsigned int uiPort)
{
  unsigned int uiXc = XC_INST;
  unsigned int uiXpec;

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETH2PS_PORTS )
    return ETH2PS_ERR_INVAL_PORT;
#endif

  uiXpec = 2 * uiXc + uiPort;

  if( 0 == (NX_READ32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec]) & MSK_ETH2PS_ARM2XPEC_INTERRUPTS_FLUSH_UNICAST_TABLE) )
    NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec], MSK_ETH2PS_ARM2XPEC_INTERRUPTS_FLUSH_UNICAST_TABLE);

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Get PHY Phase Offset
* \description
*   Gets the current phase offset of the PHY, which means additionally receive delay offset.
*   This function should be called once after upon the first received packet after assertion of Ethernet link,
*   in order to determine the exact delay.
*   The value of phase offset is kept (constant) until link is down.
* \class
*   Synchronization Service Class
* \params
*   uiPort               [in]  Switch port number
*   pbPhyPhaseOffsetNs    [out] Current PHY receive delay offset [nanoseconds]
*   pvUser                [in] User specific parameter
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PORT                                                    */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_GetPhyPhaseOffset(unsigned int uiPort, uint8_t* pbPhyPhaseOffsetNs, void* pvUser)
{
#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETH2PS_PORTS )
    return ETH2PS_ERR_INVAL_PORT;
#endif

#if defined(HAL_ETH2PS_TARGET_NX4000)
  /* netX4000 */
  *pbPhyPhaseOffsetNs = 0;

#elif defined(HAL_ETH2PS_TARGET_NX90)
  /* netX90 */
  if( !s_t2Ps.atPort[uiPort].fExtMii )
  {
    /* internal PHY */
    unsigned int uiRxOffset = 0;
    uint8_t bVal = 0;

    if(uiPort == 0) {
      uiRxOffset = (s_ptIntPhyCfg->ulInt_phy_cfg_status & MSK_NXT(int_phy_cfg_status_phy0_rx_offset)) >> SRT_NXT(int_phy_cfg_status_phy0_rx_offset);
    } else if(uiPort == 1) {
      uiRxOffset = (s_ptIntPhyCfg->ulInt_phy_cfg_status & MSK_NXT(int_phy_cfg_status_phy1_rx_offset)) >> SRT_NXT(int_phy_cfg_status_phy1_rx_offset);
    }

    /* calculate phase offset in ns */
    if( uiRxOffset == 0 ) bVal = 0;
    else if(uiRxOffset == 1) bVal = 32;
    else if(uiRxOffset == 2) bVal = 24;
    else if(uiRxOffset == 3) bVal = 16;
    else if(uiRxOffset == 4) bVal = 8;

    *pbPhyPhaseOffsetNs = bVal;
  }
  else
  {
    /* external PHY */
    *pbPhyPhaseOffsetNs = 0;
  }
#endif
  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Set Parameter
* \description
*   Sets a parameter in the 2-Port Switch.
* \class
*   Control Service Class
* \params
*   uiPort       [in]  Switch port number
*   eParam        [in]  Parameter to be set
*   ulValue       [in]  Value to set parameter to
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_SetParameter(unsigned int uiPort, ETH2PS_PARAM_E eParam, uint32_t ulValue)
{
  unsigned int uiXc = XC_INST; /* default: invalid link and SPEED10 and HDX */
  unsigned int uiXpec;
  ETH2PS_RESULT_E eRslt = ETH2PS_OKAY;

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETH2PS_PORTS )
    return ETH2PS_ERR_INVAL_PORT;
#endif

  uiXpec = 2 * uiXc + uiPort;

  switch (eParam)
  {
  case ETH2PS_PARAM_IRQ_EN_MSK:
    /* set interrupt enable masks */
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_IRQMASK_AREABASE.ulETH2PS_IND_CNF_HI_IRQ_MASK                , (ulValue & MSK_ETH2PS_IRQ_IND_CNF_HI));
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_IRQMASK_AREABASE.ulETH2PS_IND_CNF_LO_IRQ_MASK                , (ulValue & MSK_ETH2PS_IRQ_IND_CNF_LO));
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_IRQMASK_AREABASE.ulETH2PS_LINK_STA_CHANGED_IRQ_MASK          , (ulValue & MSK_ETH2PS_IRQ_LINK_CHANGED));
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_IRQMASK_AREABASE.ulETH2PS_CIRCULATING_FRAME_BLOCKED_IRQ_MASK , (ulValue & MSK_ETH2PS_IRQ_CIRCULATING_FRM));
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_IRQMASK_AREABASE.ulETH2PS_BCN_TIMEOUT_IRQ_MASK               , (ulValue & MSK_ETH2PS_IRQ_BCN_TIMEOUT));
    break;

  case ETH2PS_PARAM_AGING_TIME:
    if( 0 == ulValue )
    {
      eRslt = ETH2PS_ERR_INVAL_PARAM;
    }
    else
    {
      NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_CONFIGAREABASE.ulETH2PS_AGING_TIME_IN_MILLISECONDS, ulValue);
    }
    break;

  case ETH2PS_PARAM_PORT_ENABLE:
    NX_WRITE32(s_aptXpecRam[uiPort^1]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_TX_ENABLE, ulValue);
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_EMPTY_PTR_FIFO_MIN_FILL_LEVEL_LIMIT,
      ( ulValue != 0 ) ? 3 : 0xffffffffUL);
    break;

  case ETH2PS_PARAM_LINK_INPUT_ENABLE:
    // set link mode input enable
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_LINK_INPUT_EN, ulValue);

    // Note: tpec0 transmits to tpu1 and vice versa
    NX_WRITE32(s_aptXpecRam[uiPort^1]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_TX_LINK_CHK_EN, (0 != ulValue) ? MSK_XPEC_STATCFG_PHY_LED_LINK : 0);
    break;

  case ETH2PS_PARAM_BPDU_ONLY:
    NX_WRITE32(s_aptXpecRam[uiPort^1]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_TX_BPDU_ONLY, ulValue);
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_RX_BPDU_ONLY, ulValue);
    break;

  case ETH2PS_PARAM_BPDU_SUPPORT_ENABLE:
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_BPDU_MAC,
      ( ulValue != 0 ) ? ETH2PS_BPDU_MAC : 0);
    break;

  case ETH2PS_PARAM_DISABLE_LEARNING:
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_RX_DIS_LEARNING,
      ( ulValue != 0 ) ? MSK_ETH2PS_RX_DIS_LEARNING_VAL : 0);
    break;

  case ETH2PS_PARAM_DSCP_PRIORIZATION:
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_CONFIGAREABASE.ulETH2PS_DSCP_PRIORIZATION, ulValue);
    break;

  case ETH2PS_PARAM_DLR_SUPPORT_ENABLE:
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_DLR_MAC,
      ( ulValue != 0 ) ? ETH2PS_DLR_MAC : 0);
    break;

  case ETH2PS_PARAM_DLR_BCN_IND_ENABLE:
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BRIDGECFG_AREABASE.ulETH2PS_RCV_ENABLED_BEACON,
      ( ulValue != 0 ) ? MSK_ETH2PS_HELP_RX_FRWD_FRAME_TO_LOCAL_PORT : 0);
    break;

  case ETH2PS_PARAM_DLR_BCN_PORT_MATCH_ENABLE:
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BEACON_CTRL_AREABASE.ulETH2PS_BEACON_DETECTION_CTRL_COMPARE_PORT, ulValue);
    break;

  case ETH2PS_PARAM_DLR_BCN_RCV_PORT:
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BEACON_CTRL_AREABASE.ulETH2PS_LAST_BEACON_RX_PORT, ulValue);
    break;

  case ETH2PS_PARAM_DLR_BCN_PRECEDENCE:
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BEACON_CTRL_AREABASE.ulETH2PS_ACTIVE_SVR_PRECEDENCE, ulValue);
    break;

  case ETH2PS_PARAM_DLR_BCN_TIMEOUT:
    if( ulValue == 0 )
    {
      /* stop beacon timeout */
      NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec], MSK_ETH2PS_ARM2XPEC_INTERRUPTS_STOP_BEACON_TIMEOUT);
    }
    else
    {
      /* start beacon timeout */
      NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_BEACON_CTRL_AREABASE.ulETH2PS_BEACON_TIMEOUT, ulValue);
      NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec], MSK_ETH2PS_ARM2XPEC_INTERRUPTS_START_BEACON_TIMEOUT);
    }
    break;

  case ETH2PS_PARAM_INGRESS_LATENCY:
    s_t2Ps.atPort[uiPort].ulIngressLatency = ulValue + RPU_TIMESTAMP_OFFSET; /* RPU time stamp behind real time stamp */

    /* INGRESS latency configured: we have to update BRIDGE_DELAY_ADD, Note: xPEC0/1 receives from Port 0/1 */
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_CONFIGAREABASE.ulETH2PS_BRIDGE_DELAY_ADD_IN_NS,
        s_t2Ps.atPort[uiPort].ulIngressLatency
      + s_t2Ps.atPort[uiPort^1].ulEgressLatency
    );
    break;

  case ETH2PS_PARAM_EGRESS_LATENCY:
    s_t2Ps.atPort[uiPort].ulEgressLatency = ulValue + TPU_TIMESTAMP_OFFSET; /* TPU time stamp before real time stamp */

    /* EGRESS latency configured: we have to update BRIDGE_DELAY_ADD, Note: xPEC0/1 transmits to Port 1/0 */
    NX_WRITE32(s_aptXpecRam[uiPort^1]->tETH2PS_CONFIGAREABASE.ulETH2PS_BRIDGE_DELAY_ADD_IN_NS,
        s_t2Ps.atPort[uiPort^1].ulIngressLatency
      + s_t2Ps.atPort[uiPort].ulEgressLatency
    );
    break;

  default:
    eRslt = ETH2PS_ERR_INVAL_PARAM;
    break;
  }

  return eRslt;
}


/*****************************************************************************/
/*! Get Beacon State
* \description
*   Retrieves the Beacon state of a received Beacon frame frame.
* \class
*   DLR Service Class
* \params
*   ulFifoEntry   [in]  FIFO entry from the frame handle to query the beacon state
* \return
*   Beacon State                                                             */
/*****************************************************************************/
ETH2PS_BCNSTATE_E Eth2PS_GetBeaconState(uint32_t ulFifoEntry)
{
  ETH2PS_BCNSTATE_E eState = ETH2PS_BCNSTATE_NO_BEACON;

  if( ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_BCN_PRECEDENCE_HIGHER )
    eState = ETH2PS_BCNSTATE_PRECEDENCE_HIGHER;
  else if( ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_BCN_RX_PORT_NEQ_LAST )
    eState = ETH2PS_BCNSTATE_RX_PORT_NEQ_LAST;
  else if( ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_BCN_RING_STATE_CHANGE )
    eState = ETH2PS_BCNSTATE_RING_STATE_CHANGE;

  return eState;
}


/*****************************************************************************/
/*! Reset PLL
* \description
*   Resets the PLL speed to initial value.
* \class
*   Precision Time Protocol Service Class
* \params
*   pvUser                 [in]   User specific parameter                    */
/*****************************************************************************/
void Eth2PS_PtpResetPll(void* pvUser)
{
  /* reset ulI_ACCUM */
  s_t2Ps.tPtpPll.ulIAccum = ETH2PS_SYSTIME_SPEED;

  /* reset system time speed */
  NX_WRITE32(s_ptSystime->ulSystime_count_value, ETH2PS_SYSTIME_SPEED);
}


/*****************************************************************************/
/*! Configure PLL
* \description
*   Configure the PLL parameters.
*   This function should be called once during initialization.
* \class
*   Precision Time Protocol Service Class
* \params
*   uIAmp2Pow               [in]  The 2-base exponent of amplification factor of the integral term of the controller
*   uPAmp2Pow               [in]  The 2-base exponent of amplification factor of the proportional term of the controller
*   ulClockSpeedVariancePpm [in]  The maximum/minimum variance of the clock speed in parts per million
*                                                                            */
/*****************************************************************************/
void Eth2PS_PtpConfigPll(unsigned int uIAmp2Pow, unsigned int uPAmp2Pow, uint32_t ulClockSpeedVariancePpm)
{
  /* compute absolute SYSTIME_SPEED offset from allowed variance in parts per million */
  uint32_t ulSystimeSpeedVariance = ETH2PS_SYSTIME_SPEED / 1000000 * ulClockSpeedVariancePpm;

  s_t2Ps.tPtpPll.ulClkBorderHi = ETH2PS_SYSTIME_SPEED + ulSystimeSpeedVariance;
  s_t2Ps.tPtpPll.ulClkBorderLo = ETH2PS_SYSTIME_SPEED - ulSystimeSpeedVariance;

  s_t2Ps.tPtpPll.uIAmp2Pow     = uIAmp2Pow;
  s_t2Ps.tPtpPll.uPAmp2Pow     = uPAmp2Pow;
}


/*****************************************************************************/
/*! Control PLL
* \description
*   Control the PLL.
*   This function should be called periodically to synchronize to the PTP clock master.
* \class
*   Precision Time Protocol Service Class
* \params
*   lDiff                  [in]   Control deviation
*   ptOutput               [out]  If not NULL, this structure is filled with the controller output
*   pvUser                 [in]   User specific parameter                    */
/*****************************************************************************/
void Eth2PS_PtpControlPll(int32_t lDiff, ETH2PS_PI_CONTROLLER_OUTPUT_T* ptOutput, void* pvUser)
{
  uint32_t ulP, ulI, ulClockCntVal, ulDiffMagnitude;
  bool fDiffDir = (lDiff >= 0);

  ulDiffMagnitude = (uint32_t)(fDiffDir ? lDiff : -lDiff);

  /* Limit ABS(Difference) */
  if( ulDiffMagnitude > 0xffff )
  {
    ulDiffMagnitude=0xffff;
  }

  /* ulP = P_FACTOR * ABS(Difference) */
  ulP = ulDiffMagnitude << s_t2Ps.tPtpPll.uPAmp2Pow;

  /* ulI = I_FACTOR * ABS(Difference) */
  ulI = ulDiffMagnitude << s_t2Ps.tPtpPll.uIAmp2Pow;

  if( fDiffDir )
  {
    s_t2Ps.tPtpPll.ulIAccum += ulI;

    /* limit I_ACCUM to specified variance */
    if( s_t2Ps.tPtpPll.ulIAccum > s_t2Ps.tPtpPll.ulClkBorderHi )
    {
      s_t2Ps.tPtpPll.ulIAccum = s_t2Ps.tPtpPll.ulClkBorderHi;
    }

    ulClockCntVal = ulP + s_t2Ps.tPtpPll.ulIAccum;

    /* limit ClockCntVal to specified variance */
    if( ulClockCntVal > s_t2Ps.tPtpPll.ulClkBorderHi )
    {
      ulClockCntVal = s_t2Ps.tPtpPll.ulClkBorderHi;
    }

    if( ptOutput )
    {
      ptOutput->lPTerm   = (int32_t)ulP;
      ptOutput->ulITerm  = s_t2Ps.tPtpPll.ulIAccum;
      ptOutput->ulOutput = ulClockCntVal;
    }
  }
  else
  {
    s_t2Ps.tPtpPll.ulIAccum -= ulI;

    /* limit I_ACCUM to specified variance */
    if( s_t2Ps.tPtpPll.ulIAccum < s_t2Ps.tPtpPll.ulClkBorderLo )
    {
      s_t2Ps.tPtpPll.ulIAccum = s_t2Ps.tPtpPll.ulClkBorderLo;
    }

    ulClockCntVal = s_t2Ps.tPtpPll.ulIAccum - ulP;

    /* limit ClockCntVal to specified variance */
    if( ulClockCntVal < s_t2Ps.tPtpPll.ulClkBorderLo )
    {
      ulClockCntVal = s_t2Ps.tPtpPll.ulClkBorderLo;
    }

    if( ptOutput )
    {
      ptOutput->lPTerm   = (int32_t)(0 - ulP);
      ptOutput->ulITerm  = s_t2Ps.tPtpPll.ulIAccum;
      ptOutput->ulOutput = ulClockCntVal;
    }
  }

  /* set system time speed */
  NX_WRITE32(s_ptSystime->ulSystime_count_value, ulClockCntVal);
}


/*****************************************************************************/
/*! Initialize the generation of Cyclic Events
* \description
*   This function initializes the cyclic event generator.
*   All other functions of the Cyclic Service Class have to be called after
*   this function.
* \class
*   Cyclic Service Class
* \params
*   pvUser  [in] User specific parameter
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INIT_FAILED                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_CyclicInitialize(void* pvUser)
{
  /* reset generation of cyclic events */
  NX_WRITE32(s_ptTriggerSample->ulTrigger_activate, 0);

  return ETH2PS_OKAY;
}


/*****************************************************************************/
/*! Configure the generation of Cyclic Events
* \description
*   This function sets all parameters for the generation of cyclic events.
* \class
*   Cyclic Service Class
* \params
*   ptCfg  [in] Parameter structure
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_CyclicConfig(ETH2PS_CYCLIC_CFG_T* ptCfg)
{
  uint32_t ulCfgCyclicOp = 0;

  /* parameter check DT_TRG0 if TRG0 activated */
  if( ptCfg->atTrgCfg[0].fTrgEn )
  {
    /* check against hardware specific limit */
    if( ( ptCfg->atTrgCfg[0].ulPeriod < ETH2PS_SYNC_PERIOD_MIN )
     || ( ptCfg->atTrgCfg[0].ulPeriod >= ETH2PS_SYSTIME_BORDER )  )
      return ETH2PS_ERR_INVAL_PARAM;
  }

  /* parameter check DT_TRG1 if TRG1 activated */
  if( ptCfg->atTrgCfg[1].fTrgEn )
  {
    /* check against hardware specific limit */
    if( ( ptCfg->atTrgCfg[1].ulPeriod < ETH2PS_SYNC_PERIOD_MIN )
     || ( ptCfg->atTrgCfg[1].ulPeriod >= ETH2PS_SYSTIME_BORDER ) )
      return ETH2PS_ERR_INVAL_PARAM;
  }

  /* parameter check if both TRG0 and TRG1 activated */
  if( ptCfg->atTrgCfg[0].fTrgEn && ptCfg->atTrgCfg[1].fTrgEn )
  {
    /* check that DT_TRG0 and DT_TRG1 are multiples of each other */
    if( (  ptCfg->atTrgCfg[1].ulPeriod > ptCfg->atTrgCfg[0].ulPeriod )
     && ( (ptCfg->atTrgCfg[1].ulPeriod % ptCfg->atTrgCfg[0].ulPeriod) != 0 ) )
      return ETH2PS_ERR_INVAL_PARAM;

    if( (  ptCfg->atTrgCfg[1].ulPeriod < ptCfg->atTrgCfg[0].ulPeriod )
     && ( (ptCfg->atTrgCfg[0].ulPeriod % ptCfg->atTrgCfg[1].ulPeriod) != 0 ) )
      return ETH2PS_ERR_INVAL_PARAM;
  }

  /* configure trigger_sample unit */
  s_t2Ps.ulTriggerActivate = 0;

  if( ptCfg->atTrgCfg[0].fTrgEn )       s_t2Ps.ulTriggerActivate |= MSK_NXT(trigger_activate_sync0_activate);
  if( ptCfg->atTrgCfg[0].fTrgPolarity ) ulCfgCyclicOp |= MSK_NXT(trigger_sample_config_sync0_polarity);
  if( ptCfg->atTrgCfg[0].fTrgOe )       ulCfgCyclicOp |= MSK_NXT(trigger_sample_config_sync0_oe);
  if( ptCfg->atTrgCfg[0].fIrqEn )       ulCfgCyclicOp |= MSK_NXT(trigger_sample_config_sync0_irq_en);

  if( ptCfg->atTrgCfg[1].fTrgEn )       s_t2Ps.ulTriggerActivate |= MSK_NXT(trigger_activate_sync1_activate);
  if( ptCfg->atTrgCfg[1].fTrgPolarity ) ulCfgCyclicOp |= MSK_NXT(trigger_sample_config_sync1_polarity);
  if( ptCfg->atTrgCfg[1].fTrgOe )       ulCfgCyclicOp |= MSK_NXT(trigger_sample_config_sync1_oe);
  if( ptCfg->atTrgCfg[1].fIrqEn )       ulCfgCyclicOp |= MSK_NXT(trigger_sample_config_sync1_irq_en);

  NX_WRITE32(s_ptTriggerSample->ulTrigger_sample_config, ulCfgCyclicOp);
  NX_WRITE32(s_ptTriggerSample->ulTrigger_impulse_length, ptCfg->ulTrgPulseLen | (ptCfg->ulTrgPulseLen << 16));

  NX_WRITE32(s_ptTriggerSample->aulTrigger__cyc_time[0], ptCfg->atTrgCfg[0].ulPeriod);
  NX_WRITE32(s_ptTriggerSample->aulTrigger__cyc_time[1], ptCfg->atTrgCfg[1].ulPeriod);

  s_t2Ps.aulTriggerOffset[0] = ptCfg->atTrgCfg[0].ulStartOffset;
  s_t2Ps.aulTriggerOffset[1] = ptCfg->atTrgCfg[1].ulStartOffset;

  return ETH2PS_OKAY;
}

/*****************************************************************************/
/*! Start Generation of Cyclic Events
* \description
*   Starts the generation of cyclic events.
* \class
*   Cyclic Service Class
* \params
*   ulStartTimeNs  [in] Start time of the first cycle [ns]                   */
/*****************************************************************************/
void Eth2PS_CyclicStart(uint32_t ulStartTimeNs)
{
  uint32_t ulDiff = (ETH2PS_SYSTIME_BORDER - ulStartTimeNs);
  uint32_t ulAbsStart0, ulAbsStart1;

  /* wrap absolute start time of each trigger if necessary */
  ulAbsStart0 = ( ulDiff > s_t2Ps.aulTriggerOffset[0] ) ? (ulStartTimeNs + s_t2Ps.aulTriggerOffset[0]) : (s_t2Ps.aulTriggerOffset[0] - ulDiff);
  ulAbsStart1 = ( ulDiff > s_t2Ps.aulTriggerOffset[1] ) ? (ulStartTimeNs + s_t2Ps.aulTriggerOffset[1]) : (s_t2Ps.aulTriggerOffset[1] - ulDiff);
  NX_WRITE32(s_ptTriggerSample->aulTrigger__starttime_ns[0], ulAbsStart0);
  NX_WRITE32(s_ptTriggerSample->aulTrigger__starttime_ns[1], ulAbsStart1);

  /* Start trigger_sample unit */
  NX_WRITE32(s_ptTriggerSample->ulTrigger_activate, s_t2Ps.ulTriggerActivate | MSK_NXT(trigger_activate_trigger_unit_activate));
}


/*****************************************************************************/
/*! Reset Generation of Cyclic Events
* \description
*   Resets the generation of cyclic trigger events.
* \class
*   Cyclic Service Class                                                     */
/*****************************************************************************/
void Eth2PS_CyclicStop(void)
{
  NX_WRITE32(s_ptTriggerSample->ulTrigger_activate, 0);
}

/*****************************************************************************/
/*! Get Cyclic Interrupts
* \description
*   Retrieves and confirms the current interrupt requests
*   from the synchronization unit.
* \class
*   Cyclic Service Class
* \return
*   Bit mask of pending interrupts                                           */
/*****************************************************************************/
uint32_t Eth2PS_CyclicGetCnfIrq(void)
{
  uint32_t ulIrq;

  /* get IRQs */
  ulIrq = NX_READ32(s_ptTriggerSample->ulTrigger_sample_irq);

  /* confirm IRQs */
  NX_WRITE32(s_ptTriggerSample->ulTrigger_sample_irq, ulIrq);

  return ulIrq;
}

/*****************************************************************************/
/*! Set Link Mode
* \description
*   This function sets the link mode of a switch port.
*   Note: These values must match the mode the connected PHY is set to. Also in case of link down this function has to be called.
* \class
*   Control Service Class
* \params
*   uiPort            [in] XC port number
*   fValid            [in] true: link up
*   uiSpeed           [in] 10/100
*   fFdx              [in] true: FDX
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_SetLinkMode( unsigned int uiPort, bool fValid, unsigned int uiSpeed, bool fFdx )
{
  unsigned int uiXc = XC_INST; /* default: invalid link and SPEED10 and HDX */
  unsigned int uiXpec;
  uint32_t ulVal = 0;

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check the port number */
  if(uiPort >= ETH2PS_PORTS)
    return ETH2PS_ERR_INVAL_PORT;
  /* check speed */
  if( (uiSpeed != 10)
    &&(uiSpeed != 100)
    )
    return ETH2PS_ERR_INVAL_PARAM;
#endif

  uiXpec = 2 * uiXc + (uiPort ^ 1);

  if( fValid )
  {
    /* link valid: set speed and duplex */
    ulVal |= MSK_ETH2PS_ARM2XPEC_INTERRUPTS_VALID;
    if( uiSpeed == 100 ) ulVal |= MSK_ETH2PS_ARM2XPEC_INTERRUPTS_SPEED100;
    if( fFdx ) ulVal |= MSK_ETH2PS_ARM2XPEC_INTERRUPTS_FDX;
  }

  /* set request (Note: xPEC0 writes sr6 (link mode of P1), xPEC1 writes sr2 (link mode of P0) */
  NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec], MSK_ETH2PS_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ | ulVal);

  /* wait for xPEC confirmed request */
  while( NX_READ32(s_ptXpecIrqRegs->aulIrq_xpec[uiXpec] & MSK_ETH2PS_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ) ) {}

  return ETH2PS_OKAY;
}

/*****************************************************************************/
/*! Get fully qualified frame handle from FIFO entry
* \description
*   Initialize a fully qualified frame handle from FIFO entry.
* \class
*   Communication Service Class
* \params
*   ulFifoEntry     [in]  FIFO entry
*   ptFrame         [out] Frame handle structure filled out by the function on success
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_InitFrameHandleFromFifoEntry(uint32_t ulFifoEntry, ETH2PS_FRAME_HANDLE_T* ptFrame)
{
  unsigned int uiPort;
  uint32_t ulFrameBufNo;

  /* compute frame buffer number and xPEC port */
  uiPort       = (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_PORT)             >> SRT_ETH2PS_FIFO_ELEMENT_PORT;
  ulFrameBufNo = (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER) >> SRT_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER;

  if( (ulFrameBufNo < ETH2PS_FRAME_BUF_NUM_MIN)
    ||(ulFrameBufNo >= ETH2PS_FRAME_BUF_CNT)
    ) {
    return ETH2PS_ERR_INVAL_PARAM;
  }

  /* fill out frame handle structure */
  ptFrame->ulFifoEntry = ulFifoEntry;
  ptFrame->ptHdr       = (ETH2PS_FRAME_BUF_HDR_T*)(ETH2PS_XC_INTRAM_PTR[XC_INST * 2] + ulFrameBufNo * ETH_FRAME_BUF_SIZE_DW);
  ptFrame->pbData      = (uint8_t*)(ptFrame->ptHdr + 1);

  /* indication or confirmation? */
  if( 0 != (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_CON_EN) )
  {
    /* confirmation */
    ptFrame->usLength = (uint16_t)NX_READ32(s_aptXpecRam[uiPort^1]->tETH2PS_FRAME_LEN_AREA.aulDataField[ulFrameBufNo]);
  }
  else
  {
    /* indication */
    ptFrame->usLength = (uint16_t)NX_READ32(s_aptXpecRam[uiPort]->tETH2PS_FRAME_LEN_AREA.aulDataField[ulFrameBufNo]);
  }

  return ETH2PS_OKAY;
}

/*****************************************************************************/
/*! Set frame length from FIFO entry
* * \description
*   Set frame length within xPEC DRAM regarding frame buffer number got by FIFO entry.
* \class
*   Communication Service Class
* \params
*   ulFifoEntry     [in]  FIFO entry
*   usLength        [in]  Frame length
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_SetFrameLengthFromFifoEntry(uint32_t ulFifoEntry, uint16_t usLength)
{
  unsigned int uiPort;
  uint32_t ulFrameBufNo;

  /* compute frame buffer number and xPEC port */
  uiPort       = (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_PORT)             >> SRT_ETH2PS_FIFO_ELEMENT_PORT;
  ulFrameBufNo = (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER) >> SRT_ETH2PS_FIFO_ELEMENT_FRAME_BUF_NUMBER;

  if( (ulFrameBufNo < ETH2PS_FRAME_BUF_NUM_MIN)
    ||(ulFrameBufNo >= ETH2PS_FRAME_BUF_CNT)
    ) {
    return ETH2PS_ERR_INVAL_PARAM;
  }

  /* indication or confirmation? */
  if( 0 != (ulFifoEntry & MSK_ETH2PS_FIFO_ELEMENT_CON_EN) )
  {
    /* confirmation */
    /* set frame length in xPEC DRAM */
    NX_WRITE32(s_aptXpecRam[uiPort^1]->tETH2PS_FRAME_LEN_AREA.aulDataField[ulFrameBufNo], (uint32_t)usLength);
  }
  else
  {
    /* indication */
    /* set frame length in xPEC DRAM */
    NX_WRITE32(s_aptXpecRam[uiPort]->tETH2PS_FRAME_LEN_AREA.aulDataField[ulFrameBufNo], (uint32_t)usLength);
  }

  return ETH2PS_OKAY;
}

/*****************************************************************************/
/*! Configure MII
* \description
*   This function configures the MII that is used.
*   Note: Only call this function before XC started.
*   Note: Use this function only when connecting an external PHY to compensate delays between external PHY and internal MAC logic.
*   Note: Default value fits to internal PHYs if available else to external MII.
* \class
*   Control Service Class
* \params
*   uiPort           [in] XC port number
*   uiCfg            [in] MII configuration; 0: internal PHY, 1: external MII
* \return
*   ETH2PS_OKAY
*   ETH2PS_ERR_INVAL_PARAM                                                   */
/*****************************************************************************/
ETH2PS_RESULT_E Eth2PS_CfgMii( unsigned int uiPort,
                               unsigned int uiCfg )
{
  ETH2PS_RESULT_E eRslt = ETH2PS_OKAY;
  uint32_t ulVal = 0;
  unsigned int uiMiiOutputPhase = 0;
  uint32_t ulTxCorrBase = 0;
  unsigned int uiMac = XC_INST * 2 + uiPort;

#ifndef __ETH2PS_DISABLE_CHECKS__
  /* check port no */
  if(uiPort >= ETH2PS_PORTS)
    return ETH2PS_ERR_INVAL_PORT;
#endif

  switch(uiCfg)
  {
  case 0:
    /* internal PHY (default) */
    s_t2Ps.atPort[uiPort].fExtMii = false;
    uiMiiOutputPhase = 0;
    ulTxCorrBase = 15;
    break;

  case 1:
    /* external MII */
    s_t2Ps.atPort[uiPort].fExtMii = true;
#if defined(HAL_ETH2PS_TARGET_NX4000)
    /* Note: In netx4000FULL the pads XM??_TX{D?,EN,ER} have output delays of round 10 ns. */
    /* Note: In netx4000FULL a additional output-flip-flop is inserted in each path to external MII (to pads XM??_TX{D?,EN,ER}). */
    uiMiiOutputPhase = 2;
#if XC_INST == 1
    ulTxCorrBase = 35; /* +35 ns */
#else
    ulTxCorrBase = 0xfffffffb; /* -5 ns */
#endif
#elif defined(HAL_ETH2PS_TARGET_NX90)
    /* Note: In netx90 the pads XM??_TX{D?,EN,ER} have output delays of round 10 ns. */
    uiMiiOutputPhase = 3;
    ulTxCorrBase = 5;
#endif
    break;

  default:
    eRslt = ETH2PS_ERR_INVAL_PARAM;
    break;
  }

  if( eRslt == ETH2PS_OKAY )
  {
    /* change default output phase */
    ulVal  = NX_READ32(s_aptXmacArea[uiMac]->ulXmac_config_tx_nibble_fifo);
    ulVal &= ~MSK_NXT(xmac_config_tx_nibble_fifo_output_phase);
    ulVal |= uiMiiOutputPhase << SRT_NXT(xmac_config_tx_nibble_fifo_output_phase);
    NX_WRITE32( s_aptXmacArea[uiMac]->ulXmac_config_tx_nibble_fifo, ulVal );

    /* update correction base values, Note: tpec0 serves tpu1 and vice versa */
    NX_WRITE32(s_aptXpecRam[uiPort^1]->tETH2PS_CONFIGAREABASE.ulETH2PS_TS_COR_BASE_TX, ulTxCorrBase);
  }

  return eRslt;
}
