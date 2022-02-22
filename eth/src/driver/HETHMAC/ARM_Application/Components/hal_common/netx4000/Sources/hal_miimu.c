/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_miimu.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Definitions                                                              */
/*****************************************************************************/
#define EXT_PHY0_ADDR 2
#define EXT_PHY1_ADDR 3

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_ASIC_CTRL(s_ptAsicCtrl)
HW_PTR_MIIMU(s_ptMiimu)
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
#ifndef HALDEF
#define HALDEF(name) name
#endif

int HALDEF(MIIMU_ReadPhyReg)( unsigned int uMiimuPreamble,
                             unsigned int uMiimuMdcFreq,
                             unsigned int uMiimuRtaField,
                             unsigned int uMiimuPhyAddr,
                             unsigned int uMiimuReqAddr,
                             uint16_t*    pusData,
                             void*        pvUser )
{
  uint32_t ulMiimuUnitSel = 4;

  /* check if it's an internal PHY (internal PHYs clamped to Address b0000[0|1]) */
  if (uMiimuPhyAddr == 0) ulMiimuUnitSel = 0;
  if (uMiimuPhyAddr == 1) ulMiimuUnitSel = 1;
  if (uMiimuPhyAddr == EXT_PHY0_ADDR)    ulMiimuUnitSel = 2;
  if (uMiimuPhyAddr == EXT_PHY1_ADDR)    ulMiimuUnitSel = 3;

  if (ulMiimuUnitSel == 4)
  {
    /* external PHY */
    s_ptMiimu->ulMiimu = ((uint32_t)uMiimuPreamble   << HW_SRT(miimu_preamble))
                        |((uint32_t)uMiimuMdcFreq    << HW_SRT(miimu_mdc_period))
                        |((uint32_t)uMiimuRtaField   << HW_SRT(miimu_rta))
                        |((uint32_t)uMiimuReqAddr    << HW_SRT(miimu_regaddr))
                        |((uint32_t)uMiimuPhyAddr    << HW_SRT(miimu_phyaddr))
                        |HW_MSK(miimu_snrdy);

    while ((s_ptMiimu->ulMiimu & HW_MSK(miimu_snrdy)) != 0);
  
    *pusData = (uint16_t) ((s_ptMiimu->ulMiimu & HW_MSK(miimu_data)) >> HW_SRT(miimu_data));
  }
  else
  {
    /* internal PHYs */
    s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu = ((uint32_t)uMiimuPreamble   << HW_SRT(int_phy_ctrl_miimu_preamble))
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
int HALDEF(MIIMU_WritePhyReg)( unsigned int uMiimuPreamble,
                              unsigned int uMiimuMdcFreq,
                              unsigned int uMiimuWtaField,
                              unsigned int uMiimuPhyAddr,
                              unsigned int uMiimuReqAddr,
                              uint16_t     usData,
                              void*        pvUser )
{
  uint32_t ulMiimuUnitSel = 4;

  /* check if it's an internal PHY (internal PHYs clamped to Address b0000[0|1]) */
  if (uMiimuPhyAddr == 0) ulMiimuUnitSel = 0;
  if (uMiimuPhyAddr == 1) ulMiimuUnitSel = 1;
  if (uMiimuPhyAddr == EXT_PHY0_ADDR)    ulMiimuUnitSel = 2;
  if (uMiimuPhyAddr == EXT_PHY1_ADDR)    ulMiimuUnitSel = 3;

  if (ulMiimuUnitSel == 4)
  {
    /* external PHY */
    s_ptMiimu->ulMiimu = ((uint32_t)uMiimuPreamble << HW_SRT(miimu_preamble))
                        |((uint32_t)uMiimuMdcFreq  << HW_SRT(miimu_mdc_period))
                        |((uint32_t)uMiimuWtaField << HW_SRT(miimu_rta))
                        |((uint32_t)uMiimuReqAddr  << HW_SRT(miimu_regaddr))
                        |((uint32_t)uMiimuPhyAddr  << HW_SRT(miimu_phyaddr))
                        |((uint32_t)usData         << HW_SRT(miimu_data))
                        |HW_MSK(miimu_opmode)
                        |HW_MSK(miimu_snrdy);

    while ((s_ptMiimu->ulMiimu & HW_MSK(miimu_snrdy)) != 0);
  }
  else
  {
    /* internal PHYs */
    s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu = ((uint32_t)uMiimuPreamble << HW_SRT(int_phy_ctrl_miimu_preamble))
                                                           |((uint32_t)uMiimuMdcFreq  << HW_SRT(int_phy_ctrl_miimu_mdc_period))
                                                           |((uint32_t)uMiimuWtaField << HW_SRT(miimu_rta))
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
int HALDEF(MIIMU_ReadPhyRegSw)( unsigned int uMiimuPhyAddr,
                               unsigned int uMiimuReqAddr,
                               uint16_t*    pusData,
                               void*        pvUser )
{
  unsigned long ulMiimuUnitSel = 4;
  volatile unsigned long* pulMiimuSwReg;
  unsigned int uCnt, uVal, uDelay;
  
  /* check if it's an internal PHY (internal PHYs clamped to Address b0000[0|1]) */
  if (uMiimuPhyAddr == 0) ulMiimuUnitSel = 0;
  if (uMiimuPhyAddr == 1) ulMiimuUnitSel = 1;
  if (uMiimuPhyAddr == EXT_PHY0_ADDR)    ulMiimuUnitSel = 2;
  if (uMiimuPhyAddr == EXT_PHY1_ADDR)    ulMiimuUnitSel = 3;

  if (ulMiimuUnitSel == 4)
  {
    /* external PHY */
    pulMiimuSwReg = (volatile unsigned long*) &(s_ptMiimu->ulMiimu_sw);
  }
  else
  {
    /* internal PHYs */
    pulMiimuSwReg = (volatile unsigned long*) &(s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu_sw);
  }
  
  uDelay=2;
  
  /* send PREAMBLE */
  uCnt=32;
  do {
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |HW_MSK(miimu_sw_mdo);
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|HW_MSK(miimu_sw_mdo);
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* start */
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);

  /* command */
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);

  /* PHY address */
  uCnt=5;
  do {
    uVal=((uMiimuPhyAddr>>(uCnt-1))&1)<<HW_SRT(miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* REG address */
  uCnt=5;
  do {
    uVal=((uMiimuReqAddr>>(uCnt-1))&1)<<HW_SRT(miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* TA */
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);

  /* receive Data */
  *pusData=0;
  uCnt=16;
  do {
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable);
    SYS_SLEEP_CC(uDelay);
    if( *pulMiimuSwReg&HW_MSK(miimu_sw_mdi_ro) ) *pusData |= (uint16_t)(1<<(uCnt-1));
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdc);
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
int HALDEF(MIIMU_WritePhyRegSw)( unsigned int uMiimuPhyAddr,
                                unsigned int uMiimuReqAddr,
                                uint16_t     usData,
                                void*        pvUser )
{
  unsigned long ulMiimuUnitSel = 4;
  volatile unsigned long* pulMiimuSwReg;
  unsigned int uCnt, uVal, uDelay;
  
  /* check if it's an internal PHY (internal PHYs clamped to Address b0000[0|1]) */
  if (uMiimuPhyAddr == 0) ulMiimuUnitSel = 0;
  if (uMiimuPhyAddr == 1) ulMiimuUnitSel = 1;
  if (uMiimuPhyAddr == EXT_PHY0_ADDR)    ulMiimuUnitSel = 2;
  if (uMiimuPhyAddr == EXT_PHY1_ADDR)    ulMiimuUnitSel = 3;
  
  if (ulMiimuUnitSel == 4)
  {
    /* external PHY */
    pulMiimuSwReg = (volatile unsigned long*) &(s_ptMiimu->ulMiimu_sw);
  }
  else
  {
    /* internal PHYs */
    pulMiimuSwReg = (volatile unsigned long*) &(s_aptPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu_sw);

  }
  
  uDelay=2;

  /* PREAMBLE */
  uCnt=32;
  do {
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |HW_MSK(miimu_sw_mdo);
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|HW_MSK(miimu_sw_mdo);
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* start */
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);

  /* command */
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);

  /* PHY address */
  uCnt=5;
  do {
    uVal=((uMiimuPhyAddr>>(uCnt-1))&1)<<HW_SRT(miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* REG address */
  uCnt=5;
  do {
    uVal=((uMiimuReqAddr>>(uCnt-1))&1)<<HW_SRT(miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* send TA */
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|HW_MSK(miimu_sw_mdo);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe);
  SYS_SLEEP_CC(uDelay);
  *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc);
  SYS_SLEEP_CC(uDelay);

  /* send Data */
  uCnt=16;
  do {
    uVal=(unsigned int)((usData>>(uCnt-1))&1)<<HW_SRT(miimu_sw_mdo);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)                      |uVal;
    SYS_SLEEP_CC(uDelay);
    *pulMiimuSwReg = HW_MSK(miimu_sw_enable)|HW_MSK(miimu_sw_mdoe)|HW_MSK(miimu_sw_mdc)|uVal;
    SYS_SLEEP_CC(uDelay);
    uCnt--;
  } while( uCnt!=0 );

  /* deactivate interface */
  *pulMiimuSwReg=0;
  
  return 0;
}
