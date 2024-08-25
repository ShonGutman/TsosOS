#include "drivers/screen.h"
#include "cpu/interrupts/isr.h"

void main() 
{
    isr_install();

    //print project name and our name
    char* osName = "TSOS\nBy - Shon and Tedy\n";
    clear_screen();

    print(osName);


}