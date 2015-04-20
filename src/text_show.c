/*
 * text_show.c
 *
 *  Created on: 2015年4月19日
 *      Author: yami
 */

/**
 * @file		text_show.c
 * @author		yami
 * @date		2015-04-19 14:05:06
 * @brief
 * 显示文字
 */

#include "types.h"
#include "text_func.h"
#include "version.h"


SECTION(".8005B9C")
void* 	text_show(void*a0,void*a1){
	u8		*text;
#ifdef	TLP_CN_V1
	text	=	a0;
	text	=	(u8*)sub_80BFC54(a0,a1,text_show_h);
#else
	u32		*code_pointers;
	u32		code_font_ptr;
	u8		code;
	struct{
		u32		u0_;
		u32		*u4_;		//字库基址
		u32		*u8_;		//执行的函数指针
	}*var1;
	var1			=	*((void**)0x2028d70);	//字库基址
	code_pointers	=	var1->u4_;

#ifdef TLP_CN
	text			=	a1;
	code_font_ptr	=	sub_80BFC54((u32)code_pointers,text,text_get_font);
	//以上等价于text_get_font(text,code_pointers);
	text			++;
	if(*text>=0x80){
		text		++;
	}
#else
	text			=	a1;
	code			=	*text;
	code_font_ptr	= 	code_pointers[code];
	text			++;
	if(!code){
		//这个是
		code_font_ptr	=	code_pointers['?'];
	}
#endif
	sub_80BFC54(a0,code_font_ptr,var1->u8_);
#endif
	return text;
}


SECTION(".8005B60")
void		text2_show(void*a0,void*a1){
	u8		*text;
#ifdef	TLP_CN_V1
	text	=	(u8*)sub_80BFC54(a0,a1,text2_show_h);
#else
	//未写
#endif
}

SECTION(".8005C00")
s32		text2_show_width(void*a0){
	//文本的宽度
	struct{
		u32		u0_;
		u32		*u4_;		//字库基址
		u32		*u8_;		//执行的函数指针
	}*var1;
	var1			=	*((void**)0x2028d70);	//字库基址
	return	sub_80BFC54(a0,var1->u4_,text2_show_h_width);
}



