#ifndef _VEC3_H
#define _VEC3_H
#include <xmmintrin.h>
#include <smmintrin.h>
#include <exception>

__declspec(align(16)) struct Vec3
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
    __forceinline Vec3 operator/(float divisor)
    {
        if (divisor == 0)
        {
            throw(std::exception("Vec3: divisor is zero!"));
        }
        __m128 temp = _mm_setr_ps(divisor, divisor, divisor, 1.0f);
        return Vec3(_mm_div_ps(_data, temp));
    }
    __forceinline friend Vec3 operator*(const Vec3& vec3, float scalar)
    {
        __m128 temp = _mm_setr_ps(scalar, scalar, scalar, 1.0f);
        return Vec3(_mm_mul_ps(vec3._data, temp));
    }
    __forceinline friend Vec3 operator*(float scalar, const Vec3& vec3)
    {
        return vec3 * scalar;
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
        _data = _mm_mul_ps(_data, temp);
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
        return _mm_sqrt_ss(_mm_dp_ps(_data, _data, 0b01110001)).m128_f32[0];
    }

    // Test to see if it's a zero vector (x, y, z are zero)
    __forceinline bool IsZero()
    {
        // Using dot product instead of checking x, y and z individually
        // since the later one introduces around 60% more time according 
        // to testing.
        return _mm_dp_ps(_data, _data, 0b01110001).m128_f32[0] == 0;
    }

    // Set W to 1.
    __forceinline void ToPoint()
    {
        _data = _mm_insert_ps(_data, _mm_set_ps1(1.0f), 0b11110000);
    }

    // Set W to 0.
    __forceinline void ToDir()
    {
        _data = _mm_insert_ps(_data, _mm_set_ps1(0.0f), 0b11110000);
    }



    // Normalize the vector. Notice that since only Vector
    // can be normalized, W component gets set to 0.
    __forceinline void Normalize()
    {
        __m128 temp = _mm_dp_ps(_data, _data, 0b01110111);
        temp = _mm_rsqrt_ps(temp);
        temp = _mm_insert_ps(temp, _mm_set_ps1(0.0f), 0b11110000); // set w of temp to 0.0f
        _data = _mm_mul_ps(_data, temp);
    }
    // Return a normalized version of the Vec3.
    // Notice this DOES NOT modify _data.
    __forceinline Vec3 Normalized()
    {
        __m128 temp = _mm_dp_ps(_data, _data, 0b01110111);
        temp = _mm_rsqrt_ps(temp);
        temp = _mm_insert_ps(temp, _mm_set_ps1(0.0f), 0b11110000); // set w of temp to 0.0f
        return Vec3(_mm_mul_ps(_data, temp));
    }

    // Dot product.
    __forceinline float Dot(const Vec3 &toDot)
    {
        return _mm_dp_ps(_data, toDot._data, 0b01110001).m128_f32[0];
    }

    // Return cross vector. Does not change Data.
    __forceinline Vec3 Cross(const Vec3 &toCross)
    {
        __m128 temp0 = _mm_shuffle_ps(_data, _data, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 temp1 = _mm_shuffle_ps(toCross._data, toCross._data, _MM_SHUFFLE(3, 1, 0, 2));
        __m128 result = _mm_mul_ps(temp0, temp1);

        temp0 = _mm_shuffle_ps(_data, _data, _MM_SHUFFLE(3, 1, 0, 2));
        temp1 = _mm_shuffle_ps(toCross._data, toCross._data, _MM_SHUFFLE(3, 0, 2, 1));
        return Vec3(_mm_sub_ps(result, _mm_mul_ps(temp0, temp1)));
    }

};

#if false
// Slow version, just wondering how much  speed improvement that was...
struct Vec3Slow
{
    float m_X, m_Y, m_Z, m_W;
    __forceinline Vec3Slow() : m_X(0), m_Y(0), m_Z(0), m_W(1.0) {}

    // W set to 1.0f, representing a point.
    __forceinline Vec3Slow(float xIn, float yIn, float zIn)
        : m_X(xIn), m_Y(yIn), m_Z(zIn), m_W(1.0) {}
    __forceinline Vec3Slow(float xIn, float yIn, float zIn, float wIn)
        : m_X(xIn), m_Y(yIn), m_Z(zIn), m_W(wIn) {}


    ~Vec3Slow() {};

    // Common operators.
    __forceinline void operator=(const Vec3Slow &rhs)
    {
        m_X = rhs.m_X;
        m_Y = rhs.m_Y;
        m_Z = rhs.m_Z;
        m_W = rhs.m_W;
    }

    __forceinline void Add(const Vec3Slow &rhs)
    {
        m_X += rhs.m_X;
        m_Y += rhs.m_X;
        m_Z += rhs.m_X;
        m_W += rhs.m_W;
    }


    // Squared length of the vector.
    __forceinline float SqLen()
    {
        return m_X * m_X + m_Y * m_Y + m_Z * m_Z;
    }

    // length of the vector.
    __forceinline float Len()
    {
        return sqrt(m_X * m_X + m_Y * m_Y + m_Z * m_Z);
    }

    // Test to see if it's a zero vector (x, y, z are zero)


    // Normalize the vector. Notice that since only Vector
    // can be normalized, W component gets set to 0.
    __forceinline void Normalize()
    {
        float length = Len();
        m_X /= length;
        m_Y /= length;
        m_Z /= length;
    }
    // Return a normalized version of the Vec3Slow.
    // Notice this DOES NOT modify _data.
    __forceinline Vec3Slow Normalized()
    {
        float length = Len();
        return Vec3Slow(m_X / length, m_Y / length, m_Z / length, m_W);
    }

    // Dot product.
    __forceinline float Dot(const Vec3Slow &toDot)
    {
        return m_X * toDot.m_X + m_Y * toDot.m_Y + m_Z * toDot.m_Z;
    }

    // Return cross vector. Does not change Data.
    __forceinline Vec3Slow Cross(const Vec3Slow &toCross)
    {
        float newX = m_Y * toCross.m_Z - m_Z * toCross.m_Y;
        float newY = m_Z * toCross.m_X - m_X * toCross.m_Z;
        float newZ = m_X * toCross.m_Y - m_Y * toCross.m_X;

        return Vec3Slow(newX, newY, newZ);
    }
};
#endif
#endif