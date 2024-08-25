#include "memory.h"

void memcopy(void *dstptr, const void *srcptr, const uint32 size)
{
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;

    for(uint32 i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

void memset(void* dstptr, const unsigned char val, const uint32 size)
{
    unsigned char* dst = (unsigned char*)dstptr;

    for(uint32 i = 0; i < size; i++)
    {
        dst[i] = val;
    }
}