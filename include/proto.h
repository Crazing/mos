
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            proto.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* klib.asm */
PUBLIC void	out_byte(t_port port, t_8 value);
PUBLIC t_8	in_byte(t_port port);
PUBLIC void	disable_int();
PUBLIC void	enable_int();
PUBLIC void	disp_str(char * info);
PUBLIC void	disp_color_str(char * info, int color);

/* protect.c */
PUBLIC void	init_prot();
PUBLIC t_32	seg2phys(t_16 seg);
PUBLIC void	disable_irq(int irq);
PUBLIC void	enable_irq(int irq);

/* klib.c */
PUBLIC t_bool	is_alphanumeric(char ch);
PUBLIC void	delay(int time);

/* kernel.asm */
PUBLIC void	restart();

/* main.c */
PUBLIC void	TestA();
PUBLIC void	TestB();
PUBLIC void	TestC();

/* i8259.c */
PUBLIC void	put_irq_handler(int iIRQ, t_pf_irq_handler handler);
PUBLIC void	spurious_irq(int irq);

/* clock.c */
PUBLIC void	clock_handler(int irq);
PUBLIC void	milli_delay(int milli_sec);

/* proc.c */
PUBLIC void	schedule();

/* keyboard.c */
PUBLIC void	keyboard_handler(int irq);
PUBLIC void	keyboard_read(TTY* p_tty);

/* tty.c */
PUBLIC void	task_tty();
PUBLIC void	in_process(TTY* p_tty, t_32 key);

/* console.c */
PUBLIC void	out_char(CONSOLE* p_con, char ch);
PUBLIC t_bool	is_current_console(CONSOLE* p_con);



/************************************************************************/
/*                        ������ϵͳ�������                            */
/************************************************************************/

/* proc.c */
PUBLIC	int	sys_get_ticks();	/* t_sys_call */

/* syscall.asm */
PUBLIC	void	sys_call();		/* t_pf_int_handler */
PUBLIC	int	get_ticks();

