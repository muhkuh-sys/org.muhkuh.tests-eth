	.section .init_code, "ax"

	.extern main_standalone
	.extern stack_top

	.global start
    .global tEthernetParameter

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
@ Save the registers and switch to the new stack.
@
	@ Save all working register.
	push    {r4, lr}

	@ Save the old stack position.
	mov     r3, sp

	@ Set own stack position.
	ldr     r2, =stack_top
	mov     sp, r2

	@ Store the old stack position on the new stack.
	push    {r3}


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
@
	@ Call the main routine. It requires the pointer to the parameter block in r0.
	ldr     r0, =tEthernetParameter
	bl      main_standalone


@----------------------------------------------------------------------------
@
@ Restore the registers and switch to the old stack.
@
	@ Restore the old stack position.
	pop     {r4}
	mov     sp, r4

	@ Restore the working register.
	pop     {r4, lr}
	bx      lr

    .endfunc

@--------------------------------------

  .end
