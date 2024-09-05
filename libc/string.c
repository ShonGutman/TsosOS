#include "string.h"


/**
 * K&R implementation
 */
void int_to_ascii(int n, char *str) 
{
    int i, sign;
    if ((sign = n) < 0)
    {
        n = -n;
    } 
    i = 0;
    do 
    {
        str[i++] = n % 10 + '0';
    } 
    while ((n /= 10) > 0);

    if (sign < 0)
    {
        str[i++] = '-';
    } 
    str[i] = STRING_TERMINATOR;
    reverse(str);
}

/* K&R */
void reverse(char *str) 
{
    int c, i, j;
    j = strlen(str)-1;
    for (i = 0; i < j; i++, j--) 
    {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

/* K&R */
int strlen(char *str) 
{
    int i = 0;
    while (str[i] != STRING_TERMINATOR) 
    {
        ++i;
    }
    return i;
}

void append(char *str, char n) 
{
    int len = strlen(str);
    str[len] = n;
    str[len+1] = STRING_TERMINATOR;
}

void backspace(char *str) 
{
    int len = strlen(str);
    str[len-1] = STRING_TERMINATOR;
}

/* K&R 
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int strcmp(const char *str1, const char *str2) 
{
    int i;
    for (i = 0; str1[i] == str2[i]; i++) 
    {
        if (str1[i] == STRING_TERMINATOR)
        {
            return 0;
        } 
    }
    return str1[i] - str2[i];
}