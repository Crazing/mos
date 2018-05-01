/*
 * File: /include/lib/string.h
 * Project: MOS
 * File Created: Wednesday, 25th April 2018 8:37:50 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 1st May 2018 6:44:26 pm
 * Modified By: zhixiang
 * -----
 * FileContent: �ַ�����������
 */

#ifndef	_MOS_STRING_H_
#define	_MOS_STRING_H_


#include "type.h"

//�����ֽڽ��и���
void* memcpy(void* p_dst, void* p_src, t_32 size);

//��ָ���������ַ����Ƶ�dst
void  memset(void* p_dst, t_8 ch, t_32 size);

//�ַ�������
t_8*  strcpy(t_8* p_dst, t_8* p_src);

//��ȡ�ַ�������
t_32  strlen(const t_8* p_src);

//�Ӵ�����
t_8*  strstr(const t_8* string, const t_8* search);

//�����ַ����е�һ�γ���ch��λ��
t_8*  strchr(const t_8* str, t_8 ch);

#endif /* _MOS_STRING_H_ */