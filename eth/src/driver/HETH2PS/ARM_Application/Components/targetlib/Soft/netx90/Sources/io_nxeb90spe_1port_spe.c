/**
 * IO-configuration for NXEB 90-SPE
 */

#include "io.h"
#include "asic.h"
#include "hal_mmio.h"
#include "hal_padctrl.h"
#include "hal_asicctrl.h"
#include "hal_mled_ctrl.h"
#include "regdef_netx90_arm_com.h"

#define CONCAT(a,b) a##b
#define BF(name, val) ((uint32_t)val << CONCAT(SRT_NX90_, name))
#define BM(name) (CONCAT(MSK_NX90_, name))

#define MMIO_OUT_MIN  4
#define MMIO_OUT_CNT  4
#define MMIO_OUT_MSK ((1U << MMIO_OUT_CNT) - 1U)

#define MMIO_IN_MIN   0
#define MMIO_IN_CNT   4
#define MMIO_IN_MSK ((1U << MMIO_IN_CNT) - 1U)


static uint32_t const s_aulPadCtrlTable[] =
{
  /*          PORT                     IE DS              PE */

  /* set driver strength to 8mA for brighter LEDs */
  PADCTRL_CFG(PAD_CTRL_PORT_MLED0,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED1,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED2,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED3,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),

  /* enable inputs and outputs */
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO0,     1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO1,     1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO2,     1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO3,     1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),

  PADCTRL_CFG(PAD_CTRL_PORT_MMIO4,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO5,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO6,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MMIO7,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),

  /* Enable inputs at MII1.RXD */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD0, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD1, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD2, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD3, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXDV, 1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXCLK,1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),
  PADCTRL_CFG(PAD_CTRL_PORT_COM_IO0,   1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),  /* MII1.RX_ER muxed to COM_IO0 with sel_extphy */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXCLK,1, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),

  /* Disable input of COL/CRS pins as they assert FCS and alignment errors
   * at the MAC (even in full-duplex mode) */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_RDN,   0, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),  /* MII1.COL muxed to HIF_RDN with sel_extphy */
  PADCTRL_CFG(PAD_CTRL_PORT_COM_IO1,   0, PADCTRL_DS_4MA, PADCTRL_PE_PD50K),  /* MII1.CRS muxed to COM_IO1 with sel_extphy */

  /* Use high output driver strength of 8mA to achieve small rise and fall times.
   * Errors have been observed with 4mA drivers! */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD0, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD1, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD2, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXD3, 0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_TXEN, 1, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* Activate IE of MII1.TX_EN to feed this signal
                                                                                 back to the MLED blink unit for activity LED */

  /* Added for software MDIO */
  PADCTRL_CFG(PAD_CTRL_PORT_MII_MDIO,  1, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MII_MDC,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),

  /* Added for interrupt on MMIO17 (INT_N of extPHY) */
  // HIF_DIRQ input enable is enabled by default, as well as pull up enable
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_DIRQ,  1, PADCTRL_DS_4MA, PADCTRL_PE_PU50K),

  /* External SD-RAM */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A0,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A0 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A1,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A1 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A2,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A2 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A3,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A3 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A4,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A4 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A5,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A5 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A6,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A6 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A7,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A7 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A8,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A8 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A9,    0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A9 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A10,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A10 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A11,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A11 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A12,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_A12 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A13,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_BA0 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A14,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_BA1 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A15,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_RAS_N */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A16,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_CAS_N */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_A17,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_DQM0 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_BHEN,  0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_DQM1 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_CSN,   0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),   /* SD_CS_N */

  PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXER, 1, PADCTRL_DS_8MA, PADCTRL_PE_PD50K),  /* SD_D0 */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_CRS,  1, PADCTRL_DS_8MA, PADCTRL_PE_PD50K),  /* SD_D1 */
  PADCTRL_CFG(PAD_CTRL_PORT_MII1_COL,  1, PADCTRL_DS_8MA, PADCTRL_PE_PD50K),  /* SD_D2 */
  PADCTRL_CFG(PAD_CTRL_PORT_PHY0_LED_LINK_IN, 1, PADCTRL_DS_8MA, PADCTRL_PE_PD50K),  /* SD_D3 */
  PADCTRL_CFG(PAD_CTRL_PORT_PHY1_LED_LINK_IN, 1, PADCTRL_DS_8MA, PADCTRL_PE_PD50K),  /* SD_D4 */
  PADCTRL_CFG(PAD_CTRL_PORT_MII0_TXEN, 1, PADCTRL_DS_8MA, PADCTRL_PE_PD50K),  /* SD_D5 */
  PADCTRL_CFG(PAD_CTRL_PORT_MII0_COL,  1, PADCTRL_DS_8MA, PADCTRL_PE_PD50K),  /* SD_D6 */
  PADCTRL_CFG(PAD_CTRL_PORT_MII0_CRS,  1, PADCTRL_DS_8MA, PADCTRL_PE_PD50K),  /* SD_D7 */

  PADCTRL_CFG(PAD_CTRL_PORT_HIF_D0,    1, PADCTRL_DS_8MA, PADCTRL_PE_PU50K),  /* SD_D8 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_D1,    1, PADCTRL_DS_8MA, PADCTRL_PE_PU50K),  /* SD_D9 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_D2,    1, PADCTRL_DS_8MA, PADCTRL_PE_PU50K),  /* SD_D10 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_D3,    1, PADCTRL_DS_8MA, PADCTRL_PE_PU50K),  /* SD_D11 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_D4,    1, PADCTRL_DS_8MA, PADCTRL_PE_PU50K),  /* SD_D12 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_D5,    1, PADCTRL_DS_8MA, PADCTRL_PE_PU50K),  /* SD_D13 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_D6,    1, PADCTRL_DS_8MA, PADCTRL_PE_PU50K),  /* SD_D14 */
  PADCTRL_CFG(PAD_CTRL_PORT_HIF_D7,    1, PADCTRL_DS_8MA, PADCTRL_PE_PU50K),  /* SD_D15 */
};

/**
 * Initialize outputs
 */
void io_output_init(void)
{
  MMIO_PioOutLineCfgRst(MMIO_OUT_MSK << MMIO_OUT_MIN, 0, 0, 0);
  MMIO_SetOeLine(MMIO_OUT_MSK << MMIO_OUT_MIN, 0, 0, 0);
}

/**
 * Get board output count
 * \return unsigned int Number of user-definable outputs
 */
unsigned int io_output_cnt(void)
{
  return MMIO_OUT_CNT;
}

/**
 * Set states of all four user-definable outputs on MMIO7..4
 * \param uint32_t ulOutput    [in]   States of MMIO[7:4]
 */
void io_output_set(uint32_t ulOutput)
{
  ulOutput &= MMIO_OUT_MSK;
  MMIO_PioOutLineCfgSet(( ulOutput) << MMIO_OUT_MIN, 0, 0, 0);
  MMIO_PioOutLineCfgRst((~ulOutput) << MMIO_OUT_MIN, 0, 0, 0);
}

/**
 * Initialize inputs
 */
void io_input_init(void)
{
}

/**
 * Get board input count
 * \return unsigned int Number of user-definable inputs
 */
unsigned int io_input_cnt(void)
{
  return MMIO_IN_CNT;
}

/**
 * Get status of all four user definable inputs on MMIO0..3
 * \return uint32_t Input states of MMIO[3:0]
 */
uint32_t io_input_get(void)
{
  return ((MMIO_GetInLine(0) >> MMIO_IN_MIN) & MMIO_IN_MSK);
}

/**
 * \see io_input_get()
 */
uint32_t io_address_get(void)
{
  return io_input_get();
}

/**
 * Initialize IO, PAD_CTRL, select *_extphy pinning and put external PHY in reset
 */
void io_init(void)
{
  /* Set PAD_CTRL with the configuration table above */
  if( PADCTRL_RESULT_OK != PadCtrl_SetCfgTable(s_aulPadCtrlTable) )
  {
    while(1);
  }

  /* Map MLED2 to internal PHY LEDs of port 0 */
  MLEDCTRL_ConfigureOutput(0, 4, MLED_CTRL_SEL_PHY_CTRL0_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 5, MLED_CTRL_SEL_PHY_CTRL0_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);

  /* Map MLED3 to PHY LEDs of port 1, but only led1 (activity). LED0 (link status) is controlled
   * via software, as the pins PHY_LED_LINK_INx (link status of external PHY) are occupied with
   * SD-RAM */
  /* Activity LED is generated with external MII's TX_EN and RX_DV signals in MLED blink unit */
  MLEDCTRL_ConfigureOutput(0, 6, MLED_CTRL_SEL_PHY_CTRL1_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 7, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);

  /* Select line-mode for MLED[1:0] */
  MLEDCTRL_ConfigureOutput(0, 0, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 1, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 2, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 3, MLED_CTRL_SEL_LINE_REGISTER, false, MLED_CTRL_BRIGHTNESS_MAX);

  MLEDCTRL_SetLine(0, 0x0);
  MLEDCTRL_Start(0, 3, 0);

  /* Connect XC to external PHY, no TX_ER */
  AsicCtrl_SetIoConfig(2, BF(io_config1_sel_xm1_mii_cfg, 5)       /* Select full MII1 with COL / CRS  */
                        | BF(io_config1_sel_xc1_mdio, 0b01));     /* Connect MDIO of xC1 to external MII_MDIO  */


  AsicCtrl_SetIoConfig(9, BF(io_config8_sel_extphy, 0b1));        /* Select *_extphy pins in pinning table
                                                                     to enable the use of SD-RAM with an
                                                                     external PHY */

  /* Uncomment this line only if the crystal of the external PHY is not populated on the board
   * and the netX 90 should be used as a clock source for the external PHY  */
  // AsicCtrl_SetIoConfig(3, BF(io_config2_clk25out_oe, 0x1));

  /* Apply reset to external PHYs */
  AsicCtrl_SetResetOut(true, true);

  /* Wait 1ms (creates the minimum length for a reset pulse) */
  sleep(1);

  /* Initialize IO */
  io_output_init();
  io_input_init();
}
