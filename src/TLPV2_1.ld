/*
由于上个版本是由V1版本汉化的,
所以当到V2版本的时候,ROM的数据位置就经常变更了,
所以添加了V2.1版本的链接脚本,以便试用与补丁

*/


INCLUDE link.ld
INCLUDE text.ld

SECTIONS
{
	/*tlpROM里面的数据段排序*/
	/*.	=	0x9000000;*/
	/*tlp里面的数据有些不规律,所以这段是220KB*/
	/*. =     0x09940800;这个会让数据更麻烦
			所以把他们移到0x09CFB000
	*/
	. =		0x09CFB000;
	
	
	
	/*V2.1版本的空数据地址(参照TLPV2_V1空数据段)*/
	
	

	
	. =		0x09BD0000;
	
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
	
	/*
	V2.1版本的空数据地址(参照TLPV2_V1空数据段)
	这里是为了把一些字库数据存放到其他空间
	这样大约还能存放600KB左右的文字,
	如果再定义其他空间,理论还能达到1MB
	*/

	. =		0x09EF1500;
	.rodata : 
	{
		KEEP (*(EXCLUDE_FILE (data_*.o) .rodata))
	}
}