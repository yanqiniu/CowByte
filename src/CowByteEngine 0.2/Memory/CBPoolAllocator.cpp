#include "CBPoolAllocator.h"


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
    reinterpret_cast<CBPoolBlock*>(pMarker + (m_nTotalBlocks - 1) * m_blockSize)->_next = nullptr;
}



void CBMemPool::Shutdown()
{
    delete[] m_pBlocks;
    m_pFreeList = nullptr;
}



void* CBMemPool::Allocate(size_t size)
{
    assert(size <= m_blockSize && "Cannot allocate larger than pool size!");
    assert(m_nFreeBlocks > 0 && "No more free blocks in pool!");
    CBPoolBlock* toRet = m_pFreeList;
    m_pFreeList = m_pFreeList->_next;
    //memset(toRet, 0, sizeof(CBPoolBlock)); // At last, erase next pointer and padding
    --m_nFreeBlocks;
    return toRet;
}


void CBMemPool::Free(void* ptr)
{
    if (ptr == nullptr)
        return;

    // CAUTION: casting to to block pointer.
    CBPoolBlock *blockPtr = reinterpret_cast<CBPoolBlock*>(ptr);

    memset(ptr, 0, m_blockSize);
    blockPtr->_next = m_pFreeList;
    m_pFreeList = blockPtr;

    ++m_nFreeBlocks;
}



inline size_t CBMemPool::Capacity()
{
    return m_blockSize * m_nTotalBlocks;
}
