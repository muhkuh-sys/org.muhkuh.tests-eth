/****************************************************************************************
*  Internet Control Message Protocol
*
*  Part of the TCP/IP family
*
*  This protocol replies to "ping" requests from other hosts.
****************************************************************************************/

#include "ip_stack.h"
#include "icmp.h"
#include "ip.h"
#include "eth.h"


/****************************************************************************************
*  Functions
****************************************************************************************/

/* Processes an ICMP message
 * The IP header is also passed into the function so it can be re-used for the reply message.
 * The data buffer has to be local for the application, no netX internal memory is allowed.
 */
void icmp_process_message(ETH_INFO_T* ptEthInfoIn, void* pvData, unsigned int uiIpHdrLen, unsigned int uiTotalLen )
{
  IP_HEADER_T* ptIp;
  ICMP_ECHO_T* ptIcmp;
  uint32_t ulIpAddr;
  ETH_INFO_T tEthInfoOut;
  uint8_t* pbData = (uint8_t*)pvData;
  
  /* IP payload is an ICMP packet */
  ptIcmp = (ICMP_ECHO_T*)(pbData + uiIpHdrLen);

  if( ptIcmp->type != ICMP_TYPE_ECHO_REQUEST         /* packet must be an ICMP request */
   || ip_checksum((uint16_t*)ptIcmp, uiTotalLen - uiIpHdrLen) != 0  /* does packet's checksum match our's? */)
  {
    /* ignore packet */
    return;
  }

  /******** ICMP ECHO ********/

  /* set reply opcode */
  ptIcmp->type = ICMP_TYPE_ECHO_REPLY;

  /* calculate ICMP checksum
   * Note: We call checksum_dec() instead of checksum() because modifying the given checksum
   *       of the request is much faster than computing the checksum of the whole packet data. */
  ptIcmp->checksum = ip_checksum_dec(ptIcmp->checksum, ICMP_TYPE_ECHO_REQUEST - ICMP_TYPE_ECHO_REPLY);


  /******** IP PACKET ********/

  /* swap addresses in IP header
   * Note: The IP checksum does not change */
  ptIp        = (IP_HEADER_T*)pbData;
  ulIpAddr    = ptIp->daddr;
  ptIp->daddr = ptIp->saddr;
  ptIp->saddr = ulIpAddr;


  /******** ETHERNET FRAME ********/

  tEthInfoOut.ptEth       = ptEthInfoIn->ptEth;
  tEthInfoOut.uiPort      = 0; /* Unknown Port */
  tEthInfoOut.ptDstAddr   = ptEthInfoIn->ptSrcAddr;
  tEthInfoOut.ptSrcAddr   = NULL;
  tEthInfoOut.usEtherType = ETH_TYPE_IP;
  tEthInfoOut.ulVlanTag   = 0; /* No VLAN Tag */

  eth_send(
    &tEthInfoOut,
    pbData,
    (uint16_t)uiTotalLen,
    ETH_TC_NRT, /* priority */
    0, /* no transmit time stamp */
    NULL /* no confirmation call-back */
  );
}
