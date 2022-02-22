/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_sr.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_SR_MOTION(s_aptSrMotion)
HW_PTR_XC_STATCFG_SHARED(s_ptXcStatCfgShared)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

uint32_t SR_GetStatcfg(unsigned int uiXcInst, unsigned int uiXpec)
{
  return s_aptSrMotion[uiXcInst]->aulSr_statcfg[uiXpec];
}

void SR_SetConfig(unsigned int uiXcInst, unsigned int uiXpec, uint32_t ulValue)
{
  s_aptSrMotion[uiXcInst]->aulSr_statcfg[uiXpec] = ulValue;
}

uint32_t SR_ReadSr(unsigned int uiXcInst, unsigned int uiRegIdx)
{
  return s_aptSrMotion[uiXcInst]->aulSr_sr[uiRegIdx];
}

void SR_WriteSr(unsigned int uiXcInst, unsigned int uiRegIdx, uint32_t ulValue)
{
  s_aptSrMotion[uiXcInst]->aulSr_sr[uiRegIdx] = ulValue;
}

uint32_t SR_ReadStatBitsShared(void)
{
  return s_ptXcStatCfgShared->ulSr_stat_bits_shared;
}

void SR_WriteStatBitsShared(uint32_t ulValue)
{
  s_ptXcStatCfgShared->ulSr_stat_bits_shared = ulValue;
}
