/*
 * File: /src/main/kernel.c
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 1:10:04 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 2:57:35 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 内核主体文件
 */


#include "kernel.h"

//定义tss
TSS		            tss;

//定义内核中的GDT
ut_8		    gdt_ptr[6] = {0};	    // 0~15:Limit  16~47:Base
DESCRIPTOR	    gdt[GDT_SIZE] = {0};

static void init_descriptor(DESCRIPTOR * p_desc, ut_32 base, ut_32 limit, ut_16 attribute)
{
	p_desc->limit_low		 = limit & 0x0FFFF;		        // 段界限 1		(2 字节)
	p_desc->base_low		 = base & 0x0FFFF;		        // 段基址 1		(2 字节)
	p_desc->base_mid		 = (base >> 16) & 0x0FF;		// 段基址 2		(1 字节)
	p_desc->attr1			 = attribute & 0xFF;		    // 属性 1
	p_desc->limit_high_attr2 = ((limit >> 16) & 0x0F) |
						       (attribute >> 8) & 0xF0;     // 段界限 2 + 属性 2
	p_desc->base_high		 = (base >> 24) & 0x0FF;		// 段基址 3		(1 字节)
}

//由段获取段基址
static ut_32 get_segaddr(ut_16 seg)
{
	DESCRIPTOR* p_dest = &gdt[seg >> 3];

	return (p_dest->base_high << 24) | (p_dest->base_mid << 16) | (p_dest->base_low);
}

//初始化内核中的gdt
static void init_gdt()
{
    //将gdt的长度和地址存到gdt_ptr
    store_gdt(gdt_ptr);

    // 将 LOADER 中的 GDT 复制到新的 GDT 中
	memcpy(	&gdt,				                // New GDT
		(void*)(*((ut_32*)(&gdt_ptr[2]))),      // Base  of Old GDT
		*((ut_16*)(&gdt_ptr[0])) + 1	        // Limit of Old GDT
		);
        
	// gdt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sgdt 以及 lgdt 的参数。
	ut_16* p_gdt_limit = (ut_16*)(&gdt_ptr[0]);
	ut_32* p_gdt_base  = (ut_32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base  = (ut_32)&gdt;

    //加载新的gdt
    load_gdt(gdt_ptr);

    /* 填充 GDT 中 TSS 这个描述符 */
	memset(&tss, 0, sizeof(tss));
	tss.ss0		= SELECTOR_KERNEL_DS;
	init_descriptor(&gdt[INDEX_TSS],
			GETVIRADDR(get_segaddr(SELECTOR_KERNEL_DS), &tss),
			sizeof(tss) - 1,
			DA_386TSS);
	tss.iobase	= sizeof(tss);	/* 没有I/O许可位图 */

    //加载tss
    load_tss();
    
	/* 填充 GDT 中每个进程的 LDT 的描述符 */
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

//初始化idt
static void init_idt()
{
    // 初始化中断处理
    init_interrupt();
    
    //加载idt
    load_idt(idt_ptr);
}

//内核初始化函数
void init_kernel()
{  
	disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n-----\"init_kernel\" begins-----\n");

    //初始化gdt
    init_gdt();
    
	//初始化idt
    init_idt();
	
    //初始化进程
	init_proc();

	disp_str("-----\"init_kernel\" finished-----\n");
}

//内核运行
void mos_main()
{
    //打开中断功能
    open_interrupt();

    //跳入进程
    restart();
}