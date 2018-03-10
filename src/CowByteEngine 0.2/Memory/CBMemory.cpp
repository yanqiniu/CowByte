#include "CBMemory.h"
#include "CBMemArena.h"

#ifndef N_OVERLOAD_GLOBAL_NEW_DELETE
void* operator new(size_t size)
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
    ptr = nullptr;
}
void operator delete[](void* ptr, size_t size)
{
    CBMemArena::Get().Free(ptr, size);
    ptr = nullptr;
}

// Disable original ptr delete.
void operator delete(void* ptr)
{
    // Do nothing.
    DbgWARNING("Attempting using delete without size!");
    assert(false);
}
void operator delete[](void* ptr)
{
    // Do nothing.
    DbgWARNING("Attempting using delete[] without size!");
    assert(false);
}
#endif