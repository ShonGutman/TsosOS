#include "screen.h"


/**
 * small explenation about VGA:
 *  
 *      The VGA has many internal registers, but only a few I/O ports.
 * 
 * Port Functions:
 *      0x3D4 is the "index" or "address" port
 *      0x3D5 is the "data" port
 * 
 * How it works:
 *      Writing to 0x3D4 selects which internal register you want to access.
 *      Reading or writing to 0x3D5 then accesses the data in the selected register.
 */

/* Screen I/O ports */
#define VGA_CONTROL_REFISTER_PORT 0x3d4
#define VGA_DATA_REFISTER_PORT 0x3d5

/* request index of register out of all VGA possible registers */
#define CURSER_HIGH_LOCATION 0x0e
#define CURSER_LOW_LOCATION 0x0f


uint32 get_cursor_offset()
{
    // Request the high byte of the cursor position
    port_byte_out(VGA_CONTROL_REFISTER_PORT, CURSER_HIGH_LOCATION);

    uint32 position = port_byte_in(VGA_DATA_REFISTER_PORT);

    position = position << 8; // Shift left by one byte to position the high byte correctly

    // Request the low byte of the cursor position
    port_byte_out(VGA_CONTROL_REFISTER_PORT, CURSER_LOW_LOCATION);

    position |= port_byte_in(VGA_DATA_REFISTER_PORT);

    /* VGA cells consist of a character and its color attribute */
    return position * 2;
}