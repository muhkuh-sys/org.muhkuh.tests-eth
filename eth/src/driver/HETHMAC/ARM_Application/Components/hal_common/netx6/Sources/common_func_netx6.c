
/** This module combines all common function into a big module.
 *  It is recommended to use only the single netx6_*.c modules that are
 *  necessary for the application instead of this file.
 *  WARNING: Linking this module into your application might produce a vast overhead!
 *  It was only implemented for compatibility reasons.
 */

/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "common_func_netx6.h"
#include "defines.h"
#include "hal_resources_defines_netx6.h"

/* combine all common functions in this module */

#include "netx6_asicctrl.c"
#undef  __USE_ASIC_CTRL
#define __USE_ASIC_CTRL

#include "netx6_bufman.c"

#include "netx6_dpm.c"
#undef  __USE_DPM
#define __USE_DPM

#include "netx6_fmmusm.c"

#include "netx6_gpio.c"

#include "netx6_pio.c"

#include "netx6_intphy.c"
#undef  __USE_INT_PHY_CTRL
#define __USE_INT_PHY_CTRL

#include "netx6_miimu.c"

#include "netx6_mmio.c"

#include "netx6_pfifo.c"

#include "netx6_rdyrun.c"

#include "netx6_systime.c"

#include "netx6_trgsmpl.c"

#include "netx6_xc.c"
#undef  __USE_XPEC_IRQ_REGS
#define __USE_XPEC_IRQ_REGS

#include "netx6_xpecirq.c"

#include "netx6_xpic.c"
