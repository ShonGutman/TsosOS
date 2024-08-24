#include "pic.h"

#define MASTER_PIC_COMMAND_PORT 0x20
#define MASTER_PIC_DATA_PORT 0X21

#define SLAVE_PIC_COMMAND_PORT 0xa0
#define SLAVE_PIC_DATA_PORT 0xa1

void pic_remap(const uint32 master_offset, const uint32 slave_offset);

void pic_init()
{

}

void pic_remap(const uint32 master_offset, const uint32 slave_offset)
{

}
