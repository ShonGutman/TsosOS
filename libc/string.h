#ifndef STRINGS_H
#define STRINGS_H

#define STRING_TERMINATOR '\0'
#include "headers/types.h"
/*
 * turns an int into ascii
 * @param n - the number to turn to ascii
 * @param str - the str the number is going to be in
 * @return none
 */
void int_to_ascii(uint32 number, char *str);

/*
 * reverses a string
 * @param str - the str to be reveresd
 * @return none
 */
void reverse(char *str);

/*
 * returns string length
 * @param str - the str who's length will be checked
 * @return The length of the str
 */
uint32 strlen(char *str);

/*
 * adds a back space to the end of a string 
 * @param str - the str
 * @return none
 */
void backspace(char *str);

/*
 * appends a string with a char
 * @param str - the str that is going to be appended
 * @param n - the char that is going to be appended to the string 
 * @return none
 */
void append(char *str, char n);

/*
 * compares two strings and returns the result
 * @param str1- the first string
 * @param str2 - the second string
 * @return wether the strings size is the same and if the strings themselves are the same
 */
uint32 strcmp(const char *str1, const char *str2);

#endif