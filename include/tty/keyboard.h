/*
 * File: /include/tty/keyboard.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:49:28 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Friday, 27th April 2018 10:35:09 am
 * Modified By: zhixiang
 * -----
 * FileContent: �������봦�����
 */


#ifndef	_MOS_KEYBOARD_H_
#define	_MOS_KEYBOARD_H_

#include "global.h"
#include "common.h"

//�����жϴ�����
void keyboard_handler(int irq);

//��ʼ�������ж�
void init_keyboard();

//�Ӽ������뻺������ȡ�ַ���ת����ϵͳ����ʶ��ı�ʾ��
t_bool keyboard_read(t_32* code);

#endif /* _MOS_KEYBOARD_H_ */