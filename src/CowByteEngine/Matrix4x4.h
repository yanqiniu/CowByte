#ifndef _MATRIX4x4_H
#define _MATRIX4x4_H
#include <xmmintrin.h>
#include <smmintrin.h>

//////////////////////////////////////////////////////////////////////////
/*

CBE SIMD fast 4x4 matrix struct.

Notice that CBE expects row vectors. 
That is, when mul with a matrix, do vec4 * matrix4x4.
Each __m128 in _data represent a column in the matrix.

*/
//////////////////////////////////////////////////////////////////////////

class Vec3;

__declspec(align(16)) struct Matrix4x4
{
    __m128 _data[4]; // 4 columns, row vector

    // Default constructor that does nothing.
    Matrix4x4();
    // Row major constructor.
    Matrix4x4(float e11, float e12, float e13, float e14,
                            float e21, float e22, float e23, float e24,
                            float e31, float e32, float e33, float e34,
                            float e41, float e42, float e43, float e44);
    Matrix4x4(float rows[4][4]);
    // Row major list constructor. This constructor takes 
    // a pointer so it's not as safe, but it's efficient.
    Matrix4x4(float *e);

    float At(size_t row, size_t col);
    void Transpose();

    static Matrix4x4 Identity();
    static Matrix4x4 Translate(float x, float y, float z);
    static Matrix4x4 Translate(Vec3 *vec);
};


#endif // !_MATRIX4x4_H
