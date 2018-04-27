/*
 * File: /src/tty/tty.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 11:48:42 am
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Friday, 27th April 2018 10:37:31 am
 * Modified By: zhixiang
 * -----
 * FileContent: 终端任务程序主体
 */


#include "tty.h"

TTY     tty_table[NR_TTY];

//处理从输入缓冲区处理返回的标识符
static void handle_key(TTY* p_tty, t_32 key)
{
	if (!(key & FLAG_EXT)) {
		if (p_tty->inbuf_count < TTY_IN_BYTES) {
			*(p_tty->p_inbuf_head) = key;
			p_tty->p_inbuf_head++;
			if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES) {
				p_tty->p_inbuf_head = p_tty->in_buf;
			}
			p_tty->inbuf_count++;
		}
	}
	else {
		int raw_code = key & MASK_RAW;
		switch(raw_code) {
		case UP:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {	/* Shift + Up */
				scroll_screen(p_tty->p_console, SCROLL_SCREEN_UP);
			}
			break;
		case DOWN:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {	/* Shift + Down */
				scroll_screen(p_tty->p_console, SCROLL_SCREEN_DOWN);
			}
			break;
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
		case F11:
		case F12:
			if ((key & FLAG_ALT_L) || (key & FLAG_ALT_R)) {	/* Alt + F1~F12 */
				select_console(raw_code - F1);
			}
			break;
		default:
			break;
		}
	}
}

//尝试读取字符
static void tty_do_read(TTY* p_tty)
{
    t_32     key  = 0;
    t_bool   flag = FALSE;
	if (is_current_console(p_tty->p_console)) {
		flag = keyboard_read(&key);
        if (flag)
            handle_key(p_tty, key);
	}
}


//尝试写入字符串
static void tty_do_write(TTY* p_tty)
{
	if (p_tty->inbuf_count) {
		char ch = *(p_tty->p_inbuf_tail);
		p_tty->p_inbuf_tail++;
		if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_tail = p_tty->in_buf;
		}
		p_tty->inbuf_count--;

		out_char(p_tty->p_console, ch);
	}
}

//初始化tty
static void init_tty(TTY* p_tty)
{
	p_tty->inbuf_count = 0;
	p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;

	init_screen(p_tty);
}

//终端进程
void task_tty()
{
	TTY*	p_tty;

	for (p_tty=tty_table;p_tty<tty_table+NR_TTY;p_tty++) {
		init_tty(p_tty);
	}

	select_console(0);

	while (1) {
		for (p_tty=tty_table;p_tty<tty_table+NR_TTY;p_tty++) {
			tty_do_read(p_tty);
			tty_do_write(p_tty);
		}
	}
}