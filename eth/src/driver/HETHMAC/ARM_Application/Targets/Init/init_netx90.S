  .section .init_code, "ax"

  .extern main
  .extern stack_top

  .global start

#define ADR_ACCESS_KEY 0xff4012c0

#ifdef __NETX90_MPW
#define ADR_CLK_EN0    0xff401250
#define ADR_CLK_EN1    0xff401258
#else
#define ADR_CLK_EN0    0xff401268
#define ADR_CLK_EN1    0xff401270
#endif

  /* The Cortex-M4 in the netX90 is a THUMB-only CPU. */
  .syntax unified
  .thumb


  .func start, start
  .type start, STT_FUNC
start:
  /* unlock access key */
  ldr     r0, =ADR_ACCESS_KEY
  ldr     r1, [r0]
  str     r1, [r0]

  /* set clock enable0
   * Note: Do not enable arm_app unless a valid program was written into intflash2!
   */
  ldr     r0, =ADR_CLK_EN0
  ldr     r1, =0xf7fff7ff
  str     r1, [r0]

  /* unlock access key */
  ldr     r0, =ADR_ACCESS_KEY
  ldr     r1, [r0]
  str     r1, [r0]

  /* set clock enable1 */
  ldr     r0, =ADR_CLK_EN1
  ldr     r1, =0xffffffff
  str     r1, [r0]

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

