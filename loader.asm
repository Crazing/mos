
org	0100h

	mov	ax, 0B800h
	mov	gs, ax
	mov	ah, 0Fh				; 0000: �ڵ�    1111: ����
	mov	al, 'L'
	mov	[gs:((80 * 0 + 39) * 2)], ax	; ��Ļ�� 0 ��, �� 39 �С�

	jmp	$		; Start
