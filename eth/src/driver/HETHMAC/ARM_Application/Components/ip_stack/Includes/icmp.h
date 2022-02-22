#ifndef _ICMP_H
#define _ICMP_H

/****************************************************************************************
*  Includes
****************************************************************************************/

#include "eth.h"

/****************************************************************************************
*  Definitions
****************************************************************************************/

/* structure of an ICMP message */
typedef __PACKED_PRE struct ICMP_ECHO_Ttag {
  uint8_t   type;        /**< Message type: 8=echo request, 0=echo reply    */
  uint8_t   code;        /**< Code, 0 for echo messages */
  uint16_t  checksum;    /**< Ones' complement checksum over header and data */
  uint16_t  identifier;  /**< not available in echo messages */
  uint16_t  sequence;    /**< not available in echo messages */
  uint32_t  index;       /**< not available in echo messages */
} __PACKED_POST ICMP_ECHO_T;

#define ICMP_TYPE_ECHO_REPLY                    (0)
#define ICMP_TYPE_ECHO_REQUEST                  (8)


/****************************************************************************************
*  Functions
****************************************************************************************/

void icmp_process_message(ETH_INFO_T* ptFrom, void* pvData, unsigned int uiIpHdrLen, unsigned int uiTotalLen);

#endif /* _ICMP_H */
