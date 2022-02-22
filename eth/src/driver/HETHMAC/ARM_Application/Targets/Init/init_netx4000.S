  .section .init_code, "ax"
  .arm

  /* Standard definitions of mode bits and interrupt (I & F) flags in PSRs */

  .equ    CPSR_IRQ, 0xD2
  .equ    CPSR_FIQ, 0xD1
  .equ    CPSR_SVC, 0xD3
  .equ    CPSR_ABT, 0xD7
  .equ    CPSR_UND, 0xDB
  .equ    CPSR_SYS, 0xDF

  /* Amount of memory (in bytes) allocated for stacks */

  .equ    Len_FIQ_Stack,   0x100      /* 256 Bytes */
  .equ    Len_IRQ_Stack,   0xC00      /* ~3kB */
  .equ    Len_ABT_Stack,   0x100      /* 1kB */
  .equ    Len_UND_Stack,   0x100      /* 1kB */
  .equ    Len_SVC_Stack,   0x100      /* 1kB */
  .equ    Len_USR_Stack,   0x1000     /* 4kB */

# Add lengths >0 for FIQ_Stack, ABT_Stack, UND_Stack if you need them.
# Offsets will be loaded as immediate values.
# Offsets must be 8 byte aligned.

  .equ    Offset_FIQ_Stack,   0
  .equ    Offset_IRQ_Stack,   Offset_FIQ_Stack + Len_FIQ_Stack
  .equ    Offset_ABT_Stack,   Offset_IRQ_Stack + Len_IRQ_Stack
  .equ    Offset_UND_Stack,   Offset_ABT_Stack + Len_ABT_Stack
  .equ    Offset_SVC_Stack,   Offset_UND_Stack + Len_UND_Stack
  .equ    Offset_USR_Stack,   Offset_SVC_Stack + Len_SVC_Stack
  .equ    Offset_Topof_Stack, Offset_USR_Stack

  .extern main
  .extern stack_top

  .global start

  .func start

start:
        /* Write DTCM Region Register */
        ldr     r0, =0x20021 /* DTCM base=0x20000, size=128k, enable=1 */
        mcr     p15, 0, r0, c9, c1, 0

        /* Write ITCM Region Register */
        ldr     r0, =0x21    /* ITCM base=0, size=128k, enable=1 */
        dsb
        mcr     p15, 0, r0, c9, c1, 1
        isb

  /* Initialize stacks */
        LDR     r0, =stack_top           /* Load the address of the top of stack */
        MSR     CPSR_c, #CPSR_FIQ
        SUB     sp, r0, #Offset_FIQ_Stack
        MSR     CPSR_c, #CPSR_IRQ
        SUB     sp, r0, #Offset_IRQ_Stack
        MSR     CPSR_c, #CPSR_SVC
        SUB     sp, r0, #Offset_SVC_Stack
        MSR     CPSR_c, #CPSR_UND
        SUB     sp, r0, #Offset_UND_Stack
        MSR     CPSR_c, #CPSR_ABT
        SUB     sp, r0, #Offset_ABT_Stack
        MSR     CPSR_c, #CPSR_SYS
        SUB     sp, r0, #Offset_Topof_Stack  /* Get the Top of the Stack */
        MOV     r1, sp

  /* unlock access key */
  ldr     r0, =0xf408017c
  ldr     r1, [r0]
  str     r1, [r0]

  /* set clock enable */
  ldr     r0, =0xf4080138
  ldr     r1, =0x00ffffff
  str     r1, [r0]

  /* clear .bss section */
  mov     r0, #0                       /* Load the Fill pattern 0 */
  ldr     r1, =__bss_start__           /* Load start address of BSS section */
  ldr     r2, =__bss_end__             /* Load end address of BSS section */
LoopZI:
  cmp     r1, r2
  strlo   r0, [r1], #4                 /* Store the Fill pattern */
  blo     LoopZI                       /* Loop until the end address of reached */

  /* clear stacks */
  mov     r0, #0                       /* Load the Fill pattern 0 */
  ldr     r2, =stack_top               /* Load end address of stack */
  sub     r1, r2, #0x2000              /* Load start address of stack */
LoopStack:
  cmp     r1, r2
  strlo   r0, [r1], #4                 /* Store the Fill pattern */
  blo     LoopStack                    /* Loop until the end address of reached */

  ldr     lr, =main
  bx      lr

  .endfunc

@--------------------------------------

  .end

