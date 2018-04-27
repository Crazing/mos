/*
 * File: /src/interrupt/sys_call.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 3:26:17 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 23rd April 2018 9:43:39 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 系统调用文件
 */


#include "sys_call.h"

//系统调用
t_sys_call		sys_call_table[NR_SYS_CALL] = {sys_get_ticks};

//获取ticks系统调用
t_32 sys_get_ticks()
{
	return ticks;
}