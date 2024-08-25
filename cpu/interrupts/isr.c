#include "isr.h"
#include "idt.h"
#include "pic.h"
#include "drivers/screen.h"

void isr_handler(interrupt_registers_struct regs)
{
    print("interrupt occured! system hlts");
    while(1);
}

void isr_install()
{
    set_idt_entry(0, (uint32)isr0);
    set_idt_entry(1, (uint32)isr1);
    set_idt_entry(2, (uint32)isr2);
    set_idt_entry(3, (uint32)isr3);
    set_idt_entry(4, (uint32)isr4);
    set_idt_entry(5, (uint32)isr5);
    set_idt_entry(6, (uint32)isr6);
    set_idt_entry(7, (uint32)isr7);
    set_idt_entry(8, (uint32)isr8);
    set_idt_entry(9, (uint32)isr9);
    set_idt_entry(10, (uint32)isr10);
    set_idt_entry(11, (uint32)isr11);
    set_idt_entry(12, (uint32)isr12);
    set_idt_entry(13, (uint32)isr13);
    set_idt_entry(14, (uint32)isr14);
    set_idt_entry(15, (uint32)isr15);
    set_idt_entry(16, (uint32)isr16);
    set_idt_entry(17, (uint32)isr17);
    set_idt_entry(18, (uint32)isr18);
    set_idt_entry(19, (uint32)isr19);
    set_idt_entry(20, (uint32)isr20);
    set_idt_entry(21, (uint32)isr21);
    set_idt_entry(22, (uint32)isr22);
    set_idt_entry(23, (uint32)isr23);
    set_idt_entry(24, (uint32)isr24);
    set_idt_entry(25, (uint32)isr25);
    set_idt_entry(26, (uint32)isr26);
    set_idt_entry(27, (uint32)isr27);
    set_idt_entry(28, (uint32)isr28);
    set_idt_entry(29, (uint32)isr29);
    set_idt_entry(30, (uint32)isr30);
    set_idt_entry(31, (uint32)isr31);

    pic_init();
    idt_init();
}