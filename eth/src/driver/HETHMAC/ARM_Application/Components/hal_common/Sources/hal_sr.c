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

#ifndef HALDEF
#define HALDEF(name) name
#endif

uint32_t HALDEF(SR_GetStatcfg)(unsigned int uXcNo, unsigned int uXpecNo)
{
  return s_aptSrMotion[uXcNo]->aulSr_statcfg[uXpecNo];
}

void HALDEF(SR_SetConfig)(unsigned int uXcNo, unsigned int uXpecNo, uint32_t ulValue)
{
  s_aptSrMotion[uXcNo]->aulSr_statcfg[uXpecNo] = ulValue;
}

uint32_t HALDEF(SR_ReadSr)(unsigned int uXcNo, unsigned int uRegNo)
{
  return s_aptSrMotion[uXcNo]->aulSr_sr[uRegNo];
}

void HALDEF(SR_WriteSr)(unsigned int uXcNo, unsigned int uRegNo, uint32_t ulValue)
{
  s_aptSrMotion[uXcNo]->aulSr_sr[uRegNo] = ulValue;
}

uint32_t HALDEF(SR_ReadStatBitsShared)(void)
{
  return s_ptXcStatCfgShared->ulSr_stat_bits_shared;
}

void HALDEF(SR_WriteStatBitsShared)(uint32_t ulValue)
{
  s_ptXcStatCfgShared->ulSr_stat_bits_shared = ulValue;
}
