
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               loader.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

org  0100h

	jmp	LABEL_START		; Start

; ������ FAT12 ���̵�ͷ, ֮���԰���������Ϊ�����õ��˴��̵�һЩ��Ϣ
%include	"fat12hdr.inc"
%include	"load.inc"
%include	"pm.inc"


; GDT ------------------------------------------------------------------------------------------------------------------------------------------------------------
;                                                �λ�ַ            �ν���     , ����
LABEL_GDT:			Descriptor             0,                    0, 0						; ��������
LABEL_DESC_FLAT_C:		Descriptor             0,              0fffffh, DA_CR  | DA_32 | DA_LIMIT_4K			; 0 ~ 4G
LABEL_DESC_FLAT_RW:		Descriptor             0,              0fffffh, DA_DRW | DA_32 | DA_LIMIT_4K			; 0 ~ 4G
LABEL_DESC_VIDEO:		Descriptor	 0B8000h,               0ffffh, DA_DRW                         | DA_DPL3	; �Դ��׵�ַ
; GDT ------------------------------------------------------------------------------------------------------------------------------------------------------------

GdtLen		equ	$ - LABEL_GDT
GdtPtr		dw	GdtLen - 1				; �ν���
		dd	BaseOfLoaderPhyAddr + LABEL_GDT		; ����ַ

; GDT ѡ���� ----------------------------------------------------------------------------------
SelectorFlatC		equ	LABEL_DESC_FLAT_C	- LABEL_GDT
SelectorFlatRW		equ	LABEL_DESC_FLAT_RW	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT + SA_RPL3
; GDT ѡ���� ----------------------------------------------------------------------------------


BaseOfStack	equ	0100h


LABEL_START:			; <--- �����￪ʼ *************
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack

	mov	dh, 0			; "Loading  "
	call	DispStrRealMode		; ��ʾ�ַ���

	; �õ��ڴ���
	mov	ebx, 0			; ebx = ����ֵ, ��ʼʱ��Ϊ 0
	mov	di, _MemChkBuf		; es:di ָ��һ����ַ��Χ�������ṹ��Address Range Descriptor Structure��
.MemChkLoop:
	mov	eax, 0E820h		; eax = 0000E820h
	mov	ecx, 20			; ecx = ��ַ��Χ�������ṹ�Ĵ�С
	mov	edx, 0534D4150h		; edx = 'SMAP'
	int	15h			; int 15h
	jc	.MemChkFail
	add	di, 20
	inc	dword [_dwMCRNumber]	; dwMCRNumber = ARDS �ĸ���
	cmp	ebx, 0
	jne	.MemChkLoop
	jmp	.MemChkOK
.MemChkFail:
	mov	dword [_dwMCRNumber], 0
.MemChkOK:

	; ������ A �̵ĸ�Ŀ¼Ѱ�� KERNEL.BIN
	mov	word [wSectorNo], SectorNoOfRootDirectory	
	xor	ah, ah	; ��
	xor	dl, dl	; �� ������λ
	int	13h	; ��
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
	cmp	word [wRootDirSizeForLoop], 0	; ��
	jz	LABEL_NO_KERNELBIN		; �� �жϸ�Ŀ¼���ǲ����Ѿ�����, ��������ʾû���ҵ� KERNEL.BIN
	dec	word [wRootDirSizeForLoop]	; ��
	mov	ax, BaseOfKernelFile
	mov	es, ax			; es <- BaseOfKernelFile
	mov	bx, OffsetOfKernelFile	; bx <- OffsetOfKernelFile	����, es:bx = BaseOfKernelFile:OffsetOfKernelFile = BaseOfKernelFile * 10h + OffsetOfKernelFile
	mov	ax, [wSectorNo]		; ax <- Root Directory �е�ĳ Sector ��
	mov	cl, 1
	call	ReadSector

	mov	si, KernelFileName	; ds:si -> "KERNEL  BIN"
	mov	di, OffsetOfKernelFile	; es:di -> BaseOfKernelFile:???? = BaseOfKernelFile*10h+????
	cld
	mov	dx, 10h
LABEL_SEARCH_FOR_KERNELBIN:
	cmp	dx, 0					; ��
	jz	LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR	; �� ѭ����������, ����Ѿ�������һ�� Sector, ��������һ�� Sector
	dec	dx					; ��
	mov	cx, 11
LABEL_CMP_FILENAME:
	cmp	cx, 0			; ��
	jz	LABEL_FILENAME_FOUND	; �� ѭ����������, ����Ƚ��� 11 ���ַ������, ��ʾ�ҵ�
	dec	cx			; ��
	lodsb				; ds:si -> al
	cmp	al, byte [es:di]	; if al == es:di
	jz	LABEL_GO_ON
	jmp	LABEL_DIFFERENT
LABEL_GO_ON:
	inc	di
	jmp	LABEL_CMP_FILENAME	;	����ѭ��

LABEL_DIFFERENT:
	and	di, 0FFE0h		; else��	��ʱdi��ֵ��֪����ʲô, di &= e0 Ϊ�������� 20h �ı���
	add	di, 20h			;     ��
	mov	si, KernelFileName	;     �� di += 20h  ��һ��Ŀ¼��Ŀ
	jmp	LABEL_SEARCH_FOR_KERNELBIN;   ��

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
	add	word [wSectorNo], 1
	jmp	LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_KERNELBIN:
	mov	dh, 2			; "No KERNEL."
	call	DispStrRealMode		; ��ʾ�ַ���
	jmp	$			; û���ҵ� KERNEL.BIN, ��ѭ��������

LABEL_FILENAME_FOUND:			; �ҵ� KERNEL.BIN ��������������
	mov	ax, RootDirSectors
	and	di, 0FFF0h		; di -> ��ǰ��Ŀ�Ŀ�ʼ

	push	eax
	mov	eax, [es : di + 01Ch]		; ��
	mov	dword [dwKernelSize], eax	; ������ KERNEL.BIN �ļ���С
	pop	eax

	add	di, 01Ah		; di -> �� Sector
	mov	cx, word [es:di]
	push	cx			; ����� Sector �� FAT �е����
	add	cx, ax
	add	cx, DeltaSectorNo	; ��ʱ cl ������ LOADER.BIN ����ʼ������ (�� 0 ��ʼ�������)
	mov	ax, BaseOfKernelFile
	mov	es, ax			; es <- BaseOfKernelFile
	mov	bx, OffsetOfKernelFile	; bx <- OffsetOfKernelFile	����, es:bx = BaseOfKernelFile:OffsetOfKernelFile = BaseOfKernelFile * 10h + OffsetOfKernelFile
	mov	ax, cx			; ax <- Sector ��

LABEL_GOON_LOADING_FILE:
	push	ax			; ��
	push	bx			; ��
	mov	ah, 0Eh			; �� ÿ��һ���������� "Loading  " �����һ����, �γ�������Ч��:
	mov	al, '.'			; ��
	mov	bl, 0Fh			; �� Loading ......
	int	10h			; ��
	pop	bx			; ��
	pop	ax			; ��

	mov	cl, 1
	call	ReadSector
	pop	ax			; ȡ���� Sector �� FAT �е����
	call	GetFATEntry
	cmp	ax, 0FFFh
	jz	LABEL_FILE_LOADED
	push	ax			; ���� Sector �� FAT �е����
	mov	dx, RootDirSectors
	add	ax, dx
	add	ax, DeltaSectorNo
	add	bx, [BPB_BytsPerSec]
	jmp	LABEL_GOON_LOADING_FILE
LABEL_FILE_LOADED:

	call	KillMotor		; �ر��������

	mov	dh, 1			; "Ready."
	call	DispStrRealMode		; ��ʾ�ַ���
	
; ����׼�����뱣��ģʽ -------------------------------------------

; ���� GDTR
	lgdt	[GdtPtr]

; ���ж�
	cli

; �򿪵�ַ��A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

; ׼���л�������ģʽ
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

; �������뱣��ģʽ
	jmp	dword SelectorFlatC:(BaseOfLoaderPhyAddr+LABEL_PM_START)


;============================================================================
;����
;----------------------------------------------------------------------------
wRootDirSizeForLoop	dw	RootDirSectors	; Root Directory ռ�õ�������
wSectorNo		dw	0		; Ҫ��ȡ��������
bOdd			db	0		; ��������ż��
dwKernelSize		dd	0		; KERNEL.BIN �ļ���С

;============================================================================
;�ַ���
;----------------------------------------------------------------------------
KernelFileName		db	"KERNEL  BIN", 0	; KERNEL.BIN ֮�ļ���
; Ϊ�򻯴���, ����ÿ���ַ����ĳ��Ⱦ�Ϊ MessageLength
MessageLength		equ	9
LoadMessage:		db	"Loading  "
Message1		db	"Ready.   "
Message2		db	"No KERNEL"
;============================================================================

;----------------------------------------------------------------------------
; ������: DispStrRealMode
;----------------------------------------------------------------------------
; ���л���:
;	ʵģʽ������ģʽ����ʾ�ַ����ɺ��� DispStr ��ɣ�
; ����:
;	��ʾһ���ַ���, ������ʼʱ dh ��Ӧ�����ַ������(0-based)
DispStrRealMode:
	mov	ax, MessageLength
	mul	dh
	add	ax, LoadMessage
	mov	bp, ax			; ��
	mov	ax, ds			; �� ES:BP = ����ַ
	mov	es, ax			; ��
	mov	cx, MessageLength	; CX = ������
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; ҳ��Ϊ0(BH = 0) �ڵװ���(BL = 07h)
	mov	dl, 0
	add	dh, 3			; �ӵ� 3 ��������ʾ
	int	10h			; int 10h
	ret
;----------------------------------------------------------------------------
; ������: ReadSector
;----------------------------------------------------------------------------
; ����:
;	�����(Directory Entry �е� Sector ��)Ϊ ax �ĵ� Sector ��ʼ, �� cl �� Sector ���� es:bx ��
ReadSector:
	; -----------------------------------------------------------------------
	; �������������������ڴ����е�λ�� (������ -> �����, ��ʼ����, ��ͷ��)
	; -----------------------------------------------------------------------
	; ��������Ϊ x
	;                           �� ����� = y >> 1
	;       x           �� �� y ��
	; -------------- => ��      �� ��ͷ�� = y & 1
	;  ÿ�ŵ�������     ��
	;                   �� �� z => ��ʼ������ = z + 1
	push	bp
	mov	bp, sp
	sub	esp, 2			; �ٳ������ֽڵĶ�ջ���򱣴�Ҫ����������: byte [bp-2]

	mov	byte [bp-2], cl
	push	bx			; ���� bx
	mov	bl, [BPB_SecPerTrk]	; bl: ����
	div	bl			; y �� al ��, z �� ah ��
	inc	ah			; z ++
	mov	cl, ah			; cl <- ��ʼ������
	mov	dh, al			; dh <- y
	shr	al, 1			; y >> 1 (��ʵ�� y/BPB_NumHeads, ����BPB_NumHeads=2)
	mov	ch, al			; ch <- �����
	and	dh, 1			; dh & 1 = ��ͷ��
	pop	bx			; �ָ� bx
	; ����, "�����, ��ʼ����, ��ͷ��" ȫ���õ� ^^^^^^^^^^^^^^^^^^^^^^^^
	mov	dl, [BS_DrvNum]		; �������� (0 ��ʾ A ��)
.GoOnReading:
	mov	ah, 2			; ��
	mov	al, byte [bp-2]		; �� al ������
	int	13h
	jc	.GoOnReading		; �����ȡ���� CF �ᱻ��Ϊ 1, ��ʱ�Ͳ�ͣ�ض�, ֱ����ȷΪֹ

	add	esp, 2
	pop	bp

	ret

;----------------------------------------------------------------------------
; ������: GetFATEntry
;----------------------------------------------------------------------------
; ����:
;	�ҵ����Ϊ ax �� Sector �� FAT �е���Ŀ, ������� ax ��
;	��Ҫע�����, �м���Ҫ�� FAT �������� es:bx ��, ���Ժ���һ��ʼ������ es �� bx
GetFATEntry:
	push	es
	push	bx
	push	ax
	mov	ax, BaseOfKernelFile	; ��
	sub	ax, 0100h		; �� �� BaseOfKernelFile �������� 4K �ռ����ڴ�� FAT
	mov	es, ax			; ��
	pop	ax
	mov	byte [bOdd], 0
	mov	bx, 3
	mul	bx			; dx:ax = ax * 3
	mov	bx, 2
	div	bx			; dx:ax / 2  ==>  ax <- ��, dx <- ����
	cmp	dx, 0
	jz	LABEL_EVEN
	mov	byte [bOdd], 1
LABEL_EVEN:;ż��
	xor	dx, dx			; ���� ax ���� FATEntry �� FAT �е�ƫ����. ���������� FATEntry ���ĸ�������(FATռ�ò�ֹһ������)
	mov	bx, [BPB_BytsPerSec]
	div	bx			; dx:ax / BPB_BytsPerSec  ==>	ax <- ��   (FATEntry ���ڵ���������� FAT ��˵��������)
					;				dx <- ���� (FATEntry �������ڵ�ƫ��)��
	push	dx
	mov	bx, 0			; bx <- 0	����, es:bx = (BaseOfKernelFile - 100):00 = (BaseOfKernelFile - 100) * 10h
	add	ax, SectorNoOfFAT1	; �˾�ִ��֮��� ax ���� FATEntry ���ڵ�������
	mov	cl, 2
	call	ReadSector		; ��ȡ FATEntry ���ڵ�����, һ�ζ�����, �����ڱ߽緢������, ��Ϊһ�� FATEntry ���ܿ�Խ��������
	pop	dx
	add	bx, dx
	mov	ax, [es:bx]
	cmp	byte [bOdd], 1
	jnz	LABEL_EVEN_2
	shr	ax, 4
LABEL_EVEN_2:
	and	ax, 0FFFh

LABEL_GET_FAT_ENRY_OK:

	pop	bx
	pop	es
	ret
;----------------------------------------------------------------------------


;----------------------------------------------------------------------------
; ������: KillMotor
;----------------------------------------------------------------------------
; ����:
;	�ر��������
KillMotor:
	push	dx
	mov	dx, 03F2h
	mov	al, 0
	out	dx, al
	pop	dx
	ret
;----------------------------------------------------------------------------


; �Ӵ��Ժ�Ĵ����ڱ���ģʽ��ִ�� ----------------------------------------------------
; 32 λ�����. ��ʵģʽ���� ---------------------------------------------------------
[SECTION .s32]

ALIGN	32

[BITS	32]

LABEL_PM_START:
	mov	ax, SelectorVideo
	mov	gs, ax
	mov	ax, SelectorFlatRW
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	ss, ax
	mov	esp, TopOfStack

	push	szMemChkTitle
	call	DispStr
	add	esp, 4

	call	DispMemInfo
	call	SetupPaging

	;mov	ah, 0Fh				; 0000: �ڵ�    1111: ����
	;mov	al, 'P'
	;mov	[gs:((80 * 0 + 39) * 2)], ax	; ��Ļ�� 0 ��, �� 39 �С�

	call	InitKernel

	;jmp	$

	;***************************************************************
	jmp	SelectorFlatC:KernelEntryPointPhyAddr	; ��ʽ�����ں� *
	;***************************************************************
	; �ڴ濴��ȥ�������ģ�
	;              ��                                    ��
	;              ��                 .                  ��
	;              ��                 .                  ��
	;              ��                 .                  ��
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;              ��������������Page  Tables��������������
	;              ������������(��С��LOADER����)����������
	;    00101000h ���������������������������������������� PageTblBase
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;    00100000h ����������Page Directory Table���������� PageDirBase  <- 1M
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;       F0000h ����������������System ROM��������������
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;       E0000h ����������Expansion of system ROM ������
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;       C0000h ��������Reserved for ROM expansion������
	;              �ǩ�������������������������������������
	;              ���������������������������������������� B8000h �� gs
	;       A0000h ��������Display adapter reserved��������
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;       9FC00h ������extended BIOS data area (EBDA)����
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;       90000h ����������������LOADER.BIN�������������� somewhere in LOADER �� esp
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;       80000h ����������������KERNEL.BIN��������������
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;       30000h ������������������KERNEL���������������� 30400h �� KERNEL ��� (KernelEntryPointPhyAddr)
	;              �ǩ�������������������������������������
	;              ��                                    ��
	;        7E00h ��              F  R  E  E            ��
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;        7C00h ��������������BOOT  SECTOR��������������
	;              �ǩ�������������������������������������
	;              ��                                    ��
	;         500h ��              F  R  E  E            ��
	;              �ǩ�������������������������������������
	;              ����������������������������������������
	;         400h ����������ROM BIOS parameter area ������
	;              �ǩ�������������������������������������
	;              ���������������������
	;           0h ���������Int  Vectors�������
	;              ���������������������������������������� �� cs, ds, es, fs, ss
	;
	;
	;		����������		����������
	;		���������� Tinixʹ��	���������� ����ʹ�õ��ڴ�
	;		����������		����������
	;		����������		����������
	;		��      �� δʹ�ÿռ�	������ ���Ը��ǵ��ڴ�
	;		����������		����������
	;
	; ע��KERNEL ��λ��ʵ�����Ǻ����ģ�����ͨ��ͬʱ�ı� LOAD.INC �е� KernelEntryPointPhyAddr �� MAKEFILE �в��� -Ttext ��ֵ���ı䡣
	;     ���磬����� KernelEntryPointPhyAddr �� -Ttext ��ֵ����Ϊ 0x400400���� KERNEL �ͻᱻ���ص��ڴ� 0x400000(4M) ��������� 0x400400��
	;




; ------------------------------------------------------------------------
; ��ʾ AL �е�����
; ------------------------------------------------------------------------
DispAL:
	push	ecx
	push	edx
	push	edi

	mov	edi, [dwDispPos]

	mov	ah, 0Fh			; 0000b: �ڵ�    1111b: ����
	mov	dl, al
	shr	al, 4
	mov	ecx, 2
.begin:
	and	al, 01111b
	cmp	al, 9
	ja	.1
	add	al, '0'
	jmp	.2
.1:
	sub	al, 0Ah
	add	al, 'A'
.2:
	mov	[gs:edi], ax
	add	edi, 2

	mov	al, dl
	loop	.begin
	;add	edi, 2

	mov	[dwDispPos], edi

	pop	edi
	pop	edx
	pop	ecx

	ret
; DispAL ����-------------------------------------------------------------


; ------------------------------------------------------------------------
; ��ʾһ��������
; ------------------------------------------------------------------------
DispInt:
	mov	eax, [esp + 4]
	shr	eax, 24
	call	DispAL

	mov	eax, [esp + 4]
	shr	eax, 16
	call	DispAL

	mov	eax, [esp + 4]
	shr	eax, 8
	call	DispAL

	mov	eax, [esp + 4]
	call	DispAL

	mov	ah, 07h			; 0000b: �ڵ�    0111b: ����
	mov	al, 'h'
	push	edi
	mov	edi, [dwDispPos]
	mov	[gs:edi], ax
	add	edi, 4
	mov	[dwDispPos], edi
	pop	edi

	ret
; DispInt ����------------------------------------------------------------

; ------------------------------------------------------------------------
; ��ʾһ���ַ���
; ------------------------------------------------------------------------
DispStr:
	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [dwDispPos]
	mov	ah, 0Fh
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; �ǻس���?
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
	mov	[dwDispPos], edi

	pop	edi
	pop	esi
	pop	ebx
	pop	ebp
	ret
; DispStr ����------------------------------------------------------------

; ------------------------------------------------------------------------
; ����
; ------------------------------------------------------------------------
DispReturn:
	push	szReturn
	call	DispStr			;printf("\n");
	add	esp, 4

	ret
; DispReturn ����---------------------------------------------------------


; ------------------------------------------------------------------------
; �ڴ濽������ memcpy
; ------------------------------------------------------------------------
; void* MemCpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
MemCpy:
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
	inc	esi			; ��
					; �� ���ֽ��ƶ�
	mov	byte [es:edi], al	; ��
	inc	edi			; ��

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
; MemCpy ����-------------------------------------------------------------




; ��ʾ�ڴ���Ϣ --------------------------------------------------------------
DispMemInfo:
	push	esi
	push	edi
	push	ecx

	mov	esi, MemChkBuf
	mov	ecx, [dwMCRNumber]	;for(int i=0;i<[MCRNumber];i++) // ÿ�εõ�һ��ARDS(Address Range Descriptor Structure)�ṹ
.loop:					;{
	mov	edx, 5			;	for(int j=0;j<5;j++)	// ÿ�εõ�һ��ARDS�еĳ�Ա����5����Ա
	mov	edi, ARDStruct		;	{			// ������ʾ��BaseAddrLow��BaseAddrHigh��LengthLow��LengthHigh��Type
.1:					;
	push	dword [esi]		;
	call	DispInt			;		DispInt(MemChkBuf[j*4]); // ��ʾһ����Ա
	pop	eax			;
	stosd				;		ARDStruct[j*4] = MemChkBuf[j*4];
	add	esi, 4			;
	dec	edx			;
	cmp	edx, 0			;
	jnz	.1			;	}
	call	DispReturn		;	printf("\n");
	cmp	dword [dwType], 1	;	if(Type == AddressRangeMemory) // AddressRangeMemory : 1, AddressRangeReserved : 2
	jne	.2			;	{
	mov	eax, [dwBaseAddrLow]	;
	add	eax, [dwLengthLow]	;
	cmp	eax, [dwMemSize]	;		if(BaseAddrLow + LengthLow > MemSize)
	jb	.2			;
	mov	[dwMemSize], eax	;			MemSize = BaseAddrLow + LengthLow;
.2:					;	}
	loop	.loop			;}
					;
	call	DispReturn		;printf("\n");
	push	szRAMSize		;
	call	DispStr			;printf("RAM size:");
	add	esp, 4			;
					;
	push	dword [dwMemSize]	;
	call	DispInt			;DispInt(MemSize);
	add	esp, 4			;

	pop	ecx
	pop	edi
	pop	esi
	ret
; ---------------------------------------------------------------------------

; ������ҳ���� --------------------------------------------------------------
SetupPaging:
	; �����ڴ��С����Ӧ��ʼ������PDE�Լ�����ҳ��
	xor	edx, edx
	mov	eax, [dwMemSize]
	mov	ebx, 400000h	; 400000h = 4M = 4096 * 1024, һ��ҳ���Ӧ���ڴ��С
	div	ebx
	mov	ecx, eax	; ��ʱ ecx Ϊҳ��ĸ�����Ҳ�� PDE Ӧ�õĸ���
	test	edx, edx
	jz	.no_remainder
	inc	ecx		; ���������Ϊ 0 ��������һ��ҳ��
.no_remainder:
	push	ecx		; �ݴ�ҳ�����

	; Ϊ�򻯴���, �������Ե�ַ��Ӧ��ȵ������ַ. ���Ҳ������ڴ�ն�.

	; ���ȳ�ʼ��ҳĿ¼
	mov	ax, SelectorFlatRW
	mov	es, ax
	mov	edi, PageDirBase	; �˶��׵�ַΪ PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase | PG_P  | PG_USU | PG_RWW
.1:
	stosd
	add	eax, 4096		; Ϊ�˼�, ����ҳ�����ڴ�����������.
	loop	.1

	; �ٳ�ʼ������ҳ��
	pop	eax			; ҳ�����
	mov	ebx, 1024		; ÿ��ҳ�� 1024 �� PTE
	mul	ebx
	mov	ecx, eax		; PTE���� = ҳ����� * 1024
	mov	edi, PageTblBase	; �˶��׵�ַΪ PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.2:
	stosd
	add	eax, 4096		; ÿһҳָ�� 4K �Ŀռ�
	loop	.2

	mov	eax, PageDirBase
	mov	cr3, eax
	mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax
	jmp	short .3
.3:
	nop

	ret
; ��ҳ����������� ----------------------------------------------------------



; InitKernel ---------------------------------------------------------------------------------
; �� KERNEL.BIN �����ݾ�����������ŵ��µ�λ��
; --------------------------------------------------------------------------------------------
InitKernel:	; ����ÿһ�� Program Header������ Program Header �е���Ϣ��ȷ����ʲô�Ž��ڴ棬�ŵ�ʲôλ�ã��Լ��Ŷ��١�
	xor	esi, esi
	mov	cx, word [BaseOfKernelFilePhyAddr + 2Ch]; �� ecx <- pELFHdr->e_phnum
	movzx	ecx, cx					; ��
	mov	esi, [BaseOfKernelFilePhyAddr + 1Ch]	; esi <- pELFHdr->e_phoff
	add	esi, BaseOfKernelFilePhyAddr		; esi <- OffsetOfKernel + pELFHdr->e_phoff
.Begin:
	mov	eax, [esi + 0]
	cmp	eax, 0				; PT_NULL
	jz	.NoAction
	push	dword [esi + 010h]		; size	��
	mov	eax, [esi + 04h]		;	��
	add	eax, BaseOfKernelFilePhyAddr	;	�� ::memcpy(	(void*)(pPHdr->p_vaddr),
	push	eax				; src	��		uchCode + pPHdr->p_offset,
	push	dword [esi + 08h]		; dst	��		pPHdr->p_filesz;
	call	MemCpy				;	��
	add	esp, 12				;	��
.NoAction:
	add	esi, 020h			; esi += pELFHdr->e_phentsize
	dec	ecx
	jnz	.Begin

	ret
; InitKernel ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


; SECTION .data1 ֮��ʼ ---------------------------------------------------------------------------------------------
[SECTION .data1]

ALIGN	32

LABEL_DATA:
; ʵģʽ��ʹ����Щ����
; �ַ���
_szMemChkTitle:			db	"BaseAddrL BaseAddrH LengthLow LengthHigh   Type", 0Ah, 0
_szRAMSize:			db	"RAM size:", 0
_szReturn:			db	0Ah, 0
;; ����
_dwMCRNumber:			dd	0	; Memory Check Result
_dwDispPos:			dd	(80 * 6 + 0) * 2	; ��Ļ�� 6 ��, �� 0 �С�
_dwMemSize:			dd	0
_ARDStruct:			; Address Range Descriptor Structure
	_dwBaseAddrLow:		dd	0
	_dwBaseAddrHigh:	dd	0
	_dwLengthLow:		dd	0
	_dwLengthHigh:		dd	0
	_dwType:		dd	0
_MemChkBuf:	times	256	db	0
;
;; ����ģʽ��ʹ����Щ����
szMemChkTitle		equ	BaseOfLoaderPhyAddr + _szMemChkTitle
szRAMSize		equ	BaseOfLoaderPhyAddr + _szRAMSize
szReturn		equ	BaseOfLoaderPhyAddr + _szReturn
dwDispPos		equ	BaseOfLoaderPhyAddr + _dwDispPos
dwMemSize		equ	BaseOfLoaderPhyAddr + _dwMemSize
dwMCRNumber		equ	BaseOfLoaderPhyAddr + _dwMCRNumber
ARDStruct		equ	BaseOfLoaderPhyAddr + _ARDStruct
	dwBaseAddrLow	equ	BaseOfLoaderPhyAddr + _dwBaseAddrLow
	dwBaseAddrHigh	equ	BaseOfLoaderPhyAddr + _dwBaseAddrHigh
	dwLengthLow	equ	BaseOfLoaderPhyAddr + _dwLengthLow
	dwLengthHigh	equ	BaseOfLoaderPhyAddr + _dwLengthHigh
	dwType		equ	BaseOfLoaderPhyAddr + _dwType
MemChkBuf		equ	BaseOfLoaderPhyAddr + _MemChkBuf


; ��ջ�������ݶε�ĩβ
StackSpace:	times	1000h	db	0
TopOfStack	equ	BaseOfLoaderPhyAddr + $	; ջ��
; SECTION .data1 ֮���� ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

