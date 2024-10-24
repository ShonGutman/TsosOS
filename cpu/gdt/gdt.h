#ifndef GDT_H
#define GDT_H   

#include "headers/types.h"

typedef struct
{
    uint16 length_0_to_15;
    uint16 base_0_to_15;
    uint8 base_16_to_23;

    uint8 type_flags : 4;
    uint8 type : 1;
    uint8 privilege_level : 2;
    uint8 present : 1;

    uint8 length_16_to_19 : 4;
    uint8 other_flags : 4;

    uint8 base_24_to_31;

}__attribute__((packed)) gdt_entry_struct;

typedef struct 
{
    uint16 size;
    uint32 offset;

}__attribute__((packed)) gdt_info_struct;

/*
 * Initializes the Global Descriptor Table (GDT) by setting up its size and offset,
 * and then loading it into the CPU using the `lgdt` assembly instruction.
 */
void init_gdt();


#endif