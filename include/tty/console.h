/*
 * File: /include/tty/console.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:26:16 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 30th April 2018 1:06:08 am
 * Modified By: zhixiang
 * -----
 * FileContent: 控制台函数
 */


#ifndef	_MOS_CONSOLE_H_
#define	_MOS_CONSOLE_H_

#include "global.h"
#include "common.h"

//申请一个控制台
t_bool apply_console(CONSOLE** p_pcon);

//选择控制台
void select_console(const CONSOLE* p_con);

//向控制台输出字符
void out_char(CONSOLE* p_con, ut_8 ch);

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