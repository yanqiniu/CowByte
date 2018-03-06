#ifndef _CBPOOLALLOCATOR_H
#define _CBPOOLALLOCATOR_H
#include <assert.h>
#include <cstring>

// ! size must be divisible by 16!
template <size_t size>
struct PoolBlock
{
    
    union PoolBlockData 
    {
        // This is a union since memory content and next pointer
        // are never needed at the same time.
        // This helps reduce block size and prevent ruining 
        // alignment.

        char _mem[size]; // make sure this sits at top of PoolBlock struct cuz it will be cast.
        PoolBlock<size> *_next;
    };

    PoolBlockData _data;

    // Ensure the array allocated is 16-byte aligned
    void* operator new[](size_t size) { return _aligned_malloc(size, 16); }
    void operator delete[](void* ptr) { _aligned_free(ptr); }
private:
    // prevent allocation of single block.
    void* operator new(size_t size) {}
    void operator delete(void *ptr) {}
};

// Stands for: Fixed sized pool allocator / arena.
// Essentially we are gonna have a set of these with 
// varying sizes.
// ! blockByteSize must be divisible by 16!
template <size_t blockByteSize, unsigned int numBlocks>
class FSPoolAllocator
{
public:
    FSPoolAllocator();
    ~FSPoolAllocator();

    //Allocate the pool on the heap, setup free list.
    void Initialize();
    // Free memory used by the arena.
    void Shutdown();

    // Return address to a free block.
    void* Allocate(size_t size); 
    // Free memory and add block back to free list.
    void Free(void* ptr);

    size_t Capacity();


private:
    PoolBlock<blockByteSize> *m_pBlocks; // pointer to all blocks in the pool.
    PoolBlock<blockByteSize> *m_pFreeList; // head of the free block list.
    unsigned int m_NumFreeBlocks;
    int _padding;
};

template <size_t blockByteSize, unsigned int numBlocks>
FSPoolAllocator<blockByteSize, numBlocks>::FSPoolAllocator()
{
    Initialize();
}

template <size_t blockByteSize, unsigned int numBlocks>
FSPoolAllocator<blockByteSize, numBlocks>::~FSPoolAllocator()
{
    Shutdown();
}

template <size_t blockByteSize, unsigned int numBlocks>
void FSPoolAllocator<blockByteSize, numBlocks>::Initialize()
{
    assert(blockByteSize % 16 == 0 && "Block size must be divisible by 16!");

    // Allocate the blocks on the heap.
    m_pBlocks = new PoolBlock<blockByteSize>[numBlocks];
    
    // Setup the free list.
    m_pFreeList = m_pBlocks;
    for (unsigned int i = 0; i < numBlocks - 1; ++i)
    {
        m_pFreeList[i]._data._next = &m_pFreeList[i + 1];
    }
    m_pFreeList[numBlocks - 1]._data._next = nullptr;
    m_NumFreeBlocks = numBlocks;
}


template <size_t blockByteSize, unsigned int numBlocks>
void FSPoolAllocator<blockByteSize, numBlocks>::Shutdown()
{
    delete[] m_pBlocks;
    m_pFreeList = nullptr;
}


template <size_t blockByteSize, unsigned int numBlocks>
inline void* FSPoolAllocator<blockByteSize, numBlocks>::Allocate(size_t size)
{
    size_t t = blockByteSize;
    assert(size <= blockByteSize && "Cannot allocate larger than pool size!");
    assert(m_NumFreeBlocks > 0 && "No more free blocks in arena!");
    
    void* toRet = (void*)m_pFreeList->_data._mem;
    m_pFreeList = m_pFreeList->_data._next;
    memset(toRet, 0, blockByteSize);
    --m_NumFreeBlocks;
    return toRet;
}

template <size_t blockByteSize, unsigned int numBlocks>
inline void FSPoolAllocator<blockByteSize, numBlocks>::Free(void* ptr)
{
    if (ptr == nullptr)
        return;

    // CAUTION: casting to to block pointer.
    PoolBlock<blockByteSize> *blockPtr = reinterpret_cast<PoolBlock<blockByteSize>*>(ptr);

    // No need to reset the data, data block is wiped
    // during allocation.
    //memset(blockPtr, 0, blockByteSize);
    blockPtr->_data._next = m_pFreeList;
    m_pFreeList = blockPtr;

    ++m_NumFreeBlocks;
}


template <size_t blockByteSize, unsigned int numBlocks>
inline size_t FSPoolAllocator<blockByteSize, numBlocks>::Capacity()
{
    return blockByteSize * numBlocks;
}


#endif
