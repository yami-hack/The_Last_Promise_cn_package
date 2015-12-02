
SECTIONS{
	. = 0x8002AF4;
	.sub_8002AF4 :
	{
		*sub_8002AF4.o(.text)
	}
	
	. = 0x80040B8;
	.sub_80040B8 : 
	{
		*sub_80040B8.o(.text)
	}
	
	. = 0x8009FA8;
	.sub_8009FA8 : 
	{
		*sub_8009FA8.o(.text)
	}
}

