#include <ctime>
#include <stdio.h>
#include "Vec3.h"

int main()
{
    std::clock_t start;
    double duration;
    Vec3 vecList[20000];
    for (int i = 0; i < 20000; ++i)
    {
        vecList[i] = Vec3(rand()/1000, rand()/1000, rand()/1000, 0.0f);
        //vecList[i] = Vec3(0, 0, 0, 0);
    }

    start = std::clock();
    //for (int i = 0; i < 20000000; ++i)
    //{
    //    Vec3 testVec(21.43645f, 72.2351f, -13.4f);
    //    //Vec3 testVec(1.0f, 1.0f, 0.0f);
    //    testVec.Normalize();
    //}
    Vec3 vec1(1.1f, 2.1f, 3.1f);
    Vec3 vec2(4.1f, 5.1f, 6.1f);
    Vec3 vec3 = vec1.Cross(vec2);

    for (int i = 0; i < 3000; ++i)
    {
        for (int j = 0; j < 19999; ++j)
        {
            Vec3 crossVec = vecList[j].Cross(vecList[j+1]);
            //float Dot0 = crossVec.Dot(vecList[j]);
            //float Dot1 = crossVec.Dot(vecList[j + 1]);

            //if (Dot0 != 0 || Dot1 != 0)
            //{
            //    int k = 0;
            //}
            //float addup = temp._data.m128_f32[0] * temp._data.m128_f32[0] +
            //    temp._data.m128_f32[1] * temp._data.m128_f32[1] +
            //    temp._data.m128_f32[2] * temp._data.m128_f32[2];
            //addup = sqrt(addup);
            //if (std::abs(addup - 1.0f) > 0.0001f)
            //{
            //    printf("<%f, %f, %f, %f> converted to <%f, %f, %f, %f>, length is %f\n", vecList[j]._data.m128_f32[0],
            //        vecList[j]._data.m128_f32[1],
            //        vecList[j]._data.m128_f32[2],
            //        vecList[j]._data.m128_f32[3],
            //        vecList[j]._data.m128_f32[0],
            //        vecList[j]._data.m128_f32[1],
            //        vecList[j]._data.m128_f32[2],
            //        vecList[j]._data.m128_f32[3],
            //        addup);
            //}
        }
    }
    /* Your algorithm here */

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    system("Pause");
}