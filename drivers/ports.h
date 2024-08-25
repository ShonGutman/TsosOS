#ifndef PORTS_H
#define PORTS_H

#include "headers/types.h"

/*
 * Reads a byte of data from the specified I/O port using the `in` assembly instruction.
 * The byte is read from the port number provided and returned to the caller.
 *
 * @param port The 16-bit I/O port number to read from. The value of this parameter 
 *             is moved into the `dx` register, which the `in` instruction uses.
 *
 * @return The byte of data read from the specified I/O port, returned as an 8-bit value.
 */
uint8 port_byte_in(const uint16 port);

/*
 * Sends a byte of data to the specified I/O port using the `out` assembly instruction.
 * The byte is written to the port number provided by the caller.
 *
 * @param port The 16-bit I/O port number to which the data will be sent. The value is moved 
 *             into the `dx` register, which is used by the `out` instruction.
 * 
 * @param data The 8-bit data to be sent to the specified I/O port. The value is moved 
 *             into the `al` register, which is used by the `out` instruction.
 */
void port_byte_out(const uint16 port, const uint8 data);

/*
 * Reads a word (2 bytes) of data from the specified I/O port using the `in` assembly instruction.
 * The word is read from the port number provided and returned to the caller.
 *
 * @param port The 16-bit I/O port number to read from. The value is moved 
 *             into the `dx` register, which is used by the `in` instruction.
 *
 * @return The word of data (16-bit) read from the specified I/O port, returned as a uint16 value.
 */
uint16 port_word_in(const uint16 port);

/*
 * Sends a word (2 bytes) of data to the specified I/O port using the `out` assembly instruction.
 * The word is written to the port number provided by the caller.
 *
 * @param port The 16-bit I/O port number to which the data will be sent. The value is moved 
 *             into the `dx` register, which is used by the `out` instruction.
 * 
 * @param data The 16-bit data to be sent to the specified I/O port. The value is moved 
 *             into the `ax` register, which is used by the `out` instruction.
 */
void port_word_out(const uint16 port, const uint16 data);

#endif