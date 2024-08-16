#include "ports.h"

uint8 port_byte_in(const uint16 port)
{
    uint8 result = 0;

    /** 
     * Inline assembly instruction
     * source and destination registers are switched from NASM
     * 
     * "=a" (result) - it sets the value of result to the value of register al after the instruction
     * "d" (port) - puts value of port into register dx
     *
    */
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(const uint16 port, const uint8 data)
{
    // Both variables are used as input in the assembly instruction, no output needs to be stored.
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16 port_word_in(const uint16 port)
{
    uint16 result = 0;

    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(const uint16 port, const uint16 data)
{
        // Both variables are used as input in the assembly instruction, no output needs to be stored.
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
