#ifndef _CBMEMORY_H
#define _CBMEMORY_H

#define MEMORY_USE_POOL_ALLOCATION

#ifdef MEMORY_USE_POOL_ALLOCATION
#include "CBPoolAllocator.h"
FSPoolAllocator<256, 256> MemAllocator;

inline static void* operator new(size_t size)
{
    return MemAllocator.Allocate(size);
}

inline static void operator delete(void* ptr)
{
    MemAllocator.Free(ptr);
}


#endif // MEMORY_USE_POOL_ALLOCATION



#endif // !_CBMEMORY_H
