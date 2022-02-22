#ifndef __COMMON_FUNC_NETX90_H
#define __COMMON_FUNC_NETX90_H

#ifndef CPU_IS_ARM_APP
#include "hw_irqid_arm_com.h"
#include "hw_irqid_xpic_com.h"
#include "hw_irqid_external_dpm0.h"
#include "hw_irqid_external_dpm1.h"
#include "hw_irqid_internal_dpm.h"
#else
#include "hw_irqid_arm_app.h"
#include "hw_irqid_xpic_app.h"
#endif

#include "hal_armirq.h"
#include "hal_armtimer.h"
#include "hal_asicctrl.h"
#include "hal_blink.h"
#include "hal_dpm.h"
#include "hal_gpio.h"
#include "hal_irq.h"
#include "hal_mcp.h"
#include "hal_hif_ioctrl.h"
#include "hal_miimu.h"
#include "hal_mled_ctrl.h"
#include "hal_mmio.h"
#include "hal_padctrl.h"
#include "hal_pfifo.h"
#include "hal_phyctrl.h"
#include "hal_phy_bcm5241.h"
#include "hal_pio.h"
#include "hal_rdyrun.h"
#include "hal_systime_lt.h"
#include "hal_systime.h"
#include "hal_uart.h"
#include "hal_xc.h"
#include "hal_xpic.h"
#include "hal_intphy.h"

#endif /* #ifndef __COMMON_FUNC_NETX90_H */
