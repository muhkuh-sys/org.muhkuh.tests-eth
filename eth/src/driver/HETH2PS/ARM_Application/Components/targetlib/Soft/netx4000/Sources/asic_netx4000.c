#include "asic.h"
#include "phy.h"
#include "hal_mmio.h"
#include "hal_armtimer.h"
#include "hal_pfifo.h"
#include "hal_gpio.h"
#include "hal_pio.h"
#include "hal_systime.h"
#include "hal_armirq.h"
#include "hal_irq.h"
#include "hal_rdyrun.h"
#include "hal_intphy.h"

void asic_init(void)
{
  unsigned int uiIdx;

  /* clear all pending IRQs, this is important for 'soft restarts' from within the debugger */
  IRQ_Disable();
  ArmIrq_Deinit();
  ArmIrq_Init();
  IRQ_Enable();

  for(uiIdx = 0; uiIdx < 2; ++uiIdx)
  {
    systime_init(uiIdx, 0, 0);
  }

  RDYRUN_SetRdyRunLed(RDYRUN_LED_OFF);

  PIO_SetOe(0);
  PIO_SetLine(0);
  GPIO_SetLine(0);
  MMIO_SetOeLine(0, 0, 0, 0);
  MMIO_SetOutLineCfg(0, 0, 0, 0);

  for(uiIdx = 0; uiIdx < 107; ++uiIdx)
  {
    MMIO_SetCfg(uiIdx, MMIO_CONFIG_PIO_MODE, 0, 0);
  }

  /* reset pointer FIFO borders */
  PFIFO_Reset(0);
  PFIFO_Reset(1);

  for(uiIdx = 0; uiIdx < 5; ++uiIdx)
  {
    GPIO_ResetTimer(uiIdx);
  }
  GPIO_TimerIrqDisable(0x1f);
  GPIO_TimerIrqReset(0x1f);

  for(uiIdx = 0; uiIdx < 3; ++uiIdx)
  {
    ArmTimer_Stop(uiIdx);
  }
}

void systime_init(unsigned int uiInst, uint32_t ulBorder, uint32_t ulSpeed)
{
  SYSTIME_SetBorder(uiInst, ulBorder, NULL);
  SYSTIME_SetSpeed(uiInst, ulSpeed, NULL);
  SYSTIME_SetSystime(uiInst, 0, 0, NULL);
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

  SYSTIME_GetSystime(uiInst, pulSec, pulNsec, NULL);
}

void sleep(unsigned int uiMs)
{
  GPIO_Sleep(4, uiMs * 1000);
}

void irq_setup(unsigned int uiPrio, unsigned int uiIrqNo, void(*isr)(void))
{
  ArmIrq_SetIsr(uiIrqNo, (uint32_t)isr, uiPrio);
  ArmIrq_SetEnable(uiIrqNo, true);
}

void intphy_init(void)
{
  /* reset PHY */
  INTPHY_Init();

  /* initialize DualPHY macro and send both ports into "Power Down" mode */
  INTPHY_SetupDualPhy();

  /* start AutoNeg with all capable modes advertised */
  INTPHY_SetModeAutoNeg(0, MSK_INTPHY_AUTONEG_ADV_ALL_CAPABLE);
  INTPHY_SetModeAutoNeg(1, MSK_INTPHY_AUTONEG_ADV_ALL_CAPABLE);

  phy_add(0, 0, INTPHY_Write, INTPHY_Read);
  phy_add(1, 1, INTPHY_Write, INTPHY_Read);
}
