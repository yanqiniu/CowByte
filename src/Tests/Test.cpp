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

    Vec3 axix(Vec3(3.33f, 4.44f, 6345.f));
    axix.Normalize();
    Quaternion p(1.2f, 3.34f, 6.4f, 23.0f);
    Quaternion q(3.21f, 7.34f, 11.5f, 2.1f);

    Quaternion r = p*q;

    q.Inverse();
    //////////////////////////////////////////////////////////////////////////
    double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    printf("My duration: %f\n", duration);
    system("Pause");
}