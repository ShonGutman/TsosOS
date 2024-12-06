#include "floppy.h"
#include "headers/fat_headers.h"
#include "drivers/ports/ports.h"
#include "cpu/interrupts/irq.h"

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

enum FloppyCommands
{
   READ_TRACK =                 2,	// generates IRQ6
   SPECIFY =                    3,      // * set drive parameters
   SENSE_DRIVE_STATUS =         4,
   WRITE_DATA =                 5,      // * write to the disk
   READ_DATA =                  6,      // * read from the disk
   RECALIBRATE =                7,      // * seek to cylinder 0
   SENSE_INTERRUPT =            8,      // * ack IRQ6, get status of last command
   WRITE_DELETED_DATA =         9,
   READ_ID =                    10,	// generates IRQ6
   READ_DELETED_DATA =          12,
   FORMAT_TRACK =               13,     
   DUMPREG =                    14,
   SEEK =                       15,     // * seek both heads to cylinder X
   VERSION =                    16,	// * used during initialization, once
   SCAN_EQUAL =                 17,
   PERPENDICULAR_MODE =         18,	// * used during initialization, once, maybe
   CONFIGURE =                  19,     // * set controller parameters
   LOCK =                       20,     // * protect controller params from a reset
   VERIFY =                     22,
   SCAN_LOW_OR_EQUAL =          25,
   SCAN_HIGH_OR_EQUAL =         29
};

static volatile const fat_header* const fat_headers = 0x7c00;
static volatile bool floppy_interrupt_flag = false;


/**********************************************************
 * Private kernel functions signiture                     *
 **********************************************************/

static void lba_to_chs(const uint16 lba, uint8* const sector, uint8* const head, uint8* const cylinder);

static bool floopy_wait_until_ready();

static void floppy_send_command(const uint8 command);

static void wait_for_interrupt();

static void floppy_handler(interrupt_registers_struct regs);


/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/


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

/*
 * Waits until the floppy disk controller is ready for data transfer.
 * This function checks the Main Status Register (MSR) repeatedly to determine
 * if the controller's data register is ready. It polls the MSR up to 1000 times.
 *
 * @return `true` if the data register becomes ready within the polling limit, 
 *         otherwise `false`.
 */
static bool floopy_wait_until_ready()
{
    for (int i = 0; i < 1000; i++)
    {
        // Get main status register and check if the last bit is set
        // If yes, data register is ready for data transfer
        if (port_byte_in(MAIN_STATUS_REGISTER) & 0x80)
        {
            return true;
        }
    }

    return false;
}

/*
 * Sends a command to the floppy disk controller via the Data FIFO register.
 * The function first waits for the controller to be ready by polling the 
 * Main Status Register. If the controller is not ready within the polling 
 * limit, the command is not sent.
 *
 * @param command The command byte to send to the floppy disk controller.
 */
static void floppy_send_command(const uint8 command)
{
    if(!floppy_wait_until_ready())
    {
        return;
    }

    port_byte_out(DATA_FIFO, command);
}

/*
 * Waits for the floppy disk interrupt to be triggered.
 * This function continuously checks a flag (`floppy_interrupt_flag`) that is set
 * when a floppy interrupt occurs. Once the interrupt is detected, the flag is reset.
 */
static void wait_for_interrupt()
{
    while (!floppy_interrupt_flag){}
    floppy_interrupt_flag = false;  
}

/*
 * Handles the floppy disk interrupt.
 * This interrupt service routine sets the `floppy_interrupt_flag`, signaling
 * that a floppy interrupt has occurred.
 *
 * @param regs The registers state at the time of the interrupt.
 */
static void floppy_handler(interrupt_registers_struct regs)
{
    floppy_interrupt_flag = true;
}