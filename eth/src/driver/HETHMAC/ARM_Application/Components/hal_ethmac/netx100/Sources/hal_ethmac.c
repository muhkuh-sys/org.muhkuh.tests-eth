#include "hal_ethmac.h"
#include "ethmac_xpec_regdef.h"

#define ETHMAC_CONCAT(a,b) a ## b

/* netX100/netX500 specific settings */
#if defined(HAL_ETHMAC_TARGET_NX500)
#include "hal_resources_defines.h"
/* XC includes */
#include "xpec_ethmac0.h"
#include "xpec_ethmac1.h"
#include "xpec_ethmac2.h"
#include "xpec_ethmac3.h"
#include "rpu_ethmac0.h"
#include "rpu_ethmac1.h"
#include "rpu_ethmac2.h"
#include "rpu_ethmac3.h"
#include "tpu_ethmac0.h"
#include "tpu_ethmac1.h"
#include "tpu_ethmac2.h"
#include "tpu_ethmac3.h"
#define NXT(id)                  ETHMAC_CONCAT(NX500_,id)
#define MSK_NXT(id)              ETHMAC_CONCAT(MSK_NX500_,id)
#define ETHMAC_PORTS             4

/* netX50 specific settings */
#elif defined(HAL_ETHMAC_TARGET_NX50)
#include "hal_resources_defines.h"
/* XC includes */
#include "xpec_ethmac0.h"
#include "xpec_ethmac1.h"
#include "rpu_ethmac0.h"
#include "rpu_ethmac1.h"
#include "tpu_ethmac0.h"
#include "tpu_ethmac1.h"
#define NXT(id)                  ETHMAC_CONCAT(NX50_,id)
#define MSK_NXT(id)              ETHMAC_CONCAT(MSK_NX50_,id)
#define ETHMAC_PORTS             2

/* netX5 specific settings */
#elif defined(HAL_ETHMAC_TARGET_NX5)
#include "hal_resources_defines_netx5.h"
/* XC includes */
#include "xpec_ethmac0.h"
#include "xpec_ethmac1.h"
#include "rpu_ethmac0.h"
#include "rpu_ethmac1.h"
#include "tpu_ethmac0.h"
#include "tpu_ethmac1.h"
#define NXT(id)                  ETHMAC_CONCAT(NX5_,id)
#define MSK_NXT(id)              ETHMAC_CONCAT(MSK_NX5_,id)
#define ETHMAC_PORTS             2

#else
#error "NO ETHMAC HAL target defined"
#endif

/*****************************************************************************/
/* !!! You have to define which netX areas the HAL uses !!! */
__USE_XPEC_DRAM
__USE_XPEC_IRQS
__USE_XC_LOADER_FUNC_RESET
__USE_XC_LOADER_FUNC_LOAD
__USE_XC_LOADER_FUNC_START
__USE_INTRAM
__USE_POINTERFIFO
__USE_SYSTIME

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define ETHERNET_MINIMUM_FRAMELENGTH                    60
#define ETHERNET_MAXIMUM_FRAMELENGTH                    1518

#define NUM_FIFO_CHANNELS_PER_UNIT                      8       /**< Number of FIFO units per XC channel */
#define FIFO_ENTRIES                                    64      /**< FIFO depth for each of the 8 FIFOs  */
#define ETH_FRAME_BUF_SIZE                              1560    /**< size of a frame buffer              */
#define INTRAM_SEGMENT_SIZE                             0x8000  /**< size of the internal ram segments   */

#define ETHERNET_FIFO_EMPTY                             0      /**< Empty pointer FIFO               */
#define ETHERNET_FIFO_IND_HI                            1      /**< High priority indication FIFO    */
#define ETHERNET_FIFO_IND_LO                            2      /**< Low priority indication FIFO     */
#define ETHERNET_FIFO_REQ_HI                            3      /**< High priority request FIFO       */
#define ETHERNET_FIFO_REQ_LO                            4      /**< Low priority request FIFO        */
#define ETHERNET_FIFO_CON_HI                            5      /**< High priority confirmation FIFO  */
#define ETHERNET_FIFO_CON_LO                            6      /**< Low priority confirmation FIFO   */

/* confirmation error codes */
#define CONF_ERRCODE_TX_SUCCESSFUL_WITHOUT_RETRIES      0x0    /**< Confirmation: Success on first try   */
#define CONF_ERRCODE_TX_SUCCESSFUL_WITH_RETRIES         0x1    /**< Confirmation: Success after some retries */
#define CONF_ERRCODE_TX_FAILED_LATE_COLLISION           0x8    /**< Confirmation: Error late collision */
#define CONF_ERRCODE_TX_FAILED_LINK_DOWN_DURING_TX      0x9    /**< Confirmation: Error link down      */
#define CONF_ERRCODE_TX_FAILED_EXCESSIVE_COLLISION      0xa    /**< Confirmation: Error collision      */
#define CONF_ERRCODE_TX_FAILED_UTX_UFL_DURING_TX        0xb    /**< Confirmation: Error FIFO overflow  */
#define CONF_ERRCODE_TX_FAILED_FATAL_ERROR              0xc    /**< Confirmation: Error Fatal          */

#define ETHMAC_BROADCAST_MAC        0xffffffffffffULL

/* Maximum number of entries in the group address table */
#ifndef ETHMAC_GROUP_ADDR_NUM_MAX
#define ETHMAC_GROUP_ADDR_NUM_MAX   32
#endif

#ifndef ETHMAC_RCV_ALL_MULTICASTS
  uint64_t s_aullGroupAddr[ETHMAC_PORTS][ETHMAC_GROUP_ADDR_NUM_MAX + 1]; /* +1 for Broadcast address */
#endif

/*****************************************************************************/
/* Pointer to XPEC DRAM                                                      */
/*****************************************************************************/
static ETHMAC_XPEC_DPM* s_aptXpecDRam[ETHMAC_PORTS];

/* Pointer to XC microcodes */

static const unsigned long* const paulxPecCodes[]    =
{
  XcCode_xpec_ethmac0,
#if ETHMAC_PORTS > 1
  XcCode_xpec_ethmac1,
#endif
#if ETHMAC_PORTS > 2
  XcCode_xpec_ethmac2,
  XcCode_xpec_ethmac3
#endif
};

static const unsigned long* const paulxMacRpuCodes[] =
{
  XcCode_rpu_ethmac0,
#if ETHMAC_PORTS > 1
  XcCode_rpu_ethmac1,
#endif
#if ETHMAC_PORTS > 2
  XcCode_rpu_ethmac2,
  XcCode_rpu_ethmac3
#endif
};

static const unsigned long* const paulxMacTpuCodes[] =
{
  XcCode_tpu_ethmac0,
#if ETHMAC_PORTS > 1
  XcCode_tpu_ethmac1,
#endif
#if ETHMAC_PORTS > 2
  XcCode_tpu_ethmac2,
  XcCode_tpu_ethmac3
#endif
};


static const INTRAM_AREA_T const ptFIFO_INTRAM_AREA[] =
{
  {0x4000,0x8000},
#if ETHMAC_PORTS > 1
  {0x8000,0xc000},
#endif
#if ETHMAC_PORTS > 2
  {0x14000,0x18000},
  {0x18000,0x1c000}
#endif
};

/* Initialize FIFO Unit */
static void initFifoUnit( unsigned int uiPort )
{
  uint32_t     ulFifoPtr = 0;
  unsigned int uiFifoStart;
  unsigned int uiFifoEnd;
  unsigned int uiIdx;
  unsigned int uiEmptyPtrCnt;
  uint32_t     ulFifoMsk;

  ulFifoMsk = (1UL << NUM_FIFO_CHANNELS_PER_UNIT) - 1;
  ulFifoMsk = ulFifoMsk << (NUM_FIFO_CHANNELS_PER_UNIT * uiPort);

  /* set reset bit for all pointer FIFOs */
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, NX_READ32(s_ptPFifo->ulPfifo_reset) | ulFifoMsk);

  /* get FIFO start and end number of this port number */
  uiFifoStart = uiPort * NUM_FIFO_CHANNELS_PER_UNIT;
  uiFifoEnd = uiFifoStart + NUM_FIFO_CHANNELS_PER_UNIT;

  for( uiIdx = uiFifoStart; uiIdx < uiFifoEnd; uiIdx++ )
  {
    NX_WRITE32(s_ptPFifo->aulPfifo_border[uiIdx], (uiIdx * FIFO_ENTRIES) + FIFO_ENTRIES - 1);
  }

  /* clear reset bit for all pointer FIFO */
  NX_WRITE32(s_ptPFifo->ulPfifo_reset, NX_READ32(s_ptPFifo->ulPfifo_reset) & ~ulFifoMsk);

  /*** fill empty pointer FIFO ***/

  /* first DWORD in segment 0 is hard wired + IRQ vectors, so it cannot be used */
  uiEmptyPtrCnt = (INTRAM_SEGMENT_SIZE / ETH_FRAME_BUF_SIZE) - 1;

  /* each port has it's own internal ram bank */
  ulFifoPtr |= (uiPort << SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM);

  unsigned long ulFIFO_Addr;

  /* fill the empty pointer FIFO */
  for( uiIdx = 0; uiIdx <= uiEmptyPtrCnt; uiIdx++ )
  {
    ulFIFO_Addr = uiIdx * ETH_FRAME_BUF_SIZE + uiPort * INTRAM_SEGMENT_SIZE;
    if ((ulFIFO_Addr >= ptFIFO_INTRAM_AREA[uiPort].ulStart) && ((ulFIFO_Addr + ETH_FRAME_BUF_SIZE) < ptFIFO_INTRAM_AREA[uiPort].ulEnd))
    {
	ulFifoPtr &= ~MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
	ulFifoPtr |= (uiIdx << SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM);
	NX_WRITE32(s_ptPFifo->aulPfifo[uiFifoStart + ETHERNET_FIFO_EMPTY], ulFifoPtr);
    }
  }
}

/*****************************************************************************/
/*! Get MAC Address
* \description
*   This function gets the MAC address of the according Ethernet MAC.
* \class
*   Status Service Class
* \params
*   uiPort               [in]  XC port number
*   eType                [in]  Defines which MAC address shall be read
*   ptMacAddr            [out] Pointer to MAC address buffer
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_GetMacAddr( unsigned int uiPort,
                                   ETH_MAC_ADDRESS_TYPE_E eType,
                                   ETHERNET_MAC_ADDR_T* ptMacAddr )
{
  uint32_t ulTempLo, ulTempHi;

#ifndef __ETHMAC_DISABLE_CHECKS__
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  switch(eType)
  {
  case ETH_MAC_ADDRESS_CHASSIS:
    ulTempLo = NX_READ32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERFACE_MAC_ADDRESS_LO);
    ulTempHi = NX_READ32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERFACE_MAC_ADDRESS_HI);
    break;
  case ETH_MAC_ADDRESS_2ND_CHASSIS:
    ulTempLo = NX_READ32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO);
    ulTempHi = NX_READ32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI);
    break;
  default:
    return ETH_ERR_INVALID_PARAMETER;
  }

  (*ptMacAddr)[0] = (uint8_t)ulTempLo;
  (*ptMacAddr)[1] = (uint8_t)(ulTempLo >> 8);
  (*ptMacAddr)[2] = (uint8_t)(ulTempLo >> 16);
  (*ptMacAddr)[3] = (uint8_t)(ulTempLo >> 24);
  (*ptMacAddr)[4] = (uint8_t)ulTempHi;
  (*ptMacAddr)[5] = (uint8_t)(ulTempHi >> 8);

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Set MAC Address
* \description
*   Sets a MAC address for the according XC port.
*   Note: The Chassis MAC addresses shall be set before the switch is started.
* \class
*   Control Service Class
* \params
*   uiPort       [in] XC port number
*   eType        [in] Defines which MAC address shall be configured
*   tMacAddr     [in] MAC address value
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_SetMacAddr( unsigned int uiPort,
                                   ETH_MAC_ADDRESS_TYPE_E eType,
                                   const ETHERNET_MAC_ADDR_T tMacAddr )
{
  uint32_t ulMacHi, ulMacLo;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  ulMacHi = (uint32_t)(tMacAddr[5] << 8)  | (uint32_t)tMacAddr[4];
  ulMacLo = (uint32_t)(tMacAddr[3] << 24) | (uint32_t)(tMacAddr[2]  << 16) | (uint32_t)(tMacAddr[1]  << 8) | (uint32_t)tMacAddr[0];

  switch(eType)
  {
  case ETH_MAC_ADDRESS_CHASSIS:
    NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERFACE_MAC_ADDRESS_HI,  ulMacHi);
    NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERFACE_MAC_ADDRESS_LO,  ulMacLo);
    break;

  case ETH_MAC_ADDRESS_2ND_CHASSIS:
    NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_2ND_INTERFACE_MAC_ADDRESS_HI,  ulMacHi);
    NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_2ND_INTERFACE_MAC_ADDRESS_LO,  ulMacLo);
    break;

  default:
    return ETH_ERR_INVALID_PARAMETER;
  }

  return ETH_OKAY;
}

#ifndef ETHMAC_RCV_ALL_MULTICASTS
/*****************************************************************************/
/*! Add Group Address
* \description
*   Add the given Multicast group address to the address recognition filter.
* \class
*   Control Service Class
* \params
*   uiPort       [in] XC port number
*   tMacAddr     [in] Multicast MAC address value
* \return
*   ETH_OKAY
*   ETH_ERR_OUT_OF_MEMORY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_AddGroupAddr( unsigned int uiPort,
                                     const ETHERNET_MAC_ADDR_T tMacAddr)
{
  uint64_t ullEntry = 0;
  unsigned int uIdx;
  bool fAdded = false;
  unsigned int uiOfs;
  uint32_t ulMsk;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check if Multicast address is given */
  if( (tMacAddr[0] & 1) == 0 )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  /* get group address table entry */
  for(uIdx = 0; uIdx < 6; ++uIdx)
    ullEntry = (ullEntry << 8) | tMacAddr[uIdx];

  /* get offset and mask for XC hash table */
  uiOfs = tMacAddr[5] >> 1;
  ulMsk = 1UL << (((tMacAddr[5] & 1U) << 4) | (tMacAddr[4] & 0xfU));

  /* iterate over all entries */
  for(uIdx = 0; uIdx <= ETHMAC_GROUP_ADDR_NUM_MAX; ++uIdx)
  {
    if( !fAdded )
    {
      if( s_aullGroupAddr[uiPort][uIdx] == ullEntry )
        fAdded = true; /* entry already exists */
      else if( s_aullGroupAddr[uiPort][uIdx] == 0 )
      {
        /* free slot, add entry */
        s_aullGroupAddr[uiPort][uIdx] = ullEntry;
        NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_MULTICAST_FILTER_TABLE.aulDataField[uiOfs], NX_READ32(s_aptXpecDRam[uiPort]->tETHMAC_MULTICAST_FILTER_TABLE.aulDataField[uiOfs]) | ulMsk);
        fAdded = true;
      }
    }
    else
    {
      if( s_aullGroupAddr[uiPort][uIdx] == ullEntry )
        s_aullGroupAddr[uiPort][uIdx] = 0; /* remove duplicate entries */
    }
  }

  return fAdded ? ETH_OKAY : ETH_ERR_OUT_OF_MEMORY;
}


/*****************************************************************************/
/*! Delete Group Address
* \description
*   Delete the given Multicast group address from the address recognition.
* \class
*   Control Service Class
* \params
*   uiPort       [in] XC port number
*   tMacAddr     [in] Multicast MAC address value
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER
*   ETHMAC_ERR_INVAL_STATE                                                   */
/*****************************************************************************/
ETHERNET_RESULT EthMac_DeleteGroupAddr( unsigned int uiPort,
                                        const ETHERNET_MAC_ADDR_T tMacAddr)
{
  uint64_t ullEntry = 0;
  unsigned int uIdx;
  bool fClear = true;
  unsigned int uiOfs;
  uint32_t ulMsk;
  ETHERNET_RESULT eRslt = ETH_ERR_INVALID_PARAMETER;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check if Multicast filtering is enabled */
  if( s_aullGroupAddr[uiPort][0] == 0 )
    return ETH_ERR_INVAL_STATE;
#endif

  /* get group address table entry */
  for(uIdx = 0; uIdx < 6; ++uIdx)
    ullEntry = (ullEntry << 8) | tMacAddr[uIdx];

  /* get offset and mask for XC hash table */
  uiOfs = tMacAddr[5] >> 1;
  ulMsk = 1UL << (((tMacAddr[5] & 1U) << 4) | (tMacAddr[4] & 0xfU));

  /* iterate over all entries */
  for(uIdx = 0; uIdx <= ETHMAC_GROUP_ADDR_NUM_MAX; ++uIdx)
  {
    if( s_aullGroupAddr[uiPort][uIdx] == ullEntry )
    {
      /* entry found, remove it */
      s_aullGroupAddr[uiPort][uIdx] = 0;
      eRslt = ETH_OKAY;
    }
    else if( (s_aullGroupAddr[uiPort][uIdx] & 0xfff) == (ullEntry & 0xfff) )
    {
      /* entry with same hash value present: do not clear bit in XC table */
      fClear = false;
    }
  }

  if( fClear )
  {
    NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_MULTICAST_FILTER_TABLE.aulDataField[uiOfs], NX_READ32(s_aptXpecDRam[uiPort]->tETHMAC_MULTICAST_FILTER_TABLE.aulDataField[uiOfs]) & ~ulMsk);
  }

  return eRslt;
}
#endif

/*****************************************************************************/
/*! Set Traffic Classes
* \description
*   This function sets the VLAN Tag priority classification between the two traffic classes.\n
*   TC=0: High: VLAN tag priority 7..0  Low: Non-tagged frame\n
*   TC=1: High: VLAN tag priority 7..1  Low: VLAN tag priority 0,    non-tagged\n
*   TC=2: High: VLAN tag priority 7..2  Low: VLAN tag priority 1..0, non-tagged\n
*   TC=3: High: VLAN tag priority 7..3  Low: VLAN tag priority 2..0, non-tagged\n
*   TC=4: High: VLAN tag priority 7..4  Low: VLAN tag priority 3..0, non-tagged\n
*   TC=5: High: VLAN tag priority 7..5  Low: VLAN tag priority 4..0, non-tagged\n
*   TC=6: High: VLAN tag priority 7..6  Low: VLAN tag priority 5..0, non-tagged\n
*   TC=7: High: VLAN tag priority 7     Low: VLAN tag priority 6..0, non-tagged\n
*   TC=8: High: -                       Low: VLAN tag priority 7..0, non-tagged
* \class
*   Control Service Class
* \params
*   uiPort     [in] XC port number
*   uClass     [in] Traffic Class Arrangement
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_SetTrafficClassArrangement( unsigned int uiPort,
                                                   unsigned int uClass )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;

  /* check Traffic class */
  if( uClass > 8 )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_TRAFFIC_CLASS_ARRANGEMENT, uClass );

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Enable/Disable Promiscuous Mode
* \description
*   This function enables/disables promiscuous mode at the Ethernet MAC.
*   When promiscuous mode is enabled all error-free received
*   Ethernet frames are transferred into the according indication FIFO.
*   Otherwise only all error-free received broadcast Ethernet frames,
*   not filtered MultiCast Ethernet frames and UniCast Ethernet frames
*   that match the local MAC address are transferred into the according indication FIFO.
* \class
*   Control Service Class
* \params
*   uiPort  [in] XC port number
*   uEnable [in] 1/0: enables/disables promiscuous mode
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_ModePromisc( unsigned int uiPort,
                                    unsigned int uEnable )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  if( uEnable ) {
    NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_MONITORING_MODE, MSK_ETHMAC_HELP_RX_FRWD2LOCAL );
  } else {
    NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_MONITORING_MODE, 0 );
  }

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Get empty Ethernet Frame Block
* \description
*   This function gets an element from the empty FIFO.
* \class
*   Transmission Service Class
* \params
*   uiPort   [in]  XC port number
*   pptFrame [out] Pointer to pointer to Ethernet frame
*   phFrame  [out] Pointer to handle to Ethernet frame
* \return
*   ETH_OKAY
*   ETH_ERR_FIFO_EMPTY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_GetFrame( unsigned int uiPort,
                                 ETHERNET_FRAME_T** pptFrame,
                                 void** phFrame )
{
  uint32_t     ulFifoPtr = 0;
  unsigned int uiRamSegment;
  unsigned int uiFrameIdx;
  unsigned int uiFifo;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  /* get FIFO fill level and check if there is at least one element */
  uiFifo = NUM_FIFO_CHANNELS_PER_UNIT * uiPort + ETHERNET_FIFO_EMPTY;

  /* keep at least one pointer for the XC level (two parties share this empty pointer FIFO) */
  if( NX_READ32(s_ptPFifo->aulPfifo_fill_level[uiFifo]) < 2)
    return ETH_ERR_FIFO_EMPTY;

  /* retrieve the FIFO element */
  ulFifoPtr = NX_READ32(s_ptPFifo->aulPfifo[uiFifo]);

  /* extract RAM bank and frame number */
  uiFrameIdx   = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM) >> SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
  uiRamSegment = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM) >> SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM;

  /* set result */
  *pptFrame = (ETHERNET_FRAME_T*)(s_pulIntRamStart[uiRamSegment] + (ETH_FRAME_BUF_SIZE * uiFrameIdx / sizeof(uint32_t)));
  *phFrame  = (void*)ulFifoPtr;

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Get Confirmation of Transmission Request
* \description
*   This function retrieves a confirmation of the according confirmation FIFO of the Ethernet MAC.\n
*   Note: The IEEE 1588 time stamp is to be found at offset 1536 within each Ethernet-Frame Buffer.
* \class
*   Transmission Service Class
* \params
*   uiPort        [in]  XC port number
*   pptFrame      [out] Pointer to Ethernet frame
*   phFrame       [out] Pointer to handle to Ethernet frame
*   pulLength     [out] Pointer to Ethernet frame length of processed request
*   uHighPriority [in]  Confirmation priority selector
*   peResult      [out] Pointer to result code
* \return
*   ETH_OKAY
*   ETH_ERR_TX_SUCCESSFUL_WITH_RETRIES
*   ETH_ERR_TX_FAILED_LATE_COLLISION
*   ETH_ERR_TX_FAILED_LINK_DOWN_DURING_TX
*   ETH_ERR_TX_FAILED_EXCESSIVE_COLLISION
*   ETH_ERR_TX_FAILED_UTX_UFL_DURING_TX
*   ETH_ERR_TX_FAILED_FATAL_ERROR
*   ETH_ERR_FIFO_EMPTY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_GetSendCnf( unsigned int uiPort,
                                   ETHERNET_FRAME_T** pptFrame,
                                   void** phFrame,
                                   uint32_t* pulLength,
                                   unsigned int uHighPriority,
                                   ETHERNET_RESULT* peResult )
{
  unsigned int    uiFrameIdx;
  ETHERNET_RESULT eResult;
  unsigned int    uiRamSegment;
  uint32_t        ulFillLevel;
  unsigned int    uiFifo;
  uint32_t        ulFifoPtr = 0;
  uint32_t        ulVal;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  uiFifo  = uHighPriority ? ETHERNET_FIFO_CON_HI : ETHERNET_FIFO_CON_LO;
  uiFifo += NUM_FIFO_CHANNELS_PER_UNIT * uiPort;

  eResult = EthMac_GetSendCnfFillLevel(uiPort, uHighPriority, &ulFillLevel);

  if( eResult != ETH_OKAY )
    return eResult;

  if( ulFillLevel == 0 )
    return ETH_ERR_FIFO_EMPTY;

  /* retrieve the stored FIFO element */
  ulFifoPtr = NX_READ32(s_ptPFifo->aulPfifo[uiFifo]);

  /* extract RAM bank and frame number */
  uiFrameIdx   = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM) >> SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
  uiRamSegment = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM) >> SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM;

  /* set result */
  *pptFrame = (ETHERNET_FRAME_T*)(s_pulIntRamStart[uiRamSegment] + ((uiFrameIdx * ETH_FRAME_BUF_SIZE) / sizeof(uint32_t)));
  *phFrame  = (void*)ulFifoPtr;

  /* retrieve the length from the FIFO element */
  *pulLength = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN) >> SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN;

  /* decode the error code into our own version of it */
  ulVal = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_ERROR_CODE) >> SRT_ETHMAC_FIFO_ELEMENT_ERROR_CODE;

  switch(ulVal)
  {
    case CONF_ERRCODE_TX_SUCCESSFUL_WITHOUT_RETRIES:
      eResult = ETH_OKAY;
      break;
    case CONF_ERRCODE_TX_SUCCESSFUL_WITH_RETRIES:
      eResult = ETH_ERR_TX_SUCCESSFUL_WITH_RETRIES;
      break;
    case CONF_ERRCODE_TX_FAILED_LATE_COLLISION:
      eResult = ETH_ERR_TX_FAILED_LATE_COLLISION;
      break;
    case CONF_ERRCODE_TX_FAILED_LINK_DOWN_DURING_TX:
      eResult = ETH_ERR_TX_FAILED_LINK_DOWN_DURING_TX;
      break;
    case CONF_ERRCODE_TX_FAILED_EXCESSIVE_COLLISION:
      eResult = ETH_ERR_TX_FAILED_EXCESSIVE_COLLISION;
      break;
    case CONF_ERRCODE_TX_FAILED_UTX_UFL_DURING_TX:
      eResult = ETH_ERR_TX_FAILED_UTX_UFL_DURING_TX;
      break;
    case CONF_ERRCODE_TX_FAILED_FATAL_ERROR:
      eResult = ETH_ERR_TX_FAILED_FATAL_ERROR;
      break;
  }

  *peResult = eResult;

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Get Fill Level Confirmation FIFO
* \description
*   This function gets the fill level of the according confirmation FIFO.
* \class
*   Transmission Service Class
* \params
*   uiPort          [in]  XC port number
*   uHighPriority   [in]  Confirmation priority selector (1/0: high/low priority)
*   pulCnfFillLevel [out] Pointer to confirmation FIFO fill level
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_GetSendCnfFillLevel( unsigned int uiPort,
                                            unsigned int uHighPriority,
                                            uint32_t *pulCnfFillLevel )
{
  unsigned int uiFifo;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  /* select FIFO based on priority selector */
  uiFifo  = uHighPriority ? ETHERNET_FIFO_CON_HI : ETHERNET_FIFO_CON_LO;
  uiFifo += NUM_FIFO_CHANNELS_PER_UNIT * uiPort;

  /* retrieve the current fill level from the selected FIFO */
  *pulCnfFillLevel = NX_READ32(s_ptPFifo->aulPfifo_fill_level[uiFifo]);

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Send Ethernet Frame with Confirmation
* \description
*   This function initiates a transmission request.
*   After the processed transmission request the Host will get a confirmation.
* \class
*   Transmission Service Class
* \params
*   uiPort        [in]  XC port number
*   hFrame        [in]  Handle to Ethernet frame
*   ulLength      [in]  Ethernet frame length
*   uHighPriority [in]  Request priority selector (1/0: high/low priority)
* \return
*   ETH_OKAY
*   ETH_ERR_INVAL_FRAME
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_Send( unsigned int uiPort,
                             void* hFrame,
                             uint32_t ulLength,
                             unsigned int uHighPriority )
{
  uint32_t ulFifoPtr = 0;
  unsigned int uiFifo;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;

  if( (ulLength < ETHERNET_MINIMUM_FRAMELENGTH) || (ulLength > ETHERNET_MAXIMUM_FRAMELENGTH) )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  uiFifo  = uHighPriority ? ETHERNET_FIFO_REQ_HI : ETHERNET_FIFO_REQ_LO;
  uiFifo += NUM_FIFO_CHANNELS_PER_UNIT * uiPort;

  /* create FIFO element */
  ulFifoPtr = ( (uint32_t)hFrame ) & ( MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM
                                      |MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM );

  ulFifoPtr &= ~(MSK_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON | MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN);
  ulFifoPtr |= (ulLength << SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN);

  /* request transmission by writing into according request FIFO */
  NX_WRITE32(s_ptPFifo->aulPfifo[uiFifo], ulFifoPtr);

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Send Ethernet Frame without Confirmation
* \description
*   This function initiates a transmission request and suppresses confirmation.
*   After processing the frame buffer will be released automatically by xPEC.
* \class
*   Transmission Service Class
* \params
*   uiPort        [in] XC port number
*   hFrame        [in] Handle to Ethernet frame
*   ulLength      [in] Ethernet frame length
*   uHighPriority [in] Request priority selector (1/0: high/low priority)
* \return
*   ETH_OKAY
*   ETH_ERR_INVAL_FRAME
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_SendWithoutCnf( unsigned int uiPort,
                                       void* hFrame,
                                       uint32_t ulLength,
                                       unsigned int uHighPriority )
{
  uint32_t ulFifoPtr = 0;
  unsigned int uiFifo;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;

  if( (ulLength < ETHERNET_MINIMUM_FRAMELENGTH) || (ulLength > ETHERNET_MAXIMUM_FRAMELENGTH) )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  uiFifo  = uHighPriority ? ETHERNET_FIFO_REQ_HI : ETHERNET_FIFO_REQ_LO;
  uiFifo += NUM_FIFO_CHANNELS_PER_UNIT * uiPort;

  /* create FIFO element */
  ulFifoPtr = ( (uint32_t)hFrame ) & ( MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM
                                      |MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM );

  ulFifoPtr |= MSK_ETHMAC_FIFO_ELEMENT_SUPPRESS_CON;
  ulFifoPtr &= ~MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN;
  ulFifoPtr |= (ulLength << SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN);

  /* request transmission by writing into according request FIFO */
  NX_WRITE32(s_ptPFifo->aulPfifo[uiFifo], ulFifoPtr);

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Release Ethernet Frame Block
* \description
*   This function puts an Ethernet frame block back into empty pointer FIFO of the Ethernet MAC.
* \class
*   Reception Service Class
* \params
*   uiPort      [in] XC port number
*   hFrame      [in] Handle to Ethernet frame
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_ReleaseFrame( unsigned int uiPort,
                                     void* hFrame )
{
  uint32_t ulFifoPtr = 0;
  unsigned int uiFifo;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  uiFifo = ETHERNET_FIFO_EMPTY;
  uiFifo += NUM_FIFO_CHANNELS_PER_UNIT * uiPort;

  /* clear all fields except buffer number and segment number in FIFO element */
  ulFifoPtr = ((uint32_t)hFrame) & ( MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM
                                    |MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM );

  /* return pointer in empty FIFO */
  NX_WRITE32(s_ptPFifo->aulPfifo[uiFifo], ulFifoPtr);

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Get Diagnostic Counters
* \description
*   This function gets the diagnostic counters of the according Ethernet MAC.
* \class
*   Status Service Class
* \params
*   uiPort               [in]  XC port number
*   ptCounters           [out] Pointer to returned counter values
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_GetCounters( unsigned int uiPort,
                                    ETHMAC_COUNTERS_T* ptCounters )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  ptCounters->ulETHMAC_OUT_FRAMES_OKAY                   = 0; /* not supported */
  ptCounters->ulETHMAC_OUT_OCTETS                        = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_OUT_OCTETS );
  ptCounters->ulETHMAC_SINGLE_COLLISION_FRAMES           = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_SINGLE_COLLISION_FRAMES );
  ptCounters->ulETHMAC_MULTIPLE_COLLISION_FRAMES         = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_MULTIPLE_COLLISION_FRAMES );
  ptCounters->ulETHMAC_LATE_COLLISIONS                   = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_LATE_COLLISIONS );
  ptCounters->ulETHMAC_LINK_DOWN_DURING_TRANSMISSION     = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_LINK_DOWN_DURING_TRANSMISSION );
  ptCounters->ulETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION );
  ptCounters->ulETHMAC_IN_FRAMES_OKAY                    = 0; /* not supported */
  ptCounters->ulETHMAC_IN_OCTETS                         = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_IN_OCTETS );
  ptCounters->ulETHMAC_FRAME_CHECK_SEQUENCE_ERRORS       = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_FRAME_CHECK_SEQUENCE_ERRORS );
  ptCounters->ulETHMAC_ALIGNMENT_ERRORS                  = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_ALIGNMENT_ERRORS );
  ptCounters->ulETHMAC_FRAME_TOO_LONG_ERRORS             = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_FRAME_TOO_LONG_ERRORS );
  ptCounters->ulETHMAC_RUNT_FRAMES_RECEIVED              = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_RUNT_FRAMES_RECEIVED );
  ptCounters->ulETHMAC_COLLISION_FRAGMENTS_RECEIVED      = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_COLLISION_FRAGMENTS_RECEIVED );
  ptCounters->ulETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE   = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE );
  ptCounters->ulETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW   = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW );
  ptCounters->ulETHMAC_TX_FATAL_ERROR                    = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_TX_FATAL_ERROR );
  ptCounters->ulETHMAC_RX_FATAL_ERROR                    = NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_STATUS_AREA_BASE.ulETHMAC_RX_FATAL_ERROR );

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Get Ethernet Frame Indication
* \description
*   This function retrieves an indication from the
*   according indication element at the Ethernet MAC.\n
*   Note: The IEEE 1588 time stamp is to be found at offset 1536 within each Ethernet-Frame Buffer.
* \class
*   Reception Service Class
* \params
*   uiPort        [in]  XC port number
*   pptFrame      [out] Pointer to pointer to Ethernet frame
*   phFrame       [out] Pointer to handle to Ethernet frame
*   pulLength     [out] Pointer to Ethernet frame length of indication
*   uHighPriority [in]  Indication priority selector (1/0: high/low priority)
* \return
*   ETH_OKAY
*   ETH_ERR_FIFO_EMPTY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_Recv( unsigned int uiPort,
                             ETHERNET_FRAME_T** pptFrame,
                             void** phFrame,
                             uint32_t* pulLength,
                             unsigned int uHighPriority )
{
  ETHERNET_RESULT tResult;
  uint32_t ulFillLevel;
  uint32_t ulOffset = 0;
  uint32_t ulFifoPtr = 0;
  uint32_t ulVal;
  unsigned int uiFifo;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  uiFifo  = uHighPriority ? ETHERNET_FIFO_IND_HI : ETHERNET_FIFO_IND_LO;
  uiFifo += NUM_FIFO_CHANNELS_PER_UNIT * uiPort;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* get current fill level from FIFO and check if there is at least one element */
  tResult = EthMac_GetRecvFillLevel( uiPort, uHighPriority, &ulFillLevel );

  if( tResult != ETH_OKAY )
    return tResult;

  if( 0 == ulFillLevel )
    return ETH_ERR_FIFO_EMPTY;
#endif

  /* retrieve the FIFO element from the FIFO */
  ulFifoPtr = NX_READ32(s_ptPFifo->aulPfifo[uiFifo]);

  ulVal = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM) >> SRT_ETHMAC_FIFO_ELEMENT_FRAME_BUF_NUM;
  ulOffset = ulVal * ETH_FRAME_BUF_SIZE;

  /* set result */
  ulVal = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM) >> SRT_ETHMAC_FIFO_ELEMENT_INT_RAM_SEGMENT_NUM;

  *pptFrame = (ETHERNET_FRAME_T*)(s_pulIntRamStart[ulVal] + (ulOffset / sizeof(uint32_t)));
  *pulLength = (ulFifoPtr & MSK_ETHMAC_FIFO_ELEMENT_FRAME_LEN) >> SRT_ETHMAC_FIFO_ELEMENT_FRAME_LEN;
  *phFrame = (void*)ulFifoPtr;

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Get Fill Level Indication FIFO
* \description
*   This function retrieves the fill level of the according indication FIFO.
* \class
*   Reception Service Class
* \params
*   uiPort        [in]  XC port number
*   uHighPriority [in]  Indication priority selector (1/0: high/low priority)
*   pulFillLevel  [out] Indication FIFO fill level
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_GetRecvFillLevel( unsigned int uiPort,
                                         unsigned int uHighPriority,
                                         uint32_t *pulFillLevel )
{
  unsigned int uiFifo;

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  uiFifo  = uHighPriority ? ETHERNET_FIFO_IND_HI : ETHERNET_FIFO_IND_LO;
  uiFifo += NUM_FIFO_CHANNELS_PER_UNIT * uiPort;

  /* get fill level of FIFO */
  *pulFillLevel = NX_READ32(s_ptPFifo->aulPfifo_fill_level[uiFifo]);

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Get Interrupt(s)
* \description
*   This function retrieves the current interrupt requests from the according Ethernet MAC.
* \class
*   Status Service Class
* \params
*   uiPort    [in]  XC port number
*   pulIrq    [out] Pointer to interrupt events
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_GetIrq( unsigned int uiPort,
                               uint32_t *pulIrq )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  /* get IRQ events */
  *pulIrq = NX_READ32(s_ptXpecIrqs->aulIrq_xpec[uiPort]) & 0xffff;

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Confirm Interrupts
* \description
*   This function confirms a set of interrupts that were requested by the Ethernet MAC.
* \class
*   Control Service Class
* \params
*   uiPort           [in] XC port number
*   ulConfirmIrqMask [in] Mask to confirm interrupt events
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_ConfirmIrq( unsigned int uiPort,
                                   uint32_t ulConfirmIrqMask )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  /* confirm the set of IRQs */
  NX_WRITE32(s_ptXpecIrqs->aulIrq_xpec[uiPort], ulConfirmIrqMask & 0xffff);

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Get Interrupt Mask
* \description
*   This function gets a set of currently enabled interrupts at the according Ethernet MAC.
* \class
*   Status Service Class
* \params
*   uiPort               [in]  XC port number
*   pulIrqMask           [out] Pointer to unsigned long to receive the mask of enabled interrupts on the Ethernet MAC
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_GetIrqMask( unsigned int uiPort,
                                   uint32_t *pulIrqMask )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  /* retrieve the current IRQ mask */
  *pulIrqMask =  NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_IND_HI )
                |NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_IND_LO )
                |NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_CON_HI )
                |NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_CON_LO )
                |NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED )
                |NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_COL )
                |NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_EARLY_RCV )
                |NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_RX_ERR )
                |NX_READ32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_TX_ERR );

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Enable Interrupts
* \description
*   This function sets a set of interrupts to be enabled or disabled to the Ethernet MAC.
* \class
*   Control Service Class
* \params
*   uiPort                [in] XC port number
*   ulIrqMask             [in] Inclusively-ORed mask of interrupts to be enabled, otherwise they will be disabled
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_SetIrqMask( unsigned int uiPort,
                                   uint32_t ulIrqMask )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  /* set interrupt enable registers */
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_IND_HI, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_IND_HI_VAL );
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_IND_LO, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_IND_LO_VAL );
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_CON_HI, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_CON_HI_VAL );
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_CON_LO, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_CON_LO_VAL );
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_LINK_CHANGED_VAL );
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_COL, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_COL_VAL );
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_EARLY_RCV, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_EARLY_RCV_VAL );
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_RX_ERR, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_RX_ERR_VAL );
  NX_WRITE32( s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_INTERRUPTS_ENABLE_TX_ERR, ulIrqMask & MSK_ETHMAC_INTERRUPTS_ENABLE_TX_ERR_VAL );

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Initialize Ethernet MAC
* \description
*   This function initializes the according XC port as Ethernet MAC
*   and configures it with the default parameter settings.\n
*   Note that LEDs must be disabled when external PHYs are used.
* \class
*   Control Service Class
* \params
*   uiPort                [in] XC port number
*   ePhyLedCfg            [in] PHY LED behavior (0: two separate LEDs for link and activity not blinking; 1: as 0, but activity is blinking; 2: one single combined link/activity LED; 3: LEDs are disabled)
*   ulActLedFlashPeriod   [in] Flash frequency of activity LED [10ns], default: 5000000 = 50 milliseconds; The blink frequency shall not be smaller than 10ms and larger than 80 milliseconds, other values may lead to malfunction of the LED
*   pvUser                [in] User specific parameter
* \return
*   ETH_OKAY
*   ETH_ERR_INIT_FAILED
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_Initialize( unsigned int uiPort,
                                   ETHERNET_PHY_LED_CFG_E ePhyLedCfg,
                                   uint32_t ulActLedFlashPeriod,
                                   void* pvUser )
{
  unsigned int uIdx;

  #ifndef __ETHMAC_DISABLE_CHECKS__
  /* check flash period < 80ms */
  if( (ulActLedFlashPeriod < 1000000)
    ||(ulActLedFlashPeriod >= 8000000 )
  ) return ETH_ERR_INVALID_PARAMETER;

  /* check port no */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  /* initialize pointer to xPEC DRAM */
  s_aptXpecDRam[uiPort] = (ETHMAC_XPEC_DPM*) s_pulXpecDramStart[uiPort];

  /* reset the hardware block */
  if( pfnXcReset(uiPort, pvUser) != 0 )
    return ETH_ERR_INIT_FAILED;

  /* initialize the FIFO unit */
  initFifoUnit(uiPort);

  /* load the microcode into the hardware block */
  if( ( 0 != pfnXcLoad(uiPort, NXT(XC_TYPE_RPU),  (unsigned long*)paulxMacRpuCodes[uiPort], pvUser) )
    ||( 0 != pfnXcLoad(uiPort, NXT(XC_TYPE_TPU),  (unsigned long*)paulxMacTpuCodes[uiPort], pvUser) )
    ||( 0 != pfnXcLoad(uiPort, NXT(XC_TYPE_XPEC), (unsigned long*)paulxPecCodes[uiPort], pvUser) )
    )
    return ETH_ERR_INIT_FAILED;

  /* disable monitoring mode */
  NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_MONITORING_MODE, 0);

  /* set default traffic class arrangement */
  NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_TRAFFIC_CLASS_ARRANGEMENT, 4);

  /* copy the current SYSTIME border to the copy inside the hardware block */
  NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_SYSTIME_BORDER_COPY_UPPER, NX_READ32(s_ptSystime->ulSystime_border) & 0xffff0000);

  /* set default LED configuration (Note: no LED support for netX500 XC ports 2 and 3) */
  if(uiPort < 2) {
    NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_LED_CONFIG, ePhyLedCfg);
  } else {
    NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_LED_CONFIG, ETH_PHY_LED_OFF);
  }

  /* configure flash period */
  NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_CONFIG_AREA_BASE.ulETHMAC_PHY_LEDS_FLASH_PERIOD, ulActLedFlashPeriod);

#ifndef ETHMAC_RCV_ALL_MULTICASTS
  /* initialize group address table, broadcast address is first entry, all others are free */
  for(uIdx = 0; uIdx <= ETHMAC_GROUP_ADDR_NUM_MAX; ++uIdx)
    s_aullGroupAddr[uiPort][uIdx] = ( uIdx == 0 ) ? ETHMAC_BROADCAST_MAC : 0;
#else
  /* enable all Multicast addresses */
  for(uIdx = 0; uIdx < sizeof(s_aptXpecDRam[uiPort]->tETHMAC_MULTICAST_FILTER_TABLE.aulDataField)/4; ++uIdx)
  {
    NX_WRITE32(s_aptXpecDRam[uiPort]->tETHMAC_MULTICAST_FILTER_TABLE.aulDataField[uIdx], 0xffffffff);
  }
#endif

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Start Ethernet MAC
* \description
*   This function starts the previously initialized Ethernet MAC.
* \class
*   Control Service Class
* \params
*   uiPort                [in] XC port number
*   pvUser                [in] User specific parameter
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER
*   ETH_ERR_INIT_FAILED                                                      */
/*****************************************************************************/
ETHERNET_RESULT EthMac_Start( unsigned int uiPort,
                              void* pvUser )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check port no */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  if(ETH_OKAY != EthMac_ConfirmIrq( uiPort, 0xFFFF ))
    return ETH_ERR_INIT_FAILED;

  /* start XC port */
  if( pfnXcStart(uiPort, pvUser) != 0 )
    return ETH_ERR_INIT_FAILED;

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Set Link Mode
* \description
*   This function sets the link mode of the MAC.
*   Note: These values must match the mode the connected PHY is set to. Also in case of link down this function has to be called.
* \class
*   Control Service Class
* \params
*   uiPort            [in] XC port number
*   fValid            [in] true: link up
*   uiSpeed           [in] 10/100
*   fFdx              [in] true: FDX
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_SetLinkMode( unsigned int uiPort,
                                    bool fValid,
                                    unsigned int uiSpeed,
                                    bool fFdx )
{
  uint32_t ulVal = 0; /* default: invalid link and SPEED10 and HDX */

#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check the port number */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
  /* check speed */
  if( (uiSpeed != 10)
    &&(uiSpeed != 100)
    )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  if( fValid )
  {
    /* link valid: set speed and duplex */
    if(fValid) ulVal |= MSK_ETHMAC_ARM2XPEC_INTERRUPTS_VALID;
    if(uiSpeed == 100) ulVal |= MSK_ETHMAC_ARM2XPEC_INTERRUPTS_SPEED100;
    if(fFdx) ulVal |= MSK_ETHMAC_ARM2XPEC_INTERRUPTS_FDX;
  }

  /* set request */
  NX_WRITE32(s_ptXpecIrqs->aulIrq_xpec[uiPort], MSK_ETHMAC_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ | ulVal);

  /* wait for xPEC confirmed request */
  while( NX_READ32(s_ptXpecIrqs->aulIrq_xpec[uiPort] & MSK_ETHMAC_ARM2XPEC_INTERRUPTS_LINK_MODE_UPDATE_REQ) ) {}

  return ETH_OKAY;
}

/*****************************************************************************/
/*! Configure MII
* \description
*   This function configures the MII that is used.
*   Note: Only call this function before XC started.
*   Note: Use this function only when connecting an external PHY to compensate delays between external PHY and internal MAC logic.
*   Note: Default value fits to internal PHYs if available else to external MII.
* \class
*   Control Service Class
* \params
*   uiPort            [in] XC port number
*   uiCfg             [in] MII configuration; 0: internal PHY, 1: external MII
* \return
*   ETH_OKAY
*   ETH_ERR_INVALID_PARAMETER                                                */
/*****************************************************************************/
ETHERNET_RESULT EthMac_CfgMii( unsigned int uiPort,
                               unsigned int uiCfg
                             )
{
#ifndef __ETHMAC_DISABLE_CHECKS__
  /* check port no */
  if( uiPort >= ETHMAC_PORTS )
    return ETH_ERR_INVALID_PARAMETER;
#endif

  switch(uiCfg)
  {
  case 0:
    /* internal PHY (default) */
    /* nothing to do... */
    break;
  case 1:
    /* external MII */
    /* nothing to do... */
    break;
  default:
    return ETH_ERR_INVALID_PARAMETER;
    break;
  }

  return ETH_OKAY;
}

