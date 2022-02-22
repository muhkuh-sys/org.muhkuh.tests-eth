/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_iflash.h"
#include "hw_defines.h"
#include "compiler_spec.h"

#define IFLASH_MODE_READ         0
#define IFLASH_MODE_PROGRAM      1
#define IFLASH_MODE_PAGE_ERASE   2
#define IFLASH_MODE_BLOCK_ERASE  3
#define IFLASH_MODE_MANUAL       4

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_IFLASH_CFG(s_aptIFlash)

#if defined(CPU_IS_ARM)
HW_PTR_IFLASH_GLOBAL_TIMINGS(s_ptFlashTimings)
#endif

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

int IFlash_SetRedundancyPage(unsigned int uiInst, unsigned int uiRedSel, unsigned int uiPage)
{
  if( uiRedSel > 1 )
  {
    return 1; /* invalid redundancy channel */
  }

  HW_WR32(s_aptIFlash[uiInst]->aulIflash_red_cfg[uiRedSel], uiPage << 1);

  /* clear cache */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_reset, 1);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_reset, 0);

  return 0;
}

int IFlash_BlockSelect(unsigned int uiInst, IFLASH_BLOCK_E eBlock)
{
  uint32_t ulVal = 0;

  /* select block */
  switch(eBlock)
  {
  case IFLASH_BLOCK_MAIN:
    break;

  case IFLASH_BLOCK_INFO:
    ulVal |= HW_MSK(iflash_ifren_cfg_ifren);
    break;

  case IFLASH_BLOCK_INFO1:
    ulVal |= HW_MSK(iflash_ifren_cfg_ifren1);
    break;

  default: return 1;
  }

  HW_WR32(s_aptIFlash[uiInst]->ulIflash_ifren_cfg, ulVal);
  /* read-back register to flush write buffers */
  if( HW_RD32(s_aptIFlash[uiInst]->ulIflash_ifren_cfg) != ulVal )
    return -1;

  __ISB();

  return 0;
}

int IFlash_Program(unsigned int uiInst, uint32_t ulXAdr, uint32_t ulYAdr, const uint32_t* pulData, unsigned int uiLen)
{
  unsigned int uiCell;
  const uint32_t* pulSrc = pulData;

  /* setup programming */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_PROGRAM);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_xadr, ulXAdr);

  for(uiCell = 0; uiCell < uiLen; ++uiCell)
  {
    /* set column address and data */
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_yadr, ulYAdr + uiCell);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[0], *pulSrc++);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[1], *pulSrc++);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[2], *pulSrc++);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[3], *pulSrc++);

    /* start programming and wait until finished */
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_access, 1);
    while( 0 != HW_RD32(s_aptIFlash[uiInst]->ulIflash_access) );
  }

  /* cleanup to read mode */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_READ);

  return 0;
}

int IFlash_ProgramMan(unsigned int uiInst, uint32_t ulXAdr, uint32_t ulYAdr, const uint32_t* pulData, unsigned int uiLen)
{
  unsigned int uiCell;
  const uint32_t* pulSrc = pulData;
  uint32_t ulSignals = HW_MSK(iflash_signals_cfg_xe);
  uint32_t aulData[5];

  /* setup programming */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_xadr, ulXAdr);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_MANUAL);

  ulSignals |= HW_MSK(iflash_signals_cfg_prog);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);

  SYS_SLEEP_CC(500); /* Tnvs > 5.000 ns */

  ulSignals |= HW_MSK(iflash_signals_cfg_nvstr);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);

  SYS_SLEEP_CC(1000); /* Tpgs > 10.000 ns */

  for(uiCell = 0; uiCell < uiLen; ++uiCell)
  {
    /* set column address and data */
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_yadr, ulYAdr + uiCell);
    aulData[0] = *pulSrc++;
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[0], aulData[0]);
    aulData[1] = *pulSrc++;
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[1], aulData[1]);
    aulData[2] = *pulSrc++;
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[2], aulData[2]);
    aulData[3] = *pulSrc++;
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[3], aulData[3]);
    aulData[4] = HW_RD32(s_aptIFlash[uiInst]->ulIflash_din4_ecc);

    /* write data[35:0] */
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[1], 0xfffffff0 | (aulData[1] & 0xf));
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[2], 0xffffffff);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[3], 0xffffffff);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[4], 0xffffffff);

    HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals | HW_MSK(iflash_signals_cfg_ye));
    SYS_SLEEP_CC(2000); /* Tprog > 20.000 ns */
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);

    /* write data[71:36] */
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[0], 0xffffffff);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[1], (0xfffffff0 & aulData[1]) | 0xf);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[2], 0xffffff00 | (aulData[2] & 0xff));

    HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals | HW_MSK(iflash_signals_cfg_ye));
    SYS_SLEEP_CC(2000); /* Tprog > 20.000 ns */
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);

    /* write data[107:72] */
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[1], 0xffffffff);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[2], (0xffffff00 & aulData[2]) | 0xff);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[3], 0xfffff000 | (aulData[3] & 0xfff));

    HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals | HW_MSK(iflash_signals_cfg_ye));
    SYS_SLEEP_CC(2000); /* Tprog > 20.000 ns */
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);

    /* write data[143:108] */
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[2], 0xffffffff);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[3], (0xfffff000 & aulData[3]) | 0xfff);
    HW_WR32(s_aptIFlash[uiInst]->aulIflash_din[4], aulData[4]);

    HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals | HW_MSK(iflash_signals_cfg_ye));
    SYS_SLEEP_CC(2000); /* Tprog > 20.000 ns */
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);
  }

  ulSignals &= ~HW_MSK(iflash_signals_cfg_prog);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);
  SYS_SLEEP_CC(500); /* Tnvh > 5.000 ns */

  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, 0);
  SYS_SLEEP_CC(1000); /* Trcv > 10.000 ns */

  /* cleanup to read mode */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_READ);

  return 0;
}

int IFlash_StartEraseMan(unsigned int uiInst, unsigned int uiPage)
{
  uint32_t ulSignals = HW_MSK(iflash_signals_cfg_xe);

  if( uiPage == IFLASH_PAGE_ALL )
  {
    ulSignals |= HW_MSK(iflash_signals_cfg_mas1);
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_xadr, 0);
  }
  else
  {
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_xadr, uiPage * 8);
  }

  /* setup programming */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_MANUAL);

  ulSignals |= HW_MSK(iflash_signals_cfg_erase);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);

  SYS_SLEEP_CC(500); /* Tnvs > 5.000 ns */

  ulSignals |= HW_MSK(iflash_signals_cfg_nvstr);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);

  return 0;
}

int IFlash_StopEraseMan(unsigned int uiInst)
{
  uint32_t ulSignals = HW_RD32(s_aptIFlash[uiInst]->ulIflash_signals_cfg);

  ulSignals &= ~HW_MSK(iflash_signals_cfg_erase);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, ulSignals);

  if( 0 != (ulSignals & HW_MSK(iflash_signals_cfg_mas1)) )
  {
    SYS_SLEEP_CC(10000); /* Tnvh1 > 100.000 ns */
  }
  else
  {
    SYS_SLEEP_CC(500); /* Tnvh > 5.000 ns */
  }

  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, 0);
  SYS_SLEEP_CC(1000); /* Trcv > 10.000 ns */

  /* cleanup to read mode */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_READ);

  return 0;
}

int IFlash_StartErase(unsigned int uiInst, unsigned int uiPage)
{
  /* setup programming */
  if( uiPage == IFLASH_PAGE_ALL )
  {
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_BLOCK_ERASE);
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_xadr, 0);
  }
  else
  {
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_PAGE_ERASE);
    HW_WR32(s_aptIFlash[uiInst]->ulIflash_xadr, uiPage * 8);
  }

  /* start programming */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_access, 1);

  return 0;
}

int IFlash_StopErase(unsigned int uiInst)
{
  /* wait until programming finished */
  if( 0 != HW_RD32(s_aptIFlash[uiInst]->ulIflash_access) )
  {
    return 1;
  }

  /* cleanup to read mode */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_READ);

  return 0;
}

int IFlash_EnterTestmode(unsigned int uiInst, unsigned int uiCode)
{
  uint32_t ulSignals = 0;
  uint32_t ulIfren   = 0;

  if( uiCode & 0x01 ) ulSignals |= HW_MSK(iflash_signals_cfg_mas1);
  if( uiCode & 0x02 ) ulIfren   |= HW_MSK(iflash_ifren_cfg_ifren);
  if( uiCode & 0x04 ) ulSignals |= HW_MSK(iflash_signals_cfg_xe);
  if( uiCode & 0x08 ) ulSignals |= HW_MSK(iflash_signals_cfg_ye);
  if( uiCode & 0x10 ) ulSignals |= HW_MSK(iflash_signals_cfg_erase);
  if( uiCode & 0x20 ) ulSignals |= HW_MSK(iflash_signals_cfg_prog);

  /* test mode sequences are always manual */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, 0);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_ifren_cfg, 0);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_MANUAL);

  /* Test mode reset */
  if( (uiCode & 0x80) == 0 ) HW_WR32(s_aptIFlash[uiInst]->ulIflash_special_cfg, 0);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_special_cfg, HW_MSK(iflash_special_cfg_tmr));
  if( (uiCode & 0x80) == 0 ) HW_WR32(s_aptIFlash[uiInst]->ulIflash_special_cfg, 0);

  /* latch test mode code */
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, HW_MSK(iflash_signals_cfg_nvstr) | HW_MSK(iflash_signals_cfg_se));
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, HW_MSK(iflash_signals_cfg_nvstr) | HW_MSK(iflash_signals_cfg_se) | ulSignals);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_ifren_cfg, ulIfren);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_special_cfg, HW_MSK(iflash_special_cfg_tmr));
  if( (uiCode & 0x80) == 0 ) HW_WR32(s_aptIFlash[uiInst]->ulIflash_special_cfg, 0);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_ifren_cfg, 0);
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, HW_MSK(iflash_signals_cfg_nvstr) | HW_MSK(iflash_signals_cfg_se));
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_signals_cfg, 0);

  return 0;
}

int IFlash_ExitTestmode(unsigned int uiInst)
{
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_special_cfg, HW_MSK(iflash_special_cfg_tmr));
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_mode_cfg, IFLASH_MODE_READ);

  return 0;
}

int IFlash_SetProtection(unsigned int uiInst, uint32_t ulVal)
{
  HW_WR32(s_aptIFlash[uiInst]->ulIflash_protection_info, ulVal);

  return 0;
}

#if defined(CPU_IS_ARM)
int IFlash_SetGlobalTiming(unsigned int uiMultiplier, unsigned int uiDivisor)
{
  uint32_t ulTmp;

  if( uiDivisor == 0 )
    return 1;

  ulTmp = HW_DFLT_VAL(wait_5us) * uiMultiplier;
  HW_WR32(s_ptFlashTimings->ulWait_5us, ulTmp / uiDivisor);
  ulTmp = HW_DFLT_VAL(wait_10us) * uiMultiplier;
  HW_WR32(s_ptFlashTimings->ulWait_10us, ulTmp / uiDivisor);
  ulTmp = HW_DFLT_VAL(wait_20us) * uiMultiplier;
  HW_WR32(s_ptFlashTimings->ulWait_20us, ulTmp / uiDivisor);
  ulTmp = HW_DFLT_VAL(wait_100us) * uiMultiplier;
  HW_WR32(s_ptFlashTimings->ulWait_100us, ulTmp / uiDivisor);
  ulTmp = HW_DFLT_VAL(wait_20ms) * uiMultiplier;
  HW_WR32(s_ptFlashTimings->ulWait_20ms, ulTmp / uiDivisor);

  return 0;
}
#endif
