/*
 * File: /src/main/kernel.c
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 1:10:04 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 2:57:35 pm
 * Modified By: zhixiang
 * -----
 * FileContent: �ں������ļ�
 */


#include "kernel.h"

//����tss
TSS		            tss;

//�����ں��е�GDT
ut_8		    gdt_ptr[6] = {0};	    // 0~15:Limit  16~47:Base
DESCRIPTOR	    gdt[GDT_SIZE] = {0};

static void init_descriptor(DESCRIPTOR * p_desc, ut_32 base, ut_32 limit, ut_16 attribute)
{
	p_desc->limit_low		 = limit & 0x0FFFF;		        // �ν��� 1		(2 �ֽ�)
	p_desc->base_low		 = base & 0x0FFFF;		        // �λ�ַ 1		(2 �ֽ�)
	p_desc->base_mid		 = (base >> 16) & 0x0FF;		// �λ�ַ 2		(1 �ֽ�)
	p_desc->attr1			 = attribute & 0xFF;		    // ���� 1
	p_desc->limit_high_attr2 = ((limit >> 16) & 0x0F) |
						       (attribute >> 8) & 0xF0;     // �ν��� 2 + ���� 2
	p_desc->base_high		 = (base >> 24) & 0x0FF;		// �λ�ַ 3		(1 �ֽ�)
}

//�ɶλ�ȡ�λ�ַ
static ut_32 get_segaddr(ut_16 seg)
{
	DESCRIPTOR* p_dest = &gdt[seg >> 3];

	return (p_dest->base_high << 24) | (p_dest->base_mid << 16) | (p_dest->base_low);
}

//��ʼ���ں��е�gdt
static void init_gdt()
{
    //��gdt�ĳ��Ⱥ͵�ַ�浽gdt_ptr
    store_gdt(gdt_ptr);

    // �� LOADER �е� GDT ���Ƶ��µ� GDT ��
	memcpy(	&gdt,				                // New GDT
		(void*)(*((ut_32*)(&gdt_ptr[2]))),      // Base  of Old GDT
		*((ut_16*)(&gdt_ptr[0])) + 1	        // Limit of Old GDT
		);
        
	// gdt_ptr[6] �� 6 ���ֽڣ�0~15:Limit  16~47:Base������ sgdt �Լ� lgdt �Ĳ�����
	ut_16* p_gdt_limit = (ut_16*)(&gdt_ptr[0]);
	ut_32* p_gdt_base  = (ut_32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base  = (ut_32)&gdt;

    //�����µ�gdt
    load_gdt(gdt_ptr);

    /* ��� GDT �� TSS ��������� */
	memset(&tss, 0, sizeof(tss));
	tss.ss0		= SELECTOR_KERNEL_DS;
	init_descriptor(&gdt[INDEX_TSS],
			GETVIRADDR(get_segaddr(SELECTOR_KERNEL_DS), &tss),
			sizeof(tss) - 1,
			DA_386TSS);
	tss.iobase	= sizeof(tss);	/* û��I/O���λͼ */

    //����tss
    load_tss();
    
	/* ��� GDT ��ÿ�����̵� LDT �������� */
	t_32 i;
	PROCESS* p_proc	= proc_table;
	t_16 index_ldt  = INDEX_LDT_FIRST;
	for(i=0;i<NR_TASKS + NR_PROCS;i++){
		init_descriptor(&gdt[index_ldt],
				GETVIRADDR(get_segaddr(SELECTOR_KERNEL_DS), proc_table[i].ldts),
				LDT_SIZE * sizeof(DESCRIPTOR) - 1,
				DA_LDT);
		p_proc++;
		index_ldt += 1;
	}
}

//��ʼ��idt
static void init_idt()
{
    // ��ʼ���жϴ���
    init_interrupt();
    
    //����idt
    load_idt(idt_ptr);
}

//�ں˳�ʼ������
void init_kernel()
{  
	disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n-----\"init_kernel\" begins-----\n");

    //��ʼ��gdt
    init_gdt();
    
	//��ʼ��idt
    init_idt();
	
    //��ʼ������
	init_proc();

	disp_str("-----\"init_kernel\" finished-----\n");
}

//�ں�����
void mos_main()
{
    //���жϹ���
    open_interrupt();

    //�������
    restart();
}