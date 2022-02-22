#ifndef __HAL_MCP_H
#define __HAL_MCP_H

#include <stdint.h>

#ifndef HALDEC
#define HALDEC(name) name
#endif

void        HALDEC(MCP_IrqSet)     ( uint32_t ulMsk );    /* WR irq_set_raw */
void        HALDEC(MCP_IrqCnf)     ( uint32_t ulMsk );    /* WR irq_reset_raw */
void        HALDEC(MCP_IrqEn)      ( uint32_t ulMsk );    /* WR irq_set_mask */
void        HALDEC(MCP_IrqDis)     ( uint32_t ulMsk );    /* WR irq_reset_mask */
uint32_t    HALDEC(MCP_GetMsk)     ( void );              /* RD irq_set_mask */
uint32_t    HALDEC(MCP_GetIrq)     ( void );              /* RD irq_masked */
uint32_t    HALDEC(MCP_GetIrqRaw)  ( void );              /* RD irq_set_raw */

#endif /* #ifndef __HAL_MCP_H */
