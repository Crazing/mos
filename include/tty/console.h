/*
 * File: /include/tty/console.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:26:16 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 30th April 2018 1:06:08 am
 * Modified By: zhixiang
 * -----
 * FileContent: ����̨����
 */


#ifndef	_MOS_CONSOLE_H_
#define	_MOS_CONSOLE_H_

#include "global.h"
#include "common.h"

//����һ������̨
t_bool apply_console(CONSOLE** p_pcon);

//ѡ�����̨
void select_console(const CONSOLE* p_con);

//�����̨����ַ�
void out_char(CONSOLE* p_con, ut_8 ch);

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