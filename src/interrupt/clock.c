/*
 * File: /src/interrupt/clock.c
 * Project: MOS
 * File Created: Tuesday, 24th April 2018 11:32:48 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Friday, 27th April 2018 12:39:15 am
 * Modified By: zhixiang
 * -----
 * FileContent: ʱ���жϳ�������
 */


#include "clock.h"

//ʱ���жϴ�����
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

//�ӳٺ���
void milli_delay(int milli_sec)
{
	int t = get_ticks();

	while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}

//����ʱ���ж�
void init_clock()
{
	// ��ʼ�� 8253 PIT
	out_byte(TIMER_MODE, RATE_GENERATOR);
	out_byte(TIMER0, (t_8) (TIMER_FREQ/HZ) );
	out_byte(TIMER0, (t_8) ((TIMER_FREQ/HZ) >> 8));
    
    // �趨ʱ���жϴ������
	put_irq_handler(CLOCK_IRQ, clock_handler);	
}
