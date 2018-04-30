;
;File: /src/main/kernel.asm
;Project: MOS
;File Created: Sunday, 22nd April 2018 1:02:11 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Tuesday, 24th April 2018 10:56:15 pm
;Modified By: zhixiang
;-----
;FileContent: �ں�����ļ�
;


%include "kernel.inc"

;��������
global disp_pos
global StackTop

;��������
global _start	; ���� _start
global load_gdt ; ����load_gdt
global store_gdt; ����store_gdt
global load_idt ; ����load_idt
global store_idt; ����store_idt
global load_tss ; ����load_tss

bits 32
[SECTION .bss]
StackSpace		resb	KERNELSTACKSIZE
StackTop:		; ջ��

[SECTION .data]
disp_pos		dd  0

[section .text]	; �����ڴ�

_start:
	; ��ʱ�ڴ濴��ȥ�������ģ�����ϸ���ڴ������ LOADER.ASM ����˵������
	;              ��                                    ��
	;              ��                 ...                ��
	;              �ǩ�������������������������������������
	;              ��������������Page  Tables��������������
	;              ������������(��С��LOADER����)���������� PageTblBase
	;    00101000h �ǩ�������������������������������������
	;              ����������Page Directory Table���������� PageDirBase = 1M
	;    00100000h �ǩ�������������������������������������
	;              ���������� Hardware  Reserved ���������� B8000h �� gs
	;       9FC00h �ǩ�������������������������������������
	;              ����������������LOADER.BIN�������������� somewhere in LOADER �� esp
	;       90000h �ǩ�������������������������������������
	;              ����������������KERNEL.BIN��������������
	;       80000h �ǩ�������������������������������������
	;              ������������������KERNEL���������������� 30400h �� KERNEL ��� (KernelEntryPointPhyAddr)
	;       30000h �ǩ�������������������������������������
	;              ��                 ...                ��
	;              ��                                    ��
	;           0h ���������������������������������������� �� cs, ds, es, fs, ss
	;
	;
	; GDT �Լ���Ӧ���������������ģ�
	;
	;		              Descriptors               Selectors
	;              ����������������������������������������
	;              ��         Dummy Descriptor           ��
	;              �ǩ�������������������������������������
	;              ��         DESC_FLAT_C    (0��4G)     ��   8h = cs
	;              �ǩ�������������������������������������
	;              ��         DESC_FLAT_RW   (0��4G)     ��  10h = ds, es, fs, ss
	;              �ǩ�������������������������������������
	;              ��         DESC_VIDEO                 ��  1Bh = gs
	;              ����������������������������������������
	;
	; ע��! ��ʹ�� C �����ʱ��һ��Ҫ��֤ ds, es, ss �⼸���μĴ�����ֵ��һ����
	; ��Ϊ�������п��ܱ����ʹ�����ǵĴ���, ��������Ĭ��������һ����. ���紮�����������õ� ds �� es.
	;
	;


    ; �ں˴����￪ʼ����
    ; ��ʼ��bss�Σ�ȫ����0
    mov  eax, 0
    mov  edi, __bss_start
clear_bss:
    stosd
    cmp  edi, _end
    jnz  clear_bss

	; �� esp �� LOADER Ų�� KERNEL
	mov	 esp, StackTop	  ; ��ջ�� bss ����
	call      init_kernel ; ��ʼ���ں�
    call      mos_main    ; �ں�����

;void load_gdt(ut_8 *gdt_ptr);
load_gdt:
	push	ebp
	mov	ebp, esp

    push esi
    mov  esi, [ebp + 8] ; gdt
    lgdt [esi]
    pop  esi

	pop	 ebp
	ret

;void store_gdt(ut_8 *gdt_ptr);
store_gdt:
	push	ebp
	mov	ebp, esp

    push esi
    mov  esi, [ebp + 8] ; gdt
    sgdt [esi]
    pop  esi

	pop	 ebp
	ret

;void load_idt(ut_8 *idt_ptr);
load_idt:
	push	ebp
	mov	ebp, esp

    push esi
    mov  esi, [ebp + 8] ; gdt
    lidt [esi]
    pop  esi

	pop	 ebp
	ret

;void store_idt(ut_8 *idt_ptr);
store_idt:
	push	ebp
	mov	ebp, esp

    push esi
    mov  esi, [ebp + 8] ; gdt
    sidt [esi]
    pop  esi

	pop	 ebp
	ret

;void load_tss();
load_tss:
    push	ebp
	mov	 ebp, esp

    push eax
    xor	 eax, eax
	mov	 ax, SELECTOR_TSS
	ltr	 ax
    pop  eax

    pop ebp
    ret