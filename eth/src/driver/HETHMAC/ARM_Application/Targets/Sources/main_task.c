#include <string.h>
#include "hal_ethmac.h"
#include "eth_mac.h"
#include "uprintf.h"
#include "sys.h"
#include "ip.h"
#include "phy.h"

#define PORT_CNT_MAX 4U
#define ACTIVITY_LED_FLASH_PERIOD 5000000 /* 50 milliseconds */

/* Register 0 - Basic Control Register Bit Definition */
#define PHY_CONTROL_RESET                  (0x8000)        /* PHY reset                          */
#define PHY_CONTROL_LOOPBACK               (0x4000)        /* Enable loop back                   */
#define PHY_CONTROL_SPEED_SELECT_100       (0x2000)        /* Select Speed 100MBit               */
#define PHY_CONTROL_AUTO_NEG_ENABLE        (0x1000)        /* Auto-Negotiation Enable            */
#define PHY_CONTROL_POWER_DOWN             (0x0800)        /* Power-down                         */
#define PHY_CONTROL_ISOLATE                (0x0400)        /* Electrically Isolate PHY from MII  */
#define PHY_CONTROL_AUTO_NEG_RESTART       (0x0200)        /* Restart Auto-Negotiation           */
#define PHY_CONTROL_FULL_DUPLEX            (0x0100)        /* FDX Mode                           */

/* Register 4/5 - Auto Negotiation Advertisement Register Bit Definition, Reg4: own capabilities, Reg5: Link partner capabilities */
#define PHY_ADV_NEXT_PAGE                  (0x8000)        /* Ability to send multiple pages     */
#define PHY_ADV_REMOTE_FAULT               (0x2000)        /* Remote fault                       */
#define PHY_ADV_PAUSE                      (0x0400)        /* PAUSE capability                   */
#define PHY_ADV_100_BASE_T4                (0x0200)        /* 100BASE-T4 capability              */
#define PHY_ADV_100_BASE_TX_FDX            (0x0100)        /* 100BASE-TX FDX capability          */
#define PHY_ADV_100_BASE_TX                (0x0080)        /* 100BASE-TX capability              */
#define PHY_ADV_10_BASE_T_FDX              (0x0040)        /* 10BASE-T FDX capability            */
#define PHY_ADV_10_BASE_T                  (0x0020)        /* 10BASE-T capability                */
#define PHY_ADV_SELECTOR_FIELD             (0x001f)        /* <00001> = IEEE 802.3, read-only    */

#define PHY_ADV_ALL               (PHY_ADV_10_BASE_T\
                                  |PHY_ADV_10_BASE_T_FDX\
                                  |PHY_ADV_100_BASE_TX\
                                  |PHY_ADV_100_BASE_TX_FDX)


#define IP_ADDR_DFLT         IP_ADDR(192, 168, 200, 158)
#define IP_SUBNETMASK_DFLT   IP_ADDR(  0,   0,   0,   0)
#define IP_GATEWAY_ADDR_DFLT IP_ADDR(  0,   0,   0,   0)

/* MAC Addresses */
const ETH_MAC_ADDR_T g_atMacAdr[] =
{
  { 0x00, 0xb2, 0x23, 0x34, 0x45, 0x00 },
  { 0x00, 0xb2, 0x23, 0x34, 0x45, 0x01 },
  { 0x00, 0xb2, 0x23, 0x34, 0x45, 0x02 },
  { 0x00, 0xb2, 0x23, 0x34, 0x45, 0x03 }
};

ETH_T* g_aptEth[PORT_CNT_MAX];

/* pointer to link state strings */
static char *pszLnk[PORT_CNT_MAX], *pszDplx[PORT_CNT_MAX], *pszSpeed[PORT_CNT_MAX];

/* MII timing configuration, passed into EthMac_CfgMii() */
/* Note: Value of zero is the default configuration which may be overwritten
 * for external PHYs in main.c */
unsigned int g_auiMiiCfg[PORT_CNT_MAX] = { 0, 0, 0, 0 };

#define FOREACH(idx, max) for(idx = 0; idx < max; ++idx)

static void print_counter(const char* pszName, ETHMAC_COUNTERS_T* ptCnt, size_t sizOffset, unsigned int uiPortCnt)
{
  unsigned int uiPort;
  uint32_t* pulBase;

  uprint(pszName);
  FOREACH(uiPort, uiPortCnt)
  {
    pulBase = (uint32_t*)&ptCnt[uiPort];
    uprintf("  %08x", pulBase[sizOffset/sizeof(uint32_t)]);
  }
  uprint("\n");
}

int main_task(unsigned int uiPortCnt)
{
  unsigned int uiPort;
  ETH_MAC_CONFIG_T tCfg;
  ETHMAC_COUNTERS_T   atEthCnt[PORT_CNT_MAX];
  uint32_t ulSystimeS_Last = 0;

  if( uiPortCnt > PORT_CNT_MAX )
  {
    return -1;
  }

  memset(atEthCnt, 0, sizeof(atEthCnt));

  for(uiPort = 0; uiPort < PORT_CNT_MAX; ++uiPort)
  {
    /* initialize link state strings to "Down 10HD" */
    pszLnk[uiPort] = "Invalid";
    pszDplx[uiPort] = " ";
    pszSpeed[uiPort] = " ";
  }

  tCfg.ullSystimeBorder = SYSTIME_BORDER;

  /* initialize all channels */
  FOREACH(uiPort, uiPortCnt)
  {
    /* set AUTO_NEG_ADVERTISEMENT */
    //phy_write(uiPort, 4, 1 | PHY_ADV_ALL);             /* advertise all modes */
    //phy_write(uiPort, 4, 1 | PHY_ADV_10_BASE_T);       /* advertise 10HD only */
    //phy_write(uiPort, 4, 1 | PHY_ADV_10_BASE_T_FDX);   /* advertise 10FD only */
    //phy_write(uiPort, 4, 1 | PHY_ADV_100_BASE_TX);     /* advertise 100HD only */
    //phy_write(uiPort, 4, 1 | PHY_ADV_100_BASE_TX_FDX); /* advertise 100FD only */
    //phy_write(uiPort, 0, PHY_CONTROL_AUTO_NEG_ENABLE | PHY_CONTROL_AUTO_NEG_RESTART); /* restart auto-neg */
    //phy_write(uiPort, 0, PHY_CONTROL_SPEED_SELECT_100); /* Force 100HD */
    //phy_write(uiPort, 0, 0); /* Force 10HD */

    tCfg.ptAddrChassis0 = &g_atMacAdr[uiPort];
    tCfg.uiMiiCfg = g_auiMiiCfg[uiPort];
    g_aptEth[uiPort] = eth_mac_init(uiPort, &tCfg);
    if( g_aptEth[uiPort] == NULL )
    {
      return -2;
    }

    if( ETH_OKAY != EthMac_SetIrqMask(uiPort,
                         MSK_ETHMAC_IRQ_IND_HI
                        |MSK_ETHMAC_IRQ_IND_LO
                        |MSK_ETHMAC_IRQ_CON_HI
                        |MSK_ETHMAC_IRQ_CON_LO
                        |MSK_ETHMAC_IRQ_LINK_CHANGED
  //                      |MSK_ETHMAC_IRQ_COL
  //                      |MSK_ETHMAC_IRQ_EARLY_RCV
  //                      |MSK_ETHMAC_IRQ_RX_ERR
  //                      |MSK_ETHMAC_IRQ_TX_ERR
                         ) )
    {
      return -3;
    }

    eth_mac_start(g_aptEth[uiPort]);
  }

  ip_init();
  ip_config(IP_ADDR_DFLT, IP_SUBNETMASK_DFLT, IP_GATEWAY_ADDR_DFLT);

  /* initialize the IRQ, set ISR */
  irq_init();

  sys_led_rdyrun_set(LED_COLOUR_GREEN);

  /* packets are handled in the ISR, show statistics */
  while(1)
  {
    uint32_t ulSystimeS, ulSystimeNs;

    /* poll USB connection */
    sys_poll();

    /* get system time for cyclic tasks */
    sys_time_get(&ulSystimeS, &ulSystimeNs);

    /* once per second .. */
    if( ulSystimeS != ulSystimeS_Last )
    {
      ulSystimeS_Last = ulSystimeS;

      /* let RdyRun LED blink */
      sys_led_rdyrun_set((ulSystimeS & 1) ? LED_COLOUR_OFF : LED_COLOUR_GREEN);

      /* get statistics */
      FOREACH(uiPort, uiPortCnt)
      {
        EthMac_GetCounters(uiPort, &atEthCnt[uiPort]);

        /* get actual MAU type from interface */
        pszLnk[uiPort] = (g_aptEth[uiPort]->patIf[0].tMauType.uIsLinkUp == 1) ? "Up  " : "Down";
        pszDplx[uiPort] = (g_aptEth[uiPort]->patIf[0].tMauType.uIsFullDuplex == 1) ? "FD" : "HD";
        pszSpeed[uiPort] = (g_aptEth[uiPort]->patIf[0].tMauType.uSpeed == 100) ? "100" : "10 ";
      }

      /* print statistics */
      uprint("\n");
      uprint("Statistics Port                "); FOREACH(uiPort, uiPortCnt){ uprintf("  %8d", uiPort); } uprint("\n");
      uprint("Link                            "); FOREACH(uiPort, uiPortCnt){ uprintf(" %s%s%s", pszLnk[uiPort], pszDplx[uiPort], pszSpeed[uiPort]); } uprint("\n");
      print_counter("OutFramesOk                    ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_OUT_FRAMES_OKAY),                   uiPortCnt);
      print_counter("OutOctets                      ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_OUT_OCTETS),                        uiPortCnt);
      print_counter("SingleCollisionFrames          ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_SINGLE_COLLISION_FRAMES),           uiPortCnt);
      print_counter("MultipleCollisionFrames        ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_MULTIPLE_COLLISION_FRAMES),         uiPortCnt);
      print_counter("LateCollisions                 ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_LATE_COLLISIONS),                   uiPortCnt);
      print_counter("LinkDownDuringTransmission     ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_LINK_DOWN_DURING_TRANSMISSION),     uiPortCnt);
      print_counter("UtxUnderflowDuringTransmission ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_UTX_UNDERFLOW_DURING_TRANSMISSION), uiPortCnt);
      print_counter("InFramesOk                     ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_IN_FRAMES_OKAY),                    uiPortCnt);
      print_counter("InOctets                       ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_IN_OCTETS),                         uiPortCnt);
      print_counter("FrameCheckSequenceErrors       ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_FRAME_CHECK_SEQUENCE_ERRORS),       uiPortCnt);
      print_counter("AlignmentErrors                ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_ALIGNMENT_ERRORS),                  uiPortCnt);
      print_counter("FrameTooLongErrors             ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_FRAME_TOO_LONG_ERRORS),             uiPortCnt);
      print_counter("RuntFramesReceived             ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_RUNT_FRAMES_RECEIVED),              uiPortCnt);
      print_counter("CollisionFragmentsReceived     ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_COLLISION_FRAGMENTS_RECEIVED),      uiPortCnt);
      print_counter("FramesDroppedDueLowResource    ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_FRAMES_DROPPED_DUE_LOW_RESOURCE),   uiPortCnt);
      print_counter("FramesDroppedDueUrxOverflow    ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_FRAMES_DROPPED_DUE_URX_OVERFLOW),   uiPortCnt);
      print_counter("TxFatalError                   ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_TX_FATAL_ERROR),                    uiPortCnt);
      print_counter("RxFatalError                   ", atEthCnt, offsetof(ETHMAC_COUNTERS_T, ulETHMAC_RX_FATAL_ERROR),                    uiPortCnt);
    }
  }

  return 0;
}

void fatal_error(const char* pszMsg, ...)
{
  va_list ptArgs;

  va_start(ptArgs, pszMsg);
  vuprintf(pszMsg, ptArgs);
  va_end(ptArgs);

  while(1);
  {
    /* poll USB connection to trace debug messages */
    sys_poll();
  }
}
