#include "io.h"
#include "hal_mmio.h"
#include "hal_padctrl.h"
#include "hal_asicctrl.h"
#include "hal_mled_ctrl.h"
#include "regdef_netx90_arm_com.h"

#define CONCAT(a,b) a##b
#define BF(name, val) ((uint32_t)val << CONCAT(SRT_NX90_, name))
#define BM(name) (CONCAT(MSK_NX90_, name))

#define MMIO_OUT_MIN  0
#define MMIO_OUT_CNT  4
#define MMIO_OUT_MSK ((1U << MMIO_OUT_CNT) - 1U)

#define MMIO_IN_CNT   0

/* map application trigger to X610.9 and X610.10 */
#define MMIO_APPTRG_MIN 6

static uint32_t const s_aulPadCtrlTable[] =
{
  /*          PORT                     IE DS              PE */

  /* set driver strength to 8mA for brighter LEDs */
  PADCTRL_CFG(PAD_CTRL_PORT_MLED0,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED1,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED2,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
  PADCTRL_CFG(PAD_CTRL_PORT_MLED3,     0, PADCTRL_DS_8MA, PADCTRL_PE_NONE),
};

void io_apptrg_init(void)
{
  MMIO_PioOeLineCfgSet(3UL << MMIO_APPTRG_MIN, 0, 0, 0);
}

void io_apptrg_set(unsigned int uiIdx, unsigned int uiVal)
{
  MMIO_SetOutput(MMIO_APPTRG_MIN + uiIdx, uiVal);
}

void io_output_init(void)
{
  MMIO_PioOutLineCfgRst(MMIO_OUT_MSK << MMIO_OUT_MIN, 0, 0, 0);
  MMIO_SetOeLine(MMIO_OUT_MSK << MMIO_OUT_MIN, 0, 0, 0);
}

unsigned int io_output_cnt(void)
{
  return MMIO_OUT_CNT;
}

void io_output_set(uint32_t ulOutput)
{
  ulOutput &= MMIO_OUT_MSK;
  MMIO_PioOutLineCfgSet(( ulOutput) << MMIO_OUT_MIN, 0, 0, 0);
  MMIO_PioOutLineCfgRst((~ulOutput) << MMIO_OUT_MIN, 0, 0, 0);
}

void io_input_init(void)
{
}

unsigned int io_input_cnt(void)
{
  return MMIO_IN_CNT;
}

uint32_t io_input_get(void)
{
  return 0;
}

uint32_t io_address_get(void)
{
  return 0;
}

void io_init(void)
{
  MMIO_SetCfg(4, MMIO_CONFIG_XC_TRIGGER0, 0, 0); /* X610.7: Trigger0 */
  MMIO_SetCfg(5, MMIO_CONFIG_XC_TRIGGER1, 0, 0); /* X610.8: Trigger1 */

  if( PADCTRL_RESULT_OK != PadCtrl_SetCfgTable(s_aulPadCtrlTable) )
  {
    while(1);
  }

  /* Map MLED2 to INTPHY LEDs of port 0 */
  MLEDCTRL_ConfigureOutput(0, 4, MLED_CTRL_SEL_PHY_CTRL0_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 5, MLED_CTRL_SEL_PHY_CTRL0_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);

  /* Map MLED3 to INTPHY LEDs of port 1 */
  MLEDCTRL_ConfigureOutput(0, 6, MLED_CTRL_SEL_PHY_CTRL1_LED0, false, MLED_CTRL_BRIGHTNESS_MAX);
  MLEDCTRL_ConfigureOutput(0, 7, MLED_CTRL_SEL_PHY_CTRL1_LED1, false, MLED_CTRL_BRIGHTNESS_MAX);

  MLEDCTRL_Start(0, 3, 0);
}
