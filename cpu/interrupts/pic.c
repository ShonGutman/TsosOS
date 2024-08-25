#include "pic.h"

#define MASTER_PIC_COMMAND_PORT 0x20
#define MASTER_PIC_DATA_PORT 0X21

#define SLAVE_PIC_COMMAND_PORT 0xa0
#define SLAVE_PIC_DATA_PORT 0xa1


/**********************************************************
 * Private kernel functions signiture                     *
 **********************************************************/

void pic_remap(const uint32 master_offset, const uint32 slave_offset);


/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

void pic_init()
{
    /**
     * When the CPU boots, the PIC maps:
     * - IRQs 0-7 to interrupts 8-15.
     * - IRQs 8-15 to interrupts 112-119.
     * 
     * This causes conflicts with the CPU's ISRs (Interrupt Service Routines) which are mapped 
     * to interrupts 0-31. To avoid this, we remap the IRQs:
     * - IRQs 0-7 are remapped to interrupts 32-39.
     * - IRQs 8-15 are remapped to interrupts 40-47.
     */
    pic_remap(32, 40);
}


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

/*
 * Remaps the master and slave PIC (Programmable Interrupt Controller) to new interrupt offsets.
 * This is done to ensure that the IRQs do not overlap with the CPU's reserved interrupts (0-31).
 *
 * @param master_offset The new interrupt offset for the master PIC.
 * @param slave_offset  The new interrupt offset for the slave PIC.
 */
void pic_remap(const uint32 master_offset, const uint32 slave_offset)
{
    //send initalization sequence to the master command PIC and slave command PIC
    port_byte_out(MASTER_PIC_COMMAND_PORT, 0x11);
    port_byte_out(SLAVE_PIC_COMMAND_PORT, 0x11);

    //set master PIC offset
    port_byte_out(MASTER_PIC_DATA_PORT, master_offset);

    //set slave PIC offset
    port_byte_out(SLAVE_PIC_DATA_PORT, slave_offset);

    /**
     * Tell Master PIC that there is a slave PIC at IRQ2 (4 = 0000 0100)
     * 
     * Why we send 4?
     * each bit represnt one of the IRQs
     * most left bit represnt IRQ7 and most right bit represent IRQ0
     *     
    */
    port_byte_out(MASTER_PIC_DATA_PORT, 0x04);

    //Tell slave PIC about this role
    port_byte_out(SLAVE_PIC_DATA_PORT, 0x02);

    //we are in 8086 mode
    port_byte_out(MASTER_PIC_DATA_PORT, 0x01);
    port_byte_out(SLAVE_PIC_DATA_PORT, 0x01);

    //enable all IRQs
    port_byte_out(MASTER_PIC_DATA_PORT, 0);
    port_byte_out(SLAVE_PIC_DATA_PORT, 0);
}
