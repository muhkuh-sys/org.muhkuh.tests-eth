/****************************************************************************************
*  User Datagram Protocol
*
*  Part of the TCP/IP family
****************************************************************************************/

#include "udp.h"
#include "ip.h"
#include <string.h>

#define UDP_COMBLK_MAX     8
#define UDP_BUF_SIZE     512

#define RINGBUF_LEN(rb)  ((rb->uiWr <  rb->uiRd) ? (sizeof(rb->abBuf) - rb->uiRd + rb->uiWr) : (rb->uiWr - rb->uiRd))
#define RINGBUF_FLEN(rb) ((rb->uiRd <= rb->uiWr) ? (sizeof(rb->abBuf) - rb->uiWr + rb->uiRd) : (rb->uiRd - rb->uiWr))

typedef struct UDP_COMBLK_Ttag
{
  UDP_ADR_T tAdr;
  unsigned int uiIf; /* Interfaces */

  /* ring buffer */
  volatile unsigned int uiWr;
  volatile unsigned int uiRd;
  uint8_t abBuf[UDP_BUF_SIZE];
} UDP_COMBLK_T;

/****************************************************************************************
*  Variables
****************************************************************************************/

static UDP_COMBLK_T s_atUdpcb[UDP_COMBLK_MAX];
static unsigned int s_uiDynPort = IP_PORT_DYN_MIN;

/****************************************************************************************
*  Functions
****************************************************************************************/

void udp_init(void)
{
  int iUdpcb;
  for(iUdpcb = 0; iUdpcb < UDP_COMBLK_MAX; ++iUdpcb)
  {
    s_atUdpcb[iUdpcb].tAdr.uiPort = 0;
  }
  s_uiDynPort = IP_PORT_DYN_MIN;
}

int udp_open(void)
{
  int iUdpcb = 0;
  UDP_COMBLK_T* ptUdpcb;

  while( s_atUdpcb[iUdpcb].tAdr.uiPort != 0 )
  {
    if( ++iUdpcb == UDP_COMBLK_MAX )
    {
      /* no resources */
      return -1;
    }
  }

  ptUdpcb = &s_atUdpcb[iUdpcb];
  ptUdpcb->uiIf = 0; /* not bound to any interface, match all interfaces */
  ptUdpcb->uiWr = 0;
  ptUdpcb->uiRd = 0;
  ptUdpcb->tAdr.ulIpAdr = IP_ADDR_ANY;
  ptUdpcb->tAdr.uiPort  = s_uiDynPort;

  /* assign dynamic port */
  if( ++s_uiDynPort > IP_PORT_DYN_MAX )
  {
    s_uiDynPort = IP_PORT_DYN_MIN;
  }

  return iUdpcb;
}

int udp_bind(int iUdpcb, UDP_ADR_T* ptAdr)
{
  UDP_COMBLK_T* ptUdpcb = &s_atUdpcb[iUdpcb];

  if( ( iUdpcb >= UDP_COMBLK_MAX ) || ( ptUdpcb->tAdr.uiPort == 0 ) )
  {
    /* invalid PCB */
    return -1;
  }

  ptUdpcb->tAdr.ulIpAdr = ptAdr->ulIpAdr;
  ptUdpcb->tAdr.uiPort  = ptAdr->uiPort;

  return 0;
}

static void udp_buf_write(UDP_COMBLK_T* ptUdpcb, const void* pvData, unsigned int uiLen)
{
  int iSiz = UDP_BUF_SIZE - ptUdpcb->uiWr;
  const uint8_t* pbData = (const uint8_t*)pvData;

  if( uiLen >= iSiz )
  {
    memcpy(&ptUdpcb->abBuf[ptUdpcb->uiWr], pbData, iSiz);
    ptUdpcb->uiWr = 0;
    pbData += iSiz;
    uiLen  -= iSiz;
  }

  if( uiLen != 0 )
  {
    memcpy(&ptUdpcb->abBuf[ptUdpcb->uiWr], pbData, uiLen);
    ptUdpcb->uiWr += uiLen;
  }
}

static void udp_buf_read(UDP_COMBLK_T* ptUdpcb, void* pvData, unsigned int uiLen, bool fPeek)
{
  unsigned int uiSiz = UDP_BUF_SIZE - ptUdpcb->uiRd;
  unsigned int uiRd  = ptUdpcb->uiRd;
  uint8_t* pbData = (uint8_t*)pvData;

  if( uiSiz <= uiLen )
  {
    memcpy(pbData, &ptUdpcb->abBuf[uiRd], uiSiz);
    uiRd = 0;
    pbData += uiSiz;
    uiLen  -= uiSiz;
  }

  if( uiLen != 0 )
  {
    memcpy(pbData, &ptUdpcb->abBuf[uiRd], uiLen);
    uiRd += uiLen;
  }

  if( !fPeek )
  {
    ptUdpcb->uiRd = uiRd;
  }
}

static void udp_buf_skip(UDP_COMBLK_T* ptUdpcb, unsigned int uiLen)
{
  unsigned int uiRd = ptUdpcb->uiRd;

  uiRd += uiLen;
  if( uiRd >= sizeof(ptUdpcb->abBuf) )
  {
    uiRd -= sizeof(ptUdpcb->abBuf);
  }
  ptUdpcb->uiRd = uiRd;
}

static void udp_buf_pkt(UDP_COMBLK_T* ptUdpcb, const uint8_t* pbData, UDP_MSGHDR_T* ptMsgHdr)
{
  unsigned int uiLen = ptMsgHdr->uiLen;
  unsigned int uiTotLen = sizeof(UDP_MSGHDR_T) + uiLen;
  unsigned int uiSiz = RINGBUF_FLEN(ptUdpcb);

  if( uiTotLen >= uiSiz )
  {
    /* not enough memory */
    return;
  }

  /* store packet header */
  udp_buf_write(ptUdpcb, ptMsgHdr, sizeof(UDP_MSGHDR_T));

  /* store packet data */
  udp_buf_write(ptUdpcb, pbData, uiLen);
}

int udp_close(int iUdpcb)
{
  UDP_COMBLK_T* ptUdpcb = &s_atUdpcb[iUdpcb];

  if( ( iUdpcb >= UDP_COMBLK_MAX ) || ( ptUdpcb->tAdr.uiPort == 0 ) )
  {
    /* invalid PCB */
    return -1;
  }

  ptUdpcb->tAdr.uiPort = 0;

  return 0;
}

int udp_bind_interface(int iUdpcb, unsigned int uiInterface)
{
  UDP_COMBLK_T* ptUdpcb = &s_atUdpcb[iUdpcb];

  if( ( iUdpcb >= UDP_COMBLK_MAX ) || ( ptUdpcb->tAdr.uiPort == 0 ) )
  {
    /* invalid PCB */
    return -1;
  }

  /* add interface */
  ptUdpcb->uiIf |= 1U << uiInterface;

  return 0;
}

int udp_sendto(int iUdpcb, void* pvBuf, UDP_MSGHDR_T* ptMsgHdr, int ttl, bool fHwTs)
{
  SG_T atMsg[4];
  UDP_HEADER_T tUdp;
  UDP_COMBLK_T* ptUdpcb = &s_atUdpcb[iUdpcb];
  unsigned int uiTlen;

  if( ( iUdpcb >= UDP_COMBLK_MAX ) || ( ptUdpcb->tAdr.uiPort == 0 ) )
  {
    /* invalid PCB */
    return -1;
  }

  atMsg[3].pvData = pvBuf;
  atMsg[3].uiLen  = ptMsgHdr->uiLen;

  /* add UDP header */
  uiTlen = ptMsgHdr->uiLen + sizeof(tUdp);
  tUdp.sport = htons((uint16_t)ptUdpcb->tAdr.uiPort);
  tUdp.dport = htons((uint16_t)ptMsgHdr->tAdr.uiPort);
  tUdp.tlen  = htons((uint16_t)uiTlen);
  tUdp.crc   = 0;

  atMsg[2].pvData = (void*)&tUdp;
  atMsg[2].uiLen  = sizeof(tUdp);

  return ip_sendto(&atMsg[2], 2, 2, uiTlen, ptMsgHdr->tAdr.ulIpAdr, ptUdpcb->tAdr.ulIpAdr, ptUdpcb->uiIf, fHwTs ? &ptMsgHdr->ullTs : NULL, ttl);
}

int udp_recv(int iUdpcb, void* pvBuf, UDP_MSGHDR_T* ptMsgHdr)
{
  UDP_COMBLK_T* ptUdpcb = &s_atUdpcb[iUdpcb];
  unsigned int uiMaxLen = ptMsgHdr->uiLen;
  unsigned int uiPktLen;

  if( ( iUdpcb >= UDP_COMBLK_MAX ) || ( ptUdpcb->tAdr.uiPort == 0 ) )
  {
    /* Invalid PCB */
    return -1;
  }

  if( RINGBUF_LEN(ptUdpcb) < sizeof(UDP_MSGHDR_T) )
  {
    /* no packet available */
    ptMsgHdr->uiLen = 0;
    return 0;
  }

  /* get the data but leave it in the buffer because we might want to read it later */
  udp_buf_read(ptUdpcb, ptMsgHdr, sizeof(UDP_MSGHDR_T), true);
  uiPktLen = ptMsgHdr->uiLen;

  if( RINGBUF_LEN(ptUdpcb) < (uiPktLen + sizeof(UDP_MSGHDR_T)) )
  {
    /* packet incomplete */
    ptMsgHdr->uiLen = 0;
    return 0;
  }

  if( uiPktLen > uiMaxLen )
  {
    /* not enough memory provided */
    ptMsgHdr->uiLen = uiPktLen;
    return -2;
  }

  /* we already have the header */
  udp_buf_skip(ptUdpcb, sizeof(UDP_MSGHDR_T));

  /* read the data */
  udp_buf_read(ptUdpcb, pvBuf, uiPktLen, false);

  return (int)uiPktLen;
}

void udp_process_message(ETH_INFO_T* ptEthInfo, void* pvData, unsigned int uiIpHdrLen, unsigned int uiTotalLen)
{
  uint8_t* pbData = (uint8_t*)pvData;
  IP_HEADER_T* ptIp = (IP_HEADER_T*)pvData;
  UDP_HEADER_T* ptUdp;
  unsigned int uiPktLen;
  unsigned int uiPort;
  int iUdpcb;
  UDP_COMBLK_T* ptUdpcb;
  UDP_MSGHDR_T tMsgHdr;
  unsigned int uiInterface = 1U << ptEthInfo->uiPort;
  uint32_t ulDstAdr = ntohl(ptIp->daddr);

  /* skip IP header */
  uiTotalLen -= uiIpHdrLen;
  pbData     += uiIpHdrLen;
  if( uiTotalLen < sizeof(UDP_HEADER_T) )
  {
    /* incomplete header */
    return;
  }

  ptUdp = (UDP_HEADER_T*)pbData;
  uiPktLen = ntohs(ptUdp->tlen);

  if( uiTotalLen < uiPktLen )
  {
    /* invalid packet length */
    return;
  }

  uiPort = ntohs(ptUdp->dport);

  /* skip UDP header */
  uiPktLen -= sizeof(UDP_HEADER_T);
  pbData   += sizeof(UDP_HEADER_T);

  /* find a matching PCB */
  for(iUdpcb = 0; iUdpcb < UDP_COMBLK_MAX; ++iUdpcb)
  {
    ptUdpcb = &s_atUdpcb[iUdpcb];

    if( ( ptUdpcb->tAdr.uiPort == 0 ) || ( ptUdpcb->tAdr.uiPort != uiPort ) )
    {
      /* destination port mismatch */
      continue;
    }

    if( ( !IS_ANY(ptUdpcb->tAdr.ulIpAdr) ) && ( ptUdpcb->tAdr.ulIpAdr != ulDstAdr ) )
    {
      /* destination address mismatch */
      continue;
    }

    if( ( ptUdpcb->uiIf != 0 ) && ( (ptUdpcb->uiIf & uiInterface) == 0 ) )
    {
      /* interface mismatch */
      continue;
    }

    /* matching PCB found, consume packet */
    tMsgHdr.uiLen = uiPktLen;
    tMsgHdr.tAdr.ulIpAdr = ntohl(ptIp->saddr);
    tMsgHdr.tAdr.uiPort  = ntohs(ptUdp->sport);
    tMsgHdr.ullTs = ptEthInfo->tTimestamp.ullSystime;
    udp_buf_pkt(ptUdpcb, pbData, &tMsgHdr);
    return;
  }
}
