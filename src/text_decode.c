/*
 * text_decode.c
 *
 *  Created on: 2015年4月19日
 *      Author: yami
 */


/**
 * @file		text_decode.c
 * @author		yami
 * @date		2015-04-19 00:56:59
 * @brief
 * 这是解码源码
 */

#include "types.h"

extern		u8*		texts[];
extern		void	text_decode(void*,void*);


u8*	SECTION(".8012C9C")	 text_index_decode(u32 index,u8*dst){
	text_decode(
			texts[index],
			dst
			);
	return dst;
}


