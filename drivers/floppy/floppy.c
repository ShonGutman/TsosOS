#include "floppy.h"
#include "headers/fat_headers.h"
#include "drivers/ports/ports.h"
#include "cpu/interrupts/irq.h"
#include "libc/string.h"
#include "cpu/timer/timer.h"
#include "drivers/screen/screen.h"

enum FloppyRegisters
{
   STATUS_REGISTER_A                = 0x3F0,    // read-only, NOT REALLY USED
   STATUS_REGISTER_B                = 0x3F1,    // read-only, NOT REALLY USED
   DIGITAL_OUTPUT_REGISTER          = 0x3F2,
   TAPE_DRIVE_REGISTER              = 0x3F3,    // NOT REALLY USED
   MAIN_STATUS_REGISTER             = 0x3F4,    // read-only
   DATARATE_SELECT_REGISTER         = 0x3F4,    // write-only, NOT REALLY USED
   DATA_FIFO                        = 0x3F5,
   DIGITAL_INPUT_REGISTER           = 0x3F7,    // read-only, NOT REALLY USED
   CONFIGURATION_CONTROL_REGISTER   = 0x3F7     // write-only
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

enum DIGITAL_OUTPUT_MASK 
{
 
	DOR_MASK_DRIVE0			     =	0,	//00000000 - here for completeness sake
	DOR_MASK_DRIVE1			     =	1,	//00000001
	DOR_MASK_DRIVE2			     =	2,	//00000010
	DOR_MASK_DRIVE3			     =	3,	//00000011
	DOR_MASK_RESET			     =	4,	//00000100
	DOR_MASK_DMA			     =	8,	//00001000
	DOR_MASK_DRIVE0_MOTOR		 =	16,	//00010000
	DOR_MASK_DRIVE1_MOTOR		 =	32,	//00100000
	DOR_MASK_DRIVE2_MOTOR		 =	64,	//01000000
	DOR_MASK_DRIVE3_MOTOR		 =	128	//10000000
};

#define STEP_RATE 3             // 3 ms
#define HEAD_LOAD_TIME 16       // 16 ms
#define HEAD_UNLOAD_TIME 240    // 240 ms

#define DEVICE_NUMBER 0
#define NUMBER_OF_FLOPPY_PER_FDC 4

static volatile const fat_header* const fat_headers = 0x7c00;
static volatile bool floppy_interrupt_flag = false;


/**********************************************************
 * Private kernel functions signiture                     *
 **********************************************************/

static void floppy_reset();

static void lba_to_chs(const uint16 lba, uint8* const sector, uint8* const head, uint8* const cylinder);

static bool floopy_wait_until_ready();

static void floppy_send_command(const uint8 command);

static uint8 floppy_read_data();

static void floppy_motor(const bool enable);

static void floppy_confirm_interrupt(uint8* st0, uint8* cylinder);

static void wait_for_interrupt();

static void floppy_handler(interrupt_registers_struct regs);


/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/


/*
 * Resets the floppy disk controller and configures its settings for operation.
 * The function disables the controller, re-enables it, waits for an interrupt, 
 * and performs the necessary initialization commands, including setting the 
 * transfer speed and timing parameters.
 */
static void floppy_reset()
{
    // Disable floppy controller
    port_byte_out(DIGITAL_OUTPUT_REGISTER, 0);

    // Enable floppy controller (reset (0x04) | DMA (0x08))
    port_byte_out(DIGITAL_OUTPUT_REGISTER, DOR_MASK_RESET | DOR_MASK_DMA);

    // Wait for interrupt and continue reset sequence
    wait_for_interrupt();

    // FDC can be connected to several floppy disks.
    for(int i = 0; i < NUMBER_OF_FLOPPY_PER_FDC; i++)
    {
        uint8 st0 = 0;
        uint8 cylinder = 0;
        floppy_confirm_interrupt(&st0, &cylinder);

        //debugging perpuse
        char string[4] = {0};
        int_to_ascii(st0, string);
        print(string);
    }

    // Set transfer speed to 500 kb/s
    //  00 - 500 Kb/s
    //  01 - 250 Kb/s
    //  10 - 100 Kb/s
    //  11 - 1 Mb/s
    outb(CONFIGURATION_CONTROL_REGISTER, 0);

    floppy_send_command(SPECIFY);

    // S S S S H H H H
    // S = Step rate
    // H = Head unload time
    floppy_send_command(((STEP_RATE & 0xf) << 4) | (HEAD_UNLOAD_TIME & 0xf));

    // H H H H H H H DMA
    // H = Head load time
    floppy_send_command((HEAD_LOAD_TIME << 1) | 1);
}


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
 * Reads a byte of data from the floppy disk controller's Data FIFO register.
 * Before reading, the function ensures the controller is ready by checking 
 * the Main Status Register. If the controller is not ready, a default value 
 * of `0xFF` is returned.
 *
 * @return The byte read from the floppy disk controller, or `0xFF` if the 
 *         controller is not ready.
 */
static uint8 floppy_read_data()
{
    if (!floppy_wait_until_ready())
    {
        return 0xff;
    }

    return port_byte_in(DATA_FIFO);
}

/*
 * Controls the floppy drive motor by enabling or disabling it.
 * Sends the appropriate command to the DIGITAL_OUTPUT_REGISTER and waits for the motor to spin up or down.
 *
 * @param enable Boolean flag indicating whether to enable (true) or disable (false) the motor.
 *               If true, the motor is turned on; if false, the motor is turned off.
 */
static void floppy_motor(const bool enable)
{
    port_byte_out(DIGITAL_OUTPUT_REGISTER, DOR_MASK_RESET | (enable ? DOR_MASK_DRIVE0_MOTOR : 0));
    sleep(600);
}

/*
 * Confirms a floppy disk interrupt by sending the SENSE_INTERRUPT command to the floppy controller.
 * The function reads the status register (st0) and the current cylinder after acknowledging the interrupt.
 *
 * @param st0      Pointer to a variable where the status register (st0) value will be stored.
 * @param cylinder Pointer to a variable where the current cylinder value will be stored.
 */
static void floppy_confirm_interrupt(uint8* st0, uint8* cylinder)
{
    // Tell connected devices that we catched the interrupt (SENSE_INTERRUPT command)
    floppy_send_command(SENSE_INTERRUPT);

    *st0 = floppy_read_data();
    *cylinder = floppy_read_data();
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