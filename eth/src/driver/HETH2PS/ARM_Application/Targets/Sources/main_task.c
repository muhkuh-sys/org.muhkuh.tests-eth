#include "eth_2ps.h"
#include "ip.h"
#include "uprintf.h"
#include "sys.h"
#include "sync.h"
#include "targetlib.h"
#include "sniprintf.h"


/****************************************************************************************
*  Definitions
****************************************************************************************/



/* Local IP addresses */
#define IP0 192U
#define IP1 168U
#define IP2 200U
#define IP3 152U

#define IP_ADDR_DFLT         IP_ADDR(IP0, IP1, IP2, IP3)
#define IP_SUBNETMASK_DFLT   IP_ADDR(  0,   0,   0,   0)
#define IP_GATEWAY_ADDR_DFLT IP_ADDR(  0,   0,   0,   0)

/* MAC addresses */
#define CHASSIS_MAC_ADDRESSES { 0x00, 0xb2, 0x23, 0x34, 0x45, 0x00 },\
                              { 0x00, 0xb4, 0x43, 0x32, 0x21, 0x00 }

#define PORT_CNT_MAX 4U
#define ACTIVITY_LED_FLASH_PERIOD 5000000 /* 50 milliseconds */

static void diag_print(void);
static void poll(void);

/****************************************************************************************
*  Variables
****************************************************************************************/

ETH2PS_MAC_ADDR_T g_atChassisMacAddr[] = { CHASSIS_MAC_ADDRESSES };

char g_szErrorDescr[256];
char g_szMacAdr[18];
char g_szIpAdr[16];
uint32_t g_ulSystimeS;
ETH_T* g_ptEth;

unsigned int g_uDiagPrintState;
static bool fFatalError = false;

SYS_CFG_T g_tSysCfg;


/****************************************************************************************
*  Functions
****************************************************************************************/

void main_task(void)
{
  ETH_2PS_CONFIG_T tEthCfg;
  unsigned int uiPort;

  led_check();

  /* reset global variables */
  g_szErrorDescr[0] = 0;
  g_ulSystimeS      = 0;
  g_uDiagPrintState = 0;

  /*lint -e(534) ignore return value */
  sniprintf(g_szMacAdr, sizeof(g_szMacAdr), "%02x:%02x:%02x:%02x:%02x:%02x", g_atChassisMacAddr[0][0],
                                                         g_atChassisMacAddr[0][1],
                                                         g_atChassisMacAddr[0][2],
                                                         g_atChassisMacAddr[0][3],
                                                         g_atChassisMacAddr[0][4],
                                                         g_atChassisMacAddr[0][5]);

  /*lint -e(534) ignore return value */
  sniprintf(g_szIpAdr, sizeof(g_szIpAdr), "%d.%d.%d.%d", IP0, IP1, IP2, IP3);

  for(uiPort = 0; uiPort < 2; ++uiPort)
  {
    /* set AUTO_NEG_ADVERTISEMENT, this has no effect on PHYs that use clause 45 MDIO only */
    phy_write(uiPort, 4, 1 | PHY_ADV_ALL);             /* advertise all modes */
    //phy_write(uiPort, 4, 1 | PHY_ADV_10_BASE_T);       /* advertise 10HD only */
    //phy_write(uiPort, 4, 1 | PHY_ADV_10_BASE_T_FDX);   /* advertise 10FD only */
    //phy_write(uiPort, 4, 1 | PHY_ADV_100_BASE_TX);     /* advertise 100HD only */
    //phy_write(uiPort, 4, 1 | PHY_ADV_100_BASE_TX_FDX); /* advertise 100FD only */
    phy_write(uiPort, 0, PHY_CONTROL_AUTO_NEG_ENABLE | PHY_CONTROL_AUTO_NEG_RESTART); /* restart auto-neg */
    //phy_write(uiPort, 0, PHY_CONTROL_SPEED_SELECT_100); /* Force 100HD */
    //phy_write(uiPort, 0, PHY_CONTROL_SPEED_SELECT_100 | PHY_CONTROL_FULL_DUPLEX); /* Force 100FD */
    //phy_write(uiPort, 0, 0); /* Force 10HD */
  }

  /* initialize driver for Ethernet 2-Port Switch */
  tEthCfg.ptAddrChassis0 = (ETH_MAC_ADDR_T*)&g_atChassisMacAddr[0][0];
  tEthCfg.ptAddrChassis1 = (ETH_MAC_ADDR_T*)&g_atChassisMacAddr[1][0];
  tEthCfg.ulAgingTimeMs  = 10000; /* 10 seconds */

  /* Copy target specific port configuration */
  for(uiPort = 0; uiPort < 2; ++uiPort)
  {
    tEthCfg.atPortCfg[uiPort].ulIngressLatency = g_tSysCfg.atPortCfg[uiPort].ulIngressLatency;
    tEthCfg.atPortCfg[uiPort].ulEgressLatency  = g_tSysCfg.atPortCfg[uiPort].ulEgressLatency;
    tEthCfg.atPortCfg[uiPort].fSetExtMii = g_tSysCfg.atPortCfg[uiPort].fSetExtMii;
    tEthCfg.atPortCfg[uiPort].fNoPhyLinkInput = g_tSysCfg.atPortCfg[uiPort].fNoPhyLinkInput;
    tEthCfg.atPortCfg[uiPort].fDisablePort = g_tSysCfg.atPortCfg[uiPort].fDisablePort;
  }

  g_ptEth = eth_2ps_init(&tEthCfg);
  
  /* initialize stack */
  ip_init();
  ip_config(IP_ADDR_DFLT, IP_SUBNETMASK_DFLT, IP_GATEWAY_ADDR_DFLT);

  /* enable driver */
  eth_2ps_start(g_ptEth);

  /* initialize the IRQ handlers, set ISRs */
  irq_init();

  sync_init();

  /* all initialized, set RDYRUN led to green */
  led_rdyrun_set(LED_COLOUR_GRN);
  
  while(1) 
  {
    unsigned int uPortNo;

    /* poll diagnostics and do low priority Ethernet communication */
    poll();

    /* check for fatal error conditions */
    for(uPortNo = 0; uPortNo < 2; ++uPortNo)
    {
#if 0
      if( g_tEth2PS.atIf[uPortNo].tMacCnt.ulRxOverflow != 0 )
        fatal_error("main.c:%d: URX overflow detected on port %d", __LINE__, uPortNo);
#endif

      if( g_tEth2PS.atIf[uPortNo].tMacCnt.ulTxUnderrun != 0 )
        fatal_error("main.c:%d: UTX underrun detected on port %d", __LINE__, uPortNo);
    }
  }
}


/* Stop program and turn on red light */
void fatal_error(const char* pszDescription, ...)
{
  va_list ptArgs;

  /* suppress interrupt service routines */
  IRQ_Disable();

  led_rdyrun_set(LED_COLOUR_RED);

  fFatalError = true;

  /* print error description to buffer */
  va_start(ptArgs, pszDescription);
  /*lint -e(534) ignore return value */
  vsniprintf(g_szErrorDescr, sizeof(g_szErrorDescr), pszDescription, ptArgs);
  va_end(ptArgs);
}

static void poll(void)
{
  uint32_t ulSystimeS;
  static bool s_fConnected = false;

  serial_poll(0);

  if( serial_recv(0) >= 0 )
  {
    if( !s_fConnected )
    {
      s_fConnected = true;

      /* tell uprintf() how to print it's output */
      uprint_init(serial_send, NULL, 0);
    }
  }

  /* get system time for cyclic tasks */
  systime_get(g_tSysCfg.uiSystime, &ulSystimeS, NULL);

  /* once per second .. */
  if( ulSystimeS != g_ulSystimeS )
  {
    g_ulSystimeS = ulSystimeS;

    if( !fFatalError )
    {
      /* let the driver update it's diagnostic counters */
      eth_2ps_update_counters(g_ptEth);

      /* let RdyRun LED blink */
      led_rdyrun_set((ulSystimeS & 1) ? LED_COLOUR_OFF : LED_COLOUR_GRN);

      /* test generation of cyclic events */
      sync_test();
    }

    if( g_uDiagPrintState == 0 )
      g_uDiagPrintState = 1;
  }

  diag_print();
}


static const char* diag_str_link(ETH_MAU_TYPE_T* ptMauType)
{
  const char* pszStr = "DOWN    ";

  if( ptMauType->uIsLinkUp )
    pszStr = "UP      ";

  return pszStr;
}

static const char* diag_str_mautype(ETH_MAU_TYPE_T* ptMauType)
{
  const char* pszStr = NULL;

  if( ptMauType->uIsFullDuplex )
  {
    if( ptMauType->uSpeed == 100 )
      pszStr = "FD 100  ";
    else
      pszStr = "FD 10   ";
  }
  else
  {
    if( ptMauType->uSpeed == 100 )
      pszStr = "HD 100  ";
    else
      pszStr = "HD 10   ";
  }

  return pszStr;
}

/* print diagnostics */
static void diag_print(void)
{
  ETH_IF_T* patIf[2];
  ETH2PS_COUNTERS_T* aptCnt[2];

  aptCnt[0] = &g_tEth2PS.atIf[0].tMacCnt;
  aptCnt[1] = &g_tEth2PS.atIf[1].tMacCnt;

  patIf[0] = &g_ptEth->patIf[0];
  patIf[1] = &g_ptEth->patIf[1];

  switch(g_uDiagPrintState)
  {
  case 0:
    /* nothing to print */
    break;

  case 1:
    uprintf("\n---------- Ethernet 2-Port Switch ----------\n\n");
    uprintf("MAC Address: %s\n", g_szMacAdr);
    uprintf("IP  Address: %s\n", g_szIpAdr);
    uprintf("P0: Ingress / Egress latencies: %d / %d, PhyPhaseOffset: %d, SetExtMii=%d\n", g_tEth2PS.atIf[0].ulPortRxDelayLocal, g_tEth2PS.atIf[0].ulPortTxDelayLocal, g_tEth2PS.atIf[0].bPhyPhaseOffset, g_tSysCfg.atPortCfg[0].fSetExtMii ? 1 : 0);
    uprintf("P1: Ingress / Egress latencies: %d / %d, PhyPhaseOffset: %d, SetExtMii=%d\n", g_tEth2PS.atIf[0].ulPortRxDelayLocal, g_tEth2PS.atIf[0].ulPortTxDelayLocal, g_tEth2PS.atIf[1].bPhyPhaseOffset, g_tSysCfg.atPortCfg[1].fSetExtMii ? 1 : 0);
    uprintf("\n");
    uprintf("Time since startup:    %d s\n", g_ulSystimeS);
    uprintf("EMPTY_PTR  Fill Level: %d\n", g_tEth2PS.ulEmptyPtrFillLevel);
    uprintf("REQ_LO_P0  Fill Level: %d\n", Eth2PS_GetReqFillLevel(0, 0));
    uprintf("REQ_HI_P0  Fill Level: %d\n", Eth2PS_GetReqFillLevel(0, 1));
    uprintf("REQ_LO_P1  Fill Level: %d\n", Eth2PS_GetReqFillLevel(1, 0));
    uprintf("REQ_HI_P1  Fill Level: %d\n", Eth2PS_GetReqFillLevel(1, 1));
    uprintf("IND_CNF_HI Fill Level: %d\n", g_tEth2PS.ulIndCnfHiFillLevel);
    uprintf("IND_CNF_LO Fill Level: %d\n", g_tEth2PS.ulIndCnfLoFillLevel);
    uprintf("Sync irq trg0        : %d\n", g_tSync.aulIrqCnt[0]);
    uprintf("Sync irq trg1        : %d\n", g_tSync.aulIrqCnt[1]);
    uprintf("Echo indication        %d\n", g_tEth2PS.ulEchoIndCnt);
    uprintf("Echo confirmation      %d\n", g_tEth2PS.ulEchoCnfCnt);
    uprintf("\n");
    g_uDiagPrintState++;
    break;

  case 2:
    uprintf("Connection status         Port 0    Port 1\n");
    uprintf("Link status               %s  %s\n", diag_str_link(&patIf[0]->tMauType),    diag_str_link(&patIf[1]->tMauType));
    uprintf("MAU type                  %s  %s\n", diag_str_mautype(&patIf[0]->tMauType), diag_str_mautype(&patIf[1]->tMauType));
    uprintf("\n");
    uprintf("Interrupt counters        xPEC 0    xPEC 1\n");
    uprintf("IND_CNF_HI                %08x  %08x\n", g_tIsrCnt.aulIndCnfHi[0], g_tIsrCnt.aulIndCnfHi[1]);
    uprintf("IND_CNF_LO                %08x  %08x\n", g_tIsrCnt.aulIndCnfLo[0], g_tIsrCnt.aulIndCnfLo[1]);
    uprintf("LINK_CHANGED              %08x  %08x\n", g_tIsrCnt.aulLinkChng[0], g_tIsrCnt.aulLinkChng[1]);
    uprintf("CIRCULATING_FRM           %08x  %08x\n", g_tIsrCnt.aulCirclFrm[0], g_tIsrCnt.aulCirclFrm[1]);
    uprintf("BCN_TIMEOUT               %08x  %08x\n", g_tIsrCnt.aulBeaconTO[0], g_tIsrCnt.aulBeaconTO[1]);
    uprintf("\n");
    g_uDiagPrintState++;
    break;

  case 3:
    uprintf("MAC error counters        Port 0    Port 1\n");
    uprintf("TxOutOctets               %08x  %08x\n", aptCnt[0]->ulTxOutOctets,             aptCnt[1]->ulTxOutOctets);
    uprintf("TxSingleCollisions        %08x  %08x\n", aptCnt[0]->ulTxSingleCollisions,      aptCnt[1]->ulTxSingleCollisions);
    uprintf("TxMultipleCollisions      %08x  %08x\n", aptCnt[0]->ulTxMultipleCollisions,    aptCnt[1]->ulTxMultipleCollisions);
    uprintf("TxLateCollisions          %08x  %08x\n", aptCnt[0]->ulTxLateCollisions,        aptCnt[1]->ulTxLateCollisions);
    uprintf("TxUnderrun                %08x  %08x\n", aptCnt[0]->ulTxUnderrun,              aptCnt[1]->ulTxUnderrun);
    uprintf("TxAborted                 %08x  %08x\n", aptCnt[0]->ulTxAborted,               aptCnt[1]->ulTxAborted);
    uprintf("TxDiscarded               %08x  %08x\n", aptCnt[0]->ulTxDiscarded,             aptCnt[1]->ulTxDiscarded);
    uprintf("RxInOctets                %08x  %08x\n", aptCnt[0]->ulRxInOctets,              aptCnt[1]->ulRxInOctets);
    uprintf("RxFcsErrors               %08x  %08x\n", aptCnt[0]->ulRxFcsErrors,             aptCnt[1]->ulRxFcsErrors);
    uprintf("RxAlignmentErrors         %08x  %08x\n", aptCnt[0]->ulRxAlignmentErrors,       aptCnt[1]->ulRxAlignmentErrors);
    uprintf("RxFrameLengthErrors       %08x  %08x\n", aptCnt[0]->ulRxFrameLengthErrors,     aptCnt[1]->ulRxFrameLengthErrors);
    uprintf("RxRuntFrames              %08x  %08x\n", aptCnt[0]->ulRxRuntFrames,            aptCnt[1]->ulRxRuntFrames);
    uprintf("RxCollisionFragments      %08x  %08x\n", aptCnt[0]->ulRxCollisionFragments,    aptCnt[1]->ulRxCollisionFragments);
    uprintf("RxOverflow                %08x  %08x\n", aptCnt[0]->ulRxOverflow,              aptCnt[1]->ulRxOverflow);
    uprintf("RxDiscarded               %08x  %08x\n", aptCnt[0]->ulRxDiscarded,             aptCnt[1]->ulRxDiscarded);
    uprintf("RxCirculatingFrmBlocked   %08x  %08x\n", aptCnt[0]->ulRxCirculatingFrmBlocked, aptCnt[1]->ulRxCirculatingFrmBlocked);
    uprintf("RxUnknownErrors           %08x  %08x\n", aptCnt[0]->ulRxUnknownErrors,         aptCnt[1]->ulRxUnknownErrors);    uprintf("\n");
    g_uDiagPrintState++;
    break;

  case 4:
    uprintf("Interface counters        Port 0    Port 1\n");
    uprintf("InOctets                  %08x  %08x\n", patIf[0]->tIfEntry.ulInOctets,         patIf[1]->tIfEntry.ulInOctets);
    uprintf("InUcastPkts               %08x  %08x\n", patIf[0]->tIfEntry.ulInUcastPkts,      patIf[1]->tIfEntry.ulInUcastPkts);
    uprintf("InNUcastPkts              %08x  %08x\n", patIf[0]->tIfEntry.ulInNUcastPkts,     patIf[1]->tIfEntry.ulInNUcastPkts);
    uprintf("InDiscards                %08x  %08x\n", patIf[0]->tIfEntry.ulInDiscards,       patIf[1]->tIfEntry.ulInDiscards);
    uprintf("InErrors                  %08x  %08x\n", patIf[0]->tIfEntry.ulInErrors,         patIf[1]->tIfEntry.ulInErrors);
    uprintf("InUnknownProtos           %08x  %08x\n", patIf[0]->tIfEntry.ulInUnknownProtos,  patIf[1]->tIfEntry.ulInUnknownProtos);
    uprintf("OutOctets                 %08x  %08x\n", patIf[0]->tIfEntry.ulOutOctets,        patIf[1]->tIfEntry.ulOutOctets);
    uprintf("OutUcastPkts              %08x  %08x\n", patIf[0]->tIfEntry.ulOutUcastPkts,     patIf[1]->tIfEntry.ulOutUcastPkts);
    uprintf("OutNUcastPkts             %08x  %08x\n", patIf[0]->tIfEntry.ulOutNUcastPkts,    patIf[1]->tIfEntry.ulOutNUcastPkts);
    uprintf("OutDiscards               %08x  %08x\n", patIf[0]->tIfEntry.ulOutDiscards,      patIf[1]->tIfEntry.ulOutDiscards);
    uprintf("OutErrors                 %08x  %08x\n", patIf[0]->tIfEntry.ulOutErrors,        patIf[1]->tIfEntry.ulOutErrors);
    uprintf("OutQLen                   %08x  %08x\n", patIf[0]->tIfEntry.ulOutQLen,          patIf[1]->tIfEntry.ulOutQLen);
    uprintf("\n");
    if( g_szErrorDescr[0] )
      uprintf("FATAL ERROR: %s\n\n", g_szErrorDescr);
    g_uDiagPrintState++;
    break;

  default:
    g_uDiagPrintState = 0;
    break;
  }
}
