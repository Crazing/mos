/*
 * File: /include/type.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 1:41:26 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 10:11:59 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ȫ�ֻ�������
 */


#ifndef	_MOS_TYPE_H_
#define	_MOS_TYPE_H_

#include "const.h"

//��������
typedef	unsigned int		ut_32;
typedef	unsigned short		ut_16;
typedef	unsigned char		ut_8;
typedef          int		t_32;
typedef          short	    t_16;
typedef          char		t_8;
typedef	         int	    t_bool;

//�˿ں�����
typedef	unsigned int		t_port;

//�ɱ������ʹ��
typedef char*			    va_list;

//���������еĴ�����
typedef	void	(*t_pf_int_handler)	();
//�жϴ�����еĺ�������
typedef	void	(*t_pf_irq_handler)	(int irq);
//ϵͳ���ñ��еĺ�������
typedef	void*	t_sys_call;

//����������               
typedef	void	(*t_pf_task)		();

//�������� 
typedef struct s_gate
{
	ut_16	offset_low; 	    /* Offset Low */
	ut_16	selector;	        /* Selector */
	ut_8	dcount;		        /* ���ֶ�ֻ�ڵ���������������Ч��
				                ��������õ����ŵ����ӳ���ʱ������Ȩ����ת���Ͷ�ջ�ĸı䣬��Ҫ������ջ�еĲ������Ƶ��ڲ��ջ��
				                ��˫�ּ����ֶξ�������˵�������������ʱ��Ҫ���Ƶ�˫�ֲ����������� */
	ut_8	attr;		        /* P(1) DPL(2) DT(1) TYPE(4) */
	ut_16	offset_high;	    /* Offset High */
}GATE;

/* �洢��������/ϵͳ�������� */
typedef struct s_descriptor		/* �� 8 ���ֽ� */
{
	ut_16	limit_low;		    /* Limit */
	ut_16	base_low;		    /* Base */
	ut_8	base_mid;		    /* Base */
	ut_8	attr1;			    /* P(1) DPL(2) DT(1) TYPE(4) */
	ut_8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	ut_8	base_high;		    /* Base */
}DESCRIPTOR;

//����״̬������
typedef struct s_tss {
	ut_32	backlink;
	ut_32	esp0;		/* stack pointer to use during interrupt */
	ut_32	ss0;		/*   "   segment  "  "    "        "     */
	ut_32	esp1;
	ut_32	ss1;
	ut_32	esp2;
	ut_32	ss2;
	ut_32	cr3;
	ut_32	eip;
	ut_32	flags;
	ut_32	eax;
	ut_32	ecx;
	ut_32	edx;
	ut_32	ebx;
	ut_32	esp;
	ut_32	ebp;
	ut_32	esi;
	ut_32	edi;
	ut_32	es;
	ut_32	cs;
	ut_32	ss;
	ut_32	ds;
	ut_32	fs;
	ut_32	gs;
	ut_32	ldt;
	ut_16	trap;
	ut_16	iobase;	/* I/Oλͼ��ַ���ڻ����TSS�ν��ޣ��ͱ�ʾû��I/O���λͼ */
	/*ut_8	iomap[2];*/
}TSS;

//PCEͷ����������SP0�ĵ���ջ
typedef struct s_stackframe {	/* proc_ptr points here		�� Low		*/
	ut_32	gs;		            /* ��						��			*/
	ut_32	fs;		            /* ��						��			*/
	ut_32	es;		            /* ��						��			*/
	ut_32	ds;		            /* ��						��			*/
	ut_32	edi;		        /* ��						��			*/
	ut_32	esi;		        /* �� pushed by save()		��			*/
	ut_32	ebp;		        /* ��						��			*/
	ut_32	kernel_esp;	        /* <- 'popad' will ignore it			��			*/
	ut_32	ebx;		        /* ��						��ջ�Ӹߵ�ַ���͵�ַ����*/		
	ut_32	edx;		        /* ��						��			*/
	ut_32	ecx;		        /* ��						��			*/
	ut_32	eax;		        /* ��						��			*/
	ut_32	retaddr;	        /* return address for assembly code save()	    ��	*/
	ut_32	eip;		        /*  ��						��			*/
	ut_32	cs;		            /*  ��						��			*/
	ut_32	eflags;		        /*  �� these are pushed by CPU during interrupt	��	*/
	ut_32	esp;		        /*  ��						��			*/
	ut_32	ss;		            /*  ��						��High		*/
}STACK_FRAME;

//���̿��ƽṹ
typedef struct s_proc {
	STACK_FRAME			regs;			    /* process' registers saved in stack frame */

	ut_16				ldt_sel;		    /* selector in gdt giving ldt base and limit*/
	DESCRIPTOR			ldts[LDT_SIZE];		/* local descriptors for code and data */
	t_32				ticks;			    /* remained ticks */
	t_32				priority;
	ut_32				pid;			    /* process id passed in from MM */
	t_8				    name[16];		    /* name of the process */
    t_32				nr_tty;
}PROCESS;

//����ṹ��
typedef struct s_task {
	t_pf_task	initial_eip;
	t_32		stacksize;
	t_8		    name[32];
}TASK;

/* CONSOLE */
typedef struct s_console
{
	ut_32	current_start_addr;	/* ��ǰ��ʾ����ʲôλ��   */
	ut_32	original_addr;		/* ��ǰ����̨��Ӧ�Դ�λ�� */
	ut_32	v_mem_limit;		/* ��ǰ����̨ռ���Դ��С */
	ut_32	cursor;			    /* ��ǰ���λ�� */
}CONSOLE;

/* TTY */
typedef struct s_tty
{
	ut_32	    in_buf[TTY_IN_BYTES];	/* TTY ���뻺���� */
	ut_32*	    p_inbuf_head;		    /* ָ�򻺳�������һ������λ�� */
	ut_32*	    p_inbuf_tail;		    /* ָ���������Ӧ����ļ�ֵ */
	ut_32	    inbuf_count;		    /* ���������Ѿ�����˶��� */

	CONSOLE*    p_console;
}TTY;

/* Keyboard structure, 1 per console. */
typedef struct s_kb {
	ut_8*	    p_head;			        /* ָ�򻺳�������һ������λ�� */
	ut_8*	    p_tail;			        /* ָ���������Ӧ������ֽ� */
	ut_32	    count;			        /* �������й��ж����ֽ� */
	ut_8	    buf[KB_IN_BYTES];       /* ������ */
}KB_INPUT;

#endif /* _MOS_TYPE_H_ */
