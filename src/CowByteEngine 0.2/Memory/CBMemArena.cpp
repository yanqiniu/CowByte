#include "CBMemArena.h"

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

void* CBMemArena::Allocate(size_t size)
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

void CBMemArena::Free(void* ptr, size_t size)
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
