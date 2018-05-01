/*
 * File: /src/tty/keyboard.c
 * Project: MOS
 * File Created: Monday, 23rd April 2018 12:43:42 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 1st May 2018 8:17:51 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 键盘输入处理程序
 */


#include "keyboard.h"

static	KB_INPUT	kb_in;
static	t_bool		code_with_E0 = FALSE;
static	t_bool		shift_l      = FALSE;		/* l shift state	*/
static	t_bool		shift_r      = FALSE;		/* r shift state	*/
static	t_bool		alt_l        = FALSE;	    /* l alt state		*/
static	t_bool		alt_r        = FALSE;		/* r left state		*/
static	t_bool		ctrl_l       = FALSE;		/* l ctrl state		*/
static	t_bool		ctrl_r       = FALSE;	    /* l ctrl state		*/
static	t_bool		caps_lock    = FALSE;		/* Caps Lock		*/
static	t_bool		num_lock     = FALSE;		/* Num Lock		    */
static	t_bool		scroll_lock  = FALSE;		/* Scroll Lock		*/
static	t_32		column       = 0;	        /* keyrow[column] 将是 keymap 中某一个值 */

//num键影响到的小键盘按键的san码
static  t_8         pad[]         = {0x47,0x48,0x49,0x4B,0x4C,0x4D,0x4F,0x50,0x51,0x52,0x53,0x0};

//等待直到8042的缓冲区为空
static void kb_wait()	/* 等待 8042 的输入缓冲区空 */
{
	t_8 kb_stat;

	do {
		kb_stat = in_byte(KB_CMD);
	} while (kb_stat & 0x02);
}

//读取8042的返回码
static void kb_ack()
{
	t_8 kb_read;

	do {
		kb_read = in_byte(KB_DATA);
	} while (kb_read =! KB_ACK);
}

//设置LED灯
static void set_leds()
{
	t_8 leds = (caps_lock << 2) | (num_lock << 1) | scroll_lock;

	kb_wait();
	out_byte(KB_DATA, LED_CODE);
	kb_ack();

	kb_wait();
	out_byte(KB_DATA, leds);
	kb_ack();
}

//从键盘输入缓冲区获取一个字符
static ut_8 get_byte_from_kb_buf()	/* 从键盘缓冲区中读取下一个字节 */
{
	ut_8	scan_code;

	while (kb_in.count <= 0) {}	/* 等待下一个字节到来 */

	disable_int();
	scan_code = *(kb_in.p_tail);
	kb_in.p_tail++;
	if (kb_in.p_tail == kb_in.buf + KB_IN_BYTES) {
		kb_in.p_tail = kb_in.buf;
	}
	kb_in.count--;
	enable_int();
	return scan_code;
}

//从键盘输入缓冲区读取字符，转换成系统可以识别的表示符
t_bool keyboard_read(ut_32* code)
{
	ut_8	scan_code;
	t_bool	make;	/* TRUE : make  */
			        /* FALSE: break */
	ut_32	key = 0;/* 用一个整型来表示一个键。 */
			        /* 比如，如果 Home 被按下，则 key 值将为定义在 keyboard.h 中的 'HOME'。*/
	ut_32*	keyrow;	/* 指向 keymap[] 的某一行 */
    t_bool  ret = FALSE;

	if(kb_in.count > 0){
		code_with_E0 = FALSE;
		scan_code = get_byte_from_kb_buf();
		/* 下面开始解析扫描码 */
		if (scan_code == 0xE1) {
			t_32 i;
			ut_8 pausebreak_scan_code[] = {0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5};
			t_bool is_pausebreak = TRUE;
			for(i=1;i<6;i++){
				if (get_byte_from_kb_buf() != pausebreak_scan_code[i]) {
					is_pausebreak = FALSE;
					break;
				}
			}
			if (is_pausebreak) {
				key = PAUSEBREAK;
			}
		}
		else if (scan_code == 0xE0) {
			code_with_E0 = TRUE;
			scan_code = get_byte_from_kb_buf();

			/* PrintScreen 被按下 */
			if (scan_code == 0x2A) {
				code_with_E0 = FALSE;
				if ((scan_code = get_byte_from_kb_buf()) == 0xE0) {
					code_with_E0 = TRUE;
					if ((scan_code = get_byte_from_kb_buf()) == 0x37) {
						key = PRINTSCREEN;
						make = TRUE;
					}
				}
			}
			/* PrintScreen 被释放 */
			else if (scan_code == 0xB7) {
				code_with_E0 = FALSE;
				if ((scan_code = get_byte_from_kb_buf()) == 0xE0) {
					code_with_E0 = TRUE;
					if ((scan_code = get_byte_from_kb_buf()) == 0xAA) {
						key = PRINTSCREEN;
						make = FALSE;
					}
				}
			}
		} /* 如果不是 PrintScreen。则此时 scan_code 为 0xE0 紧跟的那个值。 */
		if ((key != PAUSEBREAK) && (key != PRINTSCREEN)) {
			/* 首先判断Make Code 还是 Break Code */
			make = (scan_code & FLAG_BREAK ? FALSE : TRUE);
			
			/* 先定位到 keymap 中的行 */
			keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];

			column = 0;

            //num键只对小键盘的某一范围内的按键起作用
            if(strchr(pad, (scan_code & 0x7F)))
            {
                if ((shift_l || shift_r) ^ num_lock) 
				    column = 1;
            }
            else
            {
                if ((shift_l || shift_r) ^ caps_lock) 
				    column = 1;
            }

			if (code_with_E0) {             //shift、caps、num不会对E0开始的扫描码产生影响
				column = 2;
			}
            
			key = keyrow[column];

			switch(key) {
			case SHIFT_L:
				shift_l	= make;
				break;
			case SHIFT_R:
				shift_r	= make;
				break;
			case CTRL_L:
				ctrl_l	= make;
				break;
			case CTRL_R:
				ctrl_r	= make;
				break;
			case ALT_L:
				alt_l	= make;
				break;
			case ALT_R:
				alt_l	= make;
				break;
            case CAPS_LOCK:
				if (make) {
					caps_lock   = !caps_lock;
					set_leds();
				}
				break;
			case NUM_LOCK:
				if (make) {
					num_lock    = !num_lock;
					set_leds();
				}
				break;
			case SCROLL_LOCK:
				if (make) {
					scroll_lock = !scroll_lock;
					set_leds();
				}
				break;
			default:
				break;
			}
		}

		if(make){ /* 忽略 Break Code */
			key |= shift_l	    ? FLAG_SHIFT_L	: 0;
			key |= shift_r	    ? FLAG_SHIFT_R	: 0;
			key |= ctrl_l	    ? FLAG_CTRL_L	: 0;
			key |= ctrl_r	    ? FLAG_CTRL_R	: 0;
			key |= alt_l	    ? FLAG_ALT_L	: 0;
			key |= alt_r	    ? FLAG_ALT_R	: 0;
            key |= caps_lock    ? FLAG_CAPS	    : 0;
            key |= num_lock	    ? FLAG_NUM	    : 0;
            key |= scroll_lock	? FLAG_SCROLL	: 0;

			*code = key;
            ret   = TRUE;
		}
	}

    return ret;
}

//键盘中断处理函数
void keyboard_handler(t_32 irq)
{
	ut_8 scan_code = in_byte(KB_DATA);

	if (kb_in.count < KB_IN_BYTES) {
		*(kb_in.p_head) = scan_code;
		kb_in.p_head++;
		if (kb_in.p_head == kb_in.buf + KB_IN_BYTES) {
			kb_in.p_head = kb_in.buf;
		}
		kb_in.count++;
	}
}

//初始化键盘中断
void init_keyboard()
{
	kb_in.count  = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;

	put_irq_handler(KEYBOARD_IRQ, keyboard_handler);	/* 设定键盘中断处理程序 */
}