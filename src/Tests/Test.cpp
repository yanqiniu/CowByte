#include <ctime>
#include <stdio.h>
#include "Vec3.h"

int main()
{
    std::clock_t start;
    float resultArray[20000];
    Vec3 vecList[20000];
    for (int i = 0; i < 20000; ++i)
    {
        vecList[i] = Vec3(rand()/1000, rand()/1000, rand()/1000, 0.0f);
        //vecList[i] = Vec3(0, 0, 0, 0);
    }

    start = std::clock();
    float f;
    for (int i = 0; i < 3000; ++i)
    {
        for (int j = 0; j < 19999; ++j)
        {
            resultArray[j] = vecList[j].Dot(vecList[j + 1]);
        }
    }
    /* Your algorithm here */

    double duration1 = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    float resultArraySlow[20000];
    Vec3Slow vecSlowList[20000];
    for (int i = 0; i < 20000; ++i)
    {
        vecSlowList[i] = Vec3Slow(rand() / 1000, rand() / 1000, rand() / 1000, 0.0f);
        //vecList[i] = Vec3Slow(0, 0, 0, 0);
    }

    start = std::clock();
    
    for (int i = 0; i < 3000; ++i)
    {
        for (int j = 0; j < 19999; ++j)
        {
            resultArraySlow[j] = vecSlowList[j].Dot(vecSlowList[j+1]);
        }
    }
    /* Your algorithm here */

    double durationSlow = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    printf("duration: %f,\ndurationslow: %f\n %f", duration1, durationSlow);

    system("Pause");
}