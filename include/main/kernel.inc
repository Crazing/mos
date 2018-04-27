;
;File: /include/main/kernel.inc
;Project: MOS
;File Created: Sunday, 22nd April 2018 1:05:13 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Tuesday, 24th April 2018 10:56:43 pm
;Modified By: zhixiang
;-----
;FileContent: 内核入口文件
;


; 定义堆栈大小
KERNELSTACKSIZE	    equ	    2*1024
; 定义tss在gdt中的位置
SELECTOR_TSS		equ		0x20		; TSS. 从外层跳到内存时 SS 和 ESP 的值从里面获得.

; 导入函数
extern	init_kernel
extern  mos_main