#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "../CowByteEngine/CBMemArena.h"
#include "../CowByteEngine/Matrix4x4.h"
//#include "Vec3.h"

class TestStruct
{

public:
    char data;

    ~TestStruct() {}
};

#define NUMITERATION 1000000
#define ALLOCATION_SIZE 16 //1048576
int main()
{
    std::clock_t start;
    //////////////////////////////////////////////////////////////////////////
    start = std::clock();

    void *ptr;
    for (int i = 0; i < NUMITERATION; ++i)
    {
        //ptr = new TestStruct[ALLOCATION_SIZE];
        //delete[] (ptr);
        ptr = CBMemArena::Get().Allocate(ALLOCATION_SIZE);
        CBMemArena::Get().Free(ptr, ALLOCATION_SIZE);
    }
    double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    printf("My duration: %f\n", duration);
    //////////////////////////////////////////////////////////////////////////

    start = std::clock();
    for (int i = 0; i < NUMITERATION; ++i)
    {
        ptr = malloc(ALLOCATION_SIZE);
        free(ptr);
    }
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    printf("Built-in duration: %f\n", duration);



    system("Pause");
}