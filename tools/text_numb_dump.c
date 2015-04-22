/*
 * text_numb_dump.c
 *
 *  Created on: 2015年4月22日
 *      Author: yami
 */



/**
 * @file		text_numb_dump.c
 * @author		yami
 * @date		2015-04-22 20:08:42
 * @brief
 * 只是导出编号而已,的字符串而已
 *
 * 停止,2015-04-22 20:58:27
 * 		行:67
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct{
	FILE		*file1;
	FILE		*file2;
	FILE		*file3;
	int			out_flag;
}TXT_dump;


TXT_dump		txt_dump;


static	int	ffsize(FILE*file){
	int		size;
	fseek(file,0,SEEK_END);
	size	=	ftell(file);
	fseek(file,0,SEEK_SET);
	return size;
}

static	char 	format_buf[10*1024];		//10kb
static	char	dump_str[10*1024];			//10kb

int		TXT_dump_text(TXT_dump*td){
	char		*src_txt_buf;
	int			src_size;
	FILE		*out;

	src_size		=	ffsize(td->file1);
	src_txt_buf		=	(char*)calloc(src_size+10,1);
	if(src_txt_buf){
		fprintf(stderr,"申请内存失败\n");
		return 1;
	}
	fread(src_txt_buf,src_size,1,td->file1);

	fseek(td->file2,0,SEEK_SET);

	out	=	td->file3;

	while(fgets(format_buf,10*1024,td->file2)){
		//未写
	}


	free(src_txt_buf);
	return 0;
}


int		TXT_free(TXT_dump*td){
	if(td->file1)
		fclose(td->file1);
	if(td->file2)
		fclose(td->file2);
	if(td->file3&&td->out_flag){
		fclose(td->file3);
	}

	td->file1	=
			td->file2
			=td->file3
			=0;
	return 0;
}

int		main(int c,char**v){
	memset(&txt_dump,0,sizeof(txt_dump));
	if(c<=3){
		fprintf(stderr,"参数错误:原文本 转存编号文本 输出文件\n");
		return 1;
	}
	txt_dump.file1		=	fopen(v[1],"r");
	txt_dump.file2		=	fopen(v[2],"r");
	if(c==3){
		txt_dump.file3	=	stderr;
	}
	else{
		txt_dump.file3	=	fopen(v[3],"w+");
		txt_dump.out_flag	=	1;
	}

	if(!txt_dump.file1){
		fprintf(stderr,"文件(\s)打开失败\n",v[1]);
		return 1;
	}

	if(!txt_dump.file2){
		fprintf(stderr,"文件(\s)打开失败\n",v[2]);
		return 1;
	}

	if(!txt_dump.file3&&txt_dump.out_flag){
		fprintf(stderr,"文件(\s)打开失败\n",v[3]);
		return 1;
	}

	TXT_free(&txt_dump);


}
