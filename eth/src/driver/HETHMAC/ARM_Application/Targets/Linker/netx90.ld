
ENTRY(start)

MEMORY
{
  ASSERT_EMPTY(rwx)  : ORIGIN = 0x00000000, LENGTH = 0

  INTRAM(rx)         : ORIGIN = 0x00020000, LENGTH = 0x00040000
}


/* Section Definitions */
SECTIONS
{
  .assert_empty (NOLOAD) :
  {
    /* NOTE: include all sections here which should be empty. */
    *(.data)
    *(.glue*)
    *(.vfp11_veneer)
    *(.v4_bx)
  } >ASSERT_EMPTY

  .vector_table (NOLOAD) :
  {
    /* vector table for interrupt service routine addresses */
    *(.vectors_ram)
  } >INTRAM

  .code :
  {
    KEEP(*(.header))
    *(.init_code)
    *(.text*)

    /* .rodata section which is used for read-only data (constants) */
    *(.rodata*)
  } >INTRAM

  .bss :
  {
    PROVIDE ( __bss_start__ = . );
    *(.bss*)
    *(COMMON)
    PROVIDE ( __bss_end__ = . );
  } >INTRAM

  .stack :
  {
    . = ORIGIN(INTRAM) + LENGTH(INTRAM);
    PROVIDE ( stack_top = . );
  } >INTRAM


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
