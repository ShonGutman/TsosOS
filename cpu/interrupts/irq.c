#include "irq.h"
#include "drivers/ports/ports.h"

#define TOTAL_IRQ 16

#define MASTER_PIC_COMMAND_PORT 0x20
#define SLAVE_PIC_COMMAND_PORT 0xa0

#define EOI 0x20

//this is an array of functions with this signature: void funcName(interrupt_registers_struct);
void (*irq_routines[TOTAL_IRQ])(interrupt_registers_struct) = 
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void irq_handler(interrupt_registers_struct regs)
{
    /** After every interrupt we need to send and EOI (end of interrupt) to the PICs
     * or they will not send another interrupt again
     */

    if(regs.int_number >= 40)
    {
        port_byte_out(SLAVE_PIC_COMMAND_PORT, EOI);
    }

    port_byte_out(MASTER_PIC_COMMAND_PORT, EOI);

    void (*handler)(interrupt_registers_struct);

    handler = irq_routines[regs.int_number - 32];

    //check if handler exists
    if(handler)
    {
        handler(regs);
    }
}

void irq_install_handler(const uint8 irqNumber, void (*handler)(interrupt_registers_struct regs))
{
    irq_routines[irqNumber] = handler;
}
