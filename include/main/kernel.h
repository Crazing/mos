/*
 * File: /include/main/kernel.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 1:04:49 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 24th April 2018 10:52:59 pm
 * Modified By: zhixiang
 * -----
 * FileContent: �ں������ļ�
 */


#ifndef	_MOS_KERNEL_H_
#define	_MOS_KERNEL_H_

#include "type.h"
#include "common.h"
#include "interrupt.h"
#include "process.h"

//�ں˳�ʼ������
void init_kernel();

//�ں�����
void mos_main();

//�ɶλ�ȡ�λ�ַ
t_32 get_segaddr(t_16 seg);

// ���غʹ洢gdt
void load_gdt(ut_8  *gdt_ptr);
void store_gdt(ut_8 *gdt_ptr);

// ���غʹ洢idt
void load_idt(ut_8  *idt_ptr);
void store_idt(ut_8 *idt_ptr);

// ����tss
void load_tss();

#endif /* _MOS_KERNEL_H_ */