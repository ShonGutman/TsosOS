#ifndef PORTS_H
#define PORTS_H

#include "../headers/types.h"

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

void port_byte_out(const uint16 port, const uint8 data);

uint16 port_word_in(const uint16 port);

void port_word_out(const uint16 port, const uint16 data);

#endif