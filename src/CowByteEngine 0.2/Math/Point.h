#ifndef _POINT_H
#define _POINT_H
#include "../Memory/CBMemory.h"

struct Point
{
    CBMEM_OVERLOAD_NEW_DELETE(Point);

        INT32 X;
    INT32 Y;

    Point() :
        X(0),
        Y(0)
    {

    }

    friend bool operator==(const Point& p1, const Point& p2)
    {
        return p1.X == p2.X && p1.Y == p2.Y;
    }

    friend bool operator!=(const Point& p1, const Point& p2)
    {
        return p1.X != p2.X || p1.Y != p2.Y;
    }

};

#endif
