/*
 * File: /src/lib/string.c
 * Project: MOS
 * File Created: Wednesday, 25th April 2018 8:38:50 pm
 * Author: zhixiang (1115267126@qq.com)
 * -----
 * Last Modified: Tuesday, 1st May 2018 6:44:32 pm
 * Modified By: zhixiang
 * -----
 * FileContent: 字符串操作函数
 */


#include "string.h"

//子串查找
t_8*  strstr(const t_8* string, const t_8* search)
{
    t_32 string_len = strlen(string);
    t_32 search_len = strlen(search);
    
    //任一字符串长度为0，直接返回
    if((!string_len) || (!search_len))
        return NULL;

    //子串长度大于源串长度，直接返回
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