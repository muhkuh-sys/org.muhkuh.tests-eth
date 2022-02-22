#ifndef _SYS_H
#define _SYS_H

#include <stdint.h>
#include <stdbool.h>

/****************************************************************************************
*  Definitions
****************************************************************************************/

/* IEEE 1588 time format */
#define SYSTIME_BORDER 1000000000 /* 1 second */
#define SYSTIME_SPEED  0xa0000000 /* 1ns resolution (10ns/clock cycle) */

/* Target-specific xc port configuration */
typedef struct SYS_PORT_CFG_Ttag
{
  uint32_t ulIngressLatency;
  uint32_t ulEgressLatency;
  bool     fSetExtMii;             /* true if default setting in Eth2PS_Initialize() must be overwritten (e.g. XC0 connected to external MII)  */
  bool     fNoPhyLinkInput;        /* true: disables the link status check when transmitting frames as well as the link change interrupt from
                                            xMAC to ARM */
  bool     fDisablePort;           /* true: Disable port at initialization to discard all frames that should be transmitted */
} SYS_PORT_CFG_T;

/* Target-specific configuration */
typedef struct SYS_CFG_Ttag
{
  unsigned int uiSystime;
  SYS_PORT_CFG_T atPortCfg[2];
} SYS_CFG_T;

/* target specific configuration **/
extern SYS_CFG_T g_tSysCfg;

/****************************************************************************************
*  Functions
****************************************************************************************/

void sys_poll(void);
void sys_time_get(uint32_t* pulTimeS, uint32_t* pulTimeNs);
void main_task(void);

#endif /* _SYS_H */
