;
;File: /include/main/kernel.inc
;Project: MOS
;File Created: Sunday, 22nd April 2018 1:05:13 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Tuesday, 24th April 2018 10:56:43 pm
;Modified By: zhixiang
;-----
;FileContent: �ں�����ļ�
;


; �����ջ��С
KERNELSTACKSIZE	    equ	    2*1024
; ����tss��gdt�е�λ��
SELECTOR_TSS		equ		0x20		; TSS. ����������ڴ�ʱ SS �� ESP ��ֵ��������.

; ���뺯��
extern	init_kernel
extern  mos_main

;�������
;ldĬ�����ӽű��ж����bss�ο�ʼλ��,ld --verbose
extern  __bss_start
 
;ldĬ�����ӽű��ж���Ķν���λ��,����bss�������һ����(��Ҫ�����ӣ�������),��������bss����λ��
extern  _end