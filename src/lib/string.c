/*
 * File: /src/lib/string.c
 * Project: MOS
 * File Created: Wednesday, 25th April 2018 8:38:50 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 1st May 2018 6:44:32 pm
 * Modified By: zhixiang
 * -----
 * FileContent: �ַ�����������
 */


#include "string.h"

//�Ӵ�����
t_8*  strstr(const t_8* string, const t_8* search)
{
    t_32 string_len = strlen(string);
    t_32 search_len = strlen(search);
    
    //��һ�ַ�������Ϊ0��ֱ�ӷ���
    if((!string_len) || (!search_len))
        return NULL;

    //�Ӵ����ȴ���Դ�����ȣ�ֱ�ӷ���
    if(search_len > string_len)
        return NULL;
    
    t_32 diff = string_len - search_len;
    t_32 last = 0;
    while(last <= diff)
    {
        for(t_32 i = 0; *(string + i) == *(search + i);)
        {
            ++i;
            if(i == search_len)
                return (t_8*)string; 
        }
        ++string;
        ++last;
    }
    return NULL;
}