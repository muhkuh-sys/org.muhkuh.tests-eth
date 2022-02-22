#include "io.h"
#include "serial.h"
#include "usb_globals.h"
#include "usb_requests_cdc.h"
#include "common_func_netx50.h"
#include <stdbool.h>
#include <stdio.h>

#define NXHX_OUT_MIN 24
#define NXHX_OUT_CNT 4
#define NXHX_OUT_MSK 0xf

#define NXHX_IN_MIN  28
#define NXHX_IN_CNT  4
#define NXHX_IN_MSK  0xf


void io_output_init(void)
{
  unsigned int uiIdx;

  NX50_GPIO_SetLine(0);

  for(uiIdx = 0; uiIdx < NXHX_OUT_CNT; ++uiIdx)
  {
    NX50_GPIO_SetupMode(NXHX_OUT_MIN + uiIdx, NX50_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX50_GPIO_MODE_INVERT);
  }
}

unsigned int io_output_cnt(void)
{
  return NXHX_OUT_CNT;
}

void io_output_set(uint32_t ulOutput)
{
  NX50_GPIO_SetLine((ulOutput & NXHX_OUT_MSK) << NXHX_OUT_MIN);
}

void io_input_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < NXHX_IN_CNT; ++uiIdx)
  {
    NX50_GPIO_SetupMode(NXHX_IN_MIN + uiIdx, NX50_GPIO_MODE_INPUT_READ, NX50_GPIO_MODE_NOINVERT);
  }
}

unsigned int io_input_cnt(void)
{
  return NXHX_IN_CNT;
}

uint32_t io_input_get(void)
{
  return ((NX50_GPIO_GetIn() >> NXHX_IN_MIN) & NXHX_IN_MSK);
}

uint32_t io_address_get(void)
{
  return io_input_get();
}

void io_init(void)
{
  /* Configure MMIO pin multiplexer */
  NX50_MMIO_SetCfg(  4, MMIO_CONFIG_XC_TRIGGER0,         0, 0 );  /* Sync 0: X6.1 */
  NX50_MMIO_SetCfg(  5, MMIO_CONFIG_XC_TRIGGER1,         0, 0 );  /* Sync 1: X6.4  */
  NX50_MMIO_SetCfg(  6, MMIO_CONFIG_PIO6,                0, 0 );  /* Application trigger 0: X6.2 */
  NX50_MMIO_SetCfg(  7, MMIO_CONFIG_PIO7,                0, 0 );  /* Application trigger 1: X6.3 */
  NX50_MMIO_SetCfg(  8, MMIO_CONFIG_GPIO14,              0, 0 );  /* USB_DPOS */
  NX50_MMIO_SetCfg( 12, MMIO_CONFIG_XM0_IO0,             0, 0 );  /* ETH0 LINK */
  NX50_MMIO_SetCfg( 13, MMIO_CONFIG_XM0_IO1,             0, 0 );  /* ETH0 ACT  */
  NX50_MMIO_SetCfg( 14, MMIO_CONFIG_XM1_IO0,             0, 0 );  /* ETH1 LINK */
  NX50_MMIO_SetCfg( 15, MMIO_CONFIG_XM1_IO1,             0, 0 );  /* ETH1 ACT  */
  NX50_MMIO_SetCfg( 16, MMIO_CONFIG_GPIO15,              0, 0 );  /* MMCINS */
  NX50_MMIO_SetCfg( 24, MMIO_CONFIG_GPIO24,              0, 0 );  /* LED 0 */
  NX50_MMIO_SetCfg( 25, MMIO_CONFIG_GPIO25,              0, 0 );  /* LED 1 */
  NX50_MMIO_SetCfg( 26, MMIO_CONFIG_GPIO26,              0, 0 );  /* LED 2 */
  NX50_MMIO_SetCfg( 27, MMIO_CONFIG_GPIO27,              0, 0 );  /* LED 3 */
  NX50_MMIO_SetCfg( 28, MMIO_CONFIG_PIO0,                0, 0 );  /* RTE0 STA0 */
  NX50_MMIO_SetCfg( 29, MMIO_CONFIG_PIO1,                0, 0 );  /* RTE0 STA1 */
  NX50_MMIO_SetCfg( 30, MMIO_CONFIG_PIO2,                0, 0 );  /* RTE1 STA0 */
  NX50_MMIO_SetCfg( 31, MMIO_CONFIG_PIO3,                0, 0 );  /* RTE1 STA1 */
  NX50_MMIO_SetCfg( 32, MMIO_CONFIG_UART0_CTS,           0, 0 );
  NX50_MMIO_SetCfg( 33, MMIO_CONFIG_UART0_RTS,           0, 0 );
  NX50_MMIO_SetCfg( 34, MMIO_CONFIG_UART0_RXD,           0, 0 );
  NX50_MMIO_SetCfg( 35, MMIO_CONFIG_UART0_TXD,           0, 0 );
  NX50_MMIO_SetCfg( 36, MMIO_CONFIG_GPIO28,              0, 0 );  /* DIP SW1 */
  NX50_MMIO_SetCfg( 37, MMIO_CONFIG_GPIO29,              0, 0 );  /* DIP SW2 */
  NX50_MMIO_SetCfg( 38, MMIO_CONFIG_GPIO30,              0, 0 );  /* DIP SW3 */
  NX50_MMIO_SetCfg( 39, MMIO_CONFIG_GPIO31,              0, 0 );  /* DIP SW4 */
}

static int io_usb_poll(unsigned int uiPort)
{
  if( uiPort != 0 ) return EOF;
  return (usb_pollConnection() == USB_CDC_ConnectionState_Connected) ? 0 : EOF;
}

void io_serial_init(unsigned int uiPort)
{
  if( uiPort != 0 ) return;

  /* pull USB_D+ to high */
  NX50_GPIO_SetupMode(14, NX50_GPIO_MODE_OUTPUT_SET_TO_1, NX50_GPIO_MODE_NOINVERT);

  /* initialize the USB port */
  usb_init();

  serial_add(0, 0, io_usb_poll, usb_cdc_buf_rec_peek, usb_cdc_buf_rec_get, usb_cdc_buf_send_put, usb_cdc_buf_send_flush);
}
