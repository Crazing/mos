
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              console.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_TINIX_CONSOLE_H_
#define	_TINIX_CONSOLE_H_


/* CONSOLE */
typedef struct s_console
{
	//struct s_tty*	p_tty;
	unsigned int	current_start_addr;	/* ��ǰ��ʾ����ʲôλ��   */
	unsigned int	original_addr;		/* ��ǰ����̨��Ӧ�Դ�λ�� */
	unsigned int	v_mem_limit;		/* ��ǰ����̨ռ���Դ��С */
	unsigned int	cursor;			/* ��ǰ���λ�� */
}CONSOLE;


#define DEFAULT_CHAR_COLOR	0x07	/* 0000 0111 �ڵװ��� */


#endif /* _TINIX_CONSOLE_H_ */
