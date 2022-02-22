#ifndef __ASIC_NETX5_H
#define __ASIC_NETX5_H

#include <stdint.h>

void netx5_init(void);
void netx5_systime_init(uint32_t ulBorder, uint32_t ulSpeed);
void netx5_phy_init(void);
unsigned int netx5_phy_read(unsigned int uiPhy, unsigned int uiReg);
void netx5_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue);

#endif
