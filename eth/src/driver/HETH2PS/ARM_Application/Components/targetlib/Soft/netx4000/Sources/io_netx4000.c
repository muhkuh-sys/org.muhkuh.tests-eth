#include "io.h"
#include "asic.h"
#include "serial.h"
#include "hal_usbcdc.h"
#include <string.h>

void io_serial_init(unsigned int uiPort)
{
  if( uiPort != 0 ) return;

  /* initialize the USB port */
  USBCDC_Init(0);

  /* wait at least 1us between initialization and enabling pull-up */
  sleep(1);

  /* enable Pull-Up */
  USBCDC_Connect(0);

  serial_add(uiPort, 0, NULL, USBCDC_BufRecPeek, USBCDC_BufRecGet, USBCDC_BufSendPut, USBCDC_BufSendFlush);
}
