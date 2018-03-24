#ifndef _VEC2_H
#define _VEC2_H

#include "../Utils/typedefs.h"
#include "../Memory/CBMemory.h"

struct Vec2
{
    CBMEM_OVERLOAD_NEW_DELETE(Vec2)

    FLOAT32 X;
    FLOAT32 Y;

    Vec2() :
        X(0),
        Y(0)
    {

    }

    Vec2(float inX, float inY) :
        X(inX), Y(inY)
    {

    }

    friend bool operator==(const Vec2& p1, const Vec2& p2)
    {
        return p1.X == p2.X && p1.Y == p2.Y;
    }

    friend bool operator!=(const Vec2& p1, const Vec2& p2)
    {
        return p1.X != p2.X || p1.Y != p2.Y;
    }

};

#endif
