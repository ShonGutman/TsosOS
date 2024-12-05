#include "floppy.h"
#include "headers/fat_headers.h"
#include "drivers/ports/ports.h"

enum FloppyRegisters
{
   STATUS_REGISTER_A                = 0x3F0, // read-only
   STATUS_REGISTER_B                = 0x3F1, // read-only
   DIGITAL_OUTPUT_REGISTER          = 0x3F2,
   TAPE_DRIVE_REGISTER              = 0x3F3,
   MAIN_STATUS_REGISTER             = 0x3F4, // read-only
   DATARATE_SELECT_REGISTER         = 0x3F4, // write-only
   DATA_FIFO                        = 0x3F5,
   DIGITAL_INPUT_REGISTER           = 0x3F7, // read-only
   CONFIGURATION_CONTROL_REGISTER   = 0x3F7  // write-only
};

static volatile const fat_header* const fat_headers = 0x7c00;


/*
 * Converts a Logical Block Addressing (LBA) address to Cylinder-Head-Sector (CHS) format.
 *
 * @param lba The LBA address of the disk sector to be converted.
 * @param sector Pointer to an 8-bit variable where the calculated sector value will be stored.
 * @param head Pointer to an 8-bit variable where the calculated head value will be stored.
 * @param cylinder Pointer to an 8-bit variable where the calculated cylinder value will be stored.
 */
static void lba_to_chs(const uint16 lba, uint8* const sector, uint8* const head, uint8* const cylinder)
{
    *sector = (lba % fat_headers->sectors_per_track) + 1;
    *head = (lba / fat_headers->sectors_per_track) % fat_headers->heads;
    *cylinder = (lba / fat_headers->sectors_per_track) / fat_headers->heads;
}


static bool floopy_wait_until_ready()
{
    while(!(port_byte_in(MAIN_STATUS_REGISTER) & 0x80)) {}

    return true;
}  