/*
 * File: /src/interrupt/interrupt.c
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 11:09:04 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 30th April 2018 6:10:10 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 中断处理程序主体
 */


#include "interrupt.h"

//定义内核中的IDT
ut_8		idt_ptr[6] = {0};	    // 0~15:Limit  16~47:Base
GATE	    idt[IDT_SIZE] = {0};

//初始化 386 中断门
static void init_idt_desc(GATE* p_gate, ut_8 desc_type, t_pf_int_handler handler, ut_8 privilege)
{
	ut_32	base	    = (ut_32)handler;
	p_gate->offset_low	= base & 0xFFFF;
	p_gate->selector	= SELECTOR_KERNEL_CS;
	p_gate->dcount		= 0;
	p_gate->attr		= desc_type | (privilege << 5);
	p_gate->offset_high	= (base >> 16) & 0xFFFF;
}

//静态函数用于初始化idt
static void init_idt()
{
    // 全部初始化成中断门(没有陷阱门)
	init_idt_desc(&idt[INT_VECTOR_DIVIDE],	        DA_386IGate, divide_error,		    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_DEBUG],		    DA_386IGate, single_step_exception,	PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_NMI],		        DA_386IGate, nmi,			        PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_BREAKPOINT],	    DA_386IGate, breakpoint_exception,	PRIVILEGE_USER);
	init_idt_desc(&idt[INT_VECTOR_OVERFLOW],	    DA_386IGate, overflow,			    PRIVILEGE_USER);
	init_idt_desc(&idt[INT_VECTOR_BOUNDS],	        DA_386IGate, bounds_check,		    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_INVAL_OP],	    DA_386IGate, inval_opcode,		    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_COPROC_NOT],	    DA_386IGate, copr_not_available,	PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_DOUBLE_FAULT],	DA_386IGate, double_fault,		    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_COPROC_SEG],	    DA_386IGate, copr_seg_overrun,		PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_INVAL_TSS],	    DA_386IGate, inval_tss,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_SEG_NOT],	        DA_386IGate, segment_not_present,	PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_STACK_FAULT], 	DA_386IGate, stack_exception,		PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_PROTECTION],	    DA_386IGate, general_protection,	PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_PAGE_FAULT],	    DA_386IGate, page_fault,		    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_COPROC_ERR],	    DA_386IGate, copr_error,		    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ0 + 0],	    DA_386IGate, hwint00,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ0 + 1],	    DA_386IGate, hwint01,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ0 + 2],	    DA_386IGate, hwint02,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ0 + 3],	    DA_386IGate, hwint03,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ0 + 4],	    DA_386IGate, hwint04,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ0 + 5],	    DA_386IGate, hwint05,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ0 + 6],	    DA_386IGate, hwint06,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ0 + 7],	    DA_386IGate, hwint07,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ8 + 0],	    DA_386IGate, hwint08,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ8 + 1],	    DA_386IGate, hwint09,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ8 + 2],	    DA_386IGate, hwint10,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ8 + 3],	    DA_386IGate, hwint11,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ8 + 4],	    DA_386IGate, hwint12,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ8 + 5],	    DA_386IGate, hwint13,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ8 + 6],	    DA_386IGate, hwint14,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_IRQ8 + 7],	    DA_386IGate, hwint15,			    PRIVILEGE_KRNL);
	init_idt_desc(&idt[INT_VECTOR_SYS_CALL],	    DA_386IGate, sys_call,			    PRIVILEGE_USER);

    // idt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sidt 以及 lidt 的参数。
	t_16* p_idt_limit = (ut_16*)(&idt_ptr[0]);
	t_32* p_idt_base  = (ut_32*)(&idt_ptr[2]);
	*p_idt_limit = IDT_SIZE * sizeof(GATE) - 1;
	*p_idt_base  = (ut_32)&idt;
}

//异常处理
void exception_handler(t_32 vec_no, ut_32 err_code, ut_32 eip, ut_32 cs, ut_32 eflags)
{
	ut_32 i;
	t_32 text_color = 0x74; /* 灰底红字 */
	t_8  err_description[][64] = {	"#DE Divide Error",
					                "#DB RESERVED",
					                "—  NMI Interrupt",
					                "#BP Breakpoint",
					                "#OF Overflow",
					                "#BR BOUND Range Exceeded",
					                "#UD Invalid Opcode (Undefined Opcode)",
					                "#NM Device Not Available (No Math Coprocessor)",
					                "#DF Double Fault",
					                "    Coprocessor Segment Overrun (reserved)",
					                "#TS Invalid TSS",
					                "#NP Segment Not Present",
					                "#SS Stack-Segment Fault",
					                "#GP General Protection",
					                "#PF Page Fault",
					                "—  (Intel reserved. Do not use.)",
					                "#MF x87 FPU Floating-Point Error (Math Fault)",
					                "#AC Alignment Check",
					                "#MC Machine Check",
					                "#XF SIMD Floating-Point Exception"
				                };

	/* 通过打印空格的方式清空屏幕的前五行，并把 disp_pos 清零 */
	disp_pos = 0;
	for(i=0;i<80*5;i++){
		disp_str(" ");
	}
	disp_pos = 0;

	disp_color_str("Exception! --> ", text_color);
	disp_color_str(err_description[vec_no], text_color);
	disp_color_str("\n\n", text_color);
	disp_color_str("EFLAGS:", text_color);
	disp_int(eflags);
	disp_color_str("CS:", text_color);
	disp_int(cs);
	disp_color_str("EIP:", text_color);
	disp_int(eip);

	if(err_code != 0xFFFFFFFF){
		disp_color_str("Error code:", text_color);
		disp_int(err_code);
	}
}

//初始化中断处理
void init_interrupt()
{
    init_8259A();
    init_idt();
}

//打开中断功能
void open_interrupt()
{
    //设置8259A
    set_8259A();
    
    //打开8259A
    open_8259A();
}
