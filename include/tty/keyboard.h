/*
 * File: /include/tty/keyboard.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:49:28 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 1st May 2018 7:17:16 pm
 * Modified By: zhixiang
 * -----
 * FileContent: �������봦�����
 */


#ifndef	_MOS_KEYBOARD_H_
#define	_MOS_KEYBOARD_H_

#include "global.h"
#include "common.h"
#include "string.h"

//�����жϴ�����
void keyboard_handler(t_32 irq);

//��ʼ�������ж�
void init_keyboard();

//�Ӽ������뻺������ȡ�ַ���ת����ϵͳ����ʶ��ı�ʾ��
t_bool keyboard_read(ut_32* code);

#endif /* _MOS_KEYBOARD_H_ */