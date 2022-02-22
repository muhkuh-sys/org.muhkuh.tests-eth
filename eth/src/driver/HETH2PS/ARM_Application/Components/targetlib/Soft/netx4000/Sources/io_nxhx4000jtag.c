#include "io.h"
#include "hal_gpio.h"
#include "hal_mmio.h"

#define GPIO_OUT_MIN  0
#define GPIO_OUT_CNT  4

#define GPIO_IN_MIN   4
#define GPIO_IN_CNT   4
#define GPIO_IN_MSK ((1U << GPIO_IN_CNT) - 1U)

#define MMIO_APPTRG0 43
#define MMIO_APPTRG1 58
static const unsigned int s_auiMmioApptrg[] = {MMIO_APPTRG0, MMIO_APPTRG1};

void io_apptrg_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < 2; ++uiIdx)
  {
    MMIO_SetOutput(s_auiMmioApptrg[uiIdx], 0);
    MMIO_SetMode(s_auiMmioApptrg[uiIdx], MMIO_MODE_OUT);
  }
}

void io_apptrg_set(unsigned int uiIdx, unsigned int uiVal)
{
  if( uiIdx >= 2 )
  {
    return;
  }

  MMIO_SetOutput(s_auiMmioApptrg[uiIdx], uiVal);
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
  MMIO_SetCfg(45, MMIO_CONFIG_PHY0_LED_PHY_CTRL_ACT, 1, 0);
  MMIO_SetCfg(46, MMIO_CONFIG_PHY0_LED_PHY_CTRL_LNK, 1, 0);
  MMIO_SetCfg(51, MMIO_CONFIG_PHY1_LED_PHY_CTRL_ACT, 1, 0);
  MMIO_SetCfg(52, MMIO_CONFIG_PHY1_LED_PHY_CTRL_LNK, 1, 0);
  MMIO_SetCfg(53, MMIO_CONFIG_PIO0,                  0, 0);  /* COM0 GREEN */
  MMIO_SetCfg(54, MMIO_CONFIG_PIO1,                  0, 0);  /* COM0 RED */
  MMIO_SetCfg(55, MMIO_CONFIG_PIO2,                  0, 0);  /* COM1 GREEN */
  MMIO_SetCfg(56, MMIO_CONFIG_PIO3,                  0, 0);  /* COM1 RED */
  MMIO_SetCfg(43, MMIO_CONFIG_PIO_MODE,              0, 0);  /* Application trigger 0: X1602.2 */
  MMIO_SetCfg(58, MMIO_CONFIG_PIO_MODE,              0, 0);  /* Application trigger 1: X1602.3 */
  MMIO_SetCfg(22, MMIO_CONFIG_XC0_TRIGGER0,          0, 0);  /* X1601.9 */
  MMIO_SetCfg(23, MMIO_CONFIG_XC0_TRIGGER1,          0, 0);  /* X1601.10 */
  MMIO_SetCfg( 2, MMIO_CONFIG_GPIO0,                 0, 0);  /* LED (Output) */
  MMIO_SetCfg( 3, MMIO_CONFIG_GPIO1,                 0, 0);  /* LED (Output) */
  MMIO_SetCfg( 4, MMIO_CONFIG_GPIO2,                 0, 0);  /* LED (Output) */
  MMIO_SetCfg( 5, MMIO_CONFIG_GPIO3,                 0, 0);  /* LED (Output) */
  MMIO_SetCfg( 6, MMIO_CONFIG_GPIO4,                 0, 0);  /* DIP Switch S301.1 (Input) */
  MMIO_SetCfg( 7, MMIO_CONFIG_GPIO5,                 0, 0);  /* DIP Switch S301.2 (Input) */
  MMIO_SetCfg( 8, MMIO_CONFIG_GPIO6,                 0, 0);  /* DIP Switch S301.3 (Input) */
  MMIO_SetCfg( 9, MMIO_CONFIG_GPIO7,                 0, 0);  /* DIP Switch S301.4 (Input) */
}
