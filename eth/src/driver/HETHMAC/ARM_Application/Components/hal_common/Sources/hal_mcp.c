/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_mcp.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_MCP(s_ptMcp)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(MCP_IrqSet)(uint32_t ulMsk)
{
  s_ptMcp->ulHs_irq_set_raw = ulMsk;
}

void HALDEF(MCP_IrqCnf)(uint32_t ulMsk)
{
  s_ptMcp->ulHs_irq_reset_raw = ulMsk;
}

void HALDEF(MCP_IrqEn)(uint32_t ulMsk)
{
  s_ptMcp->ulHs_irq_set_mask = ulMsk;
}

void HALDEF(MCP_IrqDis)(uint32_t ulMsk)
{
  s_ptMcp->ulHs_irq_reset_mask = ulMsk;
}

uint32_t HALDEF(MCP_GetMsk)(void)
{
  return s_ptMcp->ulHs_irq_set_mask;
}

uint32_t HALDEF(MCP_GetIrq)(void)
{
  return s_ptMcp->ulHs_irq_masked;
}

uint32_t HALDEF(MCP_GetIrqRaw)(void)
{
  return s_ptMcp->ulHs_irq_set_raw;
}

