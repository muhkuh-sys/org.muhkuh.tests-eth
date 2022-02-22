#include "io.h"
#include "hal_gpio.h"
#include "hal_mmio.h"

#define GPIO_OUT_MIN  0
#define GPIO_OUT_CNT  4

#define GPIO_IN_MIN   4
#define GPIO_IN_CNT   4
#define GPIO_IN_MSK ((1U << GPIO_IN_CNT) - 1U)


#define MMIO_APPTRG0 91
#define MMIO_APPTRG1 90
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
  MMIO_SetCfg(28, MMIO_CONFIG_PHY0_LED_PHY_CTRL_LNK, 1, 0);
  MMIO_SetCfg(29, MMIO_CONFIG_PHY0_LED_PHY_CTRL_ACT, 1, 0);
  MMIO_SetCfg(30, MMIO_CONFIG_PHY1_LED_PHY_CTRL_LNK, 1, 0);
  MMIO_SetCfg(31, MMIO_CONFIG_PHY1_LED_PHY_CTRL_ACT, 1, 0);
  MMIO_SetCfg( 0, MMIO_CONFIG_PIO0,                  0, 0);  /* COM0 GREEN */
  MMIO_SetCfg( 1, MMIO_CONFIG_PIO1,                  0, 0);  /* COM0 RED */
  MMIO_SetCfg( 2, MMIO_CONFIG_PIO2,                  0, 0);  /* COM1 GREEN */
  MMIO_SetCfg( 3, MMIO_CONFIG_PIO3,                  0, 0);  /* COM1 RED */
  MMIO_SetCfg(38, MMIO_CONFIG_GPIO0,                 0, 0);  /* LED (Output) */
  MMIO_SetCfg(36, MMIO_CONFIG_GPIO1,                 0, 0);  /* LED (Output) */
  MMIO_SetCfg(37, MMIO_CONFIG_GPIO2,                 0, 0);  /* LED (Output) */
  MMIO_SetCfg(80, MMIO_CONFIG_GPIO3,                 0, 0);  /* LED (Output) */
  MMIO_SetCfg(12, MMIO_CONFIG_GPIO4,                 0, 0);  /* DIP Switch S1200.1 (Input) */
  MMIO_SetCfg(14, MMIO_CONFIG_GPIO5,                 0, 0);  /* DIP Switch S1200.2 (Input) */
  MMIO_SetCfg(15, MMIO_CONFIG_GPIO6,                 0, 0);  /* DIP Switch S1200.3 (Input) */
  MMIO_SetCfg(81, MMIO_CONFIG_GPIO7,                 0, 0);  /* DIP Switch S1200.4 (Input) */
  MMIO_SetCfg(95, MMIO_CONFIG_XC0_TRIGGER0,          0, 0);  /* Sync 0: X500.1 */
  MMIO_SetCfg(94, MMIO_CONFIG_XC0_TRIGGER1,          0, 0);  /* Sync 1: X500.3 */
  MMIO_SetCfg(91, MMIO_CONFIG_PIO_MODE,              0, 0);  /* Application trigger 0: X500.9 */
  MMIO_SetCfg(90, MMIO_CONFIG_PIO_MODE,              0, 0);  /* Application trigger 1: X500.11 */
}
