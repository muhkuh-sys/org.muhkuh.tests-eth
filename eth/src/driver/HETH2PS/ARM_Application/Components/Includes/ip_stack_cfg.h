#ifndef IP_STACK_CFG_H_
#define IP_STACK_CFG_H_

/****************************************************************************************
*  Includes
****************************************************************************************/

#include "compiler_spec.h"
#include "hal_irq.h"

/****************************************************************************************
*  Definitions
****************************************************************************************/

typedef IRQSTATE MUTEX_T;
#define MUTEX_INIT(m) m=0;
#define MUTEX_LOCK(m) m=IRQ_LockSave();
#define MUTEX_UNLOCK(m) IRQ_LockRestore(m);

void fatal_error(const char* pszDescription, ...);
void irq_init(void);

/* resource protection */
#define ETH_ECHO_UNKNOWN_FRAMES  1

#define IP_USE_UDP   0
#define IP_USE_ICMP  1

#define LOG_ENABLE 1
#define FATAL_ERROR(...) fatal_error(__VA_ARGS__)

#define ETH_2PS_UPDATE_PHY_PHASE_OFFSET 1

#endif /* IP_STACK_CFG_H_ */
