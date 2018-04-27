/*
 * File: /src/tty/console.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:27:40 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Friday, 27th April 2018 12:34:34 am
 * Modified By: zhixiang
 * -----
 * FileContent: 控制台函数
 */


#include "console.h"

//控制台表
CONSOLE		    console_table[NR_CONSOLES];

//当前控制台
t_32		    nr_current_console;

//设置终端当前显示开始地址
static void set_video_start_addr(t_32 addr)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_START_ADDR_H);
	out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_START_ADDR_L);
	out_byte(CRTC_DATA_REG, addr & 0xFF);
	enable_int();
}

//判断是否是活动控制台
t_bool is_current_console(CONSOLE* p_con)
{
	return (p_con == &console_table[nr_current_console]);
}

//设定光标的显示位置
static void set_cursor(ut_32 position)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_CURSOR_H);
	out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_CURSOR_L);
	out_byte(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}

//向控制台输出字符
void out_char(CONSOLE* p_con, t_8 ch)
{
	t_8* p_vmem = (t_8*)(V_MEM_BASE + p_con->cursor * 2);

	*p_vmem++ = ch;
	*p_vmem++ = DEFAULT_CHAR_COLOR;
	p_con->cursor++;

	set_cursor(p_con->cursor);
}

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
void scroll_screen(CONSOLE* p_con, t_32 direction)
{
	if (direction == SCROLL_SCREEN_UP) {
		if (p_con->current_start_addr > p_con->original_addr) {
			p_con->current_start_addr -= SCREEN_WIDTH;
		}
	}
	else if (direction == SCROLL_SCREEN_DOWN) {
		if (p_con->current_start_addr + SCREEN_SIZE < p_con->original_addr + p_con->v_mem_limit) {
			p_con->current_start_addr += SCREEN_WIDTH;
		}
	}
	else{
	}

	set_video_start_addr(p_con->current_start_addr);
	set_cursor(p_con->cursor);
}

//选择控制台
void select_console(t_32 nr_console)	/* 0 ~ (NR_CONSOLES - 1) */
{
	if ((nr_console < 0) || (nr_console >= NR_CONSOLES)) {	/* invalid console number */
		return;
	}

	nr_current_console = nr_console;

	set_cursor(console_table[nr_console].cursor);
	set_video_start_addr(console_table[nr_console].current_start_addr);
}

//初始化控制台
void init_screen(TTY* p_tty)
{
	t_32 nr_tty = p_tty - tty_table;
	p_tty->p_console = console_table + nr_tty;

	t_32 v_mem_size = V_MEM_SIZE >> 1;	/* 显存总大小 (in WORD) */

	t_32 con_v_mem_size	= v_mem_size / NR_CONSOLES;		/* 每个控制台占的显存大小		(in WORD) */
	p_tty->p_console->original_addr		    = nr_tty * con_v_mem_size;		    /* 当前控制台占的显存开始地址		 (in WORD) */
	p_tty->p_console->v_mem_limit		    = con_v_mem_size;			        /* 当前控制台占的显存大小		    (in WORD) */
	p_tty->p_console->current_start_addr	= p_tty->p_console->original_addr;	/* 当前控制台显示到了显存的什么位置	  (in WORD) */

	p_tty->p_console->cursor = p_tty->p_console->original_addr;	/* 默认光标位置在最开始处 */

	if (nr_tty == 0) {
		p_tty->p_console->cursor = disp_pos / 2;	            /* 第一个控制台延用原来的光标位置 */
		disp_pos = 0;
	}
	else {
		out_char(p_tty->p_console, nr_tty + '0');
		out_char(p_tty->p_console, '#');
	}

	set_cursor(p_tty->p_console->cursor);
}