#include "asic.h"
#include "phy.h"
#include "common_func_netx10.h"
#include "regdef_netx10.h"

void asic_init(void)
{
  unsigned int uiIdx;

  /* clear all pending IRQs, this is important for 'soft restarts' from within the debugger */
  IRQ_Disable();
  NX10_VIC_ClearVic();
  NX10_VIC_InstallIsrVector(NX10_VIC_IRQVectorHandler_C);
  NX10_VIC_SetDefaultVector(NX10_VIC_ISR_Default);
  IRQ_Enable();

  systime_init(0, 0, 0);

  NX10_RDYRUN_SetRdyRunLed(NX10_RDYRUN_LED_OFF);

  NX10_MMIO_SetLine(0);

  for(uiIdx = 0; uiIdx < 24; ++uiIdx)
  {
    NX10_MMIO_SetCfg(uiIdx, NX10_MMIO_CONFIG_PIO_MODE, 0, 0);
  }

  /* reset pointer FIFO borders */
  NX10_PFIFO_Reset();

  for(uiIdx = 0; uiIdx < 2; ++uiIdx)
  {
    NX10_GPIO_ResetTimer(uiIdx);
  }

  for(uiIdx = 0; uiIdx < 2; ++uiIdx)
  {
    NX10_ArmTimer_Stop(uiIdx);
  }
}

void systime_init(unsigned int uiInst, uint32_t ulBorder, uint32_t ulSpeed)
{
  NX10_SYSTIME_SetBorder(ulBorder, NULL);
  NX10_SYSTIME_SetSpeed(ulSpeed, NULL);
  NX10_SYSTIME_SetSystime(0, 0, NULL);
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

  NX10_SYSTIME_GetSystime(pulSec, pulNsec, NULL);
}

void sleep(unsigned int uiMs)
{
  NX10_GPIO_Sleep(1, uiMs * 1000);
}

void irq_setup(unsigned int uiPrio, unsigned int uiIrqId, void (*pfnIsr)(void))
{
  NX10_VIC_SetIrqVector(uiPrio, uiIrqId | MSK_NX10_vic_vect_cntl0_ENABLE, pfnIsr);
  NX10_VIC_EnableIrqSources(1UL << uiIrqId);
}

/* read PHY register */
unsigned int intphy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;

  NX10_MIIMU_ReadPhyReg(NX10_MIIMU_SEND_PREAMBLE,
                        NX10_MIIMU_MDCFREQ_HIGH,
                        NX10_MIIMU_RTA_0_BITS,
                        uiPhy,
                        uiReg,
                        &usMiimuData,
                        0);
  return usMiimuData;
}

/* write PHY register */
void intphy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int usValue)
{
  NX10_MIIMU_WritePhyReg(NX10_MIIMU_SEND_PREAMBLE,
                         NX10_MIIMU_MDCFREQ_HIGH,
                         uiPhy,
                         uiReg,
                         (uint16_t)usValue,
                         0);
}

void intphy_init(void)
{
  NX10_PHY_CONTROL_T tPhyControlReg;

  /* initialize PHY */
  tPhyControlReg.val = 0;
  tPhyControlReg.bf.phy_address       = NX10_PHY_CTRL_PHY_ADDR;
  tPhyControlReg.bf.phy0_mode         = NX10_PHYCTRL_ALL_CAPABLE_AUTONEG_AUTOMDIXEN;
  tPhyControlReg.bf.phy0_fxmode       = 0;
  tPhyControlReg.bf.phy0_automdix     = NX10_PHY_CTRL_PHY_AUTOMDIX;
  tPhyControlReg.bf.phy0_enable       = NX10_PHY_CTRL_PHY_ENABLE;
  tPhyControlReg.bf.phy_sim_byp       = 0;
  tPhyControlReg.bf.phy_reset         = 0;
  NX10_INTPHY_Init(&tPhyControlReg);

  /* overwrite PHY ID, default value is zero */
  intphy_write(NX10_PHY_CTRL_PHY_ADDR, NX10_DRV_CB12_PHY_ID_2, PHYID_NETX_INTPHY_NEC);

  phy_add(0, 0, intphy_write, intphy_read);
}
