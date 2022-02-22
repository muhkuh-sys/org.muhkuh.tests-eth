MEMORY
{
  INTRAM(rwx)  : ORIGIN = 0x10000000, LENGTH = 0x00018000
  ITCM(rwx)    : ORIGIN = 0x00000000, LENGTH = 0x00002000
  DTCM(rw)     : ORIGIN = 0x04000000, LENGTH = 0x00002000
  SDRAM(rwx)   : ORIGIN = 0x80000000, LENGTH = 0x00800000
}

/* Define entry point of firmware (defined in InitGnu.s) */
ENTRY(start)

/* Section Definitions */
SECTIONS
{
  /* We have to locate the Stack first of all */
  .stack (NOLOAD) :
  {
    . = . + 0x8000;
    PROVIDE(top_of_stacks = .);
  } >SDRAM

  end_of_stacks = ADDR(.stack) + SIZEOF(.stack) ;

  OVERLAY 0x00000100 : NOCROSSREFS AT(end_of_stacks)
  {
    .itcm_data
    {
      PROVIDE( __load_source_itcm_data = .);
      . = ALIGN(4);
    }
  } > ITCM

  OVERLAY 0x04000000 : NOCROSSREFS AT(end_of_stacks + SIZEOF(.itcm_data))
  {
    .dtcm_data
    {
      PROVIDE( __load_source_dtcm_data = .);
      . = ALIGN(4);
    }
  } > DTCM

  .sec0 end_of_stacks + SIZEOF(.itcm_data) + SIZEOF(.dtcm_data) :
  {
    *(.init_code*)
    *(.text*) /* remaining code */

    *(.glue_7t*)
    *(.glue_7*)

     /* .rodata section which is used for read-only data (constants) */
    *(.rodata*)

   } >SDRAM

  /* .data section which is used for initialized data */
   . = ALIGN(4);
  .data :
  {
    _data = . ;
    *(.data*)
    SORT(CONSTRUCTORS)
  } >SDRAM

  . = ALIGN(4);

  .sec1 (NOLOAD) :
  {

    /* .bss section which is used for uninitialized data */
    . = ALIGN(4);
    __bss_start = . ;
    __bss_start__ = . ;

    *(.bss*)

    /* now let all the rest,typically non static variables, follow */
    *(COMMON)

    . = ALIGN(4);
    __bss_end__ = . ;
    __bss_end__ = . ;


  } >SDRAM

  . = ALIGN(4);

  PROVIDE(end         = .);
  PROVIDE(_heap_start = .);
  PROVIDE(_heap_end   = ORIGIN(SDRAM) + LENGTH(SDRAM));

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
