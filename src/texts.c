/*
 * texts.c
 *
 *  Created on: 2015年4月19日
 *      Author: yami
 */


/**
 * @file		texts.c
 * @author		yami
 * @date		2015-04-19 01:13:21
 * @brief
 * 这些是文本修改
 * 注意
 * 1,必须要GBK保存文件
 * 2,最好是以
 * 		#ifdef 	(翻译者在text_version.h定义的信息)
 * 		#include "翻译文本""
 * 		#endif
 * 	 形式来下,下面有例子
 * 3,
 * 	翻译形式
 * 	[文本ID]	=	"翻译文本"
 */

#include "types.h"
#include "text_version.h"

#define		N		+ 0x80000000
#define		TXT		(u32)

u32			texts[] = {
#include		"textpoters.h"
#ifdef	TLP_CN_YAMI_ITEMNAME_
//有yami提供的翻译文本,如果不用的话,请在text_version.h注释掉TLP_CN_YAMI_ITEMNAME_,
		//或者删掉那一行
#include		"yami/item_text.h"
#endif


};

//u32		texts[] = {
////旧版的指针,为了方便一些未修改的玩意
//#include		"textpoters.h"
//
//
//
////这个是测试
////[0x01B1]	=	TXT"一个男人是一个二货\x01of justice and dedication.\x01He suffers from deep wounds..." N,
////[0x0B9B]	=	TXT"abc"N,
////[0x04DE]	=	TXT"二货"N,
////[0x03F3]	=	TXT"铁剑"N,
//};
