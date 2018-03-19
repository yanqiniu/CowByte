#include "Quaternion.h"
#include "Vec3.h"


Quaternion::Quaternion(const Vec3 &axis, float angleInRad)
{
    // Axis must be a unit vector!
    // It isn't normalized in this function because normalization
    // is very costly so do it before calling this if you need to.

    _data = axis._data;
    _data = _mm_mul_ps(_data, _mm_set1_ps(std::sinf(angleInRad / 2)));
    _data = _CB_INSERT_SINGLE(_data, std::cosf(angleInRad / 2), 3);
}

// Converting a vector to a quat.
Quaternion::Quaternion(const Vec3 &toConvert)
{
    _data = toConvert._data;
    _CB_INSERT_SINGLE(_data, 0.0f, 3);
}

