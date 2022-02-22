/* Includes */

#include "hal_phyctrl.h"
#include "hw_defines.h"

#define PHYCTRL_PORT_CNT (sizeof(s_aptPhyCtrl)/sizeof(s_aptPhyCtrl[0]))

typedef struct PHY_CTRL_STATE_Ttag
{
  uint32_t ulMiimuMsk;
} PHY_CTRL_STATE_T;

/* Variables */
HW_PTR_PHY_CTRL(s_aptPhyCtrl)

static PHY_CTRL_STATE_T s_atPhyCtrlState[PHYCTRL_PORT_CNT];

/* Functions */

int PHYCTRL_Init(unsigned int uiPort)
{
  if(uiPort >= PHYCTRL_PORT_CNT) return 1;

  s_atPhyCtrlState[uiPort].ulMiimuMsk = ( HW_MSK(int_phy_ctrl_miimu_preamble)
                                         |HW_MSK(int_phy_ctrl_miimu_mdc_period)
                                         |HW_MSK(int_phy_ctrl_miimu_rta)
                                         |HW_MSK(int_phy_ctrl_miimu_snrdy) );

  return 0;
}


/*****************************************************************************/
/*! Set PHYCTRL Parameter
* \description
*   Set any INTPHY Parameter
* \class
*   INTPHY
* \params
*   uiPort      [in]  PHY port number
*   ePrmID      [in]  Parameter Identifier
*   ulPrmVal    [in]  Parameter Value
* \return
*   0 on success
*   1 invalid parameter value uiPort
*   2 invalid parameter value ePrmID
*   3 invalid parameter value ulPrmVal                                       */
/*****************************************************************************/
int PHYCTRL_PrmSet ( unsigned int uiPort, PHYCTRL_PRM_E ePrmID, uint32_t ulPrmVal )
{
  int iRslt = 0;
  uint32_t ulReg;

  if(uiPort >= PHYCTRL_PORT_CNT) return 1;

  /* Get the selected parameter */
  switch(ePrmID)
  {
    case PHYCTRL_PRM_LED_MODE:
      if( ulPrmVal > (uint32_t)PHYCTRL_LED_MODE_COMBINED ) return 3;
      ulReg = s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_led & ~HW_MSK(int_phy_ctrl_led_mode);
      s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_led = ulReg |(ulPrmVal << HW_SRT(int_phy_ctrl_led_mode));
      break;

    case PHYCTRL_PRM_LED_BLINK_INTERVAL_MS:
      if ( (ulPrmVal > 160) || (ulPrmVal < 10) ) return 3;
      ulPrmVal = (ulPrmVal / 10) - 1;
      ulReg = s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_led & ~HW_MSK(int_phy_ctrl_led_interval);
      s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_led = ulReg |(ulPrmVal << HW_SRT(int_phy_ctrl_led_interval));
      break;

    case PHYCTRL_PRM_LED_OUT_LINE:
      if ( ulPrmVal > 0x3 ) return 3;
      ulReg = s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_led & ~(HW_MSK(int_phy_ctrl_led_led0) | HW_MSK(int_phy_ctrl_led_led1));
      s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_led = ulReg | (ulPrmVal << HW_SRT(int_phy_ctrl_led_led0));
      break;

    case PHYCTRL_PRM_MIIM_PREAMBLE:
      if( ulPrmVal != 0 )
      {
        s_atPhyCtrlState[uiPort].ulMiimuMsk |= HW_MSK(int_phy_ctrl_miimu_preamble);
      }
      else
      {
        s_atPhyCtrlState[uiPort].ulMiimuMsk &= ~HW_MSK(int_phy_ctrl_miimu_preamble);
      }
      break;

    case PHYCTRL_PRM_MIIM_MDC_FAST:
      if( ulPrmVal != 0 )
      {
        s_atPhyCtrlState[uiPort].ulMiimuMsk &= ~HW_MSK(int_phy_ctrl_miimu_mdc_period);
      }
      else
      {
        s_atPhyCtrlState[uiPort].ulMiimuMsk |= HW_MSK(int_phy_ctrl_miimu_mdc_period);
      }
      break;

    /* Default */
    default:
      iRslt = 2;
    break;
  } /* end switch */

  return iRslt;
}

/*****************************************************************************/
/*! Get Link status
* \description
*   Returns the Link status
* \class
*   PHYCTRL
* \params
*   uiPort          [in]  PHY port number
*   puiLinkUp       [out] Current Link Status 1: Up, 0: Down
*   puiSpeed        [out] Current Speed in MBit/s
*   puiIsFullDuplex [out] Current Duplex Mode: 0: Half-Duplex, 1: Full-Duplex
* \return
*   0 on success
*   1 on parameter errors                                                    */
/*****************************************************************************/
int PHYCTRL_GetLinkState( unsigned int uiPort,
                          unsigned int* puiLinkUp,
                          unsigned int* puiSpeed,
                          unsigned int* puiIsFullDuplex )
{
  uint32_t ulValue;

  if(uiPort >= PHYCTRL_PORT_CNT) return 1;

  ulValue = s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_led;
  *puiLinkUp       = (ulValue & HW_MSK(int_phy_ctrl_led_link_ro)     ) ? 1   : 0;
  *puiSpeed        = (ulValue & HW_MSK(int_phy_ctrl_led_speed100_ro) ) ? 100 : 10;
  *puiIsFullDuplex = (ulValue & HW_MSK(int_phy_ctrl_led_duplex_ro)   ) ? 1   : 0;

  return 0;
}

/*****************************************************************************/
/*! Get ELD status from Internal PHY
* \description
*   Returns the ELD status
* \class
*   PHYCTRL
* \params
*   uiPhyNum          [in]  PHY port number
*   pfBadLink         [out] Bad link indicator: 0: Link OK, 1: Link Failure
*   puiCounter        [out] Current error counter value
* \return
*   0 on success
*   1 on parameter errors                                                    */
/*****************************************************************************/
int PHYCTRL_GetEldStatus( unsigned int  uiPort,
                          bool*         pfBadLink,
                          unsigned int* puiCounter )
{
  uint32_t ulVal;

  if(uiPort >= PHYCTRL_PORT_CNT) return 1;

  ulVal = s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_enhanced_link_detection;
  *puiCounter = (uint16_t)((ulVal & HW_MSK(int_phy_ctrl_enhanced_link_detection_counter_ro)) >> HW_SRT(int_phy_ctrl_enhanced_link_detection_counter_ro));
  *pfBadLink = (ulVal&HW_MSK(int_phy_ctrl_enhanced_link_detection_eld_bad_link_ro)) ? true : false;

  return 0;
}
/*****************************************************************************/
/*! Configure ELD unit
* \description
*   Configures the ELD unit
* \class
*   PHYCTRL
* \params
*   uiPort           [in]  PHY port number
*   uiAdd            [in]  Add value in case of RX_ERR
*   uiSub            [in]  Sub value in case of no RX_ERR
*   uiTreshold       [in]  Threshold value for link down detection
* \return
*   0 on success
*   1 on parameter errors                                                    */
/*****************************************************************************/
int PHYCTRL_ConfigEld( unsigned int uiPort,
                       unsigned int uiAdd,
                       unsigned int uiSub,
                       unsigned int uiThreshold )
{
  if(uiPort >= PHYCTRL_PORT_CNT) return 1;
  if(uiAdd > 31) return 1;
  if(uiSub > 3) return 1;
  if(uiThreshold > 4095) return 1;

  s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_enhanced_link_detection =  (  uiAdd << HW_SRT(int_phy_ctrl_enhanced_link_detection_add))
                                                                  |( uiSub << HW_SRT(int_phy_ctrl_enhanced_link_detection_sub))
                                                                  |( uiThreshold << HW_SRT(int_phy_ctrl_enhanced_link_detection_threshold));

  return 0;
}

/*****************************************************************************/
/*! Read PHY Register
* \description
*   Read PHY register over MDIO.
* \class
*   EXTPHY
* \params
*   uiPort        [in]  PHY port number
*   uiPhy         [in]  MIIM PHY Address
*   uiReg         [in]  MIIM Register Address
* \return
*   data                                                                     */
/*****************************************************************************/
unsigned int PHYCTRL_MiimRead( unsigned int uiPort,
                               unsigned int uiPhy,
                               unsigned int uiReg )
{
  unsigned int uiData;

  if( uiPhy >= 32 ) return ~0U; /* invalid PHY address */
  if( uiReg >= 32 ) return ~0U; /* invalid Register address */

  s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_miimu = ( s_atPhyCtrlState[uiPort].ulMiimuMsk
                                                |(uiPhy << HW_SRT(int_phy_ctrl_miimu_phyaddr))
                                                |(uiReg << HW_SRT(int_phy_ctrl_miimu_regaddr))
                                               );

  do {
    uiData = s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_miimu;
  }
  while( (uiData & HW_MSK(int_phy_ctrl_miimu_snrdy)) != 0 );

  return (uiData & HW_MSK(int_phy_ctrl_miimu_data)) >> HW_SRT(int_phy_ctrl_miimu_data);
}

/*****************************************************************************/
/*! Write PHY Register
* \description
*   Write PHY register over MDIO.
* \class
*   EXTPHY
* \params
*   uiPort        [in] PHY port number
*   uiPhy         [in] MIIM PHY Address
*   uiReg         [in] MIIM Register Address
*   uiData        [in] MIIM Data
* \return
*   0 on success                                                             */
/*****************************************************************************/
void PHYCTRL_MiimWrite( unsigned int uiPort,
                        unsigned int uiPhy,
                        unsigned int uiReg,
                        unsigned int uiData )
{
  s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_miimu = ( s_atPhyCtrlState[uiPort].ulMiimuMsk
                                                |HW_MSK(int_phy_ctrl_miimu_opmode)
                                                |((uint32_t)uiPhy  << HW_SRT(int_phy_ctrl_miimu_phyaddr))
                                                |((uint32_t)uiReg  << HW_SRT(int_phy_ctrl_miimu_regaddr))
                                                |((uint32_t)uiData << HW_SRT(int_phy_ctrl_miimu_data))
                                               );

  while ((s_aptPhyCtrl[uiPort]->ulInt_phy_ctrl_miimu & HW_MSK(int_phy_ctrl_miimu_snrdy)) != 0);
}
