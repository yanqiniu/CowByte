#ifndef _CBPOOLALLOCATOR_H
#define _CBPOOLALLOCATOR_H
#include <assert.h>
#include <cstring>
#include <new.h>
#include "typedefs.h"

struct CBPoolBlock
{
    /*******************************************************
     _next pointer:
    This was previously implemented as a union with actual
    data:

    union PoolBlockData
    {
        // This is a union since memory content and next pointer
        // are never needed at the same time.
        // This helps reduce block size and prevent ruining 
        // alignment.

        char _mem[blockSize]; 
        CBPoolBlock *_next;
    };

    But since I moved from templated pool to support pools
    with varying sizes better, this no longer works.

    The current approach is to simply overwrite the _next 
    ptr with data as if it's not there. And only accessing 
    it when we know it's not corrupted (when the block is 
    empty and next ptr has been reset).

    (Well yea, it's essentially still a union.)

    Yea...doesn't sound super safe. Works so far tho :/
    *******************************************************/
    CBPoolBlock *_next;

#ifndef _WIN64
    UINT32 _padding[3]; // x86 padding
#else
    UINT32 _padding[2]; // x64 padding
#endif // !_WIN64
};

// Stands for: Fixed sized pool allocator / arena.
// Essentially we are gonna have a set of these with 
// varying sizes.
// ! blockByteSize must be divisible by 16!
struct CBMemPool
{
    explicit CBMemPool(UINT32 blockSize, UINT32 nBlocks);
    ~CBMemPool();

    //Allocate the pool on the heap, setup free list.
    void Initialize(void *startPtr);
    // Free memory used by the arena.
    void Shutdown();

    // Return address to a free block.
    void* Allocate(size_t size); 
    // Free memory and add block back to free list.
    void Free(void* ptr);

    size_t Capacity();

    CBPoolBlock *m_pBlocks; // pointer to all blocks in the pool.
    CBPoolBlock *m_pFreeList; // head of the free block list.
    UINT32 m_blockSize;// technically should prob use size_t for this but unlikely the size will be that large.
    UINT32 m_nFreeBlocks;
    UINT32 m_nTotalBlocks;
    
// To make is 16 byte aligned.
#ifdef _WIN64
    int _padding;
#else
    int _padding[3];
#endif
};

CBMemPool::CBMemPool(UINT32 blockSize, UINT32 nBlocks) :
    m_pBlocks(nullptr),
    m_pFreeList(nullptr),
    m_blockSize(blockSize),
    m_nTotalBlocks(nBlocks),
    m_nFreeBlocks(nBlocks)
{

}

CBMemPool::~CBMemPool()
{
    Shutdown();
}


void CBMemPool::Initialize(void *startPtr)
{
    m_pBlocks = static_cast<CBPoolBlock*>(startPtr);
    char* pMarker = reinterpret_cast<char*>(m_pBlocks);
    // Place blocks in mem arena.
    for (int i = 0; i < m_nTotalBlocks; ++i)
    {
        new (reinterpret_cast<CBPoolBlock*>(pMarker)) CBPoolBlock();
        pMarker += (size_t)m_blockSize;
    }

    // Setup the free list.
    m_pFreeList = m_pBlocks;
    pMarker = reinterpret_cast<char*>(m_pFreeList);
    for (UINT32 i = 0; i < m_nTotalBlocks - 1; ++i)
    {
        reinterpret_cast<CBPoolBlock*>(pMarker + i * m_blockSize)->_next = 
        reinterpret_cast<CBPoolBlock*>(pMarker + (i + 1) * m_blockSize);
    }
    reinterpret_cast<CBPoolBlock*>(pMarker +(m_nTotalBlocks - 1) * m_blockSize)->_next = nullptr;
}



void CBMemPool::Shutdown()
{
    delete[] m_pBlocks;
    m_pFreeList = nullptr;
}



inline void* CBMemPool::Allocate(size_t size)
{
    assert(size <= m_blockSize && "Cannot allocate larger than pool size!");
    assert(m_nFreeBlocks > 0 && "No more free blocks in pool!");
    void* toRet = (void*)m_pFreeList;
    m_pFreeList = m_pFreeList->_next;
    //memset(toRet, 0, sizeof(CBPoolBlock)); // At last, erase next pointer and padding
    --m_nFreeBlocks;
    return toRet;
}


inline void CBMemPool::Free(void* ptr)
{
    if (ptr == nullptr)
        return;

    // CAUTION: casting to to block pointer.
    CBPoolBlock *blockPtr = reinterpret_cast<CBPoolBlock*>(ptr);

    memset(blockPtr, 0, m_blockSize);
    blockPtr->_next = m_pFreeList;
    m_pFreeList = blockPtr;

    ++m_nFreeBlocks;
}



inline size_t CBMemPool::Capacity()
{
    return m_blockSize * m_nTotalBlocks;
}


#endif
