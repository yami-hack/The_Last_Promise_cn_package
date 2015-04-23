/*
 * export_item_text.c
 *
 *  Created on: 2015年4月19日
 *      Author: yami
 */


/**
 * @file		export_item_text.c
 * @author		yami
 * @date		2015-04-19 01:16:49
 * @brief
 * 从export_text中复制过来的
 * 导出ROM里面的文本内容
 */


//#define TLP_ITEM_
//#define		TLP_CHAR_
#define		TLP_CLASS_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef		unsigned char	u8;
typedef		unsigned short	u16;
typedef		unsigned int	u32;

typedef		signed char		s8;
typedef		signed short	s16;
typedef		signed int		s32;


typedef struct{
	char*	filename;
	int		size;
	u8*		mem;
	u32		mask_point;
	u32		mask_base;
	u32		texts_base;
}rom;



rom		TLP_rom	=	{
		"fe7tlp.gba",
		0,
		0,
		0x8B808A8,		//头信息
		0x8B7D71C,		//huffman树基
		0x8B808AC		//文本指针
};


/**
 *初始化结构体数据
 * @param rom	rom结构体
 * @return		如果返回0则正常申请
 */
int		rom_init(rom*rom){
	if(!rom->mem)
	{
		if(rom->filename){
			FILE*	file;
			u8		*mem;
			file	=	fopen(rom->filename,"rb");
			if(!file){
				printf("打开文件(%s)失败\n",rom->filename);
				return 1;
			}
			fseek(file,0,SEEK_END);
			rom->size	=	ftell(file);
			if(rom->size<=0){
				printf("计算文件(%s)大小失败\n",rom->filename);
				fclose(file);
				return 1;
			}

			//申请内存
			rom->mem	=	(u8*)calloc(rom->size,1);

			if(!rom->mem){
				printf("申请内存失败\n");
				return 1;
			}

			//文件重新定位
			fseek(file,0,SEEK_SET);
			fread(rom->mem,rom->size,1,file);
			fclose(file);
		}
		else
		{
			return 1;
		}
	}
	return 0;
}


/**
 * 释放了rom的数据
 * @param rom	rom结构体
 * @return		如果返回0则正常释放
 */
int		rom_free(rom*rom){
	if(rom->mem){
		free(rom->mem);
		rom->mem	=	0;
	}
	return 0;
}


/**
 * 获得ROM里面的数据,如果返回0,则表示失败
 * @param r			rom结构体
 * @param addr		rom地址
 * @return			返回当前地址
 */
u8*		rom_data(rom*r,u32 addr){
	if(addr>=0x08000000&&addr<=0x0A000000){
		addr-=0x08000000;
		return 	r->mem + addr;
	}
	return 0;
}




int		huffman_decode(rom*r,u32 src,u8*dst){
		//开始程序
	u8			*src_data;
	u32			bit;
	int			bit_index;
//	struct{
//		u16		low;
//		u16		High;
//	}tree;
	u16			*tree_data;
	u16			*tree_data_base;
	u32			*tree_base;
	u16			tree_;
	s32			data;
	if(!r->mask_base)
		return 1;
	if(!r->mask_point)
		return 1;
	//前面的是解码失败,因为是空指针u




	tree_base	=	(u32*)rom_data(r,r->mask_base);

	tree_data_base	=	(u16*)rom_data(
			r,
			*(u32*)rom_data(r,r->mask_point)
			);

	src_data	=	rom_data(r,src);
	if(!src_data){
		printf("哈夫曼数据指针错误\n");
		return 1;
	}
	tree_data	=	tree_data_base;
	do{
		//知道循环到0结束为止
		bit		=	*src_data++;
		for(bit_index=7;bit_index>=0;bit_index--){
			tree_		=	tree_data[bit&1];
			tree_data	=	(u16*)(tree_base	+	tree_);
			bit	>>=	1;
			if((data = *((s32*)tree_data))>0){
				continue;
			}
			tree_data	=	tree_data_base;
			if(!(data&0xff00)){
				*dst	=	(u8)data;
				if(!(data&0xff))
				{
					return 0;
				}
				else{
					dst	++;
				}
			}
			else{
				//XXYY
				dst[0]	=	(u8)data;
				dst[1]	=	(u8)(((u32)(data))>>8);
				dst		+=	2;
			}
		}
	}while(1);

	return 0;
}

static	void	out_C_str_(FILE*file,u8*text,u32	index){
	u8		*src_text;
	u8		c;
	u8		format[0x50];
	src_text	=	text;
	if(file){
		sprintf(format,"%12s","[0x%04X]\t=\t");
		fprintf(file,format,index);
		fprintf(file,"\"");
		while((c = *src_text++)){
			if(c<=0x1F||c>=0x80){
				fprintf(file,"\\x%02X",c);
			}
			else if(c=='\"'){
				fprintf(file,"\\%c",c);
			}
			else{
				fprintf(file,"%c",c);
			}
		}
		fprintf(file,"\",");
	}
}


int		out_C_str(rom*r){
	u32*	texts;
	u32		text_addr;
	s32		index;
	u8		*text;
	FILE	*file;
	char	fname[0x50];

	sprintf(fname,"%s.c",r->filename);
	file	=	fopen(fname,"w+");
	texts	=	(u32*)rom_data(r,r->texts_base);
	if(!texts){
		printf("游戏文本的基指针错误\n");
		return 1;
	}




	text	=	(u8*)malloc(1024*10);
	for(index=0;index<0x1350;index++){
		text_addr	=	texts[index];
		if(text_addr&0x80000000)
		{
			text_addr&=(~0x80000000);
			if(!strcpy((char*)text,(char*)rom_data(r,text_addr)))
			{
				continue;
			}
		}
		else
		{
			if(huffman_decode(r,text_addr,text))
			{
				continue;
			}
		}
		out_C_str_(file,text,index);
		fprintf(file,"\n");
	}

	fclose(file);
	free(text);
	return 0;
}


static	char	mtext[10*1024];		//10kb

int		out_C_str_index_(FILE*file,rom*r,int index){
	u32*	texts;
	u32		text_addr;
	texts	=	(u32*)rom_data(r,r->texts_base);
	if(!texts){
		printf("游戏文本的基指针错误\n");
		return 1;
	}
	text_addr	=	texts[index];
	if(text_addr&0x80000000)
	{
		text_addr&=(~0x80000000);
		if(!strcpy((char*)mtext,(char*)rom_data(r,text_addr)))
		{
			return 1;
		}
	}
	else if (text_addr==0){
		return 1;
	}
	else
	{
		if(huffman_decode(r,text_addr,mtext))
		{
			return 1;
		}
	}

	out_C_str_(file,mtext,index);
	return 0;
}


#ifdef TLP_ITEM_
//添加的代码
typedef struct{
	u16		name_number;
	u16		desc;
	u8		u4_[32];
}item;

int		TLP_item_txt_export(rom*r,u32 addr){
	item*		item_ptr;
	item*		item_base_ptr;
	int			i;
	FILE	*file;
	char	fname[0x50];

	sprintf(fname,"%s.item.c",r->filename);
	file	=	fopen(fname,"w+");


	item_base_ptr	=	(item*)rom_data(r,addr);
	item_ptr		=	item_base_ptr;
	if(!item_ptr){
		printf("地址(0x%8X)错误,必须是gba rom的地址范围\n",addr);
		return 1;
	}

	item_ptr		=	item_base_ptr;
	fprintf(file,"//%s\n","道具名");
	for(i=0;i<0x100;i++){
		//printf("\t[0x%04X]\t=\t\t//%02X\n",item_ptr->name_number,i);
		out_C_str_index_(file,r,item_ptr->name_number);
		fprintf(file,"\t\t\t//道具编号:%02X",i);
		fprintf(file,"\n");
		item_ptr	++;
	}

	item_ptr		=	item_base_ptr;
	fprintf(file,"//%s\n","道具描述");
	for(i=0;i<0x100;i++){
		//printf("\t[0x%04X]\t=\t\t//%02X\n",item_ptr->desc,i);
		out_C_str_index_(file,r,item_ptr->desc);
		fprintf(file,"\t\t\t//道具编号:%02X",i);
		fprintf(file,"\n");
		item_ptr	++;
	}


	return 0;
}
#endif


#ifdef  TLP_CHAR_
typedef struct{
	u16		name_number;
	u16		desc;
	u8		unk_[0x30];
}type;
const	char* 	file_sub 	= 	".char.c";
const	char*	name_note	=	"人物名";
const  	char*	name_number_note	=	"人物编号";
const	char*	desc_note	=	"人物描述";
const	char*	desc_number_note	=	"";
#endif

#ifdef TLP_CLASS_
typedef struct{
	u16		name_number;
	u16		desc;
	u8		unk_[0x50];
}type;
const	char* 	file_sub 	= 	".class.c";
const	char*	name_note	=	"职业名";
const  	char*	name_number_note	=	"职业编号";
const	char*	desc_note	=	"职业描述";
const	char*	desc_number_note	=	"";
#endif


int		TLP_type_txt_export(rom*r,u32 addr){
	type*		type_ptr;
	type*		type_base_ptr;
	int			i;
	FILE	*file;
	char	fname[0x50];

	sprintf(fname,"%s%s",r->filename,file_sub);
	file	=	fopen(fname,"w+");


	type_base_ptr	=	(type*)rom_data(r,addr);
	type_ptr		=	type_base_ptr;
	if(!type_ptr){
		printf("地址(0x%8X)错误,必须是gba rom的地址范围\n",addr);
		return 1;
	}

	type_ptr		=	type_base_ptr;
	fprintf(file,"//%s\n",name_note);
	for(i=0;i<0x100;i++){
		//printf("\t[0x%04X]\t=\t\t//%02X\n",item_ptr->name_number,i);
		out_C_str_index_(file,r,type_ptr->name_number);
		fprintf(file,"\t\t\t//%s:%02X",name_number_note,i);
		fprintf(file,"\n");
		type_ptr	++;
	}

	type_ptr		=	type_base_ptr;
	fprintf(file,"//%s\n",desc_note);
	for(i=0;i<0x100;i++){
		//printf("\t[0x%04X]\t=\t\t//%02X\n",item_ptr->desc,i);
		out_C_str_index_(file,r,type_ptr->desc);
		fprintf(file,"\t\t\t//:%02X",i);
		fprintf(file,"\n");
		type_ptr	++;
	}


	return 0;
}


int		main(int c,char**v){
	if(c==1){
		//没有文件,默认使用fe7tlp.gba
		rom_init(&TLP_rom);

		//out_C_str(&TLP_rom);
		//TLP_item_txt_export(&TLP_rom,0x9250000);
#ifdef TLP_CHAR_
		TLP_type_txt_export(&TLP_rom,0x9D00000);
#endif

#ifdef TLP_CLASS_
		TLP_type_txt_export(&TLP_rom,0x9BC7000);
#endif

		rom_free(&TLP_rom);
	}
	return 0;
}


