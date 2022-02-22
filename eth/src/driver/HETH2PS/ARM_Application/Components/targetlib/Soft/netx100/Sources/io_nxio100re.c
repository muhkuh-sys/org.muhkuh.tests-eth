#include "io.h"
#include "common_func_netx500.h"
#include "usb_globals.h"
#include "usb_requests_cdc.h"
#include "serial.h"
#include <stdbool.h>
#include <stdio.h>

#define NXIO_OUT_MIN 0
#define NXIO_OUT_CNT 16
#define NXIO_OUT_MSK 0xFFFF

#define NXIO_IN_MIN  8
#define NXIO_IN_CNT  16
#define NXIO_IN_MSK  0xFFFF


void io_output_init(void)
{
  unsigned int uiIdx;

  NX500_GPIO_SetLine(0);

  for(uiIdx = 0; uiIdx < NXIO_OUT_CNT; ++uiIdx)
  {
    NX500_GPIO_SetupMode(NXIO_OUT_MIN + uiIdx, NX500_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX500_GPIO_MODE_INVERT);
  }
}

unsigned int io_output_cnt(void)
{
  return NXIO_OUT_CNT;
}

void io_output_set(uint32_t ulOutput)
{
  NX500_GPIO_SetLine(ulOutput);
}

void io_input_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < NXIO_IN_CNT; ++uiIdx)
  {
    NX500_PIO_SetMode(NXIO_IN_MIN + uiIdx, NX500_PIO_MODE_IN);
  }
}

unsigned int io_input_cnt(void)
{
  return NXIO_IN_CNT;
}

uint32_t io_input_get(void)
{
  return ((NX500_PIO_GetIn() >> 8) & NXIO_IN_MSK);
}

uint32_t io_address_get(void)
{
  unsigned long ulHifPioLo, ulHifPioHi;

  NX500_HIF_SetIoMode(0, 0);
  NX500_HIF_GetPioIn(&ulHifPioLo, &ulHifPioHi);

  return ulHifPioLo & 0xff;
}

static int io_usb_poll(unsigned int uiPort)
{
  if( uiPort != 0 ) return EOF;
  return (usb_pollConnection() == USB_CDC_ConnectionState_Connected) ? 0 : EOF;
}

void io_serial_init(unsigned int uiPort)
{
  if( uiPort != 0 ) return;

  /* initialize the USB port */
  usb_init();

  serial_add(0, 0, io_usb_poll, usb_cdc_buf_rec_peek, usb_cdc_buf_rec_get, usb_cdc_buf_send_put, usb_cdc_buf_send_flush);
}

void io_init(void)
{
}
