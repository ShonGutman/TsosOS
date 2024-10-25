#ifndef FAT_HEADERS_H
#define FAT_HEADERS_H

#include "headers/types.h"


typedef struct
{
    // Bios Parameter Block
    uint32 jump_signature : 24;
    char oem_identifier[8];         // MSWIN4.1 
    uint16 bytes_per_sector;        // 512
    uint8 sectors_per_cluster;      //1
    uint16 reserved_sectors;        //1
    uint8 fat_count;                //2
    uint16 root_dir_entries;        //224
    uint16 total_sectors;           //2880
    uint8 media_descriptor_type;    //240
    uint16 sectors_per_fat;         //9
    uint16 sectors_per_track;       //18
    uint16 heads;                   //2
    uint32 hidden_sectors;          //0
    uint32 large_sectors;           //0

    // Extended Boot Record
    uint8 drive_number;             //0
    uint8 reserved;                 //0
    uint8 signature;                //41
    uint32 volume_id;               //0x12345678
    char volume_label[11];          //TSOS OS    
    char system_identifier[8];      //FAT 12   
} __attribute__((packed)) fat_header;


#endif