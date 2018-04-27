/*
 * File: /include/interrupt/8259A.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 11:16:34 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Thursday, 26th April 2018 9:00:38 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 8259A主体程序
 */


#ifndef	_MOS_8259A_H_
#define	_MOS_8259A_H_

#include "type.h"
#include "common.h"

//初始化8259A
void init_8259A();

//设置8259A
void set_8259A();

//打开8259A
void open_8259A();

//8259A默认的中断处理程序
void spurious_irq(int irq);

//设置irq的处理函数
void put_irq_handler(int irq, t_pf_irq_handler handler);

#endif /* _MOS_8259A_H_ */