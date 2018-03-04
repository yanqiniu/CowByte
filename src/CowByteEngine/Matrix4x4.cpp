#include "Matrix4x4.h"
#include "Vec3.h"


Matrix4x4 Matrix4x4::Translate(Vec3 *vec)
{
    return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        vec->X(), vec->Y(), vec->Z(), 1.0f);
}


#pragma endregion