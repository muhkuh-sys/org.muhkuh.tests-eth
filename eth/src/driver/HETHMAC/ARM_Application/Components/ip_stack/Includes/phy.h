/****************************************************************************************
*  Ethernet PHY
*  API
****************************************************************************************/

#ifndef _PHY_H
#define _PHY_H

/****************************************************************************************
*  Includes
****************************************************************************************/

#include "eth.h" /* import ETH_MAU_TYPE_T */

/****************************************************************************************
*  Definitions
****************************************************************************************/

#define PHYID_MICREL_KSZ8041    0x00220000
#define PHYID_NATIONAL_DP83848I 0x20000000
#define PHYID_OMNIPHY           0x01810000
#define PHYID_BCM5241           0x0143BC31
#define PHYID_NETX_INTPHY       0x00000000 /* Internal PHY netX5x/100/500/4xxx */
#define PHYID_RENESAS_UPD606    0xffffffff


typedef struct PHY_OPS_Ttag
{
  void         (*write)(unsigned int uiMdioAdr, unsigned int uiReg, unsigned int uiValue);
  unsigned int (*read) (unsigned int uiMdioAdr, unsigned int uiReg);
} PHY_OPS_T;

/****************************************************************************************
*  Functions
****************************************************************************************/

int phy_init(unsigned int uiPort, unsigned int uiMdioAdr, uint32_t ulPhyId, const PHY_OPS_T* ptOps);
int phy_get_linkstate(unsigned int uiPort, ETH_MAU_TYPE_T* ptConnState);
unsigned int phy_read(unsigned int uiPort, unsigned int uiReg);
void phy_write(unsigned int uiPort, unsigned int uiReg, unsigned int uiValue);

#endif /* _PHY_H */
