/*
 * File: /src/interrupt/clock.c
 * Project: MOS
 * File Created: Tuesday, 24th April 2018 11:32:48 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Wednesday, 2nd May 2018 11:23:52 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ʱ���жϳ�������
 */


#include "clock.h"

//ʱ���жϴ��� 
static ticks = 0;

//ʱ���жϴ�����
static void clock_handler(t_32 irq)
{
	//disp_str("#");
	ticks++;
    if(ticks >= (((ut_32)1<<31)-1))
        ticks = 0;

	if (k_reenter != 0) {
		//disp_str("!");
		return;
	}

	schedule();
}

//��ȡʱ���жϴ���
t_32 get_clock_ticks()
{
    return ticks;
}

//�ӳٺ���
void milli_delay(t_32 milli_sec)
{
	t_32 t = get_ticks();

	while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}

//����ʱ���ж�
void init_clock()
{
	// ��ʼ�� 8253 PIT
	out_byte(TIMER_MODE, RATE_GENERATOR);
	out_byte(TIMER0, (ut_8) (TIMER_FREQ/HZ) );
	out_byte(TIMER0, (ut_8) ((TIMER_FREQ/HZ) >> 8));
    
    // �趨ʱ���жϴ������
	put_irq_handler(CLOCK_IRQ, clock_handler);	
}
