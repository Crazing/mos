;
;File: /src/interrupt/sys_call.asm
;Project: MOS
;File Created: Monday, 23rd April 2018 10:12:52 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Monday, 23rd April 2018 10:21:50 pm
;Modified By: zhixiang
;-----
;FileContent: 系统调用
;


%include "sys_call.inc"

; 导出符号
global	get_ticks
global  write


bits 32
[section .text]

;t_32   get_ticks();
get_ticks:
	mov	eax, _NR_get_ticks
	int	INT_VECTOR_SYS_CALL
	ret

; void write(t_8* buf, t_32 len);
write:
	mov	eax, _NR_write
	mov	ebx, [esp + 4]
	mov	ecx, [esp + 8]
	int	INT_VECTOR_SYS_CALL
	ret