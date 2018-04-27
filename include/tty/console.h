/*
 * File: /include/tty/console.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:26:16 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 25th April 2018 9:47:49 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 控制台函数
 */


#ifndef	_MOS_CONSOLE_H_
#define	_MOS_CONSOLE_H_

#include "global.h"
#include "common.h"

//初始化控制台
void init_screen(TTY* p_tty);

//选择控制台
void select_console(t_32 nr_console);

//向控制台输出字符
void out_char(CONSOLE* p_con, t_8 ch);

//判断是否是活动控制台
t_bool is_current_console(CONSOLE* p_con);

/*======================================================================*
                           scroll_screen
 *----------------------------------------------------------------------*
 滚屏.
 *----------------------------------------------------------------------*
 direction:
	SCROLL_SCREEN_UP	: 向上滚屏
	SCROLL_SCREEN_DOWN	: 向下滚屏
	其它			: 不做处理
 *======================================================================*/
void scroll_screen(CONSOLE* p_con, t_32 direction);

#endif /* _MOS_CONSOLE_H_ */