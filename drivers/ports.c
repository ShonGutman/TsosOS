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
    */
    __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}
