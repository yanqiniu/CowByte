#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "../CowByteEngine 0.2/Memory/CBMemArena.h"
#include "../CowByteEngine 0.2/Math/CBMath.h"

class TestStruct
{

public:
    char data;

    ~TestStruct() {}
};

int main()
{
    std::clock_t start;
    start = std::clock();
    //////////////////////////////////////////////////////////////////////////
    Vec3 point(2.223f, 1.1111f, -34.5f);
    //Quaternion p(1.2f, 3.34f, 6.4f, 23.0f);
    //Quaternion q(3.21f, 7.34f, 11.5f, 2.1f);
    //q.Normalize();
    Vec3 pointRes1 = point * Matrix4x4::RotationY(1.1111f);
    Vec3 pointRes2 = point;
    Quaternion q = Quaternion(Vec3(0, 1.0f, 0), 1.1111f);
    pointRes2 *= Matrix4x4::FromQuaternion(q);

    //////////////////////////////////////////////////////////////////////////
    double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    printf("My duration: %f\n", duration);
    system("Pause");
}