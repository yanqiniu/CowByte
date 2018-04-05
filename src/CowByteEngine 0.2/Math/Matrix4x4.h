#ifndef _MATRIX4x4_H
#define _MATRIX4x4_H
#include <xmmintrin.h>
#include <smmintrin.h>
#include <cmath>

#include "simdhelper.h"
#include "../Memory/CBMemory.h"

//////////////////////////////////////////////////////////////////////////
/*

CBE SIMD fast 4x4 matrix struct.

Notice that CBE expects row vectors. 
That is, when mul with a matrix, do vec4 * matrix4x4.
Each __m128 in _data represent a column in the matrix.

*/
//////////////////////////////////////////////////////////////////////////

class Vec3;
class Quaternion;

__declspec(align(16)) struct Matrix4x4
{
    CBMEM_OVERLOAD_NEW_DELETE(Matrix4x4)

    union
    {
        __m128 _data[4]; // 4 columns, row vector
        float _m[4][4]; // [col][row]
    };

#pragma region Con/Destructor, Getter/Setter

    // Default constructor that does nothing.
    __forceinline Matrix4x4()
    {
    }

    // Row major constructor.
    __forceinline Matrix4x4(float e11, float e12, float e13, float e14,
        float e21, float e22, float e23, float e24,
        float e31, float e32, float e33, float e34,
        float e41, float e42, float e43, float e44)
    {
        _data[0] = _mm_setr_ps(e11, e21, e31, e41);
        _data[1] = _mm_setr_ps(e12, e22, e32, e42);
        _data[2] = _mm_setr_ps(e13, e23, e33, e43);
        _data[3] = _mm_setr_ps(e14, e24, e34, e44);
    }

    __forceinline Matrix4x4(float rows[4][4]) :
        Matrix4x4(rows[1][1], rows[1][2], rows[1][3], rows[1][4],
            rows[2][1], rows[2][2], rows[2][3], rows[2][4],
            rows[3][1], rows[3][2], rows[3][3], rows[3][4],
            rows[4][1], rows[4][2], rows[4][3], rows[4][4])
    {

    }

    // Row major list constructor. This constructor takes 
    // a pointer so it's not as safe, but it's efficient.
    __forceinline Matrix4x4(float *e)
    {
        _data[0] = _mm_setr_ps(e[0], e[4], e[8], e[12]);
        _data[1] = _mm_setr_ps(e[1], e[5], e[9], e[13]);
        _data[2] = _mm_setr_ps(e[2], e[6], e[10], e[14]);
        _data[3] = _mm_setr_ps(e[3], e[7], e[11], e[15]);
    }

    __forceinline float At(size_t row, size_t col)
    {
        return _data[col].m128_f32[row];
    }

    __forceinline ~Matrix4x4() {}

    Vec3 Right() const;
    Vec3 Up() const;
    Vec3 Front() const;

#pragma endregion

#pragma region Common Operations

    void Transpose()
    {
        _MM_TRANSPOSE4_PS(_data[0], _data[1], _data[2], _data[3]);
    }

    __forceinline void Mul(const Matrix4x4 &matrix)
    {
        __m128 temp[4] = {_data[0], _data[1], _data[2], _data[3]};
        _MM_TRANSPOSE4_PS(temp[0], temp[1], temp[2], temp[3]);

        // First col
        _data[0] = _mm_dp_ps(temp[0], matrix._data[0], 0b11110001);// x
        _data[0] = _mm_insert_ps(_data[0], _mm_dp_ps(temp[1], matrix._data[0], 0b11110001), 0b00010000); // y
        _data[0] = _mm_insert_ps(_data[0], _mm_dp_ps(temp[2], matrix._data[0], 0b11110001), 0b00100000); // z
        _data[0] = _mm_insert_ps(_data[0], _mm_dp_ps(temp[3], matrix._data[0], 0b11110001), 0b00110000); // w

        // Second col
        _data[1] = _mm_dp_ps(temp[0], matrix._data[1], 0b11110001);// x
        _data[1] = _mm_insert_ps(_data[1], _mm_dp_ps(temp[1], matrix._data[1], 0b11110001), 0b00010000); // y
        _data[1] = _mm_insert_ps(_data[1], _mm_dp_ps(temp[2], matrix._data[1], 0b11110001), 0b00100000); // z
        _data[1] = _mm_insert_ps(_data[1], _mm_dp_ps(temp[3], matrix._data[1], 0b11110001), 0b00110000); // w

        // Third col
        _data[2] = _mm_dp_ps(temp[0], matrix._data[2], 0b11110001);// x
        _data[2] = _mm_insert_ps(_data[2], _mm_dp_ps(temp[1], matrix._data[2], 0b11110001), 0b00010000); // y
        _data[2] = _mm_insert_ps(_data[2], _mm_dp_ps(temp[2], matrix._data[2], 0b11110001), 0b00100000); // z
        _data[2] = _mm_insert_ps(_data[2], _mm_dp_ps(temp[3], matrix._data[2], 0b11110001), 0b00110000); // w

        // Forth col
        _data[3] = _mm_dp_ps(temp[0], matrix._data[3], 0b11110001);// x
        _data[3] = _mm_insert_ps(_data[3], _mm_dp_ps(temp[1], matrix._data[3], 0b11110001), 0b00010000); // y
        _data[3] = _mm_insert_ps(_data[3], _mm_dp_ps(temp[2], matrix._data[3], 0b11110001), 0b00100000); // z
        _data[3] = _mm_insert_ps(_data[3], _mm_dp_ps(temp[3], matrix._data[3], 0b11110001), 0b00110000); // w
    }
    __forceinline void operator*=(const Matrix4x4 &matrix)
    {
        Mul(matrix);
    }
    __forceinline Matrix4x4 operator*(const Matrix4x4 &matrix) const
    {
        Matrix4x4 toRet = *this;
        toRet *= matrix;
        return toRet;
    }


#pragma endregion

#pragma region Transformation

    __forceinline static Matrix4x4 Identity()
    {
        return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);
    }

    // fovy is in radian here.
    __forceinline static Matrix4x4 PerspectiveProjection(float ar, float fovy, float np, float fp)
    {
        return Matrix4x4(1.0f / (ar * std::tan(fovy / 2)), 0.0f,                      0.0f,                   0.0f,
                         0.0f,                             1.0f / std::tan(fovy / 2), 0.0f,                   0.0f,
                         0.0f,                             0.0f,                      1 / (fp - np),          1.0f,
                         0.0f,                             0.0f,                      - (np / (fp - np)),     0.0f);

    }
    __forceinline static Matrix4x4 Scale(float x, float y, float z)
    {
        return Matrix4x4(x,   0.0f, 0.0f, 0.0f,
                        0.0f, y,    0.0f, 0.0f,
                        0.0f, 0.0f, z,    0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
    }


    __forceinline static Matrix4x4 Translate(float x, float y, float z)
    {
        return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         x,    y,    z,    1.0f);
    }
    static Matrix4x4 Translate(const Vec3 &vec);

    __forceinline static Matrix4x4 RotationX(float angleInRad)
    {
        return Matrix4x4(1.0f,  0.0f,                  0.0f,                  0.0f,
                         0.0f,  std::cosf(angleInRad), std::sinf(angleInRad), 0.0f,
                         0.0f, -std::sinf(angleInRad), std::cosf(angleInRad), 0.0f,
                         0.0f,  0.0f,                  0.0f,                  1.0f);
    }

    __forceinline static Matrix4x4 RotationY(float angleInRad)
    {
        return Matrix4x4(std::cosf(angleInRad), 0.0f, -std::sinf(angleInRad), 0.0f,
                         0.0f,                  1.0f,  0.0f,                  0.0f,
                         std::sinf(angleInRad), 0.0f,  std::cosf(angleInRad), 0.0f,
                         0.0f,                  0.0f,  0.0f,                  1.0f);
    }

    __forceinline static Matrix4x4 RotationZ(float angleInRad)
    {
        return Matrix4x4(std::cosf(angleInRad), std::sinf(angleInRad), 0.0f, 0.0f,
                        -std::sinf(angleInRad), std::cosf(angleInRad), 0.0f, 0.0f,
                        0.0f,                   0.0f,                  1.0f, 0.0f,
                        0.0f,                   0.0f,                  0.0f, 1.0f);
    }

    // Extract bottom row to form a new Translation Matrix.
    __forceinline Matrix4x4 GetTranslation() const
    {
        return Matrix4x4(1.0f,     0.0f,     0.0f,     0.0f,
                         0.0f,     1.0f,     0.0f,     0.0f,
                         0.0f,     0.0f,     1.0f,     0.0f,
                        _m[0][3], _m[1][3], _m[2][3],  1.0f);
    }

    // Extract scales and form a new matrix.
    __forceinline Matrix4x4 GetScale() const
    {
        // Get length of each column.
        return Matrix4x4(_mm_sqrt_ss(_mm_dp_ps(_data[0], _data[0], 0b01110001)).m128_f32[0], 0.0f, 0.0f, 0.0f,
            0.0f, _mm_sqrt_ss(_mm_dp_ps(_data[1], _data[1], 0b01110001)).m128_f32[0], 0.0f, 0.0f,
            0.0f, 0.0f, _mm_sqrt_ss(_mm_dp_ps(_data[2], _data[2], 0b01110001)).m128_f32[0], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Extract rotations and form a new matrix.
    __forceinline Matrix4x4 GetRotation() const
    {
        Matrix4x4 toRet;

        __m128 temp = _mm_insert_ps(_data[0], _data[0], _CB_INSERT_MASK(0, 0, 0, 0, 0, 1));
        __m128 divisor = _mm_set1_ps(_mm_sqrt_ss(_mm_dp_ps(_data[0], _data[0], 0b01110001)).m128_f32[0]);
        temp = _mm_div_ps(temp, divisor);
        toRet._data[0] = temp;

        temp = _mm_insert_ps(_data[1], _data[1], _CB_INSERT_MASK(0, 0, 0, 0, 0, 1));
        divisor = _mm_set1_ps(_mm_sqrt_ss(_mm_dp_ps(_data[1], _data[1], 0b01110001)).m128_f32[0]);
        temp = _mm_div_ps(temp, divisor);
        toRet._data[1] = temp;

        temp = _mm_insert_ps(_data[2], _data[2], _CB_INSERT_MASK(0, 0, 0, 0, 0, 1));
        divisor = _mm_set1_ps(_mm_sqrt_ss(_mm_dp_ps(_data[2], _data[2], 0b01110001)).m128_f32[0]);
        temp = _mm_div_ps(temp, divisor);
        toRet._data[2] = temp;

        toRet._data[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

        return toRet;
    }

    // Extract bottom row to form a new position vector.
    Vec3 GetPosition() const;

    static Matrix4x4 Rotation(const Quaternion &quat);

    Matrix4x4 LocalRotation(const Quaternion &quat) const;

    Matrix4x4 LookAt( const Vec3 &worldTarget, const Vec3 &worldUp);

#pragma endregion

#pragma region Inverse 
    private:
    // This section is modified from https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html

    #define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

    // vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w])
    #define VecSwizzle(vec, x,y,z,w)           _mm_shuffle_ps(vec, vec, MakeShuffleMask(x,y,z,w))
    #define VecSwizzle1(vec, x)                _mm_shuffle_ps(vec, vec, MakeShuffleMask(x,x,x,x))
    // special swizzle
    #define VecSwizzle_0101(vec)               _mm_movelh_ps(vec, vec)
    #define VecSwizzle_2323(vec)               _mm_movehl_ps(vec, vec)
    #define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
    #define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

    // return (vec1[x], vec1[y], vec2[z], vec2[w])
    #define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
    // special shuffle
    #define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
    #define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)

        // for row major matrix
        // we use __m128 to represent 2x2 matrix as A = | A0  A1 |
        //                                              | A2  A3 |
        // 2x2 row major Matrix multiply A*B
        __forceinline __m128 Mat2Mul(__m128 vec1, __m128 vec2) const
        {
            return
                _mm_add_ps(_mm_mul_ps(vec1, VecSwizzle(vec2, 0, 3, 0, 3)),
                  _mm_mul_ps(VecSwizzle(vec1, 1, 0, 3, 2), VecSwizzle(vec2, 2, 1, 2, 1)));
        }
        // 2x2 row major Matrix adjugate multiply (A#)*B
        __forceinline __m128 Mat2AdjMul(__m128 vec1, __m128 vec2) const
        {
            return
                _mm_sub_ps(_mm_mul_ps(VecSwizzle(vec1, 3, 3, 0, 0), vec2),
                    _mm_mul_ps(VecSwizzle(vec1, 1, 1, 2, 2), VecSwizzle(vec2, 2, 3, 0, 1)));

        }
        // 2x2 row major Matrix multiply adjugate A*(B#)
        __forceinline __m128 Mat2MulAdj(__m128 vec1, __m128 vec2) const
        {
            return
                _mm_sub_ps(_mm_mul_ps(vec1, VecSwizzle(vec2, 3, 0, 3, 0)),
                    _mm_mul_ps(VecSwizzle(vec1, 1, 0, 3, 2), VecSwizzle(vec2, 2, 1, 2, 1)));
        }

    public: 
        // Returns the inversed version of this matrix. Does not modify _data.
        inline Matrix4x4 Inversed() const
        {
          // use block matrix method
          // A is a matrix, then i(A) or iA means inverse of A, A# (or A_ in code) means adjugate of A, |A| (or detA in code) is determinant, tr(A) is trace

          // sub matrices
          __m128 A = VecShuffle_0101(_data[0], _data[1]);
          __m128 B = VecShuffle_2323(_data[0], _data[1]);
          __m128 C = VecShuffle_0101(_data[2], _data[3]);
          __m128 D = VecShuffle_2323(_data[2], _data[3]);

          __m128 detA = _mm_set1_ps(_m[0][0] * _m[1][1] - _m[0][1] * _m[1][0]);
          __m128 detB = _mm_set1_ps(_m[0][2] * _m[1][3] - _m[0][3] * _m[1][2]);
          __m128 detC = _mm_set1_ps(_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]);
          __m128 detD = _mm_set1_ps(_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]);

    #if 0 // for determinant, float version is faster
          // determinant as (|A| |B| |C| |D|)
          __m128 detSub = _mm_sub_ps(
            _mm_mul_ps(VecShuffle(_data[0], _data[2], 0, 2, 0, 2), VecShuffle(_data[1], _data[3], 1, 3, 1, 3)),
            _mm_mul_ps(VecShuffle(_data[0], _data[2], 1, 3, 1, 3), VecShuffle(_data[1], _data[3], 0, 2, 0, 2))
          );
          __m128 detA = VecSwizzle1(detSub, 0);
          __m128 detB = VecSwizzle1(detSub, 1);
          __m128 detC = VecSwizzle1(detSub, 2);
          __m128 detD = VecSwizzle1(detSub, 3);
    #endif

          // let iM = 1/|M| * | X Y |
          //         | Z W |

          // D#C
          __m128 D_C = Mat2AdjMul(D, C);
          // A#B
          __m128 A_B = Mat2AdjMul(A, B);
          // X# = |D|A - B(D#C)
          __m128 X_ = _mm_sub_ps(_mm_mul_ps(detD, A), Mat2Mul(B, D_C));
          // W# = |A|D - C(A#B)
          __m128 W_ = _mm_sub_ps(_mm_mul_ps(detA, D), Mat2Mul(C, A_B));

          // |M| = |A|*|D| + ... (continue later)
          __m128 detM = _mm_mul_ps(detA, detD);

          // Y# = |B|C - D(A#B)#
          __m128 Y_ = _mm_sub_ps(_mm_mul_ps(detB, C), Mat2MulAdj(D, A_B));
          // Z# = |C|B - A(D#C)#
          __m128 Z_ = _mm_sub_ps(_mm_mul_ps(detC, B), Mat2MulAdj(A, D_C));

          // |M| = |A|*|D| + |B|*|C| ... (continue later)
          detM = _mm_add_ps(detM, _mm_mul_ps(detB, detC));

          // tr((A#B)(D#C))
          __m128 tr = _mm_mul_ps(A_B, VecSwizzle(D_C, 0, 2, 1, 3));
          tr = _mm_hadd_ps(tr, tr);
          tr = _mm_hadd_ps(tr, tr);
          // |M| = |A|*|D| + |B|*|C| - tr((A#B)(D#C)
          detM = _mm_sub_ps(detM, tr);

          const __m128 adjSignMask = _mm_setr_ps(1.f, -1.f, -1.f, 1.f);
          // (1/|M|, -1/|M|, -1/|M|, 1/|M|)
          __m128 rDetM = _mm_div_ps(adjSignMask, detM);

          X_ = _mm_mul_ps(X_, rDetM);
          Y_ = _mm_mul_ps(Y_, rDetM);
          Z_ = _mm_mul_ps(Z_, rDetM);
          W_ = _mm_mul_ps(W_, rDetM);

          Matrix4x4 r;

          // apply adjugate and store, here we combine adjugate shuffle and store shuffle
          r._data[0] = VecShuffle(X_, Y_, 3, 1, 3, 1);
          r._data[1] = VecShuffle(X_, Y_, 2, 0, 2, 0);
          r._data[2] = VecShuffle(Z_, W_, 3, 1, 3, 1);
          r._data[3] = VecShuffle(Z_, W_, 2, 0, 2, 0);
          return r;
        }




#pragma endregion

};


#endif // !_MATRIX4x4_H
