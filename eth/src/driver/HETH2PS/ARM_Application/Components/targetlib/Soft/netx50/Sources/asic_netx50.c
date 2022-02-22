#include "asic.h"
#include "phy.h"
#include "common_func_netx50.h"
#include <string.h>

#define MIIMU_READ(phy,reg,data)  NX50_MIIMU_ReadPhyReg(NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, NX50_MIIMU_RTA_0_BITS, phy, reg, data, 0)
#define MIIMU_WRITE(phy,reg,data) NX50_MIIMU_WritePhyReg(NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, phy, reg, data, 0)

void asic_init(void)
{
  unsigned int uiIdx;

  /* clear all pending IRQs, this is important for 'soft restarts' from within the debugger */
  IRQ_Disable();
  NX50_VIC_ClearVic();
  NX50_VIC_InstallIsrVector(NX50_VIC_IRQVectorHandler_C);
  NX50_VIC_SetDefaultVector(NX50_VIC_ISR_Default);
  IRQ_Enable();

  systime_init(0, 0, 0);

  NX50_RDYRUN_SetRdyRunLed(NX50_RDYRUN_LED_OFF);

  for(uiIdx = 0; uiIdx < 40; ++uiIdx)
  {
    NX50_MMIO_SetCfg(uiIdx, MMIO_CONFIG_INPUT, 0, 0);
  }

  /* reset pointer FIFO borders */
  NX50_PFIFO_Reset();

  for(uiIdx = 0; uiIdx < 5; ++uiIdx)
  {
    NX50_GPIO_ResetTimer(uiIdx);
  }
}

void systime_init(unsigned int uiInst, uint32_t ulBorder, uint32_t ulSpeed)
{
  NX50_SYSTIME_SetBorder(ulBorder, NULL);
  NX50_SYSTIME_SetSpeed(ulSpeed, NULL);
  NX50_SYSTIME_SetSystime(0, 0, NULL);
}

void systime_get(unsigned int uiInst, uint32_t* pulSec, uint32_t* pulNsec)
{
  uint32_t ulSec = 0, ulNsec = 0;

  if( pulSec == NULL )
  {
    pulSec = &ulSec;
  }

  if( pulNsec == NULL )
  {
    pulNsec = &ulNsec;
  }

  NX50_SYSTIME_GetSystime(pulSec, pulNsec, NULL);
}

void sleep(unsigned int uiMs)
{
  NX50_GPIO_Sleep(4, uiMs * 1000);
}

void irq_setup(unsigned int uiPrio, unsigned int uiIrqId, void (*pfnIsr)(void))
{
  NX50_VIC_SetIrqVector(uiPrio, uiIrqId | MSK_NX50_vic_vect_cntl0_ENABLE, pfnIsr);
  NX50_VIC_EnableIrqSources(1UL << uiIrqId);
}

/* write PHY register */
void intphy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  MIIMU_WRITE(uiPhy, uiReg, (uint16_t)uiValue);
}

/* read PHY register */
unsigned int intphy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;

  MIIMU_READ(uiPhy, uiReg, &usMiimuData);
  return (unsigned int)usMiimuData;
}

void intphy_init(void)
{
  /* initialize Dual-PHY */
  NX50_PHY_CONTROL_T tPhyControlReg;
  tPhyControlReg.bf.phy_address       = PHY_CTRL_PHY_ADDR;
  tPhyControlReg.bf.phy0_mode         = PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
  tPhyControlReg.bf.phy0_fxmode       = 0;
  tPhyControlReg.bf.phy0_automdix     = PHY_CTRL_PHY1_AUTOMDIX;
  tPhyControlReg.bf.phy0_np_msg_code  = PHY_CTRL_PHY1_NP_MSG_CODE;
  tPhyControlReg.bf.phy0_enable       = PHY_CTRL_PHY1_ENABLE;
  tPhyControlReg.bf.phy1_mode         = PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
  tPhyControlReg.bf.phy1_fxmode       = 0;
  tPhyControlReg.bf.phy1_automdix     = PHY_CTRL_PHY2_AUTOMDIX;
  tPhyControlReg.bf.phy1_np_msg_code  = PHY_CTRL_PHY2_NP_MSG_CODE;
  tPhyControlReg.bf.phy1_enable       = PHY_CTRL_PHY2_ENABLE;
  tPhyControlReg.bf.reserved1         = 0;
  tPhyControlReg.bf.phy_sim_byp       = 0;
  tPhyControlReg.bf.phy_reset         = 0;
  NX50_INTPHY_Init(&tPhyControlReg);

  /* overwrite PHY ID, default value is zero */
  intphy_write(PHY_CTRL_PHY_ADDR + 0, DRV_CB12_PHY_ID_2, PHYID_NETX_INTPHY_NEC);
  intphy_write(PHY_CTRL_PHY_ADDR + 1, DRV_CB12_PHY_ID_2, PHYID_NETX_INTPHY_NEC);

  phy_add(0, PHY_CTRL_PHY_ADDR + 0, intphy_write, intphy_read);
  phy_add(1, PHY_CTRL_PHY_ADDR + 1, intphy_write, intphy_read);
}

