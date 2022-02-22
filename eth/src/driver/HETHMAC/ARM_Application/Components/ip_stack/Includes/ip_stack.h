#ifndef _IP_STACK_H
#define _IP_STACK_H

#include "ip_stack_cfg.h" /* not part of library includes, provided by compiling user */
#include "sg.h"

/****************************************************************************************
*  Definitions
****************************************************************************************/

/* Local IP addresses */
#define IP_ADDR(a,b,c,d) ((a << 24)|(b << 16)|(c << 8)|d)

#ifndef FATAL_ERROR
#define FATAL_ERROR(...)
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef LOG_ENABLE
#define LOG_ENABLE 0
#endif

#ifndef MUTEX_INIT
typedef int MUTEX_T;
#define MUTEX_INIT(m) m=0;
#define MUTEX_LOCK(m)
#define MUTEX_UNLOCK(m)
#endif

#ifndef INC_CNT
#define INC_CNT(var) var++;
#endif

#ifndef IP_USE_UDP
#define IP_USE_UDP 0
#endif

#ifndef IP_USE_ICMP
#define IP_USE_ICMP 0
#endif

#endif /* _IP_STACK_H */
