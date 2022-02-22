/* Includes */
#include "hal_miimu.h"
#include "hw_defines.h"

/* Definitions */
#define EXT_PHY0_ADDR 2
#define EXT_PHY1_ADDR 3

/* Variables */
HW_PTR_ASIC_CTRL(s_ptAsicCtrl_miimu)
HW_PTR_ETH(s_ptEth)
HW_PTR_PHY_CTRL(s_aptPhyCtrl)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Read PHY Register
* \description
*   Reads PHY register over MDIO.
* \class
*   MIIMU
* \params
*   uMiimuPreamble              [in]  MIIMU PREAMBLE
*   uMiimuMdcFreq               [in]  MIIMU MDC Frequency
*   uMiimuRtaField              [in]  MIIMU RTA Field
*   uMiimuPhyAddr               [in]  MIIMU PHY Address
*   uMiimuReqAddr               [in]  MIIMU Register Address
*   pusData                     [out] MIIMU Data
*   pvUser                      [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int MIIMU_ReadPhyReg( unsigned int uMiimuPreamble,
                              unsigned int uMiimuMdcFreq,
                              unsigned int uMiimuRtaField,
                              unsigned int uMiimuPhyAddr,
                              unsigned int uMiimuReqAddr,
                              uint16_t*    pusData,
                              void*        pvUser )
{
  uint32_t ulMiimuUnitSel = 2;

  /* check if it's an internal PHY (internal PHYs clamped to Address b0000[0|1]) */
  if (uMiimuPhyAddr < 2) ulMiimuUnitSel = 0; /* use XC0.Port0 MDIO per default */

  if (ulMiimuUnitSel == 2)
  {
    /* external PHY */
    s_ptEth->ulEth_miimu = ((uint32_t)uMiimuPreamble   << HW_SRT(eth_miimu_preamble))
                        |((uint32_t)uMiimuMdcFreq    << HW_SRT(eth_miimu_mdc_period))
                        |((uint32_t)uMiimuRtaField   << HW_SRT(eth_miimu_rta))
                        |((uint32_t)uMiimuReqAddr    << HW_SRT(eth_miimu_regaddr))
                        |((uint32_t)uMiimuPhyAddr    << HW_SRT(eth_miimu_phyaddr))
                        |HW_MSK(eth_miimu_snrdy);

    while ((s_ptEth->ulEth_miimu & HW_MSK(eth_miimu_snrdy)) != 0);

    *pusData = (uint16_t) ((s_ptEth->ulEth_miimu & HW_MSK(eth_miimu_data)) >> HW_SRT(eth_miimu_data));
  }
  else
  {
    /* exotic case: INTPHY connected with XC0.Port1 MDIO */
    if( 2 == (s_ptAsicCtrl_miimu->asIo_config[1].ulConfig & HW_MSK(io_config1_mask_sel_xc1_mdio)) >> HW_SRT(io_config1_mask_sel_xc1_mdio)) {
      ulMiimuUnitSel = 1;
    }

    /* internal PHYs */
    s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu =    ((uint32_t)uMiimuPreamble   << HW_SRT(int_phy_ctrl_miimu_preamble))
                                                           |((uint32_t)uMiimuMdcFreq    << HW_SRT(int_phy_ctrl_miimu_mdc_period))
                                                           |((uint32_t)uMiimuRtaField   << HW_SRT(int_phy_ctrl_miimu_rta))
                                                           |((uint32_t)uMiimuReqAddr    << HW_SRT(int_phy_ctrl_miimu_regaddr))
                                                           |((uint32_t)uMiimuPhyAddr    << HW_SRT(int_phy_ctrl_miimu_phyaddr))
                                                           |HW_MSK(int_phy_ctrl_miimu_snrdy);

    while ((s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu & HW_MSK(int_phy_ctrl_miimu_snrdy)) != 0);

    *pusData = (uint16_t) ((s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu & HW_MSK(int_phy_ctrl_miimu_data)) >> HW_SRT(int_phy_ctrl_miimu_data));
  }

  return 0;
}

/*****************************************************************************/
/*! Write PHY Register
* \description
*   Write PHY register over MDIO.
* \class
*   MIIMU
* \params
*   uMiimuPreamble              [in]  MIIMU PREAMBLE
*   uMiimuMdcFreq               [in]  MIIMU MDC Frequency
*   uMiimuWtaField              [in]  MIIMU Turn Around Field
*   uMiimuPhyAddr               [in]  MIIMU PHY Address
*   uMiimuReqAddr               [in]  MIIMU Register Address
*   usData                      [in]  MIIMU Data
*   pvUser                      [in]  User specific parameter
* \return
*   0 on success                                                             */
/*****************************************************************************/
int MIIMU_WritePhyReg( unsigned int uMiimuPreamble,
                               unsigned int uMiimuMdcFreq,
                               unsigned int uMiimuWtaField,
                               unsigned int uMiimuPhyAddr,
                               unsigned int uMiimuReqAddr,
                               uint16_t     usData,
                               void*        pvUser )
{
  uint32_t ulMiimuUnitSel = 2;

  /* check if it's an internal PHY (internal PHYs clamped to Address b0000[0|1]) */
  if (uMiimuPhyAddr < 2) ulMiimuUnitSel = 0; /* use XC0.Port0 MDIO per default */

  if (ulMiimuUnitSel == 2)
  {
    /* external PHY */
    s_ptEth->ulEth_miimu = ((uint32_t)uMiimuPreamble << HW_SRT(eth_miimu_preamble))
                        |((uint32_t)uMiimuMdcFreq  << HW_SRT(eth_miimu_mdc_period))
                        |((uint32_t)uMiimuWtaField << HW_SRT(eth_miimu_rta))
                        |((uint32_t)uMiimuReqAddr  << HW_SRT(eth_miimu_regaddr))
                        |((uint32_t)uMiimuPhyAddr  << HW_SRT(eth_miimu_phyaddr))
                        |((uint32_t)usData         << HW_SRT(eth_miimu_data))
                        |HW_MSK(eth_miimu_opmode)
                        |HW_MSK(eth_miimu_snrdy);

    while ((s_ptEth->ulEth_miimu & HW_MSK(eth_miimu_snrdy)) != 0);
  }
  else
  {
    /* exotic case: INTPHY connected with XC0.Port1 MDIO */
    if( 2 == (s_ptAsicCtrl_miimu->asIo_config[1].ulConfig & HW_MSK(io_config1_mask_sel_xc1_mdio)) >> HW_SRT(io_config1_mask_sel_xc1_mdio)) {
      ulMiimuUnitSel = 1;
    }

    /* internal PHYs */
    s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu = ((uint32_t)uMiimuPreamble << HW_SRT(int_phy_ctrl_miimu_preamble))
                                                           |((uint32_t)uMiimuMdcFreq  << HW_SRT(int_phy_ctrl_miimu_mdc_period))
                                                           |((uint32_t)uMiimuWtaField << HW_SRT(eth_miimu_rta))
                                                           |((uint32_t)uMiimuReqAddr  << HW_SRT(int_phy_ctrl_miimu_regaddr))
                                                           |((uint32_t)uMiimuPhyAddr  << HW_SRT(int_phy_ctrl_miimu_phyaddr))
                                                           |((uint32_t)usData         << HW_SRT(int_phy_ctrl_miimu_data))
                                                           |HW_MSK(int_phy_ctrl_miimu_opmode)
                                                           |HW_MSK(int_phy_ctrl_miimu_snrdy);

    while ((s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu & HW_MSK(int_phy_ctrl_miimu_snrdy)) != 0);
  }

  return 0;
}

/*****************************************************************************/
/*! Read PHY Register
* \description
*   Reads PHY register over MIIMU_SW.
* \class
*   MIIMU
* \params
*   uMiimuPhyAddr               [in]  MIIMU PHY Address
*   uMiimuReqAddr               [in]  MIIMU Register Address
*   pusData                     [out] MIIMU Data
*   pvUser                      [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int MIIMU_ReadPhyRegSw( unsigned int uMiimuPhyAddr,
                                unsigned int uMiimuReqAddr,
                                uint16_t*    pusData,
                                void*        pvUser )
{
  unsigned long ulMiimuUnitSel = 2;
  volatile unsigned long* pulMiimuSwReg;
  unsigned int uCnt, uVal, uDelay;

  /* check if it's an internal PHY (internal PHYs clamped to Address b0000[0|1]) */
  if (uMiimuPhyAddr < 2) ulMiimuUnitSel = 0; /* use XC0.Port0 MDIO per default */

  if (ulMiimuUnitSel == 2)
  {
    /* external PHY */
    pulMiimuSwReg = (volatile unsigned long*) &(s_ptEth->ulEth_miimu_sw);
  }
  else
  {
    /* exotic case: INTPHY connected with XC0.Port1 MDIO */
    if( 2 == (s_ptAsicCtrl_miimu->asIo_config[1].ulConfig & HW_MSK(io_config1_mask_sel_xc1_mdio)) >> HW_SRT(io_config1_mask_sel_xc1_mdio)) {
      ulMiimuUnitSel = 1;
    }

    /* internal PHYs */
    pulMiimuSwReg = (volatile unsigned long*) &(s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu_sw);
  }

  uDelay=2;

  /* send PREAMBLE */
  uCnt=32;
  do {
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |HW_MSK(eth_miimu_sw_mdo);
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|HW_MSK(eth_miimu_sw_mdo);
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* start */
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);

  /* command */
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);

  /* PHY address */
  uCnt=5;
  do {
    uVal=((uMiimuPhyAddr>>(uCnt-1))&1)<<HW_SRT(eth_miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* REG address */
  uCnt=5;
  do {
    uVal=((uMiimuReqAddr>>(uCnt-1))&1)<<HW_SRT(eth_miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* TA */
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);

  /* receive Data */
  *pusData=0;
  uCnt=16;
  do {
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable);
    SYS_SLEEP_CC(uDelay);
    if( *pulMiimuSwReg&HW_MSK(eth_miimu_sw_mdi_ro) ) *pusData |= (uint16_t)(1<<(uCnt-1));
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdc);
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* deactivate interface */
  *pulMiimuSwReg=0;

  return 0;
}

/*****************************************************************************/
/*! Write PHY Register
* \description
*   Write PHY register over MIIMU_SW.
* \class
*   MIIMU
* \params
*   uMiimuPhyAddr               [in]  MIIMU PHY Address
*   uMiimuReqAddr               [in]  MIIMU Register Address
*   usData                      [in]  MIIMU Data
*   pvUser                      [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int MIIMU_WritePhyRegSw( unsigned int uMiimuPhyAddr,
                                 unsigned int uMiimuReqAddr,
                                 uint16_t     usData,
                                 void*        pvUser )
{
  unsigned long ulMiimuUnitSel = 2;
  volatile unsigned long* pulMiimuSwReg;
  unsigned int uCnt, uVal, uDelay;

  /* check if it's an internal PHY (internal PHYs clamped to Address b0000[0|1]) */
  if (uMiimuPhyAddr < 2) ulMiimuUnitSel = 0; /* use XC0.Port0 MDIO per default */

  if (ulMiimuUnitSel == 2)
  {
    /* external PHY */
    pulMiimuSwReg = (volatile unsigned long*) &(s_ptEth->ulEth_miimu_sw);
  }
  else
  {
    /* exotic case: INTPHY connected with XC0.Port1 MDIO */
    if( 2 == (s_ptAsicCtrl_miimu->asIo_config[1].ulConfig & HW_MSK(io_config1_mask_sel_xc1_mdio)) >> HW_SRT(io_config1_mask_sel_xc1_mdio)) {
      ulMiimuUnitSel = 1;
    }

    /* internal PHYs */
    pulMiimuSwReg = (volatile unsigned long*) &(s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu_sw);

  }

  uDelay=2;

  /* PREAMBLE */
  uCnt=32;
  do {
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |HW_MSK(eth_miimu_sw_mdo);
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|HW_MSK(eth_miimu_sw_mdo);
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* start */
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);

  /* command */
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);

  /* PHY address */
  uCnt=5;
  do {
    uVal=((uMiimuPhyAddr>>(uCnt-1))&1)<<HW_SRT(eth_miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* REG address */
  uCnt=5;
  do {
    uVal=((uMiimuReqAddr>>(uCnt-1))&1)<<HW_SRT(eth_miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* send TA */
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|HW_MSK(eth_miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);

  /* send Data */
  uCnt=16;
  do {
    uVal=(unsigned int)((usData>>(uCnt-1))&1)<<HW_SRT(eth_miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)                     |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(eth_miimu_sw_enable)|HW_MSK(eth_miimu_sw_mdoe)|HW_MSK(eth_miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* deactivate interface */
  *pulMiimuSwReg=0;

  return 0;
}
