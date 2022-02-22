/****************************************************************************************
*  Ethernet PHY
*  API
****************************************************************************************/

#ifndef __PHY_H
#define __PHY_H

/****************************************************************************************
*  Definitions
****************************************************************************************/

#include <stdint.h>

#define PHYID_NETX_INTPHY_NEC   0x00000010 /* netX10/50/100/500 ID written at initialization */
#define PHYID_NETX_INTPHY_REE   0x00000000 /* netX6/51/52 ID fixed (read-only) */
#define PHYID_NETX_INTPHY_REE2  0x00332000 /* netX4000 ID fixed (read-only) */
#define PHYID_NETX_INTPHY_EPHY  0x01011490 /* EPHY */
#define PHYID_MICREL_KSZ8041    0x00221510
#define PHYID_NATIONAL_DP83848I 0x20005ca0
#define PHYID_BCM5241           0x0143bc30
#define PHYID_RENESAS_UPD606    0xb8242220
#define PHYID_TLK105            0x2000a210
#define PHYID_ADIN1100_U1       0x0283bc80 /* ADIN1100 10BASE-T1L SPE PHY chip version 1 */
#define PHYID_ADIN1100_U2       0x0283bc81 /* ADIN1100 10BASE-T1L SPE PHY chip version 2 */

/* MDIO clause 22 register set */
#define PHYREG_BASIC_CONTROL                    0
#define PHYREG_BASIC_STATUS                     1
#define PHYREG_PHY_ID1                          2
#define PHYREG_PHY_ID2                          3
#define PHYREG_AUTONEG_ADV                      4
#define PHYREG_AUTONEG_ADV_LP_ABILITY           5
#define PHYREG_AUTONEG_EXPANSION                6
#define PHYREG_NEXT_PAGE_TRANSMIT               7

/* Register 0 - Basic Control Register Bit Definition */
#define PHY_CONTROL_RESET                  (0x8000)        /* PHY reset                          */
#define PHY_CONTROL_LOOPBACK               (0x4000)        /* Enable loop back                   */
#define PHY_CONTROL_SPEED_SELECT_100       (0x2000)        /* Select Speed 100MBit               */
#define PHY_CONTROL_AUTO_NEG_ENABLE        (0x1000)        /* Auto-Negotiation Enable            */
#define PHY_CONTROL_POWER_DOWN             (0x0800)        /* Power-down                         */
#define PHY_CONTROL_ISOLATE                (0x0400)        /* Electrically Isolate PHY from MII  */
#define PHY_CONTROL_AUTO_NEG_RESTART       (0x0200)        /* Restart Auto-Negotiation           */
#define PHY_CONTROL_FULL_DUPLEX            (0x0100)        /* FDX Mode                           */

/* Register 4/5 - Auto Negotiation Advertisement Register Bit Definition, Reg4: own capabilities, Reg5: Link partner capabilities */
#define PHY_ADV_NEXT_PAGE                  (0x8000)        /* Ability to send multiple pages     */
#define PHY_ADV_REMOTE_FAULT               (0x2000)        /* Remote fault                       */
#define PHY_ADV_PAUSE                      (0x0400)        /* PAUSE capability                   */
#define PHY_ADV_100_BASE_T4                (0x0200)        /* 100BASE-T4 capability              */
#define PHY_ADV_100_BASE_TX_FDX            (0x0100)        /* 100BASE-TX FDX capability          */
#define PHY_ADV_100_BASE_TX                (0x0080)        /* 100BASE-TX capability              */
#define PHY_ADV_10_BASE_T_FDX              (0x0040)        /* 10BASE-T FDX capability            */
#define PHY_ADV_10_BASE_T                  (0x0020)        /* 10BASE-T capability                */
#define PHY_ADV_SELECTOR_FIELD             (0x001f)        /* <00001> = IEEE 802.3, read-only    */

#define PHY_ADV_ALL               (PHY_ADV_10_BASE_T\
                                  |PHY_ADV_10_BASE_T_FDX\
                                  |PHY_ADV_100_BASE_TX\
                                  |PHY_ADV_100_BASE_TX_FDX)

/****************************************************************************************
*  Functions
****************************************************************************************/

int phy_add(unsigned int uiPort,
            unsigned int uiAdr,
            void         (*fnMiimWrite)(unsigned int uiAdr, unsigned int uiReg, unsigned int uiValue),
            unsigned int (*fnMiimRead) (unsigned int uiAdr, unsigned int uiReg));
int phy_get_linkstate(unsigned int uiPort, unsigned int* puiLink, unsigned int* puiSpeed, unsigned int* puiDuplex);
unsigned int phy_read(unsigned int uiPort, unsigned int uiReg);
void phy_write(unsigned int uiPort, unsigned int uiReg, unsigned int uiValue);
int phy_register_ext_link_api(uint8_t uiPort, int (*ptr)(uint8_t, unsigned int*, unsigned int*, unsigned int*));
#endif /* __PHY_H */
