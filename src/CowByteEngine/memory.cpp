#ifndef _CBMEMORY_H
#define _CBMEMORY_H
#include "CBPoolAllocator.h"

FSPoolAllocator<256, 256> MemAllocator;

inline void* operator new(size_t size)
{
    return MemAllocator.Allocate(size);
}

inline void operator delete(void* ptr)
{
    MemAllocator.Free(ptr);
}

#endif // !_CBMEMORY_H
