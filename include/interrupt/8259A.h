/*
 * File: /include/interrupt/8259A.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 11:16:34 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Sunday, 29th April 2018 10:19:05 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 8259A�������
 */


#ifndef	_MOS_8259A_H_
#define	_MOS_8259A_H_

#include "type.h"
#include "common.h"

//��ʼ��8259A
void init_8259A();

//����8259A
void set_8259A();

//��8259A
void open_8259A();

//8259AĬ�ϵ��жϴ�������
void spurious_irq(t_32 irq);

//����irq�Ĵ�������
void put_irq_handler(t_32 irq, t_pf_irq_handler handler);

#endif /* _MOS_8259A_H_ */