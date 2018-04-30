/*
 * File: /src/interrupt/sys_call.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 3:26:17 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 30th April 2018 6:16:26 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ϵͳ�����ļ�
 */


#include "sys_call.h"

static t_32 sys_get_ticks();

//ϵͳ����
t_sys_call		sys_call_table[NR_SYS_CALL] = {sys_get_ticks};

//��ȡticksϵͳ����
static t_32 sys_get_ticks()
{
    t_32 ticks = get_clock_ticks();
	return ticks;
}