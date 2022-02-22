#ifndef ETH_2PS_H_
#define ETH_2PS_H_

#include "hal_eth2ps.h" /* import ETH2PS_PFIFO_MAPPING_E */
#include "eth.h"

/****************************************************************************************
*  Definitions
****************************************************************************************/
typedef struct ETH_2PS_PORT_CONFIG_Ttag
{
  uint32_t        ulIngressLatency;
  uint32_t        ulEgressLatency;
  bool            fSetExtMii;             /* true if default setting in Eth2PS_Initialize() must be overwritten (e.g. XC0 connected to external MII)  */
  bool            fNoPhyLinkInput;        /* true: disables the link status check when transmitting frames as well as the link change interrupt from
                                                   xMAC to ARM */
  bool            fDisablePort;           /* true: Disable port at initialization to discard all frames that should be transmitted */
} ETH_2PS_PORT_CONFIG_T;

typedef struct ETH_2PS_CONFIG_Ttag
{
  ETH_MAC_ADDR_T* ptAddrChassis0;
  ETH_MAC_ADDR_T* ptAddrChassis1;
  uint32_t        ulAgingTimeMs;
  ETH_2PS_PORT_CONFIG_T atPortCfg[2];
} ETH_2PS_CONFIG_T;

typedef struct ETH_2PS_IF_Ttag
{
  ETH_MAC_ADDR_T            tAddrPort;
  ETH2PS_CONNECTION_STATE_T tConnState;
  ETH2PS_COUNTERS_T         tMacCnt;
  bool                      fPhyPhaseOffsetValid;
  uint8_t                   bPhyPhaseOffset;
  uint32_t                  ulPortRxDelayLocal;
  uint32_t                  ulPortTxDelayLocal;
} ETH_2PS_IF_T;

typedef struct ETH_2PS_Ttag
{
  ETH_MAC_ADDR_T tAddrChassis0;
  ETH_MAC_ADDR_T tAddrChassis1;
  uint32_t       ulEmptyPtrFillLevel;
  uint32_t       ulIndCnfHiFillLevel;
  uint32_t       ulIndCnfLoFillLevel;
  uint32_t       ulFrameGet;
  uint32_t       ulFrameFree;
  uint32_t       ulEchoIndCnt;
  uint32_t       ulEchoCnfCnt;
  ETH_2PS_IF_T   atIf[2];
  ETH_IF_T       atEthIf[2];
  ETH_T          tEth;
  void           (*pfnBcnTimeout)(ETH_T* ptEth, unsigned int uiPort);
} ETH_2PS_T;

typedef struct ISR_CNT_Ttag
{
  uint32_t aulIndCnfHi[2];
  uint32_t aulIndCnfLo[2];
  uint32_t aulLinkChng[2];
  uint32_t aulCirclFrm[2];
  uint32_t aulBeaconTO[2];
} ISR_CNT_T;

/****************************************************************************************
*  Variables
****************************************************************************************/

extern ETH_2PS_T g_tEth2PS;
extern ISR_CNT_T g_tIsrCnt;

/****************************************************************************************
*  Functions
****************************************************************************************/

ETH_T* eth_2ps_init(ETH_2PS_CONFIG_T* ptConfig);
void eth_2ps_start(ETH_T* ptEth);
void eth_2ps_mau_type_change_ind(ETH_T* ptEth, unsigned int uPortNum);
void eth_2ps_ind_cnf(ETH_T* ptEth, unsigned int uPriority, uint32_t ulMaxFrames);
void eth_2ps_update_counters(ETH_T* ptEth);
void eth_2ps_inform_bcn_to(ETH_T* ptEth, void (*pfnBcnTimeout)(ETH_T* ptEth, unsigned int uiPort));

void eth_2ps_irq_init(ETH_T* ptEth);
void eth_2ps_irq_handler(ETH_T* ptEth, unsigned int uPortNo, bool fPriority);

#endif /* ETH_2PS_H_ */
