#include "dma.h"
#include "drivers/ports/ports.h"

// DMA Controller Ports
#define DMA_START_ADDRESS_REGISTER              0x04 
#define DMA_COUNT_REGISTER_CHANNEL              0x05
#define DMA_EXTERNAL_PAGE_REGISTER              0x81
#define DMA_SINGLE_CHANNEL_MASK_REGISTER        0x0A
#define DMA_MODE_REGISTER                       0x0B
#define DMA_FLIP_FLOP_RESET_REGISTER            0x0C


static uint32 dma_buffer_address = 0;

void dma_init(const uint32 buffer_address)
{
    dma_buffer_address = buffer_address;
}

void dma_init_transfer(const uint8 channel, const uint8 sel, const uint16 length)
{
    //Credit: https://wiki.osdev.org/ISA_DMA

    // Tell DMA which channel we want to configure
    port_byte_out(DMA_SINGLE_CHANNEL_MASK_REGISTER, channel);

    // Reset Flip-Flop register
    port_byte_out(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Set buffer to the specified address
    port_byte_out(DMA_START_ADDRESS_REGISTER, LOW_BYTE(dma_buffer_address));
    port_byte_out(DMA_START_ADDRESS_REGISTER, HIGH_BYTE(dma_buffer_address));

    // Reset Flip-Flop register
    port_byte_out(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    port_byte_out(DMA_COUNT_REGISTER_CHANNEL, LOW_BYTE(length - 1));
    port_byte_out(DMA_COUNT_REGISTER_CHANNEL, HIGH_BYTE(length - 1));

    // We don't want to have external page register
    port_byte_out(DMA_EXTERNAL_PAGE_REGISTER, 0);

    // | MOD1 | MOD0 | DOWN | AUTO | TRA1 | TRA0 | SEL1 | SEL0 |
    // |  0   |  1   |  0   |  1   |  x   |  x   |  1   |  0   | 
    // MOD1, MOD0 - mode
    //  00 - transfer on demand
    //  01 - single DMA transfer
    //  10 - block DMA transfer
    //  11 - cascade mode (with another DMA controller)
    // DOWN - set order of data
    // AUTO - reset address after transfer if set
    // TRA1, TRA0 - transfer type
    //  00 - self test of the controller
    //  01 - reading from memory
    //  10 - writing to memory
    //  11 - invalid
    // SEL0, SEL1 - channel to change

    // 0x52 = 0b01010010
    port_byte_out(DMA_MODE_REGISTER, 0x52 | sel);

    // Release channel
    port_byte_out(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

uint8* dma_get_buffer()
{
    return (uint8*)dma_buffer_address;
}
