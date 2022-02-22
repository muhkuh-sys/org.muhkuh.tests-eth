#include "io.h"
#include "netx51_gpio.h"
#include "netx51_mmio.h"

#define GPIO_OUT_MIN 24
#define GPIO_OUT_CNT  4

#define GPIO_IN_MIN  20
#define GPIO_IN_CNT   4
#define GPIO_IN_MSK ((1U << GPIO_IN_CNT) - 1U)


void io_output_init(void)
{
  unsigned int uiIdx;

  NX51_GPIO_SetLine(0);

  for(uiIdx = 0; uiIdx < GPIO_OUT_CNT; ++uiIdx)
  {
    NX51_GPIO_SetupMode(GPIO_OUT_MIN + uiIdx, NX51_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX51_GPIO_INVERT_INVERT);
  }
}

unsigned int io_output_cnt(void)
{
  return GPIO_OUT_CNT;
}

void io_output_set(uint32_t ulOutput)
{
  NX51_GPIO_SetLine((ulOutput & 0xf) << GPIO_OUT_MIN);
}

void io_input_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < GPIO_IN_CNT; ++uiIdx)
  {
    NX51_GPIO_SetupMode(GPIO_IN_MIN + uiIdx, NX51_GPIO_MODE_INPUT_READ, NX51_GPIO_INVERT_NOINVERT);
  }
}

unsigned int io_input_cnt(void)
{
  return GPIO_IN_CNT;
}

uint32_t io_input_get(void)
{
  return ((NX51_GPIO_GetIn() >> GPIO_IN_MIN) & GPIO_IN_MSK);
}

uint32_t io_address_get(void)
{
  return io_input_get();
}

void io_init(void)
{
  NX51_MMIO_SetCfg(  0, NX51_MMIO_CONFIG_XC_TRIGGER1,         0, 0 );  /* Sync 1: XC5.2 */
  NX51_MMIO_SetCfg(  1, NX51_MMIO_CONFIG_XC_TRIGGER0,         0, 0 );  /* Sync 0: XC5.1  */
  NX51_MMIO_SetCfg(  2, NX51_MMIO_CONFIG_XC_SAMPLE1,          0, 0 );  /* Latch 1: XC5.4 */
  NX51_MMIO_SetCfg(  3, NX51_MMIO_CONFIG_XC_SAMPLE0,          0, 0 );  /* Latch 0: XC5.3 */
  NX51_MMIO_SetCfg(  6, NX51_MMIO_CONFIG_PIO6,                0, 0 );  /* Application trigger 0: X6.2 */
  NX51_MMIO_SetCfg(  7, NX51_MMIO_CONFIG_PIO7,                0, 0 );  /* Application trigger 1: X6.3 */
  NX51_MMIO_SetCfg( 12, NX51_MMIO_CONFIG_PHY0_LED0,           1, 0 );  /* ETH 0 green LED Link/Activity */
  NX51_MMIO_SetCfg( 13, NX51_MMIO_CONFIG_PHY0_LED1,           1, 0 );  /* ETH 0 yellow LED unused */
  NX51_MMIO_SetCfg( 14, NX51_MMIO_CONFIG_PHY1_LED0,           1, 0 );  /* ETH 1 green LED Link/Activity */
  NX51_MMIO_SetCfg( 15, NX51_MMIO_CONFIG_PHY1_LED1,           1, 0 );  /* ETH 1 yellow LED unused  */
  NX51_MMIO_SetCfg( 16, NX51_MMIO_CONFIG_GPIO15,              0, 0 );  /* MMC_INS */
  NX51_MMIO_SetCfg( 24, NX51_MMIO_CONFIG_GPIO24,              0, 0 );  /* LED V12 (Output) */
  NX51_MMIO_SetCfg( 25, NX51_MMIO_CONFIG_GPIO25,              0, 0 );  /* LED V13 (Output) */
  NX51_MMIO_SetCfg( 26, NX51_MMIO_CONFIG_GPIO26,              0, 0 );  /* LED V14 (Output) */
  NX51_MMIO_SetCfg( 27, NX51_MMIO_CONFIG_GPIO27,              0, 0 );  /* LED V15 (Output) */
  NX51_MMIO_SetCfg( 28, NX51_MMIO_CONFIG_PIO0,                0, 0 );  /* COM 0 (low-active) LED Green */
  NX51_MMIO_SetCfg( 29, NX51_MMIO_CONFIG_PIO1,                0, 0 );  /* COM 0 (low-active) LED Red */
  NX51_MMIO_SetCfg( 30, NX51_MMIO_CONFIG_PIO2,                0, 0 );  /* COM 1 (low-active) LED Green */
  NX51_MMIO_SetCfg( 31, NX51_MMIO_CONFIG_PIO3,                0, 0 );  /* COM 1 (low-active) LED Red */
  NX51_MMIO_SetCfg( 32, NX51_MMIO_CONFIG_UART0_CTS,           0, 0 );
  NX51_MMIO_SetCfg( 33, NX51_MMIO_CONFIG_UART0_RTS,           0, 0 );
  NX51_MMIO_SetCfg( 34, NX51_MMIO_CONFIG_UART0_RXD,           0, 0 );
  NX51_MMIO_SetCfg( 35, NX51_MMIO_CONFIG_UART0_TXD,           0, 0 );
  NX51_MMIO_SetCfg( 36, NX51_MMIO_CONFIG_GPIO20,              0, 0 );  /* DIP Switch S30.1 (Input) */
  NX51_MMIO_SetCfg( 37, NX51_MMIO_CONFIG_GPIO21,              0, 0 );  /* DIP Switch S30.2 (Input) */
  NX51_MMIO_SetCfg( 38, NX51_MMIO_CONFIG_GPIO22,              0, 0 );  /* DIP Switch S30.3 (Input) */
  NX51_MMIO_SetCfg( 39, NX51_MMIO_CONFIG_GPIO23,              0, 0 );  /* DIP Switch S30.4 (Input) */
  NX51_MMIO_SetCfg( 40, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* SPI 1 MISO */
  NX51_MMIO_SetCfg( 41, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* SPI 1 MOSI */
  NX51_MMIO_SetCfg( 42, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* SPI 1 CS0  */
  NX51_MMIO_SetCfg( 43, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* SPI 1 CLK  */
  NX51_MMIO_SetCfg( 44, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* GPIO used to get IRQ from netX6 at rising edge */
}
