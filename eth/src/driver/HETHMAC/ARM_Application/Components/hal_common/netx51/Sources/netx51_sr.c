/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_sr.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_SR_MOTION

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

uint32_t NX51_SR_GetStatcfg(unsigned int uPortNo)
{
  if( uPortNo<2 )
    return s_ptSrMotion->aulSr_statcfg[uPortNo];
  else
    return 0;
}

void NX51_SR_SetConfig(unsigned int uPortNo, uint32_t ulValue)
{
  if( uPortNo<2 ) s_ptSrMotion->aulSr_statcfg[uPortNo] = ulValue;
}

uint32_t NX51_SR_ReadSr(unsigned int uRegNo)
{
  if( uRegNo<16 )
    return s_ptSrMotion->aulSr_sr[uRegNo];
  else
    return 0;
}

void NX51_SR_WriteSr(unsigned int uRegNo, uint32_t ulValue)
{
  if( uRegNo<16 ) s_ptSrMotion->aulSr_sr[uRegNo] = ulValue;
}

uint32_t NX51_SR_ReadStatBitsShared(void)
{
  return s_ptSrMotion->ulSr_stat_bits_shared;
}

void NX51_SR_WriteStatBitsShared(uint32_t ulValue)
{
  s_ptSrMotion->ulSr_stat_bits_shared = ulValue;
}

