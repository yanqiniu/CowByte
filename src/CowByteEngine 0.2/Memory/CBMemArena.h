#ifndef _CB_MEM_ARENA_H
#define _CB_MEM_ARENA_H
#include "../Utils/CBDebug.h"
#include "CBPoolAllocator.h"

#define MAX_NUM_POOLS 16

const static size_t g_MaxNumMemPools = 16;

// TODO: yea... this needs to be tweaked.
const static UINT32 g_poolConfigs[g_MaxNumMemPools][2] = {
        // Block size, num of blocks
        {16, 1024},            //16B For smaller allocation like a single Vec3.
        {64, 1024},            //64B For single matrix.
        {0x400, 1024},         //1KB;
        {0x800, 1024},         //2KB;
        {0x1000, 1024},        //4KB;
        {0x2000, 1024},        //8KB;
        {0x4000, 1024},        //16KB;
        {0x8000, 1024},        //32KB;
        {0x10000, 1024},       //64KB;
        {0x20000, 1024},       //128KB;
        {0x40000, 1024},       //256KB;
        {0x80000, 1024},       //512KB;
        {0x100000, 1024}       //1MB;
};


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

CBMemArena::CBMemArena()
{
    // Calculate the total amount of bytes the will 
    // be allocated.
    m_TotalByteCapacity = 0;
    for (const auto& config : g_poolConfigs)
    {
        assert(config[0] % 16 == 0 && "Block size must be divisible by 16!");
        m_TotalByteCapacity += (config[0] * config[1]);
    }

    Initialize();
}

CBMemArena CBMemArena::s_instance;

void CBMemArena::Initialize()
{
    // Allocate all mem needed:
    m_pMemory = _aligned_malloc(m_TotalByteCapacity, 16);

    DbgAssert(m_pMemory, "Failed to allocate requested amount of memory for arena.");

    char* pMarker = static_cast<char*>(m_pMemory);
    int i = 0;
    for (const auto& config : g_poolConfigs)
    {
        if (config[0]) // skip empty entries created for padding.
        {
            // TODO: this smells like "somewhere on the heap"...
            // Create pool header. Using placement new because likely new has been 
            // overridden now.
            m_pPools[i] = static_cast<CBMemPool*>(_aligned_malloc(sizeof(CBMemPool), 16));
            m_pPools[i] = new(m_pPools[i]) CBMemPool(config[0], config[1]);

            // Create blocks, and setup freelist.
            m_pPools[i]->Initialize(pMarker);

            // Advance marker.
            pMarker += (config[0] * config[1]);
            ++i;
        }
    }
}

inline void* CBMemArena::Allocate(size_t size)
{
    for (const auto& pPool : m_pPools)
    {
        if (size <= pPool->m_blockSize)
        {
            return pPool->Allocate(size);
        }
    }
    DbgWARNING("Does not support dynamic allocation size of [%u]", size);
    return nullptr;
}

inline void CBMemArena::Free(void* ptr, size_t size)
{
    for (const auto& pPool : m_pPools)
    {
        if (size <= pPool->m_blockSize)
        {
            pPool->Free(ptr);
            return;
        }
    }
}

#endif // !_CB_MEM_ARENA_H
