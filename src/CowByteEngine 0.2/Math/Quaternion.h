#ifndef _CBQUATERNION_H
#define _CBQUATERNION_H

#include <xmmintrin.h>
#include <smmintrin.h>
#include <cmath>

#include "simdhelper.h"

//////////////////////////////////////////////////////////////////////////
/*

CBE SIMD fast quaternion.

Notice that CBE only works with normalized quat for simplicity and speed!

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

#pragma region Con/Destructor, Getter/Setter
    // Default constructor that does nothing.
    __forceinline Quaternion() {}

    __forceinline Quaternion(float qVx, float qVy, float qVz, float qS)
    {
        _data = _mm_setr_ps(qVx, qVy, qVz, qS);
    }

    Quaternion(const Vec3 &axis, float angleInRad);

    Quaternion(const Vec3 &toConvert);

    __forceinline Quaternion(const __m128 &inData) :
        _data(inData)
    {
    }
#pragma endregion

#pragma region Common operations

    __forceinline const Quaternion Conjugate() const
    {
        return Quaternion(_mm_mul_ps(_data, _mm_setr_ps(-1.0f, -1.0f, -1.0f, 1.0f)));
    }

    // Inverse the current quat, assuming it's unit.
    __forceinline void Inverse()
    {
        _data = _mm_mul_ps(_data, _mm_setr_ps(-1.0f, -1.0f, -1.0f, 1.0f));
    }

    // Return inversed version of this quat, assuming it's unit.
    __forceinline const Quaternion Inversed() const
    {
        return Conjugate();
    }

    __forceinline float SqrLen() const
    {
        return _mm_dp_ps(_data, _data, 0b11110001).m128_f32[0];
    }

    __forceinline float Len() const
    {
        return _mm_sqrt_ss(_mm_dp_ps(_data, _data, 0b11110001)).m128_f32[0];
    }

    __forceinline void Normalize()
    {
        __m128 temp = _mm_dp_ps(_data, _data, 0b11111111);
        temp = _mm_rsqrt_ps(temp);
        _data = _mm_mul_ps(_data, temp);
    }

    // P.Mul(Q) means a rotation Q followed by P.
    __forceinline void Mul(const Quaternion &q)
    {
        __m128 qV = _CB_INSERT_SINGLE(q._data, 0, 3);
        __m128 pV = _CB_INSERT_SINGLE(  _data, 0, 3);

        __m128 vecPart = _mm_mul_ps(_mm_shuffle_ps(_data, _data, _CB_SHUFFLE_R(3, 3, 3, 3)), qV);                    // pS * qV
        vecPart = _mm_add_ps(vecPart, _mm_mul_ps(_mm_shuffle_ps(q._data, q._data, _CB_SHUFFLE_R(3, 3, 3, 3)), pV)); // ..+ qS * pV

        // Cross pV x qV
        __m128 cross = _mm_mul_ps(_mm_shuffle_ps(pV, pV, _MM_SHUFFLE(3, 0, 2, 1)), 
                                   _mm_shuffle_ps(qV, qV, _MM_SHUFFLE(3, 1, 0, 2)));
        cross = _mm_sub_ps(cross, _mm_mul_ps(_mm_shuffle_ps(pV, pV, _MM_SHUFFLE(3, 1, 0, 2)),
                                             _mm_shuffle_ps(qV, qV, _MM_SHUFFLE(3, 0, 2, 1))));
        vecPart = _mm_add_ps(vecPart, cross); // .. + pV x qV

        __m128 scalarPart = _mm_dp_ps(_data, _mm_mul_ps(q._data, _mm_setr_ps(-1.0f, -1.0f, -1.0f, 1.0f)), 0b11111000); //(pS * qS - pV . qV)

        _data = _mm_add_ps(vecPart, scalarPart);
    }

#pragma endregion

#pragma region Common Operators

    __forceinline void operator*=(const Quaternion &q)
    {
        Mul(q);
    }

    __forceinline Quaternion operator*(const Quaternion &q)
    {
        Quaternion toRet = (*this);
        toRet.Mul(q);
        return toRet;
    }


#pragma endregion

#pragma region Vec3 and Matrix4x4 Contents



#pragma endregion



    // Conjugate of unit quaternion.



};

#endif
