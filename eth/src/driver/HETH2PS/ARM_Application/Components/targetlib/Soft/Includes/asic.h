#ifndef __ASIC_H
#define __ASIC_H

#include <stdint.h>

void asic_init(void);
void systime_init(unsigned int uiInst, uint32_t ulBorder, uint32_t ulSpeed);
void systime_get(unsigned int uiInst, uint32_t* pulSec, uint32_t* pulNsec);
void sleep(unsigned int uiMs);
void irq_setup(unsigned int uiPrio, unsigned int uiIrqId, void (*pfnIsr)(void));

void intphy_init(void);
void intphy_init_port0_only(void);  /* netX 90 only! */
void intphy_init_fx(void);
void intphy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue);
unsigned int intphy_read(unsigned int uiPhy, unsigned int uiReg);

void extphy_init(void);
void extphy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue);
unsigned int extphy_read(unsigned int uiPhy, unsigned int uiReg);

void* iflash_read_info_block(void* pvDst, const void* pvSrc, unsigned int uiSize);

#endif /* __ASIC_H */
