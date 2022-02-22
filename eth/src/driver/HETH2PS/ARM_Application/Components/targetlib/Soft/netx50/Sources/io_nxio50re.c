#include "io.h"
#include "serial.h"
#include "usb_globals.h"
#include "usb_requests_cdc.h"
#include "common_func_netx50.h"
#include <stdbool.h>
#include <stdio.h>

#define NXIO_OUT_CNT 32
#define NXIO_OUT_MSK 0xffffffffUL

#define NXIO_IN_MIN  8
#define NXIO_IN_CNT  16
#define NXIO_IN_MSK  0xffff

void io_output_init(void)
{
  NX50_HIF_SetPioOut(0, 0);

  /* configure PIO 32 - 63 as outputs (set OE to 1) */
  NX50_HIF_SetPioOe(0, 0xffffffff);
}

unsigned int io_output_cnt(void)
{
  return NXIO_OUT_CNT;
}

void io_output_set(uint32_t ulOutput)
{
  NX50_HIF_SetPioOut(0, ulOutput);
}

void io_input_init(void)
{
  /* configure PIO 64 - 84 as inputs (set OE to 0) */
  NX50_HIF_SetPioOe(1, 0);
}

unsigned int io_input_cnt(void)
{
  return NXIO_IN_CNT;
}

uint32_t io_input_get(void)
{
  return ((NX50_HIF_GetPioIn(1) & 0xf00) >> 8);
}

uint32_t io_address_get(void)
{
  uint32_t ulId = 0xff ^ NX50_HIF_GetPioIn(1);
  return 0xff & ((ulId << 4) | (ulId >> 4));
}

void io_init(void)
{
  /* Configure MMIO pin multiplexer */
  NX50_MMIO_SetCfg(  8, MMIO_CONFIG_PIO6,                0, 0 );  /* Application trigger 0: X30.7 */
  NX50_MMIO_SetCfg(  9, MMIO_CONFIG_PIO7,                0, 0 );  /* Application trigger 1: X30.8 */
  NX50_MMIO_SetCfg( 10, MMIO_CONFIG_XC_TRIGGER0,         0, 0 );  /* Sync 0: X30.9  */
  NX50_MMIO_SetCfg( 11, MMIO_CONFIG_XC_TRIGGER1,         0, 0 );  /* Sync 1: X30.10 */
  NX50_MMIO_SetCfg( 12, MMIO_CONFIG_XM0_IO0,             0, 0 );  /* ETH0 LINK */
  NX50_MMIO_SetCfg( 13, MMIO_CONFIG_XM0_IO1,             0, 0 );  /* ETH0 ACT  */
  NX50_MMIO_SetCfg( 14, MMIO_CONFIG_XM1_IO0,             0, 0 );  /* ETH1 LINK */
  NX50_MMIO_SetCfg( 15, MMIO_CONFIG_XM1_IO1,             0, 0 );  /* ETH1 ACT  */
  NX50_MMIO_SetCfg( 23, MMIO_CONFIG_GPIO15,              0, 0 );  /* MMCINS */
  NX50_MMIO_SetCfg( 28, MMIO_CONFIG_PIO0,                0, 0 );  /* RTE0 STA0 */
  NX50_MMIO_SetCfg( 29, MMIO_CONFIG_PIO1,                0, 0 );  /* RTE0 STA1 */
  NX50_MMIO_SetCfg( 30, MMIO_CONFIG_PIO2,                0, 0 );  /* RTE1 STA0 */
  NX50_MMIO_SetCfg( 31, MMIO_CONFIG_PIO3,                0, 0 );  /* RTE1 STA1 */
  NX50_MMIO_SetCfg( 38, MMIO_CONFIG_GPIO14,              0, 0 );  /* USB_DPOS */

  /* Configure HIF to I/O mode */
  NX50_HIF_SetHifMode(NX50_HIF_MODE_IO);

  /* Configure all HIF I/O pins as PIOs */
  NX50_HIF_SetIoMode(0, 0);
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
