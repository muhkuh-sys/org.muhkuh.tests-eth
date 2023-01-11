/*---------------------------------------------------------------------------
  Author : Christoph Thelen

           Hilscher GmbH, Copyright (c) 2010, All Rights Reserved

           Redistribution or unauthorized use without expressed written 
           agreement from the Hilscher GmbH is forbidden
---------------------------------------------------------------------------*/

ENTRY(start)

MEMORY
{
	ASSERT_EMPTY(rwx)      : ORIGIN = 0x00000000, LENGTH = 0

    INTRAM_XC0(rw)            : ORIGIN = 0x00004000,  LENGTH = 0x00004000
	INTRAM_XC1(rw)            : ORIGIN = 0x00008000,  LENGTH = 0x00004000
    INTRAM_CODE(rwx)          : ORIGIN = 0x0000c000,  LENGTH = 0x00008000
    INTRAM_XC2(rw)            : ORIGIN = 0x00014000,  LENGTH = 0x00004000
    INTRAM_XC3(rw)            : ORIGIN = 0x00018000,  LENGTH = 0x00004000
    INTRAM_BSS(rwx)           : ORIGIN = 0x0001c000,  LENGTH = 0x00004000
	DTCM(rw)                  : ORIGIN = 0x10000000,  LENGTH = 0x00002000
}


/* Section Definitions */
SECTIONS
{
	.assert_empty (NOLOAD) :
	{
		/* NOTE: include all sections here whioch should be empty.
		 */
		*(.data)
		*(.glue*)
		*(.vfp11_veneer)
		*(.v4_bx)
		*(.iplt)
		*(.rel.dyn)
		*(.rel.iplt)
		*(.igot.plt)
	} >ASSERT_EMPTY


    .xcCode0 ORIGIN(INTRAM_XC0) :
    {
        load_address = . ;
        KEEP(*(.header))
        *rpu_ethmac0.o(.rodata)
        *tpu_ethmac0.o(.rodata)
        *xpec_ethmac0.o(.rodata)
    } >INTRAM_XC0


    .xcCode1 ORIGIN(INTRAM_XC1) :
    {
        *rpu_ethmac1.o(.rodata)
        *tpu_ethmac1.o(.rodata)
        *xpec_ethmac1.o(.rodata)
    } >INTRAM_XC1

    .xcCode2 ORIGIN(INTRAM_XC2) :
    {
        *rpu_ethmac2.o(.rodata)
        *tpu_ethmac2.o(.rodata)
        *xpec_ethmac2.o(.rodata)
    } >INTRAM_XC2

    .xcCode3 ORIGIN(INTRAM_XC3) :
    {
        *rpu_ethmac3.o(.rodata)
        *tpu_ethmac3.o(.rodata)
        *xpec_ethmac3.o(.rodata)
    } >INTRAM_XC3

	.code ORIGIN(INTRAM_CODE) :
	{
		*(.init_code .text* .rodata*)
	} >INTRAM_CODE


	.bss ORIGIN(INTRAM_BSS):
	{
		PROVIDE ( __bss_start__ = . );
		*(.bss* COMMON)
		PROVIDE ( __bss_end__ = . );
	} >INTRAM_BSS


	.parameter ALIGN(0x0100) :
	{
		parameter_start_address = . ;
		. = . + 0x0400;
		parameter_end_address = . ;
	} >INTRAM_BSS


	.stack :
	{
		. = ORIGIN(INTRAM_BSS) + LENGTH(INTRAM_BSS) ;
		PROVIDE ( stack_top = . );
	} >INTRAM_BSS


	.serialvectors ORIGIN(DTCM)+LENGTH(DTCM)-0x10 :
	{
		PROVIDE ( tSerialVectors = . );
		. = . + 0x10;
	} >DTCM


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
