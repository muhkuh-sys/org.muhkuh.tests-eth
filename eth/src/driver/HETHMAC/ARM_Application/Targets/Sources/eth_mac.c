/****************************************************************************************
*  Ethernet Driver for the Ethernet Standard MAC HAL
*
*  This module implements the Ethernet API defined in "eth.h".
*  It is an adapter between the stack and the HAL.
****************************************************************************************/

#include "ip_stack.h"
#include "eth_mac.h"
#include "phy.h"
#include "eth.h"
#include "ip.h"
#include "arp.h"

#include <string.h>
#include <stdbool.h>


/****************************************************************************************
*  Definitions
****************************************************************************************/

#ifndef ETH_MAC_PORTS
#define ETH_MAC_PORTS 4
#endif

#ifdef ETH_MAC_FRAME_FILTER
bool ETH_MAC_FRAME_FILTER(unsigned int uPortNo, ETHERNET_FRAME_T* ptFrame, unsigned int uiLength, unsigned int uPriority, bool fCnf);
#else
#define ETH_MAC_FRAME_FILTER(p,f,len,prio,c) true
#endif

#define ETHMAC_FRAME_BUF_DATA_LEN_PRIMARY 1518

#define ETH_FRAME_BUF_SIZE  1536 /* application buffer size for one frame */

#define ETH_PRE_PADDING        0

#define ETH_MAC_NO_PERF_MEASURE 1

#define ACTIVITY_LED_FLASH_PERIOD 5000000 /* 50 milliseconds */

typedef void (*ETH_FRAME_HANDLER_CB)( ETH_INFO_T* ptEthInfoIn, uint8_t* pbData, uint16_t usSize );


typedef struct ETH_FRAME_FRAGMENT_Ttag
{
  uint8_t*                        pbData;
  uint16_t                        usSize;
  struct ETH_FRAME_FRAGMENT_Ttag* ptNext;
} ETH_FRAME_FRAGMENT_T;


/****************************************************************************************
*  Variables
****************************************************************************************/

ETH_MAC_T g_atEthMac[ETH_MAC_PORTS];

#if LOG_ENABLE
static const char* s_pszModule = __FILE__;
#endif


/****************************************************************************************
*  Functions
****************************************************************************************/

/* Initialize Ethernet interface */
ETH_T* eth_mac_init(unsigned int uiPort, const ETH_MAC_CONFIG_T* ptConfig)
{
  ETH_MAC_T* ptEthMac = &g_atEthMac[uiPort];
  ETH_T* ptEth = &ptEthMac->tEth;

  memset(ptEthMac, 0, sizeof(*ptEthMac));

  ptEthMac->uiPort = uiPort;
  MUTEX_INIT(ptEthMac->tMtx);
  ptEthMac->ullSystimeBorder = ptConfig->ullSystimeBorder;

  ptEth->fnSend      = eth_mac_send_frame;
  ptEth->uIfCnt      = sizeof(ptEthMac->atEthIf)/sizeof(ptEthMac->atEthIf[0]);
  ptEth->patIf       = ptEthMac->atEthIf;
  ptEth->pvInst      = ptEthMac;
  ptEth->pbMacAddr   = &ptEthMac->tAddrChassis0[0];

  memcpy(&ptEthMac->tAddrChassis0, ptConfig->ptAddrChassis0, 6);

  if( ETH_OKAY != EthMac_Initialize(ptEthMac->uiPort, ETH_PHY_LED_BLINK, ACTIVITY_LED_FLASH_PERIOD, NULL) )
  {
    FATAL_ERROR("%s:%d:EthMac_Initialize() failed");
  }

  /* configure MII timing */
  if( ETH_OKAY != EthMac_CfgMii(ptEthMac->uiPort, ptConfig->uiMiiCfg) )
  {
    FATAL_ERROR("%s:%d:EthMac_CfgMii() failed", s_pszModule, __LINE__);
  }

  /* set my MAC addresses */
  if( ETH_OKAY != EthMac_SetMacAddr(ptEthMac->uiPort, ETH_MAC_ADDRESS_CHASSIS, ptEthMac->tAddrChassis0) )
  {
    FATAL_ERROR("%s:%d:EthMac_SetMacAddr() failed", s_pszModule, __LINE__);
  }

  eth_add(ptEth);
  return ptEth;
}

void eth_mac_start(ETH_T* ptEth)
{
  ETH_MAC_T* ptEthMac = (ETH_MAC_T*)ptEth->pvInst;
  /* start switch */
  if( ETH_OKAY != EthMac_Start(ptEthMac->uiPort, NULL) )
  {
    FATAL_ERROR("%s:%d:EthMac_Start() failed", s_pszModule, __LINE__);
  }
}

/* Low level function for sending an Ethernet frame */
int eth_mac_send_frame(void* pvInst, uint32_t ulInterfaces, SG_T* ptSg, unsigned int uiTLen, unsigned int uiPrio, ETH_TX_CNF_HANDLER_T* ptConfirmationHandler)
{
  ETH_MAC_T* ptEthMac = (ETH_MAC_T*)pvInst;
  ETHERNET_FRAME_T*     ptFrameBuf;
  void*                 pvHandle;
  uint8_t*              pbData;
  unsigned int          uiLen, uiPadSize;
  ETHERNET_RESULT       eRslt;
  void**                ppvCnf;

  if( ulInterfaces > 1 )
  {
    return -2;
  }

  /* Allocate enough buffers for the frame */
  if( ETH_OKAY != EthMac_GetFrame(ptEthMac->uiPort, &ptFrameBuf, &pvHandle) )
  {
    return -1;
  }

  pbData = (uint8_t*)ptFrameBuf;
  uiLen  = 0;
  ppvCnf = (void**)&ptFrameBuf->abRes[6];
  *ppvCnf = ptConfirmationHandler;
  
  for(uiLen = 0; uiLen < uiTLen; ptSg++)
  {
    if( uiLen > sizeof(ETH_FRAME_T) )
    {
      EthMac_ReleaseFrame(ptEthMac->uiPort, pvHandle);
      return -3;
    }
    memcpy(pbData, ptSg->pvData, ptSg->uiLen);
    pbData += ptSg->uiLen;
    uiLen  += ptSg->uiLen;
  }

#ifdef ETH_MAC_PADDING
  uiPadSize = ( uiLen < ETH_MIN_FRAME_SIZE ) ? ETH_MIN_FRAME_SIZE : ((uiLen + 3U) & ~3U);
  if( uiLen < uiPadSize )
  {
    memset(pbData, 0, uiPadSize - uiLen);
  }
#endif

  /* set minimum frame length */
  if( uiLen < ETH_MIN_FRAME_SIZE )
  {
    uiLen = ETH_MIN_FRAME_SIZE;
  }
  
  if(ptConfirmationHandler == NULL)
  {
    eRslt = EthMac_SendWithoutCnf(ptEthMac->uiPort, pvHandle, uiLen, (uiPrio >= 4) ? 1 : 0);
  }
  else
  {
    eRslt = EthMac_Send(ptEthMac->uiPort, pvHandle, uiLen, (uiPrio >= 4) ? 1 : 0);
  }

  if( ETH_OKAY != eRslt )
  {
    EthMac_ReleaseFrame(ptEthMac->uiPort, pvHandle);
    return -4;
  }

  return 0;
}

void eth_mac_update_link_mode(ETH_T* ptEth)
{
  ETH_MAC_T* ptEthMac = (ETH_MAC_T*)ptEth->pvInst;
  ETH_MAU_TYPE_T* ptState = &ptEthMac->atEthIf[0].tMauType;

  /* get current link mode */
  phy_get_linkstate(ptEthMac->uiPort, ptState);

  /* Update link mode in MAC, please note that link mode must also updated in case of link down */
  EthMac_SetLinkMode(ptEthMac->uiPort, true, ptState->uSpeed, ptState->uIsFullDuplex);

  /* update MIB entries */
  ptEthMac->atEthIf[0].tIfEntry.bOperStatus   = ptState->uIsLinkUp ? 1 : 2;
  ptEthMac->atEthIf[0].tIfEntry.ulSpeed       = ptState->uSpeed * 1000000;

  /* reset PhyPhaseOffset */
  ptEthMac->atIf[0].fPhyPhaseOffsetValid   = false;
  ptEthMac->atIf[0].bPhyPhaseOffset        = 0;
}

/* Update diagnostic counters and other information */
void eth_mac_update_counters(ETH_T* ptEth)
{
  ETH_MAC_T* ptEthMac = (ETH_MAC_T*)ptEth->pvInst;
  unsigned int uiIf;

  EthMac_GetSendCnfFillLevel(ptEthMac->uiPort, 0, &ptEthMac->ulCnfLoFillLevel);
  EthMac_GetSendCnfFillLevel(ptEthMac->uiPort, 1, &ptEthMac->ulCnfHiFillLevel);
  EthMac_GetRecvFillLevel(ptEthMac->uiPort, 0, &ptEthMac->ulIndLoFillLevel);
  EthMac_GetRecvFillLevel(ptEthMac->uiPort, 1, &ptEthMac->ulIndHiFillLevel);

  for(uiIf = 0; uiIf < ETH_MAC_IF_CNT; ++uiIf)
  {
    ETHMAC_COUNTERS_T* ptStats = &ptEthMac->atIf[uiIf].tMacCnt;
    /* get statistics for this interface */
    EthMac_GetCounters(ptEthMac->uiPort, ptStats);

    /* Ethernet interface counters */
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulAlignmentErrors           = ptStats->ulETHMAC_ALIGNMENT_ERRORS;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulFCSErrors                 = ptStats->ulETHMAC_FRAME_CHECK_SEQUENCE_ERRORS;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulSingleCollisionFrames     = ptStats->ulETHMAC_SINGLE_COLLISION_FRAMES;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulMultipleCollisionFrames   = ptStats->ulETHMAC_MULTIPLE_COLLISION_FRAMES;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulSQETestErrors             = 0;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulDeferredTransmissions     = 0;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulLateCollisions            = ptStats->ulETHMAC_LATE_COLLISIONS;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulExcessiveCollisions       = 0;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulInternalMacTransmitErrors = ptStats->ulETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION
                                                                        + ptStats->ulETHMAC_TX_FATAL_ERROR;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulCarrierSenseErrors        = 0;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulFrameTooLongs             = ptStats->ulETHMAC_FRAME_TOO_LONG_ERRORS;
    ptEthMac->atEthIf[uiIf].tDot3StatsEntry.ulInternalMacReceiveErrors  = ptStats->ulETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE
                                                                        + ptStats->ulETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW
                                                                        + ptStats->ulETHMAC_RX_FATAL_ERROR;

    /* Generic interface counters */
    ptEthMac->atEthIf[uiIf].tIfEntry.ulInOctets                         = 0;
    ptEthMac->atEthIf[uiIf].tIfEntry.ulInErrors                         = ptStats->ulETHMAC_ALIGNMENT_ERRORS
                                                                        + ptStats->ulETHMAC_COLLISION_FRAGMENTS_RECEIVED
                                                                        + ptStats->ulETHMAC_RUNT_FRAMES_RECEIVED
                                                                        + ptStats->ulETHMAC_FRAME_CHECK_SEQUENCE_ERRORS
                                                                        + ptStats->ulETHMAC_FRAME_TOO_LONG_ERRORS;
    ptEthMac->atEthIf[uiIf].tIfEntry.ulInDiscards                       = ptStats->ulETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE
                                                                        + ptStats->ulETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW;
    ptEthMac->atEthIf[uiIf].tIfEntry.ulOutOctets                        = 0;
    ptEthMac->atEthIf[uiIf].tIfEntry.ulOutErrors                        = ptStats->ulETHMAC_LATE_COLLISIONS
                                                                        + ptStats->ulETHMAC_TX_FATAL_ERROR;
    ptEthMac->atEthIf[uiIf].tIfEntry.ulOutDiscards                      = ptStats->ulETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION;
    ptEthMac->atEthIf[uiIf].tIfEntry.ulOutQLen                          = 0;
  }
}

/* one or more frames were received */
uint32_t eth_mac_ind(ETH_T* ptEth, unsigned int uPriority, uint32_t ulMaxFrames)
{
  ETH_MAC_T* ptEthMac = (ETH_MAC_T*)ptEth->pvInst;
  ETHERNET_FRAME_T*     ptFrameBuf;
  void*                 pvHandle;
  uint32_t              uiLen;
  uint32_t              ulFillLevel, ulCnt;
  bool                  fRelease;
  ETH_INFO_T            tInfo, *ptInfo;
#ifndef ETH_MAC_NO_PERF_MEASURE
  unsigned int          uPrio = ePFifo - ETH2PS_PFIFO_IND_CNF_NRT;
  uint64_t              ullTime;
  uint32_t              ulDuration;
#endif

  ptInfo = &tInfo;
  ptInfo->ptEth  = ptEth;
  ptInfo->uiPort = 0;

  /* Get the current fill level */
  EthMac_GetRecvFillLevel(ptEthMac->uiPort, uPriority, &ulFillLevel);

  /* Limit number of processed frames */
  if( ulMaxFrames == 0 )
    ulMaxFrames = ulFillLevel;
  else
    ulMaxFrames = MIN(ulMaxFrames, ulFillLevel);

  /* Retrieve all frames in FIFO */
  for(ulCnt = 0; ulCnt < ulMaxFrames; ++ulCnt)
  {
#ifndef ETH_MAC_NO_PERF_MEASURE
    ullTime = systime_get();
#endif

    /* receive one packet */
    if( ETH_OKAY != EthMac_Recv(ptEthMac->uiPort, &ptFrameBuf, &pvHandle, &uiLen, uPriority) )
    {
      FATAL_ERROR("%s:%d:eth_mac_ind() EthMac_Recv() failed, priority: %d", s_pszModule, __LINE__, uPriority);
      continue;
    }
    INC_CNT(ptEthMac->ulFrameGet)
    fRelease = true;

    if( ( uiLen < 60 ) || ( uiLen > 1518 ) )
    {
      FATAL_ERROR("%s:%d:eth_mac_ind() port %d invalid frame length (%d) handle: %08x ind", s_pszModule, __LINE__, ptEthMac->uiPort, uiLen, (unsigned int)pvHandle);
      continue;
    }

    /* store time stamp */
    ptInfo->tTimestamp.ullSystime   = ptEthMac->ullSystimeBorder * ptFrameBuf->ulTimestampS + ptFrameBuf->ulTimestampNs;
    ptInfo->tTimestamp.ullSystimeUc = ptInfo->tTimestamp.ullSystime;

    /* handle indication */
    if( ETH_MAC_FRAME_FILTER(ptEthMac->uiPort, ptFrameBuf, uiLen, uPriority, false) )
    {
      SG_T atFrameFragments[1];

#ifdef ETH_MAC_UPDATE_PHY_PHASE_OFFSET
      /* update PHY phase offset after first received frame */
      if( ! ptEthMac->atIf[tInfo.uPortNo].fPhyPhaseOffsetValid )
      {
        ptEthMac->atIf[tInfo.uPortNo].bPhyPhaseOffset = 0;
        ptEthMac->atIf[tInfo.uPortNo].fPhyPhaseOffsetValid = true;
      }
#endif

      /* setup list of fragments */
      atFrameFragments[0].pvData = (void*)ptFrameBuf;
      atFrameFragments[0].uiLen  = uiLen;

      /* process frame */
      eth_process_frame(ptInfo, atFrameFragments, uiLen);
    }

#ifndef ETH_MAC_NO_RELEASE_FRAME
    if( fRelease )
    {
      /* put frame buffer back into resource pool */
      EthMac_ReleaseFrame(ptEthMac->uiPort, pvHandle);

      INC_CNT(ptEthMac->ulFrameFree)
    }
#endif

#ifndef ETH_MAC_NO_PERF_MEASURE
    /* latch maximum duration of frame handling */
    ulDuration = systime_get() - ullTime;
    if( fCnf )
    {
      if( ulDuration > ptEthMac->aulMaxDurCnf[uPrio] )
        ptEthMac->aulMaxDurCnf[uPrio] = ulDuration;
    }
    else
    {
      if( ulDuration > ptEthMac->aulMaxDurInd[uPrio] )
        ptEthMac->aulMaxDurInd[uPrio] = ulDuration;
    }
#endif
  }

  return ulMaxFrames;
}

/* one or more frames were received */
uint32_t eth_mac_cnf(ETH_T* ptEth, unsigned int uPriority, uint32_t ulMaxFrames)
{
  ETH_MAC_T* ptEthMac = (ETH_MAC_T*)ptEth->pvInst;
  ETHERNET_FRAME_T*     ptFrameBuf;
  void*                 pvHandle;
  uint32_t              uiLen;
  uint32_t              ulFillLevel, ulCnt;
  bool                  fRelease = true;
  ETH_INFO_T            tInfo;
  void**                ppvCnf;
  ETH_TX_CNF_HANDLER_T* ptCnfHandler;
  ETHERNET_RESULT       eCnfRslt;
#ifndef ETH_MAC_NO_PERF_MEASURE
  unsigned int          uPrio = ePFifo - ETH2PS_PFIFO_IND_CNF_NRT;
  uint64_t              ullTime;
  uint32_t              ulDuration;
#endif

  /* Get the current fill level */
  EthMac_GetSendCnfFillLevel(ptEthMac->uiPort, uPriority, &ulFillLevel);
  
  //sim_message("eth_mac_cnf(), ulFillLevel", disp_data_dez, ulFillLevel);

  /* Limit number of processed frames */
  if( ulMaxFrames == 0 )
    ulMaxFrames = ulFillLevel;
  else
    ulMaxFrames = MIN(ulMaxFrames, ulFillLevel);

  /* Retrieve all frames in FIFO */
  for(ulCnt = 0; ulCnt < ulMaxFrames; ++ulCnt)
  {
#ifndef ETH_MAC_NO_PERF_MEASURE
    ullTime = systime_get();
#endif

    /* receive one packet */
    if( ETH_OKAY != EthMac_GetSendCnf(ptEthMac->uiPort, &ptFrameBuf, &pvHandle, &uiLen, uPriority, &eCnfRslt) )
    {
      FATAL_ERROR("%s:%d:eth_mac_cnf() EthMac_GetSendCnf() failed, priority: %d", s_pszModule, __LINE__, uPriority);
      continue;
    }
    INC_CNT(ptEthMac->ulFrameGet)

    /* store time stamp */
    tInfo.tTimestamp.ullSystime   = ptEthMac->ullSystimeBorder * ptFrameBuf->ulTimestampS + ptFrameBuf->ulTimestampNs;
    tInfo.tTimestamp.ullSystimeUc = tInfo.tTimestamp.ullSystime;

    /* handle confirmation */
    if( ETH_MAC_FRAME_FILTER(ptEthMac->uiPort, ptFrameBuf, uiLen, uPriority, true) )
    {
      ppvCnf = (void*)&ptFrameBuf->abRes[6];

      /* Get the confirmation handler */
      ptCnfHandler = (ETH_TX_CNF_HANDLER_T*)*ppvCnf;

      if( ( ptCnfHandler != NULL ) && ( ptCnfHandler->pfnHandleConfirmation != NULL ) )
      {
        /* Call back application */
        fRelease = ptCnfHandler->pfnHandleConfirmation(ptCnfHandler->pvContext, ( eCnfRslt <= ETH_ERR_TX_SUCCESSFUL_WITH_RETRIES ), &tInfo.tTimestamp, (uint32_t)eCnfRslt);
      }
    }

#ifndef ETH_MAC_NO_RELEASE_FRAME
    if( fRelease )
    {
      /* put frame buffer back into resource pool */
      EthMac_ReleaseFrame(ptEthMac->uiPort, pvHandle);

      INC_CNT(ptEthMac->ulFrameFree)
    }
#endif

#ifndef ETH_MAC_NO_PERF_MEASURE
    /* latch maximum duration of frame handling */
    ulDuration = systime_get() - ullTime;
    if( fCnf )
    {
      if( ulDuration > ptEthMac->aulMaxDurCnf[uPrio] )
        ptEthMac->aulMaxDurCnf[uPrio] = ulDuration;
    }
    else
    {
      if( ulDuration > ptEthMac->aulMaxDurInd[uPrio] )
        ptEthMac->aulMaxDurInd[uPrio] = ulDuration;
    }
#endif
  }

  return ulMaxFrames;
}

/* interrupt service routine */
void eth_mac_isr(unsigned int uiPort)
{
  uint32_t ulIrqSource;
  ETH_MAC_T* ptEthMac = &g_atEthMac[uiPort];
  ETH_T* ptEth = &ptEthMac->tEth;

  /* Get COM interrupts */
  if( ETH_OKAY != EthMac_GetIrq(uiPort, &ulIrqSource) ) {
    return;
  }

  /* Confirm COM interrupts */
  EthMac_ConfirmIrq(uiPort, ulIrqSource);

  /* Check now the different interrupt sources */

  /* we got an high confirmation, i.e. one or more high prior packets were sent on port 0 */
  if( (ulIrqSource & MSK_ETHMAC_IRQ_CON_HI) != 0 ) {
    eth_mac_cnf(ptEth, 1, 0);
  }

  /* we got an high indication, i.e. one or more high prior packets were received on port 0 */
  if( (ulIrqSource & MSK_ETHMAC_IRQ_IND_HI) != 0 ) {
    eth_mac_ind(ptEth, 1, 0);
  }

  /* we got an low confirmation, i.e. one or more low prior packets were sent on port 0 */
  if( (ulIrqSource & MSK_ETHMAC_IRQ_CON_LO) != 0 ) {
    eth_mac_cnf(ptEth, 0, 0);
  }

  /* we got an low indication, i.e. one or more low prior packets were received on port 0 */
  if( (ulIrqSource & MSK_ETHMAC_IRQ_IND_LO) != 0 ) {
    eth_mac_ind(ptEth, 0, 0);
  }

  /* Update Link Mode as fast as possible to make sure that MAC runs in correct mode */
  if( (ulIrqSource & MSK_ETHMAC_IRQ_LINK_CHANGED) != 0 ) {
    eth_mac_update_link_mode(ptEth);
  }

/*

  if( 0 != (ulIrqSource & MSK_ETHMAC_IRQ_COL) ) {
    //...
  }

  if( 0 != (ulIrqSource & MSK_ETHMAC_IRQ_EARLY_RCV) ) {
    //...
  }

  if( 0 != (ulIrqSource & MSK_ETHMAC_IRQ_RX_ERR) ) {
    //...
  }

  if( 0 != (ulIrqSource & MSK_ETHMAC_IRQ_TX_ERR) ) {
    //...
  }
*/
}

