#ifndef IRQ_H
#define IRQ_H

#include "headers/types.h"

/*
 * Handles IRQ (Interrupt Request) by sending an End of Interrupt (EOI) signal to the PICs 
 * and then calling the appropriate handler for the interrupt, if one is registered.
 *
 * @param regs A structure containing the register values at the time of the interrupt.
 *             This includes the interrupt number and other CPU state information.
 */
void irq_handler(interrupt_registers_struct regs);

/** The system timer generates regular interrupts at a constant rate to keep track of time */
extern void irq0();

/** This IRQ is triggered whenever a key is pressed or released on the keyboard. */
extern void irq1();

/** IRQ2 is used to cascade signals from the slave PIC to the master PIC. */
extern void irq2();

/** Handles interrupts from the second serial port (COM2). */
extern void irq3();

/** Handles interrupts from the first serial port (COM1). */
extern void irq4();

/** LPT2 (Parallel Port) or Sound Card */
extern void irq5();

/** Manages interrupts from the floppy disk controller, used when reading or writing to a floppy disk. */
extern void irq6();

/** Handles interrupts from the primary parallel port (LPT1) */
extern void irq7();

/** Handles interrupts from the real-time clock */
extern void irq8();

/** Free for peripherals / legacy SCSI / NIC. */
extern void irq9();

/** Free for peripherals / SCSI / NIC. */
extern void irq10();

/** Free for peripherals / SCSI / NIC. */
extern void irq11();

/** Handles interrupts from a PS/2-connected mouse. */
extern void irq12();

/** FPU / Coprocessor / Inter-processor. */
extern void irq13();

/** Primary ATA Hard Disk */
extern void irq14();

/** Secondary ATA Hard Disk */
extern void irq15();

#endif