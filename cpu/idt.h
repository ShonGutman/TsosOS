#ifndef IDT_H
#define IDT_H

#include "../headers/types.h"
#include "../libc/memory.h"

typedef struct
{

    uint16 offset_low;
    uint16 segmentSelector;
    uint8 always0;

    /**
     * bits 0-3: 4 bit value which defines the type of gate this interrupt descriptor represents
     * bit 4: set to 0 for interrupt gates
     * bit 5-6: 2 bit value that defines privilege levels
     * bit 7: present bit - must be set to 1 for descriptor to be valid
     */
    uint8 flags;
    uint16 offset_high;

} __attribute__((packed)) idt_entry_struct;

typedef struct 
{
    uint16 size;
    uint16 offset;

}__attribute__((packed)) idt_info_struct;


/*
 * Sets an entry in the Interrupt Descriptor Table (IDT) with the specified interrupt number and handler offset.
 *
 * @param num The interrupt number (or index) in the IDT where the entry will be set.
 * @param offset The 32-bit address of the interrupt handler function. This address is split into 
 *               two parts: the lower 16 bits (offset_low) and the upper 16 bits (offset_high).
 */
void set_idt_entry(const uint8 num, const uint32 offset);

/*
 * Initializes the IDT by setting the size and offset of the IDT structure.
 * The IDT size is calculated based on the number of entries and the size of each entry, 
 * and the offset is set to the address of the IDT entries array.
 */
void idt_init();




/**
 * ISR - Interrupt Service Routines
 * these are the function that are executed when an interrupt oocurs due to cpu exceptions
 */


/**This interrupt is triggered when the processor attempts to divide a number by zero */
extern void isr0(); /*Divide by zero*/

/**This interrupt is used for debugging purposes. 
 * It can be triggered intentionally by setting a breakpoint in the code or by hardware-based debugging tools. */
extern void isr1(); /*Debug exception*/

/**This interrupt is a high-priority interrupt that cannot be masked or ignored. 
 * It is typically used for critical hardware events like memory parity errors or hardware failures. */
extern void isr2(); /*Non-maskable interrupt (NMI)*/

/**this interrupt can be used for debugging purposes. 
 * It can be triggered by setting a breakpoint in the code. */
extern void isr3(); /*Breakpoint*/

/**his interrupt is triggered when an arithmetic operation results in an overflow, 
 * meaning the result is too large to be represented in the current data type. */
extern void isr4(); /*Overflow*/

/**This interrupt is triggered when a value is outside the specified bounds. */
extern void isr5(); /*Out of bounds*/

/**This interrupt is triggered when the processor encounters an invalid or unrecognized instruction. */
extern void isr6(); /*Invalid opcode*/

/**his interrupt is triggered when a device is not available or ready for use. */
extern void isr7(); /*Device not available*/

/**This interrupt is a critical exception that occurs 
 * when another exception happens while the processor is already handling an exception. */
extern void isr8(); /*Double fault*/

/**This interrupt is related to the floating-point coprocessor (FPU) 
 * and is triggered when the FPU attempts to access a segment that is not present or has incorrect permissions. */
extern void isr9(); /*Coprocessor segment overrun*/

/**This interrupt is triggered when the processor encounters an invalid Task State Segment (TSS), 
 * which is used for task switching. */
extern void isr10(); /*Invalid TSS*/

/**This interrupt is triggered when the processor attempts to access a memory segment that is not present in the memory management unit (MMU). */
extern void isr11(); /*Segment not present*/

/**This interrupt is triggered when the processor encounters an error related to the stack segment, 
 * such as a stack overflow or underflow. */
extern void isr12(); /*Stack segment fault*/

/**This is a general-purpose exception that can be triggered by various errors, 
 * such as invalid memory accesses, privilege violations, or incorrect use of system resources. */
extern void isr13(); /*General protection fault*/

/**This interrupt is triggered when the processor encounters a page fault, 
 * which occurs when a memory access attempts to reference a page that is not present in physical memory. */
extern void isr14(); /*Page fault*/


extern void isr15(); /*Reserved*/

/**This interrupt is triggered when the FPU encounters an error during floating-point operations */
extern void isr16(); /*Floating-point error*/

/**This interrupt is triggered when the processor detects an alignment error, 
 * which occurs when data is not accessed on the correct memory boundary. */
extern void isr17(); /*Alignment check*/

/**This interrupt is a critical exception that indicates a severe hardware error,
 * such as a processor or memory failure. */
extern void isr18(); /*Machine check*/

/*Reserved*/
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();


#endif