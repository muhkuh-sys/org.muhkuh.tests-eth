#include "asic.h"
#include "phy.h"
#include "common_func_netx500.h"
#include <string.h>

/* place MMU translation table into INTRAM3 */
#define MMU_TABLE_BASE ((uint32_t*)Adr_NX500_intram3_base)

void asic_init(void)
{
  unsigned int uiIdx;

  /* clear all pending IRQs, this is important for 'soft restarts' from within the debugger */
  IRQ_Disable();
  NX500_VIC_ClearVic();
  NX500_VIC_InstallIsrVector(NX500_VIC_IRQVectorHandler_C);
  NX500_VIC_SetDefaultVector(NX500_VIC_ISR_Default);
  IRQ_Enable();

  systime_init(0, 0, 0);

  NX500_RDYRUN_SetRdyRunLed(NX500_RDYRUN_LED_OFF);

  /* Enable Caches */
  NX500_MMU_Init(MMU_TABLE_BASE);

  /* reset pointer FIFO borders */
  NX500_PFIFO_Reset();

  for(uiIdx = 0; uiIdx < 5; ++uiIdx)
  {
    NX500_GPIO_ResetTimer(uiIdx);
  }
}

void systime_init(unsigned int uiInst, uint32_t ulBorder, uint32_t ulSpeed)
{
  NX500_SYSTIME_SetBorder(ulBorder, NULL);
  NX500_SYSTIME_SetSpeed(ulSpeed, NULL);
  NX500_SYSTIME_SetSystime(0, 0, NULL);
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

  NX500_SYSTIME_GetSystime(pulSec, pulNsec, NULL);
}

void sleep(unsigned int uiMs)
{
  NX500_GPIO_Sleep(4, uiMs * 1000);
}

void irq_setup(unsigned int uiPrio, unsigned int uiIrqId, void (*pfnIsr)(void))
{
  NX500_VIC_SetIrqVector(uiPrio, uiIrqId | MSK_NX500_vic_vect_cntl0_ENABLE, pfnIsr);
  NX500_VIC_EnableIrqSources(1UL << uiIrqId);
}

/* read PHY register */
unsigned int intphy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;

  NX500_MIIMU_ReadPhyReg(NX500_MIIMU_SEND_PREAMBLE,
                         NX500_MIIMU_MDCFREQ_HIGH,
                         NX500_MIIMU_RTA_0_BITS,
                         uiPhy,
                         uiReg,
                         &usMiimuData,
                         0);

  return (unsigned int)usMiimuData;
}

/* write PHY register */
void intphy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  NX500_MIIMU_WritePhyReg(NX500_MIIMU_SEND_PREAMBLE,
                          NX500_MIIMU_MDCFREQ_HIGH,
                          uiPhy,
                          uiReg,
                          (uint16_t)uiValue,
                          0);
}

void intphy_init(void)
{
  NX500_PHY_CONTROL_T tPhyControlReg;

  /* initialize Dual-PHY */
  tPhyControlReg.bf.phy_address       = 30 >> 1;
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
  NX500_INTPHY_Init(&tPhyControlReg);

  /* overwrite PHY ID, default value is zero */
  intphy_write(30, DRV_CB12_PHY_ID_2, PHYID_NETX_INTPHY_NEC);
  intphy_write(31, DRV_CB12_PHY_ID_2, PHYID_NETX_INTPHY_NEC);

  phy_add(0, 30, intphy_write, intphy_read);
  phy_add(1, 31, intphy_write, intphy_read);
}
