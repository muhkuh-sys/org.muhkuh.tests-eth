/****************************************************************************************
*  Ethernet
*  API
****************************************************************************************/

#ifndef _ETH_H
#define _ETH_H

/****************************************************************************************
*  Includes
****************************************************************************************/
#include "compiler_spec.h"
#include "mib_structs.h"
#include "ip_stack.h"

/****************************************************************************************
*  Definitions
****************************************************************************************/
  
/* MAC address structure */
typedef uint8_t ETH_MAC_ADDR_T[6];

/* Media Access Unit information */
typedef struct ETH_MAU_TYPE_Ttag {
  unsigned int uSpeed;        /**< SPEED (100/10)                */
  unsigned int uIsLinkUp;     /**< LINK state   (!=0 -> Link UP) */
  unsigned int uIsFullDuplex; /**< DUPLEX state (!=0 -> FDX)     */
} ETH_MAU_TYPE_T;


typedef struct ETH_TIMESTAMP_Ttag {
  uint64_t ullSystime;
  uint64_t ullSystimeUc;
} ETH_TIMESTAMP_T;


/******** ETHERNET ********/

/* Ethernet frame header structure */
typedef __PACKED_PRE struct ETH_HDR_Ttag {
  ETH_MAC_ADDR_T    tDstAddr;    /**< Destination MAC address (DA)     */
  ETH_MAC_ADDR_T    tSrcAddr;    /**< Source MAC address (SA)          */
  uint16_t          usType;      /**< Frame length/type (LT)           */
} __PACKED_POST ETH_HDR_T;

/* Ethernet frame header structure VLAN tagged */
typedef __PACKED_PRE struct ETH_HDR_VLAN_Ttag {
  ETH_MAC_ADDR_T    tDstAddr;    /**< Destination MAC address (DA)     */
  ETH_MAC_ADDR_T    tSrcAddr;    /**< Source MAC address (SA)          */
  uint32_t          ulVlanTag;   /**< VLAN tag                         */
  uint16_t          usType;      /**< Frame length/type (LT)           */
} __PACKED_POST ETH_HDR_VLAN_T;

#define ETH_TYPE_IP   (0x0800)
#define ETH_TYPE_ARP  (0x0806)
#define ETH_TYPE_LLDP (0x88cc)
#define ETH_TYPE_PNO  (0x8892)
#define ETH_TYPE_VLAN (0x8100)
#define SIZE_VLAN_TAG (4) /* in bytes */
#define ETH_MIN_FRAME_SIZE 60

#define ETH_VLAN_TAG(tag) ((ETH_TYPE_VLAN << 16) | (tag))

/* Ethernet frame structure */
typedef __PACKED_PRE struct ETH_FRAME_Ttag {
  ETH_HDR_T  tHdr;          /**< Ethernet header                      */
  uint8_t    abData[1504];  /**< Frame data excluding DA, SA, LT, FCS */
} __PACKED_POST ETH_FRAME_T;

/* Ethernet frame structure with VLAN tag */
typedef __PACKED_PRE struct ETH_FRAME_VLAN_Ttag {
  ETH_HDR_VLAN_T  tHdr;     /**< Ethernet header with VLAN tag */
  uint8_t    abData[1500];  /**< Frame data excluding DA, SA, VLAN-tag, LT, FCS */
} __PACKED_POST ETH_FRAME_VLAN_T;


typedef struct ETH_CONFIRMATION_HANDLER_Ttag {
  bool (*pfnHandleConfirmation)(void* pvContext, bool fSuccess, ETH_TIMESTAMP_T* ptTransmissionTime, uint32_t ulHalResult);
  void* pvContext;
} ETH_TX_CNF_HANDLER_T;

typedef enum
{
  ETH_TC_NRT,    /* IEEE 802.1Q */
  ETH_TC_RT,     /* driver specific */
  ETH_TC_NWC_LO, /* IEEE 802.1D low priority, BPDU */
  ETH_TC_NWC_HI  /* IEEE 802.1D high priority, BPDU */
} ETH_TC_E;

typedef struct ETH_IF_Ttag
{
  MIB_IF_ENTRY_T         tIfEntry;
  MIB_DOT3_STATS_ENTRY_T tDot3StatsEntry;
  ETH_MAU_TYPE_T         tMauType;
} ETH_IF_T;

typedef struct ETH_Ttag
{
  int (*fnSend)(void* pvInst, uint32_t ulInterfaces, SG_T* ptSg, unsigned int uiTLen, unsigned int uiPrio, ETH_TX_CNF_HANDLER_T* ptConfirmationHandler);
  void*        pvInst;
  unsigned int uIfCnt;
  ETH_IF_T*    patIf;
  uint8_t*     pbMacAddr;
} ETH_T;

/* Ethernet node information */
typedef struct ETH_INFO_Ttag
{
  ETH_T*            ptEth;
  unsigned int      uiPort;
  ETH_TIMESTAMP_T   tTimestamp;      /**< Indication time stamp, unused for requests and confirmations */
  uint8_t*          ptDstAddr;       /**< Hardware address of the node                 */
  uint8_t*          ptSrcAddr;       /**< Hardware address of the node                 */
  uint16_t          usEtherType;     /**< Frame length/type (LT)                       */
  uint32_t          ulVlanTag;       /**< VLAN tag only valid if it starts with 0x8100 */
} ETH_INFO_T;

void   eth_add(ETH_T* ptEth);
ETH_T* eth_get(unsigned int uiIdx);
ETH_T* eth_find_if(uint32_t ulIpAdr);
int    eth_send(ETH_INFO_T* ptEthInfo, const void* pvData, unsigned int uiLen, ETH_TC_E ePrio, unsigned int uTxTstampBuf, ETH_TX_CNF_HANDLER_T* ptCnfHandler);
int    eth_sendip(SG_T* ptSg, unsigned int uiSgLen, unsigned int uiSgHdr, unsigned int uiTlen, uint32_t ulDstAdr, unsigned int uiIf, uint64_t* pullHwTs);
void   eth_process_frame(ETH_INFO_T* ptInfo, SG_T* ptFragments, unsigned int uiTlen);

#endif /* _ETH_H */
