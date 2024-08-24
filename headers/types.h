#ifndef TYPES_H
#define TYPES_H

//define signed
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

//define unsigned
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef struct
{
    uint32 ds;
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32 int_number, err_code;
    uint32 eip, cs, eflags, useresp, ss;
}interrupt_registers_struct;



#endif