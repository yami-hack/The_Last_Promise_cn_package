/*
 * text_font.c
 *
 *  Created on: 2015年4月19日
 *      Author: yami
 */


/**
 * @file		text_font.c
 * @date		2015-04-19 13:56:00
 * @date		2015-04-19 13:56:04
 * @brief
 * 有关字模的提取
 * TLP里面的字模是16x16的,bit:2的字模
 * 头5个字节暂时没用,u5_是宽度
 */

#include "version.h"
#include "types.h"

extern		void	*fonts[];
extern		void	*fonts2[];

void*		text_get_font(u32 *font_base,u8*text){
#ifdef TLP_CN
	u8		codeA;
	u8		codeB;
	codeA	=	*text++;
	codeB	=	*text++;
	if(codeA>=0x80){
		struct{
			void	*next;
			u8		codeB;
			u8		w;
			u8		h;		//?
			u8		data[1];
		}*f_font;

		if((u32)font_base==0x8B8B5B0){
			//这个是普通文本的字模基址
			f_font	=	fonts[codeA];
		}
		else{
			f_font	=	fonts2[codeA];
		}
		while(f_font){
			if(f_font->codeB==codeB){
				break;
			}
			f_font		=	f_font->next;
		}

		if(f_font){
			// !f_font==0
			return	f_font;
		}
		else{
			//f_font==0;
			return	(void*)font_base['?'];
		}
	}
#endif
	//codeA<0x80
	return (void*)font_base[codeA?codeA:'?'];
}

#ifdef	TLP_CN_V1
u8		*text_show_h(void*a0,void*a1){
	u8		*text;
	u32		*code_pointers;
	u32		code_font_ptr;
	u8		code;
	struct{
		u32		u0_;
		u32		*u4_;						//字库基址
		void	(*func)(void*,void*);		//执行的函数指针
	}*var1;
	var1			=	*((void**)0x2028d70);	//字库基址
	code_pointers	=	var1->u4_;


	text			=	a1;
	code_font_ptr	=
			(u32)text_get_font(code_pointers,text);
			//sub_80BFC54((u32)code_pointers,text,text_get_font);
	//以上等价于text_get_font(text,code_pointers);

	if(*text>=0x80){
		text		+=2;
	}
	else
	{
		text		++;
	}

	//sub_80BFC54(a0,code_font_ptr,var1->u8_);
	var1->func(a0,(void*)code_font_ptr);
	return text;
}


void		text2_show_h(void*a0,void*a1){
	u32		code_font_ptr;
	u8		*text;
	u32		*code_pointers;
	struct{
		u32		u0_;
		u32		*u4_;						//字库基址
		void	(*func)(void*,void*);		//执行的函数指针
	}*var1;

	do{
		var1			=	*((void**)0x2028d70);	//字库基址
		code_pointers	=	var1->u4_;

		code_font_ptr	=
				(u32)text_get_font(code_pointers,text);

		if(*text<=1){
			return;
		}

		if(*text>=0x80){
			text		+=2;
		}
		else
		{
			text		++;
		}
		var1->func(a0,(void*)code_font_ptr);

	}while(1);
}




s32			text2_show_h_width(u8*a0,u32*code_pointers){
	//获得文本宽度
	u8		*text;
	struct{
		void	*next;
		u8		codeB;
		u8		w;
		u8		h;		//?
		u8		data[1];
	}*code_font_ptr;
	s32		w;
	w		=	0;
	text	=	a0;
	while(*text>1)
	{
		code_font_ptr	=
				text_get_font(code_pointers,text);
		w		+=		code_font_ptr->w;
		if(*text>=0x80){
			text	+=	2;
		}
		else{
			text	++;
		}
	}
	return w;
}



#endif

