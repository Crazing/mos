;
;File: /src/main/kernel.asm
;Project: MOS
;File Created: Sunday, 22nd April 2018 1:02:11 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Tuesday, 24th April 2018 10:56:15 pm
;Modified By: zhixiang
;-----
;FileContent: 内核入口文件
;


%include "kernel.inc"

;导出变量
global disp_pos
global StackTop

;导出函数
global _start	; 导出 _start
global load_gdt ; 导出load_gdt
global store_gdt; 导出store_gdt
global load_idt ; 导出load_idt
global store_idt; 导出store_idt
global load_tss ; 导出load_tss

bits 32
[SECTION .bss]
StackSpace		resb	KERNELSTACKSIZE
StackTop:		; 栈顶

[SECTION .data]
disp_pos		dd  0

[section .text]	; 代码在此

_start:
	; 此时内存看上去是这样的（更详细的内存情况在 LOADER.ASM 中有说明）：
	;              ┃                                    ┃
	;              ┃                 ...                ┃
	;              ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■■■Page  Tables■■■■■■┃
	;              ┃■■■■■(大小由LOADER决定)■■■■┃ PageTblBase
	;    00101000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■Page Directory Table■■■■┃ PageDirBase = 1M
	;    00100000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃□□□□ Hardware  Reserved □□□□┃ B8000h ← gs
	;       9FC00h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■■■■LOADER.BIN■■■■■■┃ somewhere in LOADER ← esp
	;       90000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■■■■KERNEL.BIN■■■■■■┃
	;       80000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■■■■■KERNEL■■■■■■■┃ 30400h ← KERNEL 入口 (KernelEntryPointPhyAddr)
	;       30000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┋                 ...                ┋
	;              ┋                                    ┋
	;           0h ┗━━━━━━━━━━━━━━━━━━┛ ← cs, ds, es, fs, ss
	;
	;
	; GDT 以及相应的描述符是这样的：
	;
	;		              Descriptors               Selectors
	;              ┏━━━━━━━━━━━━━━━━━━┓
	;              ┃         Dummy Descriptor           ┃
	;              ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃         DESC_FLAT_C    (0～4G)     ┃   8h = cs
	;              ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃         DESC_FLAT_RW   (0～4G)     ┃  10h = ds, es, fs, ss
	;              ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃         DESC_VIDEO                 ┃  1Bh = gs
	;              ┗━━━━━━━━━━━━━━━━━━┛
	;
	; 注意! 在使用 C 代码的时候一定要保证 ds, es, ss 这几个段寄存器的值是一样的
	; 因为编译器有可能编译出使用它们的代码, 而编译器默认它们是一样的. 比如串拷贝操作会用到 ds 和 es.
	;
	;


    ; 内核从这里开始运行
    ; 初始化bss段，全部清0
    mov  eax, 0
    mov  edi, __bss_start
clear_bss:
    stosd
    cmp  edi, _end
    jnz  clear_bss

	; 把 esp 从 LOADER 挪到 KERNEL
	mov	 esp, StackTop	  ; 堆栈在 bss 段中
	call      init_kernel ; 初始化内核
    call      mos_main    ; 内核运行

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