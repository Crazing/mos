/*
 * File: /include/process/process.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 11:00:13 am
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 10:14:54 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ���̴����������
 */


#ifndef	_MOS_PROCESS_H_
#define	_MOS_PROCESS_H_

#include "global.h"
#include "sys_call.h"
#include "stdio.h"
#include "tty.h"

//���̵��Ⱥ���
void schedule();

//��ʼ������
void init_proc();

#endif /* _MOS_PROCESS_H_ */
