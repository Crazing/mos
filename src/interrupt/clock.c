/*
 * File: /src/interrupt/clock.c
 * Project: MOS
 * File Created: Tuesday, 24th April 2018 11:32:48 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Friday, 27th April 2018 12:39:15 am
 * Modified By: zhixiang
 * -----
 * FileContent: 时钟中断程序主体
 */


#include "clock.h"

//时钟中断处理函数
static void clock_handler(int irq)
{
	//disp_str("#");
	ticks++;
    if(ticks > 10000)
        ticks = 0;
	p_proc_ready->ticks--;

	if (k_reenter != 0) {
		//disp_str("!");
		return;
	}

	if (p_proc_ready->ticks > 0) {
		return;
	}

	schedule();
}

//延迟函数
void milli_delay(int milli_sec)
{
	int t = get_ticks();

	while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}

//设置时钟中断
void init_clock()
{
	// 初始化 8253 PIT
	out_byte(TIMER_MODE, RATE_GENERATOR);
	out_byte(TIMER0, (t_8) (TIMER_FREQ/HZ) );
	out_byte(TIMER0, (t_8) ((TIMER_FREQ/HZ) >> 8));
    
    // 设定时钟中断处理程序
	put_irq_handler(CLOCK_IRQ, clock_handler);	
}
