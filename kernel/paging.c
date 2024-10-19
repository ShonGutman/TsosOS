#include "paging.h"
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRIES 1024

#define FRAME_SIZE 0x1000 // 4 KB
#define TOTAL_MEMORY_SIZE (1024 * 1024 * 32) // Example: 32 MB of RAM
#define TOTAL_FRAMES (TOTAL_MEMORY_SIZE / FRAME_SIZE)

// Bitmap to track frame usage
uint8 frame_bytemap[TOTAL_FRAMES]; // Each byte represents a frame


// make sure that both the page tables and the directory are 4 kb alligned
PageDirectoryEntry page_directory[PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
PageTableEntry first_page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));
static void loadPageDirectory(PageDirectoryEntry* page_directory);
static void enablePaging();
uint32 allocate_frame();
void init_frame_bytemap();

void init_paging()
{
    init_frame_bytemap();   
    uint16 i = 0;
    for (i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) 
    {

        // Because there page directory and the first page table have the same 
        // amount of entries we can initialize both of them in the same loop

        // Setting default values for the page directory.
        page_directory[i].present = 0; // Mark as not present
        page_directory[i].rw = 1;      // Read/Write enabled
        page_directory[i].user = 0;    // Supervisor level
        // initializing the first page table
        first_page_table[i].present = 1;   // Page is present
        first_page_table[i].rw = 1;        // Read/Write enabled
        first_page_table[i].user = 0;      // Supervisor level
        first_page_table[i].frame_address = allocate_frame(); // Physical address is i * 0x1000
    }

    // Setting the first page directory entry to point to the first page table
    page_directory[0].present = 1; // Mark as present
    page_directory[0].rw = 1;      // Read/Write enabled
    page_directory[0].user = 0;    // Supervisor level
    page_directory[0].table_address = ((unsigned int)first_page_table) >> 12;

    loadPageDirectory(page_directory);
    enablePaging();
}

// Function to load the page directory address into CR3
static void loadPageDirectory(PageDirectoryEntry* page_directory) 
{
    asm volatile (
        "mov %0, %%cr3"
        : // No output operands
        : "r" (page_directory) // Input operand, 'r' means any general-purpose register
    );
}

// Function to enable paging by setting the paging bit (31st bit) in CR0
static void enablePaging() 
{
    asm volatile (
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"
        "mov %%eax, %%cr0"
        : // No output operands
        : // No input operands
        : "eax" // Clobbered registers
    );
}

//set all frames to free
void init_frame_bytemap() 
{
    uint32 i = 0;
    for (; i < TOTAL_FRAMES; i++)
    {
        frame_bytemap[i] = 0;
    }
    
}


uint32 allocate_frame() 
{
    uint32 i = 0;
    for (; i < TOTAL_FRAMES; i++) 
    {
        if (!frame_bytemap[i]) 
        {
            frame_bytemap[i]++; // set it to 1 meaning that the page is set
            return i;
        }
    }
    return -1; // No free frames available
}
