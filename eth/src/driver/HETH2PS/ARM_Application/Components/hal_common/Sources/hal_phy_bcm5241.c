/* Includes */
#include "hal_phy_bcm5241.h"
#include "hal_phyctrl.h"


/* BCM5241 registers */
#define BCM5241_CONTROL                             (0)        /* Control                               */
#define BCM5241_STATUS                              (1)        /* Status #1                             */
#define BCM5241_PHY_ID_1                            (2)        /* PHY Identification 1                  */
#define BCM5241_PHY_ID_2                            (3)        /* PHY Identification 2                  */
#define BCM5241_AUTO_NEG_ADVERTISEMENT              (4)        /* Auto-Negotiation Advertisement        */
#define BCM5241_AUTO_NEG_LINK_PARTNER_ABIL          (5)        /* Auto-Negotiation Link Partner Ability */
#define BCM5241_AUXILIARY_STATUS_SUM               (25)        /* Auxiliary status summary register     */
#define BCM5241_AUXILIARY_ERR_GEN_STATUS           (28)        /* Auxiliary error and general status    */
#define BCM5241_AUXILIARY_MODE                     (29)        /* Auxiliary mode                        */
#define BCM5241_AUXILIARY_MULTIPLE                 (30)        /* Auxiliary multiple register           */
#define BCM5241_BCM_TEST                           (31)        /* Broadcom test register                */

/* BCM5241 shadow registers */
#define BCM5241_SHADOW_MISC_CTRL                   (16)        /* Misc Control  register                */
#define BCM5241_SHADOW_MODE_4                      (26)        /* Mode 4 register                       */

/* Register 0 - Basic Control Register Bit Definition */
#define BCM5241_CONTROL_RESET                  (0x8000)        /* PHY reset                          */
#define BCM5241_CONTROL_LOOPBACK               (0x4000)        /* Enable loopback                    */
#define BCM5241_CONTROL_SPEED_SELECT_100       (0x2000)        /* Select Speed 100MBit               */
#define BCM5241_CONTROL_AUTO_NEG_ENABLE        (0x1000)        /* Auto-Negotiation Enable            */
#define BCM5241_CONTROL_POWER_DOWN             (0x0800)        /* Power-down                         */
#define BCM5241_CONTROL_ISOLATE                (0x0400)        /* Electrically Isolate PHY from MII  */
#define BCM5241_CONTROL_AUTO_NEG_RESTART       (0x0200)        /* Restart Auto-Negotiation           */
#define BCM5241_CONTROL_FULL_DUPLEX            (0x0100)        /* Full Duplex Mode                   */

/* Register 1 - Basic Status Register Bit Definition */
#define BCM5241_STATUS_100_BASE_T4             (0x8000)        /* 100BASE-T4 support                 */
#define BCM5241_STATUS_100_BASE_X_FDX          (0x4000)        /* 100BASE-X full duplex support      */
#define BCM5241_STATUS_100_BASE_X_HDX          (0x2000)        /* 100BASE-X half duplex support      */
#define BCM5241_STATUS_10_MBPS_FDX             (0x1000)        /* 10 Mbps full duplex support        */
#define BCM5241_STATUS_10_MBPS_HDX             (0x0800)        /* 10 Mbps half duplex support        */
#define BCM5241_STATUS_MF_PREAMBLE_SUPPRESS    (0x0040)        /* 1: Enable Preamble suppression     */
#define BCM5241_STATUS_AUTO_NEG_COMPLETE       (0x0020)        /* Auto-Negotiation complete          */
#define BCM5241_STATUS_REMOTE_FAULT            (0x0010)        /* Remote fault detected              */
#define BCM5241_STATUS_LINK_UP                 (0x0004)        /* Link status                        */
#define BCM5241_STATUS_JABBER_DETECT           (0x0002)        /* Jabber detected                    */
#define BCM5241_STATUS_EXTENDED_CAPABILITY     (0x0001)        /* Basic/extended register capability */

/* Register 4/5 - Auto Negotiation Advertisement Register Bit Definition, Reg4: own capabilites, Reg5: Link partner capabilities */
#define BCM5241_ADV_NEXT_PAGE                  (0x8000)        /* Ability to send multiple pages     */
#define BCM5241_ADV_REMOTE_FAULT               (0x2000)        /* Remote fault                       */
#define BCM5241_ADV_PAUSE                      (0x0400)        /* PAUSE capability                   */
#define BCM5241_ADV_100_BASE_T4                (0x0200)        /* 100BASE-T4 capability              */
#define BCM5241_ADV_100_BASE_TX_FDX            (0x0100)        /* 100BASE-TX full-duplex capability  */
#define BCM5241_ADV_100_BASE_TX                (0x0080)        /* 100BASE-TX capability              */
#define BCM5241_ADV_10_BASE_T_FDX              (0x0040)        /* 10BASE-T full-duplex capability    */
#define BCM5241_ADV_10_BASE_T                  (0x0020)        /* 10BASE-T capability                */
#define BCM5241_ADV_SELECTOR_FIELD             (0x001f)        /* <00001> = IEEE 802.3, read-only    */

/* Register 25 - Auxiliary Status Summary */
#define BCM5241_AUX_STAT_SUM_SPEED_IND         (0x0008)        /* 1/0: 100Mps/10Mps                  */
#define BCM5241_AUX_STAT_SUM_LINK_STATUS       (0x0004)        /* 1/0: Link up/down                  */
#define BCM5241_AUX_STAT_SUM_AUTONEG_IND       (0x0002)        /* 1: Auto-negotiation enabled        */
#define BCM5241_AUX_STAT_SUM_FDX_IND           (0x0001)        /* 1/0: Full-duplex active/inactive   */

/* Register 28 - Auxiliary Error and General Status */
#define BCM5241_AUX_ERR_MDIX_STATUS            (0x2000)        /* 1/0: MDIX/MDI in use               */
#define BCM5241_AUX_ERR_HP_AUTOMDIXDIS         (0x0800)        /* 1/0: Disable/Enable HP Auto-MDIX   */

/* Register 29 - Auxiliary Mode */
#define BCM5241_AUX_MODE_FORCE_ACT_LED_OFF     (0x0010)        /* 1/0: Disable/Enable XMT/RCV Activity LED outputs */
#define BCM5241_AUX_MODE_FORCE_LNK_LED_OFF     (0x0008)        /* 1/0: Disable/Enable Link LED output              */

/* Register 30 - Auxiliary Multiple */
#define BCM5241_AUX_MUL_RESTART_AUTONEG        (0x0100)        /* 1: Restart autonegotiation process */
#define BCM5241_AUX_MUL_SUPER_ISOLATE          (0x0008)        /* 1/0: Super isolate/Normal mode     */

/* Register 31 - Broadcom Test register */
#define BCM5241_BCM_TEST_SHADOW_REG_EN         (0x0080)        /* 1/0: Enable/Disable shadow registers  */

/* Shadow Register 26 - Mode 4 */
#define BCM5241_SHADOW_MODE_4_STANDBY_PWR      (0x0008)        /* 1/0: Standby power mode/ Normal mode */

/* Shadow Register 10 - Misc Ctrl */
#define BCM5241_SHADOW_MISC_CTRL_FORCED_AUTO_MDIX_EN (0x4000)  /* 1: Enable Auto-MDIX in forced modes */


/* Functions */

/*****************************************************************************/
/*! Initialize External PHY
* \description
*   Initialize an over MIIMU connected PHY.
* \class
*   EXTPHY
* \params
*   uiPort       [in] PHYCTRL instance
*   uiPhy        [in] MIIM PHY Address
*   ePhyMode     [in] PHY Mode
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int PHY_BCM5241_Init( unsigned int uiPort,
                      unsigned int uiPhyAdr,
                      PHY_BCM5241_MODE_E ePhyMode )
{
  unsigned short uiData;
  int fAutoMdixEn = 0;
  int iRet = 0;

  if( uiPhyAdr >= 32 ) return -1; /* invalid PHY address */

  /* MDIO bug: 1st MDIO read fails */
  uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MULTIPLE);

  /* get PHY out of standby power mode */
  if( PHY_BCM5241_MODE_POWER_DOWN_MODE != ePhyMode )
  {
    /* read Standby Power Mode, switch to shadow register area */
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_BCM_TEST);
    uiData |= BCM5241_BCM_TEST_SHADOW_REG_EN;
    PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_BCM_TEST, uiData);

    /* support auto-MDIX when auto-negotiation is disabled */
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_SHADOW_MISC_CTRL);
    uiData |= BCM5241_SHADOW_MISC_CTRL_FORCED_AUTO_MDIX_EN;
    PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_SHADOW_MISC_CTRL, uiData);

    /* clear standby power flag */
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_SHADOW_MODE_4);
    uiData &= ~BCM5241_SHADOW_MODE_4_STANDBY_PWR;
    PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_SHADOW_MODE_4, uiData);

    /* switch back to standard register area */
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_BCM_TEST);
    uiData &= ~BCM5241_BCM_TEST_SHADOW_REG_EN;
    PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_BCM_TEST, uiData);

    /* disable forcing of ACT and LNK LEDs */
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MODE);
    uiData &= ~(BCM5241_AUX_MODE_FORCE_ACT_LED_OFF | BCM5241_AUX_MODE_FORCE_LNK_LED_OFF);
    PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MODE, uiData);
  }

  /* get PHY out of super isolate mode */
  if( PHY_BCM5241_MODE_SUPER_ISOLATE != ePhyMode)
  {
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MULTIPLE);
    uiData &= ~BCM5241_AUX_MUL_SUPER_ISOLATE;
    PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MULTIPLE, uiData);
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MULTIPLE);
  }

  switch( ePhyMode )
  {
    case PHY_BCM5241_MODE_SUPER_ISOLATE:
      /* enable super isolate mode */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MULTIPLE);
      uiData |= BCM5241_AUX_MUL_SUPER_ISOLATE;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MULTIPLE, uiData);
    break;

    case PHY_BCM5241_MODE_10BASE_T_HD_NOAUTONEG_AUTOMDIXDIS:
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
      uiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_CONTROL, uiData);
    break;

    case PHY_BCM5241_MODE_10BASE_T_FD_NOAUTONEG_AUTOMDIXDIS:
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
      uiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      uiData |= BCM5241_CONTROL_FULL_DUPLEX;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_CONTROL, uiData);
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
    break;

    case PHY_BCM5241_MODE_100BASE_TX_HD_NOAUTONEG_AUTOMDIXDIS:
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
      uiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      uiData |= BCM5241_CONTROL_SPEED_SELECT_100;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_CONTROL, uiData);
    break;

    case PHY_BCM5241_MODE_100BASE_TX_FD_NOAUTONEG_AUTOMDIXDIS:
      /* set PHY mode */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
      uiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      uiData |= BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_FULL_DUPLEX;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_CONTROL, uiData);
    break;

    case PHY_BCM5241_MODE_POWER_DOWN_MODE:
      /* read Standby Power Mode, switch to shadow register area */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_BCM_TEST);
      uiData |= BCM5241_BCM_TEST_SHADOW_REG_EN;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_BCM_TEST, uiData);
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_BCM_TEST);

      /* set standby power flag */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_SHADOW_MODE_4);
      uiData |= BCM5241_SHADOW_MODE_4_STANDBY_PWR;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_SHADOW_MODE_4, uiData);
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_SHADOW_MODE_4);

      /* switch back to standard register area */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_BCM_TEST);
      uiData &= ~BCM5241_BCM_TEST_SHADOW_REG_EN;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_BCM_TEST, uiData);

      /* force LINK and ACTIVITY TO OFF */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MODE);
      uiData |= (BCM5241_AUX_MODE_FORCE_ACT_LED_OFF | BCM5241_AUX_MODE_FORCE_LNK_LED_OFF);
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_AUXILIARY_MODE, uiData);

    break;

    case PHY_BCM5241_MODE_AUTONEG_AUTOMDIXDIS:
      /* set PHY mode */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
      uiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      uiData |= BCM5241_CONTROL_AUTO_NEG_ENABLE;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_CONTROL, uiData);
    break;

    case PHY_BCM5241_MODE_AUTONEG_AUTOMDIXEN:
      /* enable AUTOMDIX */
      fAutoMdixEn = 1;

      /* set PHY mode */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
      uiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      uiData |= BCM5241_CONTROL_AUTO_NEG_ENABLE;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_CONTROL, uiData);
    break;

    case PHY_BCM5241_MODE_AUTONEG_AUTOMDIXEN_100BASE_TX_FD_ONLY:
      /* enable AUTOMDIX */
      fAutoMdixEn = 1;

      /* set PHY capability to 100BASE-TX FD only */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUTO_NEG_ADVERTISEMENT);
      uiData &= ~(BCM5241_ADV_100_BASE_TX | BCM5241_ADV_10_BASE_T_FDX | BCM5241_ADV_10_BASE_T);
      uiData |= BCM5241_ADV_100_BASE_TX_FDX;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_AUTO_NEG_ADVERTISEMENT, uiData);

      /* set PHY mode */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
      uiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      uiData |= BCM5241_CONTROL_AUTO_NEG_ENABLE;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_CONTROL, uiData);
    break;

    case PHY_BCM5241_MODE_AUTONEG_AUTOMDIXEN_100BASE_TX_HD_ONLY:
      /* enable AUTOMDIX */
      fAutoMdixEn = 1;

      /* set PHY capability to 100BASE-TX HD only */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUTO_NEG_ADVERTISEMENT);
      uiData &= ~(BCM5241_ADV_100_BASE_TX_FDX | BCM5241_ADV_10_BASE_T_FDX | BCM5241_ADV_10_BASE_T);
      uiData |= BCM5241_ADV_100_BASE_TX;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_AUTO_NEG_ADVERTISEMENT, uiData);

      /* set PHY mode */
      uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_CONTROL);
      uiData &= ~(BCM5241_CONTROL_POWER_DOWN | BCM5241_CONTROL_FULL_DUPLEX | BCM5241_CONTROL_SPEED_SELECT_100 | BCM5241_CONTROL_AUTO_NEG_ENABLE);
      uiData |= BCM5241_CONTROL_AUTO_NEG_ENABLE;
      PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_CONTROL, uiData);
    break;

    default:
      iRet = -1;
    break;
  }

  if(fAutoMdixEn)
  {
    /* enable AUTOMDIX */
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUXILIARY_ERR_GEN_STATUS);
    uiData &= ~BCM5241_AUX_ERR_HP_AUTOMDIXDIS;
    PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_AUXILIARY_ERR_GEN_STATUS, uiData);
  }
  else
  {
    /* disable HP-AUTOMDIX */
    uiData = PHYCTRL_MiimRead(uiPort, uiPhyAdr, BCM5241_AUXILIARY_ERR_GEN_STATUS);
    uiData |= BCM5241_AUX_ERR_HP_AUTOMDIXDIS;
    PHYCTRL_MiimWrite(uiPort, uiPhyAdr, BCM5241_AUXILIARY_ERR_GEN_STATUS, uiData);
  }

  return iRet;
}
