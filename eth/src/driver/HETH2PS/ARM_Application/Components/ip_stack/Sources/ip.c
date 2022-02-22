/****************************************************************************************
*  Internet Protocol
*
*  Part of the TCP/IP family
****************************************************************************************/

#include "ip_stack.h"
#include "ip.h"
#include "arp.h"
#include <string.h>

#if IP_USE_ICMP
#include "icmp.h"
#endif

#if IP_USE_UDP
#include "udp.h"
#endif


/****************************************************************************************
*  Variables
****************************************************************************************/

IP_T g_tIp;


/****************************************************************************************
*  Functions
****************************************************************************************/

/* Initialize IP protocol */
void ip_init( void )
{
  MUTEX_INIT(g_tIp.tMtxCfg)
  ip_config(0, 0, 0);
}

int ip_sendto(SG_T* ptSg,
              unsigned int uiSgLen,
              unsigned int uiSgHdr,
              unsigned int uiLen,
              uint32_t ulDstAdr,
              uint32_t ulSrcAdr,
              unsigned int uiIf,
              uint64_t* pullHwTs,
              int ttl)
{
  IP_HEADER_T tIp, *ptIp;
  unsigned int uiTlen;

  /* do we have room for the IP and Ethernet headers? */
  if(uiSgHdr < 1)
  {
    return -1;
  }

  uiTlen = uiLen + sizeof(IP_HEADER_T);

  if( uiTlen > 1500 )
  {
    return -2;
  }

  /* add IP header */
  --ptSg;
  ++uiSgLen;
  --uiSgHdr;

  if( IS_ANY(ulSrcAdr) || IS_MULTICAST(ulSrcAdr) )
  {
    ulSrcAdr = g_tIp.ulIpAddr;
  }

  ptIp = &tIp;
  memset((void*)ptIp, 0, sizeof(*ptIp));
  ptIp->ver_ihl  = 0x40 + sizeof(*ptIp)/4;
  ptIp->tlen     = htons((uint16_t)uiTlen);
  ptIp->ttl      = (uint8_t)ttl;
  ptIp->proto    = IP_PROTO_UDP;
  ptIp->saddr    = htonl(ulSrcAdr);
  ptIp->daddr    = htonl(ulDstAdr);
  ptIp->crc      = ip_checksum((uint16_t*)ptIp, sizeof(*ptIp));

  ptSg->pvData = (void*)ptIp;
  ptSg->uiLen  = sizeof(*ptIp);

  return eth_sendip(ptSg, uiSgLen, uiSgHdr, uiTlen, ulDstAdr, uiIf, pullHwTs);
}

/* Set IP parameters */
void ip_config( uint32_t ulIpAddr, uint32_t ulSubnetMask, uint32_t ulGatewayAddr )
{
  MUTEX_LOCK(g_tIp.tMtxCfg)
  g_tIp.ulIpAddr      = ulIpAddr;
  g_tIp.ulSubnetMask  = ulSubnetMask;
  g_tIp.ulGatewayAddr = ulGatewayAddr;
  MUTEX_UNLOCK(g_tIp.tMtxCfg)
}

void ip_status( uint32_t* pulIpAddr, uint32_t* pulSubnetMask, uint32_t* pulGatewayAddr )
{
  MUTEX_LOCK(g_tIp.tMtxCfg)
  *pulIpAddr      = g_tIp.ulIpAddr;
  *pulSubnetMask  = g_tIp.ulSubnetMask;
  *pulGatewayAddr = g_tIp.ulGatewayAddr;
  MUTEX_UNLOCK(g_tIp.tMtxCfg)
}

/* Processes an IP packet */
void ip_process_packet(ETH_INFO_T* ptEthInfoIn, SG_T* ptPkt, unsigned int uiTLen)
{
  IP_HEADER_T* ptIp;
  unsigned int uiTotalLen;
  uint32_t ulDstIp, ulSrcIp;
  unsigned int uiHdrLen;

  /* check for minimum packet size */
  if( uiTLen < sizeof(IP_HEADER_T) )
    return;

  if( ptPkt->uiLen < sizeof(IP_HEADER_T) )
    return;

  ptIp = (IP_HEADER_T*)ptPkt->pvData;

  /* get header length */
  uiHdrLen = 4U * (ptIp->ver_ihl & MSK_IP_VER_IHL_LENGTH);
  /* get packet length */
  uiTotalLen = ntohs(ptIp->tlen);
  /* get destination address */
  ulDstIp = ntohl(ptIp->daddr);
  ulSrcIp = ntohl(ptIp->saddr);

  /* check header */
  if( uiTotalLen     <= uiHdrLen     /* Is there any pay-load data? */
   || uiTLen         <  uiTotalLen
   || ip_checksum((uint16_t*)ptIp, uiHdrLen) != 0  /* Does packet's checksum match our's? */
   )
  {
    /* ignore packet */
    return;
  }

  /* Is the packet for us? */
  if( ( g_tIp.ulIpAddr != ulDstIp ) && !IS_MULTICAST(ulDstIp) )
  {
    /* ignore packet */
    return;
  }

  if( !IS_ANY(ulSrcIp) )
  {
    /* remember hardware address */
    arp_add(ptEthInfoIn->ptSrcAddr, ulSrcIp);
  }

  /* pass to upper layer */
  switch(ptIp->proto)
  {
#if IP_USE_ICMP
  case IP_PROTO_ICMP:
    /* handle ICMP message */
    icmp_process_message(ptEthInfoIn, ptPkt->pvData, uiHdrLen, uiTotalLen);
    break;
#endif
    
#if IP_USE_UDP
  case IP_PROTO_UDP:
    /* handle UDP message */
    udp_process_message(ptEthInfoIn, ptPkt->pvData, uiHdrLen, uiTotalLen);
    break;
#endif

  default:
    /* unknown IP packet -> ignore it */
    break;
  }
}

/* Modifies a known checksum for a given data summand
 * This function can be used to recalculate a checksum after a single word was appended to the data
 * or a few header fields have been incremented
 */
uint16_t ip_checksum_inc( uint16_t usSum, uint16_t usAdd )
{
  uint32_t ulChecksum = 0xffff ^ usSum;
  ulChecksum += usAdd;
  ulChecksum  = (ulChecksum >> 16) + (ulChecksum & 0xffff);
  return (uint16_t)(0xffff ^ ulChecksum);
}


/* Modifies a known checksum for a given data subtrahend
 * This function can be used to recalculate a checksum after a single word was cut from the data
 * or a few header fields have been decremented, e. g. the TTL field during an IP hop
 */
uint16_t ip_checksum_dec( uint16_t usSum, uint16_t usSub )
{
  uint32_t ulChecksum = usSum;
  ulChecksum += usSub;
  ulChecksum  = (ulChecksum >> 16) + (ulChecksum & 0xffff);
  return (uint16_t)ulChecksum;
}


/* Calculates the ones' complement of the ones' complement sum of an array of 16 bit values
 * This checksum method is used in several Internet protocols
 * NOTE: The buffer (pusBuffer) must be 16 bit aligned
 */
uint16_t ip_checksum( const uint16_t* pusBuffer, unsigned int uiSize )
{
  uint32_t ulChecksum = 0;

  /* loop over all 16 bit values */
  while( uiSize > 1 )
  {
    ulChecksum += *pusBuffer++;
    uiSize -= sizeof(uint16_t);
  }

  /* is one single byte left? */
  if( uiSize != 0 )
  {
    ulChecksum += *((const uint8_t*)pusBuffer);
  }

  /* convert to 16 bit value and add the overflow in bits 16-31 to the lower half */
  ulChecksum  = (ulChecksum >> 16) + (ulChecksum & 0xffff);
  ulChecksum += (ulChecksum >> 16);

  return (uint16_t)(0xffff ^ ulChecksum);
}
