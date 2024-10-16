#include "headers/types.h"
#include "drivers/screen.h"
#include "drivers/ports.h"
#include "cpu/interrupts/irq.h"
#include "timer.h"

#define FREQUENCY 100
#define TIMER_FREQUENCY_HZ 1193182

#define MODE_COMMAND_REGISTER 0x43
#define CHANNEL0_PORT 0x40

uint32 ticks = 0;

/**********************************************************
 * Private kernel functions signiture                     *
 **********************************************************/

void timer_handler(interrupt_registers_struct regs);


/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

void init_timer()
{
    // Install the timer handler for IRQ 0
    irq_install_handler(0, timer_handler);

    /**
     * Credit: https://wiki.osdev.org/Programmable_Interval_Timer
     * 
     * The Mode/Command register contains the following:
     * 
     *  Bits            Usage
     * 
     *  6 and 7         Select channel :
     *                      0 0 = Channel 0
     *                      0 1 = Channel 1
     *                      1 0 = Channel 2
     *                      1 1 = Read-back command
     * 
     *  4 and 5         Access mode :
     *                      0 0 = Latch count value command
     *                      0 1 = Access mode: low byte only
     *                      1 0 = Access mode: high byte only
     *                      1 1 = Access mode: low byte and high byte
     * 
     *  1 to 3          Operating mode :
     *                      0 0 0 = Mode 0 (interrupt on terminal count)
     *                      0 0 1 = Mode 1 (hardware re-triggerable one-shot)
     *                      0 1 0 = Mode 2 (rate generator)
     *                      0 1 1 = Mode 3 (square wave generator)
     *                      1 0 0 = Mode 4 (software triggered strobe)
     *                      1 0 1 = Mode 5 (hardware triggered strobe)
     *                      1 1 0 = Mode 2 (rate generator, same as 010b)
     *                      1 1 1 = Mode 3 (square wave generator, same as 011b)
     * 
     *  0               BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
     */
    port_byte_out(MODE_COMMAND_REGISTER, 0b00110110);

    const uint32 divisor = TIMER_FREQUENCY_HZ / FREQUENCY;
    const uint8 low = divisor & 0xff;
    const uint8 high = (divisor >> 8) & 0xff;
    port_byte_out(CHANNEL0_PORT, low);
    port_byte_out(CHANNEL0_PORT, high);
}


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

void timer_handler(interrupt_registers_struct regs)
{
    ticks++;
}