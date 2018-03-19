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

    Matrix4x4 mat = Matrix4x4::Identity();
    mat *= Matrix4x4::RotationX(DEGREE_TO_RAD(30.0f));
    mat *= Matrix4x4::RotationY(DEGREE_TO_RAD(30.0f));
    mat *= Matrix4x4::RotationZ(DEGREE_TO_RAD(30.0f));

    Matrix4x4 rotOnly = mat;

    Matrix4x4 scale = Matrix4x4::Scale(1.11f, 2.22f, 3.33f);

    mat *= scale;

    Matrix4x4 result = mat.GetTranslation();
    result = mat.GetScale();
    result = mat.GetRotation();

    //////////////////////////////////////////////////////////////////////////
    double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    printf("My duration: %f\n", duration);
    system("Pause");
}