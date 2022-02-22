#ifndef IP_STACK_CFG_H_
#define IP_STACK_CFG_H_

#include "compiler_spec.h"

/* options for eth.c */
#define LOG_ENABLE 1
#define FATAL_ERROR(...) fatal_error(__VA_ARGS__);
void fatal_error(const char* pszMsg, ...);

/* options for ip.c */
#define IP_USE_ICMP  1
#define IP_USE_UDP   0

typedef int MUTEX_T;
#define MUTEX_INIT(m) m=0;
#define MUTEX_LOCK(m)
#define MUTEX_UNLOCK(m)

/* options for eth_mac.c */
#define ETH_MAC_NO_MAU_TYPE_INIT   1
#define ETH_MAC_PADDING            0
#define ETH_MAC_NO_PERF_MEASURE    1

#endif /* IP_STACK_CFG_H_ */
