#include "Matrix4x4.h"
#include "Vec3.h"
#include "Quaternion.h"

Vec3 Matrix4x4::Right() const
{
    //return Vec3(this->_data[0]);
    return Vec3(_m[0][0], _m[1][0], _m[2][0], 0.0f);
}

Vec3 Matrix4x4::Up() const
{
    //return Vec3(this->_data[1]);
    return Vec3(_m[0][1], _m[1][1], _m[2][1], 0.0f);
}

Vec3 Matrix4x4::Front() const
{
    //return Vec3(this->_data[2]);
    return Vec3(_m[0][2], _m[1][2], _m[2][2], 0.0f);
}

// Is not inline. Prefer Translate (float, float, float).
Matrix4x4 Matrix4x4::Translate(const Vec3 &vec)
{
    return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        vec.X(), vec.Y(), vec.Z(), 1.0f);
}


Matrix4x4 Matrix4x4::Rotation(const Quaternion &quat)
{
    Matrix4x4 toRet;
    __m128 temp0 = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
    __m128 temp1 = _mm_setr_ps(-2.0f, 2.0f, 2.0f, 0.0f);
    __m128 temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(1, 0, 0, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(1, 1, 2, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp0 = _mm_add_ps(temp0, temp1); 
    temp1 = _mm_setr_ps(-2.0f, -2.0f, 2.0f, 0.0f);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(2, 2, 1, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(2, 3, 3, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    toRet._data[0] = _mm_add_ps(temp0, temp1); // first col done.

    temp0 = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
    temp1 = _mm_setr_ps(2.0f, -2.0f, 2.0f, 0.0f);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(0, 0, 1, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(1, 0, 2, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp0 = _mm_add_ps(temp0, temp1);
    temp1 = _mm_setr_ps(2.0f, -2.0f, -2.0f, 0.0f);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(2, 2, 0, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(3, 2, 3, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    toRet._data[1] = _mm_add_ps(temp0, temp1); // second col done.

    temp0 = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
    temp1 = _mm_setr_ps(2.0f, 2.0f, -2.0f, 0.0f);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(0, 1, 0, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(2, 2, 0, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp0 = _mm_add_ps(temp0, temp1);
    temp1 = _mm_setr_ps(-2.0f, 2.0f, -2.0f, 0.0f);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(1, 0, 1, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    temp2 = _mm_shuffle_ps(quat._data, quat._data, _CB_SHUFFLE_R(3, 3, 1, 0));
    temp1 = _mm_mul_ps(temp1, temp2);
    toRet._data[2] = _mm_add_ps(temp0, temp1); // third col done.

    toRet._data[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f); // forth col done.

    return toRet;
}

Matrix4x4 Matrix4x4::LocalRotation(const Quaternion &quat) const
{
    Matrix4x4 toRet = Matrix4x4::Identity();
    Matrix4x4 t = this->GetTranslation();
    toRet *= t.Inversed();
    toRet *= Matrix4x4::Rotation(quat);
    toRet *= t;

    return toRet;
}

#pragma endregion