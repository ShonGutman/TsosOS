#include "string.h"

void int_to_ascii(uint32 number, char* str) 
{
    uint32 i = 0;
    uint32 sign = 0;
    if ((sign = number) < 0)
    {
        number = -number;
    } 
    i = 0;
    do 
    {
        str[i++] = number % 10 + '0';
    } 
    while ((number /= 10) > 0);

    if (sign < 0)
    {
        str[i++] = '-';
    } 
    str[i] = STRING_TERMINATOR;
    reverse(str);
}

void reverse(char* str) 
{
    uint32 c = 0;
    uint32 i = 0;
    uint32 j = strlen(str)-1;;
    for (i = 0; i < j; i++, j--) 
    {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

uint32 strlen(char* str) 
{
    uint32 i = 0;
    while (str[i] != STRING_TERMINATOR) 
    {
        ++i;
    }
    return i;
}

void append(char* str, char n) 
{
    uint32 len = strlen(str);
    str[len] = n;
    str[len+1] = STRING_TERMINATOR;
}

void backspace(char* str) 
{
    uint32 len = strlen(str);
    str[len-1] = STRING_TERMINATOR;
}


/* Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
uint32 strcmp(const char* str1, const char* str2) 
{
    uint32 i = 0;
    for (i = 0; str1[i] == str2[i]; i++) 
    {
        if (str1[i] == STRING_TERMINATOR)
        {
            return 0;
        } 
    }
    return str1[i] - str2[i];
}