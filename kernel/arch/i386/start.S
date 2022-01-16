/*
 * multiboot v1 header constants
*/
	# align loaded modules on page boundaries
	.set MB_ALIGN,		1<<0
	# memory map
	.set MB_MEMINFO,	1<<1
	# 'flag' field
	.set MB_FLAGS,	  	MB_ALIGN | MB_MEMINFO
	# kernel identifier
	.set MB_MAGIC,	  	0x1BADB002
	# checksum
	.set MB_CHECKSUM,	-(MB_MAGIC + MB_FLAGS)

/*
 * multiboot v1 header
 * marks the program as a kernel within the first 8KiB
*/
	.section .multiboot
	.align 4
	.long MB_MAGIC
	.long MB_FLAGS
	.long MB_CHECKSUM

/*
 * Allocate 16KiB stack for initial thread
 * (16-byte aligned according to the System V ABI)
*/
	.section .bss
	.align 16
stack_bottom:
	.skip 16384
stack_top:

/*
 * Kernel entry point
 * (Loaded from protected mode)
*/
	.section .text
	.global _start
	.type _start, @function
	.extern kmain
_start:
	/* initialize stack */
	mov		$stack_top, %esp

	/* TODO: enable A20, initialize GDT */

	/* call global constructors */
	call	_init

	/* enter kernel */
	call	kmain

	/* disable interrupts and loop when finished */
	cli
1:	hlt
	jmp	1b

	/* assign label size for debugging */
	.size _start, . - _start