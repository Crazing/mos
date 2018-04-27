;
;File: /include/interrupt/interrupt.inc
;Project: MOS
;File Created: Monday, 23rd April 2018 12:30:07 am
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Monday, 23rd April 2018 9:39:40 pm
;Modified By: zhixiang
;-----
;FileContent: 中断处理程序主体
;


; 导入函数
extern	exception_handler
extern	spurious_irq
extern	disp_str

; 导入全局变量
extern	gdt_ptr
extern	idt_ptr
extern	p_proc_ready
extern	tss
extern	irq_table
extern	sys_call_table
extern  StackTop

;进程控制块头部
P_STACKBASE	    equ	0
GSREG		    equ	P_STACKBASE
FSREG		    equ	GSREG		+ 4
ESREG		    equ	FSREG		+ 4
DSREG		    equ	ESREG		+ 4
EDIREG		    equ	DSREG		+ 4
ESIREG		    equ	EDIREG		+ 4
EBPREG		    equ	ESIREG		+ 4
KERNELESPREG	equ	EBPREG		+ 4
EBXREG		    equ	KERNELESPREG	+ 4
EDXREG		    equ	EBXREG		+ 4
ECXREG		    equ	EDXREG		+ 4
EAXREG		    equ	ECXREG		+ 4
RETADR		    equ	EAXREG		+ 4
EIPREG		    equ	RETADR		+ 4
CSREG		    equ	EIPREG		+ 4
EFLAGSREG	    equ	CSREG		+ 4
ESPREG		    equ	EFLAGSREG	+ 4
SSREG		    equ	ESPREG		+ 4
P_STACKTOP	    equ	SSREG		+ 4
P_LDT_SEL	    equ	P_STACKTOP
P_LDT		    equ	P_LDT_SEL	+ 4

TSS3_S_SP0	    equ	4

INT_M_CTL	    equ	0x20	; I/O port for interrupt controller         <Master>
INT_M_CTLMASK	equ	0x21	; setting bits in this port disables ints   <Master>
INT_S_CTL	    equ	0xA0	; I/O port for second interrupt controller  <Slave>
INT_S_CTLMASK	equ	0xA1	; setting bits in this port disables ints   <Slave>

EOI		        equ	0x20