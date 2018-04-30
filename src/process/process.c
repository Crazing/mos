/*
 * File: /src/process/process.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 10:59:15 am
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 30th April 2018 6:17:10 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ���̴����������
 */


#include "process.h"

static void TestA();
static void TestB();
static void TestC();

PROCESS*	    p_proc_ready = NULL;
PROCESS	        proc_table[NR_TASKS] = {0};

static t_8	    task_stack[STACK_SIZE_TOTAL] = {0};

static TASK	    task_table[NR_TASKS] = {{task_tty, STACK_SIZE_TTY, "tty"},
					                    {TestA, STACK_SIZE_TESTA, "TestA"},
					                    {TestB, STACK_SIZE_TESTB, "TestB"},
					                    {TestC, STACK_SIZE_TESTC, "TestC"}};

//����A
static void TestA()
{
	while(1){
        //disp_int(disp_pos);
		milli_delay(3000);
	}
}

//����B
static void TestB()
{
	t_32 i = 0;
	while(1){
		milli_delay(10);
	}
}

//����C
static void TestC()
{
	t_32 i = 0;
	while(1){
		milli_delay(10);
	}
}

//��ʼ������
void init_proc()
{
    TASK*		p_task		= task_table;
	PROCESS*	p_proc		= proc_table;
	t_8*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	t_16		selector_ldt	= SELECTOR_LDT_FIRST;
	t_32 i;
	for(i=0;i<NR_TASKS;i++){
		strcpy(p_proc->name, p_task->name);	// name of the process
		p_proc->pid	= i;			// pid

		p_proc->ldt_sel	= selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;	// change the DPL
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;	// change the DPL
		p_proc->regs.cs		= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ds		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.es		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.fs		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ss		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.gs		= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;
		p_proc->regs.eip	= (t_32)p_task->initial_eip;
		p_proc->regs.esp	= (t_32)p_task_stack;
		p_proc->regs.eflags	= 0x1202;	// IF=1, IOPL=1, bit 2 is always 1.

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}
	proc_table[0].ticks = proc_table[0].priority = 15;
	proc_table[1].ticks = proc_table[1].priority =  5;
	proc_table[2].ticks = proc_table[2].priority =  5;
	proc_table[3].ticks = proc_table[3].priority =  5;

	p_proc_ready	= proc_table;
    
}

//���̵��Ⱥ���
void schedule()
{
	PROCESS*	p;
	t_32		greatest_ticks = 0;

	p_proc_ready->ticks--;
    if (p_proc_ready->ticks > 0) {
	    return;
	}
    
	while (greatest_ticks == 0) {
		for (p=proc_table; p<proc_table+NR_TASKS; p++) {
			if (p->ticks > greatest_ticks) {
				greatest_ticks = p->ticks;
				p_proc_ready = p;
			}
		}

		if (greatest_ticks == 0) {
			for (p=proc_table; p<proc_table+NR_TASKS; p++) {
				p->ticks = p->priority;
			}
		}
	}
}