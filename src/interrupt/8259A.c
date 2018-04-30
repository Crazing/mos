/*
 * File: /src/interrupt/8259A.c
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 11:15:51 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Monday, 30th April 2018 5:44:07 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 8259A�������
 */


#include "8259A.h"

//8259A�жϴ����
t_pf_irq_handler	irq_table[NR_IRQ] = {NULL};

//8259AĬ�ϵ��жϴ������
void spurious_irq(t_32 irq)
{
	disp_str("spurious_irq: ");
	disp_int(irq);
	disp_str("\n");
}

//����irq�Ĵ�����
void put_irq_handler(t_32 irq, t_pf_irq_handler handler)
{
	disable_irq(irq);
	irq_table[irq] = handler;
}

//��ʼ��8259A
void init_8259A()
{
   	out_byte(INT_M_CTL,	0x11);			        // Master 8259, ICW1.
	out_byte(INT_S_CTL,	0x11);			        // Slave  8259, ICW1.
	out_byte(INT_M_CTLMASK,	INT_VECTOR_IRQ0);	// Master 8259, ICW2. ���� '��8259' ���ж���ڵ�ַΪ 0x20.
	out_byte(INT_S_CTLMASK,	INT_VECTOR_IRQ8);	// Slave  8259, ICW2. ���� '��8259' ���ж���ڵ�ַΪ 0x28
	out_byte(INT_M_CTLMASK,	0x4);			    // Master 8259, ICW3. IR2 ��Ӧ '��8259'.
	out_byte(INT_S_CTLMASK,	0x2);			    // Slave  8259, ICW3. ��Ӧ '��8259' �� IR2.
	out_byte(INT_M_CTLMASK,	0x1);			    // Master 8259, ICW4.
	out_byte(INT_S_CTLMASK,	0x1);			    // Slave  8259, ICW4.

	out_byte(INT_M_CTLMASK,	0xFF);	            // Master 8259, OCW1. 
	out_byte(INT_S_CTLMASK,	0xFF);	            // Slave  8259, OCW1. 

	t_32 i;
	for(i=0;i<NR_IRQ;i++){
		irq_table[i] = spurious_irq;
	} 
}

//����8259A
void set_8259A()
{
    //����ʱ���ж� 
    init_clock();
    
    //���ü����ж�
    init_keyboard();
    
}

//��8259A
void open_8259A()
{
    //��8259A���Խ���ʱ���ж�
    enable_irq(CLOCK_IRQ);

    //��8259A���Խ��ռ����ж�
    enable_irq(KEYBOARD_IRQ);

}