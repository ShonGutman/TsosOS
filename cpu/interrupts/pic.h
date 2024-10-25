#ifndef PIC_H
#define PIC_H

#include "headers/types.h"
#include "drivers/ports/ports.h"

/**
 * Source: https://wiki.osdev.org/8259_PIC#Programming_the_PIC_chips
 * 
 * What is PIC:
 * - PIC stands for Programmable Interrupt Controller.
 * - It is a critical component in the x86 architecture, enabling the system to be interrupt-driven.
 * 
 * Why PIC is important:
 * - Without a PIC, the system would need to constantly poll devices to check for events, leading to inefficiency.
 * - The PIC allows devices to signal the CPU only when they need attention, improving overall system performance.
 * 
 * Note:
 * - The Slave PIC is typically connected to the Master PIC via IRQ2.
 */


/*
 * Initializes the Programmable Interrupt Controller (PIC) by remapping the IRQs to avoid conflicts
 * with the CPU's existing interrupt vector table. The IRQs are remapped so that:
 * - IRQs 0-7 are mapped to interrupts 32-39.
 * - IRQs 8-15 are mapped to interrupts 40-47.
 */
void pic_init();

#endif