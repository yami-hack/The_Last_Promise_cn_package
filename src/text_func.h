/*
 * text_func.h
 *
 *  Created on: 2015年4月19日
 *      Author: yami
 */


/**
 * @file		text_func.h
 * @author		yami
 * @date		2015-04-19 14:39:49
 * @brief
 * 一些全局函数
 */

#ifndef SRC_TEXT_FUNC_H_
#define SRC_TEXT_FUNC_H_

#include "types.h"

extern 	void*		text_get_font(u8*text,u32 *font_base);
extern	u8*			*text_show_h(void*a0,void*a1);
extern	void		text2_show_h(void*a0,void*a1);
extern	s32			text2_show_h_width(u8*a0,u8*code_pointers);

#endif /* SRC_TEXT_FUNC_H_ */
