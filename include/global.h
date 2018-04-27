/*
 * File: /include/global.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 8:55:29 am
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 25th April 2018 9:37:57 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ȫ�ֹ�������
 */


#ifndef	_MOS_GLOBAL_H_
#define	_MOS_GLOBAL_H_

#include "type.h"

//kernel.asm
extern ut_32	        disp_pos;           //��ӡλ��

//interrupt.asm
extern ut_32	        k_reenter;          //�ж������жϱ���
extern  t_32            ticks;              //ʱ���жϴ��� 

//8259A.c
extern t_pf_irq_handler	irq_table[];        //�жϴ����

//sys_call.c
extern t_sys_call       sys_call_table[];   //ϵͳ����

//kernel.c
//�����ں��е�GDT(kernel.c)
extern ut_8		        gdt_ptr[];	        // 0~15:Limit  16~47:Base
extern DESCRIPTOR	    gdt[];
extern TSS		        tss;                //����tss

//interrupt.c
//�����ں��е�IDT(interrupt.c)
extern ut_8		        idt_ptr[];	        // 0~15:Limit  16~47:Base
extern GATE	            idt[];

//process.c
//������ر���
extern PROCESS*	        p_proc_ready;
extern PROCESS	        proc_table[];
extern t_8	            task_stack[];
extern TASK	            task_table[];

//tty.c
extern TTY		        tty_table[];        //�ն˱�

//console.c
extern t_32		        nr_current_console;
extern CONSOLE		    console_table[];

//keymap.c
extern t_32             keymap[];

#endif /* _MOS_INTERRUPT_H_ */
