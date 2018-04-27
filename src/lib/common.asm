;
;File: /src/lib/common.asm
;Project: MOS
;File Created: Sunday, 22nd April 2018 4:08:11 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Wednesday, 25th April 2018 8:36:50 pm
;Modified By: zhixiang
;-----
;FileContent: 公共基础库
;


%include "common.inc"

[SECTION .text]

; 导出函数
global	disp_str
global	disp_color_str
global	out_byte
global	in_byte
global	disable_irq
global	enable_irq
global  disable_int;
global  enable_int;

;函数声明：void disp_str(char * info);
disp_str:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [disp_pos]
	mov	ah, 0Fh
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; 是回车吗?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos], edi

	pop	ebp
	ret

;函数声明：void disp_color_str(char * info, int color);
disp_color_str:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [disp_pos]
	mov	ah, [ebp + 12]	; color
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; 是回车吗?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos], edi

	pop	ebp
	ret


; void out_byte(t_port port, t_8 value);
out_byte:
    push	ebp
	mov	 ebp, esp

    push edx
	mov	 edx, [esp + 12]		; port
	mov  al, [esp + 12 + 4]	; value
	out	 dx, al
    pop  edx

	nop	; 一点延迟
	nop
    pop	 ebp
	ret


; t_8 in_byte(t_port port);
in_byte:
    push	ebp
	mov	 ebp, esp

    push edx
	mov	 edx, [esp + 12]		; port
	xor	 eax, eax
	in	 al, dx
    pop  edx

	nop	; 一点延迟
	nop
    pop  ebp
	ret

; ========================================================================
;                  t_32 disable_irq(int irq);
; ========================================================================
; Disable an interrupt request line by setting an 8259 bit.
; Equivalent code for irq < 8:
;       out_byte(INT_CTLMASK, in_byte(INT_CTLMASK) | (1 << irq));
; Returns true iff the interrupt was not already disabled.
;
disable_irq:
    push	ebp
	mov	 ebp, esp

    push ecx
	mov	 ecx, [esp + 12]		; irq
	pushf
	cli
	mov	ah, 1
	rol	ah, cl			; ah = (1 << (irq % 8))
	cmp	cl, 8
	jae	disable_8		; disable irq >= 8 at the slave 8259
disable_0:
	in	al, INT_M_CTLMASK
	test	al, ah
	jnz	dis_already		; already disabled?
	or	al, ah
	out	INT_M_CTLMASK, al	; set bit at master 8259
	popf
	mov	eax, 1			; disabled by this function
    pop ecx
    pop ebp
	ret
disable_8:
	in	al, INT_S_CTLMASK
	test	al, ah
	jnz	dis_already		; already disabled?
	or	al, ah
	out	INT_S_CTLMASK, al	; set bit at slave 8259
	popf
	mov	eax, 1			; disabled by this function
    pop ecx
    pop ebp
	ret
dis_already:
	popf
	xor	eax, eax		; already disabled
    pop ecx
    pop ebp
	ret

; ========================================================================
;                  void enable_irq(int irq);
; ========================================================================
; Enable an interrupt request line by clearing an 8259 bit.
; Equivalent code:
;	if(irq < 8){
;		out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) & ~(1 << irq));
;	}
;	else{
;		out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) & ~(1 << irq));
;	}
;
enable_irq:
        push	ebp
	    mov	 ebp, esp

        push ecx
        mov	ecx, [esp + 12]		; irq
        pushf
        cli
        mov	ah, ~1
        rol	ah, cl			; ah = ~(1 << (irq % 8))
        cmp	cl, 8
        jae	enable_8		; enable irq >= 8 at the slave 8259
enable_0:
        in	al, INT_M_CTLMASK
        and	al, ah
        out	INT_M_CTLMASK, al	; clear bit at master 8259
        popf
        pop ecx
        pop ebp
        ret
enable_8:
        in	al, INT_S_CTLMASK
        and	al, ah
        out	INT_S_CTLMASK, al	; clear bit at slave 8259
        popf
        pop ecx
        pop ebp
        ret


;void disable_int();
disable_int:
	cli
	ret

;void enable_int();
enable_int:
	sti
	ret