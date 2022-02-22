#include "io.h"
#include "serial.h"
#include "netx51_pio.h"
#include "hal_usbcdc.h"
#include <string.h>

#define PIO_APPTRG_MIN 6
#define PIO_APPTRG_CNT 2


void io_apptrg_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < PIO_APPTRG_CNT; ++uiIdx)
  {
    NX51_PIO_SetOutput(uiIdx + PIO_APPTRG_MIN, 0);
    NX51_PIO_SetMode(uiIdx + PIO_APPTRG_MIN, NX51_PIO_MODE_OUT);
  }
}

void io_apptrg_set(unsigned int uiIdx, unsigned int uiVal)
{
  if( uiIdx >= PIO_APPTRG_CNT )
  {
    return;
  }

  NX51_PIO_SetOutput(PIO_APPTRG_MIN + uiIdx, uiVal);
}

void io_serial_init(unsigned int uiPort)
{
  if( uiPort != 0 ) return;

  /* initialize the USB port */
  USBCDC_Init(0);

  serial_add(uiPort, 0, NULL, USBCDC_BufRecPeek, USBCDC_BufRecGet, USBCDC_BufSendPut, USBCDC_BufSendFlush);
}
