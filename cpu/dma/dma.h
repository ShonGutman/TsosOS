#ifndef DMA_H
#define DMA_H

#include "headers/types.h"

void dma_read_to_buffer(const uint32 address, const uint16 length);

void dma_write_from_buffer(const uint32 address, const uint16 length);

#endif