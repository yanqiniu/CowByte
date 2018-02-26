#include <ctime>
#include "Vec3.h"

int main()
{
    std::clock_t start;
    double duration;
    Vec3 vecList[20000];
    for (int i = 0; i < 20000; ++i)
    {
        vecList[i] = Vec3(rand(), rand(), rand(), 0.0f);
    }

    start = std::clock();
    Vec3 testVec(21.43645f, 72.2351f, -13.4f);

    for (int i = 0; i < 3000; ++i)
    {
        for (int j = 0; j < 20000; ++j)
        {
            vecList[j].IsZero();
        }
    }
    /* Your algorithm here */

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    system("Pause");
}