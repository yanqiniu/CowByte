#include <ctime>
#include <stdio.h>
#include "Vec3.h"
#include "Matrix4x4.h"

int main()
{
    std::clock_t start;

    //////////////////////////////////////////////////////////////////////////
    //float resultArray[20000];
    //Vec3 vecList[20000];
    //for (int i = 0; i < 20000; ++i)
    //{
    //    vecList[i] = Vec3(rand()/1000, rand()/1000, rand()/1000, 0.0f);
    //    //vecList[i] = Vec3(0, 0, 0, 0);
    //}

    start = std::clock();
    //float f;
    //for (int i = 0; i < 3000; ++i)
    //{
    //    for (int j = 0; j < 19999; ++j)
    //    {
    //        resultArray[j] = vecList[j].Dot(vecList[j + 1]);
    //    }
    //}

    Vec3 testVec(12.0f, 3.0f, 7.0f, 33.1f);

    float data[16] = { 1.0f, 5.0f, 9.0f, 13.0f, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16 };
    Matrix4x4 testMatrix(data);

    Vec3 newVec = testVec * testMatrix;
    
    //////////////////////////////////////////////////////////////////////////
    double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    printf("duration: %f\n", duration);

    system("Pause");
}