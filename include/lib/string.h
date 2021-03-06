/*
 * File: /include/lib/string.h
 * Project: MOS
 * File Created: Wednesday, 25th April 2018 8:37:50 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 1st May 2018 6:44:26 pm
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
void  memset(void* p_dst, t_8 ch, t_32 size);

//字符串复制
t_8*  strcpy(t_8* p_dst, t_8* p_src);

//获取字符串长度
t_32  strlen(const t_8* p_src);

//子串查找
t_8*  strstr(const t_8* string, const t_8* search);

//查找字符串中第一次出现ch的位置
t_8*  strchr(const t_8* str, t_8 ch);

#endif /* _MOS_STRING_H_ */