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

void MCP_IrqSet(uint32_t ulMsk)
{
  s_ptMcp->ulHs_irq_set_raw = ulMsk;
}

void MCP_IrqCnf(uint32_t ulMsk)
{
  s_ptMcp->ulHs_irq_reset_raw = ulMsk;
}

void MCP_IrqEn(uint32_t ulMsk)
{
  s_ptMcp->ulHs_irq_set_mask = ulMsk;
}

void MCP_IrqDis(uint32_t ulMsk)
{
  s_ptMcp->ulHs_irq_reset_mask = ulMsk;
}

uint32_t MCP_GetMsk(void)
{
  return s_ptMcp->ulHs_irq_set_mask;
}

uint32_t MCP_GetIrq(void)
{
  return s_ptMcp->ulHs_irq_masked;
}

uint32_t MCP_GetIrqRaw(void)
{
  return s_ptMcp->ulHs_irq_set_raw;
}
