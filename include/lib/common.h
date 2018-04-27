/*
 * File: /include/lib/common.h
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 4:09:13 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 25th April 2018 8:36:23 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ¹«¹²»ù´¡¿â
 */


#ifndef	_MOS_COMMON_H_
#define	_MOS_COMMON_H_

#include "type.h"
#include "const.h"

void	disp_str(t_8 * info);
void    disp_color_str(t_8 * info, t_32 color);
void    disp_int(t_32 input);
t_8 *   itoa(t_8 * str, t_32 num);
void    enable_irq(int irq);
t_32    disable_irq(int irq);
t_8     in_byte(t_port port);
void    out_byte(t_port port, t_8 value);
void    disable_int();
void    enable_int();

#endif /* _MOS_COMMON_H_ */