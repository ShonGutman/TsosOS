
#define VIDEO_MEMORY 0xb8000
#define WHITE_ON_BLACK 0x0f

#include "../drivers/screen.h"

void main() 
{
    char* video_memory = (char*) VIDEO_MEMORY;
    
    uint32 offset = get_cursor_offset();

    char* osName = "TSOS";

    for(int i = 0; i < 4; i++)
    {
        video_memory[offset + 2 * i] = osName[i];
        video_memory[offset + 1 + (2 * i)] = WHITE_ON_BLACK;
    }
}