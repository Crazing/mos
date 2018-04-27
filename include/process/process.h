/*
 * File: /include/process/process.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 11:00:13 am
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Friday, 27th April 2018 12:10:08 am
 * Modified By: zhixiang
 * -----
 * FileContent: 进程处理程序主体
 */


#ifndef	_MOS_PROCESS_H_
#define	_MOS_PROCESS_H_

#include "global.h"
#include "sys_call.h"
#include "tty.h"

//进程调度函数
void schedule();

//初始化进程
void init_proc();

//进程A
void TestA();

//进程B
void TestB();

//进程C
void TestC();

#endif /* _MOS_PROCESS_H_ */
