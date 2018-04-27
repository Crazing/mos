/*
 * File: /include/lib/string.h
 * Project: MOS
 * File Created: Wednesday, 25th April 2018 8:37:50 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 25th April 2018 8:43:13 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 字符串操作函数
 */

#ifndef	_MOS_STRING_H_
#define	_MOS_STRING_H_


#include "type.h"

//按照字节进行复制
void* memcpy(void* p_dst, void* p_src, t_32 size);

//将指定数量的字符复制到dst
void  memset(void* p_dst, char ch, int size);

//字符串复制
char* strcpy(char* p_dst, char* p_src);

#endif /* _MOS_STRING_H_ */