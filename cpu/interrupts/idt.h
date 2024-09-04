#ifndef IDT_H
#define IDT_H

#include "headers/types.h"

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
    uint32 offset;

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


#endif