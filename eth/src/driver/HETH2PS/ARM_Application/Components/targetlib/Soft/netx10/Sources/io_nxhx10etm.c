#include "io.h"
#include "serial.h"
#include "common_func_netx10.h"

#define MMIO_OUT_MIN  8
#define MMIO_OUT_CNT  4
#define MMIO_OUT_MSK ((1U << MMIO_OUT_CNT) - 1U)

#define MMIO_IN_MIN   4
#define MMIO_IN_CNT   4
#define MMIO_IN_MSK ((1U << MMIO_IN_CNT) - 1U)


void io_output_init(void)
{
  unsigned int uiIdx;

  NX10_MMIO_SetLine(0);

  for(uiIdx = 0; uiIdx < MMIO_OUT_CNT; ++uiIdx)
  {
    NX10_MMIO_SetMode(MMIO_OUT_MIN + uiIdx, NX10_MMIO_MODE_OUT);
  }
}

unsigned int io_output_cnt(void)
{
  return MMIO_OUT_CNT;
}

void io_output_set(uint32_t ulOutput)
{
  NX10_MMIO_SetLine((ulOutput & MMIO_OUT_MSK) << MMIO_OUT_MIN);
}

void io_input_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < MMIO_IN_CNT; ++uiIdx)
  {
    NX10_MMIO_SetMode(MMIO_OUT_MIN + uiIdx, NX10_MMIO_MODE_IN);
  }
}

unsigned int io_input_cnt(void)
{
  return MMIO_IN_CNT;
}

uint32_t io_input_get(void)
{
  return ((NX10_MMIO_GetLine() >> MMIO_IN_MIN) & MMIO_IN_MSK);
}

uint32_t io_address_get(void)
{
  return io_input_get();
}

void io_init(void)
{
  NX10_MMIO_SetCfg(16, NX10_MMIO_CONFIG_XM0_IO0,             0, 0 );  /* ETH 0 green LED Link */
  NX10_MMIO_SetCfg(17, NX10_MMIO_CONFIG_XM0_IO1,             0, 0 );  /* ETH 0 red LED Activity */
  NX10_MMIO_SetCfg(20, NX10_MMIO_CONFIG_UART0_RXD,           0, 0 );  /* UART0.RXD: X7.2 */
  NX10_MMIO_SetCfg(21, NX10_MMIO_CONFIG_UART0_TXD,           0, 0 );  /* UART0.TXD: X7.1 */
  NX10_MMIO_SetCfg(22, NX10_MMIO_CONFIG_GPIO7,               0, 0 );  /* MMC_INS */
  NX10_MMIO_SetCfg(23, NX10_MMIO_CONFIG_SPI0_CS1N,           0, 0 );  /* SPI0.CS1 */
}

void io_serial_init(unsigned int uiPort)
{
  unsigned int uiImpl;

  if(uiPort < 2)
  {
    uiImpl = uiPort;
    NX10_UART_InitSerial(uiImpl);
    serial_add(uiPort, uiImpl, NULL, NX10_UART_Peek, NX10_UART_GetCharacter, NX10_UART_PutCharacter, NULL);
  }
}
