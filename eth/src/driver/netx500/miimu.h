
#include "netx_io_areas.h"

#ifndef __MIIMU_H__
#define __MIIMU_H__

void miimu_dummy_read(unsigned uiInstance);
unsigned long Miimu_ReadPhy(unsigned int uiPhyAdr, unsigned int uMiimuReqAddr);
void Miimu_WritePhy(unsigned int uiPhyAdr, unsigned int uMiimuReqAddr, unsigned int uiData);

#endif	/* __MIIMU_H__ */

