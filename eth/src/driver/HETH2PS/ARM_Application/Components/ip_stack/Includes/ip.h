#ifndef _IP_H
#define _IP_H

/****************************************************************************************
*  Includes
****************************************************************************************/

#include "eth.h"
#include "ip_stack.h" /* MUTEX_T, MUTEX_LOCK, MUTEX_UNLOCK */


/****************************************************************************************
*  Definitions
****************************************************************************************/

/******** IPv4 ********/

/* IPv4 header */
typedef __PACKED_PRE struct IP_HEADER_Ttag {
  uint8_t  ver_ihl;         /**< Version (4 bits) + Internet header length (4 bits) */
  uint8_t  tos;             /**< Type of service */
  uint16_t tlen;            /**< Total length */
  uint16_t identification;  /**< Identification */
  uint16_t flags_fo;        /**< Flags (3 bits) + Fragment offset (13 bits) */
  uint8_t  ttl;             /**< Time to live */
  uint8_t  proto;           /**< Protocol */
  uint16_t crc;             /**< Header checksum */
  uint32_t saddr;           /**< Source address */
  uint32_t daddr;           /**< Destination address */
} __PACKED_POST IP_HEADER_T;

#define MSK_IP_VER_IHL_VERSION     (0xF0)
#define MSK_IP_VER_IHL_LENGTH      (0x0F)

#define IP_VER_IHL_V4_5DW          (0x45)

#define IP_PROTO_ICMP              (0x01)
#define IP_PROTO_TCP               (0x06)
#define IP_PROTO_UDP               (0x11)

#ifndef IP_ADDR
#define IP_ADDR(a,b,c,d) ((a << 24)|(b << 16)|(c << 8)|d)
#endif
#define IP_ADDR_MULTICAST          (0xE0000000)
#define IP_ADDR_ANY                (0)

#define IS_MULTICAST(ip) ((ip & IP_ADDR_MULTICAST) == IP_ADDR_MULTICAST)
#define IS_ANY(ip)       (ip == IP_ADDR_ANY)

#define IP_PORT_DYN_MIN 49152
#define IP_PORT_DYN_MAX 65535

typedef struct IP_Ttag
{
  uint32_t ulIpAddr;
  uint32_t ulSubnetMask;
  uint32_t ulGatewayAddr;
  MUTEX_T  tMtxCfg;
} IP_T;

/****************************************************************************************
*  Variables
****************************************************************************************/

extern IP_T g_tIp;

/****************************************************************************************
*  Functions
****************************************************************************************/

void ip_init( void );
void ip_config( uint32_t ulIpAddr, uint32_t ulSubnetMask, uint32_t ulGatewayAddr );
void ip_status( uint32_t* pulIpAddr, uint32_t* pulSubnetMask, uint32_t* pulGatewayAddr );
void ip_process_packet(ETH_INFO_T* ptEthInfoIn, SG_T* ptPkt, unsigned int uiTLen);
uint16_t ip_checksum( const uint16_t* pusBuffer, unsigned int uiSize );
uint16_t ip_checksum_inc( uint16_t usSum, uint16_t usAdd );
uint16_t ip_checksum_dec( uint16_t usSum, uint16_t usSub );
int ip_sendto(SG_T* ptSg, unsigned int uiSgLen, unsigned int uiSgHdr, unsigned int uiTlen, uint32_t ulDstAdr, uint32_t ulSrcAdr, unsigned int uiIf, uint64_t* pullHwTs, int ttl);

#endif /* _IP_H */
