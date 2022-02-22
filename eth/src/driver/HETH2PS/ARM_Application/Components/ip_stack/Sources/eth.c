/****************************************************************************************
*  Ethernet Driver for the Ethernet 2-Port Switch HAL
*
*  This module implements the Ethernet API defined in "eth.h".
*  It is an adapter between the stack and the HAL.
****************************************************************************************/
#include "eth.h"
#include "arp.h"
#include "ip.h"
#include <string.h>

/****************************************************************************************
*  Definitions
****************************************************************************************/

/* Configure if all non ARP/IP frames shall be echoed with swapped MAC addresses for test purposes */
#ifndef ETH_ECHO_UNKNOWN_FRAMES
#define ETH_ECHO_UNKNOWN_FRAMES 0
#endif

/****************************************************************************************
*  Variables
****************************************************************************************/

ETH_T* s_ptEth;

#if LOG_ENABLE
static const char* s_pszModule = __FILE__;
#endif

/****************************************************************************************
*  Functions
****************************************************************************************/

void eth_add(ETH_T* ptEth)
{
  s_ptEth = ptEth;
}

ETH_T* eth_get(unsigned int uiIdx)
{
  if( uiIdx != 0 )
  {
    return NULL;
  }
  return s_ptEth;
}

int eth_send(ETH_INFO_T* ptEthInfo, const void* pvData, unsigned int uiLen, ETH_TC_E ePrio, unsigned int uTxTstampBuf, ETH_TX_CNF_HANDLER_T* ptCnfHandler)
{
  ETH_T* ptEth = ptEthInfo->ptEth;

  union {
    ETH_HDR_VLAN_T tTagged;
    ETH_HDR_T      tUntagged;
  } tHdr;
  SG_T             atSg[2];
  unsigned int     uiPriority = 1;
  unsigned int     uiHdrLen;

  if( ptEth == NULL )
  {
    ptEth = eth_find_if(0xffffffff);
  }
    
  switch( ePrio )
  {
    case  ETH_TC_NRT:
      /* legacy Ethernet */
      if( ptEthInfo->ulVlanTag != 0 )
      {
        uiPriority = ((ptEthInfo->ulVlanTag >> 13) & 7);
      }
      break;
      
    case ETH_TC_RT:
      uiPriority = 8;
      break;
      
    case ETH_TC_NWC_LO:
      uiPriority = 9;
      break;
      
    case ETH_TC_NWC_HI:
      uiPriority = 10;
      break;
      
    default:
      return 1;
  }

  if( ptEthInfo->ptSrcAddr == NULL )
  {
    /* Source MAC is our own MAC */
    ptEthInfo->ptSrcAddr = ptEth->pbMacAddr;
  }

  memcpy((void*)&tHdr.tUntagged.tDstAddr[0], ptEthInfo->ptDstAddr, sizeof(tHdr.tUntagged.tDstAddr));
  memcpy((void*)&tHdr.tUntagged.tSrcAddr[0], ptEthInfo->ptSrcAddr, sizeof(tHdr.tUntagged.tSrcAddr));

  if( ptEthInfo->ulVlanTag != 0 )
  {
    /* tagged */
    tHdr.tTagged.ulVlanTag = htonl(ptEthInfo->ulVlanTag);
    tHdr.tTagged.usType    = htons(ptEthInfo->usEtherType);
    uiHdrLen = sizeof(tHdr.tTagged);
  }
  else
  {
    /* untagged */
    tHdr.tUntagged.usType    = htons(ptEthInfo->usEtherType);
    uiHdrLen = sizeof(tHdr.tUntagged);
  }

  atSg[0].pvData = &tHdr;
  atSg[0].uiLen  = uiHdrLen;

  atSg[1].pvData = (void*)pvData;
  atSg[1].uiLen  = uiLen;

  return ptEth->fnSend(ptEth->pvInst, ptEthInfo->uiPort, atSg, uiLen + uiHdrLen, uiPriority, ptCnfHandler);
}

typedef struct ETH_CNF_STATE_Ttag
{
  volatile int iCnfCnt;
  uint64_t* pullHwTs;
} ETH_CNF_STATE_T;

static bool eth_sendip_cnf(void* pvContext, bool fSuccess, ETH_TIMESTAMP_T* ptTransmissionTime, uint32_t ulHalResult)
{
  ETH_CNF_STATE_T* ptState = (ETH_CNF_STATE_T*)pvContext;
  *(ptState->pullHwTs) = ptTransmissionTime->ullSystime;
  ptState->iCnfCnt++;
  return true;
}

ETH_T* eth_find_if(uint32_t ulIpAdr)
{
  return s_ptEth;
}

/* Note: This function blocks if pullHwTs is not NULL, do not call in ISR */
int eth_sendip(SG_T* ptSg,
               unsigned int uiSgLen,
               unsigned int uiSgHdr,
               unsigned int uiTlen,
               uint32_t ulDstAdr,
               unsigned int uiIf,
               uint64_t* pullHwTs)
{
  ETH_T* ptEth = eth_find_if(ulDstAdr);
  ETH_HDR_T tHdr, *ptHdr;
  ETH_TX_CNF_HANDLER_T tCnf, *ptCnf;
  ETH_CNF_STATE_T tCnfState;
  int iRet;

  /* do we have room for the Ethernet header? */
  if(uiSgHdr < 1)
  {
    return -1;
  }

  /* add Ethernet header */
  --ptSg;

  ptHdr = &tHdr;
  uiTlen += sizeof(tHdr);

  arp_resolve((void*)&ptHdr->tDstAddr[0], ulDstAdr);
  memcpy((void*)&ptHdr->tSrcAddr[0], ptEth->pbMacAddr, 6);
  ptHdr->usType = htons(ETH_TYPE_IP);

  ptCnf = NULL;
  tCnfState.iCnfCnt = 0;

  if( pullHwTs != NULL )
  {
    if( (uiIf != 1) && (uiIf != 2) )
    {
      /* no multiple confirmations supported */
      return -2;
    }
    ptCnf = &tCnf;
    ptCnf->pfnHandleConfirmation = eth_sendip_cnf;
    tCnfState.pullHwTs = pullHwTs;
    ptCnf->pvContext   = &tCnfState;
  }

  ptSg->pvData = (void*)&tHdr;
  ptSg->uiLen  = sizeof(tHdr);

  iRet = ptEth->fnSend(ptEth->pvInst, uiIf, ptSg, uiTlen, 1, ptCnf);

  if( ( iRet == 0 ) && ( pullHwTs != NULL ) )
  {
    while( tCnfState.iCnfCnt != 1 );
  }

  return iRet;
}

#if ETH_ECHO_UNKNOWN_FRAMES

static bool eth_echo_cnf(void* pvContext, bool fSuccess, ETH_TIMESTAMP_T* ptTransmissionTime, uint32_t ulHalResult)
{
  return true;
}

static ETH_TX_CNF_HANDLER_T s_tEchoCnf = {eth_echo_cnf, NULL};

/* Send frame back to the sender */
static void eth_echo_frame(ETH_INFO_T* ptInfo, SG_T* ptFragments, unsigned int uiTLen)
{
  ETH_T* ptEth = ptInfo->ptEth;
  unsigned int uiPrio = 0;
  uint8_t* pbData = (uint8_t*) ptFragments->pvData;

  if( ptInfo->ulVlanTag != 0 )
  {
    uiPrio = ((ptInfo->ulVlanTag >> 13) & 7);
  }

  /* adjust MAC addresses
   * Note: this is dirty because received frame is being corrupted */
  memcpy(pbData, pbData + 6, 6);
  memcpy(pbData + 6, ptEth->pbMacAddr, 6);

  /* send back frame with confirmation enabled */
  /*lint -e(534) ignore errors during send */
  ptEth->fnSend(ptEth->pvInst, (1UL << ptInfo->uiPort), ptFragments, uiTLen, uiPrio, &s_tEchoCnf);

  /* send back frame with confirmation disabled */
  /*ptEth->fnSend(ptEth->pvInst, (1UL << ptInfo->uiPort), ptFragments, uiTLen, uiPrio, NULL);*/
  
  /* send back frame with confirmation enabled, choose port from learning table */
  /*ptEth->fnSend(ptEth->pvInst, 0, ptFragments, uiTLen, uiPrio, &s_tEchoCnf);*/
}
#endif

/* Process a raw Ethernet frame
 * The frame must be in the netX internal memory.
 */
void eth_process_frame(ETH_INFO_T* ptInfo, SG_T* ptFragments, unsigned int uiTLen)
{
  ETH_HDR_T*   ptHdr;
  bool         fForwarding = true;
  unsigned int uiHdrSize;

#ifndef ETH_DISABLE_CHECKS
  if( ptFragments == NULL )
  {
    FATAL_ERROR("%s:%d:eth_process_frame() NULL pointer ptFragment", s_pszModule, __LINE__);
    return;
  }

  if( uiTLen > sizeof(ETH_FRAME_VLAN_T) )
  {
    FATAL_ERROR("%s:%d:eth_process_frame() fragment data size too big (%d)", s_pszModule, __LINE__, uiTLen);
    return;
  }
#endif

  if( uiTLen < sizeof(ETH_HDR_VLAN_T) )
  {
    FATAL_ERROR("%s:%d:eth_process_frame() first fragment too small (%d)", s_pszModule, __LINE__, uiTLen);
    return;
  }

  /* get Ethernet frame header */
  ptHdr = (ETH_HDR_T*)ptFragments->pvData;
  ptInfo->ptDstAddr   = (uint8_t*)&ptHdr->tDstAddr[0];
  ptInfo->ptSrcAddr   = (uint8_t*)&ptHdr->tSrcAddr[0];
  ptInfo->usEtherType = ntohs(ptHdr->usType);

  /* check for VLAN tag */
  if( ETH_TYPE_VLAN != ptInfo->usEtherType )
  {
    /* No VLAN tag */
    ptInfo->ulVlanTag = 0;
    uiHdrSize         = sizeof(ETH_HDR_T);
  }
  else
  {
    /* VLAN tag present */
    ETH_HDR_VLAN_T* ptFrameVlan = (ETH_HDR_VLAN_T*)ptFragments->pvData;
    ptInfo->ulVlanTag           = ntohl(ptFrameVlan->ulVlanTag);
    ptInfo->usEtherType         = ntohs(ptFrameVlan->usType);
    uiHdrSize                   = sizeof(ETH_HDR_VLAN_T);
  }

  /* pass frame to next layer */
  switch( ptInfo->usEtherType )
  {
  case ETH_TYPE_IP:
    /* process IP packet */
    ptFragments->pvData = (uint8_t*)ptFragments->pvData + uiHdrSize;
    ptFragments->uiLen -= uiHdrSize;
    ip_process_packet(ptInfo, ptFragments, uiTLen - uiHdrSize);
    break;

  case ETH_TYPE_ARP:
    /* process ARP packet */
    ptFragments->pvData = (uint8_t*)ptFragments->pvData + uiHdrSize;
    ptFragments->uiLen -= uiHdrSize;
    arp_process_packet(ptInfo, ptFragments, uiTLen - uiHdrSize);
    break;

  default:
    /* unknown protocol */
#if ETH_ECHO_UNKNOWN_FRAMES
    /* Send frame back to the sender for test purposes */
    eth_echo_frame(ptInfo, ptFragments, uiTLen);
#endif

    INC_CNT(ptInfo->ptEth->patIf[ptInfo->uiPort].tIfEntry.ulInUnknownProtos)
    fForwarding = false;

    break;
  }

  /* increment diagnostic counters */
  if( fForwarding )
  {
    if( (ptHdr->tDstAddr[0] & 1) == 0 )
    {
      /* Unicast */
      INC_CNT(ptInfo->ptEth->patIf[ptInfo->uiPort].tIfEntry.ulInUcastPkts)
    }
    else
    {
      /* broadcast or Multicast */
      INC_CNT(ptInfo->ptEth->patIf[ptInfo->uiPort].tIfEntry.ulInNUcastPkts)
    }
  }
}
