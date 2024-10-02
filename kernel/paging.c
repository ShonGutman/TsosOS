#include "paging.h"
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRIES 1024

// make sure that both the page tables and the directory are 4 kb alligned
PageDirectoryEntry page_directory[PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
PageTableEntry first_page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));
static void loadPageDirectory(PageDirectoryEntry* page_directory);
static void enablePaging();

void init_paging()
{
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
        first_page_table[i].frame_address = i; // Physical address is i * 0x1000
    }

    // Setting the first page directory entry to point to the first page table
    page_directory[0].present = 1; // Mark as present
    page_directory[0].rw = 1;      // Read/Write enabled
    page_directory[0].user = 0;    // Supervisor level
    page_directory[0].table_address = ((unsigned int)first_page_table) >> 12;
}