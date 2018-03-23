#include "CBMemArena.h"

CBMemArena::CBMemArena() :
    m_TotalByteCapacity(0)
#ifdef _DEBUG
    , m_NumBytesUsed(0),
    m_NumBytesRequested(0)
#endif // _DEBUG
{
    // Calculate the total amount of bytes the will 
    // be allocated.

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
    // Allocate all memory needed:
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
            // overridden.
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
    // TODO: threading
    for (const auto& pPool : m_pPools)
    {
        if (pPool == NULL)
            break;
        if (size <= pPool->m_blockSize)
        {
#ifdef _DEBUG            
            m_NumBytesRequested += size;
            m_NumBytesUsed += pPool->m_blockSize;
#endif // DEBUG
            return pPool->Allocate(size);
        }
    }
    DbgWARNING("Does not support dynamic allocation size of [%u]", size);
    return nullptr;
}

void CBMemArena::Free(void* ptr, size_t size)
{
    // TODO: threading
    for (const auto& pPool : m_pPools)
    {
        if (size <= pPool->m_blockSize)
        {
#ifdef _DEBUG            
            m_NumBytesRequested -= size;
            m_NumBytesUsed -= pPool->m_blockSize;
#endif // DEBUG
            pPool->Free(ptr);
            return;
        }
    }
}
