#include "keyboard.h"
#include "ports.h"
#include "cpu/interrupts/isr.h"
#include "screen.h"
#include "cpu/interrupts/irq.h"
#include "libc/string.h"

#define MAX_KEYS 256
#define BACKSPACE 0x0E
#define ENTER 0x1C
#define CAPSLOCK 0x3A
#define SPACE_ASCII ' '
#define CAPPED_TO_NORMAL_ASCII 32
//all of the shift
#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define SHIFT_RELEASED_LEFT 0xAA
#define SHIFT_RELEASED_RIGHT 0xB6

#define ASCII_ABC_END 90
#define ASCII_ABC_START 65

#define SCANCODE_MAX 58

const char scancode_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

//when shift is pressed the characters to be printed are different
const char scancode_ascii_shifted[] = { '?', '?', '!', '@', '#', '$', '%', '^', 
    '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
    'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G', 
    'H', 'J', 'K', 'L', ':', '\"', '~', '?', '|', 'Z', 'X', 'C', 'V', 
    'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};

/**
 * The keyboard hardware communicates with the operating system by sending
 * a scancode.
 * 
 * -------------------------------------
 * 
 * A keyboard scancode is the raw data generated by the keyboard 
 * representing the key's physical location on the keyboad.
 * 
 * -------------------------------------
 * 
 * The functionality of the following code is:
 * 
 * 1: get the scancode through the pic on port 0x60
 * 
 * 2: turn that scancode into a printable character
 * 
 * 3: if the character isnt special we add him into a key buffer (256 in size)
 * and print that character.
 * if that character is backspcae we remove the last one
 * if that character is ENTER we send the entire key buffer to a procces_input function
 * 
 *  -------------------------------------
 */

//maximum amount of chars that can be inputed in one line.

static char key_buffer[MAX_KEYS];

int capslock_value_change = CAPPED_TO_NORMAL_ASCII;
int shift_pressed = 0;

void keyboard_handler(interrupt_registers_struct regs) 
{
    /* The PIC leaves us the scancode in port 0x60 */
    uint8 scancode = port_byte_in(0x60);
    //if scancode is unknow then return 
    if (scancode > SCANCODE_MAX && scancode != SHIFT_RELEASED_LEFT && scancode != SHIFT_RELEASED_RIGHT)
    {
        return;
    }

    
    // Track shift key press and release
    if (scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT) 
    {
        shift_pressed = 1;
        return;
    }
    else if (scancode == SHIFT_RELEASED_LEFT || scancode == SHIFT_RELEASED_RIGHT) 
    {
        shift_pressed = 0;
        return;
    }


    //change character value based on the amount of times capslock was pressed
    if (scancode == CAPSLOCK)
    {
        if (capslock_value_change == 0)
        {
            capslock_value_change += CAPPED_TO_NORMAL_ASCII;
        }
        else
        {
            capslock_value_change = 0;
        }
        
    }
    else if (scancode == BACKSPACE) 
    {
        //check that there are characters to delete
        if (key_buffer[0] != 0)
        {
            backspace(key_buffer);
            print_backspace();
        }
    } 
    else if (scancode == ENTER) 
    {
        if (key_buffer[0] != STRING_TERMINATOR)
        {
            print("\n");
        }
        //kernel function to handle the user input
        procces_user_input(key_buffer);
        //reset the buffer sowe would be able to use it again 
        key_buffer[0] = STRING_TERMINATOR;
    } 
    else 
    {
        char letter = 0;
        if (shift_pressed)
        {
            letter = scancode_ascii_shifted[(int)scancode];
        }
        else
        {
            letter = scancode_ascii[(int)scancode];
        }
        // we dont want to change anything other than letters
        if (letter != SPACE_ASCII && !shift_pressed && letter >= ASCII_ABC_START && letter <= ASCII_ABC_END)
        {
            letter += capslock_value_change;
        }
        char str[2] = {letter, STRING_TERMINATOR};
        append(key_buffer, letter);
        print(str);
    }
    
}

void init_keyboard() 
{
    //keyboard is IRQ 1 
   irq_install_handler(1, keyboard_handler); 
}

void procces_user_input(const char* input)
{
    if (!strcmp(input, "END"))
    {
        print("turning TsosOS off");
        asm volatile("hlt");
    }
    print(input);
    print("\n > ");
}