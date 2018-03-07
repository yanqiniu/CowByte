#ifndef _CBMEMORY_H
#define _CBMEMORY_H
#include "CBPoolAllocator.h"
#include "CBMemArena.h"
#include "Debug.h"


void* operator new(size_t size) throw()
{
    return CBMemArena::Get().Allocate(size);
}
void* operator new[](size_t size)
{
    return CBMemArena::Get().Allocate(size);
}

void operator delete(void* ptr, size_t size)
{
    CBMemArena::Get().Free(ptr, size);
}
void operator delete[](void* ptr, size_t size)
{
    CBMemArena::Get().Free(ptr, size);
}

// Disable single ptr delete.
void operator delete(void* ptr)
{
    // Do nothing.
    DbgWARNING("Attempting using delete without size!");
}
void operator delete[](void* ptr)
{
    // Do nothing.
    DbgWARNING("Attempting using delete[] without size!");
}


#endif // !_CBMEMORY_H
