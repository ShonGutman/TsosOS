#include "string.h"

void int_to_ascii(const int32 number, char* str) 
{
    uint32 i = 0;
    int32 temp = number;
    int8 negetive = 0;

    if (temp < 0)
    {
        temp *= -1;
        negetive = 1;
    } 

    do 
    {
        str[i++] = temp % 10 + '0';
    } 
    while ((temp /= 10) > 0);

    if (temp < 0)
    {
        str[i++] = '-';
    } 

    str[i] = STRING_TERMINATOR;
    reverse(str);
}

void reverse(char* str) 
{
    const uint32 size = strlen(str);
    for (uint32 i = 0; i < size / 2; i++) 
    {
        char temp = str[i];
        str[i] = str[size - i - 1];
        str[size - i - 1] = temp;
    }
}

uint32 strlen(const char* str) 
{
    uint32 i = 0;
    while (str[i] != STRING_TERMINATOR) 
    {
        i++;
    }
    return i;
}

void backspace(char* str) 
{
    uint32 len = strlen(str);
    str[len-1] = STRING_TERMINATOR;
}

void append(char* str, const char appended) 
{
    const uint32 len = strlen(str);
    str[len] = appended;
    str[len + 1] = STRING_TERMINATOR;
}



int32 strcmp(const char* str1, const char* str2) 
{
    while(*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}