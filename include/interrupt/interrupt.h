/*
 * File: /include/interrupt/interrupt.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 11:11:52 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Sunday, 29th April 2018 10:14:59 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 中断处理程序主体
 */


#ifndef	_MOS_INTERRUPT_H_
#define	_MOS_INTERRUPT_H_

#include "global.h"
#include "8259A.h"



//初始化中断处理
void    init_interrupt();

//打开中断功能
void    open_interrupt();

//异常处理
void    exception_handler(t_32 vec_no, ut_32 err_code, ut_32 eip, ut_32 cs, ut_32 eflags);

//内核跳入任务的方法
void	restart();

/* 中断处理函数 */
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