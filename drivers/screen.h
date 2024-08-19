#ifndef SCREEN_H
#define SCREEN_H

#include "ports.h"
#include "../libc/memory.h"

/*
 * Prints a null-terminated string to the `VGA` text buffer
 *
 * @param string The null-terminated string to be printed.
 */
void print(const char* string);

/*
 * Clears the entire `VGA` text buffer by setting every character cell to a space character 
 * with a white-on-black color attribute. After clearing, the cursor is reset to the 
 * beginning of the screen.
 */
void clear_screen();


#endif