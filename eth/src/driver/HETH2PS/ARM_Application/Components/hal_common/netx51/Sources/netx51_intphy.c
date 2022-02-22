/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_intphy.h"
#include "hal_resources_defines_netx51.h"

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
void NX51_INTPHY_Init( uint32_t ulPhyCtrlInit )
{
  uint16_t usMiimuData;
  unsigned int uDelayCnt;
  unsigned int uPhyAdr = ((ulPhyCtrlInit & MSK_NX51_phy_control_phy_address) >> SRT_NX51_phy_control_phy_address) << 1;

  /* write value */
  s_ptAsicCtrl->ulPhy_control = ulPhyCtrlInit | MSK_NX51_phy_control_phy_reset;

  /* delay for 100us -> do 5 MII transfers of 24us each */
  for(uDelayCnt = 5; uDelayCnt > 0; --uDelayCnt)
  {
    /*lint -e(534) Ignoring return value */
    NX51_MIIMU_ReadPhyReg( NX51_MIIMU_SEND_PREAMBLE,
                           NX51_MIIMU_MDCFREQ_LOW, /* 400ns MDC-period */
                           NX51_MIIMU_RTA_1_BITS,
                           NX51_INT_PHY1_ADDR,
                           NX51_DRV_CB12_CONTROL,
                           &usMiimuData,
                           0
                          );
  }

  if( (ulPhyCtrlInit & MSK_NX51_phy_control_phy_reset) != 0 )
  {
    /* PHY reset requested -> nothing left to do */
    return;
  }

  /* write value */
  s_ptAsicCtrl->ulPhy_control = ulPhyCtrlInit & ~MSK_NX51_phy_control_phy_reset;

  /* wait for PHY 0 ready after reset if PHY is enabled */
  if( (ulPhyCtrlInit & MSK_NX51_phy_control_phy0_enable) != 0 )
  {
    /* wait until MIIMU access to PHY 0 successfully */
    do {
      /*lint -e(534) Ignoring return value */
      NX51_MIIMU_ReadPhyReg( NX51_MIIMU_SEND_PREAMBLE,
                             NX51_MIIMU_MDCFREQ_LOW, /* 400ns MDC-period */
                             NX51_MIIMU_RTA_1_BITS,
                             uPhyAdr,
                             NX51_DRV_CB12_SILICON_REVISION,
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
      /*lint -e(534) Ignoring return value */
      NX51_MIIMU_ReadPhyReg( NX51_MIIMU_SEND_PREAMBLE,
                             NX51_MIIMU_MDCFREQ_LOW, /* 400ns MDC-period */
                             NX51_MIIMU_RTA_1_BITS,
                             uPhyAdr + 1,
                             NX51_DRV_CB12_SILICON_REVISION,
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
uint32_t NX51_INTPHY_GetLedStatus( unsigned int uPhyNum )
{
  if(uPhyNum>1) return 0;
  return s_aptIntPhyCtrl[uPhyNum]->ulInt_phy_ctrl_led&0xff;
}
/*****************************************************************************/
/*! Configure LED Blink Generator
* \description
*   Configures the PHY LED blink generator.
* \class
*   INTPHY
* \params
*   uPhyNum          [in]  Number of internal PHY
*   eBlinkMode       [in]  LED blinking mode
*   uBlinkInterval   [in]  Blink interval in 10 milliseconds steps (0=10ms, 15=160ms)
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_INTPHY_ConfigLedBlink( unsigned int                uPhyNum,
                                NX51_INT_PHYCTRL_LED_MODE_E eBlinkMode,
                                unsigned int                uBlinkInterval )
{
  if(uPhyNum>1) return -1;

  if (uBlinkInterval > 15) return -1;

  s_aptIntPhyCtrl[uPhyNum]->ulInt_phy_ctrl_led = ((uint32_t)uBlinkInterval << SRT_NX51_int_phy_ctrl_led_interval) |
                                                 ((uint32_t)eBlinkMode << SRT_NX51_int_phy_ctrl_led_mode);

  return 0;
}


/*****************************************************************************/
/*! Manually Set PHY LED
* \description
*   Sets the PHY LED manually if they are in NX51_INT_PHYCTRL_BLINK_MODE_MANUAL mode.
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
int NX51_INTPHY_ManualLedSet( unsigned int uPhyNum,
                              bool         fLed0,
                              bool         fLed1 )
{
  uint32_t  ulVal;

  if(uPhyNum>1) return -1;


  ulVal = s_aptIntPhyCtrl[uPhyNum]->ulInt_phy_ctrl_led;
  ulVal &= ~(MSK_NX51_int_phy_ctrl_led_led0 | MSK_NX51_int_phy_ctrl_led_led1);

  if (!fLed0) { ulVal |= MSK_NX51_int_phy_ctrl_led_led0; }
  if (!fLed1) { ulVal |= MSK_NX51_int_phy_ctrl_led_led1; }

  s_aptIntPhyCtrl[uPhyNum]->ulInt_phy_ctrl_led = ulVal;

  return 0;
}
