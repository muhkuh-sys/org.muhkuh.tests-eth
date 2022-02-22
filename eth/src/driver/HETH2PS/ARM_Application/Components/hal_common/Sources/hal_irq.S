  .global IRQ_Enable
  .global IRQ_Disable
  .global IRQ_LockSave
  .global IRQ_LockRestore

  .arm

  .func IRQ_Enable
IRQ_Enable:
  mrs r1, cpsr
  bic r1, r1, #0xC0
  msr cpsr_c, r1
  bx lr
  .endfunc

  .func IRQ_Disable
IRQ_Disable:
  mrs r1, cpsr
  orr r1, r1, #0xC0
  msr cpsr_c, r1
  bx lr
  .endfunc

  .func IRQ_LockSave
IRQ_LockSave:
  mrs r1, cpsr
  and r0, r1, #0xC0
  orr r1, r1, #0xC0
  msr cpsr_c, r1
  bx lr
  .endfunc

  .func IRQ_LockRestore
IRQ_LockRestore:
  mrs r1, cpsr
  bic r1, r1, #0xC0
  orr r1, r1, r0
  msr cpsr_c, r1
  bx lr
  .endfunc

  .end
