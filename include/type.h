/*
 * File: /include/type.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 1:41:26 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 10:11:59 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 全局基本类型
 */


#ifndef	_MOS_TYPE_H_
#define	_MOS_TYPE_H_

#include "const.h"

//基本类型
typedef	unsigned int		ut_32;
typedef	unsigned short		ut_16;
typedef	unsigned char		ut_8;
typedef          int		t_32;
typedef          short	    t_16;
typedef          char		t_8;
typedef	         int	    t_bool;

//端口号类型
typedef	unsigned int		t_port;

//可变参数中使用
typedef char*			    va_list;

//门描述符中的处理函数
typedef	void	(*t_pf_int_handler)	();
//中断处理表中的函数类型
typedef	void	(*t_pf_irq_handler)	(int irq);
//系统调用表中的函数类型
typedef	void*	t_sys_call;

//进程体类型               
typedef	void	(*t_pf_task)		();

//门描述符 
typedef struct s_gate
{
	ut_16	offset_low; 	    /* Offset Low */
	ut_16	selector;	        /* Selector */
	ut_8	dcount;		        /* 该字段只在调用门描述符中有效。
				                如果在利用调用门调用子程序时引起特权级的转换和堆栈的改变，需要将外层堆栈中的参数复制到内层堆栈。
				                该双字计数字段就是用于说明这种情况发生时，要复制的双字参数的数量。 */
	ut_8	attr;		        /* P(1) DPL(2) DT(1) TYPE(4) */
	ut_16	offset_high;	    /* Offset High */
}GATE;

/* 存储段描述符/系统段描述符 */
typedef struct s_descriptor		/* 共 8 个字节 */
{
	ut_16	limit_low;		    /* Limit */
	ut_16	base_low;		    /* Base */
	ut_8	base_mid;		    /* Base */
	ut_8	attr1;			    /* P(1) DPL(2) DT(1) TYPE(4) */
	ut_8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	ut_8	base_high;		    /* Base */
}DESCRIPTOR;

//任务状态描述符
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
	ut_16	iobase;	/* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
	/*ut_8	iomap[2];*/
}TSS;

//PCE头部，而且是SP0的调用栈
typedef struct s_stackframe {	/* proc_ptr points here		↑ Low		*/
	ut_32	gs;		            /* ┓						│			*/
	ut_32	fs;		            /* ┃						│			*/
	ut_32	es;		            /* ┃						│			*/
	ut_32	ds;		            /* ┃						│			*/
	ut_32	edi;		        /* ┃						│			*/
	ut_32	esi;		        /* ┣ pushed by save()		│			*/
	ut_32	ebp;		        /* ┃						│			*/
	ut_32	kernel_esp;	        /* <- 'popad' will ignore it			│			*/
	ut_32	ebx;		        /* ┃						↑栈从高地址往低地址增长*/		
	ut_32	edx;		        /* ┃						│			*/
	ut_32	ecx;		        /* ┃						│			*/
	ut_32	eax;		        /* ┛						│			*/
	ut_32	retaddr;	        /* return address for assembly code save()	    │	*/
	ut_32	eip;		        /*  ┓						│			*/
	ut_32	cs;		            /*  ┃						│			*/
	ut_32	eflags;		        /*  ┣ these are pushed by CPU during interrupt	│	*/
	ut_32	esp;		        /*  ┃						│			*/
	ut_32	ss;		            /*  ┛						┷High		*/
}STACK_FRAME;

//进程控制结构
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

//任务结构体
typedef struct s_task {
	t_pf_task	initial_eip;
	t_32		stacksize;
	t_8		    name[32];
}TASK;

/* CONSOLE */
typedef struct s_console
{
	ut_32	current_start_addr;	/* 当前显示到了什么位置   */
	ut_32	original_addr;		/* 当前控制台对应显存位置 */
	ut_32	v_mem_limit;		/* 当前控制台占的显存大小 */
	ut_32	cursor;			    /* 当前光标位置 */
}CONSOLE;

/* TTY */
typedef struct s_tty
{
	ut_32	    in_buf[TTY_IN_BYTES];	/* TTY 输入缓冲区 */
	ut_32*	    p_inbuf_head;		    /* 指向缓冲区中下一个空闲位置 */
	ut_32*	    p_inbuf_tail;		    /* 指向键盘任务应处理的键值 */
	ut_32	    inbuf_count;		    /* 缓冲区中已经填充了多少 */

	CONSOLE*    p_console;
}TTY;

/* Keyboard structure, 1 per console. */
typedef struct s_kb {
	ut_8*	    p_head;			        /* 指向缓冲区中下一个空闲位置 */
	ut_8*	    p_tail;			        /* 指向键盘任务应处理的字节 */
	ut_32	    count;			        /* 缓冲区中共有多少字节 */
	ut_8	    buf[KB_IN_BYTES];       /* 缓冲区 */
}KB_INPUT;

#endif /* _MOS_TYPE_H_ */
