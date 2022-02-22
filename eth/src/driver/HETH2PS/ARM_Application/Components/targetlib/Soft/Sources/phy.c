#include "phy.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define PHY_CNT_MAX 4

typedef struct
{
  unsigned int uiAdr;
  void         (*write)(unsigned int uiAdr, unsigned int uiReg, unsigned int uiValue);
  unsigned int (*read) (unsigned int uiAdr, unsigned int uiReg);
  uint32_t ulPhyId;

  unsigned char bRegLnk;      /**< PHY Register address where LINK status is shown */
  unsigned char bBitLnk;     /**< Bit number within PHY Register address where LINK status is shown */
  bool fInvLnk;              /**< Must be set if Duplex signal is low-active, 1/0: Half/Full */

  unsigned char bRegDpx;      /**< PHY Register address where Duplex status is shown */
  unsigned char bBitDpx;     /**< Bit number within PHY Register address where DUPLEX status is shown */
  bool fInvDpx;              /**< Must be set if DUPLEX status is 1/0: Half/Full */

  unsigned char bRegSpd;      /**< PHY Register address where SPEED status is shown */
  unsigned char bBitSpd;     /**< Bit number within PHY Register address where SPEED status is shown */
  bool fInvSpd;              /**< Must be set if Speed signal is 1/0: 10 MBit/100 MBit */

  /**< Pointer to an external API to retrieve link status information. Used for external PHYs that don't
   *   support MDIO clause 22 fully or use clause 45 */
  int (*ptExternalLinkStateApi)(uint8_t, unsigned int*, unsigned int*, unsigned int*);
} PHY_T;

static PHY_T s_atPhy[PHY_CNT_MAX];

/**
 * \brief Registers an external API to the PHY port that retrieves link, speed and duplex information
 *        when called.
 * \params
 *  uint8_t uiPort    [in]      Port number of the xC channel the PHY is connected to
 *  int (*ptr)        [in]      Function pointer to the external API function
 *
 * \return
 *  0 on success, -1 on invalid parameters
 *
 * The API function shall have the following structure:
 *    int ext_api(uint8_t port, unsigned int* link, unsigned int* speed, unsigned int* duplex)
 *
 * When set, the API will be called during runtime with the following parameters:
 *   - uint8_t port         [in]    : Number of the xC port the PHY is connected to
 *   - unsigned int* link   [out]   : Pointer to a uint to save the link status to (>= 0 : link okay, 0: no link)
 *   - unsigned int* speed  [out]   : Pointer to a uint to save the speed information to [either 10 or 100 (MBit/s)]
 *   - unsigned int* duplex [out]   : Pointer to a uint to save the duplex status to (>= 0 : full-duplex, 0: half-duplex)
 *
 * The API shall return a 0 upon successful readout of the values, or any other value upon failure.
 * When no external API is registered, the normal phy_get_linkstate() function will be called instead.
 */
int phy_register_ext_link_api(uint8_t uiPort,
                              int (*ptr)(uint8_t, unsigned int*, unsigned int*, unsigned int*))
{
  PHY_T* ptPhy;
  if(uiPort >= PHY_CNT_MAX || ptr == 0) {
    return -1;
  }

  ptPhy = &s_atPhy[uiPort];
  ptPhy->ptExternalLinkStateApi = ptr;
  return 0;
}

int phy_add(unsigned int uiPort,
            unsigned int uiAdr,
            void         (*fnMiimWrite)(unsigned int uiAdr, unsigned int uiReg, unsigned int uiValue),
            unsigned int (*fnMiimRead) (unsigned int uiAdr, unsigned int uiReg))
{
  PHY_T* ptPhy;
  int iRslt = 0;

  if(uiPort >= PHY_CNT_MAX)
  {
    return -1;
  }

  ptPhy = &s_atPhy[uiPort];
  ptPhy->uiAdr     = uiAdr;
  ptPhy->write     = fnMiimWrite;
  ptPhy->read      = fnMiimRead;
  ptPhy->ulPhyId   = ptPhy->read(ptPhy->uiAdr, 2) << 16;
  ptPhy->ulPhyId  |= ptPhy->read(ptPhy->uiAdr, 3) & 0xfff0;
  ptPhy->ptExternalLinkStateApi = 0x0;


  switch(ptPhy->ulPhyId)
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
  case PHYID_TLK105:
    ptPhy->bRegLnk      = 1;
    ptPhy->bBitLnk      = 2;
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

  case PHYID_ADIN1100_U1:
  case PHYID_ADIN1100_U2:
    /* This PHY uses a different API to retrieve link status / speed / duplex mode */
    /* \see adinphy.h */
    ptPhy->bRegLnk      = 0;
    ptPhy->bBitLnk      = 0;
    ptPhy->fInvLnk      = 0;

    ptPhy->bRegDpx      = 0;
    ptPhy->bBitDpx      = 0;
    ptPhy->fInvDpx      = 0;

    ptPhy->bRegSpd      = 0;
    ptPhy->bBitSpd      = 0;
    ptPhy->fInvSpd      = 0;
    break;

  case PHYID_NETX_INTPHY_NEC:
  case PHYID_NETX_INTPHY_REE:
  case PHYID_NETX_INTPHY_REE2:
  case PHYID_NETX_INTPHY_EPHY:
  case PHYID_RENESAS_UPD606:
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

  default:
    iRslt = 1;
    break;
  }

  return iRslt;
}

int phy_get_linkstate(unsigned int uiPort, unsigned int* puiLink, unsigned int* puiSpeed, unsigned int* puiDuplex)
{
  unsigned int uiVal;
  PHY_T* ptPhy = &s_atPhy[uiPort];
  unsigned int uiAdr = ptPhy->uiAdr;

  if(ptPhy->ptExternalLinkStateApi != NULL)
  {
    /* Use external API to retrieve link status, speed and duplex mode */
    int result = ptPhy->ptExternalLinkStateApi((uint8_t) uiPort, puiLink, puiSpeed, puiDuplex);
    if(result != 0)
    {
      /* Something did not work using the external API, set default values */
      *puiLink = 0 ^ ptPhy->fInvLnk;
      *puiDuplex = 0  ^ ptPhy->fInvDpx;
      *puiSpeed = 10;
    }
    else
    {
      /* Invert bits if neccessary */
      *puiLink ^= ptPhy->fInvLnk;
      *puiDuplex ^= ptPhy->fInvDpx;
    }
  }
  else
  {
    /* retrieve Link status via MDIO */
    uiVal = ptPhy->read(uiAdr, ptPhy->bRegLnk);
    uiVal = ((uiVal >> ptPhy->bBitLnk) & 1) ^ ptPhy->fInvLnk;

    if( !uiVal && (ptPhy->bRegLnk == 1) )
    {
      /* read link a second time to avoid LinkDown latch of Reg1.2 */
      uiVal = ptPhy->read(uiAdr, ptPhy->bRegLnk);
      uiVal = ((uiVal >> ptPhy->bBitLnk) & 1) ^ ptPhy->fInvLnk;
    }

    *puiLink = uiVal;

    /* retrieve Duplex status via MDIO if it is located in different register than Link status */
    if( ptPhy->bRegLnk != ptPhy->bRegDpx )
    {
      uiVal = ptPhy->read(uiAdr, ptPhy->bRegDpx);
    }

    *puiDuplex = ((uiVal >> ptPhy->bBitDpx) & 1) ^ ptPhy->fInvDpx;

    /* retrieve Speed status via MDIO if it is located in different register than Duplex status */
    if( ptPhy->bRegSpd != ptPhy->bRegDpx )
    {
      uiVal = ptPhy->read(uiAdr, ptPhy->bRegSpd);
    }

    uiVal = (((uiVal >> ptPhy->bBitSpd) & 1) ^ ptPhy->fInvSpd);
    *puiSpeed = uiVal ? 100 : 10;
  }
  return 0;
}

unsigned int phy_read(unsigned int uiPort, unsigned int uiReg)
{
  PHY_T* ptPhy = &s_atPhy[uiPort];
  return ptPhy->read(ptPhy->uiAdr, uiReg);
}

void phy_write(unsigned int uiPort, unsigned int uiReg, unsigned int uiValue)
{
  PHY_T* ptPhy = &s_atPhy[uiPort];
  ptPhy->write(ptPhy->uiAdr, uiReg, uiValue);
}
