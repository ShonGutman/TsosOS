#include "gdt.h"

#define GDT_ENTRIES 256

#define NULL_DESCRIPTOR_INDEX 0
#define KERNEL_CODE_DESCRIPTOR_INDEX 1
#define KERNEL_DATA_DESCRIPTOR_INDEX 2
#define USER_CODE_DESCRIPTOR_INDEX 3
#define USER_DATA_DESCRIPTOR_INDEX 4

/**
 * Granularity: if 1 then limit *= 0x1000 (to expand segment - we want it)
 * 32 bit? Yes, thus: 1
 * Long: for 64-bit so set to 0
 * Available: For software use, not used by hardware. set to 0
 */
#define OTHER_FLAGS 0b1100

/**
 * Type Flags (Code Segment):
 *  -------------------------
 * 
 *        - Executable: Contain Code? Yes, thus: 1
 *        - Conforming: can be executed from lower privilege segments? No, thus: 0
 *        - Readable: is it only executable or also readable? Yes, thus: 1
 *        - Accessed: used by CPU. set to 1 if when CPU uses the segment. Set to 0 at let CPU deal with it
 */
#define CODE_FLAGS 0b1010

/**
 * Type Flags (Data Segment):
 *  -------------------------
 * 
 *        - Executable: Contain Code? No, thus: 0
 *        - Direction: segment grows downwords (We dont want that so set to 0)
 *        - Writable: 0 if read only (in our case it is not so set to 1)
 *        - Accessed: used by CPU. set to 1 if when CPU uses the segment. Set to 0 at let CPU deal with it
 */
#define DATA_FLAGS 0b0010

gdt_entry_struct gdt_entries[GDT_ENTRIES];
gdt_info_struct gdt_info;


/**********************************************************
 * Private kernel functions signiture                     *
 **********************************************************/

static void set_null_gate();

static void set_default_gate(const uint8 index, const uint8 type_flags, const uint8 privilege_level);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

void init_gdt()
{

    set_null_gate();
    set_default_gate(KERNEL_CODE_DESCRIPTOR_INDEX, CODE_FLAGS, 0);
    set_default_gate(KERNEL_DATA_DESCRIPTOR_INDEX, DATA_FLAGS, 0);
    set_default_gate(USER_CODE_DESCRIPTOR_INDEX, CODE_FLAGS, 3);
    set_default_gate(USER_DATA_DESCRIPTOR_INDEX, DATA_FLAGS, 3);

    gdt_info.size = sizeof(gdt_entry_struct) * GDT_ENTRIES - 1;
    gdt_info.offset = (uint32)&gdt_entries;

    //load info on gdt using lgdt assembly instruction
    __asm__ ("lgdt (%0)" : : "r"(&gdt_info));
}


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

/*
 * Sets up a null entry in the Global Descriptor Table (GDT) at index 0.
 * This is a required entry in the GDT but is not used by the CPU. It acts
 * as an unused or "empty" segment.
 */
static void set_null_gate()
{
    gdt_entries[0].base_0_to_15 = 0;
    gdt_entries[0].base_16_to_23 = 0;
    gdt_entries[0].base_24_to_31 = 0;
    gdt_entries[0].length_0_to_15 = 0;
    gdt_entries[0].length_16_to_19 = 0;
    gdt_entries[0].type_flags = 0;
    gdt_entries[0].type = 0;
    gdt_entries[0].privilege_level = 0;
    gdt_entries[0].present = 0;
    gdt_entries[0].other_flags = 0;
}

/*
 * Initializes a GDT entry for either a code or data segment at the specified index.
 * The function sets the base address to 0 and the segment length to its maximum value.
 * The segment is marked as present, and the type and privilege level are set based on 
 * the provided parameters.
 *
 * @param index The index in the GDT where this entry will be placed.
 * @param type_flags (4 bit value) Flags that specify the type and access rights of the segment.
 * @param privilege_level (2 bit value) The privilege level at which the segment will operate (0-3).
 */
static void set_default_gate(const uint8 index, const uint8 type_flags, const uint8 privilege_level)
{
    gdt_entries[index].base_0_to_15 = 0;
    gdt_entries[index].base_16_to_23 = 0;
    gdt_entries[index].base_24_to_31 = 0;

    gdt_entries[index].length_0_to_15 = 0xffff;
    gdt_entries[index].length_16_to_19 = 0xf;

    gdt_entries[index].type_flags = type_flags;
    gdt_entries[index].type = 1; // this is Code / Data Segment
    gdt_entries[index].privilege_level = privilege_level;
    gdt_entries[index].present = 1; // it is being used

    gdt_entries[index].other_flags = OTHER_FLAGS;

}
