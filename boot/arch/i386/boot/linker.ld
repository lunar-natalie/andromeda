ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 0x7C00;
	.text : { *(.text) }
}