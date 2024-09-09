#include "screen.h"


/**
 * small explenation about VGA:
 *  
 *      The VGA has many internal registers, but only a few I/O ports.
 * 
 * Port Functions:
 *      0x3D4 is the "index" or "address" port
 *      0x3D5 is the "data" port
 * 
 * How it works:
 *      Writing to 0x3D4 selects which internal register you want to access.
 *      Reading or writing to 0x3D5 then accesses the data in the selected register.
 */

/* Screen I/O ports */
#define VGA_CONTROL_REFISTER_PORT 0x3d4
#define VGA_DATA_REFISTER_PORT 0x3d5

/* request index of register out of all VGA possible registers */
#define CURSER_HIGH_LOCATION 0x0e
#define CURSER_LOW_LOCATION 0x0f

#define VIDEO_MEMORY 0xb8000
#define WHITE_ON_BLACK 0x0f

#define MAX_ROWS 25
#define MAX_COLS 80

#define SPACE_ASCII ' '
#define BACKSPACE_ASCII 0x08

/**********************************************************
 * Private kernel functions signiture                     *
 **********************************************************/

static uint32 get_cursor_offset();

static void set_cursor_offset(const uint32 offset);

static uint32 get_offset(const uint32 row, const uint32 col);

static uint32 get_row_offset(const uint32 offset);

static uint32 get_col_offset(const uint32 offset);




/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

void print(const char* string)
{
    for(uint32 i = 0; string[i] != '\0'; i++)
    {
        print_char(string[i]);
    }
}

void clear_screen()
{
    char* video_memory = (char*) VIDEO_MEMORY;

    for(uint16 i = 0; i < MAX_ROWS * MAX_COLS; i++)
    {
        video_memory[2 * i] = ' ';
        video_memory[(2 * i) + 1] = WHITE_ON_BLACK;
    }
    
    set_cursor_offset(0);
}

void print_backspace() 
{
    set_cursor_offset(get_cursor_offset()-2);
    print_char(BACKSPACE_ASCII);
}



void print_char(const char c)
{
    uint32 offset = get_cursor_offset();


    if('\n' == c)
    {
        uint32 row = get_row_offset(offset);
        offset = get_offset(row + 1, 0);
    }
    else
    {
        
        
        char* video_memory = (char*) VIDEO_MEMORY;

        //because backspace and space are the same printable char (' ') we 
        // have to check accordingly
        if (c == BACKSPACE_ASCII)
        {
            video_memory[offset] = SPACE_ASCII;
        }
        else
        {
            video_memory[offset] = c;
        }
        video_memory[offset + 1] = WHITE_ON_BLACK;

        // increase offset by 2 since we printed new char to screen
        // we increase it only if the char printed is not backspace
        if (c != BACKSPACE_ASCII)
        {
            offset += 2;
        }
    }

    if (offset >= MAX_COLS * MAX_ROWS * 2)
    {
        //scroll: move all lines one line above (remove line 0)
        for(uint32 i = 1; i < MAX_ROWS; i++)
        {
            memcopy(get_offset(i - 1, 0) + VIDEO_MEMORY,
                    get_offset(i, 0) + VIDEO_MEMORY, 
                    MAX_COLS * 2);
        }

        //clear last line
        char* lastLine = get_offset(MAX_ROWS - 1, 0) + VIDEO_MEMORY;
        for(uint32 i = 0; i < MAX_COLS * 2; i++)
        {
            lastLine[i] = 0;
        }

        //decrase offset by one line
        offset -= MAX_COLS * 2;
    }

    set_cursor_offset(offset);
}





/**********************************************************
 * Private kernel functions                               *
 **********************************************************/


/*
 * Retrieves the current cursor position in the `VGA` text buffer by querying the `VGA` controller.
 * The cursor position is returned as an offset in the `VGA` memory. Each cell in `VGA` memory 
 * consists of a character and its associated color attribute, so the offset is multiplied by 2.
 *
 *
 * @return The offset in the `VGA` text buffer where the cursor is currently located, 
 *         multiplied by 2 because each cell in the `VGA` memory is 2 bytes.
 */
static uint32 get_cursor_offset()
{
    // Request the high byte of the cursor position
    port_byte_out(VGA_CONTROL_REFISTER_PORT, CURSER_HIGH_LOCATION);

    uint32 position = port_byte_in(VGA_DATA_REFISTER_PORT);

    position = position << 8; // Shift left by one byte to position the high byte correctly

    // Request the low byte of the cursor position
    port_byte_out(VGA_CONTROL_REFISTER_PORT, CURSER_LOW_LOCATION);

    position |= port_byte_in(VGA_DATA_REFISTER_PORT);

    /* VGA cells consist of a character and its color attribute */
    return position * 2;
}

/*
 * Sets the cursor position in the `VGA` text buffer to a specified offset.
 * The function converts the offset into a position that the `VGA` controller can understand 
 * by dividing it by 2 (since each `VGA` cell is 2 bytes). It then splits this position 
 * into high and low bytes, which are sent to the `VGA` controller to update the cursor position.
 *
 * @param offset The offset in the `VGA` text buffer where the cursor should be placed. 
 *               This is divided by 2 to convert it into a position that reflects 
 *               the number of cells in the VGA memory.
 */
static void set_cursor_offset(const uint32 offset)
{
    const uint16 position = (uint16)(offset / 2);

    port_byte_out(VGA_CONTROL_REFISTER_PORT, CURSER_HIGH_LOCATION);

    // Send the high byte by shifting the position right by 8 bits
    port_byte_out(VGA_DATA_REFISTER_PORT, (uint8)(position >> 8));

    port_byte_out(VGA_CONTROL_REFISTER_PORT, CURSER_LOW_LOCATION);

    // Send the low byte by using AND on the lower 8 bits of the position
    port_byte_out(VGA_DATA_REFISTER_PORT, (uint8)(position & 0xff));
}

/*
 * Calculates the memory offset for a given row and column in the `VGA` text buffer.
 *
 * @param row The row number in the `VGA` text buffer (starting from 0).
 * @param col The column number in the `VGA` text buffer (starting from 0).
 *
 * @return The memory offset for the specified row and column, where each cell is 2 bytes.
 */
static uint32 get_offset(const uint32 row, const uint32 col)
{
    return 2 * (row * MAX_COLS + col);
}


/*
 * Calculates the row number based on a given memory offset in the `VGA` text buffer.
 *
 * @param offset The memory offset in the `VGA` text buffer.
 *
 * @return The row number corresponding to the given offset.
 */
static uint32 get_row_offset(const uint32 offset)
{
    return offset / (2 * MAX_COLS);
}

/*
 * Calculates the column number based on a given memory offset in the `VGA` text buffer.
 *
 * @param offset The memory offset in the `VGA` text buffer.
 *
 * @return The column number corresponding to the given offset.
 */
static uint32 get_col_offset(const uint32 offset)
{
    return (offset % MAX_COLS) / 2;
}


