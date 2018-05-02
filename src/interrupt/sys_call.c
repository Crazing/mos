/*
 * File: /src/interrupt/sys_call.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 3:26:17 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 10:19:38 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ϵͳ�����ļ�
 */


#include "sys_call.h"

static t_32 sys_get_ticks();
static t_32 sys_write( PROCESS* p_proc, t_8* buf, t_32 len);

//ϵͳ����
t_sys_call		sys_call_table[NR_SYS_CALL] = {sys_get_ticks,sys_write};

//��ȡticksϵͳ����
static t_32 sys_get_ticks()
{
    t_32 ticks = get_clock_ticks();
	return ticks;
}

//���ն�����ַ���
static void tty_write(TTY* p_tty, t_8* buf, t_32 len)
{
	t_8* p = buf;
	t_32 i = len;

	while (i) {
		out_char(p_tty->p_console, *p);
        p++;
		i--;
	}
}

//ϵͳ���ã��������
static t_32 sys_write( PROCESS* p_proc, t_8* buf, t_32 len)
{
	tty_write(&tty_table[p_proc->nr_tty], buf, len);
	return 0;
}