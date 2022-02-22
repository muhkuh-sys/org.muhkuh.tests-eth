#ifndef __HAL_MCP_H
#define __HAL_MCP_H

#include <stdint.h>

void        MCP_IrqSet     ( uint32_t ulMsk );    /* WR irq_set_raw */
void        MCP_IrqCnf     ( uint32_t ulMsk );    /* WR irq_reset_raw */
void        MCP_IrqEn      ( uint32_t ulMsk );    /* WR irq_set_mask */
void        MCP_IrqDis     ( uint32_t ulMsk );    /* WR irq_reset_mask */
uint32_t    MCP_GetMsk     ( void );              /* RD irq_set_mask */
uint32_t    MCP_GetIrq     ( void );              /* RD irq_masked */
uint32_t    MCP_GetIrqRaw  ( void );              /* RD irq_set_raw */

#endif /* #ifndef __HAL_MCP_H */
