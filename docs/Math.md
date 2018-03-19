### CowByte Engine Math Overview

#Overview
 - The Math library used by the project is written using Intel SSE for SIMD.
 - Implements 3D Vector (with W), 4x4 matrix, and quaternion

#Standards:
 - Uses Row Vector;
 - Matrix4x4 has 4 m128 for each column, therefore on a linear layout it's a column major Matrix.
 - Left-Handed coordinate system, same as Direct3D, with Z+ "pointing into the screen", clockwise positive rotation for all three axes.