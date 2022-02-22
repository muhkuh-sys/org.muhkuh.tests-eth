#ifndef _ETH_MAC_H
#define _ETH_MAC_H

#include "hal_ethmac.h" /* import ETHERNET_MAC_ADDR_T, ETHERNET_CONNECTION_STATE_T, ... */
#include "eth.h"

/****************************************************************************************
*  Definitions
****************************************************************************************/

#define ETH_MAC_IF_CNT 1

typedef struct ETH_MAC_CONFIG_Ttag
{
  const ETH_MAC_ADDR_T* ptAddrChassis0;
  uint64_t              ullSystimeBorder;
  unsigned int          uiMiiCfg;
} ETH_MAC_CONFIG_T;

typedef struct ETH_MAC_IF_Ttag
{
  ETHERNET_MAC_ADDR_T         tAddrPort;
  ETHERNET_CONNECTION_STATE_T tConnState;
  ETHMAC_COUNTERS_T           tMacCnt;
  bool                        fPhyPhaseOffsetValid;
  uint8_t                     bPhyPhaseOffset;
  uint32_t                    ulPortRxDelayLocal;
  uint32_t                    ulPortTxDelayLocal;
} ETH_MAC_IF_T;

typedef struct ETH_MAC_Ttag
{
  ETHERNET_MAC_ADDR_T tAddrChassis0;
  unsigned int   uiPort;
  uint64_t       ullSystimeBorder;
  uint32_t       ulIndHiFillLevel;
  uint32_t       ulIndLoFillLevel;
  uint32_t       ulCnfHiFillLevel;
  uint32_t       ulCnfLoFillLevel;
  uint32_t       ulFrameGet;
  uint32_t       ulFrameFree;
  uint32_t       ulEchoIndCnt;
  uint32_t       ulEchoCnfCnt;
  ETH_MAC_IF_T   atIf[ETH_MAC_IF_CNT];
  ETH_IF_T       atEthIf[ETH_MAC_IF_CNT];
  ETH_T          tEth;
  MUTEX_T        tMtx;
} ETH_MAC_T;


/****************************************************************************************
*  Variables
****************************************************************************************/

extern ETH_MAC_T g_atEthMac[];


/****************************************************************************************
*  Functions
****************************************************************************************/

ETH_T*   eth_mac_init(unsigned int uiPort, const ETH_MAC_CONFIG_T* ptConfig);
void     eth_mac_start(ETH_T* ptEth);
int      eth_mac_send_frame(void* pvInst, uint32_t ulInterfaces, SG_T* ptSg, unsigned int uiTLen, unsigned int uiPrio, ETH_TX_CNF_HANDLER_T* ptConfirmationHandler);
void     eth_mac_update_link_mode(ETH_T* ptEth);
void     eth_mac_update_counters(ETH_T* ptEth);
uint32_t eth_mac_ind(ETH_T* ptEth, unsigned int uPriority, uint32_t ulMaxFrames);
uint32_t eth_mac_cnf(ETH_T* ptEth, unsigned int uPriority, uint32_t ulMaxFrames);

void eth_mac_isr(unsigned int uiPort);

void isr_init(void);

#endif /* _ETH_MAC_H */
