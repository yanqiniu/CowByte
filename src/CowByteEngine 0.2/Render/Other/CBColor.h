#ifndef _CBCOLOR_H
#define _CBCOLOR_H
#include <xmmintrin.h>

#include "../../Memory/CBMemory.h"
#include "../../Utils/typedefs.h"

// TODO: make this simd.
struct ColorData
{
    FLOAT32 r;
    FLOAT32 g;
    FLOAT32 b;
    FLOAT32 a;
};

__declspec(align(16)) struct CBColor
{
    CBMEM_OVERLOAD_NEW_DELETE(CBColor);

    union
    {
        __m128 _data;
        FLOAT32 _colors;
    };

    // Default to black.
    __forceinline CBColor()
    {
        _data = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
    }

    __forceinline CBColor(float rIn, float gIn, float bIn, float aIn)
    {
        _data = _mm_setr_ps(rIn, gIn, bIn, aIn);
    }

    __forceinline void Set(float rIn, float gIn, float bIn, float aIn)
    {
        _data = _mm_setr_ps(rIn, gIn, bIn, aIn);
    }
};

#endif // _CBCOLOR_H
