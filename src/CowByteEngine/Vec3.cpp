#include "Vec3.h"



Vec3::Vec3() :
    m_X(0),
    m_Y(0),
    m_Z(0),
    m_W(0)
{
}


Vec3::Vec3(float xIn, float yIn, float zIn, float wIn) :
    m_X(xIn),
    m_Y(yIn),
    m_Z(zIn),
    m_W(wIn)
{

}

Vec3::~Vec3()
{
}
