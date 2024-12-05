#include "dma.h"
#include "drivers/ports/ports.h"

// DMA Controller Ports
#define DMA_START_ADDRESS_REGISTER              0x04 // Used to set the start address of the DMA buffer
#define DMA_COUNT_REGISTER_CHANNEL              0x05 // Used to specify the length of the transfer
#define DMA_EXTERNAL_PAGE_REGISTER              0x81
#define DMA_SINGLE_CHANNEL_MASK_REGISTER        0x0A
#define DMA_MODE_REGISTER                       0x0B // Configures the operating mode of the DMA controller
#define DMA_FLIP_FLOP_RESET_REGISTER            0x0C


static uint32 dma_buffer_address = 0;



/**********************************************************
 * Private kernel functions signiture                     *
 **********************************************************/

static void dma_init(const uint32 buffer_address);

static void dma_init_transfer(const uint16 length);

static void dma_write(const uint16 length);

static void dma_read(const uint16 length);

static uint8* dma_get_buffer();

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

void dma_read_to_buffer(const uint32 address, const uint16 length)
{
    dma_init(address);
    dma_read(length);
}

void dma_write_from_buffer(const uint32 address, const uint16 length)
{
    dma_init(address);
    dma_write(length);
}


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

/*
 * Initializes the DMA controller with a specified buffer address for data transfer.
 *
 * @param buffer_address The base address of the memory buffer that the DMA controller will use.
 */
static void dma_init(const uint32 buffer_address)
{
    dma_buffer_address = buffer_address;
}

/*
 * Configures a DMA channel for a data transfer.
 * This function sets up the DMA controller with the provided channel, selection flags, and transfer length.
 * It programs the DMA controller registers, including start address, transfer length, and mode.
 *
 * @param channel The DMA channel to be configured.
 * @param sel Additional mode settings for the transfer, combined with the base mode in `DMA_MODE_REGISTER`.
 * @param length The length of the data to be transferred, in bytes.
 */
static void dma_init_transfer(const uint16 length)
{
    //Credit: https://wiki.osdev.org/ISA_DMA

    /*
        Single Channel Mask Registers:

    +---------+---------+---------+---------+---------+---------+---------+---------+
    |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
    +---------+---------+---------+---------+---------+---------+---------+---------+
    |         |         |         |         |         | MASK_ON |  SEL_1  |  SEL_0  |
    +---------+---------+---------+---------+---------+---------+---------+---------+
    */

    // Tell DMA which channel we want to configure
    // 0x06 = 0b00000110
    port_byte_out(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x06);

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
}

static void dma_write(const uint16 length)
{
    dma_init_transfer(length);

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

    // 0x5a = 0b01011010
    port_byte_out(DMA_MODE_REGISTER, 0x5a);

    // Release channel
    port_byte_out(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

static void dma_read(const uint16 length)
{
    dma_init_transfer(length);

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

    // 0x56 = 0b01010110
    port_byte_out(DMA_MODE_REGISTER, 0x56);

    // Release channel
    port_byte_out(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

/*
 * Retrieves the buffer address currently configured for DMA transfers.
 *
 * @return A pointer to the memory buffer used for DMA transfers.
 */
static uint8* dma_get_buffer()
{
    return (uint8*)dma_buffer_address;
}
