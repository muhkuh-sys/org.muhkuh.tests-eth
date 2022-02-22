#include "hal_phyled_xpic.h"
#include "phyled_xpic_ram.h"
#include "phyled_xpic2.h"

#include "hal_resources_defines_netx4000.h"
__USE_XPIC_LOADER_FUNC_RESET
__USE_XPIC_LOADER_FUNC_LOAD
__USE_XPIC_LOADER_FUNC_START
__USE_XPIC_DRAM

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define PHYLED_XPIC_INST                   2
#define PHYLED_XPIC_POLL_INTERVAL_10NS   250
#define PHYLED_XPIC_UPDATE_INTERVAL_MS    50
#define PHYLED_XPIC_SR_MAX                17
#define PHYLED_XPIC_SR_BIT_MAX            31

/*****************************************************************************/
/* Pointer to XPIC DRAM                                                      */
/*****************************************************************************/
static PHYLED_XPIC_RAM_T* s_ptXpicDRam;

/* Pointer to xPIC microcodes */
static const uint32_t* const pulXpicCodes = XpicCode_phyled_xpic2;

void PhyledXpic_Initialize(void* pvUsr)
{
  unsigned int uiXpic = PHYLED_XPIC_INST;
  unsigned int uiPort;

  pfnXpicReset(uiXpic, pvUsr);
  pfnXpicLoad(uiXpic, pulXpicCodes, pvUsr);

  s_ptXpicDRam = (PHYLED_XPIC_RAM_T*)s_apulXpicDram[uiXpic];

  s_ptXpicDRam->tDRAM_GLOBAL_CFG.ulDRAM_POLL_INTERVAL_10NS = PHYLED_XPIC_POLL_INTERVAL_10NS;

  for(uiPort = 0; uiPort < 2; ++uiPort)
  {
    DRAM_PORT_T* ptPortRam = &s_ptXpicDRam->atDRAM_PORT[uiPort];
    unsigned int uiIntervalsPerMs = 100000 / PHYLED_XPIC_POLL_INTERVAL_10NS;

    ptPortRam->ulDRAM_LED_MODE            = PHYLED_XPIC_MODE_FLASHING << SRT_AREA_DRAM_PORT0_MODE;
    ptPortRam->ulDRAM_LED_UPDATE_INTERVAL = PHYLED_XPIC_UPDATE_INTERVAL_MS * uiIntervalsPerMs;
    ptPortRam->ulDRAM_ACT_STRETCH         = 2; /* stretch activity over two update intervals */
    ptPortRam->ulDRAM_RX_SR               = (16 + uiPort) << SRT_DRAM_RX_SR0_ADR; /* sr_statcfg0/1 */
    ptPortRam->ulDRAM_RX_SR_BIT           = 16 + 8; /* xmac_status_shared.stat_rpu */
    ptPortRam->ulDRAM_TX_SR               = (16 + uiPort) << SRT_DRAM_TX_SR0_ADR; /* sr_statcfg0/1 */
    ptPortRam->ulDRAM_TX_SR_BIT           = 16 + 14; /* xmac_status_shared.utx_empty */
    ptPortRam->ulDRAM_TX_SR_INV           = 1;
  }
}

int  PhyledXpic_SetParam(unsigned int uiPort, PHYLED_XPIC_PARAM_E ePrmId, unsigned int uiValue)
{
  int iRslt = 0;
  DRAM_PORT_T* ptPortRam = &s_ptXpicDRam->atDRAM_PORT[uiPort];

  switch(ePrmId)
  {
  case PHYLED_XPIC_PARAM_MODE:
    if( uiValue > PHYLED_XPIC_MODE_COMBINED )
    {
      iRslt = 2;
    }
    else
    {
      ptPortRam->ulDRAM_LED_MODE = uiValue << SRT_AREA_DRAM_PORT0_MODE;
    }
    break;

  case PHYLED_XPIC_PARAM_TX_SR:
    if( uiValue > PHYLED_XPIC_SR_MAX )
    {
      iRslt = 2;
    }
    else
    {
      ptPortRam->ulDRAM_TX_SR = uiValue << SRT_DRAM_TX_SR0_ADR;
    }
    break;

  case PHYLED_XPIC_PARAM_TX_SR_BIT:
    if( uiValue > PHYLED_XPIC_SR_BIT_MAX )
    {
      iRslt = 2;
    }
    else
    {
      ptPortRam->ulDRAM_TX_SR_BIT = uiValue;
    }
    break;

  case PHYLED_XPIC_PARAM_TX_SR_INV:
    if( uiValue > 1 )
    {
      iRslt = 2;
    }
    else
    {
      ptPortRam->ulDRAM_TX_SR_INV = uiValue;
    }
    break;

  case PHYLED_XPIC_PARAM_RX_SR:
    if( uiValue > PHYLED_XPIC_SR_MAX )
    {
      iRslt = 2;
    }
    else
    {
      ptPortRam->ulDRAM_RX_SR = uiValue << SRT_DRAM_RX_SR0_ADR;
    }
    break;

  case PHYLED_XPIC_PARAM_RX_SR_BIT:
    if( uiValue > PHYLED_XPIC_SR_BIT_MAX )
    {
      iRslt = 2;
    }
    else
    {
      ptPortRam->ulDRAM_RX_SR_BIT = uiValue;
    }
    break;

  case PHYLED_XPIC_PARAM_RX_SR_INV:
    if( uiValue > 1 )
    {
      iRslt = 2;
    }
    else
    {
      ptPortRam->ulDRAM_RX_SR_INV = uiValue;
    }
    break;

  default:
    /* unknown parameter */
    iRslt = 1;
    break;
  }

  return iRslt;
}

void PhyledXpic_Start(void* pvUsr)
{
  pfnXpicStart(PHYLED_XPIC_INST, pvUsr);
}
