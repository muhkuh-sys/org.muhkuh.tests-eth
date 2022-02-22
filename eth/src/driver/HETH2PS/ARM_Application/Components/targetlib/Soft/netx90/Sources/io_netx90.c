#include "io.h"
#include "serial.h"
#include "hal_uart.h"
#include "hal_mmio.h"
#include <string.h>

void io_serial_init(unsigned int uiPort)
{
  if( uiPort != 0 ) return;

  /* initialize the STDOUT port */
  UART_Init(0, 115200, 8, 0, 1);

  serial_add(uiPort, 0, NULL, UART_Peek, UART_GetCharacter, UART_PutCharacter, NULL);
}
