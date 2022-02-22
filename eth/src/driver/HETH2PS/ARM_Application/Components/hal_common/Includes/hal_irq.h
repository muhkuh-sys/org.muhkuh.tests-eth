#ifndef __HAL_IRQ_H
#define __HAL_IRQ_H

typedef unsigned int IRQSTATE;

void     IRQ_Enable(void);
void     IRQ_Disable(void);
IRQSTATE IRQ_LockSave(void);
void     IRQ_LockRestore(IRQSTATE state);

#endif /* #ifndef __HAL_IRQ_H */
