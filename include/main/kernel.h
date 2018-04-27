/*
 * File: /include/main/kernel.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 1:04:49 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 24th April 2018 10:52:59 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 内核主体文件
 */


#ifndef	_MOS_KERNEL_H_
#define	_MOS_KERNEL_H_

#include "type.h"
#include "common.h"
#include "interrupt.h"
#include "process.h"

//内核初始化函数
void init_kernel();

//内核运行
void mos_main();

//由段获取段基址
t_32 get_segaddr(t_16 seg);

// 加载和存储gdt
void load_gdt(ut_8  *gdt_ptr);
void store_gdt(ut_8 *gdt_ptr);

// 加载和存储idt
void load_idt(ut_8  *idt_ptr);
void store_idt(ut_8 *idt_ptr);

// 加载tss
void load_tss();

#endif /* _MOS_KERNEL_H_ */