#ifndef _VEC3_H
#define _VEC3_H
#include <xmmintrin.h>
#include <smmintrin.h>
#include <exception>

struct Vec3
{
    __m128 _data;

    __forceinline Vec3() {}

    // W set to 1.0f, representing a point.
    __forceinline Vec3(float xIn, float yIn, float zIn)
    {
        _data = _mm_setr_ps(xIn, yIn, zIn, 1.0f);
    }
    __forceinline Vec3(float xIn, float yIn, float zIn, float wIn)
    {
        _data = _mm_setr_ps(xIn, yIn, zIn, wIn);
    }
        __forceinline Vec3(const __m128 &data)
    {
        _data = data;
    }


    ~Vec3() {};

    // ! Access individual element from __m128, slow.
    __forceinline float x() const { return _data.m128_f32[0]; }
    __forceinline float y() const { return _data.m128_f32[1]; }
    __forceinline float z() const { return _data.m128_f32[2]; }
    __forceinline float w() const { return _data.m128_f32[3]; }

    // Common operators.
    __forceinline void operator=(const Vec3 &rhs)
    { 
        _data = rhs._data; 
    }
    __forceinline Vec3 operator+(const Vec3 &rhs)
    { 
        return Vec3(_mm_add_ps(_data, rhs._data)); 
    }
    __forceinline Vec3 operator-(const Vec3 &rhs)
    {
        return Vec3(_mm_sub_ps(_data, rhs._data));
    }
    // Scalar multiplication in global section.
    // Does not divide W.
    __forceinline Vec3 operator/(float divisor)
    {
        if (divisor == 0)
        {
            throw(std::exception("Vec3: divisor is zero!"));
        }
        __m128 temp = _mm_setr_ps(divisor, divisor, divisor, 1.0f);
        return Vec3(_mm_div_ps(_data, temp));
    }

    __forceinline void operator+=(const Vec3 &rhs)
    {
        _data = _mm_add_ps(_data, rhs._data);
    }
    __forceinline void operator-=(const Vec3 &rhs)
    {
        _data = _mm_sub_ps(_data, rhs._data);
    }
    __forceinline void operator*=(float scalar)
    {
        __m128 temp = _mm_setr_ps(scalar, scalar, scalar, 1.0f);
        _data =  _mm_mul_ps(_data, temp);
    }
    __forceinline void operator/=(float divisor)
    {
        if (divisor == 0)
        {
            throw(std::exception("Vec3: divisor is zero!"));
        }
        __m128 temp = _mm_setr_ps(divisor, divisor, divisor, 1.0f);
        _data = _mm_div_ps(_data, temp);
    }


    // Common operations.
    __forceinline void Add(const Vec3 &toAdd)
    { 
        _data = _mm_add_ps(_data, toAdd._data); 
    }
    __forceinline void Scale(float scalar)
    {
        (*this) *= scalar;
    }

    // Squared length of the vector.
    __forceinline float SqLen()
    {
        __m128 temp = _mm_dp_ps(_data, _data, 0b01110001);
        return temp.m128_f32[0];
    }

    // length of the vector.
    __forceinline float Len()
    {
        __m128 temp = _mm_dp_ps(_data, _data, 0b01110001);
        return _mm_sqrt_ss(temp).m128_f32[0];
    }

    // Test to see if it's a zero vector (x, y, z are zero)
    __forceinline bool IsZero()
    {
        return (x() == 0 && y() == 0 && z() == 0);
    }



    // Normalize the vector. Notice that since only Vector
    // can be normalized, W component gets set to 1.
    __forceinline void Normalize()
    {
        //if (x() == 0 && y() == 0 && z() == 0) // zero vector
        //    return;

        //if(_data.m128_f32[4] == 0)
        //    _data.m128_f32[4]

        //__m128 temp = _mm_dp_ps(_data, _data, 0b01111111);
        //if (temp.m128_f32[0] == 0) //zero vector.
        //    return;

        //temp = _mm_rsqrt_ps(temp);

    }

    // Normalize
    // Normalized
    // Dot product
    // Cross product


};

#pragma region Global Vec3 functions and operators

// Does not scale W.
__forceinline Vec3 operator*(const Vec3& vec3, float scalar)
{
    __m128 temp = _mm_setr_ps(scalar, scalar, scalar, 1.0f);
    return Vec3(_mm_mul_ps(vec3._data, temp));
}
__forceinline Vec3 operator*(float scalar, const Vec3& vec3)
{
    return vec3 * scalar;
}

#pragma endregion


#endif // 

