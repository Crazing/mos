/*
 * File: /src/process/process.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 10:59:15 am
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 11:24:16 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 进程处理程序主体
 */


#include "process.h"

static void TestA();
static void TestB();
static void TestC();

PROCESS*	    p_proc_ready = NULL;
PROCESS	        proc_table[NR_TASKS + NR_PROCS] = {0};

static t_8	    task_stack[STACK_SIZE_TOTAL] = {0};
static TASK	    task_table[NR_TASKS]         = {{task_tty, STACK_SIZE_TTY, "tty"}};
static TASK	    user_proc_table[NR_PROCS]    = {{TestA, STACK_SIZE_TESTA, "TestA"},
						                        {TestB, STACK_SIZE_TESTB, "TestB"},
						                        {TestC, STACK_SIZE_TESTC, "TestC"}};

//进程A
static void TestA()
{
	while(1){
        printf("<Ticks:%x>", get_ticks());
		milli_delay(3000);
	}
}

//进程B
static void TestB()
{
	t_32 i = 0;
	while(1){
        printf("B");
		milli_delay(1000);
	}
}

//进程C
static void TestC()
{
	t_32 i = 0;
	while(1){
        printf("C");
		milli_delay(1000);
	}
}

//初始化进程
void init_proc()
{
    TASK*		p_task		= task_table;
	PROCESS*	p_proc		= proc_table;
	t_8*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	t_16		selector_ldt	= SELECTOR_LDT_FIRST;
	t_32    i;
	t_8		privilege;
	t_8		rpl;
	t_32	eflags;
	for(i=0;i<NR_TASKS+NR_PROCS;i++){
		if (i < NR_TASKS) {	                /* 任务 */
			p_task		= task_table + i;
			privilege	= PRIVILEGE_TASK;
			rpl		    = RPL_TASK;
			eflags		= 0x1202;	        /* IF=1, IOPL=1, bit 2 is always 1 */
		}
		else {			                    /* 用户进程 */
			p_task		= user_proc_table + (i - NR_TASKS);
			privilege	= PRIVILEGE_USER;
			rpl		    = RPL_USER;
			eflags		= 0x202;	        /* IF=1, bit 2 is always 1 */
		}

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid	= i;			        /* pid */

		p_proc->ldt_sel	= selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;	/* change the DPL */
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;/* change the DPL */
		p_proc->regs.cs		= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs		= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		p_proc->regs.eip	= (t_32)p_task->initial_eip;
		p_proc->regs.esp	= (t_32)p_task_stack;
		p_proc->regs.eflags	= eflags;

        p_proc->nr_tty		= 0;
		p_task_stack -= p_task->stacksize;
		p_proc++;
		selector_ldt += 1 << 3;
	}
	proc_table[0].ticks = proc_table[0].priority = 15;
	proc_table[1].ticks = proc_table[1].priority =  5;
	proc_table[2].ticks = proc_table[2].priority =  5;
	proc_table[3].ticks = proc_table[3].priority =  5;

    proc_table[1].nr_tty = 0;
	proc_table[2].nr_tty = 1;
	proc_table[3].nr_tty = 1;
    
	p_proc_ready	= proc_table;
    
}

//进程调度函数
void schedule()
{
	PROCESS*	p;
	t_32		greatest_ticks = 0;

	p_proc_ready->ticks--;
    if (p_proc_ready->ticks > 0) {
	    return;
	}
    
	while (greatest_ticks == 0) {
		for (p=proc_table; p<proc_table+NR_TASKS+NR_PROCS; p++) {
			if (p->ticks > greatest_ticks) {
				greatest_ticks = p->ticks;
				p_proc_ready = p;
			}
		}

		if (greatest_ticks == 0) {
			for (p=proc_table; p<proc_table+NR_TASKS+NR_PROCS; p++) {
				p->ticks = p->priority;
			}
		}
	}
}