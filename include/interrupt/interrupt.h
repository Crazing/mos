/*
 * File: /include/interrupt/interrupt.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 11:11:52 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 24th April 2018 10:57:26 pm
 * Modified By: zhixiang
 * -----
 * FileContent: �жϴ����������
 */


#ifndef	_MOS_INTERRUPT_H_
#define	_MOS_INTERRUPT_H_

#include "global.h"
#include "8259A.h"



//��ʼ���жϴ���
void    init_interrupt();

//���жϹ���
void    open_interrupt();

//�쳣����
void    exception_handler(t_32 vec_no, t_32 err_code, t_32 eip, t_32 cs, t_32 eflags);

//�ں���������ķ���
void	restart();

/* �жϴ����� */
void	divide_error();
void	single_step_exception();
void	nmi();
void	breakpoint_exception();
void	overflow();
void	bounds_check();
void	inval_opcode();
void	copr_not_available();
void	double_fault();
void	copr_seg_overrun();
void	inval_tss();
void	segment_not_present();
void	stack_exception();
void	general_protection();
void	page_fault();
void	copr_error();
void	hwint00();
void	hwint01();
void	hwint02();
void	hwint03();
void	hwint04();
void	hwint05();
void	hwint06();
void	hwint07();
void	hwint08();
void	hwint09();
void	hwint10();
void	hwint11();
void	hwint12();
void	hwint13();
void	hwint14();
void	hwint15();

void	sys_call();		/* t_pf_int_handler */

#endif /* _MOS_INTERRUPT_H_ */