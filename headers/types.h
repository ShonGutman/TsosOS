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

// Helper Macros
#define LOW_BYTE(x)           ((uint8)((x) & 0xFF))
#define HIGH_BYTE(x)          ((uint8)(((x) >> 8) & 0xFF))

typedef struct
{
    uint32 ds;
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32 int_number, err_code;
    uint32 eip, cs, eflags, useresp, ss;
}interrupt_registers_struct;



#endif