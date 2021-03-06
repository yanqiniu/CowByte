#ifndef _CBMEMORY_H
#define _CBMEMORY_H
#include <assert.h>
#include "CBMemArena.h"

/*********************************************************************
!!!
Regarding delete operator overloading provided in this TU:

According to http://en.cppreference.com/w/cpp/memory/new/operator_delete:
The delete(void* ptr, size_t size) and delete[](void* ptr, size_t size)
overloading are new features provided in C++ 17. However they do have 
their restrictions that if the type of the ptr that delete operator is 
called upon is "objects of incomplete type and arrays of non-class and 
trivially-destructible class types", the version without size specified 
gets called. (Which is also overloaded - disabled in this file.)

That being said, AVOID using delete operator on native types. If absolutely
have to, you could call explicitly:

operator delete/delete[] (ptr, size)

ALSO:
For self-defined class DO PROVIDE A DESTRUCTOR DEFINITION.
This seem to notify the compiler that the class is not a 
"trivially destructible" one and actually call the overloaded
delete[]. Even an empty destructor def does the trick.

*********************************************************************/

// Use this at public section of a class.
#define CBMEM_OVERLOAD_NEW_DELETE(ClassName)               \
static void* operator new(size_t size)                      \
{                                                           \
    return CBMemArena::Get().Allocate(size);                \
}                                                           \
static void* operator new[](size_t size)                    \
{                                                           \
    return CBMemArena::Get().Allocate(size);                \
}                                                           \
static void operator delete(void* ptr, size_t size)         \
{                                                           \
    CBMemArena::Get().Free(ptr, size);                      \
    ptr = nullptr;                                          \
}                                                           \
static void operator delete[](void* ptr, size_t size)       \
{                                                           \
    CBMemArena::Get().Free(ptr, size);                      \
    ptr = nullptr;                                          \
}                                                           


#define N_OVERLOAD_GLOBAL_NEW_DELETE
#ifndef N_OVERLOAD_GLOBAL_NEW_DELETE
void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* ptr, size_t size);
void operator delete[](void* ptr, size_t size);

// Disable original ptr delete.
void operator delete(void* ptr);
void operator delete[](void* ptr);
#endif

#endif // !_CBMEMORY_H
