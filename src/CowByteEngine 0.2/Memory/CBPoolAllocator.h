#ifndef _CBPOOLALLOCATOR_H
#define _CBPOOLALLOCATOR_H
#include <assert.h>
#include <cstring>
#include <new.h>
#include "../Utils/typedefs.h"

#define VALID_INTEGRITY static_cast<UINT32>(0xdeadbeef)

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
    UINT32 _integrity; // Set to 0xdeadbeef when the block is free. 
#ifdef _WIN64
    UINT32 _padding;
#endif
    CBPoolBlock *_next;

#ifndef _WIN64
    UINT32 _padding[2]; // x86 padding
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


#endif
