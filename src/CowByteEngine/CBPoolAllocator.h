#ifndef _CBPOOLALLOCATOR_H
#define _CBPOOLALLOCATOR_H
#include <assert.h>
#include <cstring>
#include "typedefs.h"

struct CBPoolBlock
{
    
    union PoolBlockData 
    {
        // This is a union since memory content and next pointer
        // are never needed at the same time.
        // This helps reduce block size and prevent ruining 
        // alignment.

        //void* _mem; // make sure this sits at top of PoolBlock struct cuz it will be cast.
        CBPoolBlock *_next;
    };

    PoolBlockData _data;

    // Ensure the array allocated is 16-byte aligned
    //void* operator new[](size_t size) { return _aligned_malloc(size, 16); }
    //void operator delete[](void* ptr) { _aligned_free(ptr); }
private:
    // prevent allocation of single block.
    //void* operator new(size_t size) {}
    //void operator delete(void *ptr) {}
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
    byte* pMarker = reinterpret_cast<byte*>(m_pBlocks);
    // Place blocks in mem arena.
    for (int i = 0; i < m_nTotalBlocks; ++i)
    {
        new (reinterpret_cast<CBPoolBlock*>(pMarker)) CBPoolBlock();
        //reinterpret_cast<CBPoolBlock*>(pMarker)->_data._mem = static_cast<void*>(pMarker);
        pMarker += (size_t)m_blockSize;
    }

    // Setup the free list.
    m_pFreeList = m_pBlocks;
    pMarker = reinterpret_cast<byte*>(m_pFreeList);
    for (UINT32 i = 0; i < m_nTotalBlocks - 1; ++i)
    {
        reinterpret_cast<CBPoolBlock*>(pMarker + i * m_blockSize)->_data._next = 
        reinterpret_cast<CBPoolBlock*>(pMarker + (i + 1) * m_blockSize);
        //m_pFreeList[i]._data._next = &m_pFreeList[i + 1];
    }
    reinterpret_cast<CBPoolBlock*>(pMarker +(m_nTotalBlocks - 1) * m_blockSize)->_data._next = nullptr;
}



void CBMemPool::Shutdown()
{
    delete[] m_pBlocks;
    m_pFreeList = nullptr;
}



inline void* CBMemPool::Allocate(size_t size)
{
    assert(size <= m_blockSize && "Cannot allocate larger than pool size!");
    assert(m_nFreeBlocks > 0 && "No more free blocks in arena!");
    
    void* toRet = (void*)m_pFreeList;
    m_pFreeList = m_pFreeList->_data._next;
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
    blockPtr->_data._next = m_pFreeList;
    m_pFreeList = blockPtr;

    ++m_nFreeBlocks;
}



inline size_t CBMemPool::Capacity()
{
    return m_blockSize * m_nTotalBlocks;
}


#endif
