#ifndef _UDP_H
#define _UDP_H

#include "eth.h"
#include "ip_stack.h"

/* UDP header */
typedef __PACKED_PRE struct UDP_HEADER_Ttag {
  uint16_t sport;           /**< Source address */
  uint16_t dport;           /**< Destination address */
  uint16_t tlen;            /**< Total length */
  uint16_t crc;             /**< Header checksum */
} __PACKED_POST UDP_HEADER_T;

#define UDP_HEADER_LEN  8

typedef struct UDP_ADR_Ttag
{
  uint32_t ulIpAdr;
  unsigned int uiPort;
} UDP_ADR_T;

typedef struct UDP_MSGHDR_Ttag
{
  unsigned int uiLen;
  UDP_ADR_T tAdr;
  uint64_t ullTs;
} UDP_MSGHDR_T;


void udp_init(void);
int udp_open(void);
int udp_bind(int iUdpcb, UDP_ADR_T* ptAdr);
int udp_bind_interface(int iUdpcb, unsigned int uiInterface);
int udp_recv(int iUdpcb, void* pvBuf, UDP_MSGHDR_T* ptMsgHdr);
int udp_sendto(int iUdpcb, void* pvBuf, UDP_MSGHDR_T* ptMsgHdr, int ttl, bool fHwTs);
int udp_close(int iUdpcb);

void udp_process_message(ETH_INFO_T* ptEthInfoIn, void* pvData, unsigned int uIpHdrLen, unsigned int uiTotalLen);

#endif /* TB_HIL_ETH_2PS_TSN_TESTS_SOFT_ARM_INCLUDE_UDP_H_ */
