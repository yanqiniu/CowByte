#ifndef _VEC3_H
#define _VEC3_H
#include <xmmintrin.h>
#include <smmintrin.h>
#include <exception>

class Matrix4x4;

__declspec(align(16)) struct Vec3
{
    __m128 _data;
    Vec3();

    // W set to 1.0f, representing a point.
    Vec3(float xIn, float yIn, float zIn);
    Vec3(float xIn, float yIn, float zIn, float wIn);
    Vec3(const __m128 &data);
    ~Vec3() {};

    // ! Access individual element from __m128, slow.
    float X() const;
    float Y() const;
    float Z() const;
    float W() const;
    void Set(float xIn, float yIn, float zIn, float wIn);

    void operator=(const Vec3 &rhs);
    Vec3 operator+(const Vec3 &rhs);
    Vec3 operator-(const Vec3 &rhs);
    Vec3 operator/(float divisor);

    void operator+=(const Vec3 &rhs);
    void operator-=(const Vec3 &rhs);
    void operator*=(float scalar);
    void operator/=(float divisor);

    void Add(const Vec3 &toAdd);
    void Scale(float scalar);

    // Squared length of the vector.
    float SqLen();
    // Length of the vector.
    float Len();
    // Test to see if it's a zero vector (x, y, z are zero)
    bool IsZero();
    // Set W to 1.
    void ToPoint();
    // Set W to 0.
    void ToDir();
    // Normalize the vector. Notice that since only Vector
    // can be normalized, W component gets set to 0.
    void Normalize();
    // Return a normalized version of the Vec3.
    // Notice this DOES NOT modify _data.
    Vec3 Normalized();
    float Dot(const Vec3 &toDot);
    // Return cross vector. Does not change Data.
    Vec3 Cross(const Vec3 &toCross);
    Vec3 Mul(const Matrix4x4 &matrix);
    Vec3 operator*(const Matrix4x4 &matrix);
    
};

Vec3 operator*(const Vec3& vec3, float scalar);
Vec3 operator*(float scalar, const Vec3& vec3);


#endif