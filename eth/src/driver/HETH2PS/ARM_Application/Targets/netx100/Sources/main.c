/****************************************************************************************
*  I/O Hardware Abstraction Layer
*
*  This module implements the low level I/O functions, e.g. LED control.
****************************************************************************************/

#include "sys.h"
#include "targetlib.h"

/****************************************************************************************
*  Functions
****************************************************************************************/

/* Initialize I/O interfaces and peripheral hardware */
int main(void)
{
  asic_init();
  io_init();
  io_input_init();
  io_output_init();
  io_serial_init(0);
  led_com_init(0);
  led_com_init(1);
  intphy_init();

  g_tSysCfg.uiSystime = 0;

  main_task();

  return 0;
}
