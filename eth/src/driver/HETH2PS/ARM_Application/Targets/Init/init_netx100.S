
# --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

  .equ    CPSR_IRQ, 0xD2
  .equ    CPSR_FIQ, 0xD1
  .equ    CPSR_SVC, 0xD3
  .equ    CPSR_ABT, 0xD7
  .equ    CPSR_UND, 0xDB
  .equ    CPSR_SYS, 0xDF

# --- Amount of memory (in bytes) allocated for stacks

  .equ    Len_FIQ_Stack,   256*4      /* 1kB */
  .equ    Len_IRQ_Stack,   1024*4     /* 4kB */
  .equ    Len_ABT_Stack,   256*4      /* 1kB */
  .equ    Len_UND_Stack,   256*4      /* 1kB */
  .equ    Len_SVC_Stack,   256*4      /* 1kB */
  .equ    Len_USR_Stack,   2048*4     /* 8kB */

# Add lengths >0 for FIQ_Stack, ABT_Stack, UND_Stack if you need them.
# Offsets will be loaded as immediate values.
# Offsets must be 8 byte aligned.

  .equ    Offset_FIQ_Stack, 0
  .equ    Offset_IRQ_Stack, Offset_FIQ_Stack + Len_FIQ_Stack
  .equ    Offset_ABT_Stack, Offset_IRQ_Stack + Len_IRQ_Stack
  .equ    Offset_UND_Stack, Offset_ABT_Stack + Len_ABT_Stack
  .equ    Offset_SVC_Stack, Offset_UND_Stack + Len_UND_Stack
  .equ    Offset_USR_Stack, Offset_SVC_Stack + Len_SVC_Stack
  .equ    Offset_Topof_Stack, Offset_USR_Stack


  .equ    MMU_INIT_VALUE,   0x270

#********************************************************************************
# External functions to import
#********************************************************************************
  .extern main
  .extern top_of_stacks
  .global start

#********************************************************************************
# Local data
#********************************************************************************
  .data
        .align 4
        .global ulBootOption
ulBootOption: .long 0
        .global tBootblock
tBootblock:   .space 64



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
PowerOn:
# --- Copy the Boot-Block --------------------------------------------------------------
        LDR  r2, =ulBootOption               /* Load the address of the Boot-Option variable */
        STR  r1, [r2]                        /* Save it */
        LDR     R1, =tBootblock              /* Load the destination address for the Boot-Block */
        LDR     R2, =tBootblock + 64         /* Load the end destination address which is 64 bytes ahead */
LoopBoot:
        CMP     R1, R2
        LDRLO   R3, [R0], #4                 /* Get the next value of the Boot-Block */
        STRLO   R3, [R1], #4                 /* Write this value */
        BLO     LoopBoot                     /* Copy until the whole Boot-Block is copied */

# --- Disable caches -------------------------------------------------------------------

        LDR  r1, =MMU_INIT_VALUE             /* Defined in mmuArmLib.h */
        MCR   p15, 0, r1, c1, c0, 0           /* Write to MMU CR */

        /* mt:
         * If MMU was on before this, then we'd better hope it was set
         * up for flat translation or there will be problems. The next
         * 2/3 instructions will be fetched "translated" (number depends
         * on CPU).
         *
         * We would like to discard the contents of the Write-Buffer
         * altogether, but there is no facility to do this. Failing that,
         * we do not want any pending writes to happen at a later stage,
         * so drain the Write-Buffer, i.e. force any pending writes to
         * happen now.
         */

        MOV  r1, #0                           /* data SBZ */
        MCR  p15, 0, r1, c7, c10, 4           /* drain write-buffer */
        /* Flush (invalidate) both I and D caches */
        MCR  p15, 0, r1, c7, c7, 0             /* R1 = 0 from above, data SBZ*/

# --- Initialize the different stack types  ---------------------------------------------
        LDR     r0, =top_of_stacks           /* Load the address of the top of stack */
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
        MOV    r1, sp

# --- Fill the Stack with a pattern ---------------------------------------------------
        LDR     r2, =0xDEADBEEF              /* Set Fill pattern */
LoopSt: CMP     r1, r0
        STRLO   r2, [r1], #4
        BLO     LoopSt                       /* Loop until the last pattern has been written */

# --- Clear .bss section (Zero init) -------------------------------------------------------
        MOV     R0, #0                       /* Load the Fill pattern 0 */
        LDR     R1, =__bss_start__           /* Load start address of BSS section */
        LDR     R2, =__bss_end__             /* Load end address of BSS section */
LoopZI: CMP     R1, R2
        STRLO   R0, [R1], #4                 /* Store the Fill pattern */
        BLO     LoopZI                       /* Loop until the end address of reached */

        LDR    r0, =main                     /* Load the Address of the Main() function */
        BX     r0                            /* Finally jump to it */


        .global rX_DevGetBoottype
rX_DevGetBoottype:
        ldr    r0, =ulBootOption
        ldr    r0, [r0]
        bx     lr

        .global rX_DevGetBootblock
rX_DevGetBootblock:
        ldr    r0, =tBootblock
        bx     lr

  .end
