
/** This module combines all common function into a big module.
 *  It is recommended to use only the single netx51_*.c modules that are
 *  necessary for the application instead of this file.
 *  WARNING: Linking this module into your application might produce a vast overhead!
 *  It was only implemented for compatibility reasons.
 */

/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "common_func_netx51.h"
#include "defines.h"
#include "hal_resources_defines_netx51.h"

/* combine all common functions in this module */

#include "netx51_armtimer.c"
#undef  __USE_ARM_TIMER
#define __USE_ARM_TIMER

#include "netx51_asicctrl.c"
#undef  __USE_ASIC_CTRL
#define __USE_ASIC_CTRL

#include "netx51_bufman.c"

#include "netx51_canctrl.c"

#include "netx51_dmac.c"
#undef  __USE_DMAC_CH
#define __USE_DMAC_CH

#include "netx51_dpm.c"
#undef  __USE_DPM
#define __USE_DPM

#include "netx51_fmmusm.c"

#include "netx51_gpio.c"

#include "netx51_pio.c"

#include "netx51_hsctrl.c"

#include "netx51_hif_asyncmem_ctrl.c"

#include "netx51_hif_ioctrl.c"

#include "netx51_intphy.c"
#undef  __USE_INT_PHY_CTRL
#define __USE_INT_PHY_CTRL

#include "netx51_miimu.c"

#include "netx51_mmio.c"

#include "netx51_nfifo.c"

#include "netx51_pfifo.c"

#include "netx51_spi_motion.c"

#include "netx51_systime.c"

#include "netx51_trgsmpl.c"

#include "netx51_uart.c"

#include "netx51_usbdev.c"

#include "netx51_vic.c"

#include "netx51_xpecirq.c"
#undef  __USE_XPEC_IRQ_REGS
#define __USE_XPEC_IRQ_REGS

#include "netx51_xc.c"

#include "netx51_xpic.c"

