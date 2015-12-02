

SECTIONS
{
	
	. = 0x08000000;
	.rom (COPY):
	{
		KEEP (*rom*.o(.text))
	}
}

INCLUDE text.ld
INCLUDE link.ld


/*
SECTIONS
{
	. =		0x09CFB000;
	.text : 
	{
		KEEP (*(EXCLUDE_FILE (*link.o) .text))
	}
	.data : 
	{
		KEEP (*(EXCLUDE_FILE (data_*.o) .data))
	}
	.rodata : 
	{
		KEEP (*(EXCLUDE_FILE (data_*.o) .rodata))
	}
	
}
*/

INCLUDE TLPV2_1.ld

