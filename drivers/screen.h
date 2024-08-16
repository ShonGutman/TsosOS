#ifndef SCREEN_H
#define SCREEN_H

#include "ports.h"

/*
 * Retrieves the current cursor position in the `VGA` text buffer by querying the `VGA` controller.
 * The cursor position is returned as an offset in the `VGA` memory. Each cell in `VGA` memory 
 * consists of a character and its associated color attribute, so the offset is multiplied by 2.
 *
 *
 * @return The offset in the `VGA` text buffer where the cursor is currently located, 
 *         multiplied by 2 because each cell in the `VGA` memory is 2 bytes.
 */
uint32 get_cursor_offset();


#endif