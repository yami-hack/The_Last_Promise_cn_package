/*
 * text_replace.c
 *
 *  Created on: 2015年4月20日
 *      Author: yami
 */


/**
 * @file		text_replace.c
 * @author		yami
 * @date		2015-04-20 01:24:15
 * @brief
 * 文本替换程序,生成程序
 * 例子:
 * 	1, 		a.txt内容是  "Iron Sword" = "铁剑",将会替换所以以"Iron Sword"为"铁剑",
 * 			并且标号,如 [0x03F3] = "Iron Sword",会变为[0x03F3] = "铁剑"
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef		unsigned char	u8;
typedef		unsigned short	u16;
typedef		unsigned int	u32;

typedef		signed char		s8;
typedef		signed short	s16;
typedef		signed int		s32;


typedef struct{
	FILE*		file1;
	FILE*		file2;
	FILE*		file3;
	int			out_f;
}text_replace;


text_replace	txt_replace;



int		TR_check_error(text_replace*tr){
	if(!tr->file1){
		return 1;
	}
	if(!tr->file2){
		return 2;
	}
	if(!tr->file3){
		return 3;
	}
}

int		TR_free(text_replace*tr){
	if(tr->file1)
		fclose(tr->file1);
	if(tr->file2)
		fclose(tr->file2);
	if(tr->out_f&&tr->file3)
		fclose(tr->file3);
	return 0;
}

static	u8		src_str[10*1024];		//10kb
static	u8		dst_str[10*1024];		//10kb
static	u8		flag_str[1024];
static	u8		tmp_str[10*1024];		//10kb


/**
 * 查找字符串,并且复制过去,返回的是最后处理的字符串指针
 * @param src	源文本
 * @param dst	目标文本
 * @return
 */
static	u8*		Find_copy(u8*src,u8*dst){

	while(*src!='\"'&&*src!='\0'){
		src	++;
	}
	//以获取到一个文本
	*dst++	=	*src ++;
	while(*src!='"'&&*src!='\0'){
		if(*src=='\\'){
			*dst++	=	*src ++;
		}
		*dst++	=	*src ++;
	}
	*dst++	=	*src ++;
	*dst++	=	0;
	return src;
}

static	u8*		Find_flag(u8*src,u8*dst){
	while(*src!='"'&&*src!='\n'&&*src!='\0'){
		*dst++	=	*src++;
	}
	*dst++ = 0;
	return src;
}

typedef enum{
	TR_FLAG_ERROR	=	-1,
	TR_FLAG_EQU = 1,
}TR_FLAG;

static	int		TR_flag(u8*str){
	int		f;
	f		=	0;
	while(*str!='\0'){
		if(*str=='=')
		{
			if(f&TR_FLAG_EQU)
			{
				f	|=	TR_FLAG_ERROR;
			}
			else{
				f	|=	TR_FLAG_EQU;
			}
		}
		str++;
	}
	return f;
}

static	u8*		str_to_find(u8*str,u8*begin_str,u8 v){
	while(str>=begin_str){
		if(*str==v){
			return str;
		}
		str--;
	}
	return 0;
}


static	int		ffsize(FILE*file){
	int		size;
	fseek(file,0,SEEK_END);
	size	=	ftell(file);
	fseek(file,0,SEEK_SET);
	return size;
}


int		TR_out_C(text_replace*tr){
	u8			*src;
	u8			*dst;
	u8			*current_txt;
	u8			*TR_txt;
	u8			*src_text;
	u8			*current_src_text;
	u8			*hit_str;
	u8			*hit_line_str;
	int			tf;
	int			size;
	FILE*		out_file;

	size		=	ffsize(tr->file2);
	current_txt	=	(u8*)calloc(size+10,1);
	fread(current_txt,size,1,tr->file2);
	TR_txt		=	current_txt;


	size		=	ffsize(tr->file1);
	current_src_text	=	(u8*)calloc(size+10,1);
	fread(current_src_text,size,1,tr->file1);
	src_text	=	current_src_text;


	dst			=	dst_str;
	src			=	src_str;

	out_file	=	tr->file3;

	do{
	//当前的字符串
		current_txt	=	Find_copy(current_txt,src_str);
		current_txt	=	Find_flag(current_txt,flag_str);
		current_txt	=	Find_copy(current_txt,dst_str);
		if(!*current_txt){
			//退出循环
			break;
		}
		tf			=	TR_flag(flag_str);

		if(tf==TR_FLAG_ERROR){
			return 1;
		}
		current_src_text	=	src_text;
		if(tf&TR_FLAG_EQU){
			do{
				u8				c;
				hit_str			=	strstr(current_src_text,src_str);
				if(!hit_str){
					break;
				}
				hit_line_str	=	str_to_find(hit_str,src_text,'\n') + 1;
				c				=	hit_line_str[hit_str - hit_line_str - 1];
				hit_line_str[hit_str - hit_line_str - 1] = '\0';
				sprintf(tmp_str,"%s %s\t\t//%s%s\n",
						hit_line_str,
						dst_str,
						hit_line_str,
						src_str
						);

				fprintf(out_file,"%s",tmp_str);

				hit_line_str[hit_str - hit_line_str - 1] = c;
				current_src_text	=	hit_str +  strlen(src_str);
			}while(1);
		}
	}while(1);


	free(TR_txt);
	free(src_text);
	return 0;
}




int		main(int c,char**v){
	memset(&txt_replace,0,sizeof(txt_replace));
	if(c<3){
		printf("参数错误,请检查\n");
		printf("参数格式: 程序 文本文件 替换的内容文件 [输出文件]");
		return 1;
	}
	txt_replace.file1	=	fopen(v[1],"r");
	txt_replace.file2	=	fopen(v[2],"r");
	if(c>3){
		txt_replace.file3	=	fopen(v[3],"w+");
		txt_replace.out_f	=	1;
	}
	else{
		txt_replace.file3	=	stderr;
	}

	TR_out_C(&txt_replace);

	TR_free(&txt_replace);


	return 0;
}
