#include "phy.h"

#define PHY_CNT_MAX 4

typedef struct
{
  unsigned int uiMdioAdr;
  const PHY_OPS_T* ptOps;
  uint32_t ulPhyId;

  unsigned char bRegLnk;      /**< PHY Register address where LINK status is shown */
  unsigned char bBitLnk;      /**< Bit number within PHY Register address where LINK status is shown */
  bool fInvLnk;               /**< Must be set if Duplex signal is low-active, 1/0: Half/Full */

  unsigned char bRegDpx;      /**< PHY Register address where Duplex status is shown */
  unsigned char bBitDpx;      /**< Bit number within PHY Register address where DUPLEX status is shown */
  bool fInvDpx;               /**< Must be set if DUPLEX status is 1/0: Half/Full */

  unsigned char bRegSpd;      /**< PHY Register address where SPEED status is shown */
  unsigned char bBitSpd;      /**< Bit number within PHY Register address where SPEED status is shown */
  bool fInvSpd;               /**< Must be set if Speed signal is 1/0: 10 MBit/100 MBit */
} PHY_T;

static PHY_T s_atPhy[PHY_CNT_MAX];

int phy_init(unsigned int uiPort, unsigned int uiMdioAdr, uint32_t ulPhyId, const PHY_OPS_T* ptOps)
{
  PHY_T* ptPhy;

  if(uiPort >= PHY_CNT_MAX)
  {
    return -1;
  }

  ptPhy = &s_atPhy[uiPort];
  ptPhy->uiMdioAdr = uiMdioAdr;
  ptPhy->ulPhyId   = ulPhyId;
  ptPhy->ptOps     = ptOps;

  switch(ulPhyId)
  {
  case PHYID_MICREL_KSZ8041:
    ptPhy->bRegLnk      = 1;
    ptPhy->bBitLnk      = 2;
    ptPhy->fInvLnk      = 0;

    ptPhy->bRegDpx      = 31;
    ptPhy->bBitDpx      = 4;
    ptPhy->fInvDpx      = 0;

    ptPhy->bRegSpd      = 31;
    ptPhy->bBitSpd      = 3;
    ptPhy->fInvSpd      = 0;
    break;

  case PHYID_NATIONAL_DP83848I:
    ptPhy->bRegLnk      = 0x10;
    ptPhy->bBitLnk      = 0;
    ptPhy->fInvLnk      = 0;

    ptPhy->bRegDpx      = 0x10;
    ptPhy->bBitDpx      = 2;
    ptPhy->fInvDpx      = 0;

    ptPhy->bRegSpd      = 0x10;
    ptPhy->bBitSpd      = 1;
    ptPhy->fInvSpd      = 1;
    break;

  case PHYID_BCM5241:
    ptPhy->bRegLnk      = 1;
    ptPhy->bBitLnk      = 2;
    ptPhy->fInvLnk      = 0;

    ptPhy->bRegDpx      = 28;
    ptPhy->bBitDpx      = 0;
    ptPhy->fInvDpx      = 0;

    ptPhy->bRegSpd      = 28;
    ptPhy->bBitSpd      = 1;
    ptPhy->fInvSpd      = 0;
    break;

  /*
  case PHYID_NEC:
  case PHYID_REE:
  case PHYID_RENESAS_UPD606:
  case PHYID_OMNIPHY:
  */
  default:
    ptPhy->bRegLnk      = 1;
    ptPhy->bBitLnk      = 2;
    ptPhy->fInvLnk      = 0;

    ptPhy->bRegDpx      = 31;
    ptPhy->bBitDpx      = 4;
    ptPhy->fInvDpx      = 0;

    ptPhy->bRegSpd      = 31;
    ptPhy->bBitSpd      = 3;
    ptPhy->fInvSpd      = 0;
    break;
  }

  return 0;
}

int phy_get_linkstate(unsigned int uiPort, ETH_MAU_TYPE_T* ptConnState)
{
  unsigned int uiVal;
  PHY_T* ptPhy = &s_atPhy[uiPort];
  unsigned int uiAdr = ptPhy->uiMdioAdr;

  /* retrieve Link status via MDIO */
  uiVal = ptPhy->ptOps->read(uiAdr, ptPhy->bRegLnk);
  ptConnState->uIsLinkUp = ((uiVal >> ptPhy->bBitLnk) & 1) ^ ptPhy->fInvLnk;

  if( !ptConnState->uIsLinkUp && (ptPhy->bRegLnk == 1) )
  {
    /* read link a second time to avoid LinkDown latch of Reg1.2 */
    uiVal = ptPhy->ptOps->read(uiAdr, ptPhy->bRegLnk);
    ptConnState->uIsLinkUp = ((uiVal >> ptPhy->bBitLnk) & 1) ^ ptPhy->fInvLnk;
  }

  /* retrieve Duplex status via MDIO if it is located in different register than Link status */
  if( ptPhy->bRegLnk != ptPhy->bRegDpx )
  {
    uiVal = ptPhy->ptOps->read(uiAdr, ptPhy->bRegDpx);
  }

  ptConnState->uIsFullDuplex = ((uiVal >> ptPhy->bBitDpx) & 1) ^ ptPhy->fInvDpx;

  /* retrieve Speed status via MDIO if it is located in different register than Duplex status */
  if( ptPhy->bRegSpd != ptPhy->bRegDpx )
  {
    uiVal = ptPhy->ptOps->read(uiAdr, ptPhy->bRegSpd);
  }

  ptConnState->uSpeed = (((uiVal >> ptPhy->bBitSpd) & 1) ^ ptPhy->fInvSpd);
  ptConnState->uSpeed = ptConnState->uSpeed ? 100 : 10;

  return 0;
}

unsigned int phy_read(unsigned int uiPort, unsigned int uiReg)
{
  PHY_T* ptPhy = &s_atPhy[uiPort];
  return ptPhy->ptOps->read(ptPhy->uiMdioAdr, uiReg);
}
void phy_write(unsigned int uiPort, unsigned int uiReg, unsigned int uiValue)
{
  PHY_T* ptPhy = &s_atPhy[uiPort];
  ptPhy->ptOps->write(ptPhy->uiMdioAdr, uiReg, uiValue);
}
