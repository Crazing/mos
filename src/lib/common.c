/*
 * File: /src/lib/common.c
 * Project: MOS
 * File Created: Sunday, 22nd April 2018 4:08:47 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Sunday, 29th April 2018 10:12:39 pm
 * Modified By: zhixiang
 * -----
 * FileContent: ����������
 */


#include "common.h"

//����ת�ַ���
t_8 * itoa(t_8 * str, ut_32 num)/* ����ǰ��� 0 ������ʾ����, ���� 0000B800 ����ʾ�� B800 */
{
	t_8 *	p = str;
	t_8	    ch;
	t_32	i;
	t_bool	flag = FALSE;

	*p++ = '0';
	*p++ = 'x';

	if(num == 0){
		*p++ = '0';
	}
	else{	
		for(i=28;i>=0;i-=4){
			ch = (num >> i) & 0xF;
			if(flag || (ch > 0)){
				flag = TRUE;
				ch += '0';
				if(ch > '9'){
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}


//��ӡ����
void disp_int(ut_32 input)
{
	t_8 output[16];
	itoa(output, input);
	disp_str(output);
}


