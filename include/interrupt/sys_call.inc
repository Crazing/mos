;
;File: /include/interrupt/sys_call.inc
;Project: MOS
;File Created: Monday, 23rd April 2018 10:13:24 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Monday, 23rd April 2018 10:15:54 pm
;Modified By: zhixiang
;-----
;FileContent: 系统调用
;


_NR_get_ticks		    equ	0	    ; 要跟 global.c 中 sys_call_table 的定义相对应！
INT_VECTOR_SYS_CALL	    equ	0x90