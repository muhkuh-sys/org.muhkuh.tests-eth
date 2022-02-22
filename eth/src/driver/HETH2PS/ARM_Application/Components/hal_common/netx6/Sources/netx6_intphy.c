/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_intphy.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_ASIC_CTRL
__USE_INT_PHY_CTRL

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Initialize Internal PHY
* \description
*   Initialize internal PHY.
* \class
*   INTPHY
* \params
*   ulPhyCtrlInit          [in]  PHY Control Initialize Value
* \return
*                                                                            */
/*****************************************************************************/
void NX6_INTPHY_Init( uint32_t ulPhyCtrlInit )
{
  uint16_t usMiimuData;
  unsigned int ulDelayCnt;

  /* write value */
  NX_WRITE32(s_ptAsicCtrl->ulPhy_control, ulPhyCtrlInit | MSK_NX51_phy_control_phy_reset);

  /* delay for 100us -> do 5 MIIM transfers of 24us each */
  ulDelayCnt = 5;
  do
  {
    NX6_MIIMU_ReadPhyReg( NX6_MIIMU_SEND_PREAMBLE,
                          NX6_MIIMU_MDCFREQ_LOW,
                          NX6_INT_PHY1_ADDR,
                          NX6_DRV_CB12_CONTROL,
                          &usMiimuData,
                          0 );
  } while( ulDelayCnt--!=0 );

  /* write value */
  NX_WRITE32(s_ptAsicCtrl->ulPhy_control, ulPhyCtrlInit & ~MSK_NX51_phy_control_phy_reset);

  /* wait for PHY 0 ready after reset if PHY is enabled */
  if( (ulPhyCtrlInit & MSK_NX51_phy_control_phy0_enable) != 0 )
  {
    /* wait until MIIMU access to PHY 0 successfully */
    do {
      NX6_MIIMU_ReadPhyReg( NX6_MIIMU_SEND_PREAMBLE,
                            NX6_MIIMU_MDCFREQ_LOW, /* 400ns MDC-period */
                            NX6_INT_PHY1_ADDR,
                            NX6_DRV_CB12_SILICON_REVISION,
                            &usMiimuData,
                            0
                          );
    } while( (usMiimuData==0) || (usMiimuData==0xffff) );
  }

  /* wait for PHY 1 ready after reset if PHY is enabled */
  if( (ulPhyCtrlInit & MSK_NX51_phy_control_phy1_enable) != 0 )
  {
    /* wait until MIIMU access to PHY 1 successfully */
    do {
      NX6_MIIMU_ReadPhyReg( NX6_MIIMU_SEND_PREAMBLE,
                            NX6_MIIMU_MDCFREQ_LOW, /* 400ns MDC-period */
                            NX6_INT_PHY2_ADDR,
                            NX6_DRV_CB12_SILICON_REVISION,
                            &usMiimuData,
                            0
                          );
    } while( (usMiimuData==0) || (usMiimuData==0xffff) );
  }

}

/*****************************************************************************/
/*! Get LED status from Internal PHY
* \description
*   Returns the LED status
* \class
*   INTPHY
* \params
*   uPhyNum          [in]  Number of internal PHY
* \return
*   0 on erroneous
*   Current led_status value                                                 */
/*****************************************************************************/
uint32_t NX6_INTPHY_GetLedStatus( unsigned int uPhyNum )
{
  if(uPhyNum>1) return 0;
  return NX_READ32(s_aptIntPhyCtrl[uPhyNum]->ulInt_phy_ctrl_led) & 0xff;
}
/*****************************************************************************/
/*! Configure LED Blink Generator
* \description
*   Configures the PHY LED blink generator.
* \class
*   INTPHY
* \params
*   uPhyNum          [in]  Number of internal PHY
*   ulBlinkMode      [in]  LED flashing mode
*   ulBlinkInterval  [in]  Blink interval in 10 milliseconds steps (0=10ms, 15=160ms)
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_INTPHY_ConfigLedBlink( unsigned int uPhyNum,
                               NX6_INT_PHYCTRL_LED_MODE_E eBlinkMode,
                               uint32_t ulBlinkInterval )
{
  if(uPhyNum>1) return -1;

  if (ulBlinkInterval > 15) return -1;

  NX_WRITE32(s_aptIntPhyCtrl[uPhyNum]->ulInt_phy_ctrl_led, (ulBlinkInterval << SRT_NX51_int_phy_ctrl_led_interval)
                                                          |(eBlinkMode << SRT_NX51_int_phy_ctrl_led_mode)
            );

  return 0;
}


/*****************************************************************************/
/*! Manually Set PHY LED
* \description
*   Sets the PHY LED manually if they are in NX6_INT_PHYCTRL_BLINK_MODE_MANUAL mode.
* \class
*   INTPHY
* \params
*   uPhyNum          [in]  Number of internal PHY
*   fLed0            [in]  State of LED0
*   fLed1            [in]  State of LED1
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_INTPHY_ManualLedSet( unsigned int uPhyNum,
                             int          fLed0,
                             int          fLed1 )
{
  uint32_t  ulVal;

  if(uPhyNum>1) return -1;

  ulVal = NX_READ32(s_aptIntPhyCtrl[uPhyNum]->ulInt_phy_ctrl_led);
  ulVal &= ~(MSK_NX51_int_phy_ctrl_led_led0 | MSK_NX51_int_phy_ctrl_led_led1);

  if (fLed0 != 0) { ulVal |= MSK_NX51_int_phy_ctrl_led_led0; }
  if (fLed1 != 0) { ulVal |= MSK_NX51_int_phy_ctrl_led_led1; }

  NX_WRITE32(s_aptIntPhyCtrl[uPhyNum]->ulInt_phy_ctrl_led, ulVal);

  return 0;
}
