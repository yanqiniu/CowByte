#ifndef _CBQUATERNION_H
#define _CBQUATERNION_H

#include <xmmintrin.h>
#include <smmintrin.h>
#include <cmath>

#include "simdhelper.h"

//////////////////////////////////////////////////////////////////////////
/*

CBE SIMD fast quaternion

*/
//////////////////////////////////////////////////////////////////////////

class Vec3;

__declspec(align(16)) struct Quaternion
{
    union
    {
        __m128 _data;
        float  _q[4];
    };

    // Default constructor that does nothing.
    __forceinline Quaternion() {}

    __forceinline Quaternion(float qVx, float qVy, float qVz, float qS)
    {
        _data = _mm_setr_ps(qVx, qVy, qVz, qS);
    }

    Quaternion(const Vec3 &axis, float angleInRad);
};

#endif
