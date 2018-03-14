#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "../CowByteEngine 0.2/Memory/CBMemArena.h"
#include "../CowByteEngine 0.2/Math/Matrix4x4.h"

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

    Matrix4x4 m1(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, 3, 4, 1);
    Matrix4x4 m2(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 33, 13, 4, 1);

    Matrix4x4 inv = m1.Inverse();

    Matrix4x4 m3 = m1 * m2;

    //////////////////////////////////////////////////////////////////////////
    double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    printf("My duration: %f\n", duration);
    system("Pause");
}