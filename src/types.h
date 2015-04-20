/*
 * types.h
 *
 *  Created on: 2015年4月19日
 *      Author: yami
 */


/**
 * @file	types.h
 * @author	yami
 * @date	2015-04-19 00:53:27
 * @brief
 * 有关类型的定义
 */

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

//自定义段名
#define 	SECTION(s) 	__attribute__((__section__(s)))

typedef		unsigned char	u8;
typedef		unsigned short	u16;
typedef		unsigned int	u32;

typedef		signed char		s8;
typedef		signed short	s16;
typedef		signed int		s32;


#endif /* SRC_TYPES_H_ */
