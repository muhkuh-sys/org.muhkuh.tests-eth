#include "io.h"
#include "common_func_netx500.h"
#include "usb_globals.h"
#include "usb_requests_cdc.h"
#include "asic.h"
#include "serial.h"
#include <stdbool.h>
#include <stdio.h>

#define NXHX_OUT_MIN 8
#define NXHX_OUT_CNT 4
#define NXHX_OUT_MSK 0xF

#define NXHX_IN_CNT  2
#define NXHX_IN_MSK  0x3


static unsigned int s_uiGpioInMin;
static bool s_fHasUsbPullDown;


void io_output_init(void)
{
  unsigned int uiIdx;

  NX500_GPIO_SetLine(0);

  for(uiIdx = 0; uiIdx < NXHX_OUT_CNT; ++uiIdx)
  {
    NX500_GPIO_SetupMode(uiIdx + NXHX_OUT_MIN, NX500_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, (s_uiGpioInMin == 13) ? NX500_GPIO_MODE_INVERT : NX500_GPIO_MODE_NOINVERT);
  }
}

unsigned int io_output_cnt(void)
{
  return NXHX_OUT_CNT;
}

void io_output_set(uint32_t ulOutput)
{
  NX500_GPIO_SetLine((ulOutput & NXHX_OUT_MSK) << NXHX_OUT_MIN);
}

void io_input_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < NXHX_IN_CNT; ++uiIdx)
  {
    NX500_GPIO_SetupMode(s_uiGpioInMin + uiIdx, NX500_GPIO_MODE_INPUT_READ, NX500_GPIO_MODE_NOINVERT);
  }
}

unsigned int io_input_cnt(void)
{
  return NXHX_IN_CNT;
}

uint32_t io_input_get(void)
{
  return ((NX500_GPIO_GetIn() >> s_uiGpioInMin) & NXHX_IN_MSK);
}

uint32_t io_address_get(void)
{
  return io_input_get();
}

void io_init(void)
{
  s_uiGpioInMin = 13;
  s_fHasUsbPullDown = true;

  /* detect old revision of NXHX board: Revision 0..2 have pull down at output LEDs */
  /* Probe GPIO 10 because this pin is not shared with X7 */
  NX500_GPIO_SetupMode(10, NX500_GPIO_MODE_INPUT_READ, NX500_GPIO_MODE_NOINVERT);
  if( 0 == (0x400 & NX500_GPIO_GetIn()) )
  {
    s_uiGpioInMin = 12;
    s_fHasUsbPullDown = false;
  }
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
  if(s_fHasUsbPullDown)
  {
    /* pull down USB_D+ (Bus Reset) */
    NX500_GPIO_SetupMode(12, NX500_GPIO_MODE_OUTPUT_SET_TO_0, NX500_GPIO_MODE_NOINVERT);
    sleep(10);
  }

  /* initialize the USB port */
  usb_init();

  if(s_fHasUsbPullDown)
  {
    /* pull up USB_D+ (Release Bus Reset) */
    NX500_GPIO_SetupMode(12, NX500_GPIO_MODE_OUTPUT_SET_TO_1, NX500_GPIO_MODE_NOINVERT);
  }

  serial_add(0, 0, io_usb_poll, usb_cdc_buf_rec_peek, usb_cdc_buf_rec_get, usb_cdc_buf_send_put, usb_cdc_buf_send_flush);
}
