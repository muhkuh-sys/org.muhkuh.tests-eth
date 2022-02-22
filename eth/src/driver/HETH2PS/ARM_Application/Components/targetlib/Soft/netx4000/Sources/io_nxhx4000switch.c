#include "io.h"
#include "hal_gpio.h"
#include "hal_mmio.h"

#define GPIO_OUT_MIN  0
#define GPIO_OUT_CNT  4

#define GPIO_IN_MIN   4
#define GPIO_IN_CNT   4
#define GPIO_IN_MSK ((1U << GPIO_IN_CNT) - 1U)

void io_apptrg_init(void)
{
}

void io_apptrg_set(unsigned int uiIdx, unsigned int uiVal)
{
}

void io_output_init(void)
{
  unsigned int uiIdx;

  GPIO_SetLine(0);

  for(uiIdx = 0; uiIdx < GPIO_OUT_CNT; ++uiIdx)
  {
    GPIO_SetupMode(GPIO_OUT_MIN + uiIdx, 0, GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, GPIO_INVERT_INVERT);
  }
}

unsigned int io_output_cnt(void)
{
  return GPIO_OUT_CNT;
}

void io_output_set(uint32_t ulOutput)
{
  GPIO_SetLine((ulOutput & 0xf) << GPIO_OUT_MIN);
}

void io_input_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < GPIO_IN_CNT; ++uiIdx)
  {
    GPIO_SetupMode(GPIO_IN_MIN + uiIdx, 0, GPIO_MODE_INPUT_READ, GPIO_INVERT_NOINVERT);
  }
}

unsigned int io_input_cnt(void)
{
  return GPIO_IN_CNT;
}

uint32_t io_input_get(void)
{
  return ((GPIO_GetIn() >> GPIO_IN_MIN) & GPIO_IN_MSK);
}

uint32_t io_address_get(void)
{
  return io_input_get();
}

void io_init(void)
{
  MMIO_SetCfg( 5, MMIO_CONFIG_XM10_MII_MDIO,         0, 0); /* MDIO for extPHY0 (PHY Address = 0) */
  MMIO_SetCfg( 6, MMIO_CONFIG_XM10_MII_MDC,          0, 0); /* MDC for extPHY0 (PHY Address = 0) */
  MMIO_SetCfg(40, MMIO_CONFIG_XM11_MII_MDIO,         0, 0); /* MDC  for extPHY1 (PHY Address = 1) */
  MMIO_SetCfg(41, MMIO_CONFIG_XM11_MII_MDC,          0, 0); /* MDIO for extPHY1 (PHY Address = 1) */
  MMIO_SetCfg(42, MMIO_CONFIG_PIO5,                  0, 0); /* COM2.red */
  MMIO_SetCfg(43, MMIO_CONFIG_PIO4,                  0, 0); /* COM2.green */
  MMIO_SetCfg(44, MMIO_CONFIG_PIO7,                  0, 0); /* COM3.red */
  MMIO_SetCfg(45, MMIO_CONFIG_PIO6,                  0, 0); /* COM3.green */
  MMIO_SetCfg(55, MMIO_CONFIG_PIO1,                  0, 0); /* COM0.red */
  MMIO_SetCfg(56, MMIO_CONFIG_PIO0,                  0, 0); /* COM0.green */
  MMIO_SetCfg(57, MMIO_CONFIG_PIO3,                  0, 0); /* COM1.red */
  MMIO_SetCfg(58, MMIO_CONFIG_PIO2,                  0, 0); /* COM1.green */
  MMIO_SetCfg(59, MMIO_CONFIG_XC0_TRIGGER0,          0, 0); /* X1100 */
  MMIO_SetCfg(60, MMIO_CONFIG_XC0_TRIGGER1,          0, 0); /* X1102 */
  MMIO_SetCfg(61, MMIO_CONFIG_XC1_TRIGGER0,          0, 0); /* X1103 */
  MMIO_SetCfg(62, MMIO_CONFIG_XC1_TRIGGER1,          0, 0); /* X1104 */
  MMIO_SetCfg(64, MMIO_CONFIG_PHY2_LED_ACT,          0, 1); /* invert low-active BCM5241.activity */
  MMIO_SetCfg(65, MMIO_CONFIG_PHY2_LED_LNK,          0, 1); /* invert low-active BCM5241.link     */
  MMIO_SetCfg(66, MMIO_CONFIG_PHY3_LED_ACT,          0, 1); /* invert low-active BCM5241.activity */
  MMIO_SetCfg(67, MMIO_CONFIG_PHY3_LED_LNK,          0, 1); /* invert low-active BCM5241.link     */
  MMIO_SetCfg(70, MMIO_CONFIG_PHY0_LED_PHY_CTRL_ACT, 1, 0);
  MMIO_SetCfg(71, MMIO_CONFIG_PHY0_LED_PHY_CTRL_LNK, 1, 0);
  MMIO_SetCfg(72, MMIO_CONFIG_PHY1_LED_PHY_CTRL_ACT, 1, 0);
  MMIO_SetCfg(73, MMIO_CONFIG_PHY1_LED_PHY_CTRL_LNK, 1, 0);
}
