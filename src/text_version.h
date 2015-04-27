/*
 * text_version.h
 *
 *  Created on: 2015年4月20日
 *      Author: yami
 */


/**
 * @file		text_version.h
 * @date		2015-04-20 17:00:36
 * @author		yami
 * @brief
 * 这个是文本版本,是翻译人员定义,
 *
 * 如果翻译者添加自己的版本,在这个文件添加#define TLP_CN_(NAME)_(FLAG)_
 * (NAME)是翻译者名字,英文简写,(FLAG),是标识,标识汉化了哪部分,如"ITEM",汉化道具
 * 然后再到texts.c文件夹看说明,怎么添加汉化的文本
 *	V1:
 *		2015-04-20 17:01:31
 *			yami添加一些道具名翻译,未修改
 */


#ifndef SRC_TEXT_VERSION_H_
#define SRC_TEXT_VERSION_H_


//例子,YAMI,itemname
//#define TLP_CN_YAMI_ITEMNAME_
//#define	TLP_CN_YAMI_INFO			"由yami提供的测试翻译"
//#define	TLP_CN_INFO_BASE			0x1400


//2015-04-26 20:38:03
//TLPcn 小组翻译
//文件夹在TLPcn
//
#define	TLP_CN_TLPcn_


#endif /* SRC_TEXT_VERSION_H_ */
