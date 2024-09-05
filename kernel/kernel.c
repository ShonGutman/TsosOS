#include "drivers/screen.h"
#include "cpu/interrupts/isr.h"
#include "cpu/timer/timer.h"
#include "drivers/keyboard.h"
#include "libc/string.h"
void main() 
{
    isr_install();

    //print project name and our name
    char* osName = "TSOS\nBy - Shon and Tedy\n";
    clear_screen();

    print(osName);
    //asm("int $2");
    init_timer();
    init_keyboard();
}

void procces_user_input(const char* input)
{
    if (!strcmp(input, "END"))
    {
        print("turning the TsosOS off");
        asm volatile("hlt");
    }
    print(input);
    print("\n > ");
}
