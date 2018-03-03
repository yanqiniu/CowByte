#ifndef _CBSTRING_H
#define _CBSTRING_H
#include <stdio.h>
#include <cstring>
#include "CBStringOps.h"

using namespace CBStringOps;
// CBE custom string object, as well as some
// functions that operate on cstrings.
template <size_t len>
class CBString
{
public:


    CBString();
    CBString(const char* cstring);
    ~CBString();

    // member functions.
    void   Set(const char *inStr);
    void   Set(CBString &toAppend);
    char*  Get();
    void   Clear();
    size_t Capacity();
    size_t Size();
    int    Compare(const char *rhs);
    int    Compare(CBString &rhs);
    void   Append(const char *toAppend);
    bool   Strip(StripMode mode);

    void   operator=(const char* cstring);

    // static functions (CBString/cstring)


private:
    char m_Data[len];
    static char _s_buf[len];
};

template <size_t len>
inline int CBString<len>::Compare(CBString &rhs)
{
    return strcmp(m_Data, rhs.Get());
}

template <size_t len>
inline int CBString<len>::Compare(const char *rhs)
{
    return strcmp(m_Data, rhs);
}

template <size_t len>
inline size_t CBString<len>::Capacity()
{
    return len;
}

template <size_t len>
inline bool CBString<len>::Strip(StripMode mode)
{
    return CBStringOps::Strip(m_Data, mode);
}

template <size_t len> 
char CBString<len>::_s_buf[len];

template <size_t len>
CBString<len>::CBString()
{
}

template <size_t len>
CBString<len>::CBString(const char* cstring)
{
    Set(cstring);
}

template <size_t len>
CBString<len>::~CBString()
{
}

template <size_t len>
inline void CBString<len>::Set(const char *inStr)
{
    Clear();
    sprintf_s(m_Data, len, "%s", inStr);
}

template <size_t len>
inline void CBString<len>::Set(CBString &toAppend)
{
    Set(toAppend.Get());
}

template <size_t len>
inline char* CBString<len>::Get()
{
    return m_Data;
}

// Clear data and put null terminator in front.
template <size_t len>
inline void CBString<len>::Clear()
{
    memset(m_Data, 0, len);
    m_Data[0] = '\0';
}

template <size_t len>
inline size_t CBString<len>::Size()
{
    return strlen(m_Data);
}

template <size_t len>
inline void CBString<len>::Append(const char *toAppend)
{
    memcpy(m_Data + Size(), toAppend, strlen(toAppend) + 1);
}


template <size_t len>
inline void CBString<len>::operator=(const char* cstring)
{
    Set(cstring);
}

#endif // _COWSTRING_H

