/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_miimu.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Definitions                                                              */
/*****************************************************************************/
#define GEN_DELAY(val) { unsigned int uDelayCnt; for( uDelayCnt=0; uDelayCnt<val; uDelayCnt++ ); }

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_ASIC_CTRL
__USE_MIIMU
__USE_INT_PHY_CTRL

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
int NX51_MIIMU_ReadPhyReg( unsigned int uMiimuPreamble,
                           unsigned int uMiimuMdcFreq,
                           unsigned int uMiimuRtaField,
                           unsigned int uMiimuPhyAddr,
                           unsigned int uMiimuReqAddr,
                           uint16_t*    pusData,
                           void*        pvUser )
{
  uint32_t aulIntPhyAddr[2];
  uint32_t ulMiimuUnitSel = 2;

  /* get internal phy addresses */
  aulIntPhyAddr[0] = ((s_ptAsicCtrl->ulPhy_control & MSK_NX51_phy_control_phy_address) >> SRT_NX51_phy_control_phy_address) << 1;
  aulIntPhyAddr[1] = aulIntPhyAddr[0] + 1;

  /* check if it's an internal PHY */
  if (uMiimuPhyAddr == aulIntPhyAddr[0]) ulMiimuUnitSel = 0;
  if (uMiimuPhyAddr == aulIntPhyAddr[1]) ulMiimuUnitSel = 1;

  if (ulMiimuUnitSel == 2)
  {
    /* external PHY */
    s_ptMiimu->ulMiimu = ((uint32_t)uMiimuPreamble   << SRT_NX51_miimu_preamble)
                        |((uint32_t)uMiimuMdcFreq    << SRT_NX51_miimu_mdc_period)
                        |((uint32_t)uMiimuRtaField   << SRT_NX51_miimu_rta)
                        |((uint32_t)uMiimuReqAddr    << SRT_NX51_miimu_regaddr)
                        |((uint32_t)uMiimuPhyAddr    << SRT_NX51_miimu_phyaddr)
                        |MSK_NX51_miimu_snrdy;

    while ((s_ptMiimu->ulMiimu & MSK_NX51_miimu_snrdy) != 0);

    *pusData = (uint16_t) ((s_ptMiimu->ulMiimu & MSK_NX51_miimu_data) >> SRT_NX51_miimu_data);
  }
  else
  {
    /* internal PHYs */
    s_aptIntPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu = ((uint32_t)uMiimuPreamble   << SRT_NX51_int_phy_ctrl_miimu_preamble)
                                                           |((uint32_t)uMiimuMdcFreq    << SRT_NX51_int_phy_ctrl_miimu_mdc_period)
                                                           |((uint32_t)uMiimuRtaField   << SRT_NX51_int_phy_ctrl_miimu_rta)
                                                           |((uint32_t)uMiimuReqAddr    << SRT_NX51_int_phy_ctrl_miimu_regaddr)
                                                           |((uint32_t)uMiimuPhyAddr    << SRT_NX51_int_phy_ctrl_miimu_phyaddr)
                                                           |MSK_NX51_int_phy_ctrl_miimu_snrdy;

    while ((s_aptIntPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu & MSK_NX51_int_phy_ctrl_miimu_snrdy) != 0);

    *pusData = (uint16_t) ((s_aptIntPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu & MSK_NX51_int_phy_ctrl_miimu_data) >> SRT_NX51_int_phy_ctrl_miimu_data);
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
int NX51_MIIMU_WritePhyReg( unsigned int uMiimuPreamble,
                            unsigned int uMiimuMdcFreq,
                            unsigned int uMiimuWtaField,
                            unsigned int uMiimuPhyAddr,
                            unsigned int uMiimuReqAddr,
                            uint16_t     usData,
                            void*        pvUser )
{
  uint32_t aulIntPhyAddr[2];
  uint32_t ulMiimuUnitSel = 2;

  /* get internal PHY addresses */
  aulIntPhyAddr[0] = ((s_ptAsicCtrl->ulPhy_control & MSK_NX51_phy_control_phy_address) >> SRT_NX51_phy_control_phy_address) << 1;
  aulIntPhyAddr[1] = aulIntPhyAddr[0] + 1;

  /* check if it's an internal PHY */
  if (uMiimuPhyAddr == aulIntPhyAddr[0]) ulMiimuUnitSel = 0;
  if (uMiimuPhyAddr == aulIntPhyAddr[1]) ulMiimuUnitSel = 1;

  if (ulMiimuUnitSel == 2)
  {
    /* external PHY */
    s_ptMiimu->ulMiimu = ((uint32_t)uMiimuPreamble << SRT_NX51_miimu_preamble)
                        |((uint32_t)uMiimuMdcFreq  << SRT_NX51_miimu_mdc_period)
                        |((uint32_t)uMiimuWtaField << SRT_NX51_miimu_rta)
                        |((uint32_t)uMiimuReqAddr  << SRT_NX51_miimu_regaddr)
                        |((uint32_t)uMiimuPhyAddr  << SRT_NX51_miimu_phyaddr)
                        |((uint32_t)usData         << SRT_NX51_miimu_data)
                        |MSK_NX51_miimu_opmode
                        |MSK_NX51_miimu_snrdy;

    while ((s_ptMiimu->ulMiimu & MSK_NX51_miimu_snrdy) != 0);
  }
  else
  {
    /* internal PHYs */
    s_aptIntPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu = ((uint32_t)uMiimuPreamble << SRT_NX51_int_phy_ctrl_miimu_preamble)
                                                           |((uint32_t)uMiimuMdcFreq  << SRT_NX51_int_phy_ctrl_miimu_mdc_period)
                                                           |((uint32_t)uMiimuWtaField << SRT_NX51_miimu_rta)
                                                           |((uint32_t)uMiimuReqAddr  << SRT_NX51_int_phy_ctrl_miimu_regaddr)
                                                           |((uint32_t)uMiimuPhyAddr  << SRT_NX51_int_phy_ctrl_miimu_phyaddr)
                                                           |((uint32_t)usData         << SRT_NX51_int_phy_ctrl_miimu_data)
                                                           |MSK_NX51_int_phy_ctrl_miimu_opmode
                                                           |MSK_NX51_int_phy_ctrl_miimu_snrdy;

    while ((s_aptIntPhyCtrl[ulMiimuUnitSel]->ulInt_phy_ctrl_miimu & MSK_NX51_int_phy_ctrl_miimu_snrdy) != 0);
  }
  return 0;
}
