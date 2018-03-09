#ifndef _DEBUG_H
#define _DEBUG_H
#include <stdio.h>
#include <assert.h>

#define DbgINFO(format, ...) printf("INFO: " format "\n", ##__VA_ARGS__)
#define DbgWARNING(format, ...) printf("WARNING: " format "\n", ##__VA_ARGS__)
#define DbgERROR(format, ...) printf("ERROR: " format "\n", ##__VA_ARGS__)

inline static void DbgAssert(bool expression, const char* msg)
{
    if (!expression)
    {
        DbgERROR("Assertion failed: %s", msg);
        assert(expression);
    }
}

#endif // !_DEBUG_H
