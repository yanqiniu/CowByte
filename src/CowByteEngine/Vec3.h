#ifndef _VEC3_H
#define _VEC3_H

// TODO: implement SIMD for this.
class Vec3
{
public:
    Vec3();
    Vec3(float xIn, float yIn, float zIn, float wIn);
    ~Vec3();

    float x() const { return m_X; }
    float y() const { return m_Y; }
    float z() const { return m_Z; }
    float w() const { return m_W; }

private:
    float m_X;
    float m_Y;
    float m_Z;
    float m_W;
};

#endif // 

