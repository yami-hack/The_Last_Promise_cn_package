/*
 * text_draw.c
 *
 *  Created on: 2015年4月23日
 *      Author: yami
 */


/**
 * @file		text_draw.c
 * @author		yami
 * @date		2015-04-23 19:55:46
 * @brief
 * 有关文字的绘制
 * 基本是大小等同原ROM,以后的bug待修复
 */


#include "types.h"
#include "version.h"


SECTION(".8000564")
 void	text_draw(u16*a0,u32*a1,u32*a2,u32 x){
	int			line;
	u32			*x_vpointer;
	u32			x_v;
	u32			V;
	line	=	0xF;
	//a2是字模数据
#ifdef TLP_CN
	do{
		x_vpointer	=	(void*)0x8000540;
		x_v			=	(x<<2);
		V			=	a2[0];
		if(V){
			u32		tilev,tilev2;
			u32		t1,t2;
//			x_v			*=	*a2;		//在X偏移的值
//			tilev		=	*((u32*)(a0+(x_v&0xff)));
//			tilev2		=	*((u32*)(a0+((x_v>>0x8)&0xff)));
//			tilev		&= 0xffff;
//			tilev		|=	(tilev2<<0x10);
//			*a1			|=	tilev;
//
//			tilev		=	*((u32*)(a0+((x_v>>0x10)&0xff)));
//			tilev2		=	*((u32*)(a0+((x_v>>0x18)&0xff)));
//			tilev		&= 0xffff;
//			tilev		|=	(tilev2<<0x10);
//			a1[0x10]	|=	tilev;
			tilev		=	*((u32*)(a0+(V&0xff)));
			tilev2		=	*((u32*)(a0+((V>>0x8)&0xff)));
			tilev		&= 0xffff;
			tilev		|=	(tilev2<<0x10);
			t1			=	tilev;

			tilev		=	*((u32*)(a0+((V>>0x10)&0xff)));
			tilev2		=	*((u32*)(a0+((V>>0x18)&0xff)));
			tilev		&= 0xffff;
			tilev		|=	(tilev2<<0x10);
			t2			=	tilev;

			a1[0]		|=	t1<<x_v;
			a1[0x10]	|=	((t2<<x_v)|(t1>>(32-x_v)));
			a1[0x20]	|=	(t2>>(32-x_v));

		}
		a1++;
		a2++;
	}while(--line>=0);
#else
	do{
		x_vpointer	=	(void*)0x8000540;
		x_v			=	x_vpointer[x];
		V			=	a2[0];
		if(V){
			u32		tilev,tilev2;
			x_v			*=	*a2;		//在X偏移的值
			tilev		=	*((u32*)(a0+(x_v&0xff)));
			tilev2		=	*((u32*)(a0+((x_v>>0x8)&0xff)));
			tilev		&= 0xffff;
			tilev		|=	(tilev2<<0x10);
			*a1			|=	tilev;

			tilev		=	*((u32*)(a0+((x_v>>0x10)&0xff)));
			tilev2		=	*((u32*)(a0+((x_v>>0x18)&0xff)));
			tilev		&= 0xffff;
			tilev		|=	(tilev2<<0x10);
			a1[0x10]	|=	tilev;
		}
		a1++;
		a2++;
	}while(--line>=0);
#endif
}
