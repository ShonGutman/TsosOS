#include "paging.h"
#include <stdint.h>

#define PAGE_TABLE_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRIES 1024

#define FRAME_SIZE 0x1000 // 4 KB
#define TOTAL_MEMORY_SIZE (1024 * 1024 * 32) // Example: 32 MB of RAM
#define TOTAL_FRAMES (TOTAL_MEMORY_SIZE / FRAME_SIZE)

// Bitmap to track frame usage
uint8 frame_bytemap[TOTAL_FRAMES]; // Each byte represents a frame

// Make sure that both the page tables and the directory are 4 KB aligned
PageDirectoryEntry page_directory[PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
PageTableEntry first_page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

static void loadPageDirectory(PageDirectoryEntry* page_directory);
static void enablePaging();
uint32 allocate_frame();
void init_frame_bytemap();
void map_page(uint32 phys_addr, uint32 virt_addr, uint32 flags);
void unmap_page(uint32 virt_addr);
void free_frame(uint32 frame);

// Initialize paging
void init_paging() 
{
    init_frame_bytemap();

    for (uint32 i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) 
    {
        page_directory[i].present = 0; // Initialize directory entries
    }

    // Example of mapping the first 4 MB of memory, page by page
    for (uint32 i = 0; i < 1024; i++) 
    {
        map_page(i * FRAME_SIZE, i * FRAME_SIZE, 0x3); // Present, RW
    }

    loadPageDirectory(page_directory);
    enablePaging();

    uint32_t addr = allocate_page(); // Allocate a page
    if (addr == -1) {
        // Handle allocation failure
    }

    char* ptr = (char*) addr; // Cast to char pointer for easy access
    *ptr = 'T';               // Write to the allocated page

    if (*ptr != 'T') {
        // Handle test failure
    }

    unmap_page(addr); // Unmap the page


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

// Initialize frame bytemap by marking all frames as free
void init_frame_bytemap() 
{
    for (uint32 i = 0; i < TOTAL_FRAMES; i++) 
    {
        frame_bytemap[i] = 0;
    }
}

// Allocate a frame by finding the first free frame
uint32 allocate_frame() {
    for (uint32 i = 0; i < TOTAL_FRAMES; i++) 
    {
        if (!frame_bytemap[i]) {
            frame_bytemap[i] = 1; // Mark the frame as used
            return i * FRAME_SIZE;
        }
    }
    return (uint32)-1; // No free frames available
}

// Free a frame by marking it as available
void free_frame(uint32 frame) 
{
    uint32 frame_index = frame / FRAME_SIZE;
    if (frame_index < TOTAL_FRAMES) 
    {
        frame_bytemap[frame_index] = 0; // Mark as free
    }
}

// Manually clear a page table by setting all entries to zero
void clear_page_table(PageTableEntry* table) 
{
    for (uint32 i = 0; i < PAGE_TABLE_ENTRIES; i++) 
    {
        table[i].present = 0;
        table[i].rw = 0;
        table[i].user = 0;
        table[i].frame_address = 0;
    }
}

// Map a physical address to a virtual address with specific flags
void map_page(uint32 phys_addr, uint32 virt_addr, uint32 flags) 
{
    uint32 dir_index = virt_addr >> 22;          // Top 10 bits for the page directory index
    uint32 table_index = (virt_addr >> 12) & 0x03FF; // Next 10 bits for the page table index

    // If the page directory entry is not present, allocate a new page table
    if (!page_directory[dir_index].present) 
    {
        uint32 new_page_table = allocate_frame(); // Get a new frame for the page table
        PageTableEntry* table = (PageTableEntry*)(new_page_table);
        clear_page_table(table); // Manually clear the new page table
        page_directory[dir_index].table_address = new_page_table >> 12;
        page_directory[dir_index].present = 1;
        page_directory[dir_index].rw = (flags & 0x2) ? 1 : 0; // Set read/write flag
        page_directory[dir_index].user = (flags & 0x4) ? 1 : 0; // Set user/supervisor flag
    }

    // Get the page table pointer
    PageTableEntry* table = (PageTableEntry*)(page_directory[dir_index].table_address << 12);
    table[table_index].frame_address = phys_addr >> 12; // Map the physical address
    table[table_index].present = 1;
    table[table_index].rw = (flags & 0x2) ? 1 : 0;
    table[table_index].user = (flags & 0x4) ? 1 : 0;
}

// Unmap a virtual address by clearing its entry
void unmap_page(uint32 virt_addr) 
{
    uint32 dir_index = virt_addr >> 22;          // Top 10 bits for the page directory index
    uint32 table_index = (virt_addr >> 12) & 0x03FF; // Next 10 bits for the page table index

    if (!page_directory[dir_index].present) 
    {
        return; // Page directory entry not present, nothing to unmap
    }

    // Get the page table pointer
    PageTableEntry* table = (PageTableEntry*)(page_directory[dir_index].table_address << 12);

    // Mark the page as not present
    table[table_index].present = 0;
    table[table_index].rw = 0;
    table[table_index].user = 0;

    // Optionally, free the frame if no longer needed
    free_frame(table[table_index].frame_address << 12);
}
