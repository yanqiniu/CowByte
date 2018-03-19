#ifndef _SIMD_HELPER_H
#define _SIMD_HELPER_H
// Useful __m128 stuff.

#define _CB_SHUFFLE_R(a, b, c, d) _MM_SHUFFLE(d, c, b, a)

// Example: _CB_INSERT_MASK(3, 1, 0001): insert element 3 of B to index 1 of A, and set element 3 of A to zero.
#define _CB_INSERT_MASK(indexFromB, indexToA, clearX, clearY, clearZ, clearW) (indexFromB << 6 | indexToA << 4 | 0b##clearW##clearZ##clearY##clearX) 

// Insert a float into an __m128
#define _CB_INSERT_SINGLE(m128, toInsert, insertPos) (_mm_insert_ps(m128, _mm_set_ps1(toInsert), _CB_INSERT_MASK(3, insertPos, 0, 0, 0, 0)))

#endif
