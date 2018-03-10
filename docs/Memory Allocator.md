# CowByte Engine Memory Allocator 

### Overview
 - The project uses pool allocators. More specifically, it has a series of pool allocators of different sizes. during wrong time, size of the allovation is taken in to account to decide which pool will be handing out memory / freed from .

### Objcects
CBMemArena:
```C++
class CBMemArena
{
public:
    CBMemArena();
    void Initialize(); // Create pools with g_poolConfigs, and initialize each pool.
    void* Allocate(size_t size);
    void Free(void* ptr, size_t size); // size is needed to determine which pool it was in.
    __forceinline static CBMemArena &Get() { return s_instance; }
private:
    static CBMemArena s_instance;
    void *m_pMemory; // start of the memory arena.
    CBMemPool *m_pPools[g_MaxNumMemPools]; // pointers to all pools
    size_t m_TotalByteCapacity;
#ifdef _WIN64
    int _padding[2];
#else
    int _padding;
#endif
};
```
 - This is the memory arena objects that the engine uses. Implemented as a singleton.
 - Initialize(): the mem arena objects malloc() a large chunk of memory on the heap that will be used for almost all dynamic allocation used by the engine. It then segments the chunk by creating all the pools with configs provided in g_poolConfigs[][2]. By the end of Initialize(), m_pMemory will point to the large chunk of mem, m_pPools[] will be filled with pointers to all the pool objects.
 - Allocate() and Free(): given the size of the allocation, walk through g_poolConfigs[][2] to find the right pool that should handle the request, and call it's Allocate()/Free() function.

CBPoolBlock:
```C++
struct CBPoolBlock
{
    CBPoolBlock *_next;
#ifndef _WIN64
    UINT32 _padding[3]; // x86 padding
#else
    UINT32 _padding[2]; // x64 padding
#endif // !_WIN64
};
```
- This represents a block in a pool, or just the header of that mem block.
- _next is overwritten when actual data is filled in. This should be fine, since we do not need pointer to the next block when we are using the data, only before using the data - to update the freelist. After the memory is freed, the pointer is refreshed again when the block is added back to the free list.

CBMemPool:
```C++
struct CBMemPool
{
    explicit CBMemPool(UINT32 blockSize, UINT32 nBlocks);
    ~CBMemPool();

    void Initialize(void *startPtr); //Allocate the pool on the heap, setup free list.
    void Shutdown(); // Free memory used by the pool.

    void* Allocate(size_t size);      // Return address to a free block.
    void Free(void* ptr);     // Free memory and add block back to free list.
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
}
```
 - Initialize(void *startPtr): startPtr is given by CBMemArena to decide where exactly the pool starts at. The pool places the first block at startPtr, and then the next one at (startPtr + m_blockSize), and so on. It then setup the freelist using these address. Notice that for this setup, the minimum block size the memory arena specify (currently 16 byte) must be >= 16 byte, as that's how much CBPoolBlock takes with padding.
 - Allocate() and Free are somewhat standard pool allocator operations. 


### Known Issues / Notices
 - As mentioned above, Minimum allocation size should be larger than 16.
 - Operator overload on new, new[], delete, delete[] are defined in CBMemory.h. However please notice that they might not get called 100% of the time. Please read the header file for details.

 #[End]



