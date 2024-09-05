#include "keyboard.h"
#include "ports.h"
#include "cpu/interrupts/isr.h"
#include "screen.h"
#include "cpu/interrupts/irq.h"
#include "libc/string.h"


#define MAX_KEYS 256
#define BACKSPACE 0x0E
#define ENTER 0x1C


#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};


static char key_buffer[MAX_KEYS];


static void keyboard_handler(interrupt_registers_struct regs) 
{
    /* The PIC leaves us the scancode in port 0x60 */
    uint8 scancode = port_byte_in(0x60);
    
    char letter = sc_ascii[(int)scancode];
    /* Remember that kprint only accepts char[] */
    char str[2] = {letter, STRING_TERMINATOR};
    append(key_buffer, letter);
    print(str);
    
}

void init_keyboard() 
{
    //keyboard is IRQ 1 
   irq_install_handler(1, keyboard_handler); 
}

