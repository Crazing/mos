/*
 * File: /src/tty/console.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:27:40 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 30th April 2018 6:11:43 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ����̨����
 */


#include "console.h"

//����̨��
static CONSOLE		    console_table[NR_CONSOLES] = {0};

//��ǰ����̨
static CONSOLE*		    p_current_console = NULL;

//��ǰ�ѷ���Ŀ���̨����
static t_32             nr_applied_consoles = 0;
 
//�����ն˵�ǰ��ʾ��ʼ��ַ
static void set_video_start_addr(ut_32 addr)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_START_ADDR_H);
	out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_START_ADDR_L);
	out_byte(CRTC_DATA_REG, addr & 0xFF);
	enable_int();
}

//�趨������ʾλ��
static void set_cursor(ut_32 position)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_CURSOR_H);
	out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CRTC_DATA_IDX_CURSOR_L);
	out_byte(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}

//��ʼ������̨
static init_console(CONSOLE* p_con)
{
	t_32 nr_console = p_con - console_table;

	t_32 v_mem_size = V_MEM_SIZE >> 1;	    /* �Դ��ܴ�С (in WORD) */

	t_32 con_v_mem_size	        = v_mem_size / NR_CONSOLES;		    /* ÿ������̨ռ���Դ��С		(in WORD) */
	p_con->original_addr		= nr_console * con_v_mem_size;		/* ��ǰ����̨ռ���Դ濪ʼ��ַ		 (in WORD) */
	p_con->v_mem_limit		    = con_v_mem_size;			        /* ��ǰ����̨ռ���Դ��С		    (in WORD) */
	p_con->current_start_addr	= p_con->original_addr;	            /* ��ǰ����̨��ʾ�����Դ��ʲôλ��	  (in WORD) */
    p_con->cursor               = p_con->original_addr;	            /* Ĭ�Ϲ��λ�����ʼ�� */

	if (nr_console == 0) {
		p_con->cursor = disp_pos / 2;	    /* ��һ������̨����ԭ���Ĺ��λ�� */
		disp_pos = 0;
	}
	else {
		out_char(p_con, nr_console + '0');
		out_char(p_con, '#');
	}

	set_cursor(p_con->cursor);
}

//�����̨����ַ�
void out_char(CONSOLE* p_con, ut_8 ch)
{
	ut_8* p_vmem = (ut_8*)(V_MEM_BASE + p_con->cursor * 2);

	*p_vmem++ = ch;
	*p_vmem++ = DEFAULT_CHAR_COLOR;
	p_con->cursor++;

	set_cursor(p_con->cursor);
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

//ѡ�����̨
void select_console(const CONSOLE* p_con)	/* 0 ~ (NR_CONSOLES - 1) */
{
    t_32 nr_console = p_con - console_table;
	if ((nr_console < 0) || (nr_console >= nr_applied_consoles)) {	/* invalid console number */
		return;
	}

	p_current_console = p_con;

	set_cursor(p_con->cursor);
	set_video_start_addr(p_con->current_start_addr);
}

//����һ������̨
t_bool apply_console(CONSOLE** p_pcon)
{
    t_bool ret      = FALSE;
    *p_pcon         = NULL;

    if(nr_applied_consoles < NR_CONSOLES)
    {
        *p_pcon = console_table + nr_applied_consoles;
        init_console(*p_pcon);
        ++nr_applied_consoles;
        ret = TRUE;
    }

    return ret;
}