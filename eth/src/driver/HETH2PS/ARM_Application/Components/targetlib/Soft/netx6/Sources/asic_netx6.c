#include "asic_netx6.h"
#include "phy.h"
#include "hal_rdyrun.h"
#include "netx51_access_nx6.h"
#include "netx6_dpm.h"
#include "netx6_asicctrl.h"
#include "netx6_mmio.h"
#include "netx6_pfifo.h"
#include "netx6_systime.h"
#include "netx6_intphy.h"
#include "netx6_miimu.h"
#include "regdef_netx51.h"

#include <stdlib.h>

#define MIIMU_READ(phy,reg,data)  NX6_MIIMU_ReadPhyReg(NX6_MIIMU_SEND_PREAMBLE, NX6_MIIMU_MDCFREQ_HIGH, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) NX6_MIIMU_WritePhyReg(NX6_MIIMU_SEND_PREAMBLE, NX6_MIIMU_MDCFREQ_HIGH, phy, reg, data, NULL)

void netx6_init(void)
{
  unsigned int uiIdx;

  if( 0 != NX51_ACCESSNX6_SetupParallelDpm(32,   /* Data width */
                                           true, /* EnRdy */
                                           NX6_DPM_RDYPOL_READY_WHEN_HI, /* RDY polarity, only valid of EnRdy==1 */
                                           true /* INTRAMs as switch buffer required */) )
  {
    RDYRUN_SetRdyRunLed(RDYRUN_LED_RED);
    while(1) {}
  }

  /* enable all netX6 clocks */
  NX6_AsicCtrl_SetClockEnable( 0xf3333 );
  if( 0xf3333 != NX6_AsicCtrl_GetClockEnable() ) {
    RDYRUN_SetRdyRunLed(RDYRUN_LED_RED);
    while(1) {}
  }

  for(uiIdx = 0; uiIdx < 49; ++uiIdx)
  {
    NX6_MMIO_SetCfg(uiIdx, NX6_MMIO_CONFIG_INPUT, 0, 0);
  }

  /* Configure MMIO pin multiplexer (NXHX6) */
  NX6_MMIO_SetCfg(  0, NX6_MMIO_CONFIG_GPIO0,          0, 0 ); /* Rotary Switch "Function" */
  NX6_MMIO_SetCfg(  1, NX6_MMIO_CONFIG_GPIO1,          0, 0 ); /* Rotary Switch "Function" */
  NX6_MMIO_SetCfg(  2, NX6_MMIO_CONFIG_GPIO2,          0, 0 ); /* Rotary Switch "Function" */
  NX6_MMIO_SetCfg(  3, NX6_MMIO_CONFIG_GPIO3,          0, 0 ); /* Rotary Switch "Function" */
  NX6_MMIO_SetCfg(  4, NX6_MMIO_CONFIG_GPIO4,          0, 0 ); /* Rotary Switch "Address X1" */
  NX6_MMIO_SetCfg(  6, NX6_MMIO_CONFIG_GPIO5,          0, 0 ); /* Rotary Switch "Address X1" */
  NX6_MMIO_SetCfg(  5, NX6_MMIO_CONFIG_GPIO6,          0, 0 ); /* Rotary Switch "Address X1" */
  NX6_MMIO_SetCfg(  7, NX6_MMIO_CONFIG_GPIO7,          0, 0 ); /* Rotary Switch "Address X1" */
  NX6_MMIO_SetCfg(  8, NX6_MMIO_CONFIG_GPIO8,          0, 0 ); /* Rotary Switch "Address X10" */
  NX6_MMIO_SetCfg(  9, NX6_MMIO_CONFIG_GPIO9,          0, 0 ); /* Rotary Switch "Address X10" */
  NX6_MMIO_SetCfg( 10, NX6_MMIO_CONFIG_GPIO10,         0, 0 ); /* Rotary Switch "Address X10" */
  NX6_MMIO_SetCfg( 11, NX6_MMIO_CONFIG_GPIO11,         0, 0 ); /* Rotary Switch "Address X10" */
  NX6_MMIO_SetCfg( 12, NX6_MMIO_CONFIG_PIO0,           0, 0 ); /* COM 0 (low-active) LED Green */
  NX6_MMIO_SetCfg( 13, NX6_MMIO_CONFIG_PIO1,           0, 0 ); /* COM 0 (low-active) LED Red */
  NX6_MMIO_SetCfg( 14, NX6_MMIO_CONFIG_PIO2,           0, 0 ); /* COM 1 (low-active) LED Green */
  NX6_MMIO_SetCfg( 15, NX6_MMIO_CONFIG_PIO3,           0, 0 ); /* COM 1 (low-active) LED Red */
  NX6_MMIO_SetCfg( 16, NX6_MMIO_CONFIG_PHY0_LED0,      1, 0 ); /* ETH 0 green LED Link/Activity */
  NX6_MMIO_SetCfg( 17, NX6_MMIO_CONFIG_PHY0_LED1,      1, 0 ); /* ETH 0 yellow LED unused */
  NX6_MMIO_SetCfg( 18, NX6_MMIO_CONFIG_PHY1_LED0,      1, 0 ); /* ETH 1 green LED Link/Activity */
  NX6_MMIO_SetCfg( 19, NX6_MMIO_CONFIG_PHY1_LED1,      1, 0 ); /* ETH 1 yellow LED unused  */
  NX6_MMIO_SetCfg( 20, NX6_MMIO_CONFIG_GPIO20,         0, 0 ); /* Output LED 0 */
  NX6_MMIO_SetCfg( 21, NX6_MMIO_CONFIG_GPIO21,         0, 0 ); /* Output LED 1 */
  NX6_MMIO_SetCfg( 22, NX6_MMIO_CONFIG_XC_TRIGGER0,    0, 0 ); /* SYNC 0 */
  NX6_MMIO_SetCfg( 23, NX6_MMIO_CONFIG_XC_TRIGGER1,    0, 0 ); /* SYNC 1 */

  /* reset pointer FIFO borders */
  NX6_PFIFO_Reset();
  NX6_SYSTIME_SetSystime(0, 0, NULL);
}

void netx6_systime_init(uint32_t ulBorder, uint32_t ulSpeed)
{
  NX6_SYSTIME_SetBorder(ulBorder, 0);
  NX6_SYSTIME_SetSpeed(ulSpeed, 0);
  NX6_SYSTIME_SetSystime(0, 0, 0);
}

/* read PHY register */
unsigned int netx6_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;

  MIIMU_READ(uiPhy, uiReg, &usMiimuData);

  return (unsigned int)usMiimuData;
}

/* write PHY register */
void netx6_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  MIIMU_WRITE(uiPhy, uiReg, (uint16_t)uiValue);
}

void netx6_phy_init(void)
{
  uint32_t ulPhyControlReg;

  ulPhyControlReg = 0;
  ulPhyControlReg |= NX6_PHY_CTRL_PHY_ADDR                      << SRT_NX51_phy_control_phy_address;
  ulPhyControlReg |= NX6_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << SRT_NX51_phy_control_phy0_mode;
  ulPhyControlReg |= NX6_PHY_CTRL_PHY_AUTOMDIX                  << SRT_NX51_phy_control_phy0_automdix;
  ulPhyControlReg |= NX6_PHY_CTRL_PHY_ENABLE                    << SRT_NX51_phy_control_phy0_enable;
  ulPhyControlReg |= NX6_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << SRT_NX51_phy_control_phy1_mode;
  ulPhyControlReg |= NX6_PHY_CTRL_PHY_AUTOMDIX                  << SRT_NX51_phy_control_phy1_automdix;
  ulPhyControlReg |= NX6_PHY_CTRL_PHY_ENABLE                    << SRT_NX51_phy_control_phy1_enable;

  NX6_INTPHY_Init(ulPhyControlReg);

  phy_add(0, NX6_PHY_CTRL_PHY_ADDR + 0, netx6_phy_write, netx6_phy_read);
  phy_add(1, NX6_PHY_CTRL_PHY_ADDR + 1, netx6_phy_write, netx6_phy_read);
}
