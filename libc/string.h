#ifndef STRINGS_H
#define STRINGS_H

#include "headers/types.h"

#define STRING_TERMINATOR '\0'


/*
 * turns an int into ascii
 * @param n - the number to turn to ascii
 * @param str - the str the number is going to be in
 */
void int_to_ascii(const int32 number, char* str);

/*
 * reverses a string
 * @param str - the str to be reveresd
 */
void reverse(char* str);

/*
 * returns string length
 * @param str - the str who's length will be checked
 * @return The length of the str
 */
uint32 strlen(const char* str);

/*
 * adds a back space to the end of a string 
 * @param str - the str
 * @return none
 */
void backspace(char *str);

/*
 * appends a string with a char
 * @param str - the str that is going to be appended
 * @param appended - the char that is going to be appended to the string 
 */
void append(char* str, const char appended);

/*
 * compares two strings and returns the result
 * @param str1- the first string
 * @param str2 - the second string
 * @return whether the strings size is the same and if the strings themselves are the same
 */
int32 strcmp(const char* str1, const char* str2);

#endif