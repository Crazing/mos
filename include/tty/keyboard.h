/*
 * File: /include/tty/keyboard.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:49:28 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Friday, 27th April 2018 10:35:09 am
 * Modified By: zhixiang
 * -----
 * FileContent: 键盘输入处理程序
 */


#ifndef	_MOS_KEYBOARD_H_
#define	_MOS_KEYBOARD_H_

#include "global.h"
#include "common.h"

//键盘中断处理函数
void keyboard_handler(int irq);

//初始化键盘中断
void init_keyboard();

//从键盘输入缓冲区读取字符，转换成系统可以识别的表示符
t_bool keyboard_read(t_32* code);

#endif /* _MOS_KEYBOARD_H_ */