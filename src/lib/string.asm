;
;File: /src/lib/string.asm
;Project: MOS
;File Created: Wednesday, 25th April 2018 8:39:30 pm
;Author: zhixiang (1115267126@qq.com)
;-----
;Last Modified: Wednesday, 25th April 2018 8:40:17 pm
;Modified By: zhixiang
;-----
;FileContent: �ַ�����������
;


[SECTION .text]

; ��������
global	memcpy
global	memset
global	strcpy
global  strlen
global  strchr


; ------------------------------------------------------------------------
; void* memcpy(void* es:p_dst, void* ds:p_src, t_32 size);
; ------------------------------------------------------------------------
memcpy:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	esi, [ebp + 12]	; Source
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		; �жϼ�����
	jz	.2		; ������Ϊ��ʱ����

	mov	al, [ds:esi]		; ��
	inc	esi			        ; ��
					        ; �� ���ֽ��ƶ�
	mov	byte [es:edi], al	; ��
	inc	edi			        ; ��

	dec	ecx		; ��������һ
	jmp	.1		; ѭ��
.2:
	mov	eax, [ebp + 8]	; ����ֵ

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			; ��������������
; memcpy ����-------------------------------------------------------------


; ------------------------------------------------------------------------
; void memset(void* p_dst, t_8 ch, t_32 size);
; ------------------------------------------------------------------------
memset:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	edx, [ebp + 12]	; Char to be putted
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		; �жϼ�����
	jz	.2		    ; ������Ϊ��ʱ����

	mov	byte [edi], dl		; ��
	inc	edi			        ; ��

	dec	ecx		; ��������һ
	jmp	.1		; ѭ��
.2:

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			; ��������������
; memset ����-------------------------------------------------------------


; ------------------------------------------------------------------------
; t_8* strcpy(t_8* p_dst, t_8* p_src);
; ------------------------------------------------------------------------
strcpy:
	push	  ebp
	mov	 ebp, esp

    push esi
    push edi
	mov	 esi, [ebp + 12]	; Source
	mov	 edi, [ebp + 8]	; Destination

.1:
	mov	 al,  [esi]		; ��
	inc	 esi			; ��
					    ; �� ���ֽ��ƶ�
	mov	 byte [edi], al ; ��
	inc	 edi			; ��

	cmp	 al, 0		    ; �Ƿ����� '\0'
	jnz	 .1		        ; û�����ͼ���ѭ���������ͽ���

	mov	 eax, [ebp + 8]	; ����ֵ

    pop  edi
    pop  esi
	pop	 ebp
	ret			; ��������������
; strcpy ����-------------------------------------------------------------

; ------------------------------------------------------------------------
; t_32 strlen(const t_8* p_src);
; ------------------------------------------------------------------------
strlen:
	push	  ebp
	mov	 ebp, esp

    push esi 
    mov  esi, [ebp + 8]
.1:
    lodsb
    cmp  al,  0
    jnz  .1
    mov  eax, esi
    mov  esi, [ebp + 8]
    sub  eax, esi
    sub  eax, 1

    pop  esi
    ret
; strlen ����-------------------------------------------------------------
    
; ------------------------------------------------------------------------
; t_8*  strchr(const t_8* str, t_8 ch);
; ------------------------------------------------------------------------
strchr:
	push	  ebp
	mov	 ebp, esp

    push esi 
    push ebx
    mov  bl,  [ebp + 12]
    mov  esi, [ebp + 8]
.1:    
    lodsb
    cmp  al,  bl
    jz   .2
    cmp  al,  0
    jnz  .1
    mov  eax, 0
    pop  ebx
    pop  esi
    pop  ebp
    ret
.2:
    mov  eax, esi
    sub  eax, 1
    pop  ebx
    pop  esi
    pop  ebp
    ret
; strchr ����-------------------------------------------------------------

