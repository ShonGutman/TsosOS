#ifndef PAGING_H
#define PAGING_H
#include "../headers/types.h"

// Struct for a page directory entry
typedef struct {
    uint32 present         : 1;  // Bit 0: Present
    uint32 rw              : 1;  // Bit 1: Read/Write
    uint32 user            : 1;  // Bit 2: User/Supervisor
    uint32 write_through   : 1;  // Bit 3: Write-Through
    uint32 cache_disabled  : 1;  // Bit 4: Cache Disabled
    uint32 accessed        : 1;  // Bit 5: Accessed
    uint32 reserved        : 1;  // Bit 6: Reserved
    uint32 page_size       : 1;  // Bit 7: Page Size (0 for 4 KB)
    uint32 ignored         : 4;  // Bits 8-11: Ignored
    uint32 table_address   : 20; // Bits 12-31: Physical address of page table (20-bit aligned)
} PageDirectoryEntry;

// Struct for a page table entry
typedef struct {
    uint32 present         : 1;  // Bit 0: Present
    uint32 rw              : 1;  // Bit 1: Read/Write
    uint32 user            : 1;  // Bit 2: User/Supervisor
    uint32 write_through   : 1;  // Bit 3: Write-Through
    uint32 cache_disabled  : 1;  // Bit 4: Cache Disabled
    uint32 accessed        : 1;  // Bit 5: Accessed
    uint32 dirty           : 1;  // Bit 6: Dirty
    uint32 reserved        : 1;  // Bit 7: Reserved
    uint32 global          : 1;  // Bit 8: Global
    uint32 ignored         : 3;  // Bits 9-11: Ignored
    uint32 frame_address   : 20; // Bits 12-31: Physical address of page frame (20-bit aligned)
} PageTableEntry;

void init_paging();

#endif