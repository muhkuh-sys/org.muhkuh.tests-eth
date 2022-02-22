/****************************************************************************************
*  I/O Hardware Abstraction Layer
*
*  This module implements the low level I/O functions, e.g. LED control.
****************************************************************************************/

#include "sys.h"
#include "targetlib.h"

#if defined(HAL_TARGET_NX6)
#include "asic_netx6.h"
#endif

/****************************************************************************************
*  Definitions
****************************************************************************************/

/* Select target hardware by defining TARGET_HW */
/* Possible values for TARGET_HW */
#define _NXHX51_ETM_     0

/* Set default hardware */
#ifndef TARGET_HW
#define TARGET_HW _NXHX51_ETM_
#endif

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

  /* initialize HAL target board */
#if defined(HAL_TARGET_NX6)
  netx6_init();
  netx6_phy_init();
#else
  intphy_init();
#endif

  g_tSysCfg.uiSystime = 0;

  main_task();

  return 0;
}
