/* entry point of program */
/* configure output to 32 bit format */

ENTRY(loader) 
OUTPUT_FORMAT(elf32-i386) 
OUTPUT_ARCH(i386:i386)

SECTIONS
{

/* set current location counter to 0x0100000 */

	. = 0x0100000; 

/* 
code section :
includes section named multiboot, sections that starts with text and read-only data sections 
*/

	.text :
	{
		*(.multiboot) 
		*(.text*)
		*(.rodata)   
	}

/*
initialised data section :
has constructor functions
set start_ctors symbol to current counter value
includes all elements of .init_array
includes all elements of .init_array.* sections sorted by their priority value
set end_ctors symbol to current counter value
*/

	.data :
	{
		start_ctors = .;
		KEEP(*( .init_array ));
		KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
		end_ctors = .;

		*(.data)
	}

/* uninitialised data section : */

	.bss :
	{
		*(.bss)
	}

/*
discard sections named .fini_array
discard sections named .comment from final executable
*/

	/DISCARD/ :
	{
		*(.fini_array*)
		*(.comment)
	}

}
