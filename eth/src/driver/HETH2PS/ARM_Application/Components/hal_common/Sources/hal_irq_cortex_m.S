  .global IRQ_Enable
  .global IRQ_Disable
  .global IRQ_LockSave
  .global IRQ_LockRestore

  /* The Cortex-M is a THUMB-only CPU. */
  .syntax unified
  .thumb

  .func IRQ_Enable
  .thumb_func
IRQ_Enable:
  cpsie if
  bx lr
  .endfunc

  .func IRQ_Disable
  .thumb_func
IRQ_Disable:
  cpsid i
  bx lr
  .endfunc

  .func IRQ_LockSave
  .thumb_func
IRQ_LockSave:
  mrs r0, primask
  cpsid i
  bx lr
  .endfunc

  .func IRQ_LockRestore
  .thumb_func
IRQ_LockRestore:
  msr primask, r0
  bx lr
  .endfunc

  .end
