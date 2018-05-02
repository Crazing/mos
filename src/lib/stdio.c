/*
 * File: /src/lib/stdio.c
 * Project: MOS
 * File Created: Wednesday, 2nd May 2018 9:34:19 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 11:10:48 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 格式化输入输出
 */


#include "stdio.h"

/*======================================================================*
                                vsprintf
 *======================================================================*/
static t_32 vsprintf(t_8 *buf, const t_8 *fmt, va_list args)
{
	t_8* p = NULL;
	t_8	 tmp[256] = {0};
	va_list	p_next_arg = args;

	for (p=buf;*fmt;fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}

		fmt++;

		switch (*fmt) {
		case 'x':
			itoa(tmp, *((t_32*)p_next_arg));
			strcpy(p, tmp);
			p_next_arg += 4;
			p += strlen(tmp);
			break;
		case 's':
			break;
		default:
			break;
		}
	}

	return (p - buf);
}

//格式化输出
t_32 printf(const t_8 *fmt, ...)
{
	t_32 i = 0;
	t_8  buf[256] = {0};

	va_list arg = (va_list)((char*)(&fmt) + 4);        /* 4 是参数 fmt 所占堆栈中的大小 */
	i = vsprintf(buf, fmt, arg);
	write(buf, i);

	return i;
}