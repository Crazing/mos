/*
 * File: /include/tty/console.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:26:16 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 25th April 2018 9:47:49 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ����̨����
 */


#ifndef	_MOS_CONSOLE_H_
#define	_MOS_CONSOLE_H_

#include "global.h"
#include "common.h"

//��ʼ������̨
void init_screen(TTY* p_tty);

//ѡ�����̨
void select_console(t_32 nr_console);

//�����̨����ַ�
void out_char(CONSOLE* p_con, t_8 ch);

//�ж��Ƿ��ǻ����̨
t_bool is_current_console(CONSOLE* p_con);

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
void scroll_screen(CONSOLE* p_con, t_32 direction);

#endif /* _MOS_CONSOLE_H_ */