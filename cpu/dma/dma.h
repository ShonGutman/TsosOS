#ifndef DMA_H
#define DMA_H

#include "headers/types.h"

/*
 * Initializes the DMA controller with a specified buffer address for data transfer.
 *
 * @param buffer_address The base address of the memory buffer that the DMA controller will use.
 */
void dma_init(const uint32 buffer_address);

/*
 * Configures a DMA channel for a data transfer.
 * This function sets up the DMA controller with the provided channel, selection flags, and transfer length.
 * It programs the DMA controller registers, including start address, transfer length, and mode.
 *
 * @param channel The DMA channel to be configured.
 * @param sel Additional mode settings for the transfer, combined with the base mode in `DMA_MODE_REGISTER`.
 * @param length The length of the data to be transferred, in bytes.
 */
void dma_init_transfer(const uint8 channel, const uint8 sel, const uint16 length);

/*
 * Retrieves the buffer address currently configured for DMA transfers.
 *
 * @return A pointer to the memory buffer used for DMA transfers.
 */
uint8* dma_get_buffer();

#endif