#ifndef _CB_MEM_ARENA_H
#define _CB_MEM_ARENA_H
#include <Vector>
#include "CBPoolAllocator.h"
class CBMemArena
{
public:
    CBMemArena();
    void* Allocate(size_t size);
    void Free(void* ptr, size_t size); // size is needed to determine which pool it was in.


private:
    FSPoolAllocator<16, 1024>         m_SubPool_16B; // For smaller allocation like a single Vec3.
    FSPoolAllocator<64, 1024>         m_SubPool_64B; // For single matrix.
    FSPoolAllocator<0x400, 1024>      m_SubPool_1KB;
    FSPoolAllocator<0x800, 1024>      m_SubPool_2KB;
    FSPoolAllocator<0x1000, 1024>     m_SubPool_4KB;
    FSPoolAllocator<0x2000, 1024>     m_SubPool_8KB;
    FSPoolAllocator<0x4000, 1024>     m_SubPool_16KB;
    FSPoolAllocator<0x8000, 1024>     m_SubPool_32KB;
    FSPoolAllocator<0x10000, 1024>    m_SubPool_64KB;
    FSPoolAllocator<0x20000, 1024>    m_SubPool_128KB;
    FSPoolAllocator<0x40000, 1024>    m_SubPool_256KB;
    FSPoolAllocator<0x80000, 1024>    m_SubPool_512KB;
    FSPoolAllocator<0x100000, 1024>   m_SubPool_1MB;
};

CBMemArena::CBMemArena()
{
}

void* CBMemArena::Allocate(size_t size)
{
#define SIZE_ALLOC(byteSize, pool) if (size <= byteSize) return pool.Allocate(size);
    SIZE_ALLOC(16, m_SubPool_16B)
    else SIZE_ALLOC(64, m_SubPool_64B)
    else SIZE_ALLOC(0x400, m_SubPool_1KB)
    else SIZE_ALLOC(0x800, m_SubPool_2KB)
    else SIZE_ALLOC(0x1000, m_SubPool_4KB)
    else SIZE_ALLOC(0x2000, m_SubPool_8KB)
    else SIZE_ALLOC(0x4000, m_SubPool_16KB)
    else SIZE_ALLOC(0x8000, m_SubPool_32KB)
    else SIZE_ALLOC(0x10000, m_SubPool_64KB)
    else SIZE_ALLOC(0x20000, m_SubPool_128KB)
    else SIZE_ALLOC(0x40000, m_SubPool_256KB)
    else SIZE_ALLOC(0x80000, m_SubPool_512KB)
    else SIZE_ALLOC(0x100000, m_SubPool_1MB)
    else return nullptr;
#undef SIZE_ALLOC
}

void CBMemArena::Free(void* ptr, size_t size)
{
#define SIZE_FREE(byteSize, pool) if (size <= byteSize) return pool.Free(ptr);
    SIZE_FREE(16, m_SubPool_16B)
    else SIZE_FREE(64, m_SubPool_64B)
    else SIZE_FREE(0x400, m_SubPool_1KB)
    else SIZE_FREE(0x800, m_SubPool_2KB)
    else SIZE_FREE(0x1000, m_SubPool_4KB)
    else SIZE_FREE(0x2000, m_SubPool_8KB)
    else SIZE_FREE(0x4000, m_SubPool_16KB)
    else SIZE_FREE(0x8000, m_SubPool_32KB)
    else SIZE_FREE(0x10000, m_SubPool_64KB)
    else SIZE_FREE(0x20000, m_SubPool_128KB)
    else SIZE_FREE(0x40000, m_SubPool_256KB)
    else SIZE_FREE(0x80000, m_SubPool_512KB)
    else SIZE_FREE(0x100000, m_SubPool_1MB)
    else
    {
        // undefined behavior.
    }

}


#endif // !_CB_MEM_ARENA_H
