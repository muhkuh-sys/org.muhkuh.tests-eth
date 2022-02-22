#include "io.h"
#include "asic.h"
#include "hal_mmio.h"
#include "hal_rap_portctrl.h"
#include "hal_phyctrl.h"
#include "hal_asicctrl.h"
#include "hw_defines.h"
#include "phy.h"

typedef struct MMIO_Ttag
{
  uint8_t bMmio;
  uint8_t bSel;
  uint8_t bInvOut;
  uint8_t bInvIn;
} MMIO_T;


void io_apptrg_init(void)
{
}

void io_apptrg_set(unsigned int uiIdx, unsigned int uiVal)
{
}

void io_output_init(void)
{
}

unsigned int io_output_cnt(void)
{
  return 0;
}

void io_output_set(uint32_t ulOutput)
{
}

void io_input_init(void)
{
}

unsigned int io_input_cnt(void)
{
  return 0;
}

uint32_t io_input_get(void)
{
  return 0;
}

uint32_t io_address_get(void)
{
  return 0;
}

void io_init(void)
{
  unsigned int uiIdx;
  static const MMIO_T atMmioCfg[] =
  {
    {  40, MMIO_CONFIG_XM10_MII_MDIO,         0, 0 }, /* MDIO for extPHY0 (PHY Address = 0) */
    {   9, MMIO_CONFIG_XM10_MII_MDC,          0, 0 }, /* MDC  for extPHY0 (PHY Address = 0) */
    {  41, MMIO_CONFIG_XM11_MII_MDIO,         0, 0 }, /* MDIO for extPHY1 (PHY Address = 1) */
    {  13, MMIO_CONFIG_XM11_MII_MDC,          0, 0 }, /* MDC  for extPHY1 (PHY Address = 1) */

    {  56, MMIO_CONFIG_PHY0_LED_PHY_CTRL_LNK, 1, 0 },
    {  57, MMIO_CONFIG_PHY0_LED_PHY_CTRL_ACT, 1, 0 },
    {  58, MMIO_CONFIG_PHY1_LED_PHY_CTRL_LNK, 1, 0 },
    {  59, MMIO_CONFIG_PHY1_LED_PHY_CTRL_ACT, 1, 0 },
    {  42, MMIO_CONFIG_PHY2_LED_PHY_CTRL_LNK, 1, 0 },
    {   1, MMIO_CONFIG_PHY2_LED_PHY_CTRL_ACT, 1, 0 },
    {   2, MMIO_CONFIG_PHY3_LED_PHY_CTRL_LNK, 1, 0 },
    {   3, MMIO_CONFIG_PHY3_LED_PHY_CTRL_ACT, 1, 0 },

    {  45, MMIO_CONFIG_PHY2_LED_LNK,          0, 1 }, /* invert low-active PHY.link input */
    {  50, MMIO_CONFIG_PHY3_LED_LNK,          0, 1 }, /* invert low-active PHY.link input */

    {  60, MMIO_CONFIG_PIO1,                  0, 0 }, /* COM0.red   */
    {  61, MMIO_CONFIG_PIO0,                  0, 0 }, /* COM0.green */
    {  62, MMIO_CONFIG_PIO3,                  0, 0 }, /* COM1.red   */
    {  63, MMIO_CONFIG_PIO2,                  0, 0 }, /* COM1.green */
    {   4, MMIO_CONFIG_PIO5,                  0, 0 }, /* COM2.red   */
    {   5, MMIO_CONFIG_PIO4,                  0, 0 }, /* COM2.green */
    {   6, MMIO_CONFIG_PIO7,                  0, 0 }, /* COM3.red   */
    {   7, MMIO_CONFIG_PIO6,                  0, 0 }, /* COM3.green */

    {  54, MMIO_CONFIG_XC0_TRIGGER0,          0, 0 }, /* XC0.Sync0 (X1606.2) */
    {  53, MMIO_CONFIG_XC0_TRIGGER1,          0, 0 }, /* XC0.Sync1 (X1606.3) */
    {  52, MMIO_CONFIG_XC1_TRIGGER0,          0, 0 }, /* XC1.Sync0 (X1605.2) */
    {  55, MMIO_CONFIG_XC1_TRIGGER1,          0, 0 }, /* XC1.Sync1 (X1605.3) */
  };

  static const uint32_t aulPortCtrl[] =
  {
    /* Disable pull resistors at external MII, drive all MII outputs with high current */
  #define RAP_PORT_CTRL_CMOSIO_DRV_MII RAP_PORT_CTRL_CMOSIO_DRV_8MA
    /* MMIO08:MII2_RXER */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P3_8,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO09:MII2_MDC  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P3_9,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_8MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO10:MII2_COL  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P3_10, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO11:MII2_CRS  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P3_11, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO12:MII3_RXER */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P3_12, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO13:MII3_MDC  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P3_13, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_8MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO14:MII3_COL  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P3_14, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO15:MII3_CRS  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P3_15, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO16:MII2_RXC  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_0,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO17:MII2_RXD0 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_1,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO18:MII2_RXD1 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_2,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO19:MII2_RXD2 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_3,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO20:MII2_RXD3 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_4,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO21:MII2_RXDV */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_5,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO22:MII2_TXC  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_6,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO23:MII2_TXD0 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_7,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO24:MII2_TXD1 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_8,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO25:MII2_TXD2 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_9,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO26:MII2_TXD3 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_10, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO27:MII2_TXEN */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_11, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO28:MII3_RXC  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_12, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO29:MII3_RXD0 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_13, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO30:MII3_RXD1 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_14, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO31:MII3_RXD2 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P4_15, 0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO32:MII3_RXD3 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_0,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO33:MII3_RXDV */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_1,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO34:MII3_TXC  */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_2,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_4MA, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO35:MII3_TXD0 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_3,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO36:MII3_TXD1 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_4,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO37:MII3_TXD2 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_5,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO38:MII3_TXD3 */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_6,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),
    /* MMIO39:MII3_TXEN */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_7,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_MII, RAP_PORT_CTRL_CMOSIO_UDC_NONE),

    /* Enable pull-up at MDIO pins */
    /* MMIO40:MII2_MDIO */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_8,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_8MA, RAP_PORT_CTRL_CMOSIO_UDC_PU50K),
    /* MMIO41:MII3_MDIO */ RAP_PORTCTRL_CFG(RAP_PORTCTRL_PORT_P5_9,  0, 0, RAP_PORT_CTRL_CMOSIO_DRV_8MA, RAP_PORT_CTRL_CMOSIO_UDC_PU50K),
  };

  RAP_PortCtrl_SetCfg(aulPortCtrl, (sizeof(aulPortCtrl)/sizeof(aulPortCtrl[0])));

  for(uiIdx = 0; uiIdx < (sizeof(atMmioCfg)/sizeof(atMmioCfg[0])); ++uiIdx)
  {
    const MMIO_T* ptCfg = &atMmioCfg[uiIdx];
    MMIO_SetCfg((unsigned int)ptCfg->bMmio,
                (MMIO_CONFIG_E)ptCfg->bSel,
                (unsigned int)ptCfg->bInvOut,
                (unsigned int)ptCfg->bInvIn);
  }
}

/* read external PHY register */
unsigned int extphy_read(unsigned int uiPhy, unsigned int uiReg)
{
  unsigned int uiPhyCtrlInst = uiPhy + 2; /* ExtPhy0/1 connected at PhyCtrl2/3 has address 0/1 */
  return PHYCTRL_MiimRead(uiPhyCtrlInst, uiPhy, uiReg);
}

/* write external PHY register */
void extphy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  unsigned int uiPhyCtrlInst = uiPhy + 2; /* ExtPhy0/1 connected at PhyCtrl2/3 has address 0/1 */
  PHYCTRL_MiimWrite(uiPhyCtrlInst, uiPhy, uiReg, uiValue);
}

void extphy_init(void)
{
  unsigned int uiPhyAdr;

  /* Connect XC1 to external PHY, no TX_ER */
  AsicCtrl_SetIoConfig(2, HW_BFSET(io_config2_sel_xm10_mii, 6)
                        | HW_BFSET(io_config2_sel_xm11_mii, 6)
                      );

  /* apply reset to external PHYs */
  AsicCtrl_SetResetOut(true, true); /* drive RESET_OUT low */

  /* Wait 1ms (reset pulse) */
  sleep(1);

  /* release reset */
  AsicCtrl_SetResetOut(false, true); /* drive RESET_OUT high */

  /* Wait 2ms for PHY recovers from reset */
  sleep(2);

  for(uiPhyAdr = 0; uiPhyAdr < 2; ++uiPhyAdr)
  {
    unsigned int uiPort = uiPhyAdr + 2;
    PHYCTRL_Init(uiPort);
  }

  phy_add(2, 0, extphy_write, extphy_read);
  phy_add(3, 1, extphy_write, extphy_read);
}
