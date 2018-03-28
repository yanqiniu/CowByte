#ifndef _CB_MEM_ARENA_H
#define _CB_MEM_ARENA_H
#include <malloc.h>
#include "../Utils/CBDebug.h"
#include "CBPoolAllocator.h"

const static size_t g_MaxNumMemPools = 32;

// TODO: yea... this needs to be tweaked.
const static UINT32 g_poolConfigs[g_MaxNumMemPools][2] = {
// { Block size, num of blocks }
{ 16, 4096 },             //16B For smaller allocation like a single Vec3.
{ 32, 1024 },            //32B
{ 64, 1024 },            //64B For single matrix.
{ 80, 1024 },            //80B
{ 96, 1024 },            //96B
{ 128, 1024 },           //128B
{ 256, 1024 },           //256B
{ 512, 1024 },           //512B
{ 0x400, 1024 },         //1KB;
{ 0x800, 1024 },         //2KB;
{ 0x1000, 1024 },        //4KB;
{ 0x2000, 1024 },        //8KB;
{ 0x4000, 1024 },        //16KB;
{ 0x8000, 1024 },        //32KB;
{ 0x10000, 512 },       //64KB;
{ 0x20000, 256 },       //128KB;
{ 0x40000, 128 },       //256KB;
{ 0x80000, 64 },       //512KB;
{ 0x100000, 32 },      //1MB;
{ 0x1000000, 16 }       //4MB;
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

#ifdef _DEBUG
    size_t m_NumBytesRequested; // sum of data size
    size_t m_NumBytesUsed; // sum of block size
#ifndef _WIN64
    int _debugPadding[2];
#endif
#endif

#ifndef _WIN64
    int _padding[2];
#endif

};


#endif // !_CB_MEM_ARENA_H
