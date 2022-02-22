# --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

  .equ    CPSR_IRQ, 0xD2
  .equ    CPSR_FIQ, 0xD1
  .equ    CPSR_SVC, 0xD3
  .equ    CPSR_ABT, 0xD7
  .equ    CPSR_UND, 0xDB
  .equ    CPSR_SYS, 0xDF

# --- Amount of memory (in bytes) allocated for stacks

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

#********************************************************************************
# External functions to import
#********************************************************************************
  .extern main
  .extern stack_top
  .global start

#********************************************************************************
# we are starting now with code
#********************************************************************************
  .section .init_code, "ax"

#********************************************************************************
# generate arm instructions (.code32)
#********************************************************************************
  .arm

#*******************************************************************************************
# This is the initial function we start with, either with the debugger or with the ROM-Loader
#
# In case the ROM-Loader is starting the application here, register R0 contains a pointer
# to the Boot-Block and Register R1 the type of Boot-method the ROM-Loader went through
#*******************************************************************************************
start:
        /* Drain write buffers */
        mcr  p15, 0, r1, c7, c10, 4
        
        /* set the ctrl register to ... */
        /* TBIT enable (#15 = 0) */
        /* enable ITCM (#12 = 1) */
        /* little endianess (#7 = 0) */
        /* enable buffered writes in bufferable areas (#3 = 1) */
        /* enable DTCM (#2 = 1) */
        /* disable alignment check (#1 = 0) */
        ldr  r0,  =0x1f7c
        mcr  p15, 0, r0, c1, c0, 0

# --- Initialize stacks ---------------------------------------------
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

# --- Fill the Stack with a pattern ---------------------------------------------------
        LDR     r2, =0xDEADBEEF              /* Set Fill pattern */
LoopSt: CMP     r1, r0
        STRLO   r2, [r1], #4
        BLO     LoopSt                       /* Loop until the last pattern has been written */

/* No need to relocate data section since it was already located correctly in ld file */
# --- Copy the data section -------------------------------------------------------
        ldr  r0, =__data_loadaddr__
        ldr  r1, =__data_start__
        ldr  r2, =__data_end__
copy_data:
        cmp  r1, r2
        ldrlo  r3, [r0], #4
        strlo  r3, [r1], #4
        blo  copy_data


# --- Clear .bss section (Zero init) -------------------------------------------------------
        MOV     R0, #0                       /* Load the Fill pattern 0 */
        LDR     R1, =__bss_start__           /* Load start address of BSS section */
        LDR     R2, =__bss_end__             /* Load end address of BSS section */
LoopZI: CMP     R1, R2
        STRLO   R0, [R1], #4                 /* Store the Fill pattern */
        BLO     LoopZI                       /* Loop until the end address of reached */

# --- Enable clocks -------------------------------------------------------
        LDR    r0, =0x1018c17c
        LDR    r1, [r0]
        STR    r1, [r0]
        LDR    r0, =0x1018c138
        LDR    r1, =0xf3333                   /* enable all clocks */
        STR    r1, [r0]
        
# --- Branch to main -------------------------------------------------------
        LDR    r0, =main                     /* Load the Address of the Main() function */
        BX     r0                            /* Finally jump to it */

  .end
