#include "asic.h"
#include "phy.h"
#include "netx51_vic.h"
#include "netx51_mmio.h"
#include "netx51_armtimer.h"
#include "netx51_pfifo.h"
#include "netx51_gpio.h"
#include "netx51_systime.h"
#include "netx51_intphy.h"
#include "netx51_miimu.h"
#include "hal_irq.h"
#include "hal_rdyrun.h"
#include "regdef_netx51.h"

#define MIIMU_READ(phy,reg,data)  NX51_MIIMU_ReadPhyReg(NX51_MIIMU_SEND_PREAMBLE, NX51_MIIMU_MDCFREQ_HIGH, NX51_MIIMU_RTA_1_BITS, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) NX51_MIIMU_WritePhyReg(NX51_MIIMU_SEND_PREAMBLE, NX51_MIIMU_MDCFREQ_HIGH, NX51_MIIMU_RTA_1_BITS, phy, reg, data, NULL)

void asic_init(void)
{
  unsigned int uiIdx;

  /* clear all pending IRQs, this is important for 'soft restarts' from within the debugger */
  IRQ_Disable();
  NX51_VIC_ClearVic();
  NX51_VIC_InstallIsrVector(NX51_VIC_IRQVectorHandler_C);
  NX51_VIC_SetDefaultVector(NX51_VIC_ISR_Default);
  IRQ_Enable();

  systime_init(0, 0, 0);

  RDYRUN_SetRdyRunLed(RDYRUN_LED_OFF);

  NX51_MMIO_SetLine(0, 0);

  for(uiIdx = 0; uiIdx < 49; ++uiIdx)
  {
    NX51_MMIO_SetCfg(uiIdx, NX51_MMIO_CONFIG_INPUT, 0, 0);
  }

  /* reset pointer FIFO borders */
  NX51_PFIFO_Reset();

  for(uiIdx = 0; uiIdx < 5; ++uiIdx)
  {
    NX51_GPIO_ResetTimer(uiIdx);
  }

  for(uiIdx = 0; uiIdx < 2; ++uiIdx)
  {
    NX51_ArmTimer_Stop(uiIdx);
  }
}

void systime_init(unsigned int uiInst, uint32_t ulBorder, uint32_t ulSpeed)
{
  NX51_SYSTIME_SetBorder(ulBorder, NULL);
  NX51_SYSTIME_SetSpeed(ulSpeed, NULL);
  NX51_SYSTIME_SetSystime(0, 0, NULL);
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

  NX51_SYSTIME_GetSystime(pulSec, pulNsec, NULL);
}

void sleep(unsigned int uiMs)
{
  NX51_GPIO_Sleep(4, uiMs * 1000);
}

void irq_setup(unsigned int uiPrio, unsigned int uiIrqId, void (*pfnIsr)(void))
{
  NX51_VIC_SetIrqVector(uiPrio, uiIrqId | MSK_NX51_vic_vect_cntl0_ENABLE, pfnIsr);
  NX51_VIC_EnableIrqSources(1UL << uiIrqId);
}

/* read PHY register */
unsigned int intphy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;

  MIIMU_READ(uiPhy, uiReg, &usMiimuData);

  return (unsigned int)usMiimuData;
}

/* write PHY register */
void intphy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  MIIMU_WRITE(uiPhy, uiReg, (uint16_t)uiValue);
}

void intphy_init(void)
{
  uint32_t ulPhyControlReg;

  ulPhyControlReg = 0;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ADDR                      << SRT_NX51_phy_control_phy_address;
  ulPhyControlReg |= NX51_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << SRT_NX51_phy_control_phy0_mode;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_AUTOMDIX                  << SRT_NX51_phy_control_phy0_automdix;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ENABLE                    << SRT_NX51_phy_control_phy0_enable;
  ulPhyControlReg |= NX51_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << SRT_NX51_phy_control_phy1_mode;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_AUTOMDIX                  << SRT_NX51_phy_control_phy1_automdix;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ENABLE                    << SRT_NX51_phy_control_phy1_enable;

  NX51_INTPHY_Init(ulPhyControlReg);

  phy_add(0, NX51_PHY_CTRL_PHY_ADDR + 0, intphy_write, intphy_read);
  phy_add(1, NX51_PHY_CTRL_PHY_ADDR + 1, intphy_write, intphy_read);
}
