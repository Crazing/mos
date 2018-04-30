/*
 * File: /include/lib/string.h
 * Project: MOS
 * File Created: Wednesday, 25th April 2018 8:37:50 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 30th April 2018 6:02:30 pm
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

#endif /* _MOS_STRING_H_ */