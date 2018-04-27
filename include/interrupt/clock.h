/*
 * File: /include/interrupt/clock.h
 * Project: MOS
 * File Created: Tuesday, 24th April 2018 11:33:34 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 25th April 2018 12:26:21 am
 * Modified By: zhixiang
 * -----
 * FileContent: ʱ���жϳ�������
 */


#ifndef	_MOS_CLOCK_H_
#define	_MOS_CLOCK_H_

#include "global.h"
#include "common.h"
#include "sys_call.h"
#include "process.h"
#include "8259A.h"

//�ӳٺ���
void milli_delay(int milli_sec);

//����ʱ���ж�
void init_clock();

#endif /* _MOS_CLOCK_H_ */