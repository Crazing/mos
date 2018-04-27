/*
 * File: /include/interrupt/sys_call.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 3:26:49 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 23rd April 2018 10:21:41 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 系统调用
 */


#ifndef	_MOS_SYS_CALL_H_
#define	_MOS_SYS_CALL_H_

#include "global.h"

//系统调用声明
t_32	    get_ticks();
t_32    sys_get_ticks();

#endif /* _MOS_SYS_CALL_H_ */