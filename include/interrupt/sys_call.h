/*
 * File: /include/interrupt/sys_call.h
 * Project: MOS
 * File Created: Monday, 23rd April 2018 3:26:49 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 10:05:13 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 系统调用
 */


#ifndef	_MOS_SYS_CALL_H_
#define	_MOS_SYS_CALL_H_

#include "global.h"
#include "clock.h"
#include "console.h"

//系统调用声明
t_32	    get_ticks();
void	    write(t_8* buf, t_32 len);

#endif /* _MOS_SYS_CALL_H_ */