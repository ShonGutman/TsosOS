#include "idt.h"
#include "../drivers/screen.h"

#define IDT_ENTRIES 256

#define GDT_CODE_SEGMENT_SELECTOR 0b00001000
#define FLAGS 0b10001110

idt_entry_struct idt_entries[IDT_ENTRIES];
idt_info_struct idt_info;

void set_idt_entry(const uint8 num, const uint32 offset)
{
    idt_entries[num].offset_low = offset & 0xffff;
    idt_entries[num].offset_high = (offset >> 16) & 0xffff;
    idt_entries[num].always0 = 0;
    idt_entries[num].segmentSelector = GDT_CODE_SEGMENT_SELECTOR;
    idt_entries[num].flags = FLAGS;
}

void idt_init()
{
    idt_info.size = sizeof(idt_entry_struct) * IDT_ENTRIES - 1;
    idt_info.offset = (uint32)&idt_entries;

    __asm__ ("lidt (%0)" : : "r"(&idt_info));
}

void isr_handler(interrupt_registers_struct regs)
{
    //later
}

void isr_install()
{
    //later
}
