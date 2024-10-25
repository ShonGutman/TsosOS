#include "drivers/screen/screen.h"
#include "cpu/interrupts/isr.h"
#include "cpu/timer/timer.h"
#include "cpu/gdt/gdt.h"
#include "drivers/keyboard/keyboard.h"
#include "libc/string.h"

void main() 
{
    init_gdt();
    isr_install();

    //print project name and our name
    char* osName = "TSOS\nBy - Shon and Tedy\n> ";
    clear_screen();

    print(osName);
    init_timer();
    init_keyboard();

}


