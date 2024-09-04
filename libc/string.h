#ifndef STRINGS_H
#define STRINGS_H

/*
 *  turns an int into ascii
 * @param n - the number to turn to ascii
 * @param str - the str the number is going to be in
 * @return The str with the number in it 
 */
void int_to_ascii(int n, char str[]);

/*
 *  reverses a string
 * @param s - the str to be reveresd
 * @return The reversed str  
 */
void reverse(char s[]);

/*
 *  checks string lenght
 * @param s - the str whos lenght will be checked
 * @return The lenght of the str
 */
int strlen(char s[]);

/*
 *  adds a back space to the end of a string 
 * @param str - the str
 * @return The str with a backspace 
 */
void backspace(char s[]);

/*
 *  appends a string with a char
 * @param str - the str that is going to be appended
 * @param n - the char that is going to be appended to the string 
 * @return The appended str 
 */
void append(char s[], char n);

/*
 *  compares two strings and returns the result
 * @param s1- the first string
 * @param s2 - the second string
 * @return wether the strings size is the same and if the strings themselves are the same
 */
int strcmp(char s1[], char s2[]);

#endif