#ifndef _MATRIX4x4_H
#define _MATRIX4x4_H

class Vec3;
//////////////////////////////////////////////////////////////////////////
/*

CBE SIMD fast 4x4 matrix struct.

Notice that CBE expects row vectors. 
That is, when mul with a matrix, do vec4 * matrix4x4.
Each __m128 in _data represent a column in the matrix.

*/
//////////////////////////////////////////////////////////////////////////
__declspec(align(16)) struct Matrix4x4
{
    __m128 _data[4]; // 4 columns, row vector

#pragma region Con/Destructors, Getters/Setters
    
    // Identity matrix by default.
    __forceinline Matrix4x4()
    {
        _data[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
        _data[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
        _data[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
        _data[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
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
        _data[0] = _mm_setr_ps(e[0], e[4], e[8],  e[12]);
        _data[1] = _mm_setr_ps(e[1], e[5], e[9],  e[13]);
        _data[2] = _mm_setr_ps(e[2], e[6], e[10], e[14]);
        _data[3] = _mm_setr_ps(e[3], e[7], e[11], e[15]);
    }


    __forceinline float At(size_t row, size_t col)
    {
        return _data[col].m128_f32[row];
    }
  

#pragma endregion




};


#endif // !_MATRIX4x4_H
