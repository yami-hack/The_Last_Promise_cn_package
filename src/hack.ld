


SECTIONS
{
	/*tlpROM里面的数据段排序*/
	/*.	=	0x9000000;*/
	/*tlp里面的数据有些不规律,所以这段是220KB*/
	/*. =     0x09940800;这个会让数据更麻烦
			所以把他们移到0x09CFB000
	*/
	. =		0x09CFB000;
	.text : 
	{
		KEEP (*(EXCLUDE_FILE (*link.o) .text))
	}
	
	/*以后的数据*/
	/*. =		0x09CFB000;*/
	.data : 
	{
		KEEP (*(EXCLUDE_FILE (data_*.o) .data))
	}
	.rodata : 
	{
		KEEP (*(EXCLUDE_FILE (data_*.o) .rodata))
	}
}