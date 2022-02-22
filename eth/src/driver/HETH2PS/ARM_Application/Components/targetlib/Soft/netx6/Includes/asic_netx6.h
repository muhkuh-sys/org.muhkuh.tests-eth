#ifndef __ASIC_NETX6_H
#define __ASIC_NETX6_H

#include <stdint.h>

void netx6_init(void);
void netx6_systime_init(uint32_t ulBorder, uint32_t ulSpeed);
void netx6_phy_init(void);
void netx6_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue);
unsigned int netx6_phy_read(unsigned int uiPhy, unsigned int uiReg);

#endif
