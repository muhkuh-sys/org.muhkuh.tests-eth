/****************************************************************************************
*  Address Resolution Protocol
*
*  Part of the TCP/IP family
****************************************************************************************/

#include "arp.h"
#include "ip.h"

#include <string.h>

#define ARP_TABLE_SIZE 4

typedef struct ARP_ENTRY_Ttag
{
  uint32_t ulIpAdr;
  uint8_t abMacAdr[6];
} ARP_ENTRY_T;

typedef struct ARP_Ttag
{
  unsigned int uiNxt;
  ARP_ENTRY_T atTable[ARP_TABLE_SIZE];
} ARP_T;

static ARP_T s_tArp;

/****************************************************************************************
*  Functions
****************************************************************************************/

/* Processes an ARP packet */
/*lint -e{572} this fixes warnings that come with the hton() macros */
void arp_process_packet(ETH_INFO_T* ptEthInfoIn, SG_T* ptPkt, unsigned int uiTLen)
{
  ARP_PACKET_T* ptArp;
  ARP_PACKET_T  tBuf;
  ETH_INFO_T    tEthInfoOut;
  uint32_t      ulIp = g_tIp.ulIpAddr;
  
  /* check for minimum packet size */
  if( uiTLen < sizeof(ARP_PACKET_T) )
    return;
  if( ptPkt->uiLen < sizeof(ARP_PACKET_T) )
    return;

  /* cast the Ethernet frame to an ARP packet */
  ptArp = (ARP_PACKET_T*)ptPkt->pvData;

  /* is this an ARP request for this IP? */
  if( ptArp->usOpCode != htons(ARP_OPCODE_REQUEST) )
  {
    return; /* Not a request */
  }
  else if( ptArp->usHardwAddrType != htons(ARP_HTYPE_ETHERNET) )
  {
    return; /* No Ethernet hardware address type */
  }
  else if( ptArp->usProtocolType != htons(ETH_TYPE_IP) )
  {
    return; /* no IP type */
  }
  else if( ptArp->usHardwAddrProtocolSize != htons(ARP_HLENPLEN_ETH_IPV4) )
  {
    return; /* no standard length */
  }
  else if( ptArp->ulIpAddrDst != htonl(ulIp) )
  {
    return; /* IP address mismatch */
  }

  /******** ARP REPLY ********/
  memcpy((void*)&tBuf, ptPkt->pvData, sizeof(tBuf));
  ptArp = &tBuf;

  /* set reply opcode */
  ptArp->usOpCode = ntohs(ARP_OPCODE_REPLY);
  /* destination IP is the asking IP */
  ptArp->ulIpAddrDst = ptArp->ulIpAddrSrc;
  /* source IP is ours */
  ptArp->ulIpAddrSrc = htonl(ulIp);
  /* swap MAC addresses of Ethernet header to send packet back */
  memcpy((uint8_t*)ptArp->abEthAddrDst, (uint8_t*)ptArp->abEthAddrSrc, sizeof(ETH_MAC_ADDR_T));
  memcpy((uint8_t*)ptArp->abEthAddrSrc, ptEthInfoIn->ptEth->pbMacAddr, sizeof(ETH_MAC_ADDR_T));

  /******** ETHERNET FRAME ********/
  tEthInfoOut.ptEth        = ptEthInfoIn->ptEth;
  tEthInfoOut.uiPort       = 0; /* Unknown Port */
  tEthInfoOut.ptDstAddr    = ptEthInfoIn->ptSrcAddr;
  tEthInfoOut.ptSrcAddr    = NULL;
  tEthInfoOut.usEtherType  = ETH_TYPE_ARP;
  tEthInfoOut.ulVlanTag    = 0; /* No VLAN Tag */

  /*lint -e(534) ignore return value */
  eth_send(
    &tEthInfoOut,
    (void*)ptArp,
    sizeof(*ptArp),
    ETH_TC_NRT, /* priority */
    0, /* no transmit time stamp */
    NULL /* no confirmation call-back */
  );
}

void arp_add(void* pvMac, uint32_t ulIp)
{
  ARP_ENTRY_T* ptEntry = &s_tArp.atTable[s_tArp.uiNxt];

  memcpy(ptEntry->abMacAdr, pvMac, 6);
  ptEntry->ulIpAdr = ulIp;

  if( ++s_tArp.uiNxt == ARP_TABLE_SIZE )
  {
    s_tArp.uiNxt = 0;
  }
}

void arp_resolve(void* pvMac, uint32_t ulIp)
{
  uint8_t* pbMac = (uint8_t*)pvMac;
  unsigned int uiIdx;

  if( (ulIp & 0xff100000) == 0xe0000000 )
  {
    /* IPv4 Multicast */
    *pbMac++ = 0x01;
    *pbMac++ = 0x00;
    *pbMac++ = 0x5e;
    *pbMac++ = 0x7f & (ulIp >> 16);
    *pbMac++ = 0xff & (ulIp >> 8);
    *pbMac++ = 0xff & (ulIp >> 0);
  }
  else
  {
    for(uiIdx = 0; uiIdx < ARP_TABLE_SIZE; ++uiIdx)
    {
      ARP_ENTRY_T* ptEntry = &s_tArp.atTable[uiIdx];
      if( ptEntry->ulIpAdr == ulIp )
      {
        memcpy(pvMac, ptEntry->abMacAdr, 6);
        return;
      }
    }
    /* no ARP entry, unable to map */
    memset(pvMac, 0, 6);
  }
}
