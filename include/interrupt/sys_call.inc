;
;File: /include/interrupt/sys_call.inc
;Project: MOS
;File Created: Monday, 23rd April 2018 10:13:24 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Monday, 23rd April 2018 10:15:54 pm
;Modified By: zhixiang
;-----
;FileContent: ϵͳ����
;


_NR_get_ticks		    equ	0	    ; Ҫ�� global.c �� sys_call_table �Ķ������Ӧ��
_NR_write               equ 1
INT_VECTOR_SYS_CALL	    equ	0x90