/* Memory definitions
   Note: the sdram size is fixed to 8MB, the smallest type for all our boards
*/
MEMORY
{
        INTRAM0(rwx)            : ORIGIN = 0x00000000,  LENGTH = 0x00008000
        INTRAM1(rwx)            : ORIGIN = 0x00008000,  LENGTH = 0x00008000
        INTRAM2(rwx)            : ORIGIN = 0x00010000,  LENGTH = 0x00008000
        INTRAM3(rwx)            : ORIGIN = 0x00018000,  LENGTH = 0x00008000
        INTKERNELROM(rwx)       : ORIGIN = 0x00202000,  LENGTH = 0x6000
        INTKERNELRAM(rwx)       : ORIGIN = 0x10000000,  LENGTH = 0x2000
        SDRAM(rwx)              : ORIGIN = 0x80000000,  LENGTH = 0x00800000
}

/* Define entry point of firmware (defined in InitGnu.s) */
ENTRY(start)

/* Section Definitions */
SECTIONS
{
        .KernelRom (NOLOAD):
        {
                *RomSymbols.o(.text*)
        } >INTKERNELROM =0

        .KernelRam (NOLOAD):
        {
                *RomSymbols.o(.data*)
                *(.dtcm*)
        } >INTKERNELRAM =0


        /* system vectors reserved area */
        .system 0x00000000 (NOLOAD) :
        {
                . = . + 0x80;
        } >INTRAM0


        /* reserve edd buffers for all 4 channels
           The first part of segment 0 is occupied by the system vectors and
           can't be used. All other buffers start at the same offset in the
           ram bank to keep the structure */
        .eddbuffer0 0x00000618 (NOLOAD) :
        {
                PROVIDE (eddbuffer0_start = .);
                . = . + 0x79e0;
                PROVIDE (eddbuffer0_end = .);
        } >INTRAM0
        .eddbuffer1 0x00008618 (NOLOAD) :
        {
                PROVIDE (eddbuffer1_start = .);
                . = . + 0x79e0;
                PROVIDE (eddbuffer1_end = .);
        } >INTRAM1
        .eddbuffer2 0x00010618 (NOLOAD) :
        {
                PROVIDE (eddbuffer2_start = .);
                . = . + 0x79e0;
                PROVIDE (eddbuffer2_end = .);
        } >INTRAM2
        .eddbuffer3 0x00018618 (NOLOAD) :
        {
                PROVIDE (eddbuffer3_start = .);
                . = . + 0x79e0;
                PROVIDE (eddbuffer3_end = .);
        } >INTRAM3


        .stacks (NOLOAD):
        {
                . = . + 0x8000;
                PROVIDE (top_of_stacks = .);
        } >SDRAM


        /* code section */
        .sec0 :
        {
                *(.init_code*)
                *(.text*)

                *(.glue_7t*) *(.glue_7*)

                /* .rodata section which is used for read-only data (constants) */
                *(.rodata*)

                . = ALIGN(4);

                _etext = . ;
                PROVIDE (etext = .);
        } >SDRAM


        /* .data section which is used for initialized data */
        .data :
        {
                _data = . ;

                *(.data*)
                SORT(CONSTRUCTORS)
        } >SDRAM

        /* .bss section which is used for uninitialized data */
        .bss (NOLOAD):
        {
                . = ALIGN(4);

                __bss_start__ = . ;

                *(.bss*)

                /* uninitialized data, see "Using LD, the GNU linker",
                   "Section Placement":
                   Specify where in your output file to place uninitialized
                   data with this notation. *(COMMON) by itself refers to all
                   uninitialized data from all input files (so far as it is
                   not yet allocated); */
                *(COMMON)

                /* align the end of the section to 32bit boundary */
                . = ALIGN(4);

                __bss_end__ = . ;
        } >SDRAM

        . = ALIGN(4);
        _edata = . ;
        PROVIDE (edata = .);

        _end = .;
        PROVIDE (end = .);

        . = ALIGN(4);
        abStaMem = .;
        PROVIDE (abStaMem = .);

        /* Stabs debugging sections.  */
        .stab          0 : { *(.stab) }
        .stabstr       0 : { *(.stabstr) }
        .stab.excl     0 : { *(.stab.excl) }
        .stab.exclstr  0 : { *(.stab.exclstr) }
        .stab.index    0 : { *(.stab.index) }
        .stab.indexstr 0 : { *(.stab.indexstr) }
        .comment       0 : { *(.comment) }
        /* DWARF debug sections.
           Symbols in the DWARF debugging sections are relative to the beginning
           of the section so we begin them at 0.  */
        /* DWARF 1 */
        .debug          0 : { *(.debug) }
        .line           0 : { *(.line) }
        /* GNU DWARF 1 extensions */
        .debug_srcinfo  0 : { *(.debug_srcinfo) }
        .debug_sfnames  0 : { *(.debug_sfnames) }
        /* DWARF 1.1 and DWARF 2 */
        .debug_aranges  0 : { *(.debug_aranges) }
        .debug_pubnames 0 : { *(.debug_pubnames) }
        /* DWARF 2 */
        .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
        .debug_abbrev   0 : { *(.debug_abbrev) }
        .debug_line     0 : { *(.debug_line) }
        .debug_frame    0 : { *(.debug_frame) }
        .debug_str      0 : { *(.debug_str) }
        .debug_loc      0 : { *(.debug_loc) }
        .debug_macinfo  0 : { *(.debug_macinfo) }
        /* SGI/MIPS DWARF 2 extensions */
        .debug_weaknames 0 : { *(.debug_weaknames) }
        .debug_funcnames 0 : { *(.debug_funcnames) }
        .debug_typenames 0 : { *(.debug_typenames) }
        .debug_varnames  0 : { *(.debug_varnames) }
}

