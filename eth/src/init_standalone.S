	.section .init_code, "ax"

	.extern main_standalone
	.extern stack_top

	.global start

#include "asic_types.h"

@--------------------------------------

@ The Cortex-M4 in the netX90 is a THUMB-only CPU.
@ All others start in 32 bit mode.
#if ASIC_TYP==ASIC_TYP_NETX90_MPW || ASIC_TYP==ASIC_TYP_NETX90
	.syntax unified
	.thumb
#else
	.arm
#endif


    .func start, start
    .type start, STT_FUNC
start:
@----------------------------------------------------------------------------
@
@ Switch to the new stack.
@
	@ Set own stack position.
	ldr     r2, =stack_top
	mov     sp, r2


@----------------------------------------------------------------------------
@
@ Clear the .bss section (zero init).
@
	mov     r2, #0
	ldr     r3, =__bss_start__
	ldr     r4, =__bss_end__
clear_bss:
	str     r2, [r3], #4
	cmp     r3, r4
	bmi     clear_bss


@----------------------------------------------------------------------------
@
@ Call the main routine.
@ This shoul never return.
@
	@ Call the main routine. It requires the pointer to the parameter block in r0.
	b       main_standalone

    .endfunc

@--------------------------------------

  .end
