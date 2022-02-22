/****************************************************************************************
*  Ethernet Driver for the Ethernet 2-Port Switch HAL
*
*  This module implements the Ethernet API defined in "eth.h".
*  It is an adapter between the stack and the HAL.
****************************************************************************************/

#include "ip_stack.h"
#include "eth_2ps.h"
#include "ip.h"
#include "arp.h"
#include "phy.h"

#include <string.h>


/****************************************************************************************
*  Definitions
****************************************************************************************/

#ifndef ETH_2PS_FRAME_FILTER
#define ETH_2PS_FRAME_FILTER(...) true /*lint -e506 */
#endif

/* Configure if all non ARP/IP frames shall be echoed with swapped MAC addresses for test purposes */
#ifndef ETH_ECHO_UNKNOWN_FRAMES
#define ETH_ECHO_UNKNOWN_FRAMES 0
#endif

#define ETH_FRAME_BUF_SIZE  1536 /* application buffer size for one frame */

#define ETH_PRE_PADDING        0

#define ETH_2PS_NO_PERF_MEASURE 1

/****************************************************************************************
*  Variables
****************************************************************************************/

ETH_2PS_T g_tEth2PS;
ISR_CNT_T g_tIsrCnt;

static unsigned int s_auIfPortMap[] = {3, 0, 1, 2};

#ifdef LOG_ENABLE
static const char* s_pszModule = __FILE__;
#endif


/****************************************************************************************
*  Functions
****************************************************************************************/

int eth_2ps_send(void* pvInst, uint32_t ulInterfaces, SG_T* ptSg, unsigned int uiSgLen, unsigned int uiPrio, ETH_TX_CNF_HANDLER_T* ptCnfHandler)
{
  ETH2PS_FRAME_HANDLE_T tFrameHandle;
  ETH2PS_RESULT_E       eRslt;
  unsigned int          uiLen;
  uint8_t*              pbData;
  bool                  fConfirm = (ptCnfHandler != NULL);
  unsigned int          uCnfCnt;

  /* evaluate port numbers */
  if( ulInterfaces > 3 )
  {
    return -2;
  }

  /* Allocate enough buffers for the frame */
  if( ETH2PS_OKAY != Eth2PS_GetFrame( &tFrameHandle ) )
  {
    /* no resources */
    return -1;
  }

  pbData = tFrameHandle.pbData;
  uiLen = 0;
  while( uiLen < uiSgLen )
  {
    uiLen += ptSg->uiLen;
    if( uiLen > sizeof(ETH_FRAME_T) )
    {
      /* frame too long */
      if( ETH2PS_OKAY != Eth2PS_ReleaseFrame(&tFrameHandle) )
      {
        FATAL_ERROR("%s:%d: Eth2PS_ReleaseFrame() failed", s_pszModule, __LINE__);
      }
      return -3;
    }
    memcpy(pbData, ptSg->pvData, ptSg->uiLen);
    pbData += ptSg->uiLen;
    ptSg++;
  }

  uiLen = uiSgLen;

  while( uiLen < 60 )
  {
#ifdef ETH_2PS_PADDING
    *pbData++ = 0;
#endif
    ++uiLen;
  }

  /* Store the confirmation call-back in the frame for asynchronous confirmation */
  tFrameHandle.ptHdr->ulUserData0 = (uint32_t)ptCnfHandler;

  tFrameHandle.usLength = (uint16_t)uiLen;
  eRslt = Eth2PS_Send(s_auIfPortMap[ulInterfaces], &tFrameHandle, (uiPrio >= 4) ? 1 : 0, fConfirm, &uCnfCnt);

  if( ETH2PS_OKAY != eRslt )
  {
    /* send failed */
    if( ETH2PS_OKAY != Eth2PS_ReleaseFrame(&tFrameHandle) )
    {
      FATAL_ERROR("%s:%d: Eth2PS_ReleaseFrame() failed", s_pszModule, __LINE__);
    }
    return -4;
  }

  return 0;
}

/* Initialize Ethernet interface */
ETH_T* eth_2ps_init(ETH_2PS_CONFIG_T* ptConfig)
{
  ETH_2PS_T* ptEth2PS = &g_tEth2PS;
  ETH2PS_CFG_T tCfg;
  ETH_T* ptEth = &ptEth2PS->tEth;
  unsigned int uiPort;
  uint32_t ulIrqEn =  MSK_ETH2PS_IRQ_IND_CNF_LO
                    + MSK_ETH2PS_IRQ_IND_CNF_HI
                    + MSK_ETH2PS_IRQ_LINK_CHANGED
                    + MSK_ETH2PS_IRQ_CIRCULATING_FRM;

  memset(&tCfg, 0, sizeof(tCfg));
  memset(&g_tIsrCnt, 0, sizeof(g_tIsrCnt));
  memset(&g_tEth2PS, 0, sizeof(g_tEth2PS));

  ptEth->fnSend      = eth_2ps_send;
  ptEth->uIfCnt      = sizeof(ptEth2PS->atEthIf)/sizeof(ptEth2PS->atEthIf[0]);
  ptEth->patIf       = ptEth2PS->atEthIf;
  ptEth->pvInst      = ptEth2PS;
  ptEth->pbMacAddr   = &ptEth2PS->tAddrChassis0[0];

  memcpy(&ptEth2PS->tAddrChassis0[0], ptConfig->ptAddrChassis0, 6);
  memcpy(&ptEth2PS->tAddrChassis1[0], ptConfig->ptAddrChassis1, 6);

  tCfg.ePhyLedCfg          = ETH2PS_PHYLED_BLINK;
  tCfg.ulActLedFlashPeriod = 50;

  if( ETH2PS_OKAY != Eth2PS_Initialize(&tCfg, NULL) )
  {
    FATAL_ERROR("%s:%d:Eth2PS_Initialize() failed", s_pszModule, __LINE__);
  }

  /* set my MAC addresses */
  if( ETH2PS_OKAY != Eth2PS_SetMacAddr(ETH2PS_MAC_ADDRESS_CHASSIS, *ptConfig->ptAddrChassis0) )
  {
    FATAL_ERROR("%s:%d:Eth2PS_SetMacAddr() failed", s_pszModule, __LINE__);
  }

  if( ETH2PS_OKAY != Eth2PS_SetMacAddr(ETH2PS_MAC_ADDRESS_2ND_CHASSIS, *ptConfig->ptAddrChassis1) )
  {
    FATAL_ERROR("%s:%d:Eth2PS_SetMacAddr() failed", s_pszModule, __LINE__);
  }

  for(uiPort = 0; uiPort < 2; ++uiPort)
  {
    /* set aging time */
    if( ETH2PS_OKAY != Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_AGING_TIME, ptConfig->ulAgingTimeMs) )
    {
      FATAL_ERROR("%s:%d:Eth2PS_SetParameter(ETH2PS_PARAM_AGING_TIME) failed", s_pszModule, __LINE__);
    }

    /* set PHY latencies */
    if( ETH2PS_OKAY != Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_INGRESS_LATENCY, ptConfig->atPortCfg[uiPort].ulIngressLatency ) )
    {
      FATAL_ERROR("%s:%d:Eth2PS_SetParameter(ETH2PS_PARAM_INGRESS_LATENCY) failed", s_pszModule, __LINE__);
    }

    if( ETH2PS_OKAY != Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_EGRESS_LATENCY, ptConfig->atPortCfg[uiPort].ulEgressLatency ) )
    {
      FATAL_ERROR("%s:%d:Eth2PS_SetParameter(ETH2PS_PARAM_EGRESS_LATENCY) failed", s_pszModule, __LINE__);
    }

    /* enable all IRQs on switch ports */
    if( ETH2PS_OKAY != Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_IRQ_EN_MSK, ulIrqEn) )
    {
      FATAL_ERROR("%s:%d:Eth2PS_SetParameter(ETH2PS_PARAM_IRQ_EN_MSK) failed", s_pszModule, __LINE__);
    }

    /* configure default MII to external MII usage if requested */
    if( ptConfig->atPortCfg[uiPort].fSetExtMii )
    {
      if( ETH2PS_OKAY != Eth2PS_CfgMii(uiPort, 1) )
      {
        FATAL_ERROR("%s:%d:Eth2PS_CfgMii() failed", s_pszModule, __LINE__);
      }
    }

    /* Disable Tx link check if needed */
    if(ptConfig->atPortCfg[uiPort].fNoPhyLinkInput)
    {
      if( ETH2PS_OKAY != Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_LINK_INPUT_ENABLE, 0) )
      {
        FATAL_ERROR("%s:%d:Eth2PS_SetParameter(ETH2PS_PARAM_LINK_INPUT_ENABLE) failed", s_pszModule, __LINE__);
      }
    }

    /* Set initial state of PORT_ENABLE */
    if(ptConfig->atPortCfg[uiPort].fDisablePort)
    {
      if( ETH2PS_OKAY != Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_PORT_ENABLE, 0))
      {
        FATAL_ERROR("%s:%d:Eth2PS_SetParameter(ETH2PS_PARAM_PORT_ENABLE) failed", s_pszModule, __LINE__);
      }
    }

    /* initialize Ingress and Egress latencies in driver */
    ptEth2PS->atIf[uiPort].ulPortRxDelayLocal = ptConfig->atPortCfg[uiPort].ulIngressLatency;
    ptEth2PS->atIf[uiPort].ulPortTxDelayLocal = ptConfig->atPortCfg[uiPort].ulEgressLatency;
  }

  eth_add(ptEth);

  return ptEth;
}

void eth_2ps_start(ETH_T* ptEth)
{
  /* start switch */
  if( ETH2PS_OKAY != Eth2PS_Start(0) )
  {
    FATAL_ERROR("%s:%d:Eth2PS_Start() failed", s_pszModule, __LINE__);
  }

#ifndef ETH_2PS_NO_MAU_TYPE_INIT
  /* get initial connection state */
  eth_2ps_mau_type_change_ind(ptEth, 0);
  eth_2ps_mau_type_change_ind(ptEth, 1);
#endif
}


void eth_2ps_mau_type_change_ind(ETH_T* ptEth, unsigned int uiPort)
{
  ETH_2PS_T* ptEth2PS = (ETH_2PS_T*)ptEth->pvInst;
  ETH_MAU_TYPE_T* ptState = &ptEth2PS->atEthIf[uiPort].tMauType;

  /* get current link mode */
  if( 0 != phy_get_linkstate(uiPort, &ptState->uIsLinkUp, &ptState->uSpeed, &ptState->uIsFullDuplex) )
  {
    FATAL_ERROR("%s:%d: phy_get_linkstate() failed!", s_pszModule, __LINE__);
  }

  /* Update link mode in MAC, please note that link mode must also updated in case of link down */
  if( ETH2PS_OKAY != Eth2PS_SetLinkMode(uiPort, true, ptState->uSpeed, ptState->uIsFullDuplex) )
  {
    FATAL_ERROR("%s:%d: Eth2PS_SetLinkMode() failed!", s_pszModule, __LINE__);
  }

  ptEth2PS->atEthIf[uiPort].tIfEntry.bOperStatus = ptState->uIsLinkUp ? 1 : 2;
  ptEth2PS->atEthIf[uiPort].tIfEntry.ulSpeed     = ptState->uSpeed * 1000000;

  /* reset PhyPhaseOffset */
  ptEth2PS->atIf[uiPort].fPhyPhaseOffsetValid   = false;
  ptEth2PS->atIf[uiPort].bPhyPhaseOffset        = 0;
}


/* Update diagnostic counters and other information */
void eth_2ps_update_counters(ETH_T* ptEth)
{
  ETH_2PS_T* ptEth2PS = (ETH_2PS_T*)ptEth->pvInst;
  unsigned int uiPort;

  ptEth2PS->ulEmptyPtrFillLevel = Eth2PS_GetEmptyFillLevel();
  ptEth2PS->ulIndCnfLoFillLevel = Eth2PS_GetIndCnfFillLevel(0);
  ptEth2PS->ulIndCnfHiFillLevel = Eth2PS_GetIndCnfFillLevel(1);

  for(uiPort = 0; uiPort < ETH2PS_PORTS; ++uiPort)
  {
    /* get statistics for both channels */
    ETH2PS_COUNTERS_T* ptMacCnt = &ptEth2PS->atIf[uiPort].tMacCnt;
    if( ETH2PS_OKAY != Eth2PS_GetCounters(uiPort, ptMacCnt) )
    {
      FATAL_ERROR("%s:%d:Eth2PS_GetCounters() failed", s_pszModule, __LINE__);
    }

    /* Ethernet interface counters */
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulAlignmentErrors           = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulFCSErrors                 = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulSingleCollisionFrames     = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulMultipleCollisionFrames   = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulSQETestErrors             = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulDeferredTransmissions     = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulLateCollisions            = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulExcessiveCollisions       = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulInternalMacTransmitErrors = ptMacCnt->ulTxUnderrun;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulCarrierSenseErrors        = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulFrameTooLongs             = 0;
    ptEth2PS->atEthIf[uiPort].tDot3StatsEntry.ulInternalMacReceiveErrors  = ptMacCnt->ulRxOverflow
                                                                          + ptMacCnt->ulRxDiscarded;

    /* Generic interface counters */
    ptEth2PS->atEthIf[uiPort].tIfEntry.ulInOctets                         = ptMacCnt->ulRxInOctets;
    ptEth2PS->atEthIf[uiPort].tIfEntry.ulInErrors                         = ptMacCnt->ulRxFcsErrors
                                                                          + ptMacCnt->ulRxAlignmentErrors
                                                                          + ptMacCnt->ulRxFrameLengthErrors
                                                                          + ptMacCnt->ulRxRuntFrames
                                                                          + ptMacCnt->ulRxCollisionFragments;
    ptEth2PS->atEthIf[uiPort].tIfEntry.ulInDiscards                       = ptMacCnt->ulRxOverflow
                                                                          + ptMacCnt->ulRxDiscarded;
    ptEth2PS->atEthIf[uiPort].tIfEntry.ulOutOctets                        = ptMacCnt->ulTxOutOctets;
    ptEth2PS->atEthIf[uiPort].tIfEntry.ulOutErrors                        = ptMacCnt->ulTxUnderrun;
    ptEth2PS->atEthIf[uiPort].tIfEntry.ulOutDiscards                      = ptMacCnt->ulTxDiscarded
                                                                          + ptMacCnt->ulTxUnderrun;
    ptEth2PS->atEthIf[uiPort].tIfEntry.ulOutQLen                          = Eth2PS_GetReqFillLevel(uiPort, 0)
                                                                          + Eth2PS_GetReqFillLevel(uiPort, 1);
  }
}

/* one or more frames were received */
void eth_2ps_ind_cnf(ETH_T* ptEth, unsigned int uPriority, uint32_t ulMaxFrames)
{
  ETH_2PS_T* ptEth2PS = (ETH_2PS_T*)ptEth->pvInst;
  ETH2PS_FRAME_HANDLE_T tFrameHandle;
  unsigned int          uiLen;
  uint32_t              ulFillLevel, ulCnt;
  ETH2PS_FRAME_INFO_T   tFrmInfo;
  bool                  fRelease;
  ETH_TX_CNF_HANDLER_T* ptCnfHandler;
  ETH_INFO_T            tInfo;
#ifndef ETH_2PS_NO_PERF_MEASURE
  unsigned int          uPrio = ePFifo - ETH2PS_PFIFO_IND_CNF_NRT;
  uint64_t              ullTime;
  uint32_t              ulDuration;
#endif

  /* Get the current fill level */
  ulFillLevel = Eth2PS_GetIndCnfFillLevel(uPriority);

  /* Limit number of processed frames */
  if( ulMaxFrames == 0 )
    ulMaxFrames = ulFillLevel;
  else
    ulMaxFrames = MIN(ulMaxFrames, ulFillLevel);

  /* Retrieve all frames in FIFO */
  for(ulCnt = 0; ulCnt < ulMaxFrames; ++ulCnt)
  {
#ifndef ETH_2PS_NO_PERF_MEASURE
    ullTime = systime_get();
#endif

    /* receive one packet */
    if( ETH2PS_OKAY != Eth2PS_GetIndCnf(uPriority, &tFrameHandle, &tFrmInfo) )
    {
      //sim_message("Eth2PS_GetIndCnf", disp_only, 0);
      FATAL_ERROR("%s:%d:eth_2ps_ind_cnf() Eth2PS_GetIndCnf() failed, priority: %d", s_pszModule, __LINE__, uPriority);
      continue;
    }
    INC_CNT(ptEth2PS->ulFrameGet)
    uiLen = tFrameHandle.usLength;
    fRelease = true;

    if( ( uiLen < 60 ) || ( uiLen > 1518 ) )
    {
      FATAL_ERROR("%s:%d:eth_2ps_ind_cnf() port %d invalid frame length (%d) handle: %08x %s", s_pszModule, __LINE__, tFrmInfo.uPortNo, uiLen, tFrameHandle.ulFifoEntry, tFrmInfo.fCnf ? "cnf" : "ind");
      continue;
    }

    /* store time stamp */
    tInfo.tTimestamp.ullSystime   = ((uint64_t)tFrmInfo.ulTimeS << 32) | tFrmInfo.ulTimeNs;
    tInfo.tTimestamp.ullSystimeUc = tInfo.tTimestamp.ullSystime;

    if( ETH_2PS_FRAME_FILTER(tFrmInfo.uPortNo, &tFrameHandle, uPriority, tFrmInfo.fCnf) )
    {
      /* handle indication */
      if( tFrmInfo.fCnf )
      {
        /* Get the confirmation handler */
        ptCnfHandler = (ETH_TX_CNF_HANDLER_T*)tFrameHandle.ptHdr->ulUserData0;

        if( ( ptCnfHandler != NULL ) && ( ptCnfHandler->pfnHandleConfirmation != NULL ) )
        {
          /* Call back application */
          fRelease = ptCnfHandler->pfnHandleConfirmation(ptCnfHandler->pvContext, ( tFrmInfo.eCnfResult <= ETH2PS_CNF_ERR_CODE_SUCCESSFUL_WITH_RETRIES ), &tInfo.tTimestamp, (uint32_t)tFrmInfo.eCnfResult);
        }
      }
      else
      {
        SG_T atFrameFragments[1];
        unsigned int uiPort = tFrmInfo.uPortNo;

#ifdef ETH_2PS_UPDATE_PHY_PHASE_OFFSET
        /* update PHY phase offset after first received frame */
        if( ! ptEth2PS->atIf[uiPort].fPhyPhaseOffsetValid )
        {
          if( ETH2PS_OKAY != Eth2PS_GetPhyPhaseOffset(uiPort, &ptEth2PS->atIf[uiPort].bPhyPhaseOffset, NULL) )
          {
            FATAL_ERROR("%s:%d:Eth2PS_GetPhyPhaseOffset() failed", s_pszModule, __LINE__);
          }
          ptEth2PS->atIf[uiPort].fPhyPhaseOffsetValid = true;

          /* update port ingress latency */
          if( ETH2PS_OKAY != Eth2PS_SetParameter(uiPort, ETH2PS_PARAM_INGRESS_LATENCY, ptEth2PS->atIf[uiPort].ulPortRxDelayLocal + ptEth2PS->atIf[uiPort].bPhyPhaseOffset ) )
          {
            FATAL_ERROR("%s:%d:Eth2PS_SetParameter(ETH2PS_PARAM_INGRESS_LATENCY) failed", s_pszModule, __LINE__);
          }

        }
#endif

        /* setup list of fragments */
        atFrameFragments[0].pvData = tFrameHandle.pbData;
        atFrameFragments[0].uiLen  = uiLen;

        /* process frame */
        tInfo.ptEth  = &ptEth2PS->tEth;
        tInfo.uiPort = tFrmInfo.uPortNo;
        eth_process_frame(&tInfo, atFrameFragments, uiLen);
      }
    }

#ifndef ETH_2PS_NO_RELEASE_FRAME
    if( fRelease )
    {
      /* put frame buffer back into resource pool */
      if( ETH2PS_OKAY != Eth2PS_ReleaseFrame(&tFrameHandle) )
      {
        FATAL_ERROR("%s:%d:Eth2PS_ReleaseFrame() failed", s_pszModule, __LINE__);
      }

      INC_CNT(ptEth2PS->ulFrameFree)
    }
#endif

#ifndef ETH_2PS_NO_PERF_MEASURE
    /* latch maximum duration of frame handling */
    ulDuration = systime_get() - ullTime;
    if( fCnf )
    {
      if( ulDuration > ptEth2PS->aulMaxDurCnf[uPrio] )
        ptEth2PS->aulMaxDurCnf[uPrio] = ulDuration;
    }
    else
    {
      if( ulDuration > ptEth2PS->aulMaxDurInd[uPrio] )
        ptEth2PS->aulMaxDurInd[uPrio] = ulDuration;
    }
#endif
  }
}

void eth_2ps_inform_bcn_to(ETH_T* ptEth, void (*pfnBcnTimeout)(ETH_T* ptEth, unsigned int uiPort))
{
  ETH_2PS_T* ptEth2PS = (ETH_2PS_T*)ptEth->pvInst;
  ptEth2PS->pfnBcnTimeout = pfnBcnTimeout;
}

static void eth_2ps_bcn_timeout(ETH_T* ptEth, unsigned int uPortNo)
{
  ETH_2PS_T* ptEth2PS = (ETH_2PS_T*)ptEth->pvInst;
  if( ptEth2PS->pfnBcnTimeout != NULL )
  {
    ptEth2PS->pfnBcnTimeout(ptEth, uPortNo);
  }
}

/* Handle all xPEC interrupts */
void eth_2ps_irq_handler(ETH_T* ptEth, unsigned int uPortNo, bool fPriority )
{
  uint32_t ulIrqMsk;

  ulIrqMsk = Eth2PS_GetCnfIrq(uPortNo, fPriority);

  /* Check now the different interrupt sources */

  if( (ulIrqMsk & MSK_ETH2PS_IRQ_IND_CNF_HI) != 0 )
  {
    /* we got an high indication/confirmation, i.e. one or more high priority packets were received or sent */
    ++g_tIsrCnt.aulIndCnfHi[uPortNo];

    /* process indications/confirmations */
    eth_2ps_ind_cnf(ptEth, 1, 0);
  }

  if( (ulIrqMsk & MSK_ETH2PS_IRQ_IND_CNF_LO) != 0 )
  {
    /* we got an low indication/confirmation, i.e. one or more low priority packets were received or sent */
    ++g_tIsrCnt.aulIndCnfLo[uPortNo];

    /* process indications/confirmations */
    eth_2ps_ind_cnf(ptEth, 0, 0);
  }

  if( (ulIrqMsk & MSK_ETH2PS_IRQ_LINK_CHANGED) != 0 )
  {
    /* link has changed on one port */
    ++g_tIsrCnt.aulLinkChng[uPortNo];

    eth_2ps_mau_type_change_ind(ptEth, uPortNo);
  }

  if( (ulIrqMsk & MSK_ETH2PS_IRQ_CIRCULATING_FRM) != 0 )
  {
    /* we received and dropped a frame from ourself */
    ++g_tIsrCnt.aulCirclFrm[uPortNo];
  }

  if( (ulIrqMsk & MSK_ETH2PS_IRQ_BCN_TIMEOUT) != 0 )
  {
    /* beacon reception timed out */
    ++g_tIsrCnt.aulBeaconTO[uPortNo];

    eth_2ps_bcn_timeout(ptEth, uPortNo);
  }
}
