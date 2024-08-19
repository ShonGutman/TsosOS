#ifndef MEMORY_H
#define MEMORY_H

#include "../headers/types.h"


/*
 * Copies a block of memory from a source pointer to a destination pointer, for a specified number of bytes.
 *
 * @param dstptr Pointer to the destination memory where the data will be copied.
 * @param srcptr Pointer to the source memory from which the data will be copied.
 * @param size   The number of bytes to copy from the source to the destination.
 */
void memcopy(void* dstptr, const void* srcptr, const uint32 size);

#endif