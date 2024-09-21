#ifndef PAGING_H
#define PAGING_H
#include "../headers/types.h"


typedef struct page_table_entry {
    uint8 p     : 1;   // present
    uint8 rw    : 1;   // read/write
    uint8 us    : 1;   // user or supervisor
    uint8 pwt   : 1;   // Write-Through
    uint8 pcd   : 1;   // Cache Disable
    uint8 a     : 1;   // accessed
    uint8 d     : 1;   // dirty
    uint8 pat   : 1;   // Page Attribute Table
    uint8 g     : 1;   // global
    uint8 avl   : 3;   // available
    uint32 addr : 20;  // address of physical frame
}page_table_entry;

typedef struct page_directory_entry {
    uint8 p     : 1;   // present
    uint8 rw    : 1;   // read/write
    uint8 us    : 1;   // user or supervisor
    uint8 pwt   : 1;   // Write-Through
    uint8 pcd   : 1;   // Cache Disable
    uint8 a     : 1;   // accessed
    uint8 avl   : 1;   // avliable bit
    uint8 ps    : 1;   // when ps=0 the addr table field represent the actual address of each page table
    uint8 available   : 4;   // available bits
    uint32 addr : 20;  // address of physical frame
}page_directory_entry;



#endif