
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              console.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*
	�س���:	�ѹ���Ƶ���һ��
	���м�:	�ѹ��ǰ������һ��
*/


#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"


/* ���ļ��ں������� */
PRIVATE void	set_cursor(unsigned int position);
PRIVATE void	set_video_start_addr(t_32 addr);


/*======================================================================*
                           init_screen
 *======================================================================*/
PUBLIC void init_screen(TTY* p_tty)
{
	int nr_tty = p_tty - tty_table;
	p_tty->p_console = console_table + nr_tty;

	int v_mem_size = V_MEM_SIZE >> 1;	/* �Դ��ܴ�С (in WORD) */

	int con_v_mem_size			= v_mem_size / NR_CONSOLES;		/* ÿ������̨ռ���Դ��С		(in WORD) */
	p_tty->p_console->original_addr		= nr_tty * con_v_mem_size;		/* ��ǰ����̨ռ���Դ濪ʼ��ַ		(in WORD) */
	p_tty->p_console->v_mem_limit		= con_v_mem_size;			/* ��ǰ����̨ռ���Դ��С		(in WORD) */
	p_tty->p_console->current_start_addr	= p_tty->p_console->original_addr;	/* ��ǰ����̨��ʾ�����Դ��ʲôλ��	(in WORD) */

	p_tty->p_console->cursor = p_tty->p_console->original_addr;	/* Ĭ�Ϲ��λ�����ʼ�� */

	if (nr_tty == 0) {
		p_tty->p_console->cursor = disp_pos / 2;	/* ��һ������̨����ԭ���Ĺ��λ�� */
		disp_pos = 0;
	}
	else {
		out_char(p_tty->p_console, nr_tty + '0');
		out_char(p_tty->p_console, '#');
	}

	set_cursor(p_tty->p_console->cursor);
}


/*======================================================================*
                           out_char
 *======================================================================*/
PUBLIC void out_char(CONSOLE* p_con, char ch)
{
	t_8* p_vmem = (t_8*)(V_MEM_BASE + p_con->cursor * 2);

	*p_vmem++ = ch;
	*p_vmem++ = DEFAULT_CHAR_COLOR;
	p_con->cursor++;

	set_cursor(p_con->cursor);
}


/*======================================================================*
                           is_current_console
 *======================================================================*/
PUBLIC t_bool is_current_console(CONSOLE* p_con)
{
	return (p_con == &console_table[nr_current_console]);
}


/*======================================================================*
                            set_cursor
 *======================================================================*/
PRIVATE void set_cursor(unsigned int position)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_CURSOR_H);
	out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_CURSOR_L);
	out_byte(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}


/*======================================================================*
                          set_video_start_addr
 *======================================================================*/
PRIVATE void set_video_start_addr(t_32 addr)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_START_ADDR_H);
	out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_START_ADDR_L);
	out_byte(CRTC_DATA_REG, addr & 0xFF);
	enable_int();
}


/*======================================================================*
                           select_console
 *======================================================================*/
PUBLIC void select_console(int nr_console)	/* 0 ~ (NR_CONSOLES - 1) */
{
	if ((nr_console < 0) || (nr_console >= NR_CONSOLES)) {	/* invalid console number */
		return;
	}

	nr_current_console = nr_console;

	set_cursor(console_table[nr_console].cursor);
	set_video_start_addr(console_table[nr_console].current_start_addr);
}


/*======================================================================*
                           scroll_screen
 *----------------------------------------------------------------------*
 ����.
 *----------------------------------------------------------------------*
 direction:
	SCROLL_SCREEN_UP	: ���Ϲ���
	SCROLL_SCREEN_DOWN	: ���¹���
	����			: ��������
 *======================================================================*/
PUBLIC void scroll_screen(CONSOLE* p_con, int direction)
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



