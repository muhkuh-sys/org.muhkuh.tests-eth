  .section .init_code, "ax"

  .extern main
  .extern stack_top

  .global start

  /* The Cortex-M4 in the netX90 is a THUMB-only CPU. */
  .syntax unified
  .thumb


  .func start, start
  .type start, STT_FUNC
start:
  /* Clear the .bss section (zero init). */
  mov     r2, #0
  ldr     r3, =__bss_start__
  ldr     r4, =__bss_end__
clear_bss:
  str     r2, [r3], #4
  cmp     r3, r4
  bmi     clear_bss

  /* Set own stack position. */
  ldr     r2, =stack_top
  mov     sp, r2

  b       main
  .endfunc

  .end

