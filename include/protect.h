
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            protect.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_TINIX_PROTECT_H_
#define	_TINIX_PROTECT_H_


/* �洢��������/ϵͳ�������� */
typedef struct s_descriptor		/* �� 8 ���ֽ� */
{
	t_16	limit_low;		/* Limit */
	t_16	base_low;		/* Base */
	t_8	base_mid;		/* Base */
	t_8	attr1;			/* P(1) DPL(2) DT(1) TYPE(4) */
	t_8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	t_8	base_high;		/* Base */
}DESCRIPTOR;

/* �������� */
typedef struct s_gate
{
	t_16	offset_low;	/* Offset Low */
	t_16	selector;	/* Selector */
	t_8	dcount;		/* ���ֶ�ֻ�ڵ���������������Ч��
				��������õ����ŵ����ӳ���ʱ������Ȩ����ת���Ͷ�ջ�ĸı䣬��Ҫ������ջ�еĲ������Ƶ��ڲ��ջ��
				��˫�ּ����ֶξ�������˵�������������ʱ��Ҫ���Ƶ�˫�ֲ����������� */
	t_8	attr;		/* P(1) DPL(2) DT(1) TYPE(4) */
	t_16	offset_high;	/* Offset High */
}GATE;

typedef struct s_tss {
	t_32	backlink;
	t_32	esp0;		/* stack pointer to use during interrupt */
	t_32	ss0;		/*   "   segment  "  "    "        "     */
	t_32	esp1;
	t_32	ss1;
	t_32	esp2;
	t_32	ss2;
	t_32	cr3;
	t_32	eip;
	t_32	flags;
	t_32	eax;
	t_32	ecx;
	t_32	edx;
	t_32	ebx;
	t_32	esp;
	t_32	ebp;
	t_32	esi;
	t_32	edi;
	t_32	es;
	t_32	cs;
	t_32	ss;
	t_32	ds;
	t_32	fs;
	t_32	gs;
	t_32	ldt;
	t_16	trap;
	t_16	iobase;	/* I/Oλͼ��ַ���ڻ����TSS�ν��ޣ��ͱ�ʾû��I/O���λͼ */
	/*t_8	iomap[2];*/
}TSS;

/* GDT */
/* ���������� */
#define	INDEX_DUMMY		0	// ��
#define	INDEX_FLAT_C		1	// �� LOADER �����Ѿ�ȷ���˵�.
#define	INDEX_FLAT_RW		2	// ��
#define	INDEX_VIDEO		3	// ��
#define	INDEX_TSS		4
#define	INDEX_LDT_FIRST		5
/* ѡ���� */
#define	SELECTOR_DUMMY		   0		// ��
#define	SELECTOR_FLAT_C		0x08		// �� LOADER �����Ѿ�ȷ���˵�.
#define	SELECTOR_FLAT_RW	0x10		// ��
#define	SELECTOR_VIDEO		(0x18+3)	// ��<-- RPL=3
#define	SELECTOR_TSS		0x20		// TSS. ����������ڴ�ʱ SS �� ESP ��ֵ��������.
#define SELECTOR_LDT_FIRST	0x28

#define	SELECTOR_KERNEL_CS	SELECTOR_FLAT_C
#define	SELECTOR_KERNEL_DS	SELECTOR_FLAT_RW
#define	SELECTOR_KERNEL_GS	SELECTOR_VIDEO

/* ÿ��������һ�������� LDT, ÿ�� LDT �е�����������: */
#define LDT_SIZE		2

/* ����������ֵ˵�� */
#define	DA_32			0x4000	/* 32 λ��				*/
#define	DA_LIMIT_4K		0x8000	/* �ν�������Ϊ 4K �ֽ�			*/
#define	DA_DPL0			0x00	/* DPL = 0				*/
#define	DA_DPL1			0x20	/* DPL = 1				*/
#define	DA_DPL2			0x40	/* DPL = 2				*/
#define	DA_DPL3			0x60	/* DPL = 3				*/
/* �洢������������ֵ˵�� */
#define	DA_DR			0x90	/* ���ڵ�ֻ�����ݶ�����ֵ		*/
#define	DA_DRW			0x92	/* ���ڵĿɶ�д���ݶ�����ֵ		*/
#define	DA_DRWA			0x93	/* ���ڵ��ѷ��ʿɶ�д���ݶ�����ֵ	*/
#define	DA_C			0x98	/* ���ڵ�ִֻ�д��������ֵ		*/
#define	DA_CR			0x9A	/* ���ڵĿ�ִ�пɶ����������ֵ		*/
#define	DA_CCO			0x9C	/* ���ڵ�ִֻ��һ�´��������ֵ		*/
#define	DA_CCOR			0x9E	/* ���ڵĿ�ִ�пɶ�һ�´��������ֵ	*/
/* ϵͳ������������ֵ˵�� */
#define	DA_LDT			0x82	/* �ֲ��������������ֵ			*/
#define	DA_TaskGate		0x85	/* ����������ֵ				*/
#define	DA_386TSS		0x89	/* ���� 386 ����״̬������ֵ		*/
#define	DA_386CGate		0x8C	/* 386 ����������ֵ			*/
#define	DA_386IGate		0x8E	/* 386 �ж�������ֵ			*/
#define	DA_386TGate		0x8F	/* 386 ����������ֵ			*/

/* ѡ��������ֵ˵�� */
/* ����, SA_ : Selector Attribute */
#define	SA_RPL_MASK	0xFFFC
#define	SA_RPL0		0
#define	SA_RPL1		1
#define	SA_RPL2		2
#define	SA_RPL3		3

#define	SA_TI_MASK	0xFFFB
#define	SA_TIG		0
#define	SA_TIL		4

/* �ж����� */
#define	INT_VECTOR_DIVIDE		0x0
#define	INT_VECTOR_DEBUG		0x1
#define	INT_VECTOR_NMI			0x2
#define	INT_VECTOR_BREAKPOINT		0x3
#define	INT_VECTOR_OVERFLOW		0x4
#define	INT_VECTOR_BOUNDS		0x5
#define	INT_VECTOR_INVAL_OP		0x6
#define	INT_VECTOR_COPROC_NOT		0x7
#define	INT_VECTOR_DOUBLE_FAULT		0x8
#define	INT_VECTOR_COPROC_SEG		0x9
#define	INT_VECTOR_INVAL_TSS		0xA
#define	INT_VECTOR_SEG_NOT		0xB
#define	INT_VECTOR_STACK_FAULT		0xC
#define	INT_VECTOR_PROTECTION		0xD
#define	INT_VECTOR_PAGE_FAULT		0xE
#define	INT_VECTOR_COPROC_ERR		0x10

/* �ж����� */
#define	INT_VECTOR_IRQ0			0x20
#define	INT_VECTOR_IRQ8			0x28

/* ϵͳ���� */
#define	INT_VECTOR_SYS_CALL		0x90


/* �� */
/* ���Ե�ַ �� �����ַ */
#define vir2phys(seg_base, vir)	(t_32)(((t_32)seg_base) + (t_32)(vir))


#endif /* _TINIX_PROTECT_H_ */
