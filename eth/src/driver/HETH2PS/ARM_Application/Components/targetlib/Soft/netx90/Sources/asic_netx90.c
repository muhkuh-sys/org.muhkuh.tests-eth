#include "asic.h"
#include "phy.h"
#include "crc.h"
#include "hal_mmio.h"
#include "hal_mled_ctrl.h"
#include "hal_armtimer.h"
#include "hal_pfifo.h"
#include "hal_gpio.h"
#include "hal_systime.h"
#include "hal_armirq.h"
#include "hal_irq.h"
#include "hal_rdyrun.h"
#include "hal_intphy.h"
#include "hal_iflash.h"
#include "hal_padctrl.h"
#include "hal_asicctrl.h"
#include "regdef_netx90_arm_com.h"

#define CONCAT(a,b) a##b
#define BF(name, val) ((uint32_t)val << CONCAT(SRT_NX90_, name))
#define BM(name) (CONCAT(MSK_NX90_, name))

#define IFLASH_PAGESIZE  0x1000 /* 4kByte per page */
#define IFLASH_SIZE_INFO 0x2000 /* 8kByte total size of info block */

void asic_init(void)
{
  unsigned int uiIdx;

  /* clear all pending IRQs, this is important for 'soft restarts' from within the debugger */
  IRQ_Disable();
  ArmIrq_Deinit();
  ArmIrq_Init();
  IRQ_Enable();

  systime_init(0, 0, 0);

  RDYRUN_SetRdyRunLed(RDYRUN_LED_OFF);

  MMIO_SetOeLine(0, 0, 0, 0);
  MMIO_SetOutLineCfg(0, 0, 0, 0);

  for(uiIdx = 0; uiIdx < 18; ++uiIdx)
  {
    MMIO_SetCfg(uiIdx, MMIO_CONFIG_PIO_MODE, 0, 0);
  }

  MLEDCTRL_Stop(0);
  MLEDCTRL_SetLine(0, 0);

  /* reset pointer FIFO borders */
  PFIFO_Reset(0);

  GPIO_ResetTimer(0);
  GPIO_TimerIrqDisable(0x1);
  GPIO_TimerIrqReset(0x1);

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
  ArmTimer_Sleep(2, uiMs * 100000);
}

void irq_setup(unsigned int uiPrio, unsigned int uiIrqNo, void(*isr)(void))
{
  ArmIrq_SetIsr(uiIrqNo, (uint32_t)isr, uiPrio);
  ArmIrq_SetEnable(uiIrqNo, true);
}

/* Note: This function must not be located in INTFLASH */
void* iflash_read_info_block(void* pvDst, const void* pvSrc, unsigned int uiSize)
{
  unsigned int uiIflash, uiIdx;
  unsigned char *pbSrc, *pbDst;

  if( (unsigned int)pvSrc >= Addr_NX90_intflash0 && ((unsigned int)pvSrc + uiSize) < (Addr_NX90_intflash0 + IFLASH_SIZE_INFO) )
  {
    uiIflash = 0;
  }
  else if( (unsigned int)pvSrc >= Addr_NX90_intflash1 && ((unsigned int)pvSrc + uiSize) < (Addr_NX90_intflash1 + IFLASH_SIZE_INFO) )
  {
    uiIflash = 1;
  }
  else if( (unsigned int)pvSrc >= Addr_NX90_intflash2 && ((unsigned int)pvSrc + uiSize) < (Addr_NX90_intflash2 + IFLASH_SIZE_INFO) )
  {
    uiIflash = 2;
  }
  else
  {
    return NULL;
  }

  IFlash_BlockSelect(uiIflash, IFLASH_BLOCK_INFO);

  pbDst = (unsigned char*)pvDst;
  pbSrc = (unsigned char*)pvSrc;
  for(uiIdx = 0; uiIdx < uiSize; ++uiIdx)
  {
    *pbDst++ = *pbSrc++;
  }

  IFlash_BlockSelect(uiIflash, IFLASH_BLOCK_MAIN);

  return pvDst;
}

static int intphy_trim(void)
{
  int iRet = 0;
  union {
    unsigned char abBuf[96];
    uint32_t aulBuf[24];
  } uBuf; /* buffer for CAL page offset 0..95 including CRC32 */

  /* read trimming parameters from intflash0, CAL page0 (INFO page) */
  if( NULL == iflash_read_info_block(uBuf.abBuf, (void*)Addr_NX90_intflash0, 96) )
  {
    /* unable to read from flash */
    return -2;
  }

  if( uBuf.aulBuf[23] != crc_gen_crc32b(&uBuf.abBuf[16], 76) )
  {
    /* invalid crc32 in page0 */

    /* The CAL page is doubled in both info pages. We check both of them */
    if( NULL == iflash_read_info_block(uBuf.abBuf, (void*)(Addr_NX90_intflash0 + IFLASH_PAGESIZE), 96) )
    {
      /* unable to read from flash */
      return -2;
    }

    if( uBuf.aulBuf[23] != crc_gen_crc32b(&uBuf.abBuf[16], 76) )
    {
      /* invalid crc32 in page1, no valid calibration data in both pages */
      return 1;
    }
  }

  /* CAL page data read and validated */

  /* Apply PHY trimming parameters */

  /* TxAmp100 port 0 */
  if( (uBuf.abBuf[86] > 0) && (uBuf.abBuf[86] < 64) ) {
    if( INTPHY_PrmSet(INTPHY_PRM_PHY_CTRL_EXT0_TXAMP100, uBuf.abBuf[86]) ) iRet = -1;
  } else {
    iRet = 2;
  }

  /* TxAmp100 port 1 */
  if( (uBuf.abBuf[87] > 0) && (uBuf.abBuf[87] < 64) ) {
    if( INTPHY_PrmSet(INTPHY_PRM_PHY_CTRL_EXT1_TXAMP100, uBuf.abBuf[87]) ) iRet = -1;
  } else {
    iRet = 2;
  }

  /* TxAmp10 port 0 */
  if( (uBuf.abBuf[88] < 64) ) {
    if( INTPHY_PrmSet(INTPHY_PRM_PHY_CTRL_EXT0_TXAMP10, uBuf.abBuf[88]) ) iRet = -1;
  } else {
    iRet = 2;
  }

  /* TxAmp10 port 1 */
  if( (uBuf.abBuf[89] < 64) ) {
    if( INTPHY_PrmSet(INTPHY_PRM_PHY_CTRL_EXT1_TXAMP10, uBuf.abBuf[89]) ) iRet = -1;
  } else {
    iRet = 2;
  }

  return iRet;
}

void intphy_write(unsigned int uiPhy, unsigned int uiAddr, unsigned int uiVal)
{
  unsigned int uiBank = uiAddr >> 5;

  if( uiBank == 0 )
  {
    INTPHY_Write(uiPhy, uiAddr, uiVal);
  }
  else
  {
    --uiBank;
    INTPHY_WriteBank(uiPhy, uiBank, uiAddr & 0x1f, uiVal);
  }
}

unsigned int intphy_read(unsigned int uiPhy, unsigned int uiAddr)
{
  unsigned int uiBank = uiAddr >> 5;
  unsigned int uiVal;

  if( uiBank == 0 )
  {
    uiVal = INTPHY_Read(uiPhy, uiAddr);
  }
  else
  {
    --uiBank;
    uiVal = INTPHY_ReadBank(uiPhy, uiBank, uiAddr & 0x1f);
  }

  return uiVal;
}

/* Reset internal Dual-PHY */
void intphy_init(void)
{
  /* Connect XC to internal PHY */
  AsicCtrl_SetIoConfig(1, BF(io_config0_sel_xm0_mii_cfg, 8)
                        | BF(io_config0_sel_xc0_mdio, 2));
  AsicCtrl_SetIoConfig(2, BF(io_config1_sel_xm1_mii_cfg, 8));

  /* All PHY LEDs must be inverted */
  AsicCtrl_SetPhyCtrl0(BM(phy_ctrl0_phy0_led_invert)
                     | BM(phy_ctrl0_phy1_led_invert));

  INTPHY_Init();
  intphy_trim();

  /* initialize EPHY macro and send both ports into "Power Down" mode */
  INTPHY_SetupDualPhy();

  /* start AutoNeg with all capable modes advertised */
  INTPHY_SetModeAutoNeg(0, MSK_INTPHY_AUTONEG_ADV_ALL_CAPABLE);
  INTPHY_SetModeAutoNeg(1, MSK_INTPHY_AUTONEG_ADV_ALL_CAPABLE);

  phy_add(0, 0, intphy_write, intphy_read);
  phy_add(1, 1, intphy_write, intphy_read);
}

void intphy_init_port0_only(void)
{
  /* Connect XC to internal PHY */
  AsicCtrl_SetIoConfig(1, BF(io_config0_sel_xm0_mii_cfg, 8)
                        | BF(io_config0_sel_xc0_mdio, 2));

  /* All PHY LEDs must be inverted */
  AsicCtrl_SetPhyCtrl0(AsicCtrl_GetPhyCtrl0() | BM(phy_ctrl0_phy0_led_invert));

  INTPHY_Init();
  intphy_trim();

  /* initialize EPHY macro and send both ports into "Power Down" mode */
  INTPHY_SetupDualPhy();

  /* start AutoNeg with all capable modes advertised on port 0 */
  INTPHY_SetModeAutoNeg(0, MSK_INTPHY_AUTONEG_ADV_ALL_CAPABLE);

  phy_add(0, 0, intphy_write, intphy_read);
}

void intphy_init_fx(void)
{
  INTPHY_CTRL_T tPhyCtrl;
  unsigned int uiPort;

  static uint32_t const aulPadCtrlTableFx[] =
  {
    /* Note: Enable input at PHY1.FRX */
    PADCTRL_CFG(PAD_CTRL_PORT_MII1_RXD0, 1, PADCTRL_DS_4MA, PADCTRL_PE_NONE)
  };

  PadCtrl_SetCfgTable(aulPadCtrlTableFx);

  AsicCtrl_SetIoConfig(3, AsicCtrl_GetIoConfig(3)
                        | BM(io_config2_sel_fo0)
                        | BM(io_config2_sel_fo1));

  INTPHY_Init();

  intphy_trim();

  tPhyCtrl.uiPhyMode     = INTPHY_MODE_100BASE_TXFX_FD_NOAUTONEG_CRSRX;
  tPhyCtrl.fPhyReset     = false;
  tPhyCtrl.fPhySimBypass = false;
  for(uiPort = 0; uiPort < INTPHY_PORT_CNT; ++uiPort) {
    tPhyCtrl.afPhyFxMode[uiPort]   = true;
    tPhyCtrl.afPhyAutoMdix[uiPort] = false;
    tPhyCtrl.afPhyEnable[uiPort]   = true;
  }
  INTPHY_Reset(&tPhyCtrl);

  phy_add(0, 0, intphy_write, intphy_read);
  phy_add(1, 1, intphy_write, intphy_read);
}
