#ifndef _CBDEBUG_H
#define _CBDEBUG_H
#include <stdio.h>
#include <assert.h>
#include <exception>
#include <d3d11_1.h>

#define DbgINFO(format, ...) printf("INFO: " format "\n", ##__VA_ARGS__)
#define DbgWARNING(format, ...) printf("WARNING: " format "\n", ##__VA_ARGS__)
#define DbgERROR(format, ...) printf("ERROR: " format "\n", ##__VA_ARGS__)

inline static void DbgAssert(bool expression, const char* msg)
{
    if (!expression)
    {
        DbgERROR("Assertion failed: %s", msg);
        assert(expression);
    }
}

namespace DirectX
{
    inline bool ResultNotFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            return false;
        }
        return true;
    }

    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
    }


}

#endif // !_DEBUG_H
