#include <ctime>
#include <stdio.h>
#include "Vec3.h"
#include "Timer.h"

int main()
{
    std::clock_t start;
    Vec3 vecList[20000];
    for (int i = 0; i < 20000; ++i)
    {
        vecList[i] = Vec3(rand()/1000, rand()/1000, rand()/1000, 0.0f);
        //vecList[i] = Vec3(0, 0, 0, 0);
    }

    start = std::clock();

    for (int i = 0; i < 3000; ++i)
    {
        for (int j = 0; j < 19999; ++j)
        {
            vecList[j].Add(vecList[j + 1]);
        }
    }
    /* Your algorithm here */

    double duration1 = (std::clock() - start) / (double)CLOCKS_PER_SEC;

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
            vecSlowList[j].Add(vecSlowList[j+1]);
        }
    }
    /* Your algorithm here */

    double durationSlow = (std::clock() - start) / (double)CLOCKS_PER_SEC;



    system("Pause");
}