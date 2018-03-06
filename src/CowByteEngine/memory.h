#ifndef _CBMEMORY_H
#define _CBMEMORY_H
#include "CBPoolAllocator.h"
#include "CBMemArena.h"
#include "Debug.h"

CBMemArena MemArena;

inline void* operator new(size_t size)
{
    return MemArena.Allocate(size);
}
inline void* operator new[](size_t size)
{
    return MemArena.Allocate(size);
}

inline void operator delete(void* ptr, size_t size)
{
    MemArena.Free(ptr, size);
}
inline void operator delete[](void* ptr, size_t size)
{
    MemArena.Free(ptr, size);
}

// Disable single ptr delete.
inline void operator delete(void* ptr)
{
    // Do nothing.
    DbgWARNING("Attempting using delete without size!");
}
inline void operator delete[](void* ptr)
{
    // Do nothing.
    DbgWARNING("Attempting using delete[] without size!");
}


#endif // !_CBMEMORY_H
