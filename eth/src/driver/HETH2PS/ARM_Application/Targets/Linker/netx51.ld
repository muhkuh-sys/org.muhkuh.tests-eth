/***************************************************************************
 *   Copyright (C) 2010 by Hilscher GmbH                                   *
 *                                                                         *
 *   Author: Burkhard Ilsen (bilsen@hilscher.com)                          *
 *                                                                         *
 *   Redistribution or unauthorized use without expressed written          *
 *   agreement from the Hilscher GmbH is forbidden.                        *
 ***************************************************************************/

ENTRY(start)

MEMORY
{
  /* INTRAMs used as iTCMs */
  INTRAM_ITCM(rwx): ORIGIN = 0x08000080, LENGTH = (0x00040000 - 0x80)

  /* intram0 and intram1 are used as ITCM because exception vectors are located in intram0 */
  ITCM(rwx):        ORIGIN = 0x00000080, LENGTH = (0x00040000 - 0x80)

  /* INTRAMs used as dTCMs */
  INTRAM_DTCM(rwx): ORIGIN = 0x08040000, LENGTH = 0x00030000

  /* intram2 to intram4 are used as DTCM */
  DTCM(rw):         ORIGIN = 0x04040000, LENGTH = 0x00030000

  /* intram5 is reserved for xpic instructions */
  INTRAM5(rw):      ORIGIN = 0x08070000, LENGTH = 0x00008000

  /* intram6 is reserved for xpic data */
  INTRAM6(rw):      ORIGIN = 0x08078000, LENGTH = 0x00008000

  /* intram7 is reserved for xpec0 data */
  INTRAM7(rw):      ORIGIN = 0x08080000, LENGTH = 0x00010000

  /* intram8 is reserved for xpec1 data */
  INTRAM8(rw):      ORIGIN = 0x08090000, LENGTH = 0x00010000

  /* intramhs is reserved for DPM */
  INTRAMHS(rw):     ORIGIN = 0x080a0000, LENGTH = 0x00008000

  /* sdram size is 8Mbyte, the minimum size for all Hilscher boards */
  SDRAM(rwx):       ORIGIN = 0x80000000, LENGTH = 0x00800000
}


/* Section Definitions */
SECTIONS
{
  .code.init :
  {
    *(.init_code*)
  }  >INTRAM_ITCM

  .code ORIGIN(ITCM) + SIZEOF(.code.init):
  {
    *(.header*)
    *(.text*)
    *(.glue*)
    *(.vfp11_veneer*)
    *(.v4_bx*)
  } AT>INTRAM_ITCM

  .rodata :
  {
    /* read only data */
    *(.rodata*)
  } >DTCM AT>INTRAM_DTCM

  .data :
  {
    __data_start__ = . ;
    /* initialized read/write data */
    *(.data*)
    SORT(CONSTRUCTORS)
    __data_end__ = ALIGN(4) ;
  } >DTCM AT>INTRAM_ITCM

  PROVIDE (__data_loadaddr__ = LOADADDR(.data));
  PROVIDE (__data_loadend__  = LOADADDR(.data) + SIZEOF(.data));

  .bss (NOLOAD):
  {
    /* zero-initialized data */
    __bss_start__ = . ;

    *(.bss*)
    *(COMMON)

    __bss_end__ = ALIGN(4) ;
  } > DTCM


  /* set the top of the stack to the end of INTRAM4 */
  stack_top = ORIGIN(DTCM) + LENGTH(DTCM) ;

  _end = .;
  PROVIDE (end = .);

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

