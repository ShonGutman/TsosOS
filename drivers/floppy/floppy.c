#include "floppy.h"
#include "headers/fat_headers.h"

volatile const fat_header* fat_headers = 0x7c00;


/*
 * Converts a Logical Block Addressing (LBA) address to Cylinder-Head-Sector (CHS) format.
 *
 * @param lba The LBA address of the disk sector to be converted.
 * @param sector Pointer to an 8-bit variable where the calculated sector value will be stored.
 * @param head Pointer to an 8-bit variable where the calculated head value will be stored.
 * @param cylinder Pointer to an 8-bit variable where the calculated cylinder value will be stored.
 */
void lba_to_chs(const uint16 lba, uint8* const sector, uint8* const head, uint8* const cylinder)
{
    *sector = (lba % fat_headers->sectors_per_track) + 1;
    *head = (lba / fat_headers->sectors_per_track) % fat_headers->heads;
    *cylinder = (lba / fat_headers->sectors_per_track) / fat_headers->heads;
}