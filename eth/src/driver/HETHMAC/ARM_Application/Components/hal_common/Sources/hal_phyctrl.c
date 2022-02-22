/* Includes */

#include "hal_phyctrl.h"
#include "hw_defines.h"

#define PHYCTRL_PORT_CNT 2

/* Variables */
HW_PTR_PHY_CTRL(s_aptPhyCtrl)

/* Functions */

/*****************************************************************************/
/*! Get LED status from Internal PHY
* \description
*   Returns the LED status
* \class
*   PHYCTRL
* \params
*   uPhyNum          [in]  Number of internal Phy
* \return
*                                                                            */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

uint32_t HALDEF(PHYCTRL_GetLedStatus)( unsigned int uXcInst,
                                       unsigned int uPhyNum )
{
  unsigned int uUnitNum = uXcInst * PHYCTRL_PORT_CNT + uPhyNum;

  if(uXcInst > 1) return 0;
  if(uPhyNum > 1) return 0;
  return s_aptPhyCtrl[uUnitNum]->ulInt_phy_ctrl_led & 0xff;
}

/*****************************************************************************/
/*! Get Link status
* \description
*   Returns the Link status
* \class
*   PHYCTRL
* \params
*   uPhyNum          [in]  Number of internal PHY
* \return
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(PHYCTRL_GetLinkState)( unsigned int uXcInst,
                                       unsigned int uPhyNum,
                                       unsigned int* puiLinkUp,
                                       unsigned int* puiSpeed,
                                       unsigned int* puiIsFullDuplex)
{
  unsigned int uUnitNum = uXcInst * PHYCTRL_PORT_CNT + uPhyNum;
  uint32_t ulValue;

  if(uXcInst > 1) return 0;
  if(uPhyNum > 1) return 0;

  ulValue = s_aptPhyCtrl[uUnitNum]->ulInt_phy_ctrl_led;
  *puiLinkUp       = (ulValue & HW_MSK(int_phy_ctrl_led_link_ro)     ) ? 1   : 0;
  *puiSpeed        = (ulValue & HW_MSK(int_phy_ctrl_led_speed100_ro) ) ? 100 : 10;
  *puiIsFullDuplex = (ulValue & HW_MSK(int_phy_ctrl_led_duplex_ro)   ) ? 1   : 0;

  return 0;
}

/*****************************************************************************/
/*! Configure LED Blink Generator
* \description
*   Configures the PHY LED blink generator.
* \class
*   PHYCTRL
* \params
*   uPhyNum          [in]  Number of internal PHY
*   eBlinkMode       [in]  LED blinking mode
*   uBlinkInterval   [in]  Blink interval in 10 milliseconds steps (0=10ms, 15=160ms)
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(PHYCTRL_ConfigLedBlink)( unsigned int       uXcInst,
                                    unsigned int       uPhyNum,
                                    PHYCTRL_LED_MODE_E eBlinkMode,
                                    uint32_t           ulBlinkInterval )
{
  unsigned int uUnitNum = uXcInst * PHYCTRL_PORT_CNT + uPhyNum;

  if(uXcInst > 1) return -1;
  if(uPhyNum > 1) return -1;
  if (ulBlinkInterval > 15) return -1;

  s_aptPhyCtrl[uUnitNum]->ulInt_phy_ctrl_led = (ulBlinkInterval << HW_SRT(int_phy_ctrl_led_interval))
                                              |((uint32_t)eBlinkMode << HW_SRT(int_phy_ctrl_led_mode));

  return 0;
}


/*****************************************************************************/
/*! Manually Set PHY LED
* \description
*   Sets the PHY LED manually if they are in INT_PHYCTRL_BLINK_MODE_MANUAL mode.
* \class
*   PHYCTRL
* \params
*   uPhyNum          [in]  Number of internal PHY
*   fLed0            [in]  State of LED0
*   fLed1            [in]  State of LED1
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(PHYCTRL_ManualLedSet)( unsigned int uXcInst,
                                  unsigned int uPhyNum,
                                  bool         fLed0,
                                  bool         fLed1 )
{
  uint32_t  ulVal;
  unsigned int uUnitNum = uXcInst * PHYCTRL_PORT_CNT + uPhyNum;

  if(uXcInst > 1) return -1;
  if(uPhyNum > 1) return -1;

  ulVal = s_aptPhyCtrl[uUnitNum]->ulInt_phy_ctrl_led;
  ulVal &= ~(HW_MSK(int_phy_ctrl_led_led0) | HW_MSK(int_phy_ctrl_led_led1));

  if (!fLed0) { ulVal |= HW_MSK(int_phy_ctrl_led_led0); }
  if (!fLed1) { ulVal |= HW_MSK(int_phy_ctrl_led_led1); }

  s_aptPhyCtrl[uUnitNum]->ulInt_phy_ctrl_led = ulVal;

  return 0;
}

/*****************************************************************************/
/*! Get ELD status from Internal PHY
* \description
*   Returns the ELD status
* \class
*   PHYCTRL
* \params
*   uPhyNum          [in]  Number of internal PHY
*   pfBadLink        [out] Buffer for Bad link
*   uPhyNum          [out  Buffer for Counter value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(PHYCTRL_GetEldStatus)( unsigned int uXcInst,
                                  unsigned int uPhyNum,
                                  bool* pfBadLink,
                                  unsigned short* pusCounter )
{
  unsigned int uPortNo = uXcInst * PHYCTRL_PORT_CNT + uPhyNum;
  uint32_t ulVal;

  if(uXcInst > 1) return -1;
  if(uPhyNum > 1) return -1;

  ulVal = s_aptPhyCtrl[uPortNo]->ulInt_phy_ctrl_enhanced_link_detection;
  *pusCounter = (uint16_t)((ulVal & HW_MSK(int_phy_ctrl_enhanced_link_detection_counter_ro))>>HW_SRT(int_phy_ctrl_enhanced_link_detection_counter_ro));
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
*   uPhyNum          [in]  Number of internal PHY
*   bAdd             [in]  Add
*   bSub             [in]  Sub
*   usTreshold       [in]  TH
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(PHYCTRL_ConfigEld)( unsigned int   uXcInst,
                               unsigned int   uPhyNum,
                               unsigned char  bAdd,
                               unsigned char  bSub,
                               unsigned short usTreshold )
{
  unsigned int uPortNo = uXcInst * PHYCTRL_PORT_CNT + uPhyNum;

  if(uXcInst > 1) return -1;
  if(uPhyNum > 1) return -1;
  if(bAdd > 31) return -1;
  if(bSub > 3) return -1;
  if(usTreshold > 4095) return -1;

  s_aptPhyCtrl[uPortNo]->ulInt_phy_ctrl_enhanced_link_detection =  (    (uint32_t)bAdd << HW_SRT(int_phy_ctrl_enhanced_link_detection_add))
                                                                     |( (uint32_t)bSub << HW_SRT(int_phy_ctrl_enhanced_link_detection_sub))
                                                                     |( (uint32_t)usTreshold << HW_SRT(int_phy_ctrl_enhanced_link_detection_threshold));

  return 0;
}

