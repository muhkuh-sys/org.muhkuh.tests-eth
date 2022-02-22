#ifndef _ARP_H
#define _ARP_H

/****************************************************************************************
*  Includes
****************************************************************************************/

#include "eth.h"

/****************************************************************************************
*  Definitions
****************************************************************************************/

/* Structure of data area of an Ethernet ARP packet */
typedef __PACKED_PRE struct ARP_PACKET_Ttag {
  uint16_t usHardwAddrType;         /**< Type of abEthAddrSrc and abEthAddrDst (1 for Ethernet)           */
  uint16_t usProtocolType;          /**< Type of ulIpAddrSrc and ulIpAddrDst (0x0800 for IP)              */
  uint16_t usHardwAddrProtocolSize; /**< 1 Byte HW address size, 1 Byte protocol size (0x0604 for Eth/IP) */
  uint16_t usOpCode;                /**< Opcode defining the message type: 1=request, 2=reply             */
  uint8_t  abEthAddrSrc[6];         /**< HW address of the sender                                         */
  uint32_t ulIpAddrSrc;             /**< IP address of the sender                                         */
  uint8_t  abEthAddrDst[6];         /**< Requested HW address                                             */
  uint32_t ulIpAddrDst;             /**< Requested IP address                                             */
} __PACKED_POST ARP_PACKET_T;

/* ARP hardware address types */
#define ARP_HTYPE_ETHERNET              (0x0001)

/* ARP OP codes */
#define ARP_OPCODE_REQUEST              (0x0001)    /* 0x0001: ARP request */
#define ARP_OPCODE_REPLY                (0x0002)    /* 0x0002: ARP reply */

#define ARP_HLENPLEN_ETH_IPV4           (0x0604)

/* State of ARP cache entries */
#define ARP_ST_EMPTY                    (0)
#define ARP_ST_RESOLVING                (1)
#define ARP_ST_VALID                    (2)
#define ARP_ST_STATIC                   (3)

/* ARP entry flags */
#define ARP_FLAG_NEED                   (0)
#define ARP_FLAG_FOUND                  (1)
#define ARP_FLAG_STATIC                 (255)    /* cannot be removed */

/* Type of newly created entries */
#define ARP_TYPE_DYNAMIC                (FALSE)
#define ARP_TYPE_STATIC                 (TRUE)

/****************************************************************************************
*  Functions
****************************************************************************************/

void arp_process_packet(ETH_INFO_T* ptEthInfoIn, SG_T* ptPkt, unsigned int uiTLen);
void arp_add(void* pvMac, uint32_t ulIp);
void arp_resolve(void* pvDst, uint32_t ulIp);

#endif /* _ARP_H */
